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

#include "MIPS32.h"

#define REG_STACK(a) (a)
#define LOCALS(i) (i * 4)

#define STACK_PUSH(num) \
  fprintf(out, "  addi $sp, $sp, -4\n"); \
  fprintf(out, "  sw $%d, 0($sp)\n", num);

#define STACK_POP() \
  fprintf(out, "  lw $, 0($sp)\n"); \
  fprintf(out, "  addi $sp, $sp, 4\n"); \

// ABI is:
// r0  $zero Always 0
// r1  $at Reserved for pseudo instructions?
// r2  $v0 Return value 0
// r3  $v1 Return value 1
// r4  $a0 Function Argument 0
// r5  $a1 Function Argument 1
// r6  $a2 Function Argument 2
// r7  $a3 Function Argument 3
// r8  $t0 Start of stack
// r9  $t1
// r10 $t2
// r11 $t3
// r12 $t4
// r13 $t5
// r14 $t6
// r15 $t7 Top Of Stack
// r16 $s0 Saved registers 0  (point to constant pool)
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
// r28 $gp Heap pointer
// r29 $sp Stack pointer
// r30 $fp Frame pointer (link register)
// r31 $ra Return address

MIPS32::MIPS32() :
  reg(0),
  reg_max(8),
  stack(0),
  org(0),
  ram_start(0),
  ram_end(0),
  is_main(0)
{

}

MIPS32::~MIPS32()
{
  insert_constants_pool();
}

int MIPS32::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".mips32\n");

  // Set where RAM starts / ends
  fprintf(out, "ram_start equ 0x%x\n", ram_start);
  fprintf(out, "ram_end equ 0x%x\n", ram_end);

  return 0;
}

int MIPS32::start_init()
{
  // Add any set up items (stack, registers, etc).
  fprintf(out, ".org 0x%x\n", org);
  fprintf(out, "start:\n");

  // REVIEW - Use li instead of real instructions?
  fprintf(out, "  lui $s0, constant_pool >> 16\n");
  fprintf(out, "  ori $s0, (constant_pool & 0xffff)\n");

  return 0;
}

int MIPS32::insert_static_field_define(const char *name, const char *type, int index)
{
  fprintf(out, "%s equ ram_start+%d\n", name, (index + 1) * 4);
  return 0;
}


int MIPS32::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");
  fprintf(out, "  li $gp, ram_start+%d\n", (field_count + 1) * 4);
  return 0;
}

int MIPS32::insert_field_init_boolean(char *name, int index, int value)
{
  return -1;
}

int MIPS32::insert_field_init_byte(char *name, int index, int value)
{
  return -1;
}

int MIPS32::insert_field_init_short(char *name, int index, int value)
{
  return -1;
}

int MIPS32::insert_field_init_int(char *name, int index, int value)
{
  return -1;
}

int MIPS32::insert_field_init(char *name, int index)
{
  return -1;
}

void MIPS32::method_start(int local_count, int max_stack, int param_count, const char *name)
{
}

void MIPS32::method_end(int local_count)
{
}

int MIPS32::push_integer(int32_t n)
{
#if 0
  if (n > 0x7fffffff || n < 0x80000000)
  {
    printf("Error: literal value %d bigger than signed 32 bit.\n", n);
    return -1;
  }
#endif

  if (reg < reg_max)
  {
    fprintf(out, "  li $t%d, %d\n", reg, n);
    reg++;
  }
    else
  {
    STACK_PUSH(n)
  }

  return 0;
}

int MIPS32::push_integer_local(int index)
{
  return -1;
}

int MIPS32::push_ref_static(const char *name, int index)
{
  return -1;
}

int MIPS32::push_ref_local(int index)
{
  return push_integer_local(index);
}

int MIPS32::push_fake()
{
  if (stack != 0) { return -1; }
  reg++;
  return 0;
}

int MIPS32::push_long(int64_t n)
{
  return -1;
}

int MIPS32::push_float(float f)
{
  return -1;
}

int MIPS32::push_double(double f)
{
  return -1;
}

int MIPS32::push_byte(int8_t b)
{
  if (reg < reg_max)
  {
    fprintf(out, "  li $t%d, 0x%x\n", reg, (int32_t)b);
    reg++;
  }
    else
  {
    STACK_PUSH(((int32_t)b))
  }

  return 0;
}

int MIPS32::push_short(int16_t s)
{
  if (reg < reg_max)
  {
    fprintf(out, "  li $t%d, 0x%x\n", reg, (int32_t)s);
    reg++;
  }
    else
  {
    STACK_PUSH(((int32_t)s))
  }

  return 0;
}

int MIPS32::push_ref(char *name)
{
  return -1;
}

int MIPS32::pop_integer_local(int index)
{
  return -1;
}

int MIPS32::pop_ref_local(int index)
{
  return pop_integer_local(index);
}

int MIPS32::pop()
{
  return -1;
}

int MIPS32::dup()
{
  return -1;
}

int MIPS32::dup2()
{
  return -1;
}

int MIPS32::swap()
{
  return -1;
}

int MIPS32::add_integer()
{
  return stack_alu("add"); 
}

int MIPS32::add_integer(int num)
{
  if (stack != 0) { return -1; }
  if (num < 0 || num > 0xffff) { return -1; }
  fprintf(out, "  addi $t%d, $t%d, %d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int MIPS32::sub_integer()
{
  return stack_alu("sub"); 
}

int MIPS32::sub_integer(int num)
{
  if (stack != 0) { return -1; }
  if (num < 0 || num > 0xffff) { return -1; }
  fprintf(out, "  subi $t%d, $t%d, %d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int MIPS32::mul_integer()
{
  return -1;
}

int MIPS32::div_integer()
{
  return -1;
}

int MIPS32::mod_integer()
{
  return -1;
}

int MIPS32::neg_integer()
{
  return -1;
}

int MIPS32::shift_left_integer()
{
  return -1;
}

int MIPS32::shift_left_integer(int num)
{
  return -1;
}

int MIPS32::shift_right_integer()
{
  return -1;
}

int MIPS32::shift_right_integer(int num)
{
  return -1;
}

int MIPS32::shift_right_uinteger()
{
  return -1;
}

int MIPS32::shift_right_uinteger(int num)
{
  return -1;
}

int MIPS32::and_integer()
{
  return stack_alu("and"); 
}

int MIPS32::and_integer(int num)
{
  if (stack != 0) { return -1; }
  if (num < 0 || num > 0xffff) { return -1; }
  fprintf(out, "  andi $t%d, $t%d, %d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int MIPS32::or_integer()
{
  return stack_alu("or"); 
}

int MIPS32::or_integer(int num)
{
  if (stack != 0) { return -1; }
  if (num < 0 || num > 0xffff) { return -1; }
  fprintf(out, "  ori $t%d, $t%d, %d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int MIPS32::xor_integer()
{
  return stack_alu("xor"); 
}

int MIPS32::xor_integer(int num)
{
  if (stack != 0) { return -1; }
  if (num < 0 || num > 0xffff) { return -1; }
  fprintf(out, "  xori $t%d, $t%d, %d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int MIPS32::inc_integer(int index, int num)
{
  return -1;
}

int MIPS32::integer_to_byte()
{
  return -1;
}

int MIPS32::integer_to_short()
{
  return -1;
}

int MIPS32::jump_cond(const char *label, int cond, int distance)
{
  return -1;
}

int MIPS32::jump_cond_integer(const char *label, int cond, int distance)
{
  return -1;
}

int MIPS32::return_local(int index, int local_count)
{
  return -1;
}

int MIPS32::return_integer(int local_count)
{
  return -1;
}

int MIPS32::return_void(int local_count)
{
  return -1;
}

int MIPS32::jump(const char *name, int distance)
{
  fprintf(out, "  beq name\n");

  return 0;
}

int MIPS32::call(const char *name)
{
  return -1;
}

int MIPS32::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int MIPS32::put_static(const char *name, int index)
{
  return -1;
}

int MIPS32::get_static(const char *name, int index)
{
  return -1;
}

int MIPS32::brk()
{
  return -1;
}

int MIPS32::new_array(uint8_t type)
{
  return -1;
}

int MIPS32::insert_array(const char *name, int32_t *data, int len, uint8_t type)
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

int MIPS32::insert_string(const char *name, uint8_t *bytes, int len)
{
  fprintf(out, ".align 32\n");
  fprintf(out, "  dc32 %d\n", len);
  return insert_utf8(name, bytes, len);
}

int MIPS32::push_array_length()
{
  return -1;
}

int MIPS32::push_array_length(const char *name, int field_id)
{
  return -1;
}

int MIPS32::array_read_byte()
{
  return -1;
}

int MIPS32::array_read_short()
{
  return -1;
}

int MIPS32::array_read_int()
{
  return -1;
}

int MIPS32::array_read_byte(const char *name, int field_id)
{
  return -1;
}

int MIPS32::array_read_short(const char *name, int field_id)
{
  return -1;
}

int MIPS32::array_read_int(const char *name, int field_id)
{
  return -1;
}

int MIPS32::array_write_byte()
{
  return -1;
}

int MIPS32::array_write_short()
{
  return -1;
}

int MIPS32::array_write_int()
{
  return -1;
}

int MIPS32::array_write_byte(const char *name, int field_id)
{
  return -1;
}

int MIPS32::array_write_short(const char *name, int field_id)
{
  return -1;
}

int MIPS32::array_write_int(const char *name, int field_id)
{
  return -1;
}

int MIPS32::stack_alu(const char *instr)
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


