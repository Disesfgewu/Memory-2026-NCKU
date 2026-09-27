/*
 * Copyright (C) 2017 CAMELab
 */

#ifndef __Latency_h__
#define __Latency_h__

#include "util/old/SimpleSSD_types.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#include "pal/config.hh"

/*==============================
    Latency
==============================*/
class Latency {
 protected:
  SSDLab::PAL::Config::NANDTiming timing;
  SSDLab::PAL::Config::NANDPower power;

 public:
  Latency(SSDLab::PAL::Config::NANDTiming,
          SSDLab::PAL::Config::NANDPower);
  virtual ~Latency();

  // Get Latency for PageAddress(L/C/MSBpage), Operation(RWE),
  // BusyFor(Ch.DMA/Mem.Work)
  virtual uint64_t GetLatency(uint32_t, uint8_t, uint8_t) { return 0; };
  virtual inline uint8_t GetPageType(uint32_t) { return PAGE_NUM; };

  // Setup DMA speed and pagesize
  virtual uint64_t GetPower(uint8_t, uint8_t);
};

#endif  //__Latency_h__
