//Copyright (C) 2017 CAMELab

#ifndef __HIL_NVME_SUBSYSTEM__
#define __HIL_NVME_SUBSYSTEM__

#include "hil/hil.hh"
#include "hil/nvme/abstract_subsystem.hh"

namespace SSDLab {

namespace HIL {

namespace NVMe {

class Subsystem : public AbstractSubsystem {
 protected:
  HIL *pHIL;

  std::list<Namespace *> lNamespaces;
  uint32_t queueAllocated;

  HealthInfo globalHealth;
  uint32_t logicalPageSize;
  uint64_t totalLogicalPages;
  uint64_t allocatedLogicalPages;

  // Stats
  uint64_t commandCount;

  void convertUnit(Namespace *, uint64_t, uint64_t, Request &);
  bool createNamespace(uint32_t, Namespace::Information *);
  bool destroyNamespace(uint32_t);
  void fillIdentifyNamespace(uint8_t *, Namespace::Information *);

  // Admin commands
  bool deleteSQueue(SQEntryWrapper &, RequestFunction &);
  bool createSQueue(SQEntryWrapper &, RequestFunction &);
  bool getLogPage(SQEntryWrapper &, RequestFunction &);
  bool deleteCQueue(SQEntryWrapper &, RequestFunction &);
  bool createCQueue(SQEntryWrapper &, RequestFunction &);
  bool identify(SQEntryWrapper &, RequestFunction &);
  bool abort(SQEntryWrapper &, RequestFunction &);
  bool setFeatures(SQEntryWrapper &, RequestFunction &);
  bool getFeatures(SQEntryWrapper &, RequestFunction &);
  bool asyncEventReq(SQEntryWrapper &, RequestFunction &);
  bool namespaceManagement(SQEntryWrapper &, RequestFunction &);
  bool namespaceAttachment(SQEntryWrapper &, RequestFunction &);
  bool formatNVM(SQEntryWrapper &, RequestFunction &);

 public:
  Subsystem(Controller *, ConfigData &);
  ~Subsystem();

  void init() override;

  void submitCommand(SQEntryWrapper &, RequestFunction) override;
  void getNVMCapacity(uint64_t &, uint64_t &) override;
  uint32_t validNamespaceCount() override;

  void read(Namespace *, uint64_t, uint64_t, DMAFunction &, void *);
  void write(Namespace *, uint64_t, uint64_t, DMAFunction &, void *);
  void flush(Namespace *, DMAFunction &, void *);
  void trim(Namespace *, uint64_t, uint64_t, DMAFunction &, void *);

  void getStatList(std::vector<Stats> &, std::string) override;
  void getStatValues(std::vector<double> &) override;
  void resetStatValues() override;
};

}  // namespace NVMe

}  // namespace HIL

}  // namespace SSDLab

#endif
