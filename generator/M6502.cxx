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

#include "M6502.h"

// ABI is:
// A
// X
// Y

M6502::M6502()
{

}

M6502::~M6502()
{

}

int M6502::open(char *filename)
{
  return 0;
}

int M6502::init_heap(int field_count)
{
  return -1;
}

int M6502::insert_field_init_boolean(char *name, int index, int value)
{
  return -1;
}

int M6502::insert_field_init_byte(char *name, int index, int value)
{
  return -1;
}

int M6502::insert_field_init_short(char *name, int index, int value)
{
  return -1;
}

int M6502::insert_field_init_int(char *name, int index, int value)
{
  return -1;
}

int M6502::insert_field_init(char *name, int index)
{
  return -1;
}


void M6502::method_start(int local_count, const char *name)
{
}

void M6502::method_end(int local_count)
{
}

int M6502::push_integer(int32_t n)
{
  return -1;
}

int M6502::push_integer_local(int index)
{
  return -1;
}

int M6502::push_long(int64_t n)
{
  return -1;
}

int M6502::push_float(float f)
{
  return -1;
}

int M6502::push_double(double f)
{
  return -1;
}

int M6502::push_byte(int8_t b)
{
  return -1;
}

int M6502::push_short(int16_t s)
{
  return -1;
}

int M6502::pop_integer_local(int index)
{
  return -1;
}

int M6502::pop()
{
  return -1;
}

int M6502::dup()
{
  return -1;
}

int M6502::dup2()
{
  return -1;
}

int M6502::swap()
{
  return -1;
}

int M6502::add_integers()
{
  return -1;
}

int M6502::sub_integers()
{
  return -1;
}

int M6502::mul_integers()
{
  return -1;
}

int M6502::div_integers()
{
  return -1;
}

int M6502::mod_integers()
{
  return -1;
}

int M6502::neg_integer()
{
  return -1;
}

int M6502::shift_left_integer()
{
  return -1;
}

int M6502::shift_right_integer()
{
  return -1;
}

int M6502::shift_right_uinteger()
{
  return -1;
}

int M6502::and_integer()
{
  return -1;
}

int M6502::or_integer()
{
  return -1;
}

int M6502::xor_integer()
{
  return -1;
}

int M6502::inc_integer(int index, int num)
{
  return -1;
}

int M6502::jump_cond(const char *label, int cond)
{
  return -1;
}

int M6502::jump_cond_integer(const char *label, int cond)
{
  return -1;
}

int M6502::return_local(int index, int local_count)
{
  return -1;
}

int M6502::return_integer(int local_count)
{
  return -1;
}

int M6502::return_void(int local_count)
{
  return -1;
}

int M6502::jump(const char *name)
{
  return -1;
}

int M6502::call(const char *name)
{
  return -1;
}

int M6502::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int M6502::put_static(int index)
{
  return -1;
}

int M6502::brk()
{
  return -1;
}

int M6502::insert_array(const char *name, int32_t *data, int len, uint8_t type)
{
  return -1;
}

int M6502::push_array_length(const char *name, int field_id)
{
  return -1;
}

int M6502::array_read_byte(const char *name, int field_id)
{
  return -1;
}

int M6502::array_read_short(const char *name, int field_id)
{
  return -1;
}

int M6502::array_read_int(const char *name, int field_id)
{
  return -1;
}

int M6502::array_write_byte(const char *name, int field_id)
{
  return -1;
}

int M6502::array_write_short(const char *name, int field_id)
{
  return -1;
}

int M6502::array_write_int(const char *name, int field_id)
{
  return -1;
}


#if 0
void M6502::close()
{
}
#endif

