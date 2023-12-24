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

#ifndef JAVA_GRINDER_GENERATOR_API_ICE_FUN_H
#define JAVA_GRINDER_GENERATOR_API_ICE_FUN_H

class API_IceFun
{
public:
  // IceFun FPGA board.
  virtual int ice_fun_setTone_I() { return -1; }
  virtual int ice_fun_setServo_II() { return -1; }
};

#endif

