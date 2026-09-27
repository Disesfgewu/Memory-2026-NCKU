/*
 * Copyright (C) 2017 CAMELab
 */

#pragma once

#ifndef __SIM_ENGINE__
#define __SIM_ENGINE__

#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <unordered_map>

#include "ssd/sim/simulator.hh"
#include "util/stopwatch.hh"

class Engine : public SSDLab::Simulator {
 private:
  std::mutex mTick;
  uint64_t simTick;
  SSDLab::Event counter;
  bool forceStop;
  std::unordered_map<SSDLab::Event, SSDLab::EventFunction> eventList;
  std::list<std::pair<SSDLab::Event, uint64_t>> eventQueue;

  Stopwatch watch;

  std::mutex m;
  uint64_t eventHandled;

  bool insertEvent(SSDLab::Event, uint64_t, uint64_t * = nullptr);
  bool removeEvent(SSDLab::Event);
  bool isEventExist(SSDLab::Event, uint64_t * = nullptr);

 public:
  Engine();
  ~Engine();

  uint64_t getCurrentTick() override;

  SSDLab::Event allocateEvent(SSDLab::EventFunction) override;
  void scheduleEvent(SSDLab::Event, uint64_t) override;
  void descheduleEvent(SSDLab::Event) override;
  bool isScheduled(SSDLab::Event, uint64_t * = nullptr) override;
  void deallocateEvent(SSDLab::Event) override;

  bool doNextEvent();
  void stopEngine();
  void printStats(std::ostream &);
  void getStat(uint64_t &);
};

#endif
