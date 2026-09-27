/*
 * Copyright (C) 2017 CAMELab
 */

#pragma once

#ifndef __SIM_TRACE__
#define __SIM_TRACE__

#include <cinttypes>

namespace SSDLab {

typedef enum {
  LOG_COMMON,
  LOG_CPU,
  LOG_HIL,
  LOG_HIL_NVME,
  LOG_HIL_SATA,
  LOG_HIL_UFS,
  LOG_ICL,
  LOG_ICL_GENERIC_CACHE,
  LOG_FTL,
  LOG_FTL_PAGE_MAPPING,
  LOG_PAL,
  LOG_PAL_OLD,
  LOG_NUM
} LOG_ID;

void debugprint(LOG_ID, const char *, ...);
void debugprint(LOG_ID, const uint8_t *, uint64_t);

void panic(const char *, ...);
void warn(const char *, ...);
void info(const char *, ...);

}  // namespace SSDLab

#endif
