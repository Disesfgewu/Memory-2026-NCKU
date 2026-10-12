#include "ftl/lab2_gc.hh"

#include "ftl/page_mapping.hh"
#include "util/algorithm.hh"
#include "util/bitset.hh"

namespace SSDLab {

namespace FTL {

namespace Lab2GC {

void doGarbageCollection(PageMapping &ftl, std::vector<uint32_t> &blocksToReclaim, uint64_t &tick) {
  PAL::Request req(1);
  std::vector<PAL::Request> readRequests;
  std::vector<PAL::Request> writeRequests;
  std::vector<PAL::Request> eraseRequests;
  uint64_t beginAt;
  uint64_t readFinishedAt = tick;
  uint64_t writeFinishedAt = tick;
  uint64_t eraseFinishedAt = tick;

  for (auto &iter : blocksToReclaim) {
    // TODO: Find victim block in ftl.blocks
    auto victimIt = ftl.blocks.find(iter);
    Block &victimBlock = victimIt->second;

    for (uint32_t pageIndex = 0; pageIndex < ftl.param.pagesInBlock; pageIndex++) {
      // TODO: GC Implementation 
      std::vector<uint64_t> lpns;
      Bitset validBits(1);
      if (!victimBlock.getPageInfo(pageIndex, lpns, validBits)) {
        continue;
      }

      uint64_t lpn = lpns[0];
      Bitset ioFlag(1);
      ioFlag.set(0, true);

      uint32_t newBlockIndex = ftl.getLastFreeBlock(ioFlag);

      auto newFreeBlockIt = ftl.blocks.find(newBlockIndex);
      Block &newFreeBlock = newFreeBlockIt->second;

      uint32_t newPageIndex = newFreeBlock.getNextWritePageIndex(0);

      auto mappingIt = ftl.table.find(lpn);
      mappingIt->second[0].first = newBlockIndex;
      mappingIt->second[0].second = newPageIndex;

      victimBlock.read(pageIndex, 0, tick);
      victimBlock.invalidate(pageIndex, 0);
      newFreeBlock.write(newPageIndex, lpn, 0, tick);

      // Generate read request (from victim)
      req.blockIndex = iter;
      req.pageIndex = pageIndex;
      req.ioFlag = ioFlag;
      readRequests.push_back(req);

      // Generate write request (to new free block)
      req.blockIndex = newBlockIndex;
      req.pageIndex = newPageIndex;
      req.ioFlag = ioFlag;
      writeRequests.push_back(req);
    }
    // TODO: Create an erase request and add it to eraseRequests
    req.blockIndex = iter;
    req.pageIndex = 0;
    req.ioFlag.set(0, true);
    eraseRequests.push_back(req);
  }

  // Process all requests. No modification is required below.
  for (auto &iter : readRequests) {
    beginAt = tick;

    ftl.pPAL->read(iter, beginAt);

    readFinishedAt = MAX(readFinishedAt, beginAt);
  }

  for (auto &iter : writeRequests) {
    beginAt = readFinishedAt;

    ftl.pPAL->write(iter, beginAt);

    writeFinishedAt = MAX(writeFinishedAt, beginAt);
  }

  for (auto &iter : eraseRequests) {
    beginAt = readFinishedAt;

    ftl.eraseInternal(iter, beginAt);

    eraseFinishedAt = MAX(eraseFinishedAt, beginAt);
  }

  tick = MAX(writeFinishedAt, eraseFinishedAt);
  tick += applyLatency(CPU::FTL__PAGE_MAPPING, CPU::DO_GARBAGE_COLLECTION);
}

}  // namespace Lab2GC

}  // namespace FTL

}  // namespace SSDLab
