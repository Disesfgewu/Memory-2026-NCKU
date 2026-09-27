/*
 * Copyright (C) 2017 CAMELab
 */

#ifndef __LatencySLC_h__
#define __LatencySLC_h__

#include "Latency.h"

class LatencySLC : public Latency {
 public:
  LatencySLC(SSDLab::PAL::Config::NANDTiming,
             SSDLab::PAL::Config::NANDPower);
  ~LatencySLC();

  uint64_t GetLatency(uint32_t, uint8_t, uint8_t) override;
  inline uint8_t GetPageType(uint32_t) override;
};

#endif  //__LatencyTLC_h__
