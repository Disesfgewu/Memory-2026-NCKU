/*
 * Copyright (C) 2017 CAMELab
 */

#ifndef __DRAM_SIMPLE__
#define __DRAM_SIMPLE__

#include <list>

#include "dram/abstract_dram.hh"

namespace SSDLab {

namespace DRAM {

class SimpleDRAM : public AbstractDRAM {
 private:
  struct Stat {
    uint64_t count;
    uint64_t size;

    Stat();
  };

  uint64_t pageFetchLatency;
  double interfaceBandwidth;

  uint64_t lastDRAMAccess;
  bool ignoreScheduling;

  Event autoRefresh;

  Stat readStat;
  Stat writeStat;

  uint64_t updateDelay(uint64_t, uint64_t &);
  void updateStats(uint64_t);

 public:
  SimpleDRAM(ConfigReader &p);
  ~SimpleDRAM();

  void read(void *, uint64_t, uint64_t &) override;
  void write(void *, uint64_t, uint64_t &) override;

  void setScheduling(bool) override;
  bool isScheduling() override;

  void getStatList(std::vector<Stats> &, std::string) override;
  void getStatValues(std::vector<double> &) override;
  void resetStatValues() override;
};

}  // namespace DRAM

}  // namespace SSDLab

#endif
