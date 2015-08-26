/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 * W65816 written by Joe Davisson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "W65816.h"

W65816::W65816() :
  stack(0),
  is_main(0),
  need_mul_integer(0),
  need_div_integer(0)
{
}

W65816::~W65816()
{
  if(need_mul_integer) { insert_mul_integer(); }
  if(need_div_integer) { insert_mul_integer(); }
}

int W65816::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".65816\n");

  // ram start
  fprintf(out, "ram_start equ 0x8000\n");
  fprintf(out, "heap_ptr equ ram_start\n");

  // points to locals
  fprintf(out, "locals equ 0x00\n");

  // temp variables
  fprintf(out, "result equ 0x00\n");
  fprintf(out, "remainder equ 0x02\n");
  fprintf(out, "length equ 0x04\n");
  fprintf(out, "value1 equ 0x06\n");
  fprintf(out, "value2 equ 0x08\n");
  fprintf(out, "value3 equ 0x10\n");

  // start
  fprintf(out, ".org 0x2000\n");
  fprintf(out, "; change to 16-bit mode\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  xce\n");
  fprintf(out, "; all 16-bit registers\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "; set up stack\n");
  fprintf(out, "  lda #0x1FF\n");
  fprintf(out, "  tcs\n");
  fprintf(out, "; set up direct-page\n");
  fprintf(out, "  pea 0x0000\n");
//FIXME remove
  fprintf(out, "; do something\n");
  fprintf(out, "  lda #0xabcd\n");
  fprintf(out, "  sta 0x3000\n");
  fprintf(out, "; exit to monitor\n");
//  fprintf(out, "  sec\n");
//  fprintf(out, "  xce\n");
//  fprintf(out, "loop:\n");
//  fprintf(out, "  jmp loop\n");
  fprintf(out, "  brk\n");

  return 0;
}

int W65816::start_init()
{
  return 0;
}

int W65816::insert_static_field_define(const char *name, const char *type, int index)
{
  return -1;
}

int W65816::init_heap(int field_count)
{
  return -1;
}

int W65816::insert_field_init_boolean(char *name, int index, int value)
{
  return -1;
}

int W65816::insert_field_init_byte(char *name, int index, int value)
{
  return insert_field_init_int(name, index, value);
}

int W65816::insert_field_init_short(char *name, int index, int value)
{
  return insert_field_init_int(name, index, value);
}

int W65816::insert_field_init_int(char *name, int index, int value)
{
  return -1;
}

int W65816::insert_field_init(char *name, int index)
{
  return -1;
}

void W65816::method_start(int local_count, int max_stack, int param_count, const char *name)
{
}

void W65816::method_end(int local_count)
{
}

int W65816::push_integer(int32_t n)
{
  return -1;
}

int W65816::push_integer_local(int index)
{
  return -1;
}

int W65816::push_ref_static(const char *name, int index)
{
  return -1;
}

int W65816::push_ref_local(int index)
{
  return push_integer_local(index);
}

int W65816::push_fake()
{
  return -1;
}

int W65816::push_long(int64_t n)
{
  return -1;
}

int W65816::push_float(float f)
{
  return -1;
}

int W65816::push_double(double f)
{
  return -1;
}

int W65816::push_byte(int8_t b)
{
  int32_t value = (int32_t)b;

  return push_integer(value);
}

int W65816::push_short(int16_t s)
{
  int32_t value = (int32_t)s;

  return push_integer(value);
}

int W65816::push_ref(char *name)
{
  // Need to move the address of name to the top of stack
  return -1;
}

int W65816::pop_integer_local(int index)
{
  return -1;
}

int W65816::pop_ref_local(int index)
{
  return pop_integer_local(index);
}

int W65816::pop()
{
  return -1;
}

int W65816::dup()
{
  return -1;
}

int W65816::dup2()
{
  return -1;
}

int W65816::swap()
{
  return -1;
}

int W65816::add_integer()
{
  return -1;
}

int W65816::add_integer(int num)
{
  return -1;
}

int W65816::sub_integer()
{
  return -1;
}

int W65816::sub_integer(int num)
{
  return -1;
}

int W65816::mul_integer()
{
  return -1;
}

int W65816::div_integer()
{
  return -1;
}

int W65816::mod_integer()
{
  return -1;
}

int W65816::neg_integer()
{
  return -1;
}

int W65816::shift_left_integer()
{
  return -1;
}

int W65816::shift_left_integer(int num)
{
  return -1;
}

int W65816::shift_right_integer()
{
  return -1;
}

int W65816::shift_right_integer(int num)
{
  return -1;
}

int W65816::shift_right_uinteger()
{
  return -1;
}

int W65816::shift_right_uinteger(int num)
{
  return -1;
}

int W65816::and_integer()
{
  return -1;
}

int W65816::and_integer(int num)
{
  return -1;
}

int W65816::or_integer()
{
  return -1;
}

int W65816::or_integer(int num)
{
  return -1;
}

int W65816::xor_integer()
{
  return -1;
}

int W65816::xor_integer(int num)
{
  return -1;
}

int W65816::inc_integer(int index, int num)
{
  return -1;
}

int W65816::integer_to_byte()
{
  return -1;
}

int W65816::integer_to_short()
{
  return -1;
}

int W65816::jump_cond(const char *label, int cond, int distance)
{
  return -1;
}

int W65816::jump_cond_integer(const char *label, int cond, int distance)
{
  return -1;
}

int W65816::return_local(int index, int local_count)
{
  return -1;
}

int W65816::return_integer(int local_count)
{
  return -1;
}

int W65816::return_void(int local_count)
{
//FIXME for testing
  return 0;
}

int W65816::jump(const char *name, int distance)
{
  return -1;
}

int W65816::call(const char *name)
{
  return -1;
}

int W65816::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int W65816::put_static(const char *name, int index)
{
  return -1;
}

int W65816::get_static(const char *name, int index)
{
  return -1;
}

int W65816::brk()
{
  return -1;
}

int W65816::new_array(uint8_t type)
{
  return -1;
}

int W65816::insert_array(const char *name, int32_t *data, int len, uint8_t type)
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

int W65816::insert_string(const char *name, uint8_t *bytes, int len)
{
  return -1;
}

int W65816::push_array_length()
{
  return -1;
}

int W65816::push_array_length(const char *name, int field_id)
{
  return -1;
}

int W65816::array_read_byte()
{
  return -1;
}

int W65816::array_read_short()
{
  return -1;
}

int W65816::array_read_int()
{
  return -1;
}

int W65816::array_read_byte(const char *name, int field_id)
{
  return -1;
}

int W65816::array_read_short(const char *name, int field_id)
{
  return -1;
}

int W65816::array_read_int(const char *name, int field_id)
{
  return -1;
}

int W65816::array_write_byte()
{
  return -1;
}

int W65816::array_write_short()
{
  return -1;
}

int W65816::array_write_int()
{
  return -1;
}

int W65816::array_write_byte(const char *name, int field_id)
{
  return -1;
}

int W65816::array_write_short(const char *name, int field_id)
{
  return -1;
}

int W65816::array_write_int(const char *name, int field_id)
{
  return -1;
}

// subroutines
void W65816::insert_mul_integer()
{
}

void W65816::insert_div_integer()
{
}

