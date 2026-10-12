#!/usr/bin/env python3
"""Link student GC with the bundled lab2.a, run both versions, and compare."""

import argparse
import hashlib
import itertools
import os
import platform
import subprocess
import sys
import tempfile
from pathlib import Path


GC_SYMBOL = "SSDLab::FTL::Lab2GC::doGarbageCollection"


def first_difference(reference, student):
    missing = object()
    checkpoint = "before first checkpoint"
    for line_number, (left, right) in enumerate(
        itertools.zip_longest(reference, student, fillvalue=missing), 1
    ):
        if left is not missing and left.startswith("CHECKPOINT "):
            checkpoint = left.rstrip("\n")
        if left != right:
            return (
                line_number,
                checkpoint,
                "<EOF>" if left is missing else left.rstrip("\n"),
                "<EOF>" if right is missing else right.rstrip("\n"),
            )
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


def has_complete_snapshot(path):
    first_line = ""
    last_line = ""
    with path.open(encoding="utf-8") as source:
        for line_number, line in enumerate(source):
            if line_number == 0:
                first_line = line
            last_line = line
    return first_line == "LAB2_SNAPSHOT_V1\n" and last_line == "END_SNAPSHOT\n"


def verify_gc_exercised(directory):
    contents = []
    for name in ("simulator.log", "debug.log"):
        path = directory / name
        if path.is_file():
            contents.append(path.read_text(encoding="utf-8", errors="replace"))
    if "GC   | Done" not in "\n".join(contents):
        raise RuntimeError(
            f"No completed GC operation found in logs under {directory}"
        )


def run_one(executable, sim_config, ssd_content, directory):
    directory.mkdir()
    snapshot = directory / "ftl.snapshot"
    generated_config = directory / "ssd.cfg"
    generated_config.write_text(ssd_content, encoding="utf-8")

    environment = os.environ.copy()
    environment["LAB2_SNAPSHOT_PATH"] = str(snapshot)
    with (directory / "simulator.log").open("w", encoding="utf-8") as log:
        result = subprocess.run(
            [
                str(executable),
                str(sim_config),
                str(generated_config),
                str(directory),
            ],
            stdout=log,
            stderr=subprocess.STDOUT,
            env=environment,
            check=False,
        )
    if result.returncode != 0:
        raise RuntimeError(
            f"{directory.name} simulation failed ({result.returncode}); "
            f"see {directory / 'simulator.log'}"
        )
    if not snapshot.exists() or not has_complete_snapshot(snapshot):
        raise RuntimeError(f"Incomplete FTL snapshot: {snapshot}")
    verify_gc_exercised(directory)
    return snapshot


def verify_student_symbol(build_directory):
    candidates = list(
        (build_directory / "CMakeFiles" / "lab2_student_gc.dir").rglob(
            "lab2_gc_student.cc.o"
        )
    )
    if len(candidates) != 1:
        raise RuntimeError("Cannot locate the compiled student GC object")
    result = subprocess.run(
        ["nm", "-C", "--defined-only", str(candidates[0])],
        text=True,
        stdout=subprocess.PIPE,
        check=True,
    )
    if GC_SYMBOL not in result.stdout:
        raise RuntimeError(
            "lab2_gc_student.cc does not define Lab2GC::doGarbageCollection"
        )


def build_executables(root, jobs):
    if not (root / "lab2.a").is_file():
        raise RuntimeError(f"Missing precompiled archive: {root / 'lab2.a'}")
    build_identity = "\0".join(
        (str(root), platform.node(), platform.machine())
    )
    path_key = hashlib.sha256(build_identity.encode("utf-8")).hexdigest()[:12]
    build_directory = root / ".build" / path_key
    subprocess.run(
        ["cmake", "-S", str(root), "-B", str(build_directory)], check=True
    )
    subprocess.run(
        [
            "cmake",
            "--build",
            str(build_directory),
            "--target",
            "lab2_reference",
            "lab2_student",
            "--parallel",
            str(jobs),
        ],
        check=True,
    )
    verify_student_symbol(build_directory)

    reference = build_directory / "lab2_reference"
    student = build_directory / "lab2_student"
    if not reference.is_file() or not student.is_file():
        raise RuntimeError("Built comparison executable is missing")
    return reference, student


def main():
    root = Path(__file__).resolve().parent
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--simulation-config",
        type=Path,
        default=root / "config" / "sim.cfg",
    )
    parser.add_argument(
        "--ssd-config",
        type=Path,
        default=root / "config" / "ssd.cfg",
    )
    parser.add_argument(
        "--output-directory", type=Path, default=root / "results"
    )
    parser.add_argument(
        "--jobs",
        type=int,
        default=min(os.cpu_count() or 1, 8),
        help="Maximum parallel compilation jobs",
    )
    parser.add_argument(
        "--self-test",
        action="store_true",
        help="Check comparison detects a changed line",
    )
    args = parser.parse_args()

    if args.self_test:
        sample = ["CHECKPOINT MAPPING_TABLE\n", "MAPPING 2 1:0\n"]
        changed = [sample[0], "MAPPING 2 1:1\n"]
        assert first_difference(iter(sample), iter(sample)) is None
        difference = first_difference(iter(sample), iter(changed))
        assert difference[:2] == (2, "CHECKPOINT MAPPING_TABLE")
        assert first_difference(iter(sample), iter(sample[:1]))[2:] == (
            "MAPPING 2 1:0",
            "<EOF>",
        )
        print("Comparator self-test: PASS")
        return 0

    if args.jobs < 1:
        parser.error("--jobs must be at least 1")

    simulation_config = args.simulation_config.resolve(strict=True)
    content = args.ssd_config.resolve(strict=True).read_text(encoding="utf-8")
    validate_ssd_config(content)
    reference_executable, student_executable = build_executables(
        root, args.jobs
    )

    args.output_directory.mkdir(parents=True, exist_ok=True)
    output = Path(
        tempfile.mkdtemp(
            prefix="lab2-compare-", dir=args.output_directory.resolve()
        )
    )
    print(f"Output: {output}", flush=True)

    reference = run_one(
        reference_executable,
        simulation_config,
        content,
        output / "reference",
    )
    student = run_one(
        student_executable,
        simulation_config,
        content,
        output / "student",
    )

    matched = compare_files(reference, student, "FTL snapshot")
    (output / "result.txt").write_text(
        "match\n" if matched else "fail\n", encoding="utf-8"
    )
    return 0 if matched else 1


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (
        OSError,
        ValueError,
        RuntimeError,
        subprocess.CalledProcessError,
    ) as exc:
        print(f"Comparison failed: {exc}", file=sys.stderr)
        sys.exit(2)
