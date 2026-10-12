//Copyright (C) 2017 CAMELab

#ifndef __HIL_SATA_HBA__
#define __HIL_SATA_HBA__

#include <queue>

#include "hil/sata/def.hh"
#include "hil/sata/interface.hh"
#include "sim/config_reader.hh"
#include "sim/simulator.hh"
#include "sim/statistics.hh"

namespace SSDLab {

namespace HIL {

namespace SATA {

class Device;

class Completion {
 public:
  uint32_t slotIndex;  // PxCI bit index
  uint32_t maskIS;     // PxIS bit mask

  // Received FIS
  FIS fis;

  // Function handler when completion is done
  // This only for NCQ operations (READ/WRITE FPDMA QUEUED)
  DMAFunction func;
  void *context;

  Completion();
  Completion(DMAFunction &, void *);
};

struct RequestContext {
  uint32_t idx;
  CommandHeader header;
};

typedef std::function<void(Completion &)> RequestFunction;

class HBA : public StatObject {
 private:
  Interface *pInterface;
  Device *pDevice;
  ConfigReader &conf;

  DMAInterface *pHostDMA;
  DMAInterface *pPHY;
  DMAInterface *pDeviceDMA;

  AHCIGHCRegister ghc;
  AHCIPortRegister port;

  // Request handling
  Event workEvent;
  Event requestEvent;
  uint64_t workInterval;
  uint64_t requestInterval;
  uint64_t maxRequest;
  uint64_t requestCounter;
  uint64_t lastWorkAt;

  bool submitFISPending;
  std::queue<uint32_t> lRequestQueue;
  std::queue<Completion> lResponseQueue;

  // Port state
  bool deviceInited;

  void init();
  void updateInterrupt();

  void processCommand(uint32_t);
  void work();
  void handleRequest();
  void handleResponse();
  void interruptCleared();

 public:
  HBA(Interface *, ConfigReader &);
  ~HBA();

  // For Device
  void submitFIS(Completion &);
  void submitSignal(Completion &);

  // For Interface
  void readAHCIRegister(uint32_t, uint32_t, uint8_t *);
  void writeAHCIRegister(uint32_t, uint32_t, uint8_t *);
};

}  // namespace SATA

}  // namespace HIL

}  // namespace SSDLab

#endif
