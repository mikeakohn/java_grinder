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

#ifndef _ARM_H
#define _ARM_H

#include "Generator.h"

class ARM : public Generator
{
public:
  ARM();
  virtual ~ARM();

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
  virtual int invoke_static_method(const char *name, int params, int is_void);
  virtual int brk();
  //virtual void close();

  // GPIO functions
  virtual int ioport_setPinsAsInput(int port);
  virtual int ioport_setPinsAsOutput(int port);
  virtual int ioport_setPinsValue(int port);
  virtual int ioport_setPinsHigh(int port);
  virtual int ioport_setPinsLow(int port);
  virtual int ioport_setPinAsOutput(int port);
  virtual int ioport_setPinAsInput(int port);
  virtual int ioport_setPinHigh(int port);
  virtual int ioport_setPinLow(int port);
  virtual int ioport_isPinInputHigh(int port);
  virtual int ioport_getPortInputValue(int port);
  virtual int ioport_setPortOutputValue(int port);
};

#endif

