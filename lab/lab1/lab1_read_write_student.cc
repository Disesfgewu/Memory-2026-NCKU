#include "lab1_read_write_student.hh"

#include <algorithm>
#include <unordered_map>
#include <vector>

#include "ftl/page_mapping.hh"
#include "util/algorithm.hh"

namespace SSDLab {

namespace FTL {

void Lab1ReadWriteStudent::readInternal(PageMapping &ftl, Request &req,
                                        uint64_t &tick) {
  PAL::Request palRequest(req);
  uint64_t beginAt;
  uint64_t finishedAt = tick;

  auto mappingList = ftl.table.find(req.lpn);

  // An unmapped LPN does not issue a NAND read.
  if (mappingList == ftl.table.end()) {
    return;
  }

  // Mapping-table access latency
  ftl.pDRAM->read(&(*mappingList), 8, tick);

  // TODO: Read Implementation
  for (uint32_t idx = 0; idx < ftl.bitsetSize; idx++) {
    // Check if the page is requested to be read
    if (req.ioFlag.test(idx)) {
      // Get the physical page mapping
      auto &mapping = mappingList->second[idx];

      // Check if the mapping is valid
      if (mapping.first < ftl.param.totalPhysicalBlocks && mapping.second < ftl.param.pagesInBlock) {
        // Set index informations in palRequest for the NAND read operation
        palRequest.blockIndex = mapping.first;
        palRequest.pageIndex = mapping.second;

        if (ftl.bRandomTweak) {
          palRequest.ioFlag.reset();
          palRequest.ioFlag.set(idx);
        }

        auto block = ftl.blocks.find(palRequest.blockIndex);
        if (block == ftl.blocks.end()) {
          // This should not happen, but if it does, we skip the read operation for this page.
          continue;
        }

        // Update ticks
        beginAt = tick;

        // Issue PAL read
        block->second.read(palRequest.pageIndex, idx, beginAt);
        ftl.pPAL->read(palRequest, beginAt);

        finishedAt = MAX(finishedAt, beginAt);
      }
    }
  }
  tick = finishedAt;
  tick += applyLatency(CPU::FTL__PAGE_MAPPING, CPU::READ_INTERNAL);
}


void Lab1ReadWriteStudent::writeInternal(PageMapping &ftl, Request &req,
                                         uint64_t &tick) {
  PAL::Request palRequest(req);
  uint64_t beginAt;
  uint64_t finishedAt = tick;

  auto mappingList = ftl.table.find(req.lpn);

  // TODO: Write Implementation - Write Path
  std::unordered_map<uint32_t, Block>:: iterator block;
  
  // Invalid Old Physical Page
  if (mappingList != ftl.table.end()) {
    for (uint32_t idx = 0; idx < ftl.bitsetSize; idx++) {
      // Same as above, check if the page is requested to be written
      if (req.ioFlag.test(idx)) {
        auto &mapping = mappingList->second[idx];

        if (mapping.first < ftl.param.totalPhysicalBlocks && mapping.second < ftl.param.pagesInBlock) {
          block = ftl.blocks.find(mapping.first);
          block->second.invalidate(mapping.second, idx);
        }
      }
    }
  }
  else {
    // if the mappingList is not found, we need to create a new entry in the table for this LPN
    auto ret = ftl.table.emplace(
      req.lpn, std::vector<std::pair<uint32_t, uint32_t>>( ftl.bitsetSize, {
        ftl.param.totalPhysicalBlocks, ftl.param.pagesInBlock
      })
    );
    mappingList = ret.first;
  }

  // Get the next free block for writing
  block = ftl.blocks.find(ftl.getLastFreeBlock(req.ioFlag));

  if (block == ftl.blocks.end()) {
    // This should not happen, but if it does, we skip the write operation.
    return;
  }

  /*
   * Mapping-table access latency.
   * This timing model is provided and should not be modified.
   */
  ftl.pDRAM->read(&(*mappingList), 8, tick);
  ftl.pDRAM->write(&(*mappingList), 8, tick);

  // TODO: Write Implementation - Physical Write

  for (uint32_t idx = 0; idx < ftl.bitsetSize; idx++) {
    // Check if the page is requested to be written same as above TODO
    if (req.ioFlag.test(idx)) {
      
      // Allocate a new physical page for writing
      uint32_t pageIndex = block->second.getNextWritePageIndex(idx);
      auto &mapping = mappingList->second[idx];

      beginAt = tick;
      block->second.write(pageIndex, req.lpn, idx, beginAt);

      // Update LPN Mapping
      mapping.first = block->first;
      mapping.second = pageIndex;

      // Issue PAL write
      palRequest.blockIndex = block->first;
      palRequest.pageIndex = pageIndex;
      palRequest.ioFlag.reset();
      palRequest.ioFlag.set(idx);

      ftl.pPAL->write(palRequest, beginAt);
      finishedAt = MAX(finishedAt, beginAt);
    }
  }

  tick = finishedAt;
  tick += applyLatency(CPU::FTL__PAGE_MAPPING, CPU::WRITE_INTERNAL);

  // The GC-related code should not be modified.
  static float gcThreshold =
      ftl.conf.readFloat(CONFIG_FTL, FTL_GC_THRESHOLD_RATIO);

  if (ftl.freeBlockRatio() < gcThreshold) {
    ftl.handleOnDemandGC(tick);
  }
}

}  // namespace FTL

}  // namespace SSDLab
