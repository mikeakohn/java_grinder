/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 */

#ifndef _MSP430X_H
#define _MSP430X_H

#include "Generator.h"
#include "MSP430.h"

class MSP430X : public MSP430
{
public:
  MSP430X(uint8_t chip_type);
  virtual ~MSP430X();

  virtual int shift_left_integer();
  virtual int shift_left_integer(int count);
  virtual int shift_right_integer();
  virtual int shift_right_integer(int count);
  virtual int shift_right_uinteger();
  virtual int shift_right_uinteger(int count);

  // Timer functions
  virtual int timer_setInterval_II(int cycles, int divider);
  virtual int timer_setListener_Z(int const_value);
  virtual int timer_getValue();
  virtual int timer_setValue_I();
  virtual int timer_setValue_I(int const_value);

  // CPU functions
  virtual int cpu_setClock25();
  virtual int cpu_setClockExternal2();

private:
  void insert_set_vcore_up();

  bool need_set_vcore_up:1;
};

#endif

