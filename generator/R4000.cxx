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

#include "generator/R4000.h"

#define REG_STACK(a) (a)
#define LOCALS(i) (-(i * 4))

// ABI is:
// r0  $zero Always 0
// r1  $at Reserved for pseudo instructions?
// r2  $v0 Return value 0
// r3  $v1 Return value 1
// r4  $a0 Function Argument 0
// r5  $a1 Function Argument 1
// r6  $a2 Function Argument 2
// r7  $a3 Function Argument 3
// r8  $t0 Start of register stack
// r9  $t1
// r10 $t2
// r11 $t3
// r12 $t4
// r13 $t5
// r14 $t6
// r15 $t7 Top Of Stack
// r16 $s0 Saved registers 0
// r17 $s1 Saved registers 1
// r18 $s2 Saved registers 2
// r19 $s3 Saved registers 3
// r20 $s4 Saved registers 4
// r21 $s5 Saved registers 5
// r22 $s6 Saved registers 6
// r23 $s7 Saved registers 7
// r24 $t8 Temporary
// r25 $t9 Temporary
// r26 $k0 kernel (point to I/O ports or something)
// r27 $k1 kernel (do not use?)
// r28 $gp Heap pointer (point to statics)
// r29 $sp Stack pointer
// r30 $fp Frame pointer (link register)
// r31 $ra Return address

// RAM is:
// ram_start:
//   static 1..
//   static 2..
//   static 3..
//   etc
//   HEAP <--- $gp
//
//   STACK <--- $sp

R4000::R4000() :
  reg(0),
  reg_max(8),
  stack(0),
  org(0),
  ram_start(0),
  ram_end(0),
  physical_address(0),
  is_main(false)
{

}

R4000::~R4000()
{
  fprintf(out, ".align 32\n");
  insert_constants_pool();
}

int R4000::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".mips\n");

  // Set where RAM starts / ends
  //fprintf(out, "  ram_start equ 0x%x\n", ram_start);
  //fprintf(out, "  ram_end equ 0x%x\n", ram_end);

  return 0;
}

int R4000::start_init()
{
  // Add any set up items (stack, registers, etc).
  fprintf(out, ".org 0x%x\n", org);
  fprintf(out, "start:\n");

  return 0;
}

int R4000::insert_static_field_define(
  std::string &name,
  std::string &type,
  int index)
{
  fprintf(out, "  %s equ 0x%x\n", name.c_str(), ram_end - (index + 1) * 4);
  return 0;
}


int R4000::init_heap(int field_count)
{
  fprintf(out,
     "  ;; Set up heap and static initializers\n"
     "  li $gp, 0x%x\n"
     "  move $sp, $gp\n"
     "  ;; Align stack to 64 byte boundary\n"
     "  li $at, -64\n"
     "  and $sp, $sp, $at\n",
     ram_end - (field_count * 4));

  return 0;
}

int R4000::field_init_int(std::string &name, int index, int value)
{
  uint32_t n = (uint32_t)value;

  fprintf(out, "  li $t9, 0x%x\n", ram_end - ((index + 1) * 4));

  if (n == 0)
  {
    fprintf(out, "  sw $0, 0($t9)\n");
    return 0;
  }
    else
  if ((n & 0x0000ffff) == 0x0000)
  {
    fprintf(out, "  lui $t8, 0x%04x\n", n >> 16);
  }
    else
  {
    fprintf(out, "  li $t8, 0x%04x\n", n);
  }

  fprintf(out, "  sw $t8, 0x%04x($t9) ; static %s\n", index * 4, name.c_str());

  return 0;
}

int R4000::field_init_ref(std::string &name, int index)
{
  fprintf(out, "  ; static init\n");
  fprintf(out, "  li $t8, _%s\n", name.c_str());
  fprintf(out, "  li $t9, %s\n", name.c_str());
  fprintf(out, "  sw $t8, ($t9)\n");

  return 0;
}

void R4000::method_start(
  int local_count,
  int max_stack,
  int param_count,
  std::string &name)
{
  is_main = name == "main";

  fprintf(out, "%s:\n", name.c_str());
  fprintf(out, "  ; %s(local_count=%d, max_stack=%d, param_count=%d)\n", name.c_str(), local_count, max_stack, param_count);
  fprintf(out, "  addiu $fp, $sp, -4\n");
  fprintf(out, "  addiu $sp, $sp, -%d\n", local_count * 4);
}

void R4000::method_end(int local_count)
{
  if (!is_main)
  {
    // If this isn't main() restore stack pointer.
    //fprintf(out, "  addiu $sp, $sp, %d\n", local_count * 4);
  }

  fprintf(out, "\n");
}

int R4000::push_local_var_int(int index)
{
  if (reg < 8)
  {
    fprintf(out, "  lw $t%d, %d($fp) ; local_%d\n", reg, LOCALS(index), index);
    reg++;
  }
    else
  {
    fprintf(out, "  lw $t8, %d($fp) ; local_%d\n", LOCALS(index), index);
    STACK_PUSH(8)
  }

  return 0;
}

int R4000::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int R4000::push_local_var_float(int index)
{
  return push_local_var_int(index);
}

int R4000::push_ref_static(std::string &name, int index)
{
  if (reg < reg_max)
  {
    fprintf(out, "  addiu $t%d, $s1, %d\n", reg, index * 4);
    reg++;
  }
    else
  {
    fprintf(out, "  addiu $t8, $s1, %d\n", index * 4);
    STACK_PUSH(8);
  }

  return 0;
}

int R4000::push_fake()
{
  if (stack != 0) { return -1; }
  reg++;
  return 0;
}

int R4000::push_int(int32_t n)
{
  uint32_t value = (uint32_t)n;

  fprintf(out, "  ; push_int(%d)\n", n);

  if (value == 0)
  {
    if (reg < reg_max)
    {
      fprintf(out, "  move $t%d, $0\n", reg);
      reg++;
    }
      else
    {
      STACK_PUSH(0)
    }
  }
    else
  if ((value & 0x0000ffff) == 0x0000)
  {
    if (reg < reg_max)
    {
      fprintf(out, "  lui $t%d, 0x%04x\n", reg, value >> 16);
      reg++;
    }
      else
    {
      fprintf(out, "  lui $t8, 0x%04x\n", value >> 16);
      STACK_PUSH(8)
    }
  }
    else
  if (n >= 0 && n < 0xffff)
  {
    if (reg < reg_max)
    {
      fprintf(out, "  ori $t%d, $0, 0x%04x\n", reg, value);
      reg++;
    }
      else
    {
      fprintf(out, "  ori $t8, $0, 0x%04x\n", value);
      STACK_PUSH(8)
    }
  }
    else
  if (n >= -32768 && n < 32768)
  {
    if (reg < reg_max)
    {
      fprintf(out, "  addiu $t%d, $0, %d\n", reg, n);
      reg++;
    }
      else
    {
      fprintf(out, "  addiu $t8, $0, %d\n", n);
      STACK_PUSH(8)
    }
  }
    else
  {
    if (reg < reg_max)
    {
      fprintf(out, "  li $t%d, 0x%04x\n", reg, value);
      reg++;
    }
      else
    {
      fprintf(out, "  li $t8, 0x%04x\n", value);
      STACK_PUSH(8)
    }
  }

  return 0;
}

#if 0
int R4000::push_long(int64_t n)
{
  return -1;
}
#endif

int R4000::push_float(float f)
{
  uint32_t *data = (uint32_t *)&f;
  uint32_t value = *data;

  fprintf(out, "  ; push_float(%f) -- data=0x%08x\n", f, value);

  if (value == 0)
  {
    if (reg < reg_max)
    {
      fprintf(out, "  move $t%d, $0\n", reg);
      reg++;
    }
      else
    {
      STACK_PUSH(0)
    }
  }
    else
  {
    if (reg < reg_max)
    {
      fprintf(out, "  li $t%d, 0x%04x\n", reg, value);
      reg++;
    }
      else
    {
      fprintf(out, "  li $t8, 0x%04x\n", value);
      STACK_PUSH(8)
    }
  }

  return 0;
}

#if 0
int R4000::push_double(double f)
{
  return -1;
}
#endif

int R4000::push_ref(std::string &name, int index)
{
  fprintf(out, "  ; push_ref(%s)\n", name.c_str());
  fprintf(out, "  li $t8, %s\n", name.c_str());

  if (reg < reg_max)
  {
    fprintf(out, "  lw $t%d, ($t8)\n", reg);
    reg++;
  }
    else
  {
    fprintf(out, "  lw $t8, ($t8)\n");
    STACK_PUSH(8);
  }

  return 0;
}

int R4000::pop_local_var_int(int index)
{
  if (stack > 0)
  {
    STACK_POP(8);
    fprintf(out, "  sw $t8, %d($fp) ; local_%d\n", LOCALS(index), index);
  }
    else
  {
    fprintf(out, "  sw $t%d, %d($fp) ; local_%d\n", reg - 1, LOCALS(index), index);
    reg--;
  }

  return 0;
}

int R4000::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int R4000::pop_local_var_float(int index)
{
  return pop_local_var_int(index);
}

int R4000::pop()
{
  fprintf(out, "  ; pop()\n");

  if (stack > 0)
  {
    fprintf(out, "  addiu $sp, $sp, 4\n"); \
    stack--;
  }
    else
  {
    reg--;
  }

  return 0;
}

int R4000::dup()
{
  fprintf(out, "  ; dup()\n");

  if (stack > 0)
  {
    fprintf(out, "  lw $t8, 0($sp)\n"); \
    STACK_PUSH(8);
  }
    else
  {
    if (reg < 8)
    {
      fprintf(out, "  move $t%d, $t%d\n", reg, reg - 1);
      reg++;
    }
      else
    {
      STACK_PUSH(reg - 1);
    }
  }

  return 0;
}

int R4000::dup2()
{
  fprintf(out, "  ; dup2()\n");

  if (stack >= 2)
  {
    fprintf(out, "  lw $t8, -4($sp)\n");
    fprintf(out, "  lw $t9, 0($sp)\n");
    STACK_PUSH(8);
    STACK_PUSH(9);
    return 0;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  lw $t9, 0($sp)\n");
    STACK_PUSH(reg - 1);
    STACK_PUSH(9);
    return 0;
  }
    else
  if (reg < 7)
  {
    fprintf(out, "  move $t%d, $t%d\n", reg, reg - 2);
    fprintf(out, "  move $t%d, $t%d\n", reg + 1, reg - 1);
    reg += 2;
    return 0;
  }
    else
  {
    fprintf(out, "  move $t%d, $t%d\n", reg, reg - 2);
    reg++;
    STACK_PUSH(7);
  }

  return 0;
}

int R4000::swap()
{
  if (stack >= 2)
  {
    fprintf(out, "  lw $t9, 0($sp)\n");
    fprintf(out, "  lw $t8, -4($sp)\n");
    fprintf(out, "  sw $t8, 0($sp)\n");
    fprintf(out, "  sw $t9, -4($sp)\n");
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  lw $t8, 0($sp)\n");
    fprintf(out, "  sw $t7, 0($sp)\n");
    fprintf(out, "  move $t7, $t8\n");
  }
    else
  {
    fprintf(out, "  move $t8, $t%d\n", reg - 1);
    fprintf(out, "  move $t%d, $t%d\n", reg - 1, reg - 2);
    fprintf(out, "  move $t%d, $t8\n", reg - 2);
  }

  return 0;
}

int R4000::add_integer()
{
  return stack_alu("add");
}

int R4000::add_integer(int num)
{
  if (stack != 0) { return -1; }
  if (num < -32768 || num > 32767) { return -1; }
  fprintf(out, "  addiu $t%d, $t%d, %d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int R4000::sub_integer()
{
  return stack_alu("subu");
}

int R4000::sub_integer(int num)
{
  if (stack != 0) { return -1; }
  num = -num;
  if (num < -32768 || num > 32767) { return -1; }
  fprintf(out, "  addiu $t%d, $t%d, %d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int R4000::mul_integer()
{
  if (stack == 0)
  {
    fprintf(out, "  mult $t%d, $t%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
    fprintf(out, "  mflo $t%d\n", REG_STACK(reg-2));
    reg--;
  }
    else
  if (stack == 1)
  {
    STACK_POP(8);
    fprintf(out, "  mult $t%d, $t8\n", REG_STACK(reg-1));
    fprintf(out, "  mflo $t%d\n", REG_STACK(reg-1));
  }
    else
  {
    STACK_POP(8);
    STACK_POP(9);
    fprintf(out, "  mult $t9, $t8\n");
    fprintf(out, "  mflo $t9\n");
    STACK_PUSH(9);
  }

  return 0;
}

int R4000::div_integer()
{
  divide();

  if (stack > 0)
  {
    fprintf(out, "  mflo $t8\n");
    STACK_PUSH(8);
  }
    else
  {
    fprintf(out, "  mflo $t%d\n", REG_STACK(reg));
    reg++;
  }

  return 0;
}

int R4000::mod_integer()
{
  divide();

  if (stack > 0)
  {
    // OUCH.  This is probably very rare anyway
    STACK_POP(8);
    fprintf(out, "  mfhi $t8\n");
    STACK_PUSH(8);
  }
    else
  {
    fprintf(out, "  mfhi $t%d\n", REG_STACK(reg - 1));
  }

  return 0;
}

int R4000::neg_integer()
{
  if (stack > 0)
  {
    STACK_POP(8);
    fprintf(out, "  negu $t8, $t8\n");
    STACK_PUSH(8);
  }
    else
  {
    fprintf(out, "  negu $t%d, $t%d\n", REG_STACK(reg - 1), REG_STACK(reg - 1));
  }

  return 0;
}

int R4000::shift_left_integer()
{
  return stack_alu("sll");
  return 0;
}

int R4000::shift_left_integer(int num)
{
  if (num < 0 || num > 31) { printf("Shift out of range\n"); return -1; }

  if (stack > 0)
  {
    STACK_POP(8);
    fprintf(out, "  sll $t8, $t8, %d\n", num);
    STACK_PUSH(8);
  }
    else
  {
    fprintf(out, "  sll $t%d, $t%d, %d\n", REG_STACK(reg - 1), REG_STACK(reg - 1), num);
  }

  return 0;
}

int R4000::shift_right_integer()
{
  return stack_alu("sra");
  return 0;
}

int R4000::shift_right_integer(int num)
{
  if (num < 0 || num > 31) { printf("Shift out of range\n"); return -1; }

  if (stack > 0)
  {
    STACK_POP(8);
    fprintf(out, "  sra $t8, $t8, %d\n", num);
    STACK_PUSH(8);
  }
    else
  {
    fprintf(out, "  sra $t%d, $t%d, %d\n", REG_STACK(reg - 1), REG_STACK(reg - 1), num);
  }

  return 0;
}

int R4000::shift_right_uinteger()
{
  return stack_alu("srl");
  return 0;
}

int R4000::shift_right_uinteger(int num)
{
  if (num < 0 || num > 31) { printf("Shift out of range\n"); return -1; }

  if (stack > 0)
  {
    STACK_POP(8);
    fprintf(out, "  srl $t8, $t8, %d\n", num);
    STACK_PUSH(8);
  }
    else
  {
    fprintf(out, "  srl $t%d, $t%d, %d\n", REG_STACK(reg - 1), REG_STACK(reg - 1), num);
  }

  return 0;
}

int R4000::and_integer()
{
  return stack_alu("and");
}

int R4000::and_integer(int num)
{
  if (stack != 0) { return -1; }
  if (num < 0 || num > 0xffff) { return -1; }
  fprintf(out, "  andi $t%d, $t%d, %d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int R4000::or_integer()
{
  return stack_alu("or");
}

int R4000::or_integer(int num)
{
  if (stack != 0) { return -1; }
  if (num < 0 || num > 0xffff) { return -1; }
  fprintf(out, "  ori $t%d, $t%d, %d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int R4000::xor_integer()
{
  return stack_alu("xor");
}

int R4000::xor_integer(int num)
{
  if (stack != 0) { return -1; }
  if (num < 0 || num > 0xffff) { return -1; }
  fprintf(out, "  xori $t%d, $t%d, %d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int R4000::inc_integer(int index, int num)
{
  fprintf(out, "  ; inc_integer(local_%d,%d)\n", index, num);
  fprintf(out, "  lw $t8, %d($fp)\n", LOCALS(index));
  fprintf(out, "  addiu $t8, $t8, %d\n", num);
  fprintf(out, "  sw $t8, %d($fp)\n", LOCALS(index));

  return 0;
}

int R4000::integer_to_byte()
{
  fprintf(out, "  ; integer_to_byte() - sign extend byte to word\n");

  if (stack > 0)
  {
    fprintf(out, "  lb $t8, 0($sp)\n");
    fprintf(out, "  sw $t8, 0($sp)\n");
  }
    else
  {
    //fprintf(out, "  seb $t%d, $t%d\n", REG_STACK(reg - 1), REG_STACK(reg - 1));
    fprintf(out, "  sll $t%d, $t%d, 24\n", REG_STACK(reg - 1), REG_STACK(reg - 1));
    fprintf(out, "  sra $t%d, $t%d, 24\n", REG_STACK(reg - 1), REG_STACK(reg - 1));
  }

  return 0;
}

int R4000::integer_to_short()
{
  fprintf(out, "  ; integer_to_short() - sign extend halfword to word\n");

  if (stack > 0)
  {
    fprintf(out, "  lh $t8, 0($sp)\n");
    fprintf(out, "  sw $t8, 0($sp)\n");
  }
    else
  {
    //fprintf(out, "  seh $t%d, $t%d\n", REG_STACK(reg - 1), REG_STACK(reg - 1));
    fprintf(out, "  sll $t%d, $t%d, 16\n", REG_STACK(reg - 1), REG_STACK(reg - 1));
    fprintf(out, "  sra $t%d, $t%d, 16\n", REG_STACK(reg - 1), REG_STACK(reg - 1));
  }

  return 0;
}

int R4000::add_float()
{
  fprintf(out,
    "  ; add_float()\n"
    "  mtc1 $t%d, $f0\n"
    "  mtc1 $t%d, $f1\n"
    "  add.s $f0, $f0, $f1\n"
    "  mfc1 $t%d, $f0\n",
    reg - 2,
    reg - 1,
    reg - 2);

  reg -= 1;

  return 0;
}

int R4000::sub_float()
{
  fprintf(out,
    "  mtc1 $t%d, $f0\n"
    "  mtc1 $t%d, $f1\n"
    "  sub.s $f0, $f0, $f1\n"
    "  mfc1 $t%d, $f0\n",
    reg - 2,
    reg - 1,
    reg - 2);

  reg -= 1;

  return 0;
}

int R4000::mul_float()
{
  fprintf(out,
    "  mtc1 $t%d, $f0\n"
    "  mtc1 $t%d, $f1\n"
    "  mul.s $f0, $f0, $f1\n"
    "  mfc1 $t%d, $f0\n",
    reg - 2,
    reg - 1,
    reg - 2);

  reg -= 1;

  return 0;
}

int R4000::div_float()
{
  fprintf(out,
    "  mtc1 $t%d, $f0\n"
    "  mtc1 $t%d, $f1\n"
    "  div.s $f0, $f0, $f1\n"
    "  mfc1 $t%d, $f0\n",
    reg - 2,
    reg - 1,
    reg - 2);

  reg -= 1;

  return 0;
}

int R4000::neg_float()
{
  fprintf(out,
    "  mtc1 $t%d, $f0\n"
    "  neg.s $f0, $f0\n"
    "  mfc1 $t%d, $f0\n",
    reg - 1,
    reg - 1);

  return 0;
}

int R4000::float_to_integer()
{
  fprintf(out,
    "  mtc1 $t%d, $f0\n"
    "  cvt.w.s $f0, $f0\n"
    "  mfc1 $t%d, $f0\n",
    reg - 1,
    reg - 1);

  return 0;
}

int R4000::integer_to_float()
{
  fprintf(out,
    "  mtc1 $t%d, $f0\n"
    "  cvt.s.w $f0, $f0\n"
    "  mfc1 $t%d, $f0\n",
    reg - 1,
    reg - 1);

  return 0;
}

int R4000::jump_cond(std::string &label, int cond, int distance)
{
  fprintf(out, "  ; jump_cond(%s, %d, %d)\n", label.c_str(), cond, distance);

  switch (cond)
  {
    case COND_EQUAL:
      fprintf(out, "  beq $t%d, $0, %s\n", --reg, label.c_str());
      fprintf(out, "  nop\n");
      return 0;
    case COND_NOT_EQUAL:
      fprintf(out, "  bne $t%d, $0, %s\n", --reg, label.c_str());
      fprintf(out, "  nop\n");
      return 0;
    case COND_LESS:
      fprintf(out, "  bltz $t%d, %s\n", --reg, label.c_str());
      fprintf(out, "  nop\n");
      return 0;
    case COND_LESS_EQUAL:
      fprintf(out, "  blez $t%d, %s\n", --reg, label.c_str());
      fprintf(out, "  nop\n");
      return 0;
    case COND_GREATER:
      fprintf(out, "  bgtz $t%d, %s\n", --reg, label.c_str());
      fprintf(out, "  nop\n");
      return 0;
    case COND_GREATER_EQUAL:
      fprintf(out, "  bgez $t%d, %s\n", --reg, label.c_str());
      fprintf(out, "  nop\n");
      return 0;
    default:
      break;
  }

  return -1;
}

int R4000::jump_cond_integer(std::string &label, int cond, int distance)
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
      fprintf(out, "  beq $t%d, $t%d, %s\n", reg - 2, reg - 1, label.c_str());
      fprintf(out, "  nop\n");
      reg -= 2;
      return 0;
    case COND_NOT_EQUAL:
      fprintf(out, "  bne $t%d, $t%d, %s\n", reg - 2, reg - 1, label.c_str());
      fprintf(out, "  nop\n");
      reg -= 2;
      return 0;
    case COND_LESS:
      fprintf(out, "  subu $t%d, $t%d, $t%d\n", reg - 2, reg - 2, reg - 1);
      fprintf(out, "  bltz $t%d, %s\n", reg - 2, label.c_str());
      fprintf(out, "  nop\n");
      reg -= 2;
      return 0;
    case COND_LESS_EQUAL:
      fprintf(out, "  subu $t%d, $t%d, $t%d\n", reg - 2, reg - 2, reg - 1);
      fprintf(out, "  blez $t%d, %s\n", reg - 2, label.c_str());
      fprintf(out, "  nop\n");
      reg -= 2;
      return 0;
    case COND_GREATER:
      fprintf(out, "  subu $t%d, $t%d, $t%d\n", reg - 2, reg - 2, reg - 1);
      fprintf(out, "  bgtz $t%d, %s\n", reg - 2, label.c_str());
      fprintf(out, "  nop\n");
      reg -= 2;
      return 0;
    case COND_GREATER_EQUAL:
      fprintf(out, "  subu $t%d, $t%d, $t%d\n", reg - 2, reg - 2, reg - 1);
      fprintf(out, "  bgez $t%d, %s\n", reg - 2, label.c_str());
      fprintf(out, "  nop\n");
      reg -= 2;
      return 0;
    default:
      break;
  }

  return -1;
}

int R4000::compare_floats(int cond)
{
  const int float0 = reg - 2;
  const int float1 = reg - 1;

  fprintf(out,
    "  ; compare_floats(%s cond=%d)\n"
    "  mtc1 $t%d, $f0\n"
    "  mtc1 $t%d, $f1\n"
    "  li $t%d, 0\n",
    (cond == 0) ? "cmpl" : "cmpg",
    cond,
    float0,
    float1,
    reg - 2);

  fprintf(out,
    "  c.eq.s $f0, $f1\n"
    "  bc1t _compare_floats_%d\n",
    label_count);

  fprintf(out, "  li $t%d, -1\n", float0);
  fprintf(out, "  c.lt.s $f0, $f1\n");

  fprintf(out,
    "  bc1t _compare_floats_%d\n"
    "  nop\n"
    "  li $t%d, 1\n"
    "_compare_floats_%d:\n",
    label_count,
    reg - 2,
    label_count);

  label_count++;
  reg -= 1;

  return 0;
}

int R4000::ternary(int cond, int value_true, int value_false)
{
  fprintf(out, "  ; ternary %d ? %d : %d\n", cond, value_true, value_false);
  fprintf(out, "  ; true condition is in delay slot\n");

  switch (cond)
  {
    case COND_EQUAL:
      fprintf(out, "  beq $t%d, $t%d, ternary_%d\n", reg - 2, reg - 1, label_count);
      if (set_constant(reg - 2, value_true) != 0) { return -1; }
      if (set_constant(reg - 2, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      reg -= 1;
      return 0;
    case COND_NOT_EQUAL:
      fprintf(out, "  bne $t%d, $t%d, ternary_%d\n", reg - 2, reg - 1, label_count);
      if (set_constant(reg - 2, value_true) != 0) { return -1; }
      if (set_constant(reg - 2, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      reg -= 1;
      return 0;
    case COND_LESS:
      fprintf(out, "  subu $t%d, $t%d, $t%d\n", reg - 2, reg - 2, reg - 1);
      fprintf(out, "  bltz $t%d, ternary_%d\n", reg - 2, label_count);
      if (set_constant(reg - 2, value_true) != 0) { return -1; }
      if (set_constant(reg - 2, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      reg -= 1;
      return 0;
    case COND_LESS_EQUAL:
      fprintf(out, "  subu $t%d, $t%d, $t%d\n", reg - 2, reg - 2, reg - 1);
      fprintf(out, "  blez $t%d, ternary_%d\n", reg - 2, label_count);
      if (set_constant(reg - 2, value_true) != 0) { return -1; }
      if (set_constant(reg - 2, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      reg -= 1;
      return 0;
    case COND_GREATER:
      fprintf(out, "  subu $t%d, $t%d, $t%d\n", reg - 2, reg - 2, reg - 1);
      fprintf(out, "  bgtz $t%d, ternary_%d\n", reg - 2, label_count);
      if (set_constant(reg - 2, value_true) != 0) { return -1; }
      if (set_constant(reg - 2, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      reg -= 1;
      return 0;
    case COND_GREATER_EQUAL:
      fprintf(out, "  subu $t%d, $t%d, $t%d\n", reg - 2, reg - 2, reg - 1);
      fprintf(out, "  bgez $t%d, ternary_%d\n", reg - 2, label_count);
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

int R4000::ternary(int cond, int compare, int value_true, int value_false)
{
  fprintf(out, "  ; ternary %d (%d) ? %d : %d\n", cond, compare, value_true, value_false);
  fprintf(out, "  ; true condition is in delay slot\n");

  if (set_constant(8, compare) != 0) { return -1; }

  switch (cond)
  {
    case COND_EQUAL:
      fprintf(out, "  beq $t%d, $t8, ternary_%d\n", reg - 1, label_count);
      if (set_constant(reg - 1, value_true) != 0) { return -1; }
      if (set_constant(reg - 1, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      return 0;
    case COND_NOT_EQUAL:
      fprintf(out, "  bne $t%d, $t8, ternary_%d\n", reg - 1, label_count);
      if (set_constant(reg - 1, value_true) != 0) { return -1; }
      if (set_constant(reg - 1, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      return 0;
    case COND_LESS:
      fprintf(out, "  subu $t8, $t%d, $t8\n", reg - 1);
      fprintf(out, "  bltz $t8, ternary_%d\n", label_count);
      if (set_constant(reg - 1, value_true) != 0) { return -1; }
      if (set_constant(reg - 1, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      return 0;
    case COND_LESS_EQUAL:
      fprintf(out, "  subu $t8, $t%d, $t8\n", reg - 1);
      fprintf(out, "  blez $t8, ternary_%d\n", label_count);
      if (set_constant(reg - 1, value_true) != 0) { return -1; }
      if (set_constant(reg - 1, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      return 0;
    case COND_GREATER:
      fprintf(out, "  subu $t8, $t%d, $t8\n", reg - 1);
      fprintf(out, "  bgtz $t8, ternary_%d\n", label_count);
      if (set_constant(reg - 1, value_true) != 0) { return -1; }
      if (set_constant(reg - 1, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      return 0;
    case COND_GREATER_EQUAL:
      fprintf(out, "  subu $t8, $t%d, $t8\n", reg - 1);
      fprintf(out, "  bgez $t8, ternary_%d\n", label_count);
      if (set_constant(reg - 1, value_true) != 0) { return -1; }
      if (set_constant(reg - 1, value_false) != 0) { return -1; }
      fprintf(out, "ternary_%d:\n", label_count++);
      return 0;
    default:
      break;
  }

  return -1;
}

int R4000::return_local(int index, int local_count)
{
  if (reg != 0)
  {
    printf("Internal Error: Reg stack not empty %s:%d\n", __FILE__, __LINE__);
  }

  fprintf(out, "  lw $v0, %d($fp) ; local_%d\n", LOCALS(index), index);
  //fprintf(out, "  addiu $sp, $fp, %d\n", local_count * 4);
  //fprintf(out, "  move $sp, $fp\n");
  fprintf(out, "  addiu $sp, $fp, 4\n");
  fprintf(out, "  jr $ra\n");
  fprintf(out, "  nop\n");

  return 0;
}

int R4000::return_integer(int local_count)
{
  if (reg != 1)
  {
    printf("Internal Error: Reg stack not empty %s:%d\n", __FILE__, __LINE__);
  }

  fprintf(out, "  move $v0, $t0\n");
  fprintf(out, "  addiu $sp, $fp, 4\n");
  fprintf(out, "  jr $ra\n");
  fprintf(out, "  nop ; Delay slot\n");
  reg--;

  return 0;
}

int R4000::return_void(int local_count)
{
  if (reg != 0)
  {
    printf("Internal Error: Reg stack not empty %s:%d\n", __FILE__, __LINE__);
  }

  fprintf(out, "  addiu $sp, $fp, 4\n");
  fprintf(out, "  jr $ra\n");
  fprintf(out, "  nop ; Delay slot\n");
  return 0;
}

int R4000::jump(std::string &name, int distance)
{
  fprintf(out, "  b %s\n", name.c_str());
  fprintf(out, "  nop ; Delay slot\n");

  return 0;
}

int R4000::call(std::string &name)
{
  return -1;
}

int R4000::invoke_static_method(const char *name, int params, int is_void)
{
  int save_space;
  int save_regs;
  int n;
  int param_sp = 0;

  fprintf(out, "  ; invoke_static_method() name=%s params=%d is_void=%d\n", name, params, is_void);

  save_regs = reg - params;
  save_space = ((save_regs) * 4) + 8;

  // Save ra and fp
  fprintf(out, "  addiu $sp, $sp, -%d\n", save_space);
  fprintf(out, "  sw $ra, %d($sp)\n", (save_space - 4));
  fprintf(out, "  sw $fp, %d($sp)\n", (save_space - 8));

  // Save temp registers and parameter registers.
  for (n = 0; n < save_regs; n++)
  {
    fprintf(out, "  sw $t%d, %d($sp)\n", n, save_space - (n * 4) + 8);
  }

  param_sp = -4;

  // Push registers that are parameters.
  // Parameters are pushed left to right.
  for (n = save_regs; n < reg; n++)
  {
    fprintf(out, "  sw $t%d, %d($sp)\n", n, param_sp);
    param_sp -= 4;
  }

  // Setup parameters that are on the stack.
  for (n = 0; n < stack; n++)
  {
    fprintf(out, "  lw $at, %d($sp)\n", save_space + (n * 4));
    fprintf(out, "  sw $at, %d($sp)\n", param_sp);
    param_sp -= 4;
  }

  fprintf(out, "  jal %s\n", name);
  fprintf(out, "  nop ; Delay slot\n");

  // Restore temp registers
  for (n = 0; n < save_regs; n++)
  {
    fprintf(out, "  lw $t%d, %d($sp)\n", n, save_space - (n * 4) + 8);
  }

  // Restore ra and fp
  fprintf(out, "  lw $ra, %d($sp)\n", (save_space - 4));
  fprintf(out, "  lw $fp, %d($sp)\n", (save_space - 8));
  fprintf(out, "  addiu $sp, $sp, %d\n", save_space);

  // Decrease count on reg stack.
  if (stack > 0)
  {
    // Pick the min between stack and params.
    n = (stack > params) ? params : stack;
    fprintf(out, "  addiu $sp, $sp, %d\n", n * 4);
    params -= n;
  }

  reg -= params;

  // If this isn't a void, push return value onto register stack.
  if (!is_void)
  {
    if (reg < reg_max)
    {
      fprintf(out, "  move $t%d, $v0\n", REG_STACK(reg));
      reg++;
    }
      else
    {
      fprintf(out, "  addiu $sp, $sp, -%d\n", 4);
      fprintf(out, "  sw $v0, 0($sp)\n");
      stack++;
    }
  }

  return 0;
}

int R4000::put_static(std::string &name, int index)
{
  fprintf(out,
    "  ; put_static(%s, %d)\n"
    "  li $t9, %s\n",
    name.c_str(), index,
    name.c_str());

  if (stack > 0)
  {
    STACK_POP(8);
    fprintf(out, "  sw $t8, ($t9)\n");
  }
    else
  {
    fprintf(out, "  sw $t%d, ($t9)\n", reg - 1);
    reg--;
  }

  return 0;
}

int R4000::get_static(std::string &name, int index)
{
  fprintf(out,
    "  ; get_static(%s, %d)\n"
    "  li $t9, %s\n",
    name.c_str(), index,
    name.c_str());

  if (reg < reg_max)
  {
    fprintf(out, "  lw $t%d, ($t9)\n", reg++);
  }
    else
  {
    fprintf(out, "  lw $t8, ($t9)\n");
    STACK_PUSH(8);
  }

  return 0;
}

int R4000::brk()
{
  fprintf(out, "  break\n");
  return 0;
}

int R4000::new_array(uint8_t type)
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

  if (type == TYPE_INT || type == TYPE_FLOAT)
  {
    fprintf(out, "  sll $at, $t%d, 2\n", t);
  }
    else
  if (type == TYPE_SHORT || type == TYPE_CHAR)
  {
    fprintf(out, "  sll $at, $t%d, 1\n", t);
  }
    else
  {
    fprintf(out, "  move $at, $t%d\n", t);
  }

  // $at = length * sizeof(type) + 16 (4 bytes for length and 12 to align)
  fprintf(out, "  addiu $at, $at, 16\n");

  // Allocate stack space for array
  fprintf(out, "  subu $sp, $sp, $at\n");

  // Align stack
  fprintf(out,
    "  addiu $at, $0, -16\n"
    "  and $sp, $sp, $at\n");

  // Save count at start of array
  fprintf(out, "  sw $t%d, 12($sp)\n", t);

  if (t == 8)
  {
    fprintf(out, "  addiu $t9, $sp, 16\n");
    STACK_PUSH(9);
  }
    else
  {
    fprintf(out, "  addiu $t%d, $sp, 16\n", t);
  }

  return 0;
}

int R4000::new_object_array(std::string &class_name)
{
  int t;

  fprintf(out, "  ; new_object_array(%s)\n", class_name.c_str());

  if (stack > 0)
  {
    STACK_POP(8);
    t = 8;
  }
    else
  {
    t = reg - 1;
  }

  fprintf(out, "  sll $at, $t%d, 2\n", t);

  // $at = length * sizeof(type) + 16 (4 bytes for length and 12 to align)
  fprintf(out, "  addiu $at, $at, 16\n");

  // Allocate stack space for array
  fprintf(out, "  subu $sp, $sp, $at\n");

  // Align stack
  fprintf(out,
    "  addiu $at, $0, -16\n"
    "  and $sp, $sp, $at\n");

  // Save count at start of array
  fprintf(out, "  sw $t%d, 12($sp)\n", t);

  if (t == 8)
  {
    fprintf(out, "  addiu $t9, $sp, 16\n");
    STACK_PUSH(9);
  }
    else
  {
    fprintf(out, "  addiu $t%d, $sp, 16\n", t);
  }

  return 0;
}

int R4000::insert_array(std::string &name, int32_t *data, int len, uint8_t type)
{
  fprintf(out, ".align 128\n");
  fprintf(out, "  dc32 0, 0, 0\n");

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
    else
  if (type == TYPE_FLOAT)
  {
    return insert_float(name, data, len, TYPE_INT);
  }

  return -1;
}

int R4000::insert_string(std::string &name, uint8_t *bytes, int len)
{
  fprintf(out, ".align 32\n");
  fprintf(out, "  dc32 %d\n", len);
  return insert_utf8(name, bytes, len);
}

int R4000::push_array_length()
{
  if (stack > 0)
  {
    fprintf(out, "  lw $t8, 0($sp)\n"); \
    fprintf(out, "  lw $t8, -4($t8)\n");
    fprintf(out, "  sw $t8, 0($sp)\n"); \
    STACK_PUSH(8);
  }
    else
  {
    fprintf(out, "  lw $t%d, -4($t%d)\n", REG_STACK(reg-1), REG_STACK(reg-1));
  }

  return 0;
}

int R4000::push_array_length(std::string &name, int field_id)
{
  fprintf(out, "  lw $t8, 0x%04x($s1) ; static %s\n", field_id * 4, name.c_str());

  if (reg >= reg_max)
  {
    STACK_PUSH(8);
  }
    else
  {
    fprintf(out, "  lw $t%d, -4($t8)\n", REG_STACK(reg));
    reg++;
  }

  return 0;
}

int R4000::array_read_byte()
{
  int index_reg;
  int ref_reg;

  get_values_from_stack(&index_reg, &ref_reg);

  fprintf(out, "  addu $t%d, $t%d, $t%d\n", ref_reg, ref_reg, index_reg);

  if (reg < reg_max)
  {
    fprintf(out, "  lb $t%d, 0($t%d)\n", REG_STACK(reg), ref_reg);
    reg++;
  }
    else
  {
    fprintf(out, "  lb $t8, 0($t%d)\n", ref_reg);
    STACK_PUSH(8);
  }

  return 0;
}

int R4000::array_read_short()
{
  int index_reg;
  int ref_reg;

  get_values_from_stack(&index_reg, &ref_reg);

  fprintf(out, "  sll $t%d, $t%d, 1\n", index_reg, index_reg);
  fprintf(out, "  addu $t%d, $t%d, $t%d\n", ref_reg, ref_reg, index_reg);

  if (reg < reg_max)
  {
    fprintf(out, "  lh $t%d, 0($t%d)\n", REG_STACK(reg), ref_reg);
    reg++;
  }
    else
  {
    fprintf(out, "  lh $t8, 0($t%d)\n", ref_reg);
    STACK_PUSH(8);
  }

  return 0;
}

int R4000::array_read_int()
{
  int index_reg;
  int ref_reg;

  get_values_from_stack(&index_reg, &ref_reg);

  fprintf(out, "  sll $t%d, $t%d, 2\n", index_reg, index_reg);
  fprintf(out, "  addu $t%d, $t%d, $t%d\n", ref_reg, ref_reg, index_reg);

  if (reg < reg_max)
  {
    fprintf(out, "  lw $t%d, 0($t%d)\n", REG_STACK(reg), ref_reg);
    reg++;
  }
    else
  {
    fprintf(out, "  lw $t8, 0($t%d)\n", ref_reg);
    STACK_PUSH(8);
  }

  return 0;
}

int R4000::array_read_float()
{
  return array_read_int();
}

int R4000::array_read_object()
{
  return array_read_int();
}

int R4000::array_read_byte(std::string &name, int field_id)
{
  int index_reg;

  get_values_from_stack(&index_reg);

  fprintf(out, "  lw $at, %d($s1)\n", field_id * 4);
  fprintf(out, "  addu $at, $at, $t%d\n", index_reg);

  if (reg < reg_max)
  {
    fprintf(out, "  lb $t%d, 0($at)\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  lb $t8, 0($at)\n");
    STACK_PUSH(8);
  }

  return 0;
}

int R4000::array_read_short(std::string &name, int field_id)
{
  int index_reg;

  get_values_from_stack(&index_reg);

  fprintf(out, "  lw $at, %d($s1)\n", field_id * 4);
  fprintf(out, "  sll $t%d, $t%d, 1\n", index_reg, index_reg);
  fprintf(out, "  addu $at, $at, $t%d\n", index_reg);

  if (reg < reg_max)
  {
    fprintf(out, "  lh $t%d, 0($at)\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  lh $t8, 0($at)\n");
    STACK_PUSH(8);
  }

  return 0;
}

int R4000::array_read_int(std::string &name, int field_id)
{
  int index_reg;

  get_values_from_stack(&index_reg);

  fprintf(out, "  lw $at, %d($s1)\n", field_id * 4);
  fprintf(out, "  sll $t%d, $t%d, 2\n", index_reg, index_reg);
  fprintf(out, "  addu $at, $at, $t%d\n", index_reg);

  if (reg < reg_max)
  {
    fprintf(out, "  lw $t%d, 0($at)\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  lw $t8, 0($at)\n");
    STACK_PUSH(8);
  }

  return 0;
}

int R4000::array_read_float(std::string &name, int field_id)
{
  return array_read_int(name, field_id);
}

int R4000::array_read_object(std::string &name, int field_id)
{
  return array_read_int(name, field_id);
}

int R4000::array_write_byte()
{
  int value_reg;
  int index_reg;
  int ref_reg;

  get_values_from_stack(&value_reg, &index_reg);

  if (get_ref_from_stack(&ref_reg) == -1)
  {
    fprintf(out, "  addu $at, $at, $t%d\n", index_reg);
    fprintf(out, "  sb $t%d, 0($at)\n", value_reg);
  }
    else
  {
    fprintf(out, "  addu $t%d, $t%d, $t%d\n", ref_reg, ref_reg, index_reg);
    fprintf(out, "  sb $t%d, 0($t%d)\n", value_reg, ref_reg);
  }

  return 0;
}

int R4000::array_write_short()
{
  int value_reg;
  int index_reg;
  int ref_reg;

  get_values_from_stack(&value_reg, &index_reg);

  if (get_ref_from_stack(&ref_reg) == -1)
  {
    fprintf(out, "  sll $t%d, $t%d, 1\n", index_reg, index_reg);
    fprintf(out, "  addu $at, $at, $t%d\n", index_reg);
    fprintf(out, "  sh $t%d, 0($at)\n", value_reg);
  }
    else
  {
    fprintf(out, "  sll $t%d, $t%d, 1\n", index_reg, index_reg);
    fprintf(out, "  addu $t%d, $t%d, $t%d\n", ref_reg, ref_reg, index_reg);
    fprintf(out, "  sh $t%d, 0($t%d)\n", value_reg, ref_reg);
  }

  return 0;
}

int R4000::array_write_int()
{
  int value_reg;
  int index_reg;
  int ref_reg;

  get_values_from_stack(&value_reg, &index_reg);

  if (get_ref_from_stack(&ref_reg) == -1)
  {
    fprintf(out, "  sll $t%d, $t%d, 2\n", index_reg, index_reg);
    fprintf(out, "  addu $at, $at, $t%d\n", index_reg);
    fprintf(out, "  sw $t%d, 0($at)\n", value_reg);
  }
    else
  {
    fprintf(out, "  sll $t%d, $t%d, 2\n", index_reg, index_reg);
    fprintf(out, "  addu $t%d, $t%d, $t%d\n", ref_reg, ref_reg, index_reg);
    fprintf(out, "  sw $t%d, 0($t%d)\n", value_reg, ref_reg);
  }

  return 0;
}

int R4000::array_write_float()
{
  return array_write_int();
}

int R4000::array_write_object()
{
  return array_write_int();
}

int R4000::array_write_byte(std::string &name, int field_id)
{
  int value_reg;
  int index_reg;

  get_values_from_stack(&value_reg, &index_reg);

  fprintf(out, "  lw $at, %d($s1)\n", field_id * 4);
  fprintf(out, "  addu $at, $at, $t%d\n", index_reg);
  fprintf(out, "  sb $t%d, 0($at)\n", value_reg);

  return 0;
}

int R4000::array_write_short(std::string &name, int field_id)
{
  int value_reg;
  int index_reg;

  get_values_from_stack(&value_reg, &index_reg);

  fprintf(out, "  lw $at, %d($s1)\n", field_id * 4);
  fprintf(out, "  sll $t%d, $t%d, 1\n", index_reg, index_reg);
  fprintf(out, "  addu $at, $at, $t%d\n", index_reg);
  fprintf(out, "  sh $t%d, 0($at)\n", value_reg);

  return 0;
}

int R4000::array_write_int(std::string &name, int field_id)
{
  int value_reg;
  int index_reg;

  get_values_from_stack(&value_reg, &index_reg);

  fprintf(out, "  lw $at, %d($s1)\n", field_id * 4);
  fprintf(out, "  sll $t%d, $t%d, 2\n", index_reg, index_reg);
  fprintf(out, "  addu $at, $at, $t%d\n", index_reg);
  fprintf(out, "  sw $t%d, 0($at)\n", value_reg);

  return 0;
}

int R4000::array_write_float(std::string &name, int field_id)
{
  return array_write_int(name, field_id);
}

int R4000::array_write_object(std::string &name, int field_id)
{
  return array_write_int(name, field_id);
}

int R4000::cpu_nop()
{
  fprintf(out, "  nop\n");
  return 0;
}

int R4000::stack_alu(const char *instr)
{
  if (stack == 0)
  {
    fprintf(out, "  %s $t%d, $t%d, $t%d\n", instr, REG_STACK(reg-2), REG_STACK(reg-2), REG_STACK(reg-1));
    reg--;
  }
    else
  if (stack == 1)
  {
    STACK_POP(8);
    fprintf(out, "  %s $t%d, $t%d, $t8\n", instr, REG_STACK(reg-1), REG_STACK(reg-1));
  }
    else
  {
    STACK_POP(8);
    STACK_POP(9);
    fprintf(out, "  %s $t9, $t9, $t8\n", instr);
    STACK_PUSH(9);
  }

  return 0;
}

int R4000::divide()
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
  fprintf(out, "  div $t%d, $t%d\n", rs, rt);
  fprintf(out, "  nop\n");
  fprintf(out, "  nop\n");

  return 0;
}


int R4000::get_values_from_stack(int *value)
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

int R4000::get_values_from_stack(int *value1, int *value2)
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

int R4000::get_ref_from_stack(int *value1)
{
  if (stack > 0)
  {
    fprintf(out, "  lw $at, 0($sp)\n"); \
    fprintf(out, "  addiu $sp, $sp, 4\n"); \
    return -1;
  }
    else
  {
    *value1 = REG_STACK(reg - 1);
    reg--;

    return 0;
  }
}

int R4000::set_constant(int reg, int value)
{
  if (value == 0)
  {
    fprintf(out, "  move $t%d, $0\n", reg);
    return 0;
  }
    else
  if ((value & 0x0000ffff) == 0x0000)
  {
    fprintf(out, "  lui $t%d, 0x%04x\n", reg, value >> 16);
    return 0;
  }

  int index = get_constant(value);

  if (index == -1)
  {
    printf("Internal Error: Constants pool exhausted %s:%d\n", __FILE__, __LINE__);
    return -1;
  }
    else
  {
    fprintf(out, "  lw $t%d, 0x%04x($s0)\n", reg, index * 4);
  }

  return 0;
}

int R4000::memory_preloadByteArray_X(const char *array_name)
{
  if (reg < 8)
  {
    fprintf(out, "  li $t%d, _%s\n", reg, array_name);
    reg++;
  }
    else
  {
    fprintf(out, "  li $t8, _%s\n", array_name);
    STACK_PUSH(8)
  }

  return 0;
}

int R4000::memory_preloadShortArray_X(const char *array_name)
{
  if (reg < 8)
  {
    fprintf(out, "  li $t%d, _%s\n", reg, array_name);
    reg++;
  }
    else
  {
    fprintf(out, "  li $t8, _%s\n", array_name);
    STACK_PUSH(8)
  }

  return 0;
}

int R4000::memory_preloadIntArray_X(const char *array_name)
{
  if (reg < 8)
  {
    fprintf(out, "  li $t%d, _%s\n", reg, array_name);
    reg++;
  }
    else
  {
    fprintf(out, "  li $t8, _%s\n", array_name);
    STACK_PUSH(8)
  }

  return 0;
}

