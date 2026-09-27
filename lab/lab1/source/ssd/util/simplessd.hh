/*
 * Copyright (C) 2017 CAMELab
 */

#pragma once

#ifndef __UTIL_SIMPLESSD__
#define __UTIL_SIMPLESSD__

#include "sim/config_reader.hh"
#include "sim/cpu.hh"
#include "sim/simulator.hh"
#include "sim/statistics.hh"
#include "sim/trace.hh"

SSDLab::ConfigReader initSimpleSSDEngine(SSDLab::Simulator *,
                                            std::ostream *, std::ostream *,
                                            std::string);
void releaseSimpleSSDEngine();

#endif
