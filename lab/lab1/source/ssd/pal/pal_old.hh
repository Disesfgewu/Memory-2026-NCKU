/*
 * Copyright (C) 2017 CAMELab
 */

#ifndef __PAL_PAL_OLD__
#define __PAL_PAL_OLD__

#include <cinttypes>
#include <vector>

#include "pal/abstract_pal.hh"
#include "util/old/SimpleSSD_types.h"

class PAL2;
class PALStatistics;
class Latency;

namespace SSDLab {

namespace PAL {

class PALOLD : public AbstractPAL {
 private:
  ::PAL2 *pal;
  ::PALStatistics *stats;
  ::Latency *lat;

  Event flushEvent;
  EventFunction flushFunction;

  uint8_t lastResetTick;

  struct {
    uint64_t readCount;
    uint64_t writeCount;
    uint64_t eraseCount;
  } stat;

  void convertCPDPBP(Request &, std::vector<::CPDPBP> &);
  void printCPDPBP(::CPDPBP &, const char *);
  void printPPN(Request &, const char *);

 public:
  PALOLD(Parameter &, ConfigReader &);
  ~PALOLD();

  void read(Request &, uint64_t &) override;
  void write(Request &, uint64_t &) override;
  void erase(Request &, uint64_t &) override;

  void getStatList(std::vector<Stats> &, std::string) override;
  void getStatValues(std::vector<double> &) override;
  void resetStatValues() override;

  // Direct interface for OCSSD
  void read(::CPDPBP &, uint64_t &);
  void write(::CPDPBP &, uint64_t &);
  void erase(::CPDPBP &, uint64_t &);
};

}  // namespace PAL

}  // namespace SSDLab

#endif
