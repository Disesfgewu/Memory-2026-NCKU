//Copyright (C) 2017 CAMELab

#pragma once

#ifndef __SIM_BASE_CONFIG__
#define __SIM_BASE_CONFIG__

#include <cinttypes>
#include <cstring>
#include <string>

namespace SSDLab {

#define MATCH_SECTION(str) (strcmp(section, str) == 0)
#define MATCH_NAME(str) (strcmp(name, str) == 0)
#define MATCH_VALUE(str) (strcmp(value, str) == 0)

class BaseConfig {
 protected:
  bool convertBool(const char *);

 public:
  BaseConfig() {}
  virtual ~BaseConfig() {}

  virtual bool setConfig(const char *, const char *) = 0;
  virtual void update() {}

  virtual int64_t readInt(uint32_t) { return 0; }
  virtual uint64_t readUint(uint32_t) { return 0; }
  virtual float readFloat(uint32_t) { return 0.f; }
  virtual std::string readString(uint32_t) { return std::string(); }
  virtual bool readBoolean(uint32_t) { return false; }
};

}  // namespace SSDLab

#endif
