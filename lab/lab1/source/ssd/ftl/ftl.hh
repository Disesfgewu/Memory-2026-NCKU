/*
 * Copyright (C) 2017 CAMELab
 */

#ifndef __FTL_FTL__
#define __FTL_FTL__

#include "dram/abstract_dram.hh"
#include "pal/pal.hh"
#include "util/simplessd.hh"

namespace SSDLab {

namespace FTL {

class AbstractFTL;

typedef struct {
  uint64_t totalPhysicalBlocks;  //!< (PAL::Parameter::superBlock)
  uint64_t totalLogicalBlocks;
  uint64_t pagesInBlock;  //!< (PAL::Parameter::page)
  uint32_t pageSize;      //!< Mapping unit (PAL::Parameter::superPageSize)
  uint32_t ioUnitInPage;  //!< # smallest I/O unit in one page
  uint32_t pageCountToMaxPerf;  //!< # pages to fully utilize internal parallism
} Parameter;

class FTL : public StatObject {
 private:
  Parameter param;
  PAL::PAL *pPAL;

  ConfigReader &conf;
  AbstractFTL *pFTL;
  DRAM::AbstractDRAM *pDRAM;

 public:
  FTL(ConfigReader &, DRAM::AbstractDRAM *);
  ~FTL();

  void read(Request &, uint64_t &);
  void write(Request &, uint64_t &);
  void trim(Request &, uint64_t &);

  void format(LPNRange &, uint64_t &);

  Parameter *getInfo();
  uint64_t getUsedPageCount(uint64_t, uint64_t);

  void getStatList(std::vector<Stats> &, std::string) override;
  void getStatValues(std::vector<double> &) override;
  void resetStatValues() override;
};

}  // namespace FTL

}  // namespace SSDLab

#endif
