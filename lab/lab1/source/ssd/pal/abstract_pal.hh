/*
 * Copyright (C) 2017 CAMELab
 */

#ifndef __PAL_ABSTRACT_PAL__
#define __PAL_ABSTRACT_PAL__

#include <cinttypes>

#include "pal/pal.hh"

namespace SSDLab {

namespace PAL {

class AbstractPAL : public StatObject {
 protected:
  Parameter &param;
  ConfigReader &conf;

 public:
  AbstractPAL(Parameter &p, ConfigReader &c) : param(p), conf(c) {}
  virtual ~AbstractPAL() {}

  virtual void read(Request &, uint64_t &) = 0;
  virtual void write(Request &, uint64_t &) = 0;
  virtual void erase(Request &, uint64_t &) = 0;
};

}  // namespace PAL

}  // namespace SSDLab

#endif
