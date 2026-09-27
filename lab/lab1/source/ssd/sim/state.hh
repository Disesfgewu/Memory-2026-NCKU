/*
 * Copyright (C) 2017 CAMELab
 */

#pragma once

#ifndef __SIM_STATE__
#define __SIM_STATE__

#include <cinttypes>
#include <vector>

namespace SSDLab {

class StateObject {
 protected:
  template <class T>
  static void pushValue(std::vector<uint8_t> &, T);
  template <class T>
  static void popValue(std::vector<uint8_t> &, T);

 public:
  StateObject() {}
  virtual ~StateObject() {}

  virtual void saveState(std::vector<uint8_t> &) {}
  virtual void loadState(std::vector<uint8_t> &) {}
};

}  // namespace SSDLab

#endif
