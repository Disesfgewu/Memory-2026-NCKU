/*
 * Copyright (C) 2017 CAMELab
 */

#pragma once

#ifndef __CPU_CONFIG__
#define __CPU_CONFIG__

#include "sim/base_config.hh"

namespace SSDLab {

namespace CPU {

typedef enum {
  CPU_CLOCK,
  CPU_CORE_HIL,
  CPU_CORE_ICL,
  CPU_CORE_FTL,
} CPU_CONFIG;

class Config : public BaseConfig {
 private:
  uint64_t clock;    //!< Default: 400MHz
  uint32_t hilCore;  //!< Default: 1
  uint32_t iclCore;  //!< Default: 1
  uint32_t ftlCore;  //!< Default: 1

 public:
  Config();

  bool setConfig(const char *, const char *) override;
  void update() override;

  uint64_t readUint(uint32_t) override;
};

}  // namespace CPU

}  // namespace SSDLab

#endif
