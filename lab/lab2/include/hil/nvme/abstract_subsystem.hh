//Copyright (C) 2017 CAMELab

#ifndef __HIL_NVME_ABSTRACT_SUBSYSTEM__
#define __HIL_NVME_ABSTRACT_SUBSYSTEM__

#include "hil/nvme/namespace.hh"
#include "util/simplessd.hh"

namespace SSDLab {

namespace HIL {

namespace NVMe {

class Controller;

class AbstractSubsystem : public StatObject {
 protected:
  Controller *pParent;

  ConfigData &cfgdata;
  ConfigReader &conf;

 public:
  AbstractSubsystem(Controller *, ConfigData &);
  virtual ~AbstractSubsystem();

  virtual void init() = 0;
  virtual void submitCommand(SQEntryWrapper &, RequestFunction) = 0;
  virtual void getNVMCapacity(uint64_t &, uint64_t &) = 0;
  virtual uint32_t validNamespaceCount() = 0;
};

}  // namespace NVMe

}  // namespace HIL

}  // namespace SSDLab

#endif
