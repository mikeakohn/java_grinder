/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "TMS9900.h"

#define REG_STACK(a) (a+2)
#define LOCALS(i) (i * 4)
#define CHECK_STACK() \
  if (reg >= reg_max) { printf("Internal error: Stack blown.\n"); return -1; }

// ABI is:
// r2 start of stack
// ..
// ..
// r10 end of stack
// r11 return address
// r12 CRU base address
// r13 Saved WP register
// r14 Saved PC register
// r15 Saved ST register

TMS9900::TMS9900() :
  reg(0),
  reg_max(9),
  stack(0),
  is_main(0)
{

}

TMS9900::~TMS9900()
{

}

int TMS9900::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  // For now we only support a specific chip
  fprintf(out, ".mips\n");

  // Set where RAM starts / ends
  // FIXME - Not sure what to set this to right now
  fprintf(out, "ram_start equ 0\n");

  return 0;
}

int TMS9900::start_init()
{
  // Add any set up items (stack, registers, etc).
  //fprintf(out, ".org ???\n");
  fprintf(out, "start:\n");

  return 0;
}

int TMS9900::insert_static_field_define(const char *name, const char *type, int index)
{
  fprintf(out, "%s equ ram_start+%d\n", name, (index + 1) * 4);
  return 0;
}


int TMS9900::init_heap(int field_count)
{
  return -1;
}

int TMS9900::insert_field_init_boolean(char *name, int index, int value)
{
  return -1;
}

int TMS9900::insert_field_init_byte(char *name, int index, int value)
{
  return -1;
}

int TMS9900::insert_field_init_short(char *name, int index, int value)
{
  return -1;
}

int TMS9900::insert_field_init_int(char *name, int index, int value)
{
  return -1;
}

int TMS9900::insert_field_init(char *name, int index)
{
  return -1;
}

void TMS9900::method_start(int local_count, int max_stack, int param_count, const char *name)
{
}

void TMS9900::method_end(int local_count)
{
}

int TMS9900::push_integer(int32_t n)
{
  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);
    return -1;
  }

  uint16_t value = (n & 0xffff);

  CHECK_STACK();

  fprintf(out, "  mov #%d, r%d\n", value, REG_STACK(reg));
  reg++;
  return -1;
}

int TMS9900::push_integer_local(int index)
{
  return -1;
}

int TMS9900::push_ref_local(int index)
{
  return push_integer_local(index);
}

int TMS9900::push_fake()
{
  reg++;
  return 0;
}

int TMS9900::push_long(int64_t n)
{
  return -1;
}

int TMS9900::push_float(float f)
{
  return -1;
}

int TMS9900::push_double(double f)
{
  return -1;
}

int TMS9900::push_byte(int8_t b)
{
  int16_t n = b;
  uint16_t value = (n & 0xffff);

  CHECK_STACK();

  fprintf(out, "  mov #0x%02x, r%d\n", value, REG_STACK(reg));
  reg++;

  return 0;
}

int TMS9900::push_short(int16_t s)
{
  uint16_t value = (s & 0xffff);

  CHECK_STACK();

  fprintf(out, "  mov #0x%02x, r%d\n", value, REG_STACK(reg));
  reg++;

  return 0;
}

int TMS9900::pop_integer_local(int index)
{
  return -1;
}

int TMS9900::pop_ref_local(int index)
{
  return pop_integer_local(index);
}

int TMS9900::pop()
{
  reg--;
  return 0;
}

int TMS9900::dup()
{
  CHECK_STACK();

  fprintf(out, "  mov r%d, r0\n", REG_STACK(reg-1));
  fprintf(out, "  mov r0, r%d\n", REG_STACK(reg));
  reg++;

  return 0;
}

int TMS9900::dup2()
{
  fprintf(out, "  mov r%d, r0\n", REG_STACK(reg-2));
  fprintf(out, "  mov r%d, r1\n", REG_STACK(reg-1));

  CHECK_STACK();
  fprintf(out, "  mov r0, r%d\n", REG_STACK(reg));
  reg++;

  CHECK_STACK();
  fprintf(out, "  mov r1, r%d\n", REG_STACK(reg));
  reg++;

  return 0;
}

int TMS9900::swap()
{
  fprintf(out, "  mov r%d, r0\n", REG_STACK(reg-1));
  fprintf(out, "  mov r%d, r%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
  fprintf(out, "  mov r0, r%d\n", REG_STACK(reg-2));
  return 0;
}

int TMS9900::add_integer()
{
  return stack_alu("a"); 
}

int TMS9900::add_integer(int num)
{
  if (num < -32768 || num > 0xffff) { return -1; }

  fprintf(out, "  a #%d, %d\n", num, REG_STACK(reg-1));
  return 0;
}

int TMS9900::sub_integer()
{
  return stack_alu("s"); 
}

int TMS9900::sub_integer(int num)
{
  if (num < -32768 || num > 0xffff) { return -1; }

  fprintf(out, "  s #%d, %d\n", num, REG_STACK(reg-1));
  return 0;
}

int TMS9900::mul_integer()
{
  return -1;
}

int TMS9900::div_integer()
{
  return -1;
}

int TMS9900::mod_integer()
{
  return -1;
}

int TMS9900::neg_integer()
{
  return -1;
}

int TMS9900::shift_left_integer()
{
  return -1;
}

int TMS9900::shift_left_integer(int num)
{
  return -1;
}

int TMS9900::shift_right_integer()
{
  return -1;
}

int TMS9900::shift_right_integer(int num)
{
  return -1;
}

int TMS9900::shift_right_uinteger()
{
  return -1;
}

int TMS9900::shift_right_uinteger(int num)
{
  return -1;
}

int TMS9900::and_integer()
{
  // :(
  return -1;
}

int TMS9900::and_integer(int num)
{
  if (num < -32768 || num > 0xffff) { return -1; }
  fprintf(out, "  andi #%d, %d\n", num, REG_STACK(reg-1));
  return 0;
}

int TMS9900::or_integer()
{
  // :(
  return -1;
}

int TMS9900::or_integer(int num)
{
  if (num < -32768 || num > 0xffff) { return -1; }
  fprintf(out, "  ori #%d, %d\n", num, REG_STACK(reg-1));
  return 0;
}

int TMS9900::xor_integer()
{
  return stack_alu("xor"); 
}

int TMS9900::xor_integer(int num)
{
  if (stack != 0) { return -1; }
  if (num < 0 || num > 0xffff) { return -1; }
  fprintf(out, "  xori $t%d, $t%d, %d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int TMS9900::inc_integer(int index, int num)
{
  return -1;
}

int TMS9900::integer_to_byte()
{
  return -1;
}

int TMS9900::jump_cond(const char *label, int cond)
{
  return -1;
}

int TMS9900::jump_cond_integer(const char *label, int cond)
{
  return -1;
}

int TMS9900::return_local(int index, int local_count)
{
  return -1;
}

int TMS9900::return_integer(int local_count)
{
  return -1;
}

int TMS9900::return_void(int local_count)
{
  return -1;
}

int TMS9900::jump(const char *name)
{
  return -1;
}

int TMS9900::call(const char *name)
{
  return -1;
}

int TMS9900::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int TMS9900::put_static(const char *name, int index)
{
  return -1;
}

int TMS9900::get_static(const char *name, int index)
{
  return -1;
}

int TMS9900::brk()
{
  return -1;
}

int TMS9900::new_array(uint8_t type)
{
  return -1;
}

int TMS9900::insert_array(const char *name, int32_t *data, int len, uint8_t type)
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

int TMS9900::insert_string(const char *name, uint8_t *bytes, int len)
{
  fprintf(out, ".align 32\n");
  fprintf(out, "  dc32 %d\n", len);
  return insert_utf8(name, bytes, len);
}

int TMS9900::push_array_length()
{
  return -1;
}

int TMS9900::push_array_length(const char *name, int field_id)
{
  return -1;
}

int TMS9900::array_read_byte()
{
  return -1;
}

int TMS9900::array_read_short()
{
  return -1;
}

int TMS9900::array_read_int()
{
  return -1;
}

int TMS9900::array_read_byte(const char *name, int field_id)
{
  return -1;
}

int TMS9900::array_read_short(const char *name, int field_id)
{
  return -1;
}

int TMS9900::array_read_int(const char *name, int field_id)
{
  return -1;
}

int TMS9900::array_write_byte()
{
  return -1;
}

int TMS9900::array_write_short()
{
  return -1;
}

int TMS9900::array_write_int()
{
  return -1;
}

int TMS9900::array_write_byte(const char *name, int field_id)
{
  return -1;
}

int TMS9900::array_write_short(const char *name, int field_id)
{
  return -1;
}

int TMS9900::array_write_int(const char *name, int field_id)
{
  return -1;
}

int TMS9900::stack_alu(const char *instr)
{
  if (stack == 0)
  {
    fprintf(out, "  %s $t%d, $t%d, $t%d\n", instr, REG_STACK(reg-2), REG_STACK(reg-2), REG_STACK(reg-1));
    reg--;
  }
    else
  if (stack == 1)
  {
    // FIXME
    fprintf(out, "  error r15\n");
    fprintf(out, "  %s.w r15, r%d\n", instr, REG_STACK(reg-1));
    stack--;
  }
    else
  {
    // FIXME
    fprintf(out, "  error r15\n");
    fprintf(out, "  %s.w r15, @SP\n", instr);
  }

  return 0;
}


