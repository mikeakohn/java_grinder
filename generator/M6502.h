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
  virtual int ioport_set_pins_as_input();
  virtual int ioport_set_pins_as_output();
  virtual int ioport_set_pins_value();
  virtual int ioport_set_pins_high();
  virtual int ioport_set_pins_low();
  virtual int ioport_set_pin_as_output();
  virtual int ioport_set_pin_as_input();
  virtual int ioport_set_pin_high();
  virtual int ioport_set_pin_low();
  virtual int ioport_is_pin_input_high();
  virtual int ioport_get_port_input_value();
  virtual int ioport_set_port_output_value();

};

#endif

