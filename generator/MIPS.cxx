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

#include "MIPS.h"

// ABI is:
// r4
// r5
// r6
// r7
// r8
// r9
// r10
// r11

MIPS::MIPS() :
  reg(0),
  reg_max(6),
  stack(0),
  is_main(0)
{

}

MIPS::~MIPS()
{

}

int MIPS::open(char *filename)
{
  return 0;
}

int MIPS::start_init()
{
  return -1;
}

int MIPS::insert_static_field_define(const char *name, const char *type, int index)
{
  return -1;
}


int MIPS::init_heap(int field_count)
{
  return -1;
}

int MIPS::insert_field_init_boolean(char *name, int index, int value)
{
  return -1;
}

int MIPS::insert_field_init_byte(char *name, int index, int value)
{
  return -1;
}

int MIPS::insert_field_init_short(char *name, int index, int value)
{
  return -1;
}

int MIPS::insert_field_init_int(char *name, int index, int value)
{
  return -1;
}

int MIPS::insert_field_init(char *name, int index)
{
  return -1;
}

void MIPS::method_start(int local_count, const char *name)
{
}

void MIPS::method_end(int local_count)
{
}

int MIPS::push_integer(int32_t n)
{
  return -1;
}

int MIPS::push_integer_local(int index)
{
  return -1;
}

int MIPS::push_ref_local(int index)
{
  return push_ref_local(index);
}

int MIPS::push_long(int64_t n)
{
  return -1;
}

int MIPS::push_float(float f)
{
  return -1;
}

int MIPS::push_double(double f)
{
  return -1;
}

int MIPS::push_byte(int8_t b)
{
  return -1;
}

int MIPS::push_short(int16_t s)
{
  return -1;
}

int MIPS::pop_integer_local(int index)
{
  return -1;
}

int MIPS::pop_ref_local(int index)
{
  return pop_integer_local(index);
}

int MIPS::pop()
{
  return -1;
}

int MIPS::dup()
{
  return -1;
}

int MIPS::dup2()
{
  return -1;
}

int MIPS::swap()
{
  return -1;
}

int MIPS::add_integer()
{
  return -1;
}

int MIPS::sub_integer()
{
  return -1;
}

int MIPS::mul_integer()
{
  return -1;
}

int MIPS::div_integer()
{
  return -1;
}

int MIPS::mod_integer()
{
  return -1;
}

int MIPS::neg_integer()
{
  return -1;
}

int MIPS::shift_left_integer()
{
  return -1;
}

int MIPS::shift_right_integer()
{
  return -1;
}

int MIPS::shift_right_uinteger()
{
  return -1;
}

int MIPS::and_integer()
{
  return -1;
}

int MIPS::or_integer()
{
  return -1;
}

int MIPS::xor_integer()
{
  return -1;
}

int MIPS::inc_integer(int index, int num)
{
  return -1;
}

int MIPS::jump_cond(const char *label, int cond)
{
  return -1;
}

int MIPS::jump_cond_integer(const char *label, int cond)
{
  return -1;
}

int MIPS::return_local(int index, int local_count)
{
  return -1;
}

int MIPS::return_integer(int local_count)
{
  return -1;
}

int MIPS::return_void(int local_count)
{
  return -1;
}

int MIPS::jump(const char *name)
{
  return -1;
}

int MIPS::call(const char *name)
{
  return -1;
}

int MIPS::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int MIPS::put_static(const char *name, int index)
{
  return -1;
}

int MIPS::get_static(const char *name, int index)
{
  return -1;
}

int MIPS::brk()
{
  return -1;
}

int MIPS::new_array(uint8_t type)
{
  return -1;
}

int MIPS::insert_array(const char *name, int32_t *data, int len, uint8_t type)
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

int MIPS::push_array_length()
{
  return -1;
}

int MIPS::push_array_length(const char *name, int field_id)
{
  return -1;
}

int MIPS::array_read_byte()
{
  return -1;
}

int MIPS::array_read_short()
{
  return -1;
}

int MIPS::array_read_int()
{
  return -1;
}

int MIPS::array_read_byte(const char *name, int field_id)
{
  return -1;
}

int MIPS::array_read_short(const char *name, int field_id)
{
  return -1;
}

int MIPS::array_read_int(const char *name, int field_id)
{
  return -1;
}

int MIPS::array_write_byte()
{
  return -1;
}

int MIPS::array_write_short()
{
  return -1;
}

int MIPS::array_write_int()
{
  return -1;
}

int MIPS::array_write_byte(const char *name, int field_id)
{
  return -1;
}

int MIPS::array_write_short(const char *name, int field_id)
{
  return -1;
}

int MIPS::array_write_int(const char *name, int field_id)
{
  return -1;
}

// GPIO functions
int MIPS::ioport_setPinsAsInput(int port) { return -1; }
int MIPS::ioport_setPinsAsOutput(int port) { return -1; }
int MIPS::ioport_setPinsValue(int port) { return -1; }
int MIPS::ioport_setPinsHigh(int port) { return -1; }
int MIPS::ioport_setPinsLow(int port) { return -1; }
int MIPS::ioport_setPinAsOutput(int port) { return -1; }
int MIPS::ioport_setPinAsInput(int port) { return -1; }
int MIPS::ioport_setPinHigh(int port) { return -1; }
int MIPS::ioport_setPinLow(int port) { return -1; }
int MIPS::ioport_isPinInputHigh(int port) { return -1; }
int MIPS::ioport_getPortInputValue(int port) { return -1; }
//int MIPS::ioport_setPortOutputValue(int port) { return -1; }

