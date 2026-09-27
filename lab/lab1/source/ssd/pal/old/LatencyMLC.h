/*
 * Copyright (C) 2017 CAMELab
 */

#ifndef __LatencyMLC_h__
#define __LatencyMLC_h__

#include "Latency.h"

class LatencyMLC : public Latency {
 public:
  LatencyMLC(SSDLab::PAL::Config::NANDTiming,
             SSDLab::PAL::Config::NANDPower);
  ~LatencyMLC();

  uint64_t GetLatency(uint32_t, uint8_t, uint8_t) override;
  inline uint8_t GetPageType(uint32_t) override;
};

#endif  //__LatencyMLC_h__
