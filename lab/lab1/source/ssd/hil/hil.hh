/*
 * Copyright (C) 2017 CAMELab
 */

#ifndef __HIL_HIL__
#define __HIL_HIL__

#include <queue>

#include "icl/icl.hh"
#include "sim/dma_interface.hh"
#include "util/simplessd.hh"

namespace SSDLab {

namespace HIL {

class HIL : public StatObject {
 private:
  ConfigReader &conf;
  ICL::ICL *pICL;

  uint64_t reqCount;

  uint64_t lastScheduled;
  Event completionEvent;
  std::priority_queue<Request, std::vector<Request>, Request> completionQueue;

  struct {
    uint64_t request[2];
    uint64_t busy[3];
    uint64_t iosize[2];
    uint64_t lastBusyAt[3];
  } stat;

  void updateBusyTime(int, uint64_t, uint64_t);
  void updateCompletion();
  void completion();

 public:
  HIL(ConfigReader &);
  ~HIL();

  void read(Request &);
  void write(Request &);
  void flush(Request &);
  void trim(Request &);

  void format(Request &, bool);

  void getLPNInfo(uint64_t &, uint32_t &);
  uint64_t getUsedPageCount(uint64_t, uint64_t);

  void getStatList(std::vector<Stats> &, std::string) override;
  void getStatValues(std::vector<double> &) override;
  void resetStatValues() override;
};

}  // namespace HIL

}  // namespace SSDLab

#endif
