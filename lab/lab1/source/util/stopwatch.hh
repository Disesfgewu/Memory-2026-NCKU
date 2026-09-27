/*
 * Copyright (C) 2017 CAMELab
 */

#ifndef __UTIL_STOPWATCH__
#define __UTIL_STOPWATCH__

#include <chrono>

class Stopwatch {
 private:
  std::chrono::high_resolution_clock::time_point begin;
  std::chrono::high_resolution_clock::time_point end;

 public:
  Stopwatch();
  ~Stopwatch();

  // Get timepoint
  double getTime();

  // Stopwatch
  void start();
  void stop();
  double getDuration();
};

#endif
