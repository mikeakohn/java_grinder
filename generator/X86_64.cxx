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
#include <string.h>
#include <stdint.h>

#include "X86_64.h"

#define REG_STACK(a) (a)
#define LOCALS(i) (i * 4)

// ABI is:

X86_64::X86_64() :
  reg(0),
  reg_max(8),
  stack(0),
  is_main(0)
{

}

X86_64::~X86_64()
{

}

int X86_64::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, "BITS 64\n");

  // Set where RAM starts / ends
  // FIXME - Not sure what to set this to right now
  //fprintf(out, "ram_start equ 0\n");
  //fprintf(out, "ram_end equ 0x8000\n");

  return 0;
}

int X86_64::start_init()
{
  // Add any set up items (stack, registers, etc).
  fprintf(out, "SECTION .text\n");
  fprintf(out, "global init\n");
  fprintf(out, "init:\n");

  return 0;
}

int X86_64::insert_static_field_define(const char *name, const char *type, int index)
{
  //fprintf(out, "  %s: resb 4\n", name);
  fprintf(out, "  %s: resb 8  ; %s\n", name, type);
  return 0;
}


int X86_64::init_heap(int field_count)
{
  // Don't think we need a heap?
  return 0;
}

int X86_64::insert_field_init_boolean(char *name, int index, int value)
{
  return -1;
}

int X86_64::insert_field_init_byte(char *name, int index, int value)
{
  return -1;
}

int X86_64::insert_field_init_short(char *name, int index, int value)
{
  return -1;
}

int X86_64::insert_field_init_int(char *name, int index, int value)
{
  return -1;
}

int X86_64::insert_field_init(char *name, int index)
{
  return -1;
}

void X86_64::method_start(int local_count, int max_stack, int param_count, const char *name)
{
}

void X86_64::method_end(int local_count)
{
}

int X86_64::push_integer(int32_t n)
{
  return -1;
}

int X86_64::push_integer_local(int index)
{
  return -1;
}

int X86_64::push_ref_static(const char *name, int index)
{
  return -1;
}

int X86_64::push_ref_local(int index)
{
  return push_integer_local(index);
}

int X86_64::push_fake()
{
  return -1;
}

int X86_64::push_long(int64_t n)
{
  return -1;
}

int X86_64::push_float(float f)
{
  return -1;
}

int X86_64::push_double(double f)
{
  return -1;
}

int X86_64::push_byte(int8_t b)
{
  return -1;
}

int X86_64::push_short(int16_t s)
{
  return -1;
}

int X86_64::push_ref(char *name)
{
  return -1;
}

int X86_64::pop_integer_local(int index)
{
  return -1;
}

int X86_64::pop_ref_local(int index)
{
  return pop_integer_local(index);
}

int X86_64::pop()
{
  return -1;
}

int X86_64::dup()
{
  return -1;
}

int X86_64::dup2()
{
  return -1;
}

int X86_64::swap()
{
  return -1;
}

int X86_64::add_integer()
{
  return -1;
}

int X86_64::add_integer(int num)
{
  return -1;
}

int X86_64::sub_integer()
{
  return -1;
}

int X86_64::sub_integer(int num)
{
  return -1;
}

int X86_64::mul_integer()
{
  return -1;
}

int X86_64::div_integer()
{
  return -1;
}

int X86_64::mod_integer()
{
  return -1;
}

int X86_64::neg_integer()
{
  return -1;
}

int X86_64::shift_left_integer()
{
  return -1;
}

int X86_64::shift_left_integer(int num)
{
  return -1;
}

int X86_64::shift_right_integer()
{
  return -1;
}

int X86_64::shift_right_integer(int num)
{
  return -1;
}

int X86_64::shift_right_uinteger()
{
  return -1;
}

int X86_64::shift_right_uinteger(int num)
{
  return -1;
}

int X86_64::and_integer()
{
  return -1;
}

int X86_64::and_integer(int num)
{
  return -1;
}

int X86_64::or_integer()
{
  return -1;
}

int X86_64::or_integer(int num)
{
  return -1;
}

int X86_64::xor_integer()
{
  return -1;
}

int X86_64::xor_integer(int num)
{
  return -1;
}

int X86_64::inc_integer(int index, int num)
{
  return -1;
}

int X86_64::integer_to_byte()
{
  return -1;
}

int X86_64::integer_to_short()
{
  return -1;
}

int X86_64::jump_cond(const char *label, int cond, int distance)
{
  return -1;
}

int X86_64::jump_cond_integer(const char *label, int cond, int distance)
{
  return -1;
}

int X86_64::return_local(int index, int local_count)
{
  return -1;
}

int X86_64::return_integer(int local_count)
{
  return -1;
}

int X86_64::return_void(int local_count)
{
  return -1;
}

int X86_64::jump(const char *name, int distance)
{
  return -1;
}

int X86_64::call(const char *name)
{
  return -1;
}

int X86_64::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int X86_64::put_static(const char *name, int index)
{
  return -1;
}

int X86_64::get_static(const char *name, int index)
{
  return -1;
}

int X86_64::brk()
{
  return -1;
}

int X86_64::new_array(uint8_t type)
{
  return -1;
}

int X86_64::insert_array(const char *name, int32_t *data, int len, uint8_t type)
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

int X86_64::insert_string(const char *name, uint8_t *bytes, int len)
{
  fprintf(out, ".align 32\n");
  fprintf(out, "  dc32 %d\n", len);
  return insert_utf8(name, bytes, len);
}

int X86_64::push_array_length()
{
  return -1;
}

int X86_64::push_array_length(const char *name, int field_id)
{
  return -1;
}

int X86_64::array_read_byte()
{
  return -1;
}

int X86_64::array_read_short()
{
  return -1;
}

int X86_64::array_read_int()
{
  return -1;
}

int X86_64::array_read_byte(const char *name, int field_id)
{
  return -1;
}

int X86_64::array_read_short(const char *name, int field_id)
{
  return -1;
}

int X86_64::array_read_int(const char *name, int field_id)
{
  return -1;
}

int X86_64::array_write_byte()
{
  return -1;
}

int X86_64::array_write_short()
{
  return -1;
}

int X86_64::array_write_int()
{
  return -1;
}

int X86_64::array_write_byte(const char *name, int field_id)
{
  return -1;
}

int X86_64::array_write_short(const char *name, int field_id)
{
  return -1;
}

int X86_64::array_write_int(const char *name, int field_id)
{
  return -1;
}

