/*
 * Copyright (C) 2017 CAMELab
 */

#pragma once

#ifndef __SIM_STATISTICS__
#define __SIM_STATISTICS__

#include <cinttypes>
#include <string>
#include <vector>

namespace SSDLab {

typedef struct {
  std::string name;
  std::string desc;
} Stats;

class StatObject {
 public:
  StatObject() {}
  virtual ~StatObject() {}

  virtual void getStatList(std::vector<Stats> &, std::string) {}
  virtual void getStatValues(std::vector<double> &) {}
  virtual void resetStatValues() {}
};

}  // namespace SSDLab

#endif
