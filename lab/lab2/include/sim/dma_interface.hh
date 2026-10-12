//Copyright (C) 2017 CAMELab

#pragma once

#ifndef __SIM_DMA_INTERFACE__
#define __SIM_DMA_INTERFACE__

#include <cinttypes>
#include <functional>

namespace SSDLab {

typedef std::function<void(uint64_t, void *)> DMAFunction;

typedef struct _DMAContext {
  int counter;
  DMAFunction function;
  void *context;

  _DMAContext(DMAFunction &f) : counter(0), function(f), context(nullptr) {}
  _DMAContext(DMAFunction &f, void *c) : counter(0), function(f), context(c) {}
} DMAContext;

class DMAInterface {
 public:
  DMAInterface() {}
  virtual ~DMAInterface() {}

  virtual void dmaRead(uint64_t, uint64_t, uint8_t *, DMAFunction &,
                       void * = nullptr) = 0;
  virtual void dmaWrite(uint64_t, uint64_t, uint8_t *, DMAFunction &,
                        void * = nullptr) = 0;
};

}  // namespace SSDLab

#endif
