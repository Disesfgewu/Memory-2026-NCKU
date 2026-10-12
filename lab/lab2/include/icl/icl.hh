//Copyright (C) 2017 CAMELab

#ifndef __ICL_ICL__
#define __ICL_ICL__

#include "dram/abstract_dram.hh"
#include "ftl/ftl.hh"
#include "icl/abstract_cache.hh"
#include "util/simplessd.hh"

namespace SSDLab {

namespace ICL {

class ICL : public StatObject {
 private:
  FTL::FTL *pFTL;
  DRAM::AbstractDRAM *pDRAM;

  ConfigReader &conf;
  AbstractCache *pCache;

  uint64_t totalLogicalPages;
  uint32_t logicalPageSize;

 public:
  ICL(ConfigReader &);
  ~ICL();

  void read(Request &, uint64_t &);
  void write(Request &, uint64_t &);

  void flush(LPNRange &, uint64_t &);
  void trim(LPNRange &, uint64_t &);
  void format(LPNRange &, uint64_t &);

  void getLPNInfo(uint64_t &, uint32_t &);
  uint64_t getUsedPageCount(uint64_t, uint64_t);

  void getStatList(std::vector<Stats> &, std::string) override;
  void getStatValues(std::vector<double> &) override;
  void resetStatValues() override;
};

}  // namespace ICL

}  // namespace SSDLab

#endif
