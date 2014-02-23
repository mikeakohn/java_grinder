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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ARM.h"

// ABI is:
// r4
// r5
// r6
// r7
// r8
// r9
// r10
// r11

ARM::ARM() :
  reg(0),
  reg_max(6),
  stack(0),
  is_main(0)
{

}

ARM::~ARM()
{

}

int ARM::open(char *filename)
{
  return 0;
}

int ARM::start_init()
{
  return -1;
}

int ARM::insert_static_field_define(const char *name, const char *type, int index)
{
  return -1;
}

int ARM::init_heap(int field_count)
{
  return -1;
}

int ARM::insert_field_init_boolean(char *name, int index, int value)
{
  return -1;
}

int ARM::insert_field_init_byte(char *name, int index, int value)
{
  return -1;
}

int ARM::insert_field_init_short(char *name, int index, int value)
{
  return -1;
}

int ARM::insert_field_init_int(char *name, int index, int value)
{
  return -1;
}

int ARM::insert_field_init(char *name, int index)
{
  return -1;
}

void ARM::method_start(int local_count, const char *name)
{
}

void ARM::method_end(int local_count)
{
}

int ARM::push_integer(int32_t n)
{
  return -1;
}

int ARM::push_integer_local(int index)
{
  return -1;
}

int ARM::push_ref_local(int index)
{
  return push_integer_local(index);
}

int ARM::push_long(int64_t n)
{
  return -1;
}

int ARM::push_float(float f)
{
  return -1;
}

int ARM::push_double(double f)
{
  return -1;
}

int ARM::push_byte(int8_t b)
{
  return -1;
}

int ARM::push_short(int16_t s)
{
  return -1;
}

int ARM::pop_integer_local(int index)
{
  return -1;
}

int ARM::pop_ref_local(int index)
{
  return pop_integer_local(index);
}

int ARM::pop()
{
  return -1;
}

int ARM::dup()
{
  return -1;
}

int ARM::dup2()
{
  return -1;
}

int ARM::swap()
{
  return -1;
}

int ARM::add_integer()
{
  return -1;
}

int ARM::sub_integer()
{
  return -1;
}

int ARM::mul_integer()
{
  return -1;
}

int ARM::div_integer()
{
  return -1;
}

int ARM::mod_integer()
{
  return -1;
}

int ARM::neg_integer()
{
  return -1;
}

int ARM::shift_left_integer()
{
  return -1;
}

int ARM::shift_right_integer()
{
  return -1;
}

int ARM::shift_right_uinteger()
{
  return -1;
}

int ARM::and_integer()
{
  return -1;
}

int ARM::or_integer()
{
  return -1;
}

int ARM::xor_integer()
{
  return -1;
}

int ARM::inc_integer(int index, int num)
{
  return -1;
}

int ARM::jump_cond(const char *label, int cond)
{
  return -1;
}

int ARM::jump_cond_integer(const char *label, int cond)
{
  return -1;
}

int ARM::return_local(int index, int local_count)
{
  return -1;
}

int ARM::return_integer(int local_count)
{
  return -1;
}

int ARM::return_void(int local_count)
{
  return -1;
}

int ARM::jump(const char *name)
{
  return -1;
}

int ARM::call(const char *name)
{
  return -1;
}

int ARM::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int ARM::put_static(const char *name, int index)
{
  return -1;
}

int ARM::get_static(const char *name, int index)
{
  return -1;
}

int ARM::brk()
{
  return -1;
}

int ARM::new_array(uint8_t type)
{
  return -1;
}

int ARM::insert_array(const char *name, int32_t *data, int len, uint8_t type)
{
  if (type == TYPE_BYTE)
  { return insert_db(name, data, len, TYPE_INT); }
    else
  if (type == TYPE_SHORT)
  { return insert_dw(name, data, len, TYPE_INT); }
    else
  if (type == TYPE_INT)
  { return insert_dc32(name, data, len, TYPE_INT); }

  return -1;
}

int ARM::push_array_length()
{
  return -1;
}

int ARM::push_array_length(const char *name, int field_id)
{
  return -1;
}

int ARM::array_read_byte()
{
  return -1;
}

int ARM::array_read_short()
{
  return -1;
}

int ARM::array_read_int()
{
  return -1;
}

int ARM::array_read_byte(const char *name, int field_id)
{
  return -1;
}

int ARM::array_read_short(const char *name, int field_id)
{
  return -1;
}

int ARM::array_read_int(const char *name, int field_id)
{
  return -1;
}

int ARM::array_write_byte()
{
  return -1;
}

int ARM::array_write_short()
{
  return -1;
}

int ARM::array_write_int()
{
  return -1;
}

int ARM::array_write_byte(const char *name, int field_id)
{
  return -1;
}

int ARM::array_write_short(const char *name, int field_id)
{
  return -1;
}

int ARM::array_write_int(const char *name, int field_id)
{
  return -1;
}

#if 0
void ARM::close()
{
}
#endif

// GPIO functions
int ARM::ioport_setPinsAsInput(int port) { return -1; }
int ARM::ioport_setPinsAsOutput(int port) { return -1; }
int ARM::ioport_setPinsValue(int port) { return -1; }
int ARM::ioport_setPinsHigh(int port) { return -1; }
int ARM::ioport_setPinsLow(int port) { return -1; }
int ARM::ioport_setPinAsOutput(int port) { return -1; }
int ARM::ioport_setPinAsInput(int port) { return -1; }
int ARM::ioport_setPinHigh(int port) { return -1; }
int ARM::ioport_setPinLow(int port) { return -1; }
int ARM::ioport_isPinInputHigh(int port) { return -1; }
int ARM::ioport_getPortInputValue(int port) { return -1; }
//int ARM::ioport_setPortOutputValue(int port) { return -1; }

