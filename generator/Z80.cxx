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

#include "Z80.h"

#define REG_STACK(a) (stack_regs[a])
#define LOCALS(i) (i * 4)

// ABI is:
// bc - start of stack
// de
// af
// hl - end of stack

//static const char *cond_str[] = { "z", "nz", "lt", "le", "gt", "ge" };
static const char *stack_regs[] = { "bc", "de", "af", "hl" };

Z80::Z80() :
  reg(0),
  reg_max(4),
  stack(0),
  is_main(0)
{

}

Z80::~Z80()
{

}

int Z80::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".z80\n");

  // Set where RAM starts / ends
  // FIXME - Not sure what to set this to right now
  //fprintf(out, "ram_start equ 0\n");
  //fprintf(out, "ram_end equ 0x8000\n");

  return 0;
}

int Z80::start_init()
{
  // Add any set up items (stack, registers, etc).
  //fprintf(out, ".org ???\n");
  fprintf(out, "start:\n");

  return 0;
}

int Z80::insert_static_field_define(const char *name, const char *type, int index)
{
  //fprintf(out, "%s equ ram_start+%d\n", name, (index + 1) * 4);
  return 0;
}


int Z80::init_heap(int field_count)
{
  return -1;
}

int Z80::insert_field_init_boolean(char *name, int index, int value)
{
  return -1;
}

int Z80::insert_field_init_byte(char *name, int index, int value)
{
  return -1;
}

int Z80::insert_field_init_short(char *name, int index, int value)
{
  return -1;
}

int Z80::insert_field_init_int(char *name, int index, int value)
{
  return -1;
}

int Z80::insert_field_init(char *name, int index)
{
  return -1;
}

void Z80::method_start(int local_count, int max_stack, int param_count, const char *name)
{
}

void Z80::method_end(int local_count)
{
}

int Z80::push_integer(int32_t n)
{
  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);
    return -1;
  }

  uint16_t value = (n & 0xffff);

  if (reg < reg_max)
  {
    fprintf(out, "  ld %s, 0x%4x\n", REG_STACK(reg), value);
    reg++;
  }
    else
  {
    fprintf(out, "  ld ix, 0x%4x\n", value);
    fprintf(out, "  push ix\n");
    stack++;
  }

  return 0;
}

int Z80::push_integer_local(int index)
{
  return -1;
}

int Z80::push_ref_local(int index)
{
  return push_integer_local(index);
}

int Z80::push_fake()
{
  if (stack != 0) { return -1; }
  reg++;
  return 0;
}

int Z80::push_long(int64_t n)
{
  return -1;
}

int Z80::push_float(float f)
{
  return -1;
}

int Z80::push_double(double f)
{
  return -1;
}

int Z80::push_byte(int8_t b)
{
  uint16_t value = (((int16_t)b) & 0xffff);

  if (reg < reg_max)
  {
    fprintf(out, "  ld %s, 0x%4x\n", REG_STACK(reg), value);
    reg++;
  }
    else
  {
    fprintf(out, "  ld ix, 0x%4x\n", value);
    fprintf(out, "  push ix\n");
    stack++;
  }

  return 0;
}

int Z80::push_short(int16_t s)
{
  if (reg < reg_max)
  {
    fprintf(out, "  ld %s, 0x%4x\n", REG_STACK(reg), s);
    reg++;
  }
    else
  {
    fprintf(out, "  ld ix, 0x%4x\n", s);
    fprintf(out, "  push ix\n");
    stack++;
  }

  return 0;
}

int Z80::pop_integer_local(int index)
{
  return -1;
}

int Z80::pop_ref_local(int index)
{
  return pop_integer_local(index);
}

int Z80::pop()
{
  if (stack > 0)
  {
    fprintf(out, "  pop ix\n");
    stack--;
  }
    else
  {
    reg--;
  }

  return 0;
}

int Z80::dup()
{
  if (stack > 0)
  {
    // FIXME - I think this could be done better
    fprintf(out, "  pop ix\n");
    fprintf(out, "  push ix\n");
    fprintf(out, "  push ix\n");
    stack++;
  }
    else
  if (reg == reg_max)
  {
    fprintf(out, "  push hl\n");
    stack++;
  }
    else
  {
    fprintf(out, "  ld %s, %s\n", REG_STACK(reg), REG_STACK(reg-1));
    reg++;
  }
  return 0;
}

int Z80::dup2()
{
  return -1;
}

int Z80::swap()
{
  if (stack > 1)
  {
    // FIXME - This could be done a little faster
    fprintf(out, "  pop ix\n");
    fprintf(out, "  pop iy\n");
    fprintf(out, "  push iy\n");
    fprintf(out, "  push ix\n");
    stack++;
  }
    else
  if (stack > 0)
  {
    fprintf(out, "  pop ix\n");
    fprintf(out, "  pop hl\n");
    fprintf(out, "  ld hl, ix\n");
  }
    else
  {
    fprintf(out, "  ld ix, %s\n", REG_STACK(reg-1));
    fprintf(out, "  ld %s, %s\n", REG_STACK(reg-1), REG_STACK(reg-2));
    fprintf(out, "  ld %s, ix\n", REG_STACK(reg-2));
  }
  return 0;
}

int Z80::add_integer()
{
  return stack_alu("add");
}

int Z80::add_integer(int num)
{
  return stack_alu_const("add", num);
}

int Z80::sub_integer()
{
  return stack_alu("sub");
}

int Z80::sub_integer(int num)
{
  return stack_alu_const("sub", num);
}

int Z80::mul_integer()
{
  return -1;
}

int Z80::div_integer()
{
  return -1;
}

int Z80::mod_integer()
{
  return -1;
}

int Z80::neg_integer()
{
  return -1;
}

int Z80::shift_left_integer()
{
  return -1;
}

int Z80::shift_left_integer(int num)
{
  return -1;
}

int Z80::shift_right_integer()
{
  return -1;
}

int Z80::shift_right_integer(int num)
{
  return -1;
}

int Z80::shift_right_uinteger()
{
  return -1;
}

int Z80::shift_right_uinteger(int num)
{
  return -1;
}

int Z80::and_integer()
{
  return stack_alu("and");
}

int Z80::and_integer(int num)
{
  return stack_alu_const("and", num);
}

int Z80::or_integer()
{
  return stack_alu("or");
}

int Z80::or_integer(int num)
{
  return stack_alu_const("or", num);
}

int Z80::xor_integer()
{
  return stack_alu("xor");
}

int Z80::xor_integer(int num)
{
  return stack_alu_const("xor", num);
}

int Z80::inc_integer(int index, int num)
{
  return -1;
}

int Z80::integer_to_byte()
{
  return -1;
}

int Z80::jump_cond(const char *label, int cond)
{
  return -1;
}

int Z80::jump_cond_integer(const char *label, int cond)
{
  return -1;
}

int Z80::return_local(int index, int local_count)
{
  return -1;
}

int Z80::return_integer(int local_count)
{
  return -1;
}

int Z80::return_void(int local_count)
{
  return -1;
}

int Z80::jump(const char *name)
{
  return -1;
}

int Z80::call(const char *name)
{
  return -1;
}

int Z80::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int Z80::put_static(const char *name, int index)
{
  return -1;
}

int Z80::get_static(const char *name, int index)
{
  return -1;
}

int Z80::brk()
{
  return -1;
}

int Z80::new_array(uint8_t type)
{
  return -1;
}

int Z80::insert_array(const char *name, int32_t *data, int len, uint8_t type)
{
  fprintf(out, ".align 16\n");
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

int Z80::insert_string(const char *name, uint8_t *bytes, int len)
{
  fprintf(out, ".align 16\n");
  fprintf(out, "  dc16 %d\n", len);
  return insert_utf8(name, bytes, len);
}

int Z80::push_array_length()
{
  return -1;
}

int Z80::push_array_length(const char *name, int field_id)
{
  return -1;
}

int Z80::array_read_byte()
{
  return -1;
}

int Z80::array_read_short()
{
  return -1;
}

int Z80::array_read_int()
{
  return -1;
}

int Z80::array_read_byte(const char *name, int field_id)
{
  return -1;
}

int Z80::array_read_short(const char *name, int field_id)
{
  return -1;
}

int Z80::array_read_int(const char *name, int field_id)
{
  return -1;
}

int Z80::array_write_byte()
{
  return -1;
}

int Z80::array_write_short()
{
  return -1;
}

int Z80::array_write_int()
{
  return -1;
}

int Z80::array_write_byte(const char *name, int field_id)
{
  return -1;
}

int Z80::array_write_short(const char *name, int field_id)
{
  return -1;
}

int Z80::array_write_int(const char *name, int field_id)
{
  return -1;
}

int Z80::stack_alu(const char *instr)
{
  if (stack == 0)
  {
    fprintf(out, "  %s %s, %s\n", instr, REG_STACK(reg-2), REG_STACK(reg-1));
    reg--;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop ix\n");
    fprintf(out, "  %s %s, ix\n", instr, REG_STACK(reg-1));
    stack--;
  }
    else
  {
    fprintf(out, "  pop ix\n");
    fprintf(out, "  pop iy\n");
    fprintf(out, "  %s iy, ix\n", instr);
    fprintf(out, "  push iy\n");
  }

  return 0;
}

int Z80::stack_alu_const(const char *instr, int num)
{
  if (stack == 0)
  {
    fprintf(out, "  %s %s, %d\n", instr, REG_STACK(reg-1), num);
    reg--;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop ix\n");
    fprintf(out, "  %s ix, %d\n", instr, num);
    fprintf(out, "  push ix\n");
    stack--;
  }

  return 0;
}


