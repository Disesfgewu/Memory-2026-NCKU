/*
 * Copyright (C) 2017 CAMELab
 */

#ifndef __PAL2_TimeSlot_h__
#define __PAL2_TimeSlot_h__

#include "util/old/SimpleSSD_types.h"

#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

struct TimeSlot {
  uint64_t StartTick;
  uint64_t EndTick;

  TimeSlot(uint64_t startTick, uint64_t duration);
  TimeSlot() : StartTick(0ull), EndTick(0ull){};
};

#endif
