/*
 * Copyright (C) 2017 CAMELab
 */

#ifndef __FTL_LAB1_READ_WRITE__
#define __FTL_LAB1_READ_WRITE__

#include <cinttypes>

#include "util/def.hh"

namespace SSDLab {

namespace FTL {

class PageMapping;

class Lab1ReadWrite {
 public:
  static void readInternal(PageMapping &, Request &, uint64_t &);
  static void writeInternal(PageMapping &, Request &, uint64_t &);
};

}  // namespace FTL

}  // namespace SSDLab

#endif
