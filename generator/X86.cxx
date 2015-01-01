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
#include <string.h>
#include <stdint.h>

#include "X86.h"

#define REG_STACK(a) (a)
#define LOCALS(i) (i * 4)

// ABI is:

X86::X86() :
  reg(0),
  reg_max(8),
  stack(0),
  is_main(0)
{

}

X86::~X86()
{

}

int X86::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".bits 32\n");

  // Set where RAM starts / ends
  // FIXME - Not sure what to set this to right now
  fprintf(out, "ram_start equ 0\n");
  fprintf(out, "ram_end equ 0x8000\n");

  return 0;
}

int X86::start_init()
{
  // Add any set up items (stack, registers, etc).
  //fprintf(out, ".org ???\n");
  fprintf(out, "start:\n");

  return 0;
}

int X86::insert_static_field_define(const char *name, const char *type, int index)
{
  fprintf(out, "%s equ ram_start+%d\n", name, (index + 1) * 4);
  return 0;
}


int X86::init_heap(int field_count)
{
  return -1;
}

int X86::insert_field_init_boolean(char *name, int index, int value)
{
  return -1;
}

int X86::insert_field_init_byte(char *name, int index, int value)
{
  return -1;
}

int X86::insert_field_init_short(char *name, int index, int value)
{
  return -1;
}

int X86::insert_field_init_int(char *name, int index, int value)
{
  return -1;
}

int X86::insert_field_init(char *name, int index)
{
  return -1;
}

void X86::method_start(int local_count, int max_stack, int param_count, const char *name)
{
}

void X86::method_end(int local_count)
{
}

int X86::push_integer(int32_t n)
{
  return -1;
}

int X86::push_integer_local(int index)
{
  return -1;
}

int X86::push_ref_local(int index)
{
  return push_integer_local(index);
}

int X86::push_fake()
{
  return -1;
}

int X86::push_long(int64_t n)
{
  return -1;
}

int X86::push_float(float f)
{
  return -1;
}

int X86::push_double(double f)
{
  return -1;
}

int X86::push_byte(int8_t b)
{
  return -1;
}

int X86::push_short(int16_t s)
{
  return -1;
}

int X86::push_ref(char *name)
{
  return -1;
}

int X86::pop_integer_local(int index)
{
  return -1;
}

int X86::pop_ref_local(int index)
{
  return pop_integer_local(index);
}

int X86::pop()
{
  return -1;
}

int X86::dup()
{
  return -1;
}

int X86::dup2()
{
  return -1;
}

int X86::swap()
{
  return -1;
}

int X86::add_integer()
{
  return -1;
}

int X86::add_integer(int num)
{
  return -1;
}

int X86::sub_integer()
{
  return -1;
}

int X86::sub_integer(int num)
{
  return -1;
}

int X86::mul_integer()
{
  return -1;
}

int X86::div_integer()
{
  return -1;
}

int X86::mod_integer()
{
  return -1;
}

int X86::neg_integer()
{
  return -1;
}

int X86::shift_left_integer()
{
  return -1;
}

int X86::shift_left_integer(int num)
{
  return -1;
}

int X86::shift_right_integer()
{
  return -1;
}

int X86::shift_right_integer(int num)
{
  return -1;
}

int X86::shift_right_uinteger()
{
  return -1;
}

int X86::shift_right_uinteger(int num)
{
  return -1;
}

int X86::and_integer()
{
  return -1;
}

int X86::and_integer(int num)
{
  return -1;
}

int X86::or_integer()
{
  return -1;
}

int X86::or_integer(int num)
{
  return -1;
}

int X86::xor_integer()
{
  return -1;
}

int X86::xor_integer(int num)
{
  return -1;
}

int X86::inc_integer(int index, int num)
{
  return -1;
}

int X86::integer_to_byte()
{
  return -1;
}

int X86::integer_to_short()
{
  return -1;
}

int X86::jump_cond(const char *label, int cond, int distance)
{
  return -1;
}

int X86::jump_cond_integer(const char *label, int cond, int distance)
{
  return -1;
}

int X86::return_local(int index, int local_count)
{
  return -1;
}

int X86::return_integer(int local_count)
{
  return -1;
}

int X86::return_void(int local_count)
{
  return -1;
}

int X86::jump(const char *name, int distance)
{
  return -1;
}

int X86::call(const char *name)
{
  return -1;
}

int X86::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int X86::put_static(const char *name, int index)
{
  return -1;
}

int X86::get_static(const char *name, int index)
{
  return -1;
}

int X86::brk()
{
  return -1;
}

int X86::new_array(uint8_t type)
{
  return -1;
}

int X86::insert_array(const char *name, int32_t *data, int len, uint8_t type)
{
  fprintf(out, ".align 32\n");
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

int X86::insert_string(const char *name, uint8_t *bytes, int len)
{
  fprintf(out, ".align 32\n");
  fprintf(out, "  dc32 %d\n", len);
  return insert_utf8(name, bytes, len);
}

int X86::push_array_length()
{
  return -1;
}

int X86::push_array_length(const char *name, int field_id)
{
  return -1;
}

int X86::array_read_byte()
{
  return -1;
}

int X86::array_read_short()
{
  return -1;
}

int X86::array_read_int()
{
  return -1;
}

int X86::array_read_byte(const char *name, int field_id)
{
  return -1;
}

int X86::array_read_short(const char *name, int field_id)
{
  return -1;
}

int X86::array_read_int(const char *name, int field_id)
{
  return -1;
}

int X86::array_write_byte()
{
  return -1;
}

int X86::array_write_short()
{
  return -1;
}

int X86::array_write_int()
{
  return -1;
}

int X86::array_write_byte(const char *name, int field_id)
{
  return -1;
}

int X86::array_write_short(const char *name, int field_id)
{
  return -1;
}

int X86::array_write_int(const char *name, int field_id)
{
  return -1;
}

