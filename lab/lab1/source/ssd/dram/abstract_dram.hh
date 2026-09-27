/*
 * Copyright (C) 2017 CAMELab
 */

#ifndef __DRAM_ABSTRACT_DRAM__
#define __DRAM_ABSTRACT_DRAM__

#include <cinttypes>

#include "libdrampower/LibDRAMPower.h"
#include "util/simplessd.hh"

namespace SSDLab {

namespace DRAM {

typedef enum : uint8_t {
  ACTIVE,                //!< Row active
  IDLE,                  //!< Precharged
  POWER_DOWN_PRECHARGE,  //!< Precharge power down
  POWER_DOWN_ACTIVE,     //!< Active power down
  SELF_REFRESH,          //!< Self refresh
} DRAMState;

class AbstractDRAM : public StatObject {
 protected:
  ConfigReader &conf;

  Config::DRAMStructure *pStructure;
  Config::DRAMTiming *pTiming;
  Config::DRAMPower *pPower;

  Data::MemorySpecification spec;
  libDRAMPower *dramPower;

  void convertMemspec();

  double totalEnergy;  // Unit: pJ
  double totalPower;   // Unit: mW

 public:
  AbstractDRAM(ConfigReader &);
  virtual ~AbstractDRAM();

  virtual void read(void *, uint64_t, uint64_t &) = 0;
  virtual void write(void *, uint64_t, uint64_t &) = 0;

  // TEMP: Should be removed on v2.2
  virtual void setScheduling(bool) {}
  virtual bool isScheduling() { return true; }

  void getStatList(std::vector<Stats> &, std::string) override;
  void getStatValues(std::vector<double> &) override;
  void resetStatValues() override;
};

}  // namespace DRAM

}  // namespace SSDLab

#endif
