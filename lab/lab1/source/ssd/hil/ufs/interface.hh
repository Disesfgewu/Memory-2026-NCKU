/*
 * Copyright (C) 2017 CAMELab
 */

#ifndef __HIL_UFS_INTERFACE__
#define __HIL_UFS_INTERFACE__

#include <cinttypes>

#include "sim/dma_interface.hh"

namespace SSDLab {

namespace HIL {

namespace UFS {

class Host;

class Interface : public SSDLab::DMAInterface {
 protected:
  Host *pController;

 public:
  virtual void generateInterrupt() = 0;
  virtual void clearInterrupt() = 0;
};

}  // namespace UFS

}  // namespace HIL

}  // namespace SSDLab

#endif
