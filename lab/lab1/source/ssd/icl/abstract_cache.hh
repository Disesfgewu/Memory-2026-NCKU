/*
 * Copyright (C) 2017 CAMELab
 */

#ifndef __ICL_CACHE__
#define __ICL_CACHE__

#include "dram/abstract_dram.hh"
#include "ftl/ftl.hh"
#include "util/simplessd.hh"

namespace SSDLab {

namespace ICL {

typedef struct _Line {
  uint64_t tag;
  uint64_t lastAccessed;
  uint64_t insertedAt;
  bool dirty;
  bool valid;

  _Line();
  _Line(uint64_t, bool);
} Line;

class AbstractCache : public StatObject {
 protected:
  ConfigReader &conf;
  FTL::FTL *pFTL;
  DRAM::AbstractDRAM *pDRAM;

 public:
  AbstractCache(ConfigReader &, FTL::FTL *, DRAM::AbstractDRAM *);
  virtual ~AbstractCache();

  virtual bool read(Request &, uint64_t &) = 0;
  virtual bool write(Request &, uint64_t &) = 0;

  virtual void flush(LPNRange &, uint64_t &) = 0;
  virtual void trim(LPNRange &, uint64_t &) = 0;
  virtual void format(LPNRange &, uint64_t &) = 0;
};

}  // namespace ICL

}  // namespace SSDLab

#endif
