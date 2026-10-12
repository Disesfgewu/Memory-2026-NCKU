//Copyright (C) 2017 CAMELab

#ifndef FTL_LAB2_GC_HH
#define FTL_LAB2_GC_HH

#include <cinttypes>
#include <vector>

namespace SSDLab {

namespace FTL {

class PageMapping;

namespace Lab2GC {

void doGarbageCollection(PageMapping &, std::vector<uint32_t> &, uint64_t &);

}  // namespace Lab2GC

}  // namespace FTL

}  // namespace SSDLab

#endif
