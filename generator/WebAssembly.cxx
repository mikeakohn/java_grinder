/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2023 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "generator/WebAssembly.h"

WebAssembly::WebAssembly() :
  reg(0),
  reg_max(9),
  stack(0),
  is_main(false)
{

}

WebAssembly::~WebAssembly()
{
}

int WebAssembly::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".webasm\n");

  return 0;
}

int WebAssembly::start_init()
{
  // Add any set up items (stack, registers, etc).
  //fprintf(out, ".org ???\n");
  fprintf(out, "start:\n");

  return 0;
}

int WebAssembly::insert_static_field_define(std::string &name, std::string &type, int index)
{
  return -1;
}

int WebAssembly::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");
  //fprintf(out, "  mov #ram_start+%d, &ram_start\n", (field_count + 1) * 2);

  return -1;
}

int WebAssembly::field_init_int(std::string &name, int index, int value)
{
  return -1;
}

int WebAssembly::field_init_ref(std::string &name, int index)
{
  return -1;
}

void WebAssembly::method_start(int local_count, int max_stack, int param_count, std::string &name)
{
}

void WebAssembly::method_end(int local_count)
{
}

int WebAssembly::push_local_var_int(int index)
{
  return -1;
}

int WebAssembly::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int WebAssembly::push_ref_static(std::string &name, int index)
{
  return -1;
}

int WebAssembly::push_fake()
{
  return -1;
}

int WebAssembly::push_int(int32_t n)
{
  return -1;
}

#if 0
int WebAssembly::push_long(int64_t n)
{
  return -1;
}

int WebAssembly::push_float(float f)
{
  return -1;
}

int WebAssembly::push_double(double f)
{
  return -1;
}
#endif

int WebAssembly::push_ref(std::string &name, int index)
{
  // Need to move the address of name to the top of stack
  return -1;
}

int WebAssembly::pop_local_var_int(int index)
{
  return -1;
}

int WebAssembly::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int WebAssembly::pop()
{
  fprintf(out, "  drop\n");

  return 0;
}

int WebAssembly::dup()
{
  return -1;
}

int WebAssembly::dup2()
{
  return -1;
}

int WebAssembly::swap()
{
  return -1;
}

int WebAssembly::add_integer()
{
  return -1;
}

int WebAssembly::add_integer(int num)
{
  fprintf(out, "  i32.add\n");

  return 0;
}

int WebAssembly::sub_integer()
{
  fprintf(out, "  i32.sub\n");

  return 0;
}

int WebAssembly::sub_integer(int num)
{
  return -1;
}

int WebAssembly::mul_integer()
{
  fprintf(out, "  i32.mul\n");

  return 0;
}

int WebAssembly::div_integer()
{
  fprintf(out, "  i32.div_s\n");

  return 0;
}

int WebAssembly::mod_integer()
{
  fprintf(out, "  i32.rem_s\n");

  return 0;
}

int WebAssembly::neg_integer()
{
  fprintf(out, "  i32.neg\n");

  return 0;
}

int WebAssembly::shift_left_integer()
{
  fprintf(out, "  i32.shl\n");

  return 0;
}

int WebAssembly::shift_left_integer(int num)
{
  return -1;
}

int WebAssembly::shift_right_integer()
{
  fprintf(out, "  i32.shr_s\n");

  return 0;
}

int WebAssembly::shift_right_integer(int num)
{
  return -1;
}

int WebAssembly::shift_right_uinteger()
{
  fprintf(out, "  i32.shr_u\n");

  return 0;
}

int WebAssembly::shift_right_uinteger(int num)
{
  return -1;
}

int WebAssembly::and_integer()
{
  fprintf(out, "  i32.and\n");

  return 0;
}

int WebAssembly::and_integer(int num)
{
  return -1;
}

int WebAssembly::or_integer()
{
  fprintf(out, "  i32.or\n");

  return 0;
}

int WebAssembly::or_integer(int num)
{
  return -1;
}

int WebAssembly::xor_integer()
{
  fprintf(out, "  i32.xor\n");

  return 0;
}

int WebAssembly::xor_integer(int num)
{
  return -1;
}

int WebAssembly::inc_integer(int index, int num)
{
  return -1;
}

int WebAssembly::integer_to_byte()
{
  fprintf(out, "  i32.load8_u 0xff\n");
  fprintf(out, "  i32.xor\n");

  return 0;
}

int WebAssembly::integer_to_short()
{
  fprintf(out, "  i32.load16_u 0xffff\n");
  fprintf(out, "  i32.xor\n");

  return 0;
}

int WebAssembly::jump_cond(std::string &label, int cond, int distance)
{
  return -1;
}

int WebAssembly::jump_cond_integer(std::string &label, int cond, int distance)
{
  return -1;
}

int WebAssembly::ternary(int cond, int value_true, int value_false)
{
  return -1;
}

int WebAssembly::ternary(int cond, int compare, int value_true, int value_false)
{
  return -1;
}

int WebAssembly::return_local(int index, int local_count)
{
  return -1;
}

int WebAssembly::return_integer(int local_count)
{
  return -1;
}

int WebAssembly::return_void(int local_count)
{
  return -1;
}

int WebAssembly::jump(std::string &name, int distance)
{
  return -1;
}

int WebAssembly::call(std::string &name)
{
  return -1;
}

int WebAssembly::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int WebAssembly::put_static(std::string &name, int index)
{
  return -1;
}

int WebAssembly::get_static(std::string &name, int index)
{
  return -1;
}

int WebAssembly::brk()
{
  return -1;
}

int WebAssembly::new_array(uint8_t type)
{
  return -1;
}

int WebAssembly::insert_array(std::string &name, int32_t *data, int len, uint8_t type)
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

int WebAssembly::insert_string(std::string &name, uint8_t *bytes, int len)
{
  return -1;
}

int WebAssembly::push_array_length()
{
  return -1;
}

int WebAssembly::push_array_length(std::string &name, int field_id)
{
  return -1;
}

int WebAssembly::array_read_byte()
{
  return -1;
}

int WebAssembly::array_read_short()
{
  return -1;
}

int WebAssembly::array_read_int()
{
  return -1;
}

int WebAssembly::array_read_byte(std::string &name, int field_id)
{
  return -1;
}

int WebAssembly::array_read_short(std::string &name, int field_id)
{
  return -1;
}

int WebAssembly::array_read_int(std::string &name, int field_id)
{
  return -1;
}

int WebAssembly::array_write_byte()
{
  return -1;
}

int WebAssembly::array_write_short()
{
  return -1;
}

int WebAssembly::array_write_int()
{
  return -1;
}

int WebAssembly::array_write_byte(std::string &name, int field_id)
{
  return -1;
}

int WebAssembly::array_write_short(std::string &name, int field_id)
{
  return -1;
}

int WebAssembly::array_write_int(std::string &name, int field_id)
{
  return -1;
}

