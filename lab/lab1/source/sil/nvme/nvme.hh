/*
 * Copyright (C) 2017 CAMELab
 */

#pragma once

#ifndef __DRIVERS_NVME__
#define __DRIVERS_NVME__

#include <list>
#include <queue>

#include "bil/interface.hh"
#include "sil/nvme/prp.hh"
#include "sil/nvme/queue.hh"
#include "ssd/hil/nvme/interface.hh"
#include "ssd/util/interface.hh"

#define QUEUE_ENTRY_ADMIN 256
#define QUEUE_ENTRY_IO 1024

namespace SIL {

namespace NVMe {

// Copied from SimpleSSD-FullSystem
typedef struct _DMAEntry {
  uint64_t beginAt;
  uint64_t finishedAt;
  uint64_t addr;
  uint64_t size;
  uint8_t *buffer;
  void *context;
  SSDLab::DMAFunction func;

  _DMAEntry(SSDLab::DMAFunction &f)
      : beginAt(0),
        finishedAt(0),
        addr(0),
        size(0),
        buffer(nullptr),
        context(nullptr),
        func(f) {}
} DMAEntry;

typedef std::function<void(uint16_t, uint32_t, void *)> ResponseHandler;

typedef struct _CommandEntry {
  uint16_t iv;  // Same as Queue ID
  uint16_t opcode;
  uint16_t cid;
  void *context;

  ResponseHandler callback;

  _CommandEntry(uint16_t i, uint16_t o, uint16_t c, void *p, ResponseHandler &f)
      : iv(i), opcode(o), cid(c), context(p), callback(f) {}
} CommandEntry;

typedef struct _IOWrapper {
  uint64_t id;
  PRP *prp;
  std::function<void(uint64_t)> bioCallback;

  _IOWrapper(uint64_t i, PRP *p, std::function<void(uint64_t)> &f)
      : id(i), prp(p), bioCallback(f) {}
} IOWrapper;

class Driver : public BIL::DriverInterface, SSDLab::HIL::NVMe::Interface {
 private:
  // PCI Express (for DMA throttling)
  SSDLab::PCIExpress::PCIE_GEN pcieGen;
  uint8_t pcieLane;

  // DMA scheduling
  SSDLab::Event dmaReadEvent;
  SSDLab::Event dmaWriteEvent;
  std::queue<DMAEntry> dmaReadQueue;
  std::queue<DMAEntry> dmaWriteQueue;
  bool dmaReadPending;
  bool dmaWritePending;

  // NVMe Identify
  uint64_t capacity;
  uint32_t LBAsize;
  uint32_t namespaceID;

  // Queue
  uint16_t maxQueueEntries;
  uint16_t adminCommandID;
  uint16_t ioCommandID;
  bool phase;
  Queue *adminSQ;
  Queue *adminCQ;
  Queue *ioSQ;
  Queue *ioCQ;
  std::list<CommandEntry> pendingCommandList;

  void dmaReadDone();
  void submitDMARead();
  void dmaWriteDone();
  void submitDMAWrite();

  void increaseCommandID(uint16_t &);

  void _init0(uint16_t, void *);
  void _init1(uint16_t, void *);
  void _init2(uint16_t, void *);
  void _init3(uint16_t, uint32_t, void *);
  void _init4(uint16_t, void *);
  void _init5(uint16_t, void *);

  void _io(uint16_t, void *);

  void submitCommand(uint16_t, uint8_t *, ResponseHandler &, void *);

 public:
  Driver(Engine &, SSDLab::ConfigReader &);
  ~Driver();

  // BIL::DriverInterface
  void init(std::function<void()> &) override;
  void getInfo(uint64_t &, uint32_t &) override;
  void submitIO(BIL::BIO &) override;

  void initStats(std::vector<SSDLab::Stats> &) override;
  void getStats(std::vector<double> &) override;

  // SSDLab::DMAInterface
  void dmaRead(uint64_t, uint64_t, uint8_t *, SSDLab::DMAFunction &,
               void * = nullptr) override;
  void dmaWrite(uint64_t, uint64_t, uint8_t *, SSDLab::DMAFunction &,
                void * = nullptr) override;

  // SSDLab::HIL::NVMe::Interface
  void updateInterrupt(uint16_t, bool) override;
  void getVendorID(uint16_t &, uint16_t &) override;
};

}  // namespace NVMe

}  // namespace SIL

#endif
