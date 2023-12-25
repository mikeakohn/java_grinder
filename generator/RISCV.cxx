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
#include <string.h>
#include <stdint.h>

#include "generator/RISCV.h"

#define REG_STACK(a) (a)
#define LOCALS(i) (-(i * 4))

// ABI is:
// x0  zero Always 0
// x1  ra Return address
// x2  sp Stack pointer
// x3  gp Global pointer
// x4  tp Thread pointer
// x5  t0
// x6  t1
// x7  t2
// x8  s0 or fp
// x9  s1 Saved registers 1
// x10 a0 Function argument 0 (return value)
// x11 a1 Function argument 1 (return value)
// x12 a2
// x13 a3
// x14 a4
// x15 a5
// x16 a6
// x17 a7 Function argument 7
// x18 s2 Saved registers 2
// x19 s3 Saved registers 3
// x20 s4 Saved registers 4
// x21 s5 Saved registers 5
// x22 s6 Saved registers 6
// x23 s7 Saved registers 7
// x24 s8 Saved registers 8
// x25 s9 Saved registers 9
// x26 s10 Saved registers 10
// x27 s11 Saved registers 11
// x28 t3
// x29 t4
// x30 t5
// x31 t6

// RAM is:
// ram_start:
//   static 1..
//   static 2..
//   static 3..
//   etc
//   HEAP <--- gp
//
//   STACK <--- sp

// FIXME: Make these functions.
// Stack points to the last used slot.
// push = subtract 4, then place value
#define STACK_PUSH(t) \
  fprintf(out, "  addi sp, sp, -4\n"); \
  fprintf(out, "  sw $t%d, 0(sp)\n", t); \
  stack++;

// pop = read value, then subtract 4
#define STACK_POP(t) \
  fprintf(out, "  lw $%d, 0(sp)\n", t); \
  fprintf(out, "  addi sp, sp, 4\n"); \
  stack--;

RISCV::RISCV() :
  reg(0),
  reg_max(8),
  stack(0),
  org(0),
  ram_start(0),
  ram_end(1024),
  is_main(false)
{

}

RISCV::~RISCV()
{
  fprintf(out, ".align 32\n");
  insert_constants_pool();
}

int RISCV::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  //fprintf(out, ".mips32\n");

  // Set where RAM starts / ends
  fprintf(out, "  ram_start equ 0x%x\n", ram_start);
  fprintf(out, "  ram_end equ 0x%x\n", ram_end);

  return 0;
}

int RISCV::start_init()
{
  // Add any set up items (stack, registers, etc).
  fprintf(out, ".org 0x%x\n", org);
  fprintf(out, "start:\n");

  fprintf(out, "  li tp, ram_start       ; tp points to statics\n");
  fprintf(out, "  li sp, ram_end\n");

  return 0;
}

int RISCV::insert_static_field_define(std::string &name, std::string &type, int index)
{
  fprintf(out, "  %s equ ram_star t +%d\n", name.c_str(), index * 4);
  return 0;
}


int RISCV::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");
  fprintf(out, "  li gp, ram_start + %d\n", field_count * 4);
  return 0;
}

int RISCV::field_init_int(std::string &name, int index, int value)
{
  uint32_t n = (uint32_t)value;

  if (n == 0)
  {
    fprintf(out, "  sw zero, 0x%04x(tp)\n", index * 4);
    return 0;
  }
    else
  if ((n & 0x0000ffff) == 0x0000)
  {
    fprintf(out, "  lui t0, 0x%04x\n", n >> 16);
  }
    else
  {
    fprintf(out, "  li t0, 0x%04x\n", n);
  }

  fprintf(out, "  sw t0, 0x%04x(tp) ; static %s\n", index * 4, name.c_str());

  return 0;
}

int RISCV::field_init_ref(std::string &name, int index)
{
  fprintf(out, "  ; static init\n");
  fprintf(out, "  li t0, _%s\n", name.c_str());
  //fprintf(out, "  sw t0, 0x%04x(tp)\n", index * 4);
  fprintf(out, "  li t1, %s\n", name.c_str());
  fprintf(out, "  sw t0, (t1)\n");

  return 0;
}

void RISCV::method_start(
  int local_count,
  int max_stack,
  int param_count,
  std::string &name)
{
  is_main = name == "main";

  fprintf(out, "%s:\n", name.c_str());
  fprintf(out, "  ; %s(local_count=%d, max_stack=%d, param_count=%d)\n", name.c_str(), local_count, max_stack, param_count);
  fprintf(out, "  addi fp, sp, -4\n");
  fprintf(out, "  addi sp, sp, -%d\n", (local_count * 4) + 4);
}

void RISCV::method_end(int local_count)
{
  if (!is_main)
  {
    // If this isn't main() restore stack pointer.
    //fprintf(out, "  addi sp, sp, %d\n", local_count * 4);
  }

  fprintf(out, "\n");
}

int RISCV::push_local_var_int(int index)
{
  if (reg < 8)
  {
    fprintf(out, "  lw a%d, %d(fp) ; local_%d\n", reg, LOCALS(index), index);
    reg++;
  }
    else
  {
    fprintf(out, "  lw t0, %d(fp) ; local_%d\n", LOCALS(index), index);
    STACK_PUSH(8)
  }

  return 0;
}

int RISCV::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int RISCV::push_ref_static(std::string &name, int index)
{
  if (reg < reg_max)
  {
    fprintf(out, "  addi a%d, tp, %d\n", reg, index * 4);
    reg++;
  }
    else
  {
    fprintf(out, "  addi t0, tp, %d\n", index * 4);
    STACK_PUSH(8);
  }

  return -1;
}

int RISCV::push_fake()
{
  if (stack != 0) { return -1; }
  reg++;
  return 0;
}

int RISCV::push_int(int32_t n)
{
  fprintf(out, "  ; push_int(%d)\n", n);

  if (reg < reg_max)
  {
    fprintf(out, "  li a%d, %d\n", reg, n);
    reg++;
  }
    else
  {
    STACK_PUSH(n)
  }

  return 0;
}

#if 0
int RISCV::push_long(int64_t n)
{
  return -1;
}

int RISCV::push_float(float f)
{
  return -1;
}

int RISCV::push_double(double f)
{
  return -1;
}
#endif

int RISCV::push_ref(std::string &name, int index)
{
  fprintf(out, "  ; push_ref(%s)\n", name.c_str());
  fprintf(out, "  li t0, %s\n", name.c_str());

  if (reg < reg_max)
  {
    fprintf(out, "  lw a%d, (t0)\n", reg);
    reg++;
  }
    else
  {
    fprintf(out, "  lw t0, (t0)\n");
    STACK_PUSH(8);
  }

  return 0;
}

int RISCV::pop_local_var_int(int index)
{
  if (stack > 0)
  {
    STACK_POP(8);
    fprintf(out, "  sw t0, %d(fp) ; local_%d\n", LOCALS(index), index);
  }
    else
  {
    fprintf(out, "  sw a%d, %d(fp) ; local_%d\n", reg - 1, LOCALS(index), index);
    reg--;
  }

  return 0;
}

int RISCV::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int RISCV::pop()
{
  fprintf(out, "  ; pop()\n");

  if (stack > 0)
  {
    fprintf(out, "  addi sp, sp, 4\n"); \
    stack--;
  }
    else
  {
    reg--;
  }

  return 0;
}

int RISCV::dup()
{
  fprintf(out, "  ; dup()\n");

  if (stack > 0)
  {
    fprintf(out, "  lw t0, 0(sp)\n"); \
    STACK_PUSH(8);
  }
    else
  {
    if (reg < 8)
    {
      fprintf(out, "  mv a%d, a%d\n", reg, reg - 1);
      reg++;
    }
      else
    {
      STACK_PUSH(reg - 1);
    }
  }

  return 0;
}

int RISCV::dup2()
{
  fprintf(out, "  ; dup2()\n");

  if (stack >= 2)
  {
    fprintf(out, "  lw t0, -4(sp)\n");
    fprintf(out, "  lw t1, 0(sp)\n");
    STACK_PUSH(8);
    STACK_PUSH(9);
    return 0;
  }
    else
  if (stack == 1)
  {
    //fprintf(out, "  mv t0, a%d\n", reg - 1);
    fprintf(out, "  lw t1, 0(sp)\n");
    STACK_PUSH(reg - 1);
    STACK_PUSH(9);
    return 0;
  }
    else
  if (reg < 7)
  {
    fprintf(out, "  mv a%d, a%d\n", reg, reg - 2);
    fprintf(out, "  mv a%d, a%d\n", reg + 1, reg - 1);
    reg += 2;
    return 0;
  }
    else
  {
    fprintf(out, "  mv a%d, a%d\n", reg, reg - 2);
    reg++;
    STACK_PUSH(7);
  }

  return 0;
}

int RISCV::swap()
{
  if (stack >= 2)
  {
    fprintf(out, "  lw t1, 0(sp)\n");
    fprintf(out, "  lw t0, -4(sp)\n");
    fprintf(out, "  sw t0, 0(sp)\n");
    fprintf(out, "  sw t1, -4(sp)\n");
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  lw t0, 0(sp)\n");
    fprintf(out, "  sw a7, 0(sp)\n");
    fprintf(out, "  mv a7, t0\n");
  }
    else
  {
    fprintf(out, "  mv t0, a%d\n", reg - 1);
    fprintf(out, "  mv a%d, a%d\n", reg - 1, reg - 2);
    fprintf(out, "  mv a%d, t0\n", reg - 2);
  }

  return 0;
}

int RISCV::add_integer()
{
  return stack_alu("add");
}

int RISCV::add_integer(int num)
{
  if (stack != 0) { return -1; }
  if (num < -32768 || num > 32767) { return -1; }
  fprintf(out, "  addi a%d, a%d, %d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int RISCV::sub_integer()
{
  return stack_alu("sub");
}

int RISCV::sub_integer(int num)
{
  if (stack != 0) { return -1; }
  num = -num;
  if (num < -32768 || num > 32767) { return -1; }
  fprintf(out, "  addi a%d, a%d, %d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int RISCV::mul_integer()
{
  if (stack == 0)
  {
    fprintf(out, "  mult a%d, a%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
    fprintf(out, "  mflo a%d\n", REG_STACK(reg-2));
    reg--;
  }
    else
  if (stack == 1)
  {
    STACK_POP(8);
    fprintf(out, "  mult a%d, t0\n", REG_STACK(reg-1));
    fprintf(out, "  mflo a%d\n", REG_STACK(reg-1));
  }
    else
  {
    STACK_POP(8);
    STACK_POP(9);
    fprintf(out, "  mult t1, t0\n");
    fprintf(out, "  mflo t1\n");
    STACK_PUSH(9);
  }

  return 0;
}

int RISCV::div_integer()
{
  divide();

  if (stack > 0)
  {
    fprintf(out, "  mflo t0\n");
    STACK_PUSH(8);
  }
    else
  {
    fprintf(out, "  mflo a%d\n", REG_STACK(reg));
    reg++;
  }

  return 0;
}

int RISCV::mod_integer()
{
  divide();

  if (stack > 0)
  {
    // OUCH.  This is probably very rare anyway
    STACK_POP(8);
    fprintf(out, "  mfhi t0\n");
    STACK_PUSH(8);
  }
    else
  {
    fprintf(out, "  mfhi a%d\n", REG_STACK(reg - 1));
  }

  return 0;
}

int RISCV::neg_integer()
{
  if (stack > 0)
  {
    STACK_POP(8);
    fprintf(out, "  negu t0, t0\n");
    STACK_PUSH(8);
  }
    else
  {
    fprintf(out, "  negu a%d, a%d\n", REG_STACK(reg - 1), REG_STACK(reg - 1));
  }

  return 0;
}

int RISCV::shift_left_integer()
{
  return stack_alu("sll");
  return 0;
}

int RISCV::shift_left_integer(int num)
{
  if (num < 0 || num > 31) { printf("Shift out of range\n"); return -1; }

  if (stack > 0)
  {
    STACK_POP(8);
    fprintf(out, "  sll t0, t0, %d\n", num);
    STACK_PUSH(8);
  }
    else
  {
    fprintf(out, "  sll a%d, a%d, %d\n", REG_STACK(reg - 1), REG_STACK(reg - 1), num);
  }

  return 0;
}

int RISCV::shift_right_integer()
{
  return stack_alu("sra");
  return 0;
}

int RISCV::shift_right_integer(int num)
{
  if (num < 0 || num > 31) { printf("Shift out of range\n"); return -1; }

  if (stack > 0)
  {
    STACK_POP(8);
    fprintf(out, "  sra t0, t0, %d\n", num);
    STACK_PUSH(8);
  }
    else
  {
    fprintf(out, "  sra a%d, a%d, %d\n", REG_STACK(reg - 1), REG_STACK(reg - 1), num);
  }

  return 0;
}

int RISCV::shift_right_uinteger()
{
  return stack_alu("srl");
  return 0;
}

int RISCV::shift_right_uinteger(int num)
{
  if (num < 0 || num > 31) { printf("Shift out of range\n"); return -1; }

  if (stack > 0)
  {
    STACK_POP(8);
    fprintf(out, "  srl t0, t0, %d\n", num);
    STACK_PUSH(8);
  }
    else
  {
    fprintf(out, "  srl a%d, a%d, %d\n", REG_STACK(reg - 1), REG_STACK(reg - 1), num);
  }

  return 0;
}

int RISCV::and_integer()
{
  return stack_alu("and");
}

int RISCV::and_integer(int num)
{
  if (stack != 0) { return -1; }
  if (num < 0 || num > 0xffff) { return -1; }
  fprintf(out, "  andi a%d, a%d, %d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int RISCV::or_integer()
{
  return stack_alu("or");
}

int RISCV::or_integer(int num)
{
  if (stack != 0) { return -1; }
  if (num < 0 || num > 0xffff) { return -1; }
  fprintf(out, "  ori a%d, a%d, %d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int RISCV::xor_integer()
{
  return stack_alu("xor");
}

int RISCV::xor_integer(int num)
{
  if (stack != 0) { return -1; }
  if (num < 0 || num > 0xffff) { return -1; }
  fprintf(out, "  xori a%d, a%d, %d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int RISCV::inc_integer(int index, int num)
{
  fprintf(out, "  ; inc_integer(local_%d,%d)\n", index, num);
  fprintf(out, "  lw t0, %d(fp)\n", LOCALS(index));
  fprintf(out, "  addi t0, t0, %d\n", num);
  fprintf(out, "  sw t0, %d(fp)\n", LOCALS(index));

  return 0;
}

int RISCV::integer_to_byte()
{
  if (stack > 0)
  {
    fprintf(out, "  lw t0, 0(sp)\n");
    fprintf(out, "  seb t0, t0\n");
    fprintf(out, "  sw t0, 0(sp)\n");
  }
    else
  {
    fprintf(out, "  seb a%d, a%d\n", REG_STACK(reg - 1), REG_STACK(reg - 1));
  }

  return 0;
}

int RISCV::integer_to_short()
{
  if (stack > 0)
  {
    fprintf(out, "  lw t0, 0(sp)\n");
    fprintf(out, "  seh t0, t0\n");
    fprintf(out, "  sw t0, 0(sp)\n");
  }
    else
  {
    fprintf(out, "  seh a%d, a%d\n", REG_STACK(reg - 1), REG_STACK(reg - 1));
  }

  return 0;
}

int RISCV::jump_cond(std::string &label, int cond, int distance)
{
  fprintf(out, "  ; jump_cond(%s, %d, %d)\n", label.c_str(), cond, distance);

  switch (cond)
  {
    case COND_EQUAL:
      fprintf(out, "  beq a%d, zero, %s\n", --reg, label.c_str());
      fprintf(out, "  nop\n");
      return 0;
    case COND_NOT_EQUAL:
      fprintf(out, "  bne a%d, zero, %s\n", --reg, label.c_str());
      fprintf(out, "  nop\n");
      return 0;
    case COND_LESS:
      fprintf(out, "  bltz a%d, %s\n", --reg, label.c_str());
      fprintf(out, "  nop\n");
      return 0;
    case COND_LESS_EQUAL:
      fprintf(out, "  blez a%d, %s\n", --reg, label.c_str());
      fprintf(out, "  nop\n");
      return 0;
    case COND_GREATER:
      fprintf(out, "  bgtz a%d, %s\n", --reg, label.c_str());
      fprintf(out, "  nop\n");
      return 0;
    case COND_GREATER_EQUAL:
      fprintf(out, "  bgez a%d, %s\n", --reg, label.c_str());
      fprintf(out, "  nop\n");
      return 0;
    default:
      break;
  }

  return -1;
}

int RISCV::jump_cond_integer(std::string &label, int cond, int distance)
{
  fprintf(out, "  ; jump_cond_integer(%s, %d, %d)\n", label.c_str(), cond, distance);

  // I think this should never happen
  if (stack != 0)
  {
    printf("Internal Error: Stack not empty\n");
    return -1;
  }

  switch (cond)
  {
    case COND_EQUAL:
      fprintf(out, "  beq a%d, a%d, %s\n", reg - 2, reg - 1, label.c_str());
      fprintf(out, "  nop\n");
      reg -= 2;
      return 0;
    case COND_NOT_EQUAL:
      fprintf(out, "  bne a%d, a%d, %s\n", reg - 2, reg - 1, label.c_str());
      fprintf(out, "  nop\n");
      reg -= 2;
      return 0;
    case COND_LESS:
      fprintf(out, "  blt a%d, a%d, %s\n", reg - 2, reg - 1, label.c_str());
      fprintf(out, "  nop\n");
      reg -= 2;
      return 0;
    case COND_LESS_EQUAL:
      fprintf(out, "  ble a%d, a%d, %s\n", reg - 2, reg - 1, label.c_str());
      fprintf(out, "  nop\n");
      reg -= 2;
      return 0;
    case COND_GREATER:
      fprintf(out, "  bgt a%d, a%d, %s\n", reg - 2, reg - 1, label.c_str());
      fprintf(out, "  nop\n");
      reg -= 2;
      return 0;
    case COND_GREATER_EQUAL:
      fprintf(out, "  bge a%d, a%d, %s\n", reg - 2, reg - 1, label.c_str());
      fprintf(out, "  nop\n");
      reg -= 2;
      return 0;
    default:
      break;
  }

  return -1;
}

int RISCV::ternary(int cond, int value_true, int value_false)
{
  fprintf(out, "  ; ternary %d ? %d : %d\n", cond, value_true, value_false);
  fprintf(out, "  ; true condition is in delay slot\n");

  switch (cond)
  {
    case COND_EQUAL:
      fprintf(out, "  beq a%d, a%d, ternary_%d\n", reg - 2, reg - 1, label_count);
      if (set_constant(reg - 2, value_true) != 0) { return -1; }
      if (set_constant(reg - 2, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      reg -= 1;
      return 0;
    case COND_NOT_EQUAL:
      fprintf(out, "  bne a%d, a%d, ternary_%d\n", reg - 2, reg - 1, label_count);
      if (set_constant(reg - 2, value_true) != 0) { return -1; }
      if (set_constant(reg - 2, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      reg -= 1;
      return 0;
    case COND_LESS:
      fprintf(out, "  blt a%d, a%d, ternary_%d\n", reg - 2, reg - 1, label_count);
      if (set_constant(reg - 2, value_true) != 0) { return -1; }
      if (set_constant(reg - 2, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      reg -= 1;
      return 0;
    case COND_LESS_EQUAL:
      fprintf(out, "  ble a%d, a%d, ternary_%d\n", reg - 2, reg - 1, label_count);
      if (set_constant(reg - 2, value_true) != 0) { return -1; }
      if (set_constant(reg - 2, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      reg -= 1;
      return 0;
    case COND_GREATER:
      fprintf(out, "  bgt a%d, a%d, ternary_%d\n", reg - 2, reg - 1, label_count);
      if (set_constant(reg - 2, value_true) != 0) { return -1; }
      if (set_constant(reg - 2, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      reg -= 1;
      return 0;
    case COND_GREATER_EQUAL:
      fprintf(out, "  bge a%d, a%d, ternary_%d\n", reg - 2, reg - 1, label_count);
      if (set_constant(reg - 2, value_true) != 0) { return -1; }
      if (set_constant(reg - 2, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      reg -= 1;
      return 0;
    default:
      break;
  }

  return -1;
}

int RISCV::ternary(int cond, int compare, int value_true, int value_false)
{
  fprintf(out, "  ; ternary %d (%d) ? %d : %d\n", cond, compare, value_true, value_false);
  fprintf(out, "  ; true condition is in delay slot\n");

  if (set_constant(8, compare) != 0) { return -1; }

  switch (cond)
  {
    case COND_EQUAL:
      fprintf(out, "  beq a%d, t0, ternary_%d\n", reg - 1, label_count);
      if (set_constant(reg - 1, value_true) != 0) { return -1; }
      if (set_constant(reg - 1, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      return 0;
    case COND_NOT_EQUAL:
      fprintf(out, "  bne a%d, t0, ternary_%d\n", reg - 1, label_count);
      if (set_constant(reg - 1, value_true) != 0) { return -1; }
      if (set_constant(reg - 1, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      return 0;
    case COND_LESS:
      fprintf(out, "  blt a%d, t0, ternary_%d\n", reg - 1, label_count);
      if (set_constant(reg - 1, value_true) != 0) { return -1; }
      if (set_constant(reg - 1, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      return 0;
    case COND_LESS_EQUAL:
      fprintf(out, "  ble a%d, t0, ternary_%d\n", reg - 1, label_count);
      if (set_constant(reg - 1, value_true) != 0) { return -1; }
      if (set_constant(reg - 1, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      return 0;
    case COND_GREATER:
      fprintf(out, "  bgt a%d, t0, ternary_%d\n", reg - 1, label_count);
      if (set_constant(reg - 1, value_true) != 0) { return -1; }
      if (set_constant(reg - 1, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      return 0;
    case COND_GREATER_EQUAL:
      fprintf(out, "  bge a%d, t0, ternary_%d\n", reg - 1, label_count);
      if (set_constant(reg - 1, value_true) != 0) { return -1; }
      if (set_constant(reg - 1, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      return 0;
    default:
      break;
  }

  return -1;
}

int RISCV::return_local(int index, int local_count)
{
  if (reg != 0)
  {
    printf("Internal Error: Reg stack not empty %s:%d\n", __FILE__, __LINE__);
  }

  fprintf(out, "  lw s1, %d(fp) ; local_%d\n", LOCALS(index), index);
  fprintf(out, "  addi sp, sp, %d\n", (local_count * 4) + 4);
  fprintf(out, "  jr ra\n");
  fprintf(out, "  nop\n");

  return 0;
}

int RISCV::return_integer(int local_count)
{
  if (reg != 1)
  {
    printf("Internal Error: Reg stack not empty %s:%d\n", __FILE__, __LINE__);
  }

  fprintf(out, "  mv s1, a0\n");
  fprintf(out, "  addi sp, sp, %d\n", (local_count * 4) + 4);
  fprintf(out, "  jr ra\n");
  fprintf(out, "  nop ; Delay slot\n");
  reg--;

  return 0;
}

int RISCV::return_void(int local_count)
{
  if (reg != 0)
  {
    printf("Internal Error: Reg stack not empty %s:%d\n", __FILE__, __LINE__);
  }

  fprintf(out, "  addi sp, sp, %d\n", (local_count * 4) + 4);
  fprintf(out, "  jr ra\n");
  fprintf(out, "  nop ; Delay slot\n");
  return 0;
}

int RISCV::jump(std::string &name, int distance)
{
  fprintf(out, "  b %s\n", name.c_str());
  fprintf(out, "  nop ; Delay slot\n");

  return 0;
}

int RISCV::call(std::string &name)
{
  return -1;
}

int RISCV::invoke_static_method(const char *name, int params, int is_void)
{
  int save_space;
  int save_regs;
  //int local_index = 0;
  int n;
  int param_sp = 0;

  fprintf(out, "  ; invoke_static_method() name=%s params=%d is_void=%d\n", name, params, is_void);

  save_regs = reg - params;
  save_space = ((save_regs) * 4) + 8;

  // Save ra and fp
  fprintf(out, "  addi sp, sp, -%d\n", save_space);
  fprintf(out, "  sw ra, %d(sp)\n", save_space - 4);
  fprintf(out, "  sw fp, %d(sp)\n", save_space - 8);

  // Save temp registers and parameter registers.
  for (n = 0; n < save_regs; n++)
  {
    fprintf(out, "  sw a%d, %d(sp)\n", n, save_space - ((n + 3) * 4));
  }

  param_sp = -4;

  // Push registers that are parameters.
  // Parameters are pushed left to right.
  for (n = save_regs; n < reg; n++)
  {
    fprintf(out, "  sw a%d, %d(sp)\n", n, param_sp);
    param_sp -= 4;
  }

  // Setup parameters that are on the stack.
  for (n = 0; n < stack; n++)
  {
    fprintf(out, "  lw t2, %d(sp)\n", save_space + (n * 4) + 4);
    fprintf(out, "  sw t2, %d(sp)\n", param_sp);
    param_sp -= 4;
  }

  fprintf(out, "  jal %s\n", name);
  fprintf(out, "  nop ; Delay slot\n");

  // Restore temp registers
  for (n = 0; n < save_regs; n++)
  {
    fprintf(out, "  lw a%d, %d(sp)\n", n, save_space - (n * 4) + 8);
  }

  // Restore ra and fp
  fprintf(out, "  lw ra, %d(sp)\n", save_space - 4);
  fprintf(out, "  lw fp, %d(sp)\n", save_space - 8);
  fprintf(out, "  addi sp, sp, %d\n", save_space);

  // Decrease count on reg stack.
  if (stack > 0)
  {
    // Pick the min between stack and params.
    n = (stack > params) ? params : stack;
    fprintf(out, "  addi sp, sp, %d\n", n * 4);
    params -= n;
  }

  reg -= params;

  // If this isn't a void, push return value onto register stack.
  if (!is_void)
  {
    if (reg < reg_max)
    {
      fprintf(out, "  mv a%d, s1\n", REG_STACK(reg));
      reg++;
    }
      else
    {
      fprintf(out, "  addi sp, sp, -%d\n", 4);
      fprintf(out, "  sw s1, 0(sp)\n");
      stack++;
    }
  }

  return 0;
}

int RISCV::put_static(std::string &name, int index)
{
  fprintf(out, "  ; put_static(%s, %d)\n", name.c_str(), index);

  if (stack > 0)
  {
    STACK_POP(8);
    fprintf(out, "  sw t0, %d(tp)\n", index * 4);
  }
    else
  {
    fprintf(out, "  sw a%d, %d(tp)\n", reg - 1, index * 4);
    reg--;
  }

  return 0;
}

int RISCV::get_static(std::string &name, int index)
{
  fprintf(out, "  ; get_static(%s, %d)\n", name.c_str(), index);

  if (reg < reg_max)
  {
    fprintf(out, "  lw a%d, %d(tp)\n", reg++, index * 4);
  }
    else
  {
    fprintf(out, "  lw t0, %d(tp)\n", index * 4);
    STACK_PUSH(8);
  }

  return 0;
}

int RISCV::brk()
{
  fprintf(out, "  ebreak\n");
  return 0;
}

int RISCV::new_array(uint8_t type)
{
  int t;

  fprintf(out, "  ; new_array(%d)\n", type);

  if (stack > 0)
  {
    STACK_POP(8);
    t = 8;
  }
    else
  {
    t = reg - 1;
  }

  fprintf(out, "  sw a%d, 0(gp)\n", t);

  if (type == TYPE_INT)
  {
    fprintf(out, "  sll a%d, a%d, 2\n", t, t);
  }
    else
  if (type == TYPE_SHORT || type == TYPE_CHAR)
  {
    fprintf(out, "  sll a%d, a%d, 1\n", t, t);
  }

  fprintf(out, "  addi a%d, a%d, 4\n", t, t);
  fprintf(out, "  mv t1, gp\n");
  fprintf(out, "  add gp, gp, a%d\n", t);
  fprintf(out, "  addi t1, t1, 4\n");

  if (t == 8)
  {
    STACK_PUSH(9);
  }
    else
  {
    fprintf(out, "  mv a%d, t1\n", t);
  }

  return 0;
}

int RISCV::insert_array(std::string &name, int32_t *data, int len, uint8_t type)
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

int RISCV::insert_string(std::string &name, uint8_t *bytes, int len)
{
  fprintf(out, ".align 32\n");
  fprintf(out, "  dc32 %d\n", len);
  return insert_utf8(name, bytes, len);
}

int RISCV::push_array_length()
{
  if (stack > 0)
  {
    fprintf(out, "  lw t0, 0(sp)\n"); \
    fprintf(out, "  lw t0, -4(t0)\n");
    fprintf(out, "  sw t0, 0(sp)\n"); \
    STACK_PUSH(8);
  }
    else
  {
    fprintf(out, "  lw a%d, -4(a%d)\n", REG_STACK(reg-1), REG_STACK(reg-1));
  }

  return 0;
}

int RISCV::push_array_length(std::string &name, int field_id)
{
  fprintf(out, "  lw t0, 0x%04x(tp) ; static %s\n", field_id * 4, name.c_str());

  if (reg >= reg_max)
  {
    STACK_PUSH(8);
  }
    else
  {
    fprintf(out, "  lw a%d, -4(t0)\n", REG_STACK(reg));
    reg++;
  }

  return 0;
}

int RISCV::array_read_byte()
{
  int index_reg;
  int ref_reg;

  get_values_from_stack(&index_reg, &ref_reg);

  fprintf(out, "  add a%d, a%d, a%d\n", ref_reg, ref_reg, index_reg);

  if (reg < reg_max)
  {
    fprintf(out, "  lb a%d, 0(a%d)\n", REG_STACK(reg), ref_reg);
    reg++;
  }
    else
  {
    fprintf(out, "  lb t0, 0(a%d)\n", ref_reg);
    STACK_PUSH(8);
  }

  return 0;
}

int RISCV::array_read_short()
{
  int index_reg;
  int ref_reg;

  get_values_from_stack(&index_reg, &ref_reg);

  fprintf(out, "  sll a%d, a%d, 1\n", index_reg, index_reg);
  fprintf(out, "  add a%d, a%d, a%d\n", ref_reg, ref_reg, index_reg);

  if (reg < reg_max)
  {
    fprintf(out, "  lh a%d, 0(a%d)\n", REG_STACK(reg), ref_reg);
    reg++;
  }
    else
  {
    fprintf(out, "  lh t0, 0(a%d)\n", ref_reg);
    STACK_PUSH(8);
  }

  return 0;
}

int RISCV::array_read_int()
{
  int index_reg;
  int ref_reg;

  get_values_from_stack(&index_reg, &ref_reg);

  fprintf(out, "  sll a%d, a%d, 2\n", index_reg, index_reg);
  fprintf(out, "  add a%d, a%d, a%d\n", ref_reg, ref_reg, index_reg);

  if (reg < reg_max)
  {
    fprintf(out, "  lw a%d, 0(a%d)\n", REG_STACK(reg), ref_reg);
    reg++;
  }
    else
  {
    fprintf(out, "  lw t0, 0(a%d)\n", ref_reg);
    STACK_PUSH(8);
  }

  return 0;
}

int RISCV::array_read_byte(std::string &name, int field_id)
{
  int index_reg;

  get_values_from_stack(&index_reg);

  fprintf(out, "  lw t2, %d(tp)\n", field_id * 4);
  fprintf(out, "  add t2, t2, a%d\n", index_reg);

  if (reg < reg_max)
  {
    fprintf(out, "  lb a%d, 0(t2)\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  lb t0, 0(t2)\n");
    STACK_PUSH(8);
  }

  return 0;
}

int RISCV::array_read_short(std::string &name, int field_id)
{
  int index_reg;

  get_values_from_stack(&index_reg);

  fprintf(out, "  lw t2, %d(tp)\n", field_id * 4);
  fprintf(out, "  sll a%d, a%d, 1\n", index_reg, index_reg);
  fprintf(out, "  add t2, t2, a%d\n", index_reg);

  if (reg < reg_max)
  {
    fprintf(out, "  lh a%d, 0(t2)\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  lh t0, 0(t2)\n");
    STACK_PUSH(8);
  }

  return 0;
}

int RISCV::array_read_int(std::string &name, int field_id)
{
  int index_reg;

  get_values_from_stack(&index_reg);

  fprintf(out, "  lw t2, %d(tp)\n", field_id * 4);
  fprintf(out, "  sll a%d, a%d, 2\n", index_reg, index_reg);
  fprintf(out, "  add t2, t2, a%d\n", index_reg);

  if (reg < reg_max)
  {
    fprintf(out, "  lw a%d, 0(t2)\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  lw t0, 0(t2)\n");
    STACK_PUSH(8);
  }

  return 0;
}

int RISCV::array_write_byte()
{
  int value_reg;
  int index_reg;
  int ref_reg;

  get_values_from_stack(&value_reg, &index_reg);

  if (get_ref_from_stack(&ref_reg) == -1)
  {
    fprintf(out, "  add t2, t2, a%d\n", index_reg);
    fprintf(out, "  sb a%d, 0(t2)\n", value_reg);
  }
    else
  {
    fprintf(out, "  add a%d, a%d, a%d\n", ref_reg, ref_reg, index_reg);
    fprintf(out, "  sb a%d, 0(a%d)\n", value_reg, ref_reg);
  }

  return 0;
}

int RISCV::array_write_short()
{
  int value_reg;
  int index_reg;
  int ref_reg;

  get_values_from_stack(&value_reg, &index_reg);

  if (get_ref_from_stack(&ref_reg) == -1)
  {
    fprintf(out, "  sll a%d, a%d, 1\n", index_reg, index_reg);
    fprintf(out, "  add t2, t2, a%d\n", index_reg);
    fprintf(out, "  sh a%d, 0(t2)\n", value_reg);
  }
    else
  {
    fprintf(out, "  sll a%d, a%d, 1\n", index_reg, index_reg);
    fprintf(out, "  add a%d, a%d, a%d\n", ref_reg, ref_reg, index_reg);
    fprintf(out, "  sh a%d, 0(a%d)\n", value_reg, ref_reg);
  }

  return 0;
}

int RISCV::array_write_int()
{
  int value_reg;
  int index_reg;
  int ref_reg;

  get_values_from_stack(&value_reg, &index_reg);

  if (get_ref_from_stack(&ref_reg) == -1)
  {
    fprintf(out, "  sll a%d, a%d, 2\n", index_reg, index_reg);
    fprintf(out, "  add t2, t2, a%d\n", index_reg);
    fprintf(out, "  sw a%d, 0(t2)\n", value_reg);
  }
    else
  {
    fprintf(out, "  sll a%d, a%d, 2\n", index_reg, index_reg);
    fprintf(out, "  add a%d, a%d, a%d\n", ref_reg, ref_reg, index_reg);
    fprintf(out, "  sw a%d, 0(a%d)\n", value_reg, ref_reg);
  }

  return 0;
}

int RISCV::array_write_byte(std::string &name, int field_id)
{
  int value_reg;
  int index_reg;

  get_values_from_stack(&value_reg, &index_reg);

  fprintf(out, "  lw t2, %d(tp)\n", field_id * 4);
  fprintf(out, "  add t2, t2, a%d\n", index_reg);
  fprintf(out, "  sb a%d, 0(t2)\n", value_reg);

  return 0;
}

int RISCV::array_write_short(std::string &name, int field_id)
{
  int value_reg;
  int index_reg;

  get_values_from_stack(&value_reg, &index_reg);

  fprintf(out, "  lw t2, %d(tp)\n", field_id * 4);
  fprintf(out, "  sll a%d, a%d, 1\n", index_reg, index_reg);
  fprintf(out, "  add t2, t2, a%d\n", index_reg);
  fprintf(out, "  sh a%d, 0(t2)\n", value_reg);

  return 0;
}

int RISCV::array_write_int(std::string &name, int field_id)
{
  int value_reg;
  int index_reg;

  get_values_from_stack(&value_reg, &index_reg);

  fprintf(out, "  lw t2, %d(tp)\n", field_id * 4);
  fprintf(out, "  sll a%d, a%d, 2\n", index_reg, index_reg);
  fprintf(out, "  add t2, t2, a%d\n", index_reg);
  fprintf(out, "  sw a%d, 0(t2)\n", value_reg);

  return 0;
}

int RISCV::cpu_nop()
{
  fprintf(out, "  nop\n");
  return 0;
}

int RISCV::stack_alu(const char *instr)
{
  if (stack == 0)
  {
    fprintf(out, "  %s a%d, a%d, a%d\n", instr, REG_STACK(reg-2), REG_STACK(reg-2), REG_STACK(reg-1));
    reg--;
  }
    else
  if (stack == 1)
  {
    STACK_POP(8);
    fprintf(out, "  %s a%d, a%d, t0\n", instr, REG_STACK(reg-1), REG_STACK(reg-1));
  }
    else
  {
    STACK_POP(8);
    STACK_POP(9);
    fprintf(out, "  %s t1, t1, t0\n", instr);
    STACK_PUSH(9);
  }

  return 0;
}

int RISCV::divide()
{
  int rs, rt;

  if (stack > 0)
  {
    STACK_POP(8);
    rt = 8;
  }
    else
  {
    rt = reg - 1;
    reg--;
  }

  if (stack > 0)
  {
    STACK_POP(9);
    rs = 9;
  }
    else
  {
    rs = reg - 1;
    reg--;
  }

  // MIPS manual says DIV needs a couple instructions before the mfhi / mflo.
  fprintf(out, "  div a%d, a%d\n", rs, rt);
  fprintf(out, "  nop\n");
  fprintf(out, "  nop\n");

  return 0;
}


int RISCV::get_values_from_stack(int *value)
{
  if (stack > 0)
  {
    STACK_POP(8);
    *value = 8;
  }
    else
  {
    *value = REG_STACK(reg - 1);
    reg--;
  }

  return 0;
}

int RISCV::get_values_from_stack(int *value1, int *value2)
{
  if (stack > 0)
  {
    STACK_POP(8);
    *value1 = 8;
  }
    else
  {
    *value1 = REG_STACK(reg - 1);
    reg--;
  }

  if (stack > 0)
  {
    STACK_POP(9);
    *value2 = 8;
  }
    else
  {
    *value2 = REG_STACK(reg - 1);
    reg--;
  }

  return 0;
}

int RISCV::get_ref_from_stack(int *value1)
{
  if (stack > 0)
  {
    fprintf(out, "  lw t2, 0(sp)\n"); \
    fprintf(out, "  addi sp, sp, 4\n"); \
    return -1;
  }
    else
  {
    *value1 = REG_STACK(reg - 1);
    reg--;

    return 0;
  }
}

int RISCV::set_constant(int reg, int value)
{
  fprintf(out, "  li a%d, %d\n", reg, value);

  return 0;
}

