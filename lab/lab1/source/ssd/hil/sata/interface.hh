/*
 * Copyright (C) 2017 CAMELab
 */

#pragma once

#ifndef __HIL_SATA_INTERFACE__
#define __HIL_SATA_INTERFACE__

#include <cinttypes>

#include "sim/dma_interface.hh"

namespace SSDLab {

namespace HIL {

namespace SATA {

class HBA;

class Interface : public SSDLab::DMAInterface {
 protected:
  HBA *pController;

 public:
  virtual void updateInterrupt(bool) = 0;
};

}  // namespace SATA

}  // namespace HIL

}  // namespace SSDLab

#endif
