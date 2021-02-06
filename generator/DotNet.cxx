/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2021 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "generator/DotNet.h"

#define REG_STACK(a) (a)
#define LOCALS(i) (i * 4)

// ABI is:

DotNet::DotNet() :
  reg(0),
  reg_max(9),
  stack(0),
  is_main(false)
{

}

DotNet::~DotNet()
{
}

int DotNet::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".CPU_PART\n");

  // Set where RAM starts / ends
  //fprintf(out, "ram_start equ 0\n");
  //fprintf(out, "ram_end equ 0x8000\n");

  return 0;
}

int DotNet::start_init()
{
  // Add any set up items (stack, registers, etc).
  //fprintf(out, ".org ???\n");
  fprintf(out, "start:\n");

  return 0;
}

int DotNet::insert_static_field_define(
  std::string &name,
  std::string &type,
  int index)
{
  return -1;
}

int DotNet::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");
  //fprintf(out, "  mov #ram_start+%d, &ram_start\n", (field_count + 1) * 2);

  return -1;
}

int DotNet::field_init_int(std::string &name, int index, int value)
{
  return -1;
}

int DotNet::field_init_ref(std::string &name, int index)
{
  return -1;
}

void DotNet::method_start(
  int local_count,
  int max_stack,
  int param_count,
  std::string &name)
{
}

void DotNet::method_end(int local_count)
{
}

int DotNet::push_local_var_int(int index)
{
  return -1;
}

int DotNet::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int DotNet::push_ref_static(std::string &name, int index)
{
  return -1;
}

int DotNet::push_fake()
{
  return -1;
}

int DotNet::push_int(int32_t n)
{
  if (n >= 0 && n <= 8)
  {
    fprintf(out, "  ldc.i4.%d\n", n);
  }
    else
  if (n == -1)
  {
    fprintf(out, "  ldc.i4.m1\n");
  }
    else
  {
    fprintf(out, "  ldc.i4 %d\n", n);
  }

  return 0;
}

#if 0
int DotNet::push_long(int64_t n)
{
  return -1;
}

int DotNet::push_float(float f)
{
  return -1;
}

int DotNet::push_double(double f)
{
  return -1;
}
#endif

int DotNet::push_ref(std::string &name)
{
  // Need to move the address of name to the top of stack
  return -1;
}

int DotNet::pop_local_var_int(int index)
{
  return -1;
}

int DotNet::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int DotNet::pop()
{
  fprintf(out, "  pop\n");

  return 0;
}

int DotNet::dup()
{
  fprintf(out, "  dup\n");

  return 0;
}

int DotNet::dup2()
{
  return -1;
}

int DotNet::swap()
{
  return -1;
}

int DotNet::add_integer()
{
  fprintf(out, "  add\n");

  return 0;
}

int DotNet::add_integer(int num)
{
  return -1;
}

int DotNet::sub_integer()
{
  fprintf(out, "  sub\n");

  return 0;
}

int DotNet::sub_integer(int num)
{
  return -1;
}

int DotNet::mul_integer()
{
  fprintf(out, "  mul\n");

  return 0;
}

int DotNet::div_integer()
{
  fprintf(out, "  div\n");

  return 0;
}

int DotNet::mod_integer()
{
  fprintf(out, "  rem\n");

  return 0;
}

int DotNet::neg_integer()
{
  fprintf(out, "  neg\n");

  return 0;
}

int DotNet::shift_left_integer()
{
  fprintf(out, "  shl\n");

  return 0;
}

int DotNet::shift_left_integer(int num)
{
  return -1;
}

int DotNet::shift_right_integer()
{
  fprintf(out, "  shr\n");

  return 0;
}

int DotNet::shift_right_integer(int num)
{
  return -1;
}

int DotNet::shift_right_uinteger()
{
  fprintf(out, "  shr.un\n");

  return 0;
}

int DotNet::shift_right_uinteger(int num)
{
  return -1;
}

int DotNet::and_integer()
{
  fprintf(out, "  and\n");

  return 0;
}

int DotNet::and_integer(int num)
{
  return -1;
}

int DotNet::or_integer()
{
  fprintf(out, "  or\n");

  return 0;
}

int DotNet::or_integer(int num)
{
  return -1;
}

int DotNet::xor_integer()
{
  fprintf(out, "  xor\n");

  return 0;
}

int DotNet::xor_integer(int num)
{
  return -1;
}

int DotNet::inc_integer(int index, int num)
{
  return -1;
}

int DotNet::integer_to_byte()
{
  fprintf(out, "  conv.i1\n");

  return 0;
}

int DotNet::integer_to_short()
{
  fprintf(out, "  conv.i2\n");

  return 0;
}

int DotNet::jump_cond(std::string &label, int cond, int distance)
{
  return -1;
}

int DotNet::jump_cond_integer(std::string &label, int cond, int distance)
{
  return -1;
}

int DotNet::ternary(int cond, int value_true, int value_false)
{
  return -1;
}

int DotNet::ternary(int cond, int compare, int value_true, int value_false)
{
  return -1;
}

int DotNet::return_local(int index, int local_count)
{
  return -1;
}

int DotNet::return_integer(int local_count)
{
  return -1;
}

int DotNet::return_void(int local_count)
{
  return -1;
}

int DotNet::jump(std::string &name, int distance)
{
  return -1;
}

int DotNet::call(std::string &name)
{
  return -1;
}

int DotNet::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int DotNet::put_static(std::string &name, int index)
{
  return -1;
}

int DotNet::get_static(std::string &name, int index)
{
  return -1;
}

int DotNet::brk()
{
  return -1;
}

int DotNet::new_array(uint8_t type)
{
  return -1;
}

int DotNet::insert_array(std::string &name, int32_t *data, int len, uint8_t type)
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

int DotNet::insert_string(std::string &name, uint8_t *bytes, int len)
{
  return -1;
}

int DotNet::push_array_length()
{
  return -1;
}

int DotNet::push_array_length(std::string &name, int field_id)
{
  return -1;
}

int DotNet::array_read_byte()
{
  return -1;
}

int DotNet::array_read_short()
{
  return -1;
}

int DotNet::array_read_int()
{
  return -1;
}

int DotNet::array_read_byte(std::string &name, int field_id)
{
  return -1;
}

int DotNet::array_read_short(std::string &name, int field_id)
{
  return -1;
}

int DotNet::array_read_int(std::string &name, int field_id)
{
  return -1;
}

int DotNet::array_write_byte()
{
  return -1;
}

int DotNet::array_write_short()
{
  return -1;
}

int DotNet::array_write_int()
{
  return -1;
}

int DotNet::array_write_byte(std::string &name, int field_id)
{
  return -1;
}

int DotNet::array_write_short(std::string &name, int field_id)
{
  return -1;
}

int DotNet::array_write_int(std::string &name, int field_id)
{
  return -1;
}

