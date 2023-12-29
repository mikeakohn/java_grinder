/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2023 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_RISCV_ICE_FUN_H
#define JAVA_GRINDER_GENERATOR_RISCV_ICE_FUN_H

#include "generator/RISCV.h"

class RISCVIceFun : public RISCV
{
public:
  RISCVIceFun();
  virtual ~RISCVIceFun();

  // If this CPU is used for other things, move this into a more
  // specific file.
  virtual int ioport_setPinsValue_I(int port);
  virtual int ioport_setPinsValue_I(int port, int const_val);
  virtual int ice_fun_setTone_I();
};

#endif
