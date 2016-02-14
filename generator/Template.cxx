/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "Template.h"

#define REG_STACK(a) (a)
#define LOCALS(i) (i * 4)

// ABI is:

Template::Template() :
  reg(0),
  reg_max(9),
  stack(0),
  is_main(0)
{

}

Template::~Template()
{
}

int Template::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".CPU_PART\n");

  // Set where RAM starts / ends
  //fprintf(out, "ram_start equ 0\n");
  //fprintf(out, "ram_end equ 0x8000\n");

  return 0;
}

int Template::start_init()
{
  // Add any set up items (stack, registers, etc).
  //fprintf(out, ".org ???\n");
  fprintf(out, "start:\n");

  return 0;
}

int Template::insert_static_field_define(const char *name, const char *type, int index)
{
  return -1;
}

int Template::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");
  //fprintf(out, "  mov #ram_start+%d, &ram_start\n", (field_count + 1) * 2);

  return -1;
}

int Template::field_init_int(char *name, int index, int value)
{
  return -1;
}

int Template::field_init_ref(char *name, int index)
{
  return -1;
}

void Template::method_start(int local_count, int max_stack, int param_count, const char *name)
{
}

void Template::method_end(int local_count)
{
}

int Template::push_integer(int32_t n)
{
  return -1;
}

int Template::push_integer_local(int index)
{
  return -1;
}

int Template::push_ref_static(const char *name, int index)
{
  return -1;
}

int Template::push_ref_local(int index)
{
  return push_integer_local(index);
}

int Template::push_fake()
{
  return -1;
}

int Template::push_long(int64_t n)
{
  return -1;
}

int Template::push_float(float f)
{
  return -1;
}

int Template::push_double(double f)
{
  return -1;
}

int Template::push_byte(int8_t b)
{
  int32_t value = (int32_t)b;

  return push_integer(value);
}

int Template::push_short(int16_t s)
{
  int32_t value = (int32_t)s;

  return push_integer(value);
}

int Template::push_ref(char *name)
{
  // Need to move the address of name to the top of stack
  return -1;
}

int Template::pop_integer_local(int index)
{
  return -1;
}

int Template::pop_ref_local(int index)
{
  return pop_integer_local(index);
}

int Template::pop()
{
  return -1;
}

int Template::dup()
{
  return -1;
}

int Template::dup2()
{
  return -1;
}

int Template::swap()
{
  return -1;
}

int Template::add_integer()
{
  return -1;
}

int Template::add_integer(int num)
{
  return -1;
}

int Template::sub_integer()
{
  return -1;
}

int Template::sub_integer(int num)
{
  return -1;
}

int Template::mul_integer()
{
  return -1;
}

int Template::div_integer()
{
  return -1;
}

int Template::mod_integer()
{
  return -1;
}

int Template::neg_integer()
{
  return -1;
}

int Template::shift_left_integer()
{
  return -1;
}

int Template::shift_left_integer(int num)
{
  return -1;
}

int Template::shift_right_integer()
{
  return -1;
}

int Template::shift_right_integer(int num)
{
  return -1;
}

int Template::shift_right_uinteger()
{
  return -1;
}

int Template::shift_right_uinteger(int num)
{
  return -1;
}

int Template::and_integer()
{
  return -1;
}

int Template::and_integer(int num)
{
  return -1;
}

int Template::or_integer()
{
  return -1;
}

int Template::or_integer(int num)
{
  return -1;
}

int Template::xor_integer()
{
  return -1;
}

int Template::xor_integer(int num)
{
  return -1;
}

int Template::inc_integer(int index, int num)
{
  return -1;
}

int Template::integer_to_byte()
{
  return -1;
}

int Template::integer_to_short()
{
  return -1;
}

int Template::jump_cond(const char *label, int cond, int distance)
{
  return -1;
}

int Template::jump_cond_integer(const char *label, int cond, int distance)
{
  return -1;
}

int Template::return_local(int index, int local_count)
{
  return -1;
}

int Template::return_integer(int local_count)
{
  return -1;
}

int Template::return_void(int local_count)
{
  return -1;
}

int Template::jump(const char *name, int distance)
{
  return -1;
}

int Template::call(const char *name)
{
  return -1;
}

int Template::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int Template::put_static(const char *name, int index)
{
  return -1;
}

int Template::get_static(const char *name, int index)
{
  return -1;
}

int Template::brk()
{
  return -1;
}

int Template::new_array(uint8_t type)
{
  return -1;
}

int Template::insert_array(const char *name, int32_t *data, int len, uint8_t type)
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

int Template::insert_string(const char *name, uint8_t *bytes, int len)
{
  return -1;
}

int Template::push_array_length()
{
  return -1;
}

int Template::push_array_length(const char *name, int field_id)
{
  return -1;
}

int Template::array_read_byte()
{
  return -1;
}

int Template::array_read_short()
{
  return -1;
}

int Template::array_read_int()
{
  return -1;
}

int Template::array_read_byte(const char *name, int field_id)
{
  return -1;
}

int Template::array_read_short(const char *name, int field_id)
{
  return -1;
}

int Template::array_read_int(const char *name, int field_id)
{
  return -1;
}

int Template::array_write_byte()
{
  return -1;
}

int Template::array_write_short()
{
  return -1;
}

int Template::array_write_int()
{
  return -1;
}

int Template::array_write_byte(const char *name, int field_id)
{
  return -1;
}

int Template::array_write_short(const char *name, int field_id)
{
  return -1;
}

int Template::array_write_int(const char *name, int field_id)
{
  return -1;
}

