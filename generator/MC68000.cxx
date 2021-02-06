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
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>

#include "generator/MC68000.h"

#define REG_STACK(a) (a)
#define LOCALS(i) ((i + 1) * 4)

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
  is_main(false)
{

}

MC68000::~MC68000()
{

}

int MC68000::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  // For now only support a specific chip
  fprintf(out, ".68000\n");

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

int MC68000::insert_static_field_define(
  std::string &name,
  std::string &type,
  int index)
{
  fprintf(out, "%s equ ram_start+%d\n", name.c_str(), index * 4);
  return 0;
}

int MC68000::init_heap(int field_count)
{
  fprintf(out, "  ;; Setup heap and static initializers\n");
  fprintf(out, "  movea.l #ram_start+%d, a5\n", field_count * 4);

  return 0;
}

int MC68000::field_init_int(std::string &name, int index, int value)
{
  fprintf(out, "  move.l #%d, (%s)\n", value, name.c_str());
  return 0;
}

int MC68000::field_init_ref(std::string &name, int index)
{
  fprintf(out, "  move.l #_%s, (%s)\n", name.c_str(), name.c_str());
  return 0;
}

void MC68000::method_start(
  int local_count,
  int max_stack,
  int param_count,
  std::string &name)
{
  reg = 0;
  stack = 0;

  is_main = name == "main";

  // main() function goes here
  fprintf(out, "%s:\n", name.c_str());

  if (local_count != 0)
  {
    fprintf(out, "  link a6, #-0x%x\n", local_count * 4);
  }

#if 0
  if (!is_main)
  {
    if (local_count != 0) { fprintf(out, "  link a6, #-0x%x\n", local_count * 4); }
  }
    else
  {
    fprintf(out, "  movea.l SP, a6\n");
    fprintf(out, "  suba.l #0x%x, SP\n", local_count * 4);
  }
#endif
}

void MC68000::method_end(int local_count)
{
  fprintf(out, "\n");
}

int MC68000::push_local_var_int(int index)
{
  fprintf(out, "  move.l (-%d,a6), %s\n", LOCALS(index), push_reg());
  return 0;
}

int MC68000::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int MC68000::push_ref_static(std::string &name, int index)
{
  fprintf(out, "  move.l #_%s, %s\n", name.c_str(), push_reg());
  return 0;
}

int MC68000::push_fake()
{
  if (stack != -1) { return -1; }
  reg++;
  return 0;
}

int MC68000::push_int(int32_t n)
{
  if (reg < reg_max)
  {
    if (n >= -128 && n <=127)
    {
      fprintf(out, "  moveq #%d, d%d\n", n, REG_STACK(reg));
    }
      else
    {
      fprintf(out, "  move.l #0x%02x, d%d\n", n, REG_STACK(reg));
    }
    reg++;
  }
    else
  {
    fprintf(out, "  move.l #0x%02x, -(SP)\n", n);
    stack++;
  }

  return 0;
}

#if 0
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
#endif

int MC68000::push_ref(std::string &name)
{
  fprintf(out, "  movea.l #%s, a2\n", name.c_str());
  fprintf(out, "  move.l (a2), %s\n", push_reg());
  return 0;
}

int MC68000::pop_local_var_int(int index)
{
  fprintf(out, "  move.l %s, (-%d,a6)\n", pop_reg(), LOCALS(index));
  return 0;
}

int MC68000::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int MC68000::pop()
{
  if (stack == 0)
  {
    reg--;
  }
    else
  {
    fprintf(out, "  move.l %s, d7\n", pop_reg());
  }
  return 0;
}

int MC68000::dup()
{
  fprintf(out, "  ;; dup\n");

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
  char reg1[16];
  char reg2[16];

  fprintf(out, "  ;; dup2\n");

  if (stack == 2)
  {
    fprintf(out, "  move.l (4,SP), -(SP)\n");
    fprintf(out, "  move.l (4,SP), -(SP)\n");
    stack += 2;
    return 0;
  }

  if (stack == 1)
  {
    fprintf(out, "  move.l d%d, -(SP)\n", REG_STACK(reg-1));
    fprintf(out, "  move.l (4,SP), -(SP)\n");
    stack += 2;
    return 0;
  }

  sprintf(reg1, "d%d", REG_STACK(reg-2));
  sprintf(reg2, "d%d", REG_STACK(reg-1));

  if (reg < reg_max)
  {
    fprintf(out, "  move.l %s, d%d\n", reg1, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  move.l %s, (SP)+\n", reg1);
    stack++;
  }

  if (reg < reg_max)
  {
    fprintf(out, "  move.l %s, d%d\n", reg2, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  move.l %s, (SP)+\n", reg2);
    stack++;
  }

  return 0;
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
  if (num >= 1 && num <= 8)
  {
    fprintf(out, "  addq.l #%d, %s\n", num, top_reg());
  }
    else
  if (num >= -8 && num <= -1)
  {
    fprintf(out, "  subq.l #%d, %s\n", -num, top_reg());
  }
    else
  {
    fprintf(out, "  add.l #%d, %s\n", num, top_reg());
  }
  return 0;
}

int MC68000::sub_integer()
{
  return stack_alu("sub");
}

int MC68000::sub_integer(int num)
{
  if (num >= 1 && num <= 8)
  {
    fprintf(out, "  subq.l #%d, %s\n", num, top_reg());
  }
    else
  if (num >= -8 && num <= -1)
  {
    fprintf(out, "  addq.l #%d, %s\n", -num, top_reg());
  }
    else
  {
    fprintf(out, "  sub.l #%d, %s\n", num, top_reg());
  }

  return 0;
}

int MC68000::mul_integer()
{
  return stack_alu("muls", "w");
}

int MC68000::mul_integer(int num)
{
  fprintf(out, "  muls.w #%d, %s\n", num, top_reg());
  return 0;
}

int MC68000::div_integer()
{
  return stack_alu("divs", "w");
}

int MC68000::div_integer(int num)
{
  fprintf(out, "  divs.w #%d, %s\n", num, top_reg());
  return 0;
}

int MC68000::mod_integer()
{
  if (stack == 0)
  {
    fprintf(out, "  divs.w d%d, d%d:d%d\n", REG_STACK(reg-1), REG_STACK(reg-2), REG_STACK(reg-1));
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
    fprintf(out, "  divs.w #%d, r%d:r%d\n", num, REG_STACK(reg-2), REG_STACK(reg-1));
  }
    else
  {
    return -1;
  }

  return 0;
}

int MC68000::neg_integer()
{
  fprintf(out, "  neg.l %s\n", top_reg());
  return 0;
}

int MC68000::shift_left_integer()
{
  return stack_alu("asl");
}

int MC68000::shift_left_integer(int num)
{
  if (num > 8) { return -1; }
  fprintf(out, "  asl.l #%d, %s\n", num, top_reg());
  return 0;
}

int MC68000::shift_right_integer()
{
  return stack_alu("asr");
}

int MC68000::shift_right_integer(int num)
{
  if (num > 8) { return -1; }
  fprintf(out, "  asr.l #%d, %s\n", num, top_reg());
  return 0;
}

int MC68000::shift_right_uinteger()
{
  return stack_alu("lsr");
}

int MC68000::shift_right_uinteger(int num)
{
  if (num > 8) { return -1; }
  fprintf(out, "  lsr.l #%d, %s\n", num, top_reg());
  return 0;
}

int MC68000::and_integer()
{
  return stack_alu("and");
}

int MC68000::and_integer(int num)
{
  fprintf(out, "  and.l #%d, %s\n", num, top_reg());
  return 0;
}

int MC68000::or_integer()
{
  return stack_alu("or");
}

int MC68000::or_integer(int num)
{
  fprintf(out, "  or.l #%d, %s\n", num, top_reg());
  return 0;
}

int MC68000::xor_integer()
{
  return stack_alu("eor");
}

int MC68000::xor_integer(int num)
{
  fprintf(out, "  eori.l #%d, %s\n", num, top_reg());
  return 0;
}

int MC68000::inc_integer(int index, int num)
{
  if (num >= 1 && num <= 8)
  {
    fprintf(out, "  addq.l #%d, (-%d,a6)\n", num, LOCALS(index));
  }
    else
  if (num >= -8 && num <= -1)
  {
    fprintf(out, "  subq.l #%d, (-%d,a6)\n", -num, LOCALS(index));
  }
    else
  {
    fprintf(out, "  add.l #%d, (-%d,a6)\n", num, LOCALS(index));
  }

  return 0;
}

int MC68000::integer_to_byte()
{
  fprintf(out, "  ext.w %s\n", top_reg());
  fprintf(out, "  ext.l %s\n", top_reg());
  return 0;
}

int MC68000::integer_to_short()
{
  fprintf(out, "  ext.l %s\n", top_reg());
  return 0;
}

int MC68000::jump_cond(std::string &label, int cond, int distance)
{
  char size = get_jump_size(distance);

  fprintf(out, "  cmp.l #0, %s\n", pop_reg());
  fprintf(out, "  b%s.%c %s  ; distance=%d\n", cond_str[cond], size, label.c_str(), distance);
  return 0;
}

int MC68000::jump_cond_integer(std::string &label, int cond, int distance)
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
    fprintf(out, "  cmp.l (SP)+, d%d\n", REG_STACK(reg-1));

    stack--;
    reg--;
  }
    else
  {
    fprintf(out, "  cmp.l d%d, d%d\n", REG_STACK(reg-1), REG_STACK(reg-2));

    reg -= 2;
  }

  fprintf(out, "  b%s.%c %s  ; distance=%d\n", cond_str[cond], size, label.c_str(), distance);

  return 0;

}

int MC68000::ternary(int cond, int value_true, int value_false)
{
  return -1;
}

int MC68000::ternary(int cond, int compare, int value_true, int value_false)
{
  return -1;
}

int MC68000::return_local(int index, int local_count)
{
  return -1;
}

int MC68000::return_integer(int local_count)
{
  fprintf(out, "  move.l %s, d7\n", pop_reg());

  if (local_count != 0) { fprintf(out, "  unlk a6\n"); }
  fprintf(out, "  rts\n");
  return 0;
}

int MC68000::return_void(int local_count)
{
  if (local_count != 0) { fprintf(out, "  unlk a6\n"); }
  fprintf(out, "  rts\n");
  return 0;
}

int MC68000::jump(std::string &name, int distance)
{
  char size = get_jump_size(distance);

  fprintf(out, "  bra.%c %s  ; distance=%d\n", size, name.c_str(), distance);

  return 0;
}

int MC68000::call(std::string &name)
{
  // REVIEW - Should this be callm?
  fprintf(out, "  jsr %s\n", name.c_str());
  return 0;
}

int MC68000::invoke_static_method(const char *name, int params, int is_void)
{
  int local;
  int stack_vars = stack;
  int reg_vars = reg;
  int saved_registers;
  int n;

  fprintf(out, "  ;; invoke_static_method() name=%s params=%d is_void=%d\n", name, params, is_void);

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
    // Put d7 on the top of the stack
    fprintf(out, "  move.l d7, %s\n", push_reg());
  }

  return 0;
}

int MC68000::put_static(std::string &name, int index)
{
  fprintf(out, "  move.l %s, %s\n", pop_reg(), name.c_str());
  return 0;
}

int MC68000::get_static(std::string &name, int index)
{
  fprintf(out, "  move.l %s, %s\n", name.c_str(), push_reg());
  return 0;
}

int MC68000::brk()
{
  return -1;
}

int MC68000::new_array(uint8_t type)
{
  int array_length_reg;

  fprintf(out, "  ; new_array()\n");

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

  // Store the size so it can be added back to heap pointer.
  fprintf(out, "  move.l d%d, d5\n", REG_STACK(reg));

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
  fprintf(out, "  add.l a5, d5\n");

  // Need to align heap
  fprintf(out, "  addq.l #3, d5\n");
  fprintf(out, "  andi.l #0xfffffffc, d5\n");

  // Update heap pointer.
  fprintf(out, "  movea.l d5, a5\n");

  return 0;
}

int MC68000::insert_array(std::string &name, int32_t *data, int len, uint8_t type)
{
  fprintf(out, ".align 32\n");

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

int MC68000::insert_string(std::string &name, uint8_t *bytes, int len)
{
  fprintf(out, ".align 32\n");
  fprintf(out, "  dc32 %d\n", len);
  return insert_utf8(name, bytes, len);
}

int MC68000::push_array_length()
{
  fprintf(out, "  ;; array length\n");

  if (stack > 0)
  {
    fprintf(out, "  move.l (SP)+, a2\n");
    fprintf(out, "  move.l (-4,a2), d7\n");
    fprintf(out, "  move.l d7, -(SP)\n");
  }
    else
  {
    fprintf(out, "  movea.l d%d, a2\n", REG_STACK(reg-1));
    fprintf(out, "  move.l (-4,a2), d%d\n", REG_STACK(reg-1));
  }

  return 0;
}

int MC68000::push_array_length(std::string &name, int field_id)
{
  fprintf(out, "  ;; push array length (%s)\n", name.c_str());
  fprintf(out, "  movea.l (%s,a4), a2\n", name.c_str());

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

  fprintf(out, "  adda.l d%d, a2\n", index_reg);

  if (reg < reg_max)
  {
    fprintf(out, "  move.b (a2), d%d\n", REG_STACK(reg));
    fprintf(out, "  ext.w d%d\n", REG_STACK(reg));
    fprintf(out, "  ext.l d%d\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  move.b (a2), d7\n");
    fprintf(out, "  ext.w d7\n");
    fprintf(out, "  ext.l d7\n");
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

  fprintf(out, "  ;; array_read_short()\n");
  fprintf(out, "  lsl.l #1, d%d\n", index_reg);

  if (reg < reg_max)
  {
    fprintf(out, "  move.w (0,a2,d%d.w), d%d\n", index_reg, REG_STACK(reg));
    fprintf(out, "  ext.l d%d\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  move.w (0,a2,d%d.w), d7\n", index_reg);
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

  fprintf(out, "  ;; array_read_int()\n");
  fprintf(out, "  lsl.l #2, d%d\n", index_reg);

  if (reg < reg_max)
  {
    fprintf(out, "  move.l (0,a2,d%d.l), d%d\n", index_reg, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  move.l (0,a2,d%d.l), d7\n", index_reg);
    fprintf(out, "  move.l d7, -(SP)\n");
    stack++;
  }

  return 0;
}

int MC68000::array_read_byte(std::string &name, int field_id)
{
  fprintf(out, "  movea.l (%s,a4), a2\n", name.c_str());

  if (stack > 0)
  {
    fprintf(out, "  add.l (SP)+, a2\n");
    fprintf(out, "  move.b (a2), d7\n");
    fprintf(out, "  ext.w d7\n");
    fprintf(out, "  ext.l d7\n");
    fprintf(out, "  move.l d7, -(SP)\n");
  }
    else
  {
    fprintf(out, "  move.b (0,a2,d%d.b), d%d\n", REG_STACK(reg-1),REG_STACK(reg-1));
    fprintf(out, "  ext.w d%d\n", REG_STACK(reg-1));
    fprintf(out, "  ext.l d%d\n", REG_STACK(reg-1));
  }

  return 0;
}

int MC68000::array_read_short(std::string &name, int field_id)
{
  fprintf(out, "  movea.l (%s,a4), a2\n", name.c_str());

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
    fprintf(out, "  move.w (0,a2,d%d.w), d%d\n", REG_STACK(reg-1),REG_STACK(reg-1));
    fprintf(out, "  ext.l d%d\n", REG_STACK(reg-1));
  }

  return 0;
}

int MC68000::array_read_int(std::string &name, int field_id)
{
  // FIXME - All array code can be heavily optimized.  This is the
  // worst possible way to do this.
  // A better way is with (a2,d7.l)
  fprintf(out, "  movea.l (%s,a4), a2\n", name.c_str());

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
    fprintf(out, "  move.l (0,a2,d%d.l), d%d\n", REG_STACK(reg-1),REG_STACK(reg-1));
  }

  return 0;
}

int MC68000::array_write_byte()
{
  int value_reg;
  int index_reg;

  get_values_from_stack(&value_reg, &index_reg);
  get_ref_from_stack();

  fprintf(out, "  adda.l d%d, a2\n", index_reg);
  fprintf(out, "  move.b d%d, (a2)\n", value_reg);

  return 0;
}

int MC68000::array_write_short()
{
  int value_reg;
  int index_reg;

  get_values_from_stack(&value_reg, &index_reg);
  get_ref_from_stack();

  fprintf(out, "  ;; array_write_short()\n");
  fprintf(out, "  lsl.l #1, d%d\n", index_reg);

  fprintf(out, "  move.w d%d, (0,a2,d%d.w)\n", value_reg, index_reg);

  return 0;
}

int MC68000::array_write_int()
{
  int value_reg;
  int index_reg;

  get_values_from_stack(&value_reg, &index_reg);
  get_ref_from_stack();

  fprintf(out, "  ;; array_write_int()\n");
  fprintf(out, "  lsl.l #2, d%d\n", index_reg);

  fprintf(out, "  move.l d%d, (0,a2,d%d.l)\n", value_reg, index_reg);

  return 0;
}

int MC68000::array_write_byte(std::string &name, int field_id)
{
  int value_reg;
  int index_reg;

  get_values_from_stack(&value_reg, &index_reg);

  fprintf(out, "  move.l (%s,a4), a2\n", name.c_str());
  fprintf(out, "  move.b d%d, (0,a2,d%d.b)\n", value_reg, index_reg);

  return 0;
}

int MC68000::array_write_short(std::string &name, int field_id)
{
  int value_reg;
  int index_reg;

  get_values_from_stack(&value_reg, &index_reg);

  fprintf(out, "  lsl.l #1, d%d\n", index_reg);
  fprintf(out, "  move.l (%s,a4), a2\n", name.c_str());
  fprintf(out, "  move.w d%d, (0,a2,d%d.w)\n", value_reg, index_reg);

  return 0;
}

int MC68000::array_write_int(std::string &name, int field_id)
{
  int value_reg;
  int index_reg;

  get_values_from_stack(&value_reg, &index_reg);

  fprintf(out, "  lsl.l #2, d%d\n", index_reg);
  fprintf(out, "  move.l (%s,a4), a2\n", name.c_str());
  fprintf(out, "  move.l d%d, (0,a2,d%d.l)\n", value_reg, index_reg);

  return 0;
}

int MC68000::memory_allocStackBytes_I()
{
  const char *reg = top_reg();
  fprintf(out, "  // allocStackBytes_I\n");
  fprintf(out, "  move.l %s, d7\n", reg);
  fprintf(out, "  addq.l #3, d7\n");
  fprintf(out, "  and.b #0xfc, d7\n");
  fprintf(out, "  suba.l d7, SP\n");
  fprintf(out, "  move.l %s, -(SP)\n", reg);
  fprintf(out, "  move.l SP, %s\n", reg);
  fprintf(out, "  addq.l #4, %s\n", reg);

  return 0;
}

int MC68000::memory_allocStackShorts_I()
{
  const char *reg = top_reg();
  fprintf(out, "  ;; allocStackShorts_I\n");
  fprintf(out, "  move.l %s, d7\n", reg);
  fprintf(out, "  lsl.l #1, d7\n");
  fprintf(out, "  addq.l #3, d7\n");
  fprintf(out, "  and.b #0xfc, d7\n");
  fprintf(out, "  suba.l d7, SP\n");
  fprintf(out, "  move.l %s, -(SP)\n", reg);
  fprintf(out, "  move.l SP, %s\n", reg);
  fprintf(out, "  addq.l #4, %s\n", reg);

  return 0;
}

int MC68000::memory_allocStackInts_I()
{
  const char *reg = top_reg();
  fprintf(out, "  // allocStackInts_I\n");
  fprintf(out, "  move.l %s, d7\n", reg);
  fprintf(out, "  lsl.l #1, d7\n");
  fprintf(out, "  suba.l d7, SP\n");
  fprintf(out, "  move.l %s, -(SP)\n", reg);
  fprintf(out, "  move.l SP, %s\n", reg);
  fprintf(out, "  addq.l #4, %s\n", reg);

  return 0;
}

const char *MC68000::pop_reg()
{
  if (stack > 0)
  {
    strcpy(reg_string, "(SP)+");
    stack--;
  }
    else
  {
    sprintf(reg_string, "d%d", REG_STACK(reg-1));
    reg--;
  }

  return reg_string;
}

const char *MC68000::push_reg()
{
  if (reg < reg_max)
  {
    sprintf(reg_string, "d%d", REG_STACK(reg));
    reg++;
  }
    else
  {
    strcpy(reg_string, "-(SP)");
    stack++;
  }

  return reg_string;
}

const char *MC68000::top_reg()
{
  if (stack > 0)
  {
    strcpy(reg_string, "(SP)");
  }
    else
  {
    sprintf(reg_string, "d%d", REG_STACK(reg-1));
  }

  return reg_string;
}

int MC68000::stack_alu(const char *instr, const char *size)
{
  if (stack == 0)
  {
    fprintf(out, "  %s.%s d%d, d%d\n", instr, size, REG_STACK(reg-1), REG_STACK(reg-2));
    reg--;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  %s.%s (SP)+, d%d\n", instr, size, REG_STACK(reg-1));
    stack--;
  }
    else
  {
    fprintf(out, "  move.l (SP)+, d7\n");
    fprintf(out, "  %s.%s d7, (SP)\n", instr, size);
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
    fprintf(out, "  movea.l d%d, a2\n", REG_STACK(reg-1));
    reg--;
  }

  return 0;
}

int MC68000::get_jump_size(int distance)
{
  if (distance < 18) { return 's'; }
  if (distance < 20000) { return 'w'; }

  return 'l';
}

int MC68000::memory_preloadByteArray_X(const char *array_name)
{
  fprintf(out,
    "  ;; memory_preloadByteArray_X(%s)\n"
    "  lea (0,pc), a2\n"
    "  adda.l #_%s-$+2, a2\n"
    "  move.l a2, %s\n",
    array_name,
    array_name,
    push_reg());

  return 0;
}

int MC68000::memory_preloadIntArray_X(const char *array_name)
{
  fprintf(out,
    "  ;; memory_preloadIntArray_X(%s)\n"
    "  lea (0,pc), a2\n"
    "  adda.l #_%s-$+2, a2\n"
    "  move.l a2, %s\n",
    array_name,
    array_name,
    push_reg());

  return 0;
}

int MC68000::add_array_files()
{
  struct stat statbuf;
  std::map<std::string, ArrayFiles>::iterator iter;

  const char *constant = "dc32";

  if (get_int_size() == 16)
  {
    constant = "dc16";
  }
  else if (get_int_size() == 8)
  {
    constant = "dc8";
  }

  for (iter = preload_arrays.begin(); iter != preload_arrays.end(); iter++)
  {
    if (stat(iter->first.c_str(), &statbuf) != 0)
    {
      printf("Error opening %s\n", iter->first.c_str());
      return -1;
    }

    fprintf(out, ".align 32\n");
    fprintf(out, "  %s %d\n",
      constant,
      (int)(iter->second.type == TYPE_BYTE ?
            statbuf.st_size : statbuf.st_size / get_int_size()));

    fprintf(out, "_%s:\n", iter->second.name.c_str());
    fprintf(out, ".binfile \"%s\"\n\n", iter->first.c_str());
  }

  return 0;
}

