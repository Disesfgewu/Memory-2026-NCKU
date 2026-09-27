/*
 * Copyright (C) 2017 CAMELab
 */

#pragma once

#ifndef __UTIL_CONVERT__
#define __UTIL_CONVERT__

#include <cinttypes>

#ifdef _MSC_VER
#define strcasecmp _stricmp
#endif

bool convertBoolean(const char *);
uint64_t convertInteger(const char *, bool * = nullptr);
uint64_t convertTime(const char *, bool * = nullptr);

#endif
