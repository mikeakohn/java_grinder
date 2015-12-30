/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "M6502_8.h"

// ABI is:

M6502_8::M6502_8() :
  is_main(0)
{

}

M6502_8::~M6502_8()
{
}

int M6502_8::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".65xx\n");

  // Set where RAM starts / ends

  return 0;
}

int M6502_8::start_init()
{
  // Add any set up items (stack, registers, etc).
  //fprintf(out, ".org ???\n");
  fprintf(out, "start:\n");

  return 0;
}

int M6502_8::insert_static_field_define(const char *name, const char *type, int index)
{
  fprintf(out, "%s equ %d\n", name, (index + 1) * 4);
  return 0;
}

int M6502_8::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");

  return 0;
}

int M6502_8::insert_field_init_boolean(char *name, int index, int value)
{
  value = (value == 0) ? 0 : 1;

  return 0;
}

int M6502_8::insert_field_init_byte(char *name, int index, int value)
{
  return insert_field_init_int(name, index, value);
}

int M6502_8::insert_field_init_short(char *name, int index, int value)
{
  return insert_field_init_int(name, index, value);
}

int M6502_8::insert_field_init_int(char *name, int index, int value)
{

  return -1;
}

int M6502_8::insert_field_init(char *name, int index)
{
  return -1;
}

void M6502_8::method_start(int local_count, int max_stack, int param_count, const char *name)
{
}

void M6502_8::method_end(int local_count)
{
}

int M6502_8::push_integer(int32_t n)
{
  return -1;
}

int M6502_8::push_integer_local(int index)
{
  return -1;
}

int M6502_8::push_ref_static(const char *name, int index)
{
  return -1;
}

int M6502_8::push_ref_local(int index)
{
  return push_integer_local(index);
}

int M6502_8::push_fake()
{
  //if (stack != 0) { return -1; }

  //reg++;
  return -1;
}

int M6502_8::push_long(int64_t n)
{
  return -1;
}

int M6502_8::push_float(float f)
{
  return -1;
}

int M6502_8::push_double(double f)
{
  return -1;
}

int M6502_8::push_byte(int8_t b)
{
  int32_t value = (int32_t)b;

  return push_integer(value);
}

int M6502_8::push_short(int16_t s)
{
  int32_t value = (int32_t)s;

  return push_integer(value);
}

int M6502_8::push_ref(char *name)
{
  // Need to move the address of name to the top of stack
  return -1;
}

int M6502_8::pop_integer_local(int index)
{
  return -1;
}

int M6502_8::pop_ref_local(int index)
{
  return pop_integer_local(index);
}

int M6502_8::pop()
{
  return -1;
}

int M6502_8::dup()
{
  return -1;
}

int M6502_8::dup2()
{
  return -1;
}

int M6502_8::swap()
{
  return -1;
}

int M6502_8::add_integer()
{
  return -1;
}

int M6502_8::add_integer(int num)
{
  return -1;
}

int M6502_8::sub_integer()
{
  return -1;
}

int M6502_8::sub_integer(int num)
{
  return -1;
}

int M6502_8::mul_integer()
{
  return -1;
}

int M6502_8::div_integer()
{
  return -1;
}

int M6502_8::mod_integer()
{
  return -1;
}

int M6502_8::neg_integer()
{
  return -1;
}

int M6502_8::shift_left_integer()
{
  return -1;
}

int M6502_8::shift_left_integer(int num)
{
  return 0;
}

int M6502_8::shift_right_integer()
{
  return -1;
}

int M6502_8::shift_right_integer(int num)
{
  return -1;
}

int M6502_8::shift_right_uinteger()
{
  return -1;
}

int M6502_8::shift_right_uinteger(int num)
{
  return -1;
}

int M6502_8::and_integer()
{
  return -1;
}

int M6502_8::and_integer(int num)
{
  return -1;
}

int M6502_8::or_integer()
{
  return -1;
}

int M6502_8::or_integer(int num)
{
  return -1;
}

int M6502_8::xor_integer()
{
  return -1;
}

int M6502_8::xor_integer(int num)
{
  return -1;
}

int M6502_8::inc_integer(int index, int num)
{
  return -1;
}

int M6502_8::integer_to_byte()
{
  return -1;
}

int M6502_8::integer_to_short()
{
  return -1;
}

int M6502_8::jump_cond(const char *label, int cond, int distance)
{
  return -1;
}

int M6502_8::jump_cond_integer(const char *label, int cond, int distance)
{
  return -1;
}

int M6502_8::return_local(int index, int local_count)
{
  return -1;
}

int M6502_8::return_integer(int local_count)
{
  return -1;
}

int M6502_8::return_void(int local_count)
{
  return -1;
}

int M6502_8::jump(const char *name, int distance)
{
  return -1;
}

int M6502_8::call(const char *name)
{
  return -1;
}

int M6502_8::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int M6502_8::put_static(const char *name, int index)
{
  return -1;
}

int M6502_8::get_static(const char *name, int index)
{
  return -1;
}

int M6502_8::brk()
{
  return -1;
}

int M6502_8::new_array(uint8_t type)
{
  return -1;
}

int M6502_8::insert_array(const char *name, int32_t *data, int len, uint8_t type)
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

int M6502_8::insert_string(const char *name, uint8_t *bytes, int len)
{
  return -1;
}

int M6502_8::push_array_length()
{
  return -1;
}

int M6502_8::push_array_length(const char *name, int field_id)
{
  return -1;
}

int M6502_8::array_read_byte()
{
  return -1;
}

int M6502_8::array_read_short()
{
  return -1;
}

int M6502_8::array_read_int()
{
  return -1;
}

int M6502_8::array_read_byte(const char *name, int field_id)
{
  return -1;
}

int M6502_8::array_read_short(const char *name, int field_id)
{
  return -1;
}

int M6502_8::array_read_int(const char *name, int field_id)
{
  return -1;
}

int M6502_8::array_write_byte()
{
  return -1;
}

int M6502_8::array_write_short()
{
  return -1;
}

int M6502_8::array_write_int()
{
  return -1;
}

int M6502_8::array_write_byte(const char *name, int field_id)
{
  return -1;
}

int M6502_8::array_write_short(const char *name, int field_id)
{
  return -1;
}

int M6502_8::array_write_int(const char *name, int field_id)
{
  return -1;
}


