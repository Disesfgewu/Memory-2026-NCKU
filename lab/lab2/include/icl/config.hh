//Copyright (C) 2017 CAMELab

#ifndef __ICL_CONFIG__
#define __ICL_CONFIG__

#include "sim/base_config.hh"

namespace SSDLab {

namespace ICL {

typedef enum {
  /* Cache config */
  ICL_USE_READ_CACHE,
  ICL_USE_WRITE_CACHE,
  ICL_USE_READ_PREFETCH,
  ICL_PREFETCH_COUNT,
  ICL_PREFETCH_RATIO,
  ICL_PREFETCH_GRANULARITY,
  ICL_EVICT_POLICY,
  ICL_EVICT_GRANULARITY,
  ICL_CACHE_SIZE,
  ICL_WAY_SIZE,
  ICL_CACHE_LATENCY,
} ICL_CONFIG;

typedef enum {
  POLICY_RANDOM,               //!< Select way in random
  POLICY_FIFO,                 //!< Select way that lastly inserted
  POLICY_LEAST_RECENTLY_USED,  //!< Select way that least recently used
} EVICT_POLICY;

typedef enum {
  MODE_SUPERPAGE,  //!< Read one page from one super block (super page)
  MODE_ALL,        //!< Read one page from all NAND flashes
} PREFETCH_MODE;

typedef PREFETCH_MODE EVICT_MODE;

class Config : public BaseConfig {
 private:
  bool readCaching;            //!< Default: false
  bool writeCaching;           //!< Default: true
  bool readPrefetch;           //!< Default: false
  EVICT_POLICY evictPolicy;    //!< Default: POLICY_LEAST_RECENTLY_USED
  uint64_t cacheWaySize;       //!< Default: 1
  uint64_t cacheSize;          //!< Default: 33554432 (32MiB)
  uint64_t prefetchCount;      //!< Default: 1
  float prefetchRatio;         //!< Default: 0.5
  PREFETCH_MODE prefetchMode;  //!< Default: MODE_ALL
  EVICT_MODE evictMode;        //!< Default: MODE_ALL
  uint64_t cacheLatency;       //!< Default:

 public:
  Config();

  bool setConfig(const char *, const char *) override;
  void update() override;

  int64_t readInt(uint32_t) override;
  uint64_t readUint(uint32_t) override;
  float readFloat(uint32_t) override;
  bool readBoolean(uint32_t) override;
};

}  // namespace ICL

}  // namespace SSDLab

#endif
