/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2020 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "generator/SH4.h"

#define REG_STACK(a) (a + 1)
#define LOCALS(i) (i * 4)

// ABI is:
// r0 is temp, return value
// r1 top of stack
// r2
// r3
// r4
// r5
// r6
// r7
// r8
// r9
// r10 end of stack
// r11 is temp
// r12 is temp
// r13 is temp
// r14 points to locals
// r15 points to constants

SH4::SH4() :
  reg(0),
  reg_max(9),
  stack(0),
  is_main(0)
{

}

SH4::~SH4()
{
}

int SH4::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".sh4\n");

  // Set where RAM starts / ends
  //fprintf(out, "ram_start equ 0\n");
  //fprintf(out, "ram_end equ 0x8000\n");

  return 0;
}

int SH4::finish()
{
  fprintf(out,
    "  ;; If this function is right before the constants pool it should\n"
    "  ;; the displacement should be 0 (PC + 4)\n"
    "_load_constants_pool:\n"
    "  mov.l @(0,PC), r15\n"
    "  rts\n\n");

  insert_constants_pool();

  return 0;
}

int SH4::start_init()
{
  // Add any set up items (stack, registers, etc).
  // Start  0x0c000000
  //   End  0x1c000000
  // Video  0x04000000
  //   End  0x04800000
  fprintf(out, ".org 0x0c000000\n");
  fprintf(out, "start:\n");

  return 0;
}

int SH4::insert_static_field_define(std::string &name, std::string &type, int index)
{
  return -1;
}

int SH4::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");
  //fprintf(out, "  mov #ram_start+%d, &ram_start\n", (field_count + 1) * 2);

  return -1;
}

int SH4::field_init_int(std::string &name, int index, int value)
{
  int c = get_constant(value);

  if (c == -1) { return -1; }

  fprintf(out,
    "  ;; field_init_int(%s,%d,%d)\n"
    "  mov.l @(%d,r15), r15\n"
    "  mov.l r15, @(%d,GBR)\n",
    name.c_str(), index, value,
    c,
    index * 4);

  return 0;
}

int SH4::field_init_ref(std::string &name, int index)
{
  return -1;
}

void SH4::method_start(int local_count, int max_stack, int param_count, std::string &name)
{
}

void SH4::method_end(int local_count)
{
}

int SH4::push_local_var_int(int index)
{
  return -1;
}

int SH4::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int SH4::push_local_var_float(int index)
{
  return -1;
}

int SH4::push_ref_static(std::string &name, int index)
{
  return -1;
}

int SH4::push_fake()
{
  return -1;
}

int SH4::push_int(int32_t n)
{
  int offset = get_constant(n);

  if (offset == -1) { return -1; }

  fprintf(out, "  mov.l (%d, r15), r%d\n", offset * 4, REG_STACK(reg));

  reg++;

  return 0;
}

#if 0
int SH4::push_long(int64_t n)
{
  return -1;
}
#endif

int SH4::push_float(float f)
{
  return -1;
}

#if 0
int SH4::push_double(double f)
{
  return -1;
}
#endif

int SH4::push_ref(std::string &name)
{
  // Need to move the address of name to the top of stack
  return -1;
}

int SH4::pop_local_var_int(int index)
{
  return -1;
}

int SH4::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int SH4::pop_local_var_float(int index)
{
  return -1;
}

int SH4::pop()
{
  reg--;

  return 0;
}

int SH4::dup()
{
  fprintf(out, "  mov r%d, r%d\n", REG_STACK(reg - 1), REG_STACK(reg));

  reg++;

  return 0;
}

int SH4::dup2()
{
  fprintf(out, "  mov r%d, r%d\n", REG_STACK(reg - 2), REG_STACK(reg + 0));
  fprintf(out, "  mov r%d, r%d\n", REG_STACK(reg - 1), REG_STACK(reg + 1));

  reg += 2;

  return 0;
}

int SH4::swap()
{
  fprintf(out,
    "  mov r%d, r0\n"
    "  mov r%d, r%d\n"
    "  mov r0, r%d\n",
    REG_STACK(reg - 2),
    REG_STACK(reg - 1), REG_STACK(reg - 2),
    REG_STACK(reg - 1));

  return 0;
}

int SH4::add_integer()
{
  fprintf(out, "  add r%d, r%d\n", REG_STACK(reg - 1), REG_STACK(reg - 2));

  reg--;

  return 0;
}

int SH4::add_integer(int num)
{
  if (num < -128 || num > 127) { return -1; }

  fprintf(out, "  add #%d, r%d\n", num, REG_STACK(reg - 1));

  return 0;
}

int SH4::sub_integer()
{
  fprintf(out, "  sub r%d, r%d\n", REG_STACK(reg - 1), REG_STACK(reg - 2));

  reg--;

  return 0;
}

int SH4::sub_integer(int num)
{
  // There is no immediate sub instruction, but maybe could change to
  // an add minus?  The DT instruction decrements by 1, so use that if
  // possible.

  if (num == 1)
  {
    fprintf(out, "  dt r%d\n", REG_STACK(reg - 1));
    return 0;
  }


  return -1;
}

int SH4::mul_integer()
{
  fprintf(out,
    "  mul.l r%d, r%d\n"
    "  sts MACL, r%d\n",
    REG_STACK(reg - 1), REG_STACK(reg - 2),
    REG_STACK(reg - 2));

  reg--;

  return 0;
}

int SH4::div_integer()
{
  return -1;
}

int SH4::mod_integer()
{
  return -1;
}

int SH4::neg_integer()
{
  fprintf(out, "  neg r%d, r%d\n", REG_STACK(reg - 1), REG_STACK(reg - 1));

  return 0;
}

int SH4::shift_left_integer()
{
  // SHAD shifts left arithmetically if source reg is positive.
  fprintf(out, "  shad r%d, r%d\n", REG_STACK(reg - 1), REG_STACK(reg - 2));

  reg--;

  return 0;
}

int SH4::shift_left_integer(int num)
{
  return -1;
}

int SH4::shift_right_integer()
{
  // SHAD shifts right arithmetically if source reg is negative.
  fprintf(out, "  neg r%d\n", REG_STACK(reg - 1));
  fprintf(out, "  shad r%d, r%d\n", REG_STACK(reg - 1), REG_STACK(reg - 2));

  reg--;

  return 0;
}

int SH4::shift_right_integer(int num)
{
  return -1;
}

int SH4::shift_right_uinteger()
{
  // SHLD shifts right logically if source reg is negative.
  fprintf(out, "  neg r%d\n", REG_STACK(reg - 1));
  fprintf(out, "  shld r%d, r%d\n", REG_STACK(reg - 1), REG_STACK(reg - 2));

  return 0;
}

int SH4::shift_right_uinteger(int num)
{
  return -1;
}

int SH4::and_integer()
{
  fprintf(out, "  and r%d, r%d\n", REG_STACK(reg - 1), REG_STACK(reg - 2));

  reg--;

  return 0;
}

int SH4::and_integer(int num)
{
  if (num < 0 || num > 255) { return -1; }

  fprintf(out, "  and #%d, r%d\n", num, REG_STACK(reg - 1));

  return 0;
}

int SH4::or_integer()
{
  fprintf(out, "  or r%d, r%d\n", REG_STACK(reg - 1), REG_STACK(reg - 2));

  reg--;

  return 0;
}

int SH4::or_integer(int num)
{
  if (num < 0 || num > 255) { return -1; }

  fprintf(out, "  or #%d, r%d\n", num, REG_STACK(reg - 1));

  return 0;
}

int SH4::xor_integer()
{
  fprintf(out, "  xor r%d, r%d\n", REG_STACK(reg - 1), REG_STACK(reg - 2));

  reg--;

  return 0;
}

int SH4::xor_integer(int num)
{
  if (num < 0 || num > 255) { return -1; }

  fprintf(out, "  xor #%d, r%d\n", num, REG_STACK(reg - 1));

  return 0;
}

int SH4::inc_integer(int index, int num)
{
  fprintf(out, "  ; inc_integer(local_%d,%d)\n", index, num);

  return -1;
}

int SH4::integer_to_byte()
{
  fprintf(out, "  ; integer_to_byte() - sign extend byte to long\n");
  fprintf(out, "  exts.b r%d, r%d\n", REG_STACK(reg - 1), REG_STACK(reg - 1));

  return 0;
}

int SH4::integer_to_short()
{
  fprintf(out, "  ; integer_to_short() - sign extend word to long\n");
  fprintf(out, "  exts.w r%d, r%d\n", REG_STACK(reg - 1), REG_STACK(reg - 1));

  return 0;
}

int SH4::add_float()
{
  return -1;
}

int SH4::sub_float()
{
  return -1;
}

int SH4::mul_float()
{
  return -1;
}

int SH4::div_float()
{
  return -1;
}

int SH4::neg_float()
{
  return -1;
}

int SH4::float_to_integer()
{
  return -1;
}

int SH4::integer_to_float()
{
  return -1;
}

int SH4::jump_cond(std::string &label, int cond, int distance)
{
  fprintf(out, "  ; jump_cond(%s, %d, %d)\n", label.c_str(), cond, distance);

  switch(cond)
  {
    case COND_EQUAL:
      fprintf(out, "  cmp/eq #0, r%d\n", REG_STACK(reg - 1));
      fprintf(out, "  bt %s\n", label.c_str());
      fprintf(out, "  nop\n");
      reg--;
      return 0;
    case COND_NOT_EQUAL:
      fprintf(out, "  cmp/eq #0, r%d\n", REG_STACK(reg - 1));
      fprintf(out, "  bf %s\n", label.c_str());
      fprintf(out, "  nop\n");
      return 0;
    case COND_LESS:
      fprintf(out, "  cmp/ge #0, r%d\n", REG_STACK(reg - 1));
      fprintf(out, "  bf %s\n", label.c_str());
      fprintf(out, "  nop\n");
      return 0;
    case COND_LESS_EQUAL:
      fprintf(out, "  cmp/gt #0, r%d\n", REG_STACK(reg - 1));
      fprintf(out, "  bf %s\n", label.c_str());
      fprintf(out, "  nop\n");
      return 0;
    case COND_GREATER:
      fprintf(out, "  cmp/gt #0, r%d\n", REG_STACK(reg - 1));
      fprintf(out, "  bt %s\n", label.c_str());
      fprintf(out, "  nop\n");
      return 0;
    case COND_GREATER_EQUAL:
      fprintf(out, "  cmp/ge #0, r%d\n", REG_STACK(reg - 1));
      fprintf(out, "  bt %s\n", label.c_str());
      fprintf(out, "  nop\n");
      return 0;
    default:
      break;
  }

  return -1;
}

int SH4::jump_cond_integer(std::string &label, int cond, int distance)
{
  return -1;
}

int SH4::compare_floats(int cond)
{
  return -1;
}

int SH4::ternary(int cond, int value_true, int value_false)
{
  return -1;
}

int SH4::ternary(int cond, int compare, int value_true, int value_false)
{
  return -1;
}

int SH4::return_local(int index, int local_count)
{
  return -1;
}

int SH4::return_integer(int local_count)
{
  return -1;
}

int SH4::return_void(int local_count)
{
  return -1;
}

int SH4::jump(std::string &name, int distance)
{
  return -1;
}

int SH4::call(std::string &name)
{
  return -1;
}

int SH4::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int SH4::put_static(std::string &name, int index)
{
  return -1;
}

int SH4::get_static(std::string &name, int index)
{
  return -1;
}

int SH4::brk()
{
  return -1;
}

int SH4::new_array(uint8_t type)
{
  return -1;
}

int SH4::insert_array(std::string &name, int32_t *data, int len, uint8_t type)
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

int SH4::insert_string(std::string &name, uint8_t *bytes, int len)
{
  return -1;
}

int SH4::push_array_length()
{
  return -1;
}

int SH4::push_array_length(std::string &name, int field_id)
{
  return -1;
}

int SH4::array_read_byte()
{
  return -1;
}

int SH4::array_read_short()
{
  return -1;
}

int SH4::array_read_int()
{
  return -1;
}

int SH4::array_read_float()
{
  return -1;
}

int SH4::array_read_object()
{
  return -1;
}

int SH4::array_read_byte(std::string &name, int field_id)
{
  return -1;
}

int SH4::array_read_short(std::string &name, int field_id)
{
  return -1;
}

int SH4::array_read_int(std::string &name, int field_id)
{
  return -1;
}

int SH4::array_read_float(std::string &name, int field_id)
{
  return -1;
}

int SH4::array_read_object(std::string &name, int field_id)
{
  return -1;
}

int SH4::array_write_byte()
{
  return -1;
}

int SH4::array_write_short()
{
  return -1;
}

int SH4::array_write_int()
{
  return -1;
}

int SH4::array_write_float()
{
  return -1;
}

int SH4::array_write_object()
{
  return -1;
}

int SH4::array_write_byte(std::string &name, int field_id)
{
  return -1;
}

int SH4::array_write_short(std::string &name, int field_id)
{
  return -1;
}

int SH4::array_write_int(std::string &name, int field_id)
{
  return -1;
}

int SH4::array_write_float(std::string &name, int field_id)
{
  return -1;
}

int SH4::array_write_object(std::string &name, int field_id)
{
  return -1;
}

