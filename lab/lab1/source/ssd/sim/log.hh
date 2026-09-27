/*
 * Copyright (C) 2017 CAMELab
 */

#pragma once

#ifndef __SIM_LOG__
#define __SIM_LOG__

#include <iostream>

namespace SSDLab {

void initLogSystem(std::ostream *, std::ostream *);
void destroyLogSystem();

}  // namespace SSDLab

#endif
