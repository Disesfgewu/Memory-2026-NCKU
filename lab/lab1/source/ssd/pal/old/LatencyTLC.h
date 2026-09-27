/*
 * Copyright (C) 2017 CAMELab
 */

#ifndef __LatencyTLC_h__
#define __LatencyTLC_h__

#include "Latency.h"

class LatencyTLC : public Latency {
 public:
  LatencyTLC(SSDLab::PAL::Config::NANDTiming,
             SSDLab::PAL::Config::NANDPower);
  ~LatencyTLC();

  uint64_t GetLatency(uint32_t, uint8_t, uint8_t) override;
  inline uint8_t GetPageType(uint32_t) override;
};

#endif  //__LatencyTLC_h__
