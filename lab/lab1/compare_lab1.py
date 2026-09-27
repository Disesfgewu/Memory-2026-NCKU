#!/usr/bin/env python3
"""Link student code with the bundled lab1.a, run both FTLs, and compare."""

import argparse
import hashlib
import itertools
import os
import platform
import subprocess
import sys
import tempfile
from pathlib import Path


def first_difference(reference, student):
    missing = object()
    checkpoint = "before first checkpoint"
    for line_number, (left, right) in enumerate(
        itertools.zip_longest(reference, student, fillvalue=missing), 1
    ):
        if left is not missing and left.startswith("CHECKPOINT "):
            checkpoint = left.rstrip("\n")
        if left != right:
            return (line_number, checkpoint,
                    "<EOF>" if left is missing else left.rstrip("\n"),
                    "<EOF>" if right is missing else right.rstrip("\n"))
    return None


def compare_files(reference, student, label):
    with reference.open(encoding="utf-8") as left, student.open(
        encoding="utf-8"
    ) as right:
        difference = first_difference(left, right)

    if difference is None:
        print(f"{label}: MATCH")
        return True

    line_number, checkpoint, left, right = difference
    print(f"{label}: DIFFER at line {line_number} ({checkpoint})")
    print(f"  reference: {left}")
    print(f"  student:   {right}")
    return False


def validate_ssd_config(content):
    section = ""
    for raw in content.splitlines():
        line = raw.split("#", 1)[0].strip()
        if line.startswith("[") and line.endswith("]"):
            section = line[1:-1].lower()
            continue
        if "=" not in line:
            continue
        key, value = (part.strip().lower() for part in line.split("=", 1))
        if key == "enablediskimage" and value in ("1", "true", "yes"):
            raise ValueError("Disable disk images for independent comparison runs")
        if section == "ftl" and key == "fillingmode" and value != "0":
            raise ValueError("Use deterministic FTL FillingMode = 0")
        if section == "ftl" and key == "evictpolicy" and value in ("2", "3"):
            raise ValueError("Use deterministic FTL EvictPolicy = 0 or 1")


def make_config(content, implementation, snapshot, each_request):
    if "\n" in str(snapshot) or "\r" in str(snapshot):
        raise ValueError("Snapshot path cannot contain a newline")
    # A later [ftl] section overrides earlier values without changing the input.
    return (content.rstrip() + "\n\n[ftl]\n"
            + f"Lab1Implementation = {implementation}\n"
            + f"Lab1SnapshotFile = {snapshot}\n"
            + f"Lab1SnapshotEachRequest = {int(each_request)}\n")


def has_complete_snapshot(path):
    seen_final = False
    last_line = ""
    with path.open(encoding="utf-8") as source:
        for line in source:
            if line.startswith("CHECKPOINT ") and " FINAL tick " in line:
                seen_final = True
            last_line = line
    return seen_final and last_line == "END_CHECKPOINT\n"


def run_one(executable, sim_config, ssd_content, directory, implementation,
            each_request):
    directory.mkdir()
    snapshot = directory / "ftl.snapshot"
    generated_config = directory / "ssd.cfg"
    generated_config.write_text(
        make_config(ssd_content, implementation, snapshot, each_request),
        encoding="utf-8",
    )
    with (directory / "simulator.log").open("w", encoding="utf-8") as log:
        result = subprocess.run(
            [str(executable), str(sim_config), str(generated_config),
             str(directory)],
            stdout=log,
            stderr=subprocess.STDOUT,
            check=False,
        )
    if result.returncode != 0:
        raise RuntimeError(
            f"{directory.name} simulation failed ({result.returncode}); "
            f"see {directory / 'simulator.log'}"
        )
    if not snapshot.exists() or not has_complete_snapshot(snapshot):
        raise RuntimeError(f"Incomplete FTL snapshot: {snapshot}")
    if not (directory / "ftl.snapshot.pal").exists():
        raise RuntimeError(f"Missing PAL trace: {snapshot}.pal")
    return snapshot


def build_executable(root, jobs):
    if not (root / "lab1.a").is_file():
        raise RuntimeError(f"Missing precompiled archive: {root / 'lab1.a'}")
    # A moved bundle or a different host gets a new build tree instead of a
    # copied CMake cache containing old paths or machine-specific objects.
    build_identity = "\0".join((str(root), platform.node(),
                                platform.machine()))
    path_key = hashlib.sha256(build_identity.encode("utf-8")).hexdigest()[:12]
    build_directory = root / ".build" / path_key
    subprocess.run(["cmake", "-S", str(root), "-B", str(build_directory)],
                   check=True)
    subprocess.run(["cmake", "--build", str(build_directory),
                    "--target", "lab1_compare", "--parallel", str(jobs)],
                   check=True)
    executable = build_directory / "lab1_compare"
    if not executable.is_file():
        raise RuntimeError(f"Built executable is missing: {executable}")
    return executable


def main():
    root = Path(__file__).resolve().parent
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--simulation-config", type=Path,
                        default=root / "config" / "sim.cfg")
    parser.add_argument("--ssd-config", type=Path,
                        default=root / "config" / "ssd.cfg")
    parser.add_argument("--output-directory", type=Path,
                        default=root / "results")
    parser.add_argument("--jobs", type=int,
                        default=min(os.cpu_count() or 1, 8),
                        help="Maximum parallel compilation jobs")
    parser.add_argument("--each-request", action="store_true",
                        help="Save a full FTL snapshot after every request")
    parser.add_argument("--self-test", action="store_true",
                        help="Check comparison detects a changed line")
    args = parser.parse_args()

    if args.self_test:
        sample = ["CHECKPOINT 0 INIT tick 0\n", "LPN 2 units 1 1:0\n"]
        changed = [sample[0], "LPN 2 units 1 1:1\n"]
        assert first_difference(iter(sample), iter(sample)) is None
        difference = first_difference(iter(sample), iter(changed))
        assert difference[:2] == (2, "CHECKPOINT 0 INIT tick 0")
        assert first_difference(iter(sample), iter(sample[:1]))[2:] == (
            "LPN 2 units 1 1:0", "<EOF>")
        print("Comparator self-test: PASS")
        return 0

    if args.jobs < 1:
        parser.error("--jobs must be at least 1")

    simulation_config = args.simulation_config.resolve(strict=True)
    content = args.ssd_config.resolve(strict=True).read_text(encoding="utf-8")
    validate_ssd_config(content)
    executable = build_executable(root, args.jobs)

    args.output_directory.mkdir(parents=True, exist_ok=True)
    root = Path(tempfile.mkdtemp(prefix="lab1-compare-",
                                 dir=args.output_directory.resolve()))
    print(f"Output: {root}", flush=True)

    reference = run_one(executable, simulation_config, content,
                        root / "reference", 0, args.each_request)
    student = run_one(executable, simulation_config, content,
                      root / "student", 1, args.each_request)

    state_match = compare_files(reference, student, "FTL snapshot")
    reference_pal = Path(str(reference) + ".pal")
    student_pal = Path(str(student) + ".pal")
    pal_match = compare_files(reference_pal, student_pal, "PAL trace")
    if reference_pal.stat().st_size == 0:
        print("Warning: no PAL operations occurred; this workload may not "
              "exercise mapped reads or writes")
    return 0 if state_match and pal_match else 1


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (OSError, ValueError, RuntimeError,
            subprocess.CalledProcessError) as exc:
        print(f"Comparison failed: {exc}", file=sys.stderr)
        sys.exit(2)
