//Copyright (C) 2017 CAMELab

#pragma once

#ifndef __HIL_NVME_CONFIG__
#define __HIL_NVME_CONFIG__

#include <unordered_map>

#include "sim/base_config.hh"
#include "util/interface.hh"

namespace SSDLab {

namespace HIL {

namespace NVMe {

typedef enum {
  NVME_PCIE_GEN,
  NVME_PCIE_LANE,
  NVME_AXI_BUS_WIDTH,
  NVME_AXI_CLOCK,
  NVME_FIFO_UNIT,
  NVME_WORK_INTERVAL,
  NVME_MAX_REQUEST_COUNT,
  NVME_MAX_IO_CQUEUE,
  NVME_MAX_IO_SQUEUE,
  NVME_WRR_HIGH,
  NVME_WRR_MEDIUM,
  NVME_ENABLE_DEFAULT_NAMESPACE,
  NVME_LBA_SIZE,
  NVME_ENABLE_DISK_IMAGE,
  NVME_STRICT_DISK_SIZE,
  NVME_DISK_IMAGE_PATH,
  NVME_USE_COW_DISK
} NVME_CONFIG;

class Config : public BaseConfig {
 private:
  PCIExpress::PCIE_GEN pcieGen;  //!< Default: PCIE_3_X
  uint8_t pcieLane;              //!< Default: 4
  ARM::AXI::BUS_WIDTH axiWidth;  //!< Default: BUS_128BIT
  uint64_t axiClock;             //!< Default: 250000000 (250MHz)
  uint64_t fifoUnit;             //!< Default: 4096
  uint64_t workInterval;         //!< Default: 50000 (50ns)
  uint64_t maxRequestCount;      //!< Default: 4
  uint16_t maxIOCQueue;          //!< Default: 16
  uint16_t maxIOSQueue;          //!< Default: 16
  uint16_t wrrHigh;              //!< Default: 2
  uint16_t wrrMedium;            //!< Default: 2
  uint64_t lbaSize;              //!< Default: 512
  uint16_t defaultNamespace;     //!< Default: 1
  bool enableDiskImage;          //!< Default: False
  bool strictDiskSize;           //!< Default: False
  bool useCopyOnWriteDisk;       //!< Default: False
  std::unordered_map<uint16_t, std::string> diskImagePaths;  //!< Default: ""

 public:
  Config();

  bool setConfig(const char *, const char *) override;
  void update() override;

  int64_t readInt(uint32_t) override;
  uint64_t readUint(uint32_t) override;
  std::string readString(uint32_t) override;
  bool readBoolean(uint32_t) override;
};

}  // namespace NVMe

}  // namespace HIL

}  // namespace SSDLab

#endif
