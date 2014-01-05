/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014 by Michael Kohn
 *
 */

#ifndef _M6502_H
#define _M6502_H

#include "Generator.h"

class M6502 : public Generator
{
public:
  M6502();
  virtual ~M6502();

  virtual int open(char *filename);

  virtual void serial_init();
  virtual void method_start(int local_count, const char *name);
  virtual void method_end(int local_count);
  virtual int push_integer(int32_t n);
  virtual int push_integer_local(int index);
  virtual int push_long(int64_t n);
  virtual int push_float(float f);
  virtual int push_double(double f);
  virtual int push_byte(int8_t b);
  virtual int push_short(int16_t s);
  virtual int pop_integer_local(int index);
  virtual int pop();
  virtual int dup();
  virtual int dup2();
  virtual int swap();
  virtual int add_integers();
  virtual int sub_integers();
  virtual int mul_integers();
  virtual int div_integers();
  virtual int neg_integer();
  virtual int shift_left_integer();
  virtual int shift_right_integer();
  virtual int shift_right_uinteger();
  virtual int and_integer();
  virtual int or_integer();
  virtual int xor_integer();
  virtual int inc_integer(int index, int num);
  virtual int jump_cond(const char *label, int cond);
  virtual int jump_cond_integer(const char *label, int cond);
  virtual int return_local(int index, int local_count);
  virtual int return_integer(int local_count);
  virtual int return_void(int local_count);
  virtual int jump(const char *name);
  virtual int call(const char *name);
  virtual int brk();
  virtual void close();

  // GPIO functions
  virtual int ioport_setPinsAsInput();
  virtual int ioport_setPinsAsOutput();
  virtual int ioport_setPinsValue();
  virtual int ioport_setPinsHigh();
  virtual int ioport_setPinsLow();
  virtual int ioport_setPinAsOutput();
  virtual int ioport_setPinAsInput();
  virtual int ioport_setPinHigh();
  virtual int ioport_setPinLow();
  virtual int ioport_isPinInputHigh();
  virtual int ioport_getPortInputValue();
  virtual int ioport_setPortOutputValue();

};

#endif

