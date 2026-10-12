//Copyright (C) 2017 CAMELab

#pragma once

#ifndef __SIM_CONFIG_READER__
#define __SIM_CONFIG_READER__

#include <cinttypes>
#include <string>

#include "cpu/config.hh"
#include "dram/config.hh"
#include "ftl/config.hh"
#include "hil/nvme/config.hh"
#include "hil/sata/config.hh"
#include "hil/ufs/config.hh"
#include "icl/config.hh"
#include "lib/inih/ini.h"
#include "pal/config.hh"

namespace SSDLab {

typedef enum {
  CONFIG_CPU,
  CONFIG_DRAM,
  CONFIG_FTL,
  CONFIG_NVME,
  CONFIG_SATA,
  CONFIG_UFS,
  CONFIG_ICL,
  CONFIG_PAL,
} CONFIG_SECTION;

class ConfigReader {
 private:
  CPU::Config cpuConfig;
  DRAM::Config dramConfig;
  FTL::Config ftlConfig;
  HIL::NVMe::Config nvmeConfig;
  HIL::SATA::Config sataConfig;
  HIL::UFS::Config ufsConfig;
  ICL::Config iclConfig;
  PAL::Config palConfig;

  static int parserHandler(void *, const char *, const char *, const char *);

 public:
  bool init(std::string);

  int64_t readInt(CONFIG_SECTION, uint32_t);
  uint64_t readUint(CONFIG_SECTION, uint32_t);
  float readFloat(CONFIG_SECTION, uint32_t);
  std::string readString(CONFIG_SECTION, uint32_t);
  bool readBoolean(CONFIG_SECTION, uint32_t);

  // DRAM::Config
  DRAM::Config::DRAMStructure *getDRAMStructure();
  DRAM::Config::DRAMTiming *getDRAMTiming();
  DRAM::Config::DRAMPower *getDRAMPower();

  // PAL::Config
  uint8_t getSuperblockConfig();
  uint32_t getPageAllocationConfig();

  PAL::Config::NANDTiming *getNANDTiming();
  PAL::Config::NANDPower *getNANDPower();
};

}  // namespace SSDLab

#endif
