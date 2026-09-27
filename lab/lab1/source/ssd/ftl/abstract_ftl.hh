/*
 * Copyright (C) 2017 CAMELab
 */

#ifndef __FTL_ABSTRACT_FTL__
#define __FTL_ABSTRACT_FTL__

#include <cinttypes>

#include "ftl/ftl.hh"

namespace SSDLab {

namespace FTL {

typedef struct _Status {
  uint64_t totalLogicalPages;
  uint64_t mappedLogicalPages;
  uint64_t freePhysicalBlocks;
} Status;

class AbstractFTL : public StatObject {
 protected:
  Parameter &param;
  PAL::PAL *pPAL;
  DRAM::AbstractDRAM *pDRAM;
  Status status;

 public:
  AbstractFTL(Parameter &p, PAL::PAL *l, DRAM::AbstractDRAM *d)
      : param(p), pPAL(l), pDRAM(d) {}
  virtual ~AbstractFTL() {}

  virtual bool initialize() = 0;

  virtual void read(Request &, uint64_t &) = 0;
  virtual void write(Request &, uint64_t &) = 0;
  virtual void trim(Request &, uint64_t &) = 0;

  virtual void format(LPNRange &, uint64_t &) = 0;

  virtual Status *getStatus(uint64_t, uint64_t) = 0;
};

}  // namespace FTL

}  // namespace SSDLab

#endif
