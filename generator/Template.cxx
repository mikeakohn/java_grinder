/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2019 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "generator/Template.h"

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

int Template::insert_static_field_define(std::string &name, std::string &type, int index)
{
  return -1;
}

int Template::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");
  //fprintf(out, "  mov #ram_start+%d, &ram_start\n", (field_count + 1) * 2);

  return -1;
}

int Template::field_init_int(std::string &name, int index, int value)
{
  return -1;
}

int Template::field_init_ref(std::string &name, int index)
{
  return -1;
}

void Template::method_start(int local_count, int max_stack, int param_count, std::string &name)
{
}

void Template::method_end(int local_count)
{
}

int Template::push_local_var_int(int index)
{
  return -1;
}

int Template::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int Template::push_ref_static(std::string &name, int index)
{
  return -1;
}

int Template::push_fake()
{
  return -1;
}

int Template::push_int(int32_t n)
{
  return -1;
}

#if 0
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
#endif

int Template::push_ref(std::string &name)
{
  // Need to move the address of name to the top of stack
  return -1;
}

int Template::pop_local_var_int(int index)
{
  return -1;
}

int Template::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
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

int Template::jump_cond(std::string &label, int cond, int distance)
{
  return -1;
}

int Template::jump_cond_integer(std::string &label, int cond, int distance)
{
  return -1;
}

int Template::ternary(int cond, int value_true, int value_false)
{
  return -1;
}

int Template::ternary(int cond, int compare, int value_true, int value_false)
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

int Template::jump(std::string &name, int distance)
{
  return -1;
}

int Template::call(std::string &name)
{
  return -1;
}

int Template::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int Template::put_static(std::string &name, int index)
{
  return -1;
}

int Template::get_static(std::string &name, int index)
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

int Template::insert_array(std::string &name, int32_t *data, int len, uint8_t type)
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

int Template::insert_string(std::string &name, uint8_t *bytes, int len)
{
  return -1;
}

int Template::push_array_length()
{
  return -1;
}

int Template::push_array_length(std::string &name, int field_id)
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

int Template::array_read_byte(std::string &name, int field_id)
{
  return -1;
}

int Template::array_read_short(std::string &name, int field_id)
{
  return -1;
}

int Template::array_read_int(std::string &name, int field_id)
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

int Template::array_write_byte(std::string &name, int field_id)
{
  return -1;
}

int Template::array_write_short(std::string &name, int field_id)
{
  return -1;
}

int Template::array_write_int(std::string &name, int field_id)
{
  return -1;
}

