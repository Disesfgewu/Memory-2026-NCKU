//Copyright (C) 2017 CAMELab

#ifndef __PAL_PAL__
#define __PAL_PAL__

#include "util/def.hh"
#include "util/simplessd.hh"

namespace SSDLab {

namespace PAL {

class AbstractPAL;

typedef struct {
  uint32_t channel;          //!< Total # channels
  uint32_t package;          //!< # packages / channel
  uint32_t die;              //!< # dies / package
  uint32_t plane;            //!< # planes / die
  uint32_t block;            //!< # blocks / plane
  uint32_t page;             //!< # pages / block
  uint32_t superBlock;       //!< Total super blocks
  uint32_t pageSize;         //!< Size of page in bytes
  uint32_t superPageSize;    //!< Size of super page in bytes
  uint32_t pageInSuperPage;  //!< # pages in one superpage
} Parameter;

class PAL : public StatObject {
 private:
  Parameter param;
  AbstractPAL *pPAL;

  ConfigReader &conf;

 public:
  PAL(ConfigReader &);
  ~PAL();

  void read(Request &, uint64_t &);
  void write(Request &, uint64_t &);
  void erase(Request &, uint64_t &);
  void copyback(uint32_t, uint32_t, uint32_t, uint64_t &);

  Parameter *getInfo();

  void getStatList(std::vector<Stats> &, std::string) override;
  void getStatValues(std::vector<double> &) override;
  void resetStatValues() override;
};

}  // namespace PAL

}  // namespace SSDLab

#endif
