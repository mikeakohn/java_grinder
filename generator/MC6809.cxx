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

#include "MC6809.h"

#define REG_STACK(a) (a)
#define LOCALS(i) (i * 4)

// ABI is:

MC6809::MC6809() :
  start_org(0),
  ram_start(0x0600),
  ram_end(0x06ff),
  reg(0),
  reg_max(9),
  stack(0),
  is_main(0)
{

}

MC6809::~MC6809()
{
  insert_constants_pool();
}

int MC6809::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".6809\n");

  // Set where RAM starts / ends
  fprintf(out, "ram_start equ 0x%04x\n", ram_start);
  fprintf(out, "ram_end equ 0x0x%04x\n", ram_end);

  return 0;
}

int MC6809::start_init()
{
  // Add any set up items (stack, registers, etc).
  fprintf(out, ".org 0x%04x\n", start_org);
  fprintf(out, "start:\n");
  fprintf(out, "  lds #0x%04x ; Set SP stack pointer\n", ram_end + 1);

  return 0;
}

int MC6809::insert_static_field_define(const char *name, const char *type, int index)
{
  return -1;
}

int MC6809::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");
  //fprintf(out, "  mov #ram_start+%d, &ram_start\n", (field_count + 1) * 2);

  return -1;
}

int MC6809::insert_field_init_boolean(char *name, int index, int value)
{

  return -1;
}

int MC6809::insert_field_init_byte(char *name, int index, int value)
{
  return insert_field_init_int(name, index, value);
}

int MC6809::insert_field_init_short(char *name, int index, int value)
{
  return insert_field_init_int(name, index, value);
}

int MC6809::insert_field_init_int(char *name, int index, int value)
{
  return -1;
}

int MC6809::insert_field_init(char *name, int index)
{
  return -1;
}

void MC6809::method_start(int local_count, int max_stack, int param_count, const char *name)
{
}

void MC6809::method_end(int local_count)
{
}

int MC6809::push_integer(int32_t n)
{
  return -1;
}

int MC6809::push_integer_local(int index)
{
  return -1;
}

int MC6809::push_ref_static(const char *name, int index)
{
  return -1;
}

int MC6809::push_ref_local(int index)
{
  return push_integer_local(index);
}

int MC6809::push_fake()
{
  return -1;
}

int MC6809::push_long(int64_t n)
{
  return -1;
}

int MC6809::push_float(float f)
{
  return -1;
}

int MC6809::push_double(double f)
{
  return -1;
}

int MC6809::push_byte(int8_t b)
{
  int32_t value = (int32_t)b;

  return push_integer(value);
}

int MC6809::push_short(int16_t s)
{
  int32_t value = (int32_t)s;

  return push_integer(value);
}

int MC6809::push_ref(char *name)
{
  // Need to move the address of name to the top of stack
  return -1;
}

int MC6809::pop_integer_local(int index)
{
  return -1;
}

int MC6809::pop_ref_local(int index)
{
  return pop_integer_local(index);
}

int MC6809::pop()
{
  return -1;
}

int MC6809::dup()
{
  return -1;
}

int MC6809::dup2()
{
  return -1;
}

int MC6809::swap()
{
  return -1;
}

int MC6809::add_integer()
{
  return -1;
}

int MC6809::add_integer(int num)
{
  return -1;
}

int MC6809::sub_integer()
{
  return -1;
}

int MC6809::sub_integer(int num)
{
  return -1;
}

int MC6809::mul_integer()
{
  return -1;
}

int MC6809::div_integer()
{
  return -1;
}

int MC6809::mod_integer()
{
  return -1;
}

int MC6809::neg_integer()
{
  return -1;
}

int MC6809::shift_left_integer()
{
  return -1;
}

int MC6809::shift_left_integer(int num)
{
  return -1;
}

int MC6809::shift_right_integer()
{
  return -1;
}

int MC6809::shift_right_integer(int num)
{
  return -1;
}

int MC6809::shift_right_uinteger()
{
  return -1;
}

int MC6809::shift_right_uinteger(int num)
{
  return -1;
}

int MC6809::and_integer()
{
  return -1;
}

int MC6809::and_integer(int num)
{
  return -1;
}

int MC6809::or_integer()
{
  return -1;
}

int MC6809::or_integer(int num)
{
  return -1;
}

int MC6809::xor_integer()
{
  return -1;
}

int MC6809::xor_integer(int num)
{
  return -1;
}

int MC6809::inc_integer(int index, int num)
{
  return -1;
}

int MC6809::integer_to_byte()
{
  return -1;
}

int MC6809::integer_to_short()
{
  return -1;
}

int MC6809::jump_cond(const char *label, int cond, int distance)
{
  return -1;
}

int MC6809::jump_cond_integer(const char *label, int cond, int distance)
{
  return -1;
}

int MC6809::return_local(int index, int local_count)
{
  return -1;
}

int MC6809::return_integer(int local_count)
{
  return -1;
}

int MC6809::return_void(int local_count)
{
  return -1;
}

int MC6809::jump(const char *name, int distance)
{
  return -1;
}

int MC6809::call(const char *name)
{
  return -1;
}

int MC6809::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int MC6809::put_static(const char *name, int index)
{
  return -1;
}

int MC6809::get_static(const char *name, int index)
{
  return -1;
}

int MC6809::brk()
{
  return -1;
}

int MC6809::new_array(uint8_t type)
{
  return -1;
}

int MC6809::insert_array(const char *name, int32_t *data, int len, uint8_t type)
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

int MC6809::insert_string(const char *name, uint8_t *bytes, int len)
{
  return -1;
}

int MC6809::push_array_length()
{
  return -1;
}

int MC6809::push_array_length(const char *name, int field_id)
{
  return -1;
}

int MC6809::array_read_byte()
{
  return -1;
}

int MC6809::array_read_short()
{
  return -1;
}

int MC6809::array_read_int()
{
  return -1;
}

int MC6809::array_read_byte(const char *name, int field_id)
{
  return -1;
}

int MC6809::array_read_short(const char *name, int field_id)
{
  return -1;
}

int MC6809::array_read_int(const char *name, int field_id)
{
  return -1;
}

int MC6809::array_write_byte()
{
  return -1;
}

int MC6809::array_write_short()
{
  return -1;
}

int MC6809::array_write_int()
{
  return -1;
}

int MC6809::array_write_byte(const char *name, int field_id)
{
  return -1;
}

int MC6809::array_write_short(const char *name, int field_id)
{
  return -1;
}

int MC6809::array_write_int(const char *name, int field_id)
{
  return -1;
}

