/*
 * Copyright (C) 2017 CAMELab
 */

#pragma once

#ifndef __UTIL_PRINT__
#define __UTIL_PRINT__

#include <cinttypes>
#include <iostream>
#include <string>

void print(std::ostream &, std::string, uint32_t);
void print(std::ostream &, double, uint32_t);

#endif
