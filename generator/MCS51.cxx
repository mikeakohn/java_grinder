/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 * MCS-51 (8051) written by Joe Davisson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "MCS51.h"

#define REG_STACK(a) (a)
#define LOCALS(i) (i * 2)

// ABI is:

MCS51::MCS51() :
  reg(0),
  reg_max(9),
  stack(0),
  is_main(0)
{

}

MCS51::~MCS51()
{
}

int MCS51::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  //fprintf(out, ".CPU_PART\n");

  // Set where RAM starts / ends
  //fprintf(out, "ram_start equ 0\n");
  //fprintf(out, "ram_end equ 0x8000\n");
  fprintf(out, ".8051\n");

  fprintf(out, "stack equ 0x20\n");
  fprintf(out, "locals equ r2\n");

  return 0;
}

int MCS51::start_init()
{
  // Add any set up items (stack, registers, etc).
  fprintf(out, ".org 0x0000\n");
  fprintf(out, "start:\n");
  fprintf(out, "  mov SP,#0x20\n");

  return 0;
}

int MCS51::insert_static_field_define(const char *name, const char *type, int index)
{
  return -1;
}

int MCS51::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");
  //fprintf(out, "  mov #ram_start+%d, &ram_start\n", (field_count + 1) * 2);

  return -1;
}

int MCS51::field_init_int(char *name, int index, int value)
{
  return -1;
}

int MCS51::field_init_ref(char *name, int index)
{
  return -1;
}

void MCS51::method_start(int local_count, int max_stack, int param_count, const char *name)
{
  stack = 0;

  is_main = (strcmp(name, "main") == 0) ? 1 : 0;

  fprintf(out, "%s:\n", name);

  // main() function goes here
  if (!is_main)
  {
    fprintf(out, "  push 0x00\n");
    fprintf(out, "  clr A\n");
    fprintf(out, "  push ACC\n");
  }

  fprintf(out, "  mov locals,SP\n");
  fprintf(out, "  mov A,SP\n");
  fprintf(out, "  add A,#0x%02x\n", local_count * 2);
  fprintf(out, "  mov SP,A\n");
}

void MCS51::method_end(int local_count)
{
  fprintf(out, "\n");
}

int MCS51::push_local_var_int(int index)
{
  fprintf(out, "; push_local_var_int\n");
  fprintf(out, "  mov A,locals\n");
  fprintf(out, "  add A,%d\n", LOCALS(index));
  fprintf(out, "  mov r1,A\n");
  fprintf(out, "  mov A,@r1\n");
  fprintf(out, "  push ACC\n");
  fprintf(out, "  inc r1\n");
  fprintf(out, "  mov A,@r1\n");
  fprintf(out, "  push ACC\n");
  stack++;

  return 0;
}

int MCS51::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int MCS51::push_ref_static(const char *name, int index)
{
  return -1;
}

int MCS51::push_fake()
{
  return -1;
}

int MCS51::push_int(int32_t n)
{
  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);

    return -1;
  }

  uint16_t value = (n & 0xffff);

  fprintf(out, "; push_int\n");
  fprintf(out, "  mov A,#0x%02x\n", value & 0xff);
  fprintf(out, "  push ACC\n");
  fprintf(out, "  mov A,#0x%02x\n", value  >> 8);
  fprintf(out, "  push ACC\n");
  stack++;

  return 0;
}

int MCS51::push_long(int64_t n)
{
  return push_int((int32_t)n);
}

int MCS51::push_float(float f)
{
  return -1;
}

int MCS51::push_double(double f)
{
  return -1;
}

int MCS51::push_ref(char *name)
{
  // Need to move the address of name to the top of stack
  return -1;
}

int MCS51::pop_local_var_int(int index)
{
  fprintf(out, "; pop_local_var_int\n");

  fprintf(out, "  pop 0x05\n");
  fprintf(out, "  pop 0x04\n");

  fprintf(out, "  mov A,locals\n");
  fprintf(out, "  add A,%d\n", LOCALS(index));
  fprintf(out, "  mov r1,A\n");
  fprintf(out, "  mov @r1,0x04\n");
  fprintf(out, "  inc r1\n");
  fprintf(out, "  mov @r1,0x05\n");
  stack--;

  return 0;
}

int MCS51::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int MCS51::pop()
{
  return -1;
}

int MCS51::dup()
{
  return -1;
}

int MCS51::dup2()
{
  return -1;
}

int MCS51::swap()
{
  return -1;
}

int MCS51::add_integer()
{
  return -1;
}

int MCS51::add_integer(int num)
{
  return -1;
}

int MCS51::sub_integer()
{
  return -1;
}

int MCS51::sub_integer(int num)
{
  return -1;
}

int MCS51::mul_integer()
{
  return -1;
}

int MCS51::div_integer()
{
  return -1;
}

int MCS51::mod_integer()
{
  return -1;
}

int MCS51::neg_integer()
{
  return -1;
}

int MCS51::shift_left_integer()
{
  return -1;
}

int MCS51::shift_left_integer(int num)
{
  return -1;
}

int MCS51::shift_right_integer()
{
  return -1;
}

int MCS51::shift_right_integer(int num)
{
  return -1;
}

int MCS51::shift_right_uinteger()
{
  return -1;
}

int MCS51::shift_right_uinteger(int num)
{
  return -1;
}

int MCS51::and_integer()
{
  return -1;
}

int MCS51::and_integer(int num)
{
  return -1;
}

int MCS51::or_integer()
{
  return -1;
}

int MCS51::or_integer(int num)
{
  return -1;
}

int MCS51::xor_integer()
{
  return -1;
}

int MCS51::xor_integer(int num)
{
  return -1;
}

int MCS51::inc_integer(int index, int num)
{
  return -1;
}

int MCS51::integer_to_byte()
{
  return -1;
}

int MCS51::integer_to_short()
{
  return -1;
}

int MCS51::jump_cond(const char *label, int cond, int distance)
{
  return -1;
}

int MCS51::jump_cond_integer(const char *label, int cond, int distance)
{
  return -1;
}

int MCS51::ternary(int cond, int value_true, int value_false)
{
  return -1;
}

int MCS51::ternary(int cond, int compare, int value_true, int value_false)
{
  return -1;
}

int MCS51::return_local(int index, int local_count)
{
  return -1;
}

int MCS51::return_integer(int local_count)
{
  return -1;
}

int MCS51::return_void(int local_count)
{
  fprintf(out, "; return void\n");
  fprintf(out, "  mov locals,SP\n");

  if(!is_main)
  {
    fprintf(out, "  pop locals\n");
    fprintf(out, "  ret\n");
  }

  return 0;
}

int MCS51::jump(const char *name, int distance)
{
  return -1;
}

int MCS51::call(const char *name)
{
  return -1;
}

int MCS51::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int MCS51::put_static(const char *name, int index)
{
  return -1;
}

int MCS51::get_static(const char *name, int index)
{
  return -1;
}

int MCS51::brk()
{
  return -1;
}

int MCS51::new_array(uint8_t type)
{
  return -1;
}

int MCS51::insert_array(const char *name, int32_t *data, int len, uint8_t type)
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

int MCS51::insert_string(const char *name, uint8_t *bytes, int len)
{
  return -1;
}

int MCS51::push_array_length()
{
  return -1;
}

int MCS51::push_array_length(const char *name, int field_id)
{
  return -1;
}

int MCS51::array_read_byte()
{
  return -1;
}

int MCS51::array_read_short()
{
  return -1;
}

int MCS51::array_read_int()
{
  return -1;
}

int MCS51::array_read_byte(const char *name, int field_id)
{
  return -1;
}

int MCS51::array_read_short(const char *name, int field_id)
{
  return -1;
}

int MCS51::array_read_int(const char *name, int field_id)
{
  return -1;
}

int MCS51::array_write_byte()
{
  return -1;
}

int MCS51::array_write_short()
{
  return -1;
}

int MCS51::array_write_int()
{
  return -1;
}

int MCS51::array_write_byte(const char *name, int field_id)
{
  return -1;
}

int MCS51::array_write_short(const char *name, int field_id)
{
  return -1;
}

int MCS51::array_write_int(const char *name, int field_id)
{
  return -1;
}

