/*
 * Copyright (C) 2017 CAMELab
 */

#pragma once

#ifndef __HIL_NVME_INTERFACE__
#define __HIL_NVME_INTERFACE__

#include <cinttypes>

#include "sim/dma_interface.hh"

namespace SSDLab {

namespace HIL {

namespace NVMe {

class Controller;

class Interface : public SSDLab::DMAInterface {
 protected:
  Controller *pController;

 public:
  virtual void updateInterrupt(uint16_t, bool) = 0;
  virtual void getVendorID(uint16_t &, uint16_t &) = 0;
};

}  // namespace NVMe

}  // namespace HIL

}  // namespace SSDLab

#endif
