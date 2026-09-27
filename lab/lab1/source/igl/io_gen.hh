/*
 * Copyright (C) 2017 CAMELab
 */

#pragma once

#ifndef __IGL_IO_GEN__
#define __IGL_IO_GEN__

#include "bil/entry.hh"
#include "sim/cfg_reader.hh"
#include "sim/engine.hh"

namespace IGL {

class IOGenerator {
 protected:
  Engine &engine;
  BIL::BlockIOEntry &bioEntry;
  std::function<void()> &endCallback;

 public:
  IOGenerator(Engine &e, BIL::BlockIOEntry &b, std::function<void()> &c)
      : engine(e), bioEntry(b), endCallback(c) {}
  virtual ~IOGenerator() {}

  virtual void init(uint64_t, uint32_t) = 0;
  virtual void begin() = 0;
  virtual void printStats(std::ostream &) = 0;
  virtual void getProgress(float &) = 0;
};

}  // namespace IGL

#endif
