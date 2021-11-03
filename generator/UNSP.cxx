/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2021 by Michael Kohn
 *
 * UNSP by Simon Eriksson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "generator/UNSP.h"

#define REG_STACK(a) (a)
#define LOCALS(i) (i * 4)

// ABI is:

UNSP::UNSP() :
  reg(0),
  reg_max(9),
  stack(0),
  is_main(false)
{

}

UNSP::~UNSP()
{
}

int UNSP::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".CPU_PART\n");

  // Set where RAM starts / ends
  //fprintf(out, "ram_start equ 0\n");
  //fprintf(out, "ram_end equ 0x8000\n");

  return 0;
}

int UNSP::start_init()
{
  // Add any set up items (stack, registers, etc).
  //fprintf(out, ".org ???\n");
  fprintf(out, "start:\n");

  return 0;
}

int UNSP::insert_static_field_define(
  std::string &name,
  std::string &type,
  int index)
{
  return -1;
}

int UNSP::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");
  //fprintf(out, "  mov #ram_start+%d, &ram_start\n", (field_count + 1) * 2);

  return -1;
}

int UNSP::field_init_int(std::string &name, int index, int value)
{
  return -1;
}

int UNSP::field_init_ref(std::string &name, int index)
{
  return -1;
}

void UNSP::method_start(int local_count, int max_stack, int param_count, std::string &name)
{
}

void UNSP::method_end(int local_count)
{
}

int UNSP::push_local_var_int(int index)
{
  return -1;
}

int UNSP::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int UNSP::push_ref_static(std::string &name, int index)
{
  return -1;
}

int UNSP::push_fake()
{
  return -1;
}

int UNSP::push_int(int32_t n)
{
  return -1;
}

#if 0
int UNSP::push_long(int64_t n)
{
  return -1;
}

int UNSP::push_float(float f)
{
  return -1;
}

int UNSP::push_double(double f)
{
  return -1;
}
#endif

int UNSP::push_ref(std::string &name)
{
  // Need to move the address of name to the top of stack
  return -1;
}

int UNSP::pop_local_var_int(int index)
{
  return -1;
}

int UNSP::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int UNSP::pop()
{
  return -1;
}

int UNSP::dup()
{
  return -1;
}

int UNSP::dup2()
{
  return -1;
}

int UNSP::swap()
{
  return -1;
}

int UNSP::add_integer()
{
  return -1;
}

int UNSP::add_integer(int num)
{
  return -1;
}

int UNSP::sub_integer()
{
  return -1;
}

int UNSP::sub_integer(int num)
{
  return -1;
}

int UNSP::mul_integer()
{
  return -1;
}

int UNSP::div_integer()
{
  return -1;
}

int UNSP::mod_integer()
{
  return -1;
}

int UNSP::neg_integer()
{
  return -1;
}

int UNSP::shift_left_integer()
{
  return -1;
}

int UNSP::shift_left_integer(int num)
{
  return -1;
}

int UNSP::shift_right_integer()
{
  return -1;
}

int UNSP::shift_right_integer(int num)
{
  return -1;
}

int UNSP::shift_right_uinteger()
{
  return -1;
}

int UNSP::shift_right_uinteger(int num)
{
  return -1;
}

int UNSP::and_integer()
{
  return -1;
}

int UNSP::and_integer(int num)
{
  return -1;
}

int UNSP::or_integer()
{
  return -1;
}

int UNSP::or_integer(int num)
{
  return -1;
}

int UNSP::xor_integer()
{
  return -1;
}

int UNSP::xor_integer(int num)
{
  return -1;
}

int UNSP::inc_integer(int index, int num)
{
  return -1;
}

int UNSP::integer_to_byte()
{
  return -1;
}

int UNSP::integer_to_short()
{
  return -1;
}

int UNSP::jump_cond(std::string &label, int cond, int distance)
{
  return -1;
}

#if 0
int UNSP::jump_cond_zero(std::string &label, int cond, int distance)
{
  return -1;
}
#endif

int UNSP::jump_cond_integer(std::string &label, int cond, int distance)
{
  return -1;
}

int UNSP::ternary(int cond, int value_true, int value_false)
{
  return -1;
}

int UNSP::ternary(int cond, int compare, int value_true, int value_false)
{
  return -1;
}

int UNSP::return_local(int index, int local_count)
{
  return -1;
}

int UNSP::return_integer(int local_count)
{
  return -1;
}

int UNSP::return_void(int local_count)
{
  return -1;
}

int UNSP::jump(std::string &name, int distance)
{
  return -1;
}

int UNSP::call(std::string &name)
{
  return -1;
}

int UNSP::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int UNSP::put_static(std::string &name, int index)
{
  return -1;
}

int UNSP::get_static(std::string &name, int index)
{
  return -1;
}

int UNSP::brk()
{
  return -1;
}

int UNSP::new_array(uint8_t type)
{
  return -1;
}

int UNSP::insert_array(std::string &name, int32_t *data, int len, uint8_t type)
{
  if (type == TYPE_BYTE)
  {
    return insert_db(name, data, len, TYPE_INT);
  }
    else
  if (type == TYPE_SHORT)
  {
    return insert_dw(name, data, len, TYPE_INT);
  }
    else
  if (type == TYPE_INT)
  {
    return insert_dc32(name, data, len, TYPE_INT);
  }

  return -1;
}

int UNSP::insert_string(std::string &name, uint8_t *bytes, int len)
{
  return -1;
}

int UNSP::push_array_length()
{
  return -1;
}

int UNSP::push_array_length(std::string &name, int field_id)
{
  return -1;
}

int UNSP::array_read_byte()
{
  return -1;
}

int UNSP::array_read_short()
{
  return -1;
}

int UNSP::array_read_int()
{
  return -1;
}

int UNSP::array_read_byte(std::string &name, int field_id)
{
  return -1;
}

int UNSP::array_read_short(std::string &name, int field_id)
{
  return -1;
}

int UNSP::array_read_int(std::string &name, int field_id)
{
  return -1;
}

int UNSP::array_write_byte()
{
  return -1;
}

int UNSP::array_write_short()
{
  return -1;
}

int UNSP::array_write_int()
{
  return -1;
}

int UNSP::array_write_byte(std::string &name, int field_id)
{
  return -1;
}

int UNSP::array_write_short(std::string &name, int field_id)
{
  return -1;
}

int UNSP::array_write_int(std::string &name, int field_id)
{
  return -1;
}

