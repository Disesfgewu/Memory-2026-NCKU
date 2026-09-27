/*
 * Copyright (C) 2017 CAMELab
 */

#pragma once

#ifndef __HIL_NVME_CONTROLLER__
#define __HIL_NVME_CONTROLLER__

#include <list>
#include <unordered_map>

#include "hil/nvme/abstract_subsystem.hh"
#include "hil/nvme/def.hh"
#include "hil/nvme/dma.hh"
#include "hil/nvme/queue.hh"
#include "util/bitset.hh"
#include "util/def.hh"
#include "util/simplessd.hh"

namespace SSDLab {

namespace HIL {

namespace NVMe {

class Interface;

typedef union _RegisterTable {
  uint8_t data[64];
  struct {
    uint64_t capabilities;
    uint32_t version;
    uint32_t interruptMaskSet;
    uint32_t interruptMaskClear;
    uint32_t configuration;
    uint32_t reserved;
    uint32_t status;
    uint32_t subsystemReset;
    uint32_t adminQueueAttributes;
    uint64_t adminSQueueBaseAddress;
    uint64_t adminCQueueBaseAddress;
    uint32_t memoryBufferLocation;
    uint32_t memoryBufferSize;
  };

  _RegisterTable();
} RegisterTable;

typedef struct {
  uint64_t nextTime;
  uint32_t requestCount;
  bool valid;
  bool pending;
} AggregationInfo;

class Controller : public StatObject {
 private:
  Interface *pParent;             //!< NVMe::Interface passed from constructor
  AbstractSubsystem *pSubsystem;  //!< NVMe::Subsystem allocate in constructor

  bool bUseOCSSD;

  SSDLab::DMAInterface *pcieFIFO;
  SSDLab::DMAInterface *interconnect;

  RegisterTable registers;   //!< Table for NVMe Controller Registers
  uint64_t sqstride;         //!< Calculated SQ Stride
  uint64_t cqstride;         //!< Calculated CQ stride
  uint8_t adminQueueInited;  //!< Flag for initialization of Admin CQ/SQ
  uint16_t arbitration;      //!< Selected Arbitration Mechanism
  uint32_t interruptMask;    //!< Variable to store current interrupt mask

  uint32_t cqsize;
  uint32_t sqsize;
  CQueue **ppCQueue;  //!< Completion Queue array
  SQueue **ppSQueue;  //!< Submission Queue array

  std::list<SQEntryWrapper> lSQFIFO;  //!< Internal FIFO queue for submission
  std::list<CQEntryWrapper> lCQFIFO;  //!< Internal FIFO queue for completion

  bool shutdownReserved;

  uint64_t aggregationTime;
  uint32_t aggregationThreshold;
  std::unordered_map<uint16_t, AggregationInfo> aggregationMap;

  ConfigData cfgdata;
  ConfigReader &conf;

  Event workEvent;
  Event requestEvent;
  Event completionEvent;
  uint32_t requestCounter;
  uint32_t maxRequest;
  uint64_t requestInterval;
  uint64_t workInterval;
  uint64_t lastWorkAt;

  bool checkQueue(SQueue *, DMAFunction &, void *);

 public:
  Controller(Interface *, ConfigReader &);
  ~Controller();

  void readRegister(uint64_t, uint64_t, uint8_t *, uint64_t &);
  void writeRegister(uint64_t, uint64_t, uint8_t *, uint64_t &);
  void ringCQHeadDoorbell(uint16_t, uint16_t, uint64_t &);
  void ringSQTailDoorbell(uint16_t, uint16_t, uint64_t &);

  void clearInterrupt(uint16_t);
  void updateInterrupt(uint16_t, bool);

  int createCQueue(uint16_t, uint16_t, uint16_t, bool, bool, uint64_t,
                   DMAFunction &, void *);
  int createSQueue(uint16_t, uint16_t, uint16_t, uint8_t, bool, uint64_t,
                   DMAFunction &, void *);
  int deleteCQueue(uint16_t);
  int deleteSQueue(uint16_t);
  int abort(uint16_t, uint16_t);
  void identify(uint8_t *);
  void setCoalescingParameter(uint8_t, uint8_t);
  void getCoalescingParameter(uint8_t *, uint8_t *);
  void setCoalescing(uint16_t, bool);
  bool getCoalescing(uint16_t);

  void collectSQueue(DMAFunction &, void *);
  void handleRequest(uint64_t);
  void work();

  void submit(CQEntryWrapper &);
  void reserveCompletion();
  void completion();

  void getStatList(std::vector<Stats> &, std::string) override;
  void getStatValues(std::vector<double> &) override;
  void resetStatValues() override;
};

}  // namespace NVMe

}  // namespace HIL

}  // namespace SSDLab

#endif
