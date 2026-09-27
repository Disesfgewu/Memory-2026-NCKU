/*
 * Copyright (C) 2017 CAMELab
 */

#pragma once

#ifndef __SIM_CPU__
#define __SIM_CPU__

#include <cinttypes>
#include <functional>

#include "cpu/cpu.hh"

namespace SSDLab {

typedef struct _CPUContext {
  DMAFunction func;
  void *context;

  CPU::NAMESPACE ns;
  CPU::FUNCTION fct;
  uint64_t delay;

  _CPUContext(DMAFunction &, void *);
  _CPUContext(DMAFunction &, void *, CPU::NAMESPACE, CPU::FUNCTION);
  _CPUContext(DMAFunction &, void *, CPU::NAMESPACE, CPU::FUNCTION, uint64_t);
} CPUContext;

void initCPU(ConfigReader &);
void deInitCPU();
void getCPUStatList(std::vector<Stats> &, std::string);
void getCPUStatValues(std::vector<double> &);
void resetCPUStatValues();
void printCPULastStat();

void execute(CPU::NAMESPACE, CPU::FUNCTION, DMAFunction &, void * = nullptr,
             uint64_t = 0);
uint64_t applyLatency(CPU::NAMESPACE, CPU::FUNCTION);

void commonCPUHandler(uint64_t, void *);

// Defined at sim/cpu.cc
extern DMAFunction cpuHandler;

}  // namespace SSDLab

#endif
