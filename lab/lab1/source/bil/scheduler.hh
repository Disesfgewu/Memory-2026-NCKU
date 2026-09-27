/*
 * Copyright (C) 2017 CAMELab
 */

#pragma once

#ifndef __BIL_SCHEDULER__
#define __BIL_SCHEDULER__

#include "bil/interface.hh"

namespace BIL {

class Scheduler {
 protected:
  Engine &engine;
  DriverInterface *pInterface;

 public:
  Scheduler(Engine &e, DriverInterface *i) : engine(e), pInterface(i) {}
  virtual ~Scheduler() {}

  virtual void init() = 0;
  virtual void submitIO(BIO &) = 0;
};

}  // namespace BIL

#endif
