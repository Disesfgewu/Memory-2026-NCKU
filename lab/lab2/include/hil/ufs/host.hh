//Copyright (C) 2017 CAMELab

#ifndef __HIL_UFS_HOST__
#define __HIL_UFS_HOST__

#include <cinttypes>
#include <queue>
#include <vector>

#include "hil/ufs/def.hh"
#include "hil/ufs/interface.hh"
#include "util/simplessd.hh"

namespace SSDLab {

namespace HIL {

namespace UFS {

class Device;

typedef struct _Completion {
  bool interruptEnable;
  uint32_t bitmask;
  uint64_t finishedAt;

  _Completion() : interruptEnable(false), bitmask(0), finishedAt(0) {}

  _Completion(uint64_t t, uint32_t b, bool i)
      : interruptEnable(i), bitmask(b), finishedAt(t) {}

  bool operator()(const _Completion &a, const _Completion &b) {
    return a.finishedAt > b.finishedAt;
  }
} Completion;

class Host : public StatObject {
 private:
  Interface *pInterface;
  Device *pDevice;

  ConfigReader &conf;

  DMAInterface *axiFIFO;
  DMAInterface *mphyFIFO;
  DMAInterface *deviceFIFO;

  Event workEvent;
  Event requestEvent;
  Event completionEvent;
  uint64_t workInterval;
  uint64_t requestInterval;
  uint64_t maxRequest;
  uint64_t requestCounter;
  uint64_t lastWorkAt;

  UFSHCIRegister register_table;
  uint32_t pendingInterrupt;

  std::queue<uint32_t> lRequestQueue;
  std::priority_queue<Completion, std::vector<Completion>, Completion>
      lResponseQueue;

  struct {
    uint64_t uicCommand;
    uint64_t utpCommand;
  } stat;

  void processUIC();
  void processUTPTask();
  void processUTPTransfer(uint64_t);

  void processUTPCommand(UTPTransferReqDesc &, UTP_TRANSFER_CMD, DMAFunction &,
                         void * = nullptr);

  UPIU *getUPIU(UPIU_OPCODE);

 public:
  Host(Interface *, ConfigReader &);
  ~Host();

  // For Interface
  void readRegister(uint32_t, uint32_t &);
  void writeRegister(uint32_t, uint32_t, uint64_t);

  void completion();
  void handleRequest();
  void work();

  UFSHCIRegister &getRegisterTable();

  void getStatList(std::vector<Stats> &, std::string) override;
  void getStatValues(std::vector<double> &) override;
  void resetStatValues() override;
};

}  // namespace UFS

}  // namespace HIL

}  // namespace SSDLab

#endif
