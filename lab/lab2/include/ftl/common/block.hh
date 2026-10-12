//Copyright (C) 2017 CAMELab

#ifndef __FTL_COMMON_BLOCK__
#define __FTL_COMMON_BLOCK__

#include <cinttypes>
#include <iosfwd>
#include <vector>

#include "util/bitset.hh"

namespace SSDLab {

namespace FTL {

class Block {
 private:
  uint32_t idx;
  uint32_t pageCount;
  uint32_t ioUnitInPage;
  uint32_t *pNextWritePageIndex;

  // Following variables are used when ioUnitInPage == 1
  Bitset *pValidBits;
  Bitset *pErasedBits;
  uint64_t *pLPNs;

  // Following variables are used when ioUnitInPage > 1
  std::vector<Bitset> validBits;
  std::vector<Bitset> erasedBits;
  uint64_t **ppLPNs;

  uint64_t lastAccessed;
  uint32_t eraseCount;

 public:
  Block(uint32_t, uint32_t, uint32_t);
  Block(const Block &);      // Copy constructor
  Block(Block &&) noexcept;  // Move constructor
  ~Block();

  Block &operator=(const Block &);  // Copy assignment
  Block &operator=(Block &&);       // Move assignment

  uint32_t getBlockIndex() const;
  uint64_t getLastAccessedTime();
  uint32_t getEraseCount();
  uint32_t getValidPageCount();
  uint32_t getValidPageCountRaw();
  uint32_t getDirtyPageCount();
  uint32_t getNextWritePageIndex();
  uint32_t getNextWritePageIndex(uint32_t);
  bool getPageInfo(uint32_t, std::vector<uint64_t> &, Bitset &);
  bool read(uint32_t, uint32_t, uint64_t);
  bool write(uint32_t, uint64_t, uint32_t, uint64_t);
  void erase();
  void invalidate(uint32_t, uint32_t);

  void writeSnapshot(std::ostream &);
};

}  // namespace FTL

}  // namespace SSDLab

#endif
