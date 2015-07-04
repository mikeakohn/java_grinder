/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "MC68000.h"

#define REG_STACK(a) (a)
#define LOCALS(i) (i * 4)

// ABI is:
// d0 - top of stack
// d1
// d2
// d3
// d4 - bottom of stack
// d5 - temp
// d6 - temp
// d7 - return value / temp
// a0 - Used in sub-class (genesis)
// a1 - Used in sub-class (genesis)
// a2 - Temp pointer
// a3
// a4 - Pointer to statics
// a5 - Pointer to heap
// a6 - Link register
// a7 - Stack Pointer

static const char *cond_str[] = { "eq", "ne", "lt", "le", "gt", "ge" };

MC68000::MC68000() :
  ram_start(0),
  stack_start(512),
  reg(0),
  reg_max(5),
  stack(0),
  is_main(0)
{

}

MC68000::~MC68000()
{

}

int MC68000::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  // For now we only support a specific chip
  fprintf(out, ".680x0\n");

  // Set where RAM starts / ends
  // FIXME - Not sure what to set this to right now
  fprintf(out, "ram_start equ 0x%x\n", ram_start);
  //fprintf(out, "ram_end equ 0x8000\n");

  return 0;
}

int MC68000::start_init()
{
  // Add any set up items (stack, registers, etc).
  //fprintf(out, ".org ???\n");
  fprintf(out, "start:\n");
  fprintf(out, "  movea.l #0x%x, SP\n\n", stack_start);

  return 0;
}

int MC68000::insert_static_field_define(const char *name, const char *type, int index)
{
  fprintf(out, "%s equ ram_start+%d\n", name, index * 4);
  return 0;
}

int MC68000::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");
  //fprintf(out, "  move.l #ram_start+%d, &ram_start\n", (field_count + 1) * 2);
  fprintf(out, "  movea.l #ram_start+%d, a5\n", field_count * 4);
  return 0;
}

int MC68000::insert_field_init_boolean(char *name, int index, int value)
{
  value = (value == 0) ? 0 : 1;

  fprintf(out, "  move.l #%d, (%s)\n", value, name);
  return 0;
}

int MC68000::insert_field_init_byte(char *name, int index, int value)
{
  fprintf(out, "  move.l #%d, (%s)\n", value, name);
  return 0;
}

int MC68000::insert_field_init_short(char *name, int index, int value)
{
  fprintf(out, "  move.l #%d, (%s)\n", value, name);
  return 0;
}

int MC68000::insert_field_init_int(char *name, int index, int value)
{
  fprintf(out, "  move.l #%d, (%s)\n", value, name);
  return 0;
}

int MC68000::insert_field_init(char *name, int index)
{
  fprintf(out, "  move.l #_%s, (%s)\n", name, name);
  return 0;
}

void MC68000::method_start(int local_count, int max_stack, int param_count, const char *name)
{
  reg = 0;
  stack = 0;

  is_main = (strcmp(name, "main") == 0) ? true : false;

  // main() function goes here
  fprintf(out, "%s:\n", name);
  if (!is_main)
  {
    if (local_count != 0) { fprintf(out, "  link a6, #0x%x\n", local_count * 4); }
  }
    else
  {
    fprintf(out, "  movea.l SP, a6\n");
    fprintf(out, "  suba.l #0x%x, SP\n", local_count * 4);
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

int MC68000::push_ref_static(const char *name, int index)
{
  if (reg < reg_max)
  {
    fprintf(out, "  move.l #_%s, d%d\n", name, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  move.l #_%s, -(SP)\n", name);
    stack++;
  }

  return 0;
  return 0;
}

int MC68000::push_ref_local(int index)
{
  return push_integer_local(index);
}

int MC68000::push_fake()
{
  if (stack != -1) { return -1; }
  reg++;
  return 0;
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

int MC68000::push_ref(char *name)
{
  fprintf(out, "  movea.l #%s, a2\n", name);

  if (reg < reg_max)
  {
    fprintf(out, "  move.l (a2), d%d\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  move.l (a2), -(SP)\n");
    stack++;
  }

  return 0;
}

int MC68000::pop_integer_local(int index)
{
  if (stack > 0)
  {
    fprintf(out, "  move.l (SP)+ ,(-%d,a6)\n", LOCALS(index));
    stack--;
  }
    else
  if (reg > 0)
  {
    fprintf(out, "  move.l d%d, (-%d,a6)\n", REG_STACK(reg-1), LOCALS(index));
    reg--;
  }

  return 0;
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
  if (num >= -8 && num <= -1)
  {
    fprintf(out, "  subq.l #%d, (-%d,a6)\n", -num, index * 4);
  }
    else
  {
    fprintf(out, "  add.l #%d, (-%d,a6)\n", num, index * 4);
  }

  return 0;
}

int MC68000::integer_to_byte()
{
  if (stack == 0)
  {
    fprintf(out, "  extb.l (SP)\n");
  }
    else
  {
    fprintf(out, "  extb.l d%d\n", REG_STACK(reg-1));
  }

  return 0;
}

int MC68000::integer_to_short()
{
  if (stack == 0)
  {
    fprintf(out, "  extw.l (SP)\n");
  }
    else
  {
    fprintf(out, "  extw.l d%d\n", REG_STACK(reg-1));
  }

  return 0;
}

int MC68000::jump_cond(const char *label, int cond, int distance)
{
  char size = get_jump_size(distance);

  if (stack > 0)
  {
    fprintf(out, "  cmp.l #0, (SP)+\n");
    stack--;
  }
    else
  {
    fprintf(out, "  cmp.l #0, d%d\n", REG_STACK(reg-1));
    reg--;
  }

  fprintf(out, "  b%s.%c %s\n", cond_str[cond], size, label);
  return 0;
}

int MC68000::jump_cond_integer(const char *label, int cond, int distance)
{
  char size = get_jump_size(distance);

  if (stack > 1)
  {
    fprintf(out, "  move.l (SP)+, d7\n");
    fprintf(out, "  cmp.w d7, (SP)+\n");

    stack -= 2;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  cmp.l (SP)+, r%d\n", REG_STACK(reg-1));

    stack--;
    reg--;
  }
    else
  {
    fprintf(out, "  cmp.l d%d, d%d\n", REG_STACK(reg-1), REG_STACK(reg-2));

    reg -= 2;
  }

  fprintf(out, "  b%s.%c %s\n", cond_str[cond], size, label);

  return 0;

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

int MC68000::jump(const char *name, int distance)
{
  char size = get_jump_size(distance);

  fprintf(out, "  bra.%c %s\n", size, name);

#if 0
  if (distance < 64)
  {
    fprintf(out, "  bra.s %s\n", name);
  }
    else
  if (distance < 20000)
  {
    fprintf(out, "  bra.w %s\n", name);
  }
    else
  {
    fprintf(out, "  bra.l %s\n", name);
  }
#endif

  return 0;
}

int MC68000::call(const char *name)
{
  // REVIEW - Should this be callm?
  fprintf(out, "  jsr %s\n", name);
  return 0;
}

int MC68000::invoke_static_method(const char *name, int params, int is_void)
{
int local;
int stack_vars = stack;
int reg_vars = reg;
int saved_registers;
int n;

  printf("invoke_static_method() name=%s params=%d is_void=%d\n", name, params, is_void);

  // Push all used registers on the stack except the ones that are pulled
  // out for parameters.
  saved_registers = reg - (params - stack);
  for (n = 0; n < saved_registers; n++)
  {
    fprintf(out, "  move.l d%d, -(SP)\n", REG_STACK(n));
  }

  // Push pointer to local variables to the stack because the called
  // method will trash it.
  //fprintf(out, "  push r12\n");

  // Copy parameters onto the stack so they are local variables in
  // the called method.  Start with -4 because the return address will
  // be at 0 and r12 will be at 2.
  local = (params * -4);
  while(local != 0)
  {
    if (stack_vars > 0)
    {
      fprintf(out, "  move.l (%d,SP), (%d,SP)\n", (stack-stack_vars) * 4, local-8);
      stack_vars--;
    }
      else
    {
      fprintf(out, "  move.l d%d, (%d,SP)\n", REG_STACK(reg_vars-1), local-8);
      reg_vars--;
    }

    local += 4;
  }

  // Make the call
  fprintf(out, "  jsr %s\n", name);

  // Pop the local variables pointer
  //fprintf(out, "  pop r12\n");

  // Pop all used registers off the stack
  for (n = saved_registers-1; n >= 0; n--)
  {
    fprintf(out, "  move.l (SP)+, d%d\n", REG_STACK(n));
  }

  // Pop all params off the Java stack
  if ((stack - stack_vars) > 0)
  {
    fprintf(out, "  sub.l #%d, SP\n", (stack - stack_vars) * 4);
    params -= (stack - stack_vars);
  }

  if (params != 0)
  {
    reg -= params;
  }

  if (!is_void)
  {
    // Put r15 on the top of the stack
    if (reg < reg_max)
    {
      fprintf(out, "  move.l d7, d%d\n", REG_STACK(reg));
      reg++;
    }
      else
    {
      // REVIEW - This looks wrong
      fprintf(out, "  move.l d7, -(SP)\n");
      stack++;
    }
  }

  return 0;
}

int MC68000::put_static(const char *name, int index)
{
  if (stack > 0)
  {
    fprintf(out, "  mov (SP)+, %s\n", name);
    stack--;
  }
    else
  {
    fprintf(out, "  mov d%d, %s\n", REG_STACK(reg-1), name);
    reg--;
  }

  return 0;
}

int MC68000::get_static(const char *name, int index)
{
  if (reg < reg_max)
  {
    fprintf(out, "  move.l %s, d%d\n", name, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  move.l %s, -(SP)\n", name);
    stack++;
  }

  return 0;
}

int MC68000::brk()
{
  return -1;
}

int MC68000::new_array(uint8_t type)
{
  int array_length_reg;

  // ref = heap + 4
  // heap = heap + sizeof(array) + 4 (to hold the length of the array)

  get_values_from_stack(&array_length_reg);

  // store length @ heap
  fprintf(out, "  move.l d%d, (a5)\n", array_length_reg);

  if (type == TYPE_INT)
  {
    // int is 4 bytes
    fprintf(out, "  lsl.l #2, d%d\n", array_length_reg);
  }
    else
  if (type == TYPE_SHORT || type == TYPE_CHAR)
  {
    // short is 2 bytes
    fprintf(out, "  lsl.l #1, d%d\n", array_length_reg);
  }

  // sizeof(array) + 4 (for array size)
  fprintf(out, "  addq.l #4, a5\n");

  // Top of Java stack should equal where the heap is currently pointing
  if (reg < reg_max)
  {
    fprintf(out, "  move.l a5, d%d\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  move.l a5, -(SP)\n");
    stack++;
  }

  // Add the length of the array to heap pointer
  fprintf(out, "  add.l d%d, a5\n", array_length_reg);

  // Need to align heap
  fprintf(out, "  addq.l #3, a5\n");
  fprintf(out, "  and.l #0xfffffffc, a5\n");

  return 0;
}

int MC68000::insert_array(const char *name, int32_t *data, int len, uint8_t type)
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

int MC68000::insert_string(const char *name, uint8_t *bytes, int len)
{
  fprintf(out, ".align 32\n");
  fprintf(out, "  dc32 %d\n", len);
  return insert_utf8(name, bytes, len);
}

int MC68000::push_array_length()
{
  if (stack > 0)
  {
    fprintf(out, "  move.l (SP)+, a2\n");
    fprintf(out, "  move.l (-4,a2), d7\n");
    fprintf(out, "  move.l d7, -(SP)\n");
  }
    else
  {
    fprintf(out, "  move.l (-4,d%d), d%d\n", REG_STACK(reg-1), REG_STACK(reg-1));
  }

  return 0;
}

int MC68000::push_array_length(const char *name, int field_id)
{
  fprintf(out, "  movea.l (%s,a4), a2\n", name);

  if (reg < reg_max)
  {
    fprintf(out, "  move.l (-4,a2), d%d\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  move.l (-4,a2), d7\n");
    fprintf(out, "  move.l d7, -(SP)\n");
    stack++;
  }

  return 0;
}

int MC68000::array_read_byte()
{
  int index_reg;

  get_values_from_stack(&index_reg);
  get_ref_from_stack();
  //fprintf(out, "  add.l d%d, d%d\n", index_reg, ref_reg);
  //fprintf(out, "  move.l d%d, a2\n", ref_reg);

  if (reg < reg_max)
  {
    fprintf(out, "  move.b (d%d,a2), d%d\n", index_reg, REG_STACK(reg));
    fprintf(out, "  extb.l d%d\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  move.b (d%d,a2), d7\n", index_reg);
    fprintf(out, "  extb.l d7\n");
    fprintf(out, "  move.l d7, -(SP)\n");
    stack++;
  }

  return 0;
}

int MC68000::array_read_short()
{
  int index_reg;

  get_values_from_stack(&index_reg);
  get_ref_from_stack();
  fprintf(out, "  lsl.l #1, d%d\n", index_reg);
  //fprintf(out, "  add.l d%d, d%d\n", index_reg, ref_reg);
  //fprintf(out, "  move.l d%d, a2\n", ref_reg);

  if (reg < reg_max)
  {
    fprintf(out, "  move.w (d%d,a2), d%d\n", index_reg, REG_STACK(reg));
    fprintf(out, "  ext.l d%d\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  move.w (a2), d7\n");
    fprintf(out, "  ext.l d7\n");
    fprintf(out, "  move.l d7, -(SP)\n");
    stack++;
  }

  return 0;
}

int MC68000::array_read_int()
{
  int index_reg;

  get_values_from_stack(&index_reg);
  get_ref_from_stack();
  fprintf(out, "  lsl.l #2, d%d\n", index_reg);
  //fprintf(out, "  add.l d%d, d%d\n", index_reg, ref_reg);
  //fprintf(out, "  move.l d%d, a2\n", ref_reg);

  if (reg < reg_max)
  {
    fprintf(out, "  move.l (d%d,a2), d%d\n", index_reg, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  move.l (d%d,a2), d7\n", index_reg);
    fprintf(out, "  move.l d7, -(SP)\n");
    stack++;
  }

  return 0;
}

int MC68000::array_read_byte(const char *name, int field_id)
{
  fprintf(out, "  movea.l (%s,a4), a2\n", name);

  if (stack > 0)
  {
    fprintf(out, "  add.l (SP)+, a2\n");
    fprintf(out, "  move.b (a2), d7\n");
    fprintf(out, "  extb.l d7\n");
    fprintf(out, "  move.l d7, -(SP)\n");
  }
    else
  {
    //fprintf(out, "  add.l d%d, a2\n", REG_STACK(reg-1));
    fprintf(out, "  move.b (d%d,a2), d%d\n", REG_STACK(reg-1),REG_STACK(reg-1));
    fprintf(out, "  extb.l d%d\n", REG_STACK(reg-1));
  }

  return 0;
}

int MC68000::array_read_short(const char *name, int field_id)
{
  fprintf(out, "  movea.l (%s,a4), a2\n", name);

  if (stack > 0)
  {
    fprintf(out, "  move.l (SP)+, d7\n");
    fprintf(out, "  lsl.l #1, d7\n");
    fprintf(out, "  add.l d7, a2\n");
    fprintf(out, "  move.w (a2), d7\n");
    fprintf(out, "  ext.l d7\n");
    fprintf(out, "  move.l d7, -(SP)\n");
  }
    else
  {
    fprintf(out, "  lsl.l #1, d%d\n", REG_STACK(reg-1));
    //fprintf(out, "  add.l d%d, a2\n", REG_STACK(reg-1));
    fprintf(out, "  move.w (d%d,a2), d%d\n", REG_STACK(reg-1),REG_STACK(reg-1));
    fprintf(out, "  ext.l d%d\n", REG_STACK(reg-1));
  }

  return 0;
}

int MC68000::array_read_int(const char *name, int field_id)
{
  // FIXME - All array code can be heavily optimized.  This is the
  // worst possible way to do this.
  // A better way is with (a2,d7.l)
  fprintf(out, "  movea.l (%s,a4), a2\n", name);

  if (stack > 0)
  {
    fprintf(out, "  move.l (SP)+, d7\n");
    fprintf(out, "  lsl.l #2, d7\n");
    fprintf(out, "  add.l d7, a2\n");
    fprintf(out, "  move.l (a2), d7\n");
    fprintf(out, "  move.l d7, -(SP)\n");
  }
    else
  {
    fprintf(out, "  lsl.l #2, d%d\n", REG_STACK(reg-1));
    //fprintf(out, "  add.l d%d, a2\n", REG_STACK(reg-1));
    fprintf(out, "  move.l (d%d,a2), d%d\n", REG_STACK(reg-1),REG_STACK(reg-1));
  }

  return 0;
}

int MC68000::array_write_byte()
{
  int value_reg;
  int index_reg;

  get_values_from_stack(&value_reg, &index_reg);
  get_ref_from_stack();
  //fprintf(out, "  add.l d%d, d%d\n", index_reg, ref_reg);
  //fprintf(out, "  move.l d%d, a2\n", ref_reg);
  //fprintf(out, "  move.b d%d, (a2)\n", value_reg);
  fprintf(out, "  move.w d%d, (d%d,a2)\n", value_reg, index_reg);

  return 0;
}

int MC68000::array_write_short()
{
  int value_reg;
  int index_reg;

  get_values_from_stack(&value_reg, &index_reg);
  get_ref_from_stack();
  fprintf(out, "  lsl.l #1, d%d\n", index_reg);
  //fprintf(out, "  add.l d%d, d%d\n", index_reg, ref_reg);
  //fprintf(out, "  move.l d%d, a2\n", ref_reg);
  //fprintf(out, "  move.b d%d, (a2)\n", value_reg);
  fprintf(out, "  move.w d%d, (d%d,a2)\n", value_reg, index_reg);

  return 0;
}

int MC68000::array_write_int()
{
  int value_reg;
  int index_reg;

  get_values_from_stack(&value_reg, &index_reg);
  get_ref_from_stack();
  fprintf(out, "  lsl.l #2, d%d\n", index_reg);
  //fprintf(out, "  add.l d%d, d%d\n", index_reg, ref_reg);
  //fprintf(out, "  move.l d%d, a2\n", ref_reg);
  fprintf(out, "  move.l d%d, (d%d,a2)\n", value_reg, index_reg);

  return 0;
}

int MC68000::array_write_byte(const char *name, int field_id)
{
  int value_reg;
  int index_reg;

  get_values_from_stack(&value_reg, &index_reg);

  //fprintf(out, "  add.l %s, d%d\n", name, index_reg);
  //fprintf(out, "  move.l d%d, a2\n", index_reg);
  //fprintf(out, "  move.l d%d, (a2)\n", value_reg);
  fprintf(out, "  move.l (%s,a4), a2\n", name);
  fprintf(out, "  move.b d%d, (d%d,a2)\n", value_reg, index_reg);

  return 0;
}

int MC68000::array_write_short(const char *name, int field_id)
{
  int value_reg;
  int index_reg;

  get_values_from_stack(&value_reg, &index_reg);

  fprintf(out, "  lsl.l #1, d%d\n", index_reg);
  //fprintf(out, "  add.l %s, d%d\n", name, index_reg);
  //fprintf(out, "  move.l d%d, a2\n", index_reg);
  //fprintf(out, "  move.l d%d, (a2)\n", value_reg);
  fprintf(out, "  move.l (%s,a4), a2\n", name);
  fprintf(out, "  move.w d%d, (d%d,a2)\n", value_reg, index_reg);

  return 0;
}

int MC68000::array_write_int(const char *name, int field_id)
{
  int value_reg;
  int index_reg;

  get_values_from_stack(&value_reg, &index_reg);

  fprintf(out, "  lsl.l #2, d%d\n", index_reg);
  //fprintf(out, "  add.l %s, d%d\n", name, index_reg);
  //fprintf(out, "  move.l d%d, a2\n", index_reg);
  //fprintf(out, "  move.l d%d, (a2)\n", value_reg);
  fprintf(out, "  move.l (%s,a4), a2\n", name);
  fprintf(out, "  move.l d%d, (d%d,a2)\n", value_reg, index_reg);

  return 0;
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
printf("%d %d  reg=%d\n", REG_STACK(reg-1), REG_STACK(reg-2), reg);
    fprintf(out, "  %s.l d%d, d%d\n", instr, REG_STACK(reg-1), REG_STACK(reg-2));
    reg--;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  %s.l (SP)+, d%d\n", instr, REG_STACK(reg-1));
    stack--;
  }
    else
  {
    fprintf(out, "  move.l (SP)+, d7\n");
    fprintf(out, "  %s.l d7, (SP)\n", instr);
    stack--;
  }

  return 0;
}

int MC68000::get_values_from_stack(int *value1, int *value2, int *value3)
{
  if (stack > 0)
  {
    fprintf(out, "  move.l (SP)+, d5\n");
    *value1 = 5;
    stack--;
  }
    else
  {
    *value1 = REG_STACK(reg-1);
    reg--;
  }

  if (stack > 0)
  {
    fprintf(out, "  move.l (SP)+, d6\n");
    *value2 = 6;
    stack--;
  }
    else
  {
    *value2 = REG_STACK(reg-1);
    reg--;
  }

  if (stack > 0)
  {
    fprintf(out, "  move.l (SP)+, d7\n");
    *value3 = 7;
    stack--;
  }
    else
  {
    *value3 = REG_STACK(reg-1);
    reg--;
  }

  return 0;
}

int MC68000::get_values_from_stack(int *value1, int *value2)
{
  if (stack > 0)
  {
    fprintf(out, "  move.l (SP)+, d6\n");
    *value1 = 6;
    stack--;
  }
    else
  {
    *value1 = REG_STACK(reg-1);
    reg--;
  }

  if (stack > 0)
  {
    fprintf(out, "  move.l (SP)+, d7\n");
    *value2 = 7;
    stack--;
  }
    else
  {
    *value2 = REG_STACK(reg-1);
    reg--;
  }

  return 0;
}

int MC68000::get_values_from_stack(int *value1)
{
  if (stack > 0)
  {
    fprintf(out, "  move.l (SP)+, d6\n");
    *value1 = 6;
    stack--;
  }
    else
  {
    *value1 = REG_STACK(reg-1);
    reg--;
  }

  return 0;
}

int MC68000::get_ref_from_stack()
{
  if (stack > 0)
  {
    fprintf(out, "  move.l (SP)+, a2\n");
    stack--;
  }
    else
  {
    //*value1 = REG_STACK(reg-1);
    fprintf(out, "  move.l d%d, a2\n", REG_STACK(reg-1));
    reg--;
  }

  return 0;
}

int MC68000::get_jump_size(int distance)
{
  if (distance < 64) { return 's'; }
  if (distance < 20000) { return 'w'; }

  return 'l';
}


