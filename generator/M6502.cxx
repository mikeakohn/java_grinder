/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2019 by Michael Kohn, Joe Davisson
 *
 * M6502 written by Joe Davisson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "generator/M6502.h"

// ABI is:
// A - accumulator
// X - java stack index register
// Y - general-purpose index register

#define LOCALS(a) (a)

M6502::M6502() :
  stack(0),
  start_org(0x400),
  java_stack_lo(0x200),
  java_stack_hi(0x300),
  ram_start(0xa000),
  label_count(0),
  is_main(0),

  need_swap(0),
  need_add_integer(0),
  need_sub_integer(0),
  need_mul_integer(0),
  need_div_integer(0),
  need_neg_integer(0),
  need_shift_left_integer(0),
  need_shift_right_integer(0),
  need_shift_right_uinteger(0),
  need_and_integer(0),
  need_or_integer(0),
  need_xor_integer(0),
  need_integer_to_byte(0),
  need_dup(0),
  need_dup2(0),
  need_push_array_length(0),
  need_push_array_length2(0),
  need_array_byte_support(0),
  need_array_int_support(0),
  need_memory_read8(0),
  need_memory_write8(0),
  need_memory_read16(0),
  need_memory_write16(0),
  need_math_max(0),
  need_math_min(0),
  need_math_abs(0)
{
}

M6502::~M6502()
{
}

int M6502::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".6502\n");

  // heap
  fprintf(out, "ram_start equ 0x%04x\n", ram_start);
  fprintf(out, "heap_ptr equ ram_start\n");

  // for indirection (2 bytes)
  fprintf(out, "address equ 0xc0\n");

  // java stack
  fprintf(out, "stack_lo equ 0x%04x\n", java_stack_lo);
  fprintf(out, "stack_hi equ 0x%04x\n", java_stack_hi);

  // points to locals
  fprintf(out, "locals equ 0xc2\n");

  // temp variables
  fprintf(out, "result equ 0xc4\n");
  fprintf(out, "remainder equ 0xc6\n");
  fprintf(out, "length equ 0xc8\n");
  fprintf(out, "value1 equ 0xca\n");
  fprintf(out, "value2 equ 0xcc\n");
  fprintf(out, "value3 equ 0xce\n");

  // start at 0x0400 when using simulator
  fprintf(out, ".org 0x%04x\n", start_org);
  fprintf(out, "reset:\n");
  fprintf(out, "  sei\n");
  fprintf(out, "  cld\n");
  fprintf(out, "  ldx #0xff\n");
  fprintf(out, "  txs\n");

  return 0;
}

int M6502::finish()
{
  if (need_swap) { insert_swap(); }
  if (need_add_integer) { insert_add_integer(); }
  if (need_sub_integer) { insert_sub_integer(); }
  if (need_mul_integer) { insert_mul_integer(); }
  if (need_div_integer) { insert_div_integer(); }
  if (need_neg_integer) { insert_neg_integer(); }
  if (need_shift_left_integer) { insert_shift_left_integer(); } 
  if (need_shift_right_integer) { insert_shift_right_integer(); } 
  if (need_shift_right_uinteger) { insert_shift_right_uinteger(); }
  if (need_and_integer) { insert_and_integer(); }
  if (need_or_integer) { insert_or_integer(); } 
  if (need_xor_integer) { insert_xor_integer(); }
  if (need_integer_to_byte) { insert_integer_to_byte(); }
  if (need_dup) { insert_dup(); } 
  if (need_dup2) { insert_dup2(); }
  if (need_push_array_length) { insert_push_array_length(); } 
  if (need_push_array_length2) { insert_push_array_length2(); }
  if (need_array_byte_support) { insert_array_byte_support(); }
  if (need_array_int_support) { insert_array_int_support(); }
  if (need_memory_read8) { insert_memory_read8(); } 
  if (need_memory_write8) { insert_memory_write8(); }
  if (need_memory_read16) { insert_memory_read16(); } 
  if (need_memory_write16) { insert_memory_write16(); }
  if (need_math_max) { insert_math_max(); }
  if (need_math_min) { insert_math_min(); }
  if (need_math_abs) { insert_math_abs(); }

  return 0;
}

int M6502::start_init()
{
  return 0;
}

int M6502::insert_static_field_define(std::string &name, std::string &type, int index)
{
  fprintf(out, "  %s equ ram_start + %d\n", name.c_str(), (index + 1) * 2);

  return 0;
}

int M6502::init_heap(int field_count)
{
  fprintf(out, "  ; set up heap and static initializers\n");
  fprintf(out, "  lda #(ram_start + %d) & 0xff\n", (field_count + 1) * 2);
  fprintf(out, "  sta ram_start + 0\n");
  fprintf(out, "  lda #(ram_start + %d) >> 8\n", (field_count + 1) * 2);
  fprintf(out, "  sta ram_start + 1\n");

  return 0;
}

#if 0
int M6502::field_init_boolean(char *name, int index, int value)
{
  value = (value == 0) ? 0 : 1;
  fprintf(out, "; field_init_boolean\n");
  fprintf(out, "  lda #%d\n", value & 0xff);
  fprintf(out, "  sta %s + 0\n", name);
  fprintf(out, "  lda #%d\n", value >> 8);
  fprintf(out, "  sta %s + 1\n", name);

  return 0;
}

int M6502::field_init_byte(char *name, int index, int value)
{
  if (value < -128 || value > 255) { return -1; }
  int16_t n = value;
  uint16_t v = (n & 0xffff);


  fprintf(out, "; field_init_byte\n");
  fprintf(out, "  lda #%d\n", (uint8_t)v & 0xff);
  fprintf(out, "  sta %s + 0\n", name);
  fprintf(out, "  lda #%d\n", (uint8_t)v >> 8);
  fprintf(out, "  sta %s + 1\n", name);

  return 0;
}

int M6502::field_init_short(char *name, int index, int value)
{
  if (value < -32768 || value > 65535) { return -1; }

  fprintf(out, "; field_init_short\n");
  fprintf(out, "  lda #%d\n", value & 0xff);
  fprintf(out, "  sta %s + 0\n", name);
  fprintf(out, "  lda #%d\n", value >> 8);
  fprintf(out, "  sta %s + 1\n", name);

  return 0;
}
#endif

int M6502::field_init_int(std::string &name, int index, int value)
{
  if (value < -32768 || value > 65535) { return -1; }

  fprintf(out, "; field_init_int\n");
  fprintf(out, "  lda #%d\n", value & 0xff);
  fprintf(out, "  sta %s + 0\n", name.c_str());
  fprintf(out, "  lda #%d\n", value >> 8);
  fprintf(out, "  sta %s + 1\n", name.c_str());

  return 0;
}

int M6502::field_init_ref(std::string &name, int index)
{
  fprintf(out, "; field_init_ref\n");
  fprintf(out, "  lda #_%s & 0xff\n", name.c_str());
  fprintf(out, "  sta %s + 0\n", name.c_str());
  fprintf(out, "  lda #_%s >> 8\n", name.c_str());
  fprintf(out, "  sta %s + 1\n", name.c_str());

  return 0;
}

void M6502::method_start(int local_count, int max_stack, int param_count, std::string &name)
{
  stack = 0;
  is_main = (name == "main") ? 1 : 0;

  fprintf(out, "%s:\n", name.c_str());

  // main() function goes here
  if (!is_main)
  {
    fprintf(out, "  lda locals\n");
    fprintf(out, "  sta stack_lo,x\n");
    fprintf(out, "  lda #0\n");
    fprintf(out, "  sta stack_hi,x\n");
    fprintf(out, "  dex\n");
  }

  fprintf(out, "  stx locals\n");
  fprintf(out, "  txa\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #0x%02x\n", local_count);
  fprintf(out, "  tax\n");
}

void M6502::method_end(int local_count)
{
  fprintf(out, "\n");
}

int M6502::push_local_var_int(int index)
{
  fprintf(out, "; push_local_var_int\n");
  fprintf(out, "  ldy locals\n");
  fprintf(out, "  lda stack_lo - %d,y\n", LOCALS(index));
  fprintf(out, "  sta stack_lo,x\n");
  fprintf(out, "  lda stack_hi - %d,y\n", LOCALS(index));
  fprintf(out, "  sta stack_hi,x\n");
  fprintf(out, "  dex\n");
  stack++;

  return 0;
}

int M6502::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int M6502::push_ref_static(std::string &name, int index)
{
  fprintf(out, "; push_ref_static\n");
  fprintf(out, "  lda #_%s & 0xff\n", name.c_str());
  fprintf(out, "  sta stack_lo,x\n");
  fprintf(out, "  lda #_%s >> 8\n", name.c_str());
  fprintf(out, "  sta stack_hi,x\n");
  fprintf(out, "  dex\n");
  stack++;

  return 0;
}

int M6502::push_fake()
{
  fprintf(out, "; push_fake()\n");
  fprintf(out, "  dex\n");
  stack++;

  return 0;
}

int M6502::push_int(int32_t n)
{
  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);
    return -1;
  }

  uint16_t value = (n & 0xffff);

  fprintf(out, "; push_int\n");
  fprintf(out, "  lda #0x%02x\n", value & 0xff);
  fprintf(out, "  sta stack_lo,x\n");
  fprintf(out, "  lda #0x%02x\n", value >> 8);
  fprintf(out, "  sta stack_hi,x\n");
  fprintf(out, "  dex\n");
  stack++;

  return 0;
}

int M6502::push_ref(std::string &name)
{
  fprintf(out, "; push_ref\n");
  fprintf(out, "  lda %s + 0\n", name.c_str());
  fprintf(out, "  sta stack_lo,x\n");
  fprintf(out, "  lda %s + 1\n", name.c_str());
  fprintf(out, "  sta stack_hi,x\n");
  fprintf(out, "  dex\n");
  stack++;

  return 0;
}

int M6502::pop_local_var_int(int index)
{
  fprintf(out, "; pop_local_var_int\n");
  fprintf(out, "  ldy locals\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta stack_lo - %d,y\n", LOCALS(index));
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  sta stack_hi - %d,y\n", LOCALS(index));
  stack--;

  return 0;
}

int M6502::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int M6502::pop()
{
  fprintf(out, "; pop\n");
  fprintf(out, "  inx\n");
  stack--;

  return 0;
}

int M6502::dup()
{
  need_dup = 1;
  fprintf(out, "; dup\n");
  fprintf(out, "  jsr dup\n");
  stack++;

  return 0;
}

int M6502::dup2()
{
  need_dup2 = 1;
  fprintf(out, "; dup2\n");
  fprintf(out, "  jsr dup2\n");
  stack += 2;

  return 0;
}

int M6502::swap()
{
  need_swap = 1;
  fprintf(out, "  jsr swap\n");

  return 0;
}

int M6502::add_integer()
{
  need_add_integer = 1;
  fprintf(out, "  jsr add_integer\n");
  stack--;

  return 0;
}

int M6502::add_integer(int const_val)
{
  fprintf(out, "; add_integer (const_val)\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda #%d\n", const_val & 0xff);
  fprintf(out, "  adc stack_lo + 1,x\n");
  fprintf(out, "  sta stack_lo + 1,x\n");
  fprintf(out, "  lda #%d\n", const_val >> 8);
  fprintf(out, "  adc stack_hi + 1,x\n");
  fprintf(out, "  sta stack_hi + 1,x\n");

  return 0;
}

int M6502::sub_integer()
{
  need_sub_integer = 1;
  fprintf(out, "  jsr sub_integer\n");
  stack--;

  return 0;
}

int M6502::sub_integer(int const_val)
{
  fprintf(out, "; sub_integer (const_val)\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  lda stack_lo + 1,x\n");
  fprintf(out, "  sbc #%d\n", const_val & 0xff);
  fprintf(out, "  sta stack_lo + 1,x\n");
  fprintf(out, "  lda stack_hi + 1,x\n");
  fprintf(out, "  sbc #%d\n", const_val >> 8);
  fprintf(out, "  sta stack_hi + 1,x\n");

  return 0;
}

int M6502::mul_integer()
{
  need_mul_integer = 1;
  fprintf(out, "  jsr mul_integer\n");
  stack--;

  return 0;
}

// unsigned only for now
int M6502::div_integer()
{
  need_div_integer = 1;
  fprintf(out, "  jsr div_integer\n");
  stack--;

  return 0;
}

// unsigned only for now
int M6502::mod_integer()
{
  need_div_integer = 1;
  fprintf(out, "  jsr div_integer\n");
  fprintf(out, "  lda remainder + 0\n");
  fprintf(out, "  sta stack_lo + 1,x\n");
  fprintf(out, "  lda remainder + 1\n");
  fprintf(out, "  sta stack_hi + 1,x\n");
  stack--;

  return 0;
}

int M6502::neg_integer()
{
  need_neg_integer = 1;
  fprintf(out, "  jsr neg_integer\n");

  return 0;
}

int M6502::shift_left_integer()
{
  need_shift_left_integer = 1;
  fprintf(out, "  jsr shift_left_integer\n");
  stack--;

  return 0;
}

int M6502::shift_left_integer(int const_val)
{
  return -1;
}

int M6502::shift_right_integer()
{
  need_shift_right_integer = 1;
  fprintf(out, "  jsr shift_right_integer\n");
  stack--;

  return 0;
}

int M6502::shift_right_integer(int const_val)
{
  return -1;
}

int M6502::shift_right_uinteger()
{
  need_shift_right_uinteger = 1;
  fprintf(out, "  jsr shift_right_uinteger\n");
  stack--;

  return 0;
}

int M6502::shift_right_uinteger(int const_val)
{
  return -1;
}

int M6502::and_integer()
{
  need_and_integer = 1;
  fprintf(out, "  jsr and_integer\n");
  stack--;

  return 0;
}

int M6502::and_integer(int const_val)
{
  fprintf(out, "; and_integer (const_val)");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda #%d\n", const_val & 0xff);
  fprintf(out, "  and stack_lo + 1,x\n");
  fprintf(out, "  sta stack_lo + 1,x\n");
  fprintf(out, "  lda #%d\n", const_val >> 8);
  fprintf(out, "  and stack_hi + 1,x\n");
  fprintf(out, "  sta stack_hi + 1,x\n");

  return 0;
}

int M6502::or_integer()
{
  need_or_integer = 1;
  fprintf(out, "  jsr or_integer\n");
  stack--;

  return 0;
}

int M6502::or_integer(int const_val)
{
  fprintf(out, "; or_integer (const_val)");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda #%d\n", const_val & 0xff);
  fprintf(out, "  ora stack_lo + 1,x\n");
  fprintf(out, "  sta stack_lo + 1,x\n");
  fprintf(out, "  lda #%d\n", const_val >> 8);
  fprintf(out, "  ora stack_hi + 1,x\n");
  fprintf(out, "  sta stack_hi + 1,x\n");

  return 0;
}

int M6502::xor_integer()
{
  need_xor_integer = 1;
  fprintf(out, "  jsr xor_integer\n");
  stack--;

  return 0;
}

int M6502::xor_integer(int const_val)
{
  fprintf(out, "; xor_integer (const_val)");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda #%d\n", const_val & 0xff);
  fprintf(out, "  eor stack_lo + 1,x\n");
  fprintf(out, "  sta stack_lo + 1,x\n");
  fprintf(out, "  lda #%d\n", const_val >> 8);
  fprintf(out, "  eor stack_hi + 1,x\n");
  fprintf(out, "  sta stack_hi + 1,x\n");

  return 0;
}

int M6502::inc_integer(int index, int num)
{
  uint16_t value = num & 0xffff;

  fprintf(out, "; inc_integer num = %d\n", num);

  if (num == 1)
  {
    fprintf(out, "  txa\n");
    fprintf(out, "  ldx locals\n");
    fprintf(out, "  inc stack_lo - %d,x\n", LOCALS(index));
    fprintf(out, "  bne inc_integer_%d\n", label_count);
    fprintf(out, "  inc stack_hi - %d,x\n", LOCALS(index));
    fprintf(out, "inc_integer_%d:\n", label_count++);
    fprintf(out, "  tax\n");
  }
    else
  {
    fprintf(out, "  ldy locals\n");
    fprintf(out, "  clc\n");
    fprintf(out, "  lda stack_lo - %d,y\n", LOCALS(index));
    fprintf(out, "  adc #0x%02x\n", value & 0xff);
    fprintf(out, "  sta stack_lo - %d,y\n", LOCALS(index));
    fprintf(out, "  lda stack_hi - %d,y\n", LOCALS(index));
    fprintf(out, "  adc #0x%02x\n", value >> 8);
    fprintf(out, "  sta stack_hi - %d,y\n", LOCALS(index));
  }

  return 0;
}

int M6502::integer_to_byte()
{
  need_integer_to_byte = 1;
  fprintf(out, "jsr integer_to_byte\n");

  return 0;
}

int M6502::integer_to_short()
{
  // integers are already shorts so do nothing
  return 0;
}

int M6502::jump_cond(std::string &label, int cond, int distance)
{
  bool reverse = false;

  fprintf(out, "; jump_cond\n");
  fprintf(out, "  inx\n");

  if (cond == COND_LESS_EQUAL)
  {
    reverse = true;
    cond = COND_GREATER_EQUAL;
  }
    else
  if (cond == COND_GREATER)
  {
    reverse = true;
    cond = COND_LESS;
  }

  switch (cond)
  {
    case COND_EQUAL:
      fprintf(out, "  lda stack_lo - 0,x\n");
      fprintf(out, "  cmp #0\n");
      fprintf(out, "  bne cond_equal_skip_%d\n", label_count);
      fprintf(out, "  lda stack_hi - 0,x\n");
      fprintf(out, "  cmp #0\n");
      fprintf(out, "  bne cond_equal_skip_%d\n", label_count);
      fprintf(out, "  jmp %s\n", label.c_str());
      fprintf(out, "cond_equal_skip_%d:\n", label_count++);
      break;
    case COND_NOT_EQUAL:
      fprintf(out, "  lda stack_lo - 0,x\n");
      fprintf(out, "  cmp #0\n");
      fprintf(out, "  bne cond_not_equal_skip_%d - 3\n", label_count);
      fprintf(out, "  lda stack_hi - 0,x\n");
      fprintf(out, "  cmp #0\n");
      fprintf(out, "  beq cond_not_equal_skip_%d\n", label_count);
      fprintf(out, "  jmp %s\n", label.c_str());
      fprintf(out, "cond_not_equal_skip_%d:\n", label_count++);
      break;
    case COND_LESS:
      if (reverse == false)
      {
        fprintf(out, "  lda stack_lo - 0,x\n");
        fprintf(out, "  cmp #0\n");
        fprintf(out, "  lda stack_hi - 0,x\n");
        fprintf(out, "  sbc #0\n");
        fprintf(out, "  bvc #2\n");
        fprintf(out, "  eor #0x80\n");
        fprintf(out, "  bpl #3\n");
        fprintf(out, "  jmp %s\n", label.c_str());
      }
        else
      {
        fprintf(out, "  lda #0\n");
        fprintf(out, "  cmp stack_lo - 0,x\n");
        fprintf(out, "  lda #0\n");
        fprintf(out, "  sbc stack_hi - 0,x\n");
        fprintf(out, "  bvc #2\n");
        fprintf(out, "  eor #0x80\n");
        fprintf(out, "  bpl #3\n");
        fprintf(out, "  jmp %s\n", label.c_str());
      }
      break;
    case COND_GREATER_EQUAL:
      if (reverse == false)
      {
        fprintf(out, "  lda stack_lo - 0,x\n");
        fprintf(out, "  cmp #0\n");
        fprintf(out, "  lda stack_hi - 0,x\n");
        fprintf(out, "  sbc #0\n");
        fprintf(out, "  bvc #2\n");
        fprintf(out, "  eor #0x80\n");
        fprintf(out, "  bmi #3\n");
        fprintf(out, "  jmp %s\n", label.c_str());
      }
        else
      {
        fprintf(out, "  lda #0\n");
        fprintf(out, "  cmp stack_lo - 0,x\n");
        fprintf(out, "  lda #0\n");
        fprintf(out, "  sbc stack_hi - 0,x\n");
        fprintf(out, "  bvc #2\n");
        fprintf(out, "  eor #0x80\n");
        fprintf(out, "  bmi #3\n");
        fprintf(out, "  jmp %s\n", label.c_str());
      }
      break;
  }

  stack--;

  return 0;
}

int M6502::jump_cond_integer(std::string &label, int cond, int distance)
{
  bool reverse = false;

  fprintf(out, "; jump_cond_integer\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  inx\n");

  if (cond == COND_LESS_EQUAL)
  {
    reverse = true;
    cond = COND_GREATER_EQUAL;
  }
    else if (cond == COND_GREATER)
  {
    reverse = true;
    cond = COND_LESS;
  }

  switch (cond)
  {
    case COND_EQUAL:
      fprintf(out, "  lda stack_lo - 0,x\n");
      fprintf(out, "  cmp stack_lo - 1,x\n");
      fprintf(out, "  bne cond_integer_equal_skip_%d\n", label_count);
      fprintf(out, "  lda stack_hi - 0,x\n");
      fprintf(out, "  cmp stack_hi - 1,x\n");
      fprintf(out, "  bne cond_integer_equal_skip_%d\n", label_count);
      fprintf(out, "  jmp %s\n", label.c_str());
      fprintf(out, "cond_integer_equal_skip_%d:\n", label_count++);
      break;
    case COND_NOT_EQUAL:
      fprintf(out, "  lda stack_lo - 0,x\n");
      fprintf(out, "  cmp stack_lo - 1,x\n");
      fprintf(out, "  bne cond_integer_not_equal_skip_%d - 3\n", label_count);
      fprintf(out, "  lda stack_hi - 0,x\n");
      fprintf(out, "  cmp stack_hi - 1,x\n");
      fprintf(out, "  beq cond_integer_not_equal_skip_%d\n", label_count);
      fprintf(out, "  jmp %s\n", label.c_str());
      fprintf(out, "cond_integer_not_equal_skip_%d:\n", label_count++);
      break;
    case COND_LESS:
      if (reverse == false)
      {
        fprintf(out, "  lda stack_lo - 0,x\n");
        fprintf(out, "  cmp stack_lo - 1,x\n");
        fprintf(out, "  lda stack_hi - 0,x\n");
        fprintf(out, "  sbc stack_hi - 1,x\n");
        fprintf(out, "  bvc #2\n");
        fprintf(out, "  eor #0x80\n");
        fprintf(out, "  bpl #3\n");
        fprintf(out, "  jmp %s\n", label.c_str());
      }
        else
      {
        fprintf(out, "  lda stack_lo - 1,x\n");
        fprintf(out, "  cmp stack_lo - 0,x\n");
        fprintf(out, "  lda stack_hi - 1,x\n");
        fprintf(out, "  sbc stack_hi - 0,x\n");
        fprintf(out, "  bvc #2\n");
        fprintf(out, "  eor #0x80\n");
        fprintf(out, "  bpl #3\n");
        fprintf(out, "  jmp %s\n", label.c_str());
      }
      break;
    case COND_GREATER_EQUAL:
      if (reverse == false)
      {
        fprintf(out, "  lda stack_lo - 0,x\n");
        fprintf(out, "  cmp stack_lo - 1,x\n");
        fprintf(out, "  lda stack_hi - 0,x\n");
        fprintf(out, "  sbc stack_hi - 1,x\n");
        fprintf(out, "  bvc #2\n");
        fprintf(out, "  eor #0x80\n");
        fprintf(out, "  bmi #3\n");
        fprintf(out, "  jmp %s\n", label.c_str());
      }
        else
      {
        fprintf(out, "  lda stack_lo - 1,x\n");
        fprintf(out, "  cmp stack_lo - 0,x\n");
        fprintf(out, "  lda stack_hi - 1,x\n");
        fprintf(out, "  sbc stack_hi - 0,x\n");
        fprintf(out, "  bvc #2\n");
        fprintf(out, "  eor #0x80\n");
        fprintf(out, "  bmi #3\n");
        fprintf(out, "  jmp %s\n", label.c_str());
      }
      break;
  }

  stack -= 2;

  return 0;
}

int M6502::ternary(int cond, int value_true, int value_false)
{
  return -1;
}

int M6502::ternary(int cond, int compare, int value_true, int value_false)
{
  return -1;
}

int M6502::return_local(int index, int local_count)
{
  fprintf(out, "; return_local\n");
  fprintf(out, "  ldy locals\n");
  fprintf(out, "  lda stack_lo - %d,y\n", LOCALS(index));
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  lda stack_hi - %d,y\n", LOCALS(index));
  fprintf(out, "  sta result + 1\n");

  fprintf(out, "  ldx locals\n");

  if (!is_main)
  {
    fprintf(out, "  inx\n");
    fprintf(out, "  lda stack_lo,x\n");
    fprintf(out, "  sta locals\n");
  }

  fprintf(out, "  rts\n");

  return 0;
}

int M6502::return_integer(int local_count)
{
  fprintf(out, "; return_integer\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  sta result + 1\n");
  stack--;

  fprintf(out, "  ldx locals\n");

  if (!is_main)
  {
    fprintf(out, "  inx\n");
    fprintf(out, "  lda stack_lo,x\n");
    fprintf(out, "  sta locals\n");
  }

  fprintf(out, "  rts\n");

  return 0;
}

int M6502::return_void(int local_count)
{
  fprintf(out, "; return_void\n");
  fprintf(out, "  ldx locals\n");

  if (!is_main)
  {
    fprintf(out, "  inx\n");
    fprintf(out, "  lda stack_lo,x\n");
    fprintf(out, "  sta locals\n");
  }

  fprintf(out, "  rts\n");

  return 0;
}

int M6502::jump(std::string &name, int distance)
{
  fprintf(out, "; jump\n");
  fprintf(out, "  jmp %s\n", name.c_str());

  return 0;
}

int M6502::call(std::string &name)
{
  fprintf(out, "; call\n");
  fprintf(out, "  jsr %s\n", name.c_str());

  return 0;
}

int M6502::invoke_static_method(const char *name, int params, int is_void)
{
  int local;
  int stack_vars = stack;

  //printf("invoke_static_method() name=%s params=%d is_void=%d\n", name, params, is_void);

  fprintf(out, "; invoke_static_method\n");

  local = -params;
  while(local != 0)
  {
    if (stack_vars > 0)
    {
      fprintf(out, "  lda stack_lo + 1 + %d,x\n", (stack - stack_vars));
      fprintf(out, "  sta stack_lo + 1 %d,x\n", local - 1);
      fprintf(out, "  lda stack_hi + 1 + %d,x\n", (stack - stack_vars));
      fprintf(out, "  sta stack_hi + 1 %d,x\n", local - 1);
      stack_vars--;
    }

    local++;
  }

  fprintf(out, "  jsr %s\n", name);

  if ((stack - stack_vars) > 0)
  {
    fprintf(out, "  txa\n");
    fprintf(out, "  clc\n");
    fprintf(out, "  adc #%d\n", (stack - stack_vars));
    fprintf(out, "  tax\n");

    params -= (stack - stack_vars);
  }

  if (!is_void)
  {
    fprintf(out, "  lda result + 0\n");
    fprintf(out, "  sta stack_lo,x\n");
    fprintf(out, "  lda result + 1\n");
    fprintf(out, "  sta stack_hi,x\n");
    fprintf(out, "  dex\n");
    stack++;
  }

  return 0;
}

int M6502::put_static(std::string &name, int index)
{
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta %s + 0\n", name.c_str());
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  sta %s + 1\n", name.c_str());
  stack--;

  return 0;
}

int M6502::get_static(std::string &name, int index)
{
  fprintf(out, "  lda %s + 0\n", name.c_str());
  fprintf(out, "  sta stack_lo,x\n");
  fprintf(out, "  lda %s + 1\n", name.c_str());
  fprintf(out, "  sta stack_hi,x\n");
  fprintf(out, "  dex\n");
  stack++;

  return 0;
}

int M6502::brk()
{
  return -1;
}

int M6502::new_array(uint8_t type)
{
  if (stack > 0)
  {
    if (type == TYPE_SHORT || type == TYPE_CHAR || type == TYPE_INT)
    {
      need_array_int_support = 1;
      fprintf(out, "  jsr new_array_int\n");
    }
      else
    {
      need_array_byte_support = 1;
      fprintf(out, "  jsr new_array_byte\n");
    }
  }

  return 0;
}

int M6502::insert_array(std::string &name, int32_t *data, int len, uint8_t type)
{
  fprintf(out, "; insert_array\n");

  if (type == TYPE_BYTE)
  {
    return insert_db(name, data, len, TYPE_SHORT);
  }
    else if (type == TYPE_SHORT)
  {
    return insert_dw(name, data, len, TYPE_SHORT);
  }
    else if (type == TYPE_INT)
  {
    return insert_dw(name, data, len, TYPE_SHORT);
  }

  return -1;
}

int M6502::insert_string(std::string &name, uint8_t *bytes, int len)
{
  fprintf(out, "; insert_string\n");
  fprintf(out, "  dw %d\n", len);
  return insert_utf8(name, bytes, len);
}

int M6502::push_array_length()
{
  if (stack > 0)
  {
    need_push_array_length = 1;
    fprintf(out, "  jsr push_array_length\n");
  }

  return 0;
}

int M6502::push_array_length(std::string &name, int field_id)
{
  need_push_array_length2 = 1;
  fprintf(out, "  lda %s + 0\n", name.c_str());
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda %s + 1\n", name.c_str());
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  jsr push_array_length2\n");
  stack++;

  return 0;
}

int M6502::array_read_byte()
{
  need_array_byte_support = 1;
  fprintf(out, "  jsr array_read_byte\n");
  stack--;

  return 0;
}

int M6502::array_read_short()
{
  return array_read_int();
}

int M6502::array_read_int()
{
  need_array_int_support = 1;
  fprintf(out, "  jsr array_read_int\n");
  stack--;

  return 0;
}

int M6502::array_read_byte(std::string &name, int field_id)
{
  return -1;
}

int M6502::array_read_short(std::string &name, int field_id)
{
  return array_read_int(name, field_id);
}

int M6502::array_read_int(std::string &name, int field_id)
{
  return -1;
}

int M6502::array_write_byte()
{
  need_array_byte_support = 1;
  fprintf(out, "  jsr array_write_byte\n");
  stack -= 3;

  return 0;
}

int M6502::array_write_short()
{
  return array_write_int();
}

int M6502::array_write_int()
{
  need_array_int_support = 1;
  fprintf(out, "  jsr array_write_int\n");
  stack -= 3;

  return 0;
}

int M6502::array_write_byte(std::string &name, int field_id)
{
  return -1;
}

int M6502::array_write_short(std::string &name, int field_id)
{
  return array_write_int(name, field_id);
}

int M6502::array_write_int(std::string &name, int field_id)
{
  return -1;
}

// subroutines
void M6502::insert_swap()
{
  fprintf(out, "swap:\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta value1 + 0,x\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  sta value1 + 1,x\n");

  fprintf(out, "  lda stack_lo - 1,x\n");
  fprintf(out, "  sta stack_lo,x\n");
  fprintf(out, "  lda stack_hi - 1,x\n");
  fprintf(out, "  sta stack_hi,x\n");

  fprintf(out, "  lda value1 + 0\n");
  fprintf(out, "  sta stack_lo - 1,x\n");
  fprintf(out, "  lda value1 + 1\n");
  fprintf(out, "  sta stack_hi - 1,x\n");
  fprintf(out, "  rts\n");
}

void M6502::insert_add_integer()
{
  fprintf(out, "add_integer:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda stack_lo + 0,x\n");
  fprintf(out, "  adc stack_lo + 1,x\n");
  fprintf(out, "  sta stack_lo + 1,x\n");
  fprintf(out, "  lda stack_hi + 0,x\n");
  fprintf(out, "  adc stack_hi + 1,x\n");
  fprintf(out, "  sta stack_hi + 1,x\n");
  fprintf(out, "  rts\n");
}

void M6502::insert_sub_integer()
{
  fprintf(out, "sub_integer:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  lda stack_lo + 1,x\n");
  fprintf(out, "  sbc stack_lo + 0,x\n");
  fprintf(out, "  sta stack_lo + 1,x\n");
  fprintf(out, "  lda stack_hi + 1,x\n");
  fprintf(out, "  sbc stack_hi + 0,x\n");
  fprintf(out, "  sta stack_hi + 1,x\n");
  fprintf(out, "  rts\n");
}

void M6502::insert_mul_integer()
{
  fprintf(out, "mul_integer:\n");
  // load values
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_hi + 0,x\n");
  fprintf(out, "  sta value2 + 1\n");
  fprintf(out, "  lda stack_lo + 0,x\n");
  fprintf(out, "  sta value2 + 0\n");
  fprintf(out, "  lda stack_hi + 1,x\n");
  fprintf(out, "  sta value1 + 1\n");
  fprintf(out, "  lda stack_lo + 1,x\n");
  fprintf(out, "  sta value1 + 0\n");

  // clear result
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  ldy #16\n");

  // loop
  fprintf(out, "mul_integer_loop:\n");
  fprintf(out, "  asl result + 0\n");
  fprintf(out, "  rol result + 1\n");
  fprintf(out, "  asl value1 + 0\n");
  fprintf(out, "  rol value1 + 1\n");
  fprintf(out, "  bcc mul_integer_next\n");

  // add
  fprintf(out, "  clc\n");
  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  adc value2 + 0\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  lda result + 1\n");
  fprintf(out, "  adc value2 + 1\n");
  fprintf(out, "  sta result + 1\n");

  // next
  fprintf(out, "mul_integer_next:\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne mul_integer_loop\n");

  // push result
  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  sta stack_lo + 1,x\n");
  fprintf(out, "  lda result + 1\n");
  fprintf(out, "  sta stack_hi + 1,x\n");
  fprintf(out, "  rts\n");
}

void M6502::insert_div_integer()
{
  fprintf(out, "div_integer:\n");
  // load values
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_hi + 0,x\n");
  fprintf(out, "  sta value2 + 1\n");
  fprintf(out, "  sta value2 + 1\n");
  fprintf(out, "  lda stack_lo + 0,x\n");
  fprintf(out, "  sta value2 + 0\n");

  fprintf(out, "  lda stack_lo + 1,x\n");
  fprintf(out, "  sta value1 + 0\n");
  fprintf(out, "  lda stack_hi + 1,x\n");
  fprintf(out, "  sta value1 + 1\n");

  // clear remainder
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta remainder + 0\n");
  fprintf(out, "  sta remainder + 1\n");
  fprintf(out, "  ldy #16\n");

  // loop
  fprintf(out, "div_integer_loop:\n");
  fprintf(out, "  asl value1 + 0\n");
  fprintf(out, "  rol value1 + 1\n");
  fprintf(out, "  rol remainder + 0\n");
  fprintf(out, "  rol remainder + 1\n");

  // sub
  fprintf(out, "  sec\n");
  fprintf(out, "  lda remainder + 0\n");
  fprintf(out, "  sbc value2 + 0\n");
  fprintf(out, "  sta value3\n");
  fprintf(out, "  lda remainder + 1\n");
  fprintf(out, "  sbc value2 + 1\n");
  fprintf(out, "  bcc div_integer_next\n");

  fprintf(out, "  sta remainder + 1\n");
  fprintf(out, "  lda value3\n");
  fprintf(out, "  sta remainder + 0\n");
  fprintf(out, "  inc value1 + 0\n");

  // next
  fprintf(out, "div_integer_next:\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne div_integer_loop\n");

  // push result
  fprintf(out, "  lda value1 + 0\n");
  fprintf(out, "  sta stack_lo + 1, x\n");
  fprintf(out, "  lda value1 + 1\n");
  fprintf(out, "  sta stack_hi + 1, x\n");
  fprintf(out, "  rts\n");
}

void M6502::insert_neg_integer()
{
  fprintf(out, "neg_integer:\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sbc stack_lo + 1,x\n");
  fprintf(out, "  sta stack_lo + 1,x\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sbc stack_hi + 1,x\n");
  fprintf(out, "  sta stack_hi + 1,x\n");
  fprintf(out, "  rts\n");
}

void M6502::insert_shift_left_integer()
{
  fprintf(out, "shift_left_integer:\n");

  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo + 0,x\n");
  fprintf(out, "  tay\n");

  fprintf(out, "shift_left_integer_loop:\n");
  fprintf(out, "  asl stack_lo + 1,x\n");
  fprintf(out, "  rol stack_hi + 1,x\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne shift_left_integer_loop\n");
  fprintf(out, "  rts\n");
}

void M6502::insert_shift_right_integer()
{
  fprintf(out, "shift_right_integer:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo + 0,x\n");
  fprintf(out, "  tay\n");

  fprintf(out, "shift_right_integer_loop:\n");
  fprintf(out, "  lda stack_hi + 1,x\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  ror stack_hi + 1,x\n");
  fprintf(out, "  ror stack_lo + 1,x\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne shift_right_integer_loop\n");

  fprintf(out, "  rts\n");
}

void M6502::insert_shift_right_uinteger()
{
  fprintf(out, "shift_right_uinteger:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo + 0,x\n");
  fprintf(out, "  tay\n");
  fprintf(out, "shift_right_uinteger_loop:\n");
  fprintf(out, "  lsr stack_hi + 1,x\n");
  fprintf(out, "  ror stack_lo + 1,x\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne shift_right_uinteger_loop\n");
  fprintf(out, "  rts\n");
}

void M6502::insert_and_integer()
{
  fprintf(out, "and_integer:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo + 0,x\n");
  fprintf(out, "  and stack_lo + 1,x\n");
  fprintf(out, "  sta stack_lo + 1,x\n");
  fprintf(out, "  lda stack_hi + 0,x\n");
  fprintf(out, "  and stack_hi + 1,x\n");
  fprintf(out, "  sta stack_hi + 1,x\n");
  fprintf(out, "  rts\n");
}

void M6502::insert_or_integer()
{
  fprintf(out, "or_integer:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo + 0,x\n");
  fprintf(out, "  ora stack_lo + 1,x\n");
  fprintf(out, "  sta stack_lo + 1,x\n");
  fprintf(out, "  lda stack_hi + 0,x\n");
  fprintf(out, "  ora stack_hi + 1,x\n");
  fprintf(out, "  sta stack_hi + 1,x\n");
  fprintf(out, "  rts\n");
}

void M6502::insert_xor_integer()
{
  fprintf(out, "xor_integer:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo + 0,x\n");
  fprintf(out, "  eor stack_lo + 1,x\n");
  fprintf(out, "  sta stack_lo + 1,x\n");
  fprintf(out, "  lda stack_hi + 0,x\n");
  fprintf(out, "  eor stack_hi + 1,x\n");
  fprintf(out, "  sta stack_hi + 1,x\n");
  fprintf(out, "  rts\n");
}

void M6502::insert_integer_to_byte()
{
  fprintf(out, "integer_to_byte:\n");
  fprintf(out, "  lda stack_lo + 1,x\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  adc #0xff\n");
  fprintf(out, "  eor #0xff\n");
  fprintf(out, "  sta stack_hi + 1,x\n");
  fprintf(out, "  rts\n");
}

// FIXME untested
void M6502::insert_dup()
{
  fprintf(out, "dup:\n");
  fprintf(out, "  lda stack_lo + 1,x\n");
  fprintf(out, "  sta stack_lo + 0,x\n");
  fprintf(out, "  lda stack_hi + 1,x\n");
  fprintf(out, "  sta stack_hi + 0,x\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  rts\n");
}

void M6502::insert_dup2()
{
  fprintf(out, "dup2:\n");
  fprintf(out, "  lda stack_lo + 2,x\n");
  fprintf(out, "  sta stack_lo + 0,x\n");
  fprintf(out, "  lda stack_hi + 2,x\n");
  fprintf(out, "  sta stack_hi + 0,x\n");

  fprintf(out, "  lda stack_lo + 1,x\n");
  fprintf(out, "  sta stack_lo - 1,x\n");
  fprintf(out, "  lda stack_hi + 1,x\n");
  fprintf(out, "  sta stack_hi - 1,x\n");

  fprintf(out, "  dex\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  rts\n");
}

void M6502::insert_push_array_length()
{
  fprintf(out, "push_array_length:\n");
  fprintf(out, "  lda stack_lo + 1,x\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #2\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda stack_hi + 1,x\n");
  fprintf(out, "  sbc #0\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta stack_lo + 1,x\n");
  fprintf(out, "  ldy #1\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta stack_hi + 1,x\n");
  fprintf(out, "  rts\n");
}

void M6502::insert_push_array_length2()
{
  fprintf(out, "push_array_length2:\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  lda address + 0\n");
  fprintf(out, "  sbc #2\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda address + 1\n");
  fprintf(out, "  sbc #0\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta stack_lo,x\n");
  fprintf(out, "  ldy #1\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta stack_hi,x\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  rts\n");
}

void M6502::insert_array_byte_support()
{
  // new array_byte
  fprintf(out, "new_array_byte:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo + 0,x\n");
  fprintf(out, "  sta length + 0\n");
  fprintf(out, "  lda stack_hi + 0,x\n");
  fprintf(out, "  sta length + 1\n");

  fprintf(out, "  lda heap_ptr + 0\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda heap_ptr + 1\n");
  fprintf(out, "  sta address + 1\n");

  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda length + 0\n");
  fprintf(out, "  sta (address),y\n");
  fprintf(out, "  ldy #1\n");
  fprintf(out, "  lda length + 1\n");
  fprintf(out, "  sta (address),y\n");

  fprintf(out, "  clc\n");
  fprintf(out, "  lda length + 0\n");
  fprintf(out, "  adc #2\n");
  fprintf(out, "  sta length + 0\n");
  fprintf(out, "  lda length + 1\n");
  fprintf(out, "  adc #0\n");
  fprintf(out, "  sta length + 1\n");

  fprintf(out, "  clc\n");
  fprintf(out, "  lda heap_ptr + 0\n");
  fprintf(out, "  adc length + 0\n");
  fprintf(out, "  sta heap_ptr + 0\n");
  fprintf(out, "  lda heap_ptr + 1\n");
  fprintf(out, "  adc length + 1\n");
  fprintf(out, "  sta heap_ptr + 1\n");

  fprintf(out, "  clc\n");
  fprintf(out, "  lda address + 0\n");
  fprintf(out, "  adc #3\n");
  fprintf(out, "  and #254\n");
  fprintf(out, "  sta stack_lo + 0,x\n");
  fprintf(out, "  lda address + 1\n");
  fprintf(out, "  adc #0\n");
  fprintf(out, "  sta stack_hi + 0,x\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  rts\n");

  // array_read_byte
  fprintf(out, "array_read_byte:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda stack_lo + 1,x\n");
  fprintf(out, "  adc stack_lo + 0,x\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda stack_hi + 1,x\n");
  fprintf(out, "  adc stack_hi + 0,x\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta stack_lo + 1,x\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  adc #0xff\n");
  fprintf(out, "  eor #0xff\n");
  fprintf(out, "  sta stack_hi + 1,x\n");
  fprintf(out, "  rts\n");

  // array_write_byte
  fprintf(out, "array_write_byte:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda stack_lo + 2,x\n");
  fprintf(out, "  adc stack_lo + 1,x\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda stack_hi + 2,x\n");
  fprintf(out, "  adc stack_hi + 1,x\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda stack_lo + 0,x\n");
  fprintf(out, "  sta (address),y\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  rts\n");
}

void M6502::insert_array_int_support()
{
  // new_array int
  fprintf(out, "new_array_int:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo + 0,x\n");
  fprintf(out, "  sta length + 0\n");
  fprintf(out, "  lda stack_hi + 0,x\n");
  fprintf(out, "  sta length + 1\n");

  fprintf(out, "  lda heap_ptr + 0\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda heap_ptr + 1\n");
  fprintf(out, "  sta address + 1\n");

  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda length + 0\n");
  fprintf(out, "  sta (address),y\n");
  fprintf(out, "  ldy #1\n");
  fprintf(out, "  lda length + 1\n");
  fprintf(out, "  sta (address),y\n");

  fprintf(out, "  asl length + 0\n");
  fprintf(out, "  rol length + 1\n");

  fprintf(out, "  clc\n");
  fprintf(out, "  lda length + 0\n");
  fprintf(out, "  adc #2\n");
  fprintf(out, "  sta length + 0\n");
  fprintf(out, "  lda length + 1\n");
  fprintf(out, "  adc #0\n");
  fprintf(out, "  sta length + 1\n");

  fprintf(out, "  clc\n");
  fprintf(out, "  lda heap_ptr + 0\n");
  fprintf(out, "  adc length + 0\n");
  fprintf(out, "  sta heap_ptr + 0\n");
  fprintf(out, "  lda heap_ptr + 1\n");
  fprintf(out, "  adc length + 1\n");
  fprintf(out, "  sta heap_ptr + 1\n");

  fprintf(out, "  clc\n");
  fprintf(out, "  lda address + 0\n");
  fprintf(out, "  adc #3\n");
  fprintf(out, "  and #254\n");
  fprintf(out, "  sta stack_lo + 0,x\n");
  fprintf(out, "  lda address + 1\n");
  fprintf(out, "  adc #0\n");
  fprintf(out, "  sta stack_hi + 0,x\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  rts\n");

  // array_read_int
  fprintf(out, "array_read_int:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  asl stack_lo + 0,x\n");
  fprintf(out, "  rol stack_hi + 0,x\n");

  fprintf(out, "  clc\n");
  fprintf(out, "  lda stack_lo + 1,x\n");
  fprintf(out, "  adc stack_lo + 0,x\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda stack_hi + 1,x\n");
  fprintf(out, "  adc stack_hi + 0,x\n");
  fprintf(out, "  sta address + 1\n");

  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta stack_lo + 1,x\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta stack_hi + 1,x\n");
  fprintf(out, "  rts\n");

  // array_write_int
  fprintf(out, "array_write_int:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  asl stack_lo + 1,x\n");
  fprintf(out, "  rol stack_hi + 1,x\n");

  fprintf(out, "  clc\n");
  fprintf(out, "  lda stack_lo + 2,x\n");
  fprintf(out, "  adc stack_lo + 1,x\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda stack_hi + 2,x\n");
  fprintf(out, "  adc stack_hi + 1,x\n");
  fprintf(out, "  sta address + 1\n");

  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda stack_lo + 0,x\n");
  fprintf(out, "  sta (address),y\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  lda stack_hi + 0,x\n");
  fprintf(out, "  sta (address),y\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  rts\n");
}

// Memory API
int M6502::memory_read8_I()
{
  need_memory_read8 = 1;

  fprintf(out, "; memory_read8_I\n");
  fprintf(out, "  jsr memory_read8\n");

  return 0;
}

int M6502::memory_write8_IB()
{
  need_memory_write8 = 1;

  fprintf(out, "; memory_write8_IB\n");
  fprintf(out, "  jsr memory_write8\n");
  stack -= 2;

  return 0;
}

int M6502::memory_read16_I()
{
  need_memory_read16 = 1;

  fprintf(out, "; memory_read16_I\n");
  fprintf(out, "  jsr memory_read16\n");

  return 0;
}

int M6502::memory_write16_IS()
{
  need_memory_write16 = 1;

  fprintf(out, "; memory_write16_IS\n");
  fprintf(out, "  jsr memory_write16\n");
  stack -= 2;

  return 0;
}

int M6502::memory_addressOf_aB()
{
  fprintf(out, "; memory_addressOf_aB()\n");

  return 0;
}

int M6502::memory_addressOf_aS()
{
  fprintf(out, "; memory_addressOf_aS()\n");

  return 0;
}

int M6502::memory_addressOf_aC()
{
  fprintf(out, "; memory_addressOf_aC()\n");

  return 0;
}

int M6502::memory_addressOf_aI()
{
  fprintf(out, "; memory_addressOf_aI()\n");

  return 0;
}

void M6502::insert_memory_read8()
{
  fprintf(out, "memory_read8:\n");
  fprintf(out, "  lda stack_lo + 1,x\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda stack_hi + 1,x\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta stack_lo + 1,x\n");

  // sign-extend
  fprintf(out, "  asl\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  adc #0xff\n");
  fprintf(out, "  eor #0xff\n");
  fprintf(out, "  sta stack_hi + 1,x\n");
  fprintf(out, "  rts\n");
}

void M6502::insert_memory_write8()
{
  fprintf(out, "memory_write8:\n");
  fprintf(out, "  lda stack_lo + 2,x\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda stack_hi + 2,x\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda stack_lo + 1,x\n");
  fprintf(out, "  sta (address),y\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  rts\n");
}

void M6502::insert_memory_read16()
{
  fprintf(out, "memory_read16:\n");
  fprintf(out, "  lda stack_lo + 1,x\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda stack_hi + 1,x\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta stack_lo + 1,x\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta stack_hi + 1,x\n");
  fprintf(out, "  rts\n");
}

void M6502::insert_memory_write16()
{
  fprintf(out, "memory_write16:\n");
  fprintf(out, "  lda stack_lo + 2,x\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda stack_hi + 2,x\n");
  fprintf(out, "  sta address + 1\n");

  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda stack_lo + 1,x\n");
  fprintf(out, "  sta (address),y\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  lda stack_hi + 1,x\n");
  fprintf(out, "  sta (address),y\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  rts\n");
}

// Math API
int M6502::math_max_II()
{
  need_math_max = 1;

  fprintf(out, "  jsr math_max\n");

  return 0;
}

int M6502::math_min_II()
{
  need_math_min = 1;

  fprintf(out, "  jsr math_min\n");

  return 0;
}

int M6502::math_abs_I()
{
  need_math_abs = 1;

  fprintf(out, "  jsr math_abs\n");

  return 0;
}

void M6502::insert_math_max()
{
  fprintf(out, "math_max:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo + 1,x\n");
  fprintf(out, "  cmp stack_lo + 0,x\n");
  fprintf(out, "  lda stack_hi + 1,x\n");
  fprintf(out, "  sbc stack_hi + 0,x\n");
  fprintf(out, "  bvc #2\n");
  fprintf(out, "  eor #0x80\n");
  fprintf(out, "  bpl math_max_skip\n");
  fprintf(out, "  lda stack_lo + 0,x\n");
  fprintf(out, "  sta stack_lo + 1,x\n");
  fprintf(out, "  lda stack_hi + 0,x\n");
  fprintf(out, "  sta stack_hi + 1,x\n");
  fprintf(out, "math_max_skip:\n");
  fprintf(out, "  rts\n");
};

void M6502::insert_math_min()
{
  fprintf(out, "math_min:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo + 0,x\n");
  fprintf(out, "  cmp stack_lo + 1,x\n");
  fprintf(out, "  lda stack_hi + 0,x\n");
  fprintf(out, "  sbc stack_hi + 1,x\n");
  fprintf(out, "  bvc #2\n");
  fprintf(out, "  eor #0x80\n");
  fprintf(out, "  bpl math_min_skip\n");
  fprintf(out, "  lda stack_lo + 0,x\n");
  fprintf(out, "  sta stack_lo + 1,x\n");
  fprintf(out, "  lda stack_hi + 0,x\n");
  fprintf(out, "  sta stack_hi + 1,x\n");
  fprintf(out, "math_min_skip:\n");
  fprintf(out, "  rts\n");
};

void M6502::insert_math_abs()
{
  fprintf(out, "math_abs:\n");
  fprintf(out, "  lda stack_hi + 1,x\n");
  fprintf(out, "  and #0x80\n");
  fprintf(out, "  bne #1\n");
  fprintf(out, "  rts\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sbc stack_lo + 1,x\n");
  fprintf(out, "  sta stack_lo + 1,x\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sbc stack_hi + 1,x\n");
  fprintf(out, "  sta stack_hi + 1,x\n");
  fprintf(out, "  rts\n");
};

