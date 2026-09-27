/*
 * Copyright (C) 2017 CAMELab
 */

#pragma once

#ifndef __DRIVERS_NONE__
#define __DRIVERS_NONE__

#include "bil/interface.hh"
#include "ssd/hil/hil.hh"

namespace SIL {

namespace None {

class Driver : public BIL::DriverInterface {
 private:
  SSDLab::HIL::HIL *pHIL;

  uint64_t totalLogicalPages;
  uint32_t logicalPageSize;

 public:
  Driver(Engine &, SSDLab::ConfigReader &);
  ~Driver();

  void init(std::function<void()> &) override;
  void getInfo(uint64_t &, uint32_t &) override;
  void submitIO(BIL::BIO &) override;

  void initStats(std::vector<SSDLab::Stats> &) override;
  void getStats(std::vector<double> &) override;
};

}  // namespace None

}  // namespace SIL

#endif
