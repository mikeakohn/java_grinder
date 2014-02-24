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

#include "MC68000.h"

#define REG_STACK(a) (reg)
#define LOCALS(i) (i * 4)

// ABI is:
// d0 - top of stack
// d1
// d2
// d3
// d4
// d5 - bottom of stack
// d6
// d7 - return value / temp
// a0
// a1
// a2
// a3
// a4 Pointer to statics
// a5 Pointer to heap
// a6 Link register
// a7 Stack Pointer

MC68000::MC68000() :
  reg(0),
  reg_max(6),
  stack(0),
  is_main(0)
{

}

MC68000::~MC68000()
{

}

int MC68000::open(char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  // For now we only support a specific chip
  fprintf(out, ".680x0\n");

  // Set where RAM starts / ends
  // FIXME - Not sure what to set this to right now
  fprintf(out, "ram_start equ 0\n");
  fprintf(out, "ram_end equ 0x8000\n");

  return 0;
}

int MC68000::start_init()
{
  // Add any set up items (stack, registers, etc).
  //fprintf(out, ".org ???\n");
  fprintf(out, "start:\n");

  return 0;
}

int MC68000::insert_static_field_define(const char *name, const char *type, int index)
{
  fprintf(out, "%s equ ram_start+%d\n", name, (index + 1) * 4);
  return 0;
}

int MC68000::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");
  fprintf(out, "  move.l #ram_start+%d, &ram_start\n", (field_count + 1) * 2);
  return 0;
}

int MC68000::insert_field_init_boolean(char *name, int index, int value)
{
  value = (value == 0) ? 0 : 1;

  fprintf(out, "  move.l #%d, (%s,a4)\n", value, name);
  return 0;
}

int MC68000::insert_field_init_byte(char *name, int index, int value)
{
  fprintf(out, "  move.l #%d, (%s,a4)\n", value, name);
  return 0;
}

int MC68000::insert_field_init_short(char *name, int index, int value)
{
  fprintf(out, "  move.l #%d, (%s,a4)\n", value, name);
  return 0;
}

int MC68000::insert_field_init_int(char *name, int index, int value)
{
  fprintf(out, "  move.l #%d, (%s,a4)\n", value, name);
  return 0;
}

int MC68000::insert_field_init(char *name, int index)
{
  fprintf(out, "  move.l #_%s, (%s,a4)\n", name, name);
  return 0;
}

void MC68000::method_start(int local_count, const char *name)
{
  reg = 0;
  stack = 0;

  is_main = (strcmp(name, "main") == 0) ? true : false;

  // main() function goes here
  fprintf(out, "%s:\n", name);
  if (!is_main)
  {
    if (local_count != 0) { fprintf(out, "  link a6, #0x%x\n", local_count * 2); }
  }
    else
  {
    fprintf(out, "  movea.l SP, a6\n");
    fprintf(out, "  sub.l #0x%x, SP\n", local_count * 2);
  }
}

void MC68000::method_end(int local_count)
{
  fprintf(out, "\n");
}

int MC68000::push_integer(int32_t n)
{
  if (reg < reg_max)
  {
    fprintf(out, "  move.l #0x%02x, d%d\n", n, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  move.l #0x%02x, -(SP)\n", n);
    stack++;
  }

  return 0;
}

int MC68000::push_integer_local(int index)
{
  if (reg < reg_max)
  {
    fprintf(out, "  move.l (-%d,a6), d%d\n", LOCALS(index), REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  move.l (-%d,a6), -(SP)\n", LOCALS(index));
    stack++;
  }

  return 0;
}

int MC68000::push_ref_local(int index)
{
  return push_integer_local(index);
}

int MC68000::push_long(int64_t n)
{
  return -1;
}

int MC68000::push_float(float f)
{
  return -1;
}

int MC68000::push_double(double f)
{
  return -1;
}

int MC68000::push_byte(int8_t b)
{
  int32_t value = (int32_t)b;

  return push_integer(value);
}

int MC68000::push_short(int16_t s)
{
  int32_t value = (int32_t)s;

  return push_integer(value);
}

int MC68000::pop_integer_local(int index)
{
  return -1;
}

int MC68000::pop_ref_local(int index)
{
  return pop_integer_local(index);
}

int MC68000::pop()
{
  fprintf(out, "  adda #4, SP\n");
  return 0;
}

int MC68000::dup()
{
  if (stack > 0)
  {
    fprintf(out, "  move.l (SP), -(SP)\n");
    stack++;
  }
    else
  if (reg == reg_max)
  {
    fprintf(out, "  move.l d%d, -(SP)\n", REG_STACK(reg-1));
    stack++;
  }
    else
  {
    fprintf(out, "  move.l d%d, d%d\n", REG_STACK(reg-1), REG_STACK(reg));
    reg++;
  }

  return 0;
}

int MC68000::dup2()
{
  return -1;
}

int MC68000::swap()
{
  if (stack == 0)
  {
    fprintf(out, "  move.l d%d, d7\n", REG_STACK(reg-1));
    fprintf(out, "  move.l d%d, d%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
    fprintf(out, "  move.l d7, d%d\n", REG_STACK(reg-2));
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  move.l d%d, d7\n", REG_STACK(reg-1));
    fprintf(out, "  move.l (SP), d%d\n", REG_STACK(reg-1));
    fprintf(out, "  move.l d7, (SP)\n");
  }
    else
  {
    fprintf(out, "  move.l (4,SP), d7\n");
    fprintf(out, "  move.l (SP), (4,SP)\n");
    fprintf(out, "  move.l d7, (SP)\n");
  }

  return 0;
}

int MC68000::add_integer()
{
  return stack_alu("add");
}

int MC68000::add_integer(int num)
{
  fprintf(out, "  add.l #%d, d%d\n", num, REG_STACK(reg-1));
  return 0;
}

int MC68000::sub_integer()
{
  return stack_alu("sub");
}

int MC68000::sub_integer(int num)
{
  fprintf(out, "  sub.l #%d, d%d\n", num, REG_STACK(reg-1));
  return 0;
}

int MC68000::mul_integer()
{
  return stack_alu("muls");
}

int MC68000::mul_integer(int num)
{
  fprintf(out, "  muls.l #%d, d%d\n", num, REG_STACK(reg-1));
  return 0;
}

int MC68000::div_integer()
{
  return stack_alu("divs");
}

int MC68000::div_integer(int num)
{
  fprintf(out, "  divs.l #%d, d%d\n", num, REG_STACK(reg-1));
  return 0;
}

int MC68000::mod_integer()
{
  if (stack == 0)
  {
    fprintf(out, "  divs.l d%d, d%d:d%d\n", REG_STACK(reg-1), REG_STACK(reg-2), REG_STACK(reg-1));
    reg--;
  }
    else
  {
    return -1;
  }

  return 0;
}

int MC68000::mod_integer(int num)
{
  if (stack == 0)
  {
    fprintf(out, "  divs.l #%d, r%d:r%d\n", num, REG_STACK(reg-2), REG_STACK(reg-1));
  }
    else
  {
    return -1;
  }

  return 0;
}

int MC68000::neg_integer()
{
  if (stack == 0)
  {
    fprintf(out, "  neg.l d%d\n", REG_STACK(reg-1));
  }
    else
  {
    fprintf(out, "  neg.l (SP)\n");
  }

  return 0;
}

int MC68000::shift_left_integer()
{
  return stack_alu("asl");
}

int MC68000::shift_left_integer(int num)
{
  fprintf(out, "  asl.l #%d, d%d\n", num, REG_STACK(reg-1));
  return 0;
}

int MC68000::shift_right_integer()
{
  return stack_alu("asr");
}

int MC68000::shift_right_integer(int num)
{
  fprintf(out, "  asr.l #%d, d%d\n", num, REG_STACK(reg-1));
  return 0;
}

int MC68000::shift_right_uinteger()
{
  return stack_alu("lsr");
}

int MC68000::shift_right_uinteger(int num)
{
  fprintf(out, "  lsr.l #%d, d%d\n", num, REG_STACK(reg-1));
  return 0;
}

int MC68000::and_integer()
{
  return stack_alu("and");
}

int MC68000::and_integer(int num)
{
  fprintf(out, "  and.l #%d, d%d\n", num, REG_STACK(reg-1));
  return 0;
}

int MC68000::or_integer()
{
  return stack_alu("or");
}

int MC68000::or_integer(int num)
{
  fprintf(out, "  or.l #%d, d%d\n", num, REG_STACK(reg-1));
  return 0;
}

int MC68000::xor_integer()
{
  return stack_alu("eor");
}

int MC68000::xor_integer(int num)
{
  fprintf(out, "  eor.l #%d, d%d\n", num, REG_STACK(reg-1));
  return 0;
}

int MC68000::inc_integer(int index, int num)
{
  if (num >= 1 && num <= 8)
  {
    fprintf(out, "  addq.l #%d, (-%d,a6)\n", num, index * 4);
  }
    else
  {
    fprintf(out, "  add.l #%d, (-%d,a6)\n", num, index * 4);
  }

  return 0;
}

int MC68000::jump_cond(const char *label, int cond)
{
  return -1;
}

int MC68000::jump_cond_integer(const char *label, int cond)
{
  return -1;
}

int MC68000::return_local(int index, int local_count)
{
  return -1;
}

int MC68000::return_integer(int local_count)
{
  if (stack > 0)
  {
    fprintf(out, "  move.l (SP)+, d7\n");
  }
    else
  {
    fprintf(out, "  move.l d%d, d7\n", REG_STACK(reg - 1));
  }

  if (local_count != 0) { fprintf(out, "  ulnk\n"); }
  fprintf(out, "  ret\n");
  return 0;
}

int MC68000::return_void(int local_count)
{
  if (local_count != 0) { fprintf(out, "  ulnk\n"); }
  fprintf(out, "  ret\n");
  return 0;
}

int MC68000::jump(const char *name)
{
  return -1;
}

int MC68000::call(const char *name)
{
  return -1;
}

int MC68000::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int MC68000::put_static(const char *name, int index)
{
  return -1;
}

int MC68000::get_static(const char *name, int index)
{
  return -1;
}

int MC68000::brk()
{
  return -1;
}

int MC68000::new_array(uint8_t type)
{
  return -1;
}

int MC68000::insert_array(const char *name, int32_t *data, int len, uint8_t type)
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

int MC68000::push_array_length()
{
  return -1;
}

int MC68000::push_array_length(const char *name, int field_id)
{
  return -1;
}

int MC68000::array_read_byte()
{
  return -1;
}

int MC68000::array_read_short()
{
  return -1;
}

int MC68000::array_read_int()
{
  return -1;
}

int MC68000::array_read_byte(const char *name, int field_id)
{
  return -1;
}

int MC68000::array_read_short(const char *name, int field_id)
{
  return -1;
}

int MC68000::array_read_int(const char *name, int field_id)
{
  return -1;
}

int MC68000::array_write_byte()
{
  return -1;
}

int MC68000::array_write_short()
{
  return -1;
}

int MC68000::array_write_int()
{
  return -1;
}

int MC68000::array_write_byte(const char *name, int field_id)
{
  return -1;
}

int MC68000::array_write_short(const char *name, int field_id)
{
  return -1;
}

int MC68000::array_write_int(const char *name, int field_id)
{
  return -1;
}

#if 0
void MC68000::close()
{
}
#endif

int MC68000::stack_alu(const char *instr)
{
  if (stack == 0)
  {
    fprintf(out, "  %s.l r%d, r%d\n", instr, REG_STACK(reg-1), REG_STACK(reg-2));
    reg--;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  %s.l (SP)+, r%d\n", instr, REG_STACK(reg-1));
    stack--;
  }
    else
  {
    fprintf(out, "  move.l (SP)+, d7\n");
    fprintf(out, "  %s.l d7, (SP)\n", instr);
  }

  return 0;
}



