/*
 * Copyright (C) 2017 CAMELab
 */

#pragma once

#ifndef __LIB_MCPAT__
#define __LIB_MCPAT__

#include "XML_Parse.h"
#include "processor.h"

class McPAT {
 private:
  ParseXML *setting;

 public:
  McPAT(ParseXML *);
  ~McPAT();

  void getPower(Power &);
};

#endif
