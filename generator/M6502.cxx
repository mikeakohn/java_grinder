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

#include "M6502.h"

// ABI is:
// A
// X
// Y

#define PUSH_LO \
  fprintf(out, "; PUSH_LO\n"); \
  fprintf(out, "  ldy SP\n"); \
  fprintf(out, "  sta stack_lo,y\n")

#define PUSH_HI \
  fprintf(out, "; PUSH_HI\n"); \
  fprintf(out, "  ldy SP\n"); \
  fprintf(out, "  sta stack_hi,y\n"); \
  fprintf(out, "  dec SP\n")

#define POP_HI \
  fprintf(out, "; POP_HI\n"); \
  fprintf(out, "  inc SP\n"); \
  fprintf(out, "  ldy SP\n"); \
  fprintf(out, "  lda stack_hi,y\n"); \

#define POP_LO \
  fprintf(out, "; POP_LO\n"); \
  fprintf(out, "  ldy SP\n"); \
  fprintf(out, "  lda stack_lo,y\n")

#define LOCALS(a) (a)

M6502::M6502() :
  stack(0),
  is_main(0),

  need_swap(0),
  need_add_integer(0),
  need_sub_integer(0),
  need_mul_integer(0),
  need_div_integer(0),
  need_mod_integer(0),
  need_neg_integer(0),
  need_shift_left_integer(0),
  need_shift_right_integer(0),
  need_shift_right_uinteger(0),
  need_and_integer(0),
  need_or_integer(0),
  need_xor_integer(0),
  need_push_array_length(0),
  need_push_array_length2(0),
  need_array_byte_support(0),
  need_array_int_support(0)
{

}

M6502::~M6502()
{
  if(need_swap) { insert_swap(); }
  if(need_add_integer) { insert_add_integer(); }
  if(need_sub_integer) { insert_sub_integer(); }
  if(need_mul_integer) { insert_mul_integer(); }
  if(need_div_integer) { insert_div_integer(); }
  if(need_mod_integer) { insert_mod_integer(); }
  if(need_neg_integer) { insert_neg_integer(); }
  if(need_shift_left_integer) { insert_shift_left_integer(); }
  if(need_shift_right_integer) { insert_shift_right_integer(); }
  if(need_shift_right_uinteger) { insert_shift_right_uinteger(); }
  if(need_and_integer) { insert_and_integer(); }
  if(need_or_integer) { insert_or_integer(); }
  if(need_xor_integer) { insert_xor_integer(); }
  if(need_push_array_length) { insert_push_array_length(); }
  if(need_push_array_length2) { insert_push_array_length2(); }
  if(need_array_byte_support) { insert_array_byte_support(); }
  if(need_array_int_support) { insert_array_int_support(); }
}

int M6502::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".65xx\n");

  // heap
  fprintf(out, "ram_start equ 0x8000\n");
  fprintf(out, "heap_ptr equ ram_start\n");

  // for indirection (2 bytes)
  fprintf(out, "address equ 0xfb\n");

  // java stack
  fprintf(out, "stack_lo equ 0xc000\n");
  fprintf(out, "stack_hi equ 0xc100\n");
  fprintf(out, "SP equ 0xfd\n");

  // points to locals
  fprintf(out, "locals equ 0xfe\n");

  // temp variables
  fprintf(out, "result equ 0x20\n");
  fprintf(out, "remainder equ 0x24\n");
  fprintf(out, "length equ 0x26\n");
  fprintf(out, "value1 equ 0x2a\n");
  fprintf(out, "value2 equ 0x2c\n");
  fprintf(out, "value3 equ 0x2e\n");

  return 0;
}

int M6502::start_init()
{
  return 0;
}

int M6502::insert_static_field_define(const char *name, const char *type, int index)
{
  fprintf(out, "%s equ ram_start + %d\n", name, (index + 1) * 2);

  return 0;
}

int M6502::init_heap(int field_count)
{
  fprintf(out, "  ; Set up heap and static initializers\n");
  fprintf(out, "  lda #(ram_start + %d) & 0xff\n", (field_count + 1) * 2);
  fprintf(out, "  sta ram_start + 0\n");
  fprintf(out, "  lda #(ram_start + %d) >> 8\n", (field_count + 1) * 2);
  fprintf(out, "  sta ram_start + 1\n");

  return 0;
}

int M6502::insert_field_init_boolean(char *name, int index, int value)
{
  value = (value == 0) ? 0 : 1;
  fprintf(out, "; insert_field_init_boolean\n");
  fprintf(out, "  lda #%d\n", value & 0xff);
  fprintf(out, "  sta %s + 0\n", name);
  fprintf(out, "  lda #%d\n", value >> 8);
  fprintf(out, "  sta %s + 1\n", name);

  return 0;
}

int M6502::insert_field_init_byte(char *name, int index, int value)
{
  if (value < -128 || value > 255) { return -1; }
  int16_t n = value;
  uint16_t v = (n & 0xffff);


  fprintf(out, "; insert_field_init_short\n");
  fprintf(out, "  lda #%d\n", (uint8_t)v & 0xff);
  fprintf(out, "  sta %s + 0\n", name);
  fprintf(out, "  lda #%d\n", (uint8_t)v >> 8);
  fprintf(out, "  sta %s + 1\n", name);

  return 0;
}

int M6502::insert_field_init_short(char *name, int index, int value)
{
  if (value < -32768 || value > 65535) { return -1; }

  fprintf(out, "; insert_field_init_short\n");
  fprintf(out, "  lda #%d\n", value & 0xff);
  fprintf(out, "  sta %s + 0\n", name);
  fprintf(out, "  lda #%d\n", value >> 8);
  fprintf(out, "  sta %s + 1\n", name);

  return 0;
}

int M6502::insert_field_init_int(char *name, int index, int value)
{
  return insert_field_init_short(name, index, value);
}

int M6502::insert_field_init(char *name, int index)
{
  fprintf(out, "; insert_field_init\n");
  fprintf(out, "  lda #_%s & 0xff\n", name);
  fprintf(out, "  sta %s + 0\n", name);
  fprintf(out, "  lda #_%s >> 8\n", name);
  fprintf(out, "  sta %s + 1\n", name);

  return 0;
}

void M6502::method_start(int local_count, int max_stack, int param_count, const char *name)
{
  stack = 0;

  is_main = (strcmp(name, "main") == 0) ? 1 : 0;

  fprintf(out, "%s:\n", name);

  // main() function goes here
  if (!is_main)
  {
    fprintf(out, "  lda locals\n");
    PUSH_LO;
    fprintf(out, "  lda #0\n");
    PUSH_HI;
  }

  fprintf(out, "  lda SP\n");
  fprintf(out, "  sta locals\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #0x%02x\n", local_count);
  fprintf(out, "  sta SP\n");
}

void M6502::method_end(int local_count)
{
  fprintf(out, "\n");
}

int M6502::push_integer(int32_t n)
{
  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);

    return -1;
  }

  uint16_t value = (n & 0xffff);

  fprintf(out, "; push_integer\n");
  fprintf(out, "  lda #0x%02x\n", value & 0xff);
  PUSH_LO;
  fprintf(out, "  lda #0x%02x\n", value >> 8);
  PUSH_HI;
  stack++;

  return 0;
}

int M6502::push_integer_local(int index)
{
  fprintf(out, "; push_integer_local\n");
  fprintf(out, "  ldx locals\n");
  fprintf(out, "  lda stack_lo - %d,x\n", LOCALS(index));
  PUSH_LO;
  fprintf(out, "  lda stack_hi - %d,x\n", LOCALS(index));
  PUSH_HI;
  stack++;

  return 0;
}

int M6502::push_ref_local(int index)
{
  return push_integer_local(index);
}

int M6502::push_fake()
{
  return -1;
}

int M6502::push_long(int64_t n)
{
  return push_integer((int32_t)n);
}

int M6502::push_float(float f)
{
  printf("float is not supported right now\n");

  return -1;
}

int M6502::push_double(double f)
{
  printf("double is not supported right now\n");

  return -1;
}

int M6502::push_byte(int8_t b)
{
  int16_t n = b;
  uint16_t value = (n & 0xffff);

  fprintf(out, "; push_byte\n");
  fprintf(out, "  lda #0x%02x\n", value & 0xff);
  PUSH_LO;
  fprintf(out, "  lda #0x%02x\n", value >> 8);
  PUSH_HI;
  stack++;

  return 0;
}

int M6502::push_short(int16_t s)
{
  uint16_t value = (s & 0xffff);

  fprintf(out, "; push_short\n");
  fprintf(out, "  lda #0x%02x\n", value & 0xff);
  PUSH_LO;
  fprintf(out, "  lda #0x%02x\n", value >> 8);
  PUSH_HI;
  stack++;

  return 0;
}

int M6502::pop_integer_local(int index)
{
  fprintf(out, "; pop_integer_local\n");
  fprintf(out, "  ldx locals\n");
  POP_HI;
  fprintf(out, "  sta stack_hi - %d,x\n", LOCALS(index));
  POP_LO;
  fprintf(out, "  sta stack_lo - %d,x\n", LOCALS(index));
  stack--;

  return 0;
}

int M6502::pop_ref_local(int index)
{
  return pop_integer_local(index);
}

int M6502::pop()
{
  fprintf(out, "; pop\n");
  POP_HI;
  fprintf(out, "  sta result + 1\n");
  POP_LO;
  fprintf(out, "  sta result + 0\n");
  stack--;

  return 0;
}

int M6502::dup()
{
  fprintf(out, "  ldx SP\n");
  fprintf(out, "  lda stack_lo,x\n");
  PUSH_LO;
  fprintf(out, "  lda stack_hi,x\n");
  PUSH_HI;
  stack++;

  return 0;
}

int M6502::dup2()
{
  printf("Need to implement dup2()\n");

  return -1;
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
  return -1;
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
  return -1;
}

int M6502::mul_integer()
{
  need_mul_integer = 1;
  fprintf(out, "  jsr mul_integer\n");
  stack--;

  return 0;
}

int M6502::mul_integer(int const_val)
{
  return -1;
}

// unsigned only for now
int M6502::div_integer()
{
  need_div_integer = 1;
  fprintf(out, "  jsr div_integer\n");
  stack--;

  return 0;
}

int M6502::div_integer(int const_val)
{
  return -1;
}

// unsigned only for now
int M6502::mod_integer()
{
  need_div_integer = 1;
  need_mod_integer = 1;
  fprintf(out, "  jsr div_integer\n");
  fprintf(out, "  jsr mod_integer\n");

  return 0;
}

int M6502::mod_integer(int const_val)
{
  return -1;
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
  return -1;
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
  return -1;
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
  return -1;
}

int M6502::inc_integer(int index, int num)
{
  uint16_t value = num & 0xffff;

  fprintf(out, "; inc_integer num = %d\n", num);
  fprintf(out, "  ldx locals\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda stack_lo - %d,x\n", LOCALS(index));
  fprintf(out, "  adc #0x%02x\n", value & 0xff);
  fprintf(out, "  sta stack_lo - %d,x\n", LOCALS(index));
  fprintf(out, "  lda stack_hi - %d,x\n", LOCALS(index));
  fprintf(out, "  adc #0x%02x\n", value >> 8);
  fprintf(out, "  sta stack_hi - %d,x\n", LOCALS(index));

  return 0;
}

int M6502::integer_to_byte()
{
  fprintf(out, "; integer_to_byte\n");
  POP_HI;
  POP_LO;
  fprintf(out, "  sta result + 0\n");
  PUSH_LO;
  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  bpl #15\n");

  fprintf(out, "  lda #0xff\n");
  fprintf(out, "  sta result + 1\n");
  PUSH_HI;
  fprintf(out, "  lda #0\n");
  fprintf(out, "  beq #11\n");

  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta result + 1\n");
  PUSH_HI;

  return 0;
}

int M6502::jump_cond(const char *label, int cond)
{
  bool reverse = false;

  if (stack > 0)
  {
    fprintf(out, "; jump_cond\n");
    fprintf(out, "  inc SP\n");
    fprintf(out, "  ldx SP\n");

    if(cond == COND_LESS_EQUAL)
    {
      reverse = true;
      cond = COND_GREATER_EQUAL;
    }
      else
    if(cond == COND_GREATER)
    {
      reverse = true;
      cond = COND_LESS;
    }

    switch(cond)
    {
      case COND_EQUAL:
        fprintf(out, "  lda stack_lo - 0,x\n");
        fprintf(out, "  cmp #0\n");
        fprintf(out, "  bne #10\n");
        fprintf(out, "  lda stack_hi - 0,x\n");
        fprintf(out, "  cmp #0\n");
        fprintf(out, "  bne #3\n");
        fprintf(out, "  jmp %s\n", label);
        break;
      case COND_NOT_EQUAL:
        fprintf(out, "  lda stack_lo - 0,x\n");
        fprintf(out, "  cmp #0\n");
        fprintf(out, "  bne #7\n");
        fprintf(out, "  lda stack_hi - 0,x\n");
        fprintf(out, "  cmp #0\n");
        fprintf(out, "  beq #3\n");
        fprintf(out, "  jmp %s\n", label);
        break;
      case COND_LESS:
        if(reverse == false)
        {
          fprintf(out, "  lda stack_lo - 0,x\n");
          fprintf(out, "  cmp #0\n");
          fprintf(out, "  lda stack_hi - 0,x\n");
          fprintf(out, "  sbc #0\n");
          fprintf(out, "  bvc #2\n");
          fprintf(out, "  eor #0x80\n");
          fprintf(out, "  bpl #3\n");
          fprintf(out, "  jmp %s\n", label);
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
          fprintf(out, "  jmp %s\n", label);
        }
        break;
      case COND_GREATER_EQUAL:
        if(reverse == false)
        {
          fprintf(out, "  lda stack_lo - 0,x\n");
          fprintf(out, "  cmp #0\n");
          fprintf(out, "  lda stack_hi - 0,x\n");
          fprintf(out, "  sbc #0\n");
          fprintf(out, "  bvc #2\n");
          fprintf(out, "  eor #0x80\n");
          fprintf(out, "  bmi #3\n");
          fprintf(out, "  jmp %s\n", label);
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
          fprintf(out, "  jmp %s\n", label);
        }
        break;
    }

    stack--;
  }

  return 0;
}

int M6502::jump_cond_integer(const char *label, int cond)
{
  bool reverse = false;

  if (stack > 1)
  {
    fprintf(out, "; jump_cond_integer\n");
    fprintf(out, "  inc SP\n");
    fprintf(out, "  inc SP\n");
    fprintf(out, "  ldx SP\n");

    if(cond == COND_LESS_EQUAL)
    {
      reverse = true;
      cond = COND_GREATER_EQUAL;
    }
      else
    if(cond == COND_GREATER)
    {
      reverse = true;
      cond = COND_LESS;
    }

    switch(cond)
    {
      case COND_EQUAL:
        fprintf(out, "  lda stack_lo - 0,x\n");
        fprintf(out, "  cmp stack_lo - 1,x\n");
        fprintf(out, "  bne #11\n");
        fprintf(out, "  lda stack_hi - 0,x\n");
        fprintf(out, "  cmp stack_hi - 1,x\n");
        fprintf(out, "  bne #3\n");
        fprintf(out, "  jmp %s\n", label);
        break;
      case COND_NOT_EQUAL:
        fprintf(out, "  lda stack_lo - 0,x\n");
        fprintf(out, "  cmp stack_lo - 1,x\n");
        fprintf(out, "  bne #8\n");
        fprintf(out, "  lda stack_hi - 0,x\n");
        fprintf(out, "  cmp stack_hi - 1,x\n");
        fprintf(out, "  beq #3\n");
        fprintf(out, "  jmp %s\n", label);
        break;
      case COND_LESS:
        if(reverse == false)
        {
          fprintf(out, "  lda stack_lo - 0,x\n");
          fprintf(out, "  cmp stack_lo - 1,x\n");
          fprintf(out, "  lda stack_hi - 0,x\n");
          fprintf(out, "  sbc stack_hi - 1,x\n");
          fprintf(out, "  bvc #2\n");
          fprintf(out, "  eor #0x80\n");
          fprintf(out, "  bpl #3\n");
          fprintf(out, "  jmp %s\n", label);
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
          fprintf(out, "  jmp %s\n", label);
        }
        break;
      case COND_GREATER_EQUAL:
        if(reverse == false)
        {
          fprintf(out, "  lda stack_lo - 0,x\n");
          fprintf(out, "  cmp stack_lo - 1,x\n");
          fprintf(out, "  lda stack_hi - 0,x\n");
          fprintf(out, "  sbc stack_hi - 1,x\n");
          fprintf(out, "  bvc #2\n");
          fprintf(out, "  eor #0x80\n");
          fprintf(out, "  bmi #3\n");
          fprintf(out, "  jmp %s\n", label);
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
          fprintf(out, "  jmp %s\n", label);
        }
        break;
    }

    stack -= 2;
  }

  return 0;
}

int M6502::return_local(int index, int local_count)
{
  fprintf(out, "; return_local\n");
  fprintf(out, "  ldx locals\n");
  fprintf(out, "  lda stack_lo - %d,x\n", LOCALS(index));
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  lda stack_hi - %d,x\n", LOCALS(index));
  fprintf(out, "  sta result + 1\n");

  fprintf(out, "  lda locals\n");
  fprintf(out, "  sta SP\n");

  if (!is_main)
  {
    POP_HI;
    POP_LO;
    fprintf(out, "  sta locals\n");
  }

  fprintf(out, "  rts\n");

  return 0;
}

int M6502::return_integer(int local_count)
{
  fprintf(out, "; return_integer\n");
  POP_HI;
  fprintf(out, "  sta result + 1\n");
  POP_LO;
  fprintf(out, "  sta result + 0\n");
  stack--;

  fprintf(out, "  lda locals\n");
  fprintf(out, "  sta SP\n");

  if (!is_main)
  {
    POP_HI;
    POP_LO;
    fprintf(out, "  sta locals\n");
  }

  fprintf(out, "  rts\n");

  return 0;
}

int M6502::return_void(int local_count)
{
  fprintf(out, "; return_void\n");
  fprintf(out, "  lda locals\n");
  fprintf(out, "  sta SP\n");

  if (!is_main)
  {
    POP_HI;
    POP_LO;
    fprintf(out, "  sta locals\n");
  }

  fprintf(out, "  rts\n");

  return 0;
}

int M6502::jump(const char *name)
{
  fprintf(out, "  jmp %s\n", name);

  return 0;
}

int M6502::call(const char *name)
{
  fprintf(out, "  jsr %s\n", name);

  return 0;
}

int M6502::invoke_static_method(const char *name, int params, int is_void)
{
int local;
int stack_vars = stack;

  printf("invoke_static_method() name=%s params=%d is_void=%d\n", name, params, is_void);

  fprintf(out, "; invoke_static_method\n");

  local = -params;
  while(local != 0)
  {
    if (stack_vars > 0)
    {
      fprintf(out, "  ldx SP\n");
      fprintf(out, "  lda stack_lo + 1 + %d,x\n", (stack - stack_vars));
      fprintf(out, "  sta stack_lo + 1 %d,x\n", local-1);
      fprintf(out, "  lda stack_hi + 1 + %d,x\n", (stack - stack_vars));
      fprintf(out, "  sta stack_hi + 1 %d,x\n", local-1);
      stack_vars--;
    }

    local++;
  }

  fprintf(out, "  jsr %s\n", name);

  if ((stack - stack_vars) > 0)
  {
    fprintf(out, "  lda SP\n");
    fprintf(out, "  clc\n");
    fprintf(out, "  adc #%d\n", (stack - stack_vars));
    fprintf(out, "  sta SP\n");

    params -= (stack - stack_vars);
  }

  if (!is_void)
  {
    fprintf(out, "  lda result + 0\n");
    PUSH_LO;
    fprintf(out, "  lda result + 1\n");
    PUSH_HI;
    stack++;
  }

  return 0;
}

int M6502::put_static(const char *name, int index)
{
  if (stack > 0)
  {
    POP_HI;
    fprintf(out, "  sta %s + 1\n", name);
    POP_LO;
    fprintf(out, "  sta %s + 0\n", name);
    stack--;
  }

  return 0;
}

int M6502::get_static(const char *name, int index)
{
  fprintf(out, "  lda %s + 0\n", name);
  PUSH_LO;
  fprintf(out, "  lda %s + 1\n", name);
  PUSH_HI;
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
      fprintf(out, "jsr new_array_int\n");
    }
      else
    {
      need_array_byte_support = 1;
      fprintf(out, "jsr new_array_byte\n");
    }
  }

  return 0;
}

int M6502::insert_array(const char *name, int32_t *data, int len, uint8_t type)
{
  fprintf(out, "; insert_array\n");

  if (type == TYPE_BYTE)
  {
    fprintf(out, ".align 16\n");
    return insert_db(name, data, len, TYPE_SHORT);
  }
    else
  if (type == TYPE_SHORT)
  {
    fprintf(out, ".align 16\n");
    return insert_dw(name, data, len, TYPE_SHORT);
  }
    else
  if (type == TYPE_INT)
  {
    fprintf(out, ".align 16\n");
    return insert_dw(name, data, len, TYPE_SHORT);
  }

  return -1;
}

int M6502::insert_string(const char *name, uint8_t *bytes, int len)
{
  return -1;
}

int M6502::push_array_length()
{
  if (stack > 0)
  {
    need_push_array_length = 1;
    fprintf(out, "jsr push_array_length\n");
  }

  return 0;
}

int M6502::push_array_length(const char *name, int field_id)
{
  need_push_array_length2 = 1;
  fprintf(out, "  lda %s + 0\n", name);
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda %s + 1\n", name);
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "jsr push_array_length2\n");
  stack++;

  return 0;
}

int M6502::array_read_byte()
{
  get_values_from_stack(2);
  fprintf(out, "jsr array_read_byte\n");
  stack++;

  return 0;
}

int M6502::array_read_short()
{
  return array_read_int();
}

int M6502::array_read_int()
{
  get_values_from_stack(2);
  fprintf(out, "jsr array_read_int\n");
  stack++;

  return 0;
}

int M6502::array_read_byte(const char *name, int field_id)
{
  need_array_byte_support = 1;
  if (stack > 0)
  {
    fprintf(out, "  lda %s + 0\n", name);
    fprintf(out, "  sta address + 0\n");
    fprintf(out, "  lda %s + 1\n", name);
    fprintf(out, "  sta address + 1\n");

    fprintf(out, "jsr array_read_byte2\n");
    stack++;
  }

  return 0;
}

int M6502::array_read_short(const char *name, int field_id)
{
  return array_read_int(name, field_id);
}

int M6502::array_read_int(const char *name, int field_id)
{
  need_array_int_support = 1;

  if (stack > 0)
  {
    fprintf(out, "  lda %s + 0\n", name);
    fprintf(out, "  sta address + 0\n");
    fprintf(out, "  lda %s + 1\n", name);
    fprintf(out, "  sta address + 1\n");
    fprintf(out, "jsr array_read_int2\n");
    stack++;
  }

  return 0;
}

int M6502::array_write_byte()
{
  get_values_from_stack(3);
  fprintf(out, "jsr array_write_byte\n");

  return 0;
}

int M6502::array_write_short()
{
  return array_write_int();
}

int M6502::array_write_int()
{
  get_values_from_stack(3);
  fprintf(out, "jsr array_write_int\n");

  return 0;
}

int M6502::array_write_byte(const char *name, int field_id)
{
  get_values_from_stack(2);
  fprintf(out, "; array_write_byte2\n");
  fprintf(out, "  clc\n"); 
  fprintf(out, "  lda value2 + 0\n"); 
  fprintf(out, "  adc %s + 0\n", name); 
  fprintf(out, "  sta address + 0\n"); 
  fprintf(out, "  lda value2 + 1\n"); 
  fprintf(out, "  adc %s + 1\n", name); 
  fprintf(out, "  sta address + 1\n"); 

  fprintf(out, "  ldy #0\n"); 
  fprintf(out, "  lda value1 + 0\n"); 
  fprintf(out, "  sta (address),y\n"); 

  return 0;
}

int M6502::array_write_short(const char *name, int field_id)
{
  return array_write_int(name, field_id);
}

int M6502::array_write_int(const char *name, int field_id)
{
  get_values_from_stack(2);
  fprintf(out, "; array_write_int2\n");
  fprintf(out, "  asl value2 + 0\n");
  fprintf(out, "  rol value2 + 1\n");

  fprintf(out, "  clc\n"); 
  fprintf(out, "  lda value2 + 0\n"); 
  fprintf(out, "  adc %s + 0\n", name); 
  fprintf(out, "  sta address + 0\n"); 
  fprintf(out, "  lda value2 + 1\n"); 
  fprintf(out, "  adc %s + 1\n", name); 
  fprintf(out, "  sta address + 1\n"); 

  fprintf(out, "  ldy #0\n"); 
  fprintf(out, "  lda value1 + 0\n"); 
  fprintf(out, "  sta (address),y\n"); 
  fprintf(out, "  ldy #1\n"); 
  fprintf(out, "  lda value1 + 1\n"); 
  fprintf(out, "  sta (address),y\n"); 

  return 0;
}

int M6502::get_values_from_stack(int num)
{
  fprintf(out, "; get_values_from_stack, num = %d\n", num);
  if(num > 0)
  {
    POP_HI;
    fprintf(out, "  sta value1 + 1\n");
    POP_LO;
    fprintf(out, "  sta value1 + 0\n");
    stack--;
  }

  if(num > 1)
  {
    POP_HI;
    fprintf(out, "  sta value2 + 1\n");
    POP_LO;
    fprintf(out, "  sta value2 + 0\n");
    stack--;
  }

  if(num > 2)
  {
    POP_HI;
    fprintf(out, "  sta value3 + 1\n");
    POP_LO;
    fprintf(out, "  sta value3 + 0\n");
    stack--;
  }

  return 0;
}

// subroutines
void M6502::insert_swap()
{
  fprintf(out, "swap:\n");
  fprintf(out, "  ldx SP\n");
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
  POP_HI;
  fprintf(out, "  sta result + 1\n");
  POP_LO;
  fprintf(out, "  sta result + 0\n");
  POP_HI;
  fprintf(out, "  tax\n");
  POP_LO;
  fprintf(out, "  clc\n");
  fprintf(out, "  adc result + 0\n");
  fprintf(out, "  sta result + 0\n");
  PUSH_LO;
  fprintf(out, "  txa\n");
  fprintf(out, "  adc result + 1\n");
  fprintf(out, "  sta result + 1\n");
  PUSH_HI;
  fprintf(out, "  rts\n");
}

void M6502::insert_sub_integer()
{
  fprintf(out, "sub_integer:\n");
  POP_HI;
  fprintf(out, "  sta result + 1\n");
  POP_LO;
  fprintf(out, "  sta result + 0\n");
  POP_HI;
  fprintf(out, "  tax\n");
  POP_LO;
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc result + 0\n");
  fprintf(out, "  sta result + 0\n");
  PUSH_LO;
  fprintf(out, "  txa\n");
  fprintf(out, "  sbc result + 1\n");
  fprintf(out, "  sta result + 1\n");
  PUSH_HI;
  fprintf(out, "  rts\n");
}

void M6502::insert_mul_integer()
{
  fprintf(out, "mul_integer:\n");
  // load values
  POP_HI;
  fprintf(out, "  sta value2 + 1\n");
  POP_LO;
  fprintf(out, "  sta value2 + 0\n");
  POP_HI;
  fprintf(out, "  sta value1 + 1\n");
  POP_LO;
  fprintf(out, "  sta value1 + 0\n");

  // clear result
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  ldx #16\n");

  // loop
  fprintf(out, "  asl result + 0\n");
  fprintf(out, "  rol result + 1\n");
  fprintf(out, "  asl value1 + 0\n");
  fprintf(out, "  rol value1 + 1\n");
  fprintf(out, "  bcc #13\n");

  // add
  fprintf(out, "  clc\n");
  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  adc value2 + 0\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  lda result + 1\n");
  fprintf(out, "  adc value2 + 1\n");
  fprintf(out, "  sta result + 1\n");

  // next
  fprintf(out, "  dex\n");
  fprintf(out, "  bne #-26\n");

  // push result
  fprintf(out, "  lda result + 0\n");
  PUSH_LO;
  fprintf(out, "  lda result + 1\n");
  PUSH_HI;
  fprintf(out, "  rts\n");
}

void M6502::insert_div_integer()
{
  fprintf(out, "div_integer:\n");
  // load values
  POP_HI;
  fprintf(out, "  sta value2 + 1\n");
  POP_LO;
  fprintf(out, "  sta value2 + 0\n");

  POP_HI;
  fprintf(out, "  sta value1 + 1\n");
  POP_LO;
  fprintf(out, "  sta value1 + 0\n");

  // clear remainder
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta remainder + 0\n");
  fprintf(out, "  sta remainder + 1\n");
  fprintf(out, "  ldx #16\n");

  // loop
  fprintf(out, "  asl value1 + 0\n");
  fprintf(out, "  rol value1 + 1\n");
  fprintf(out, "  rol remainder + 0\n");
  fprintf(out, "  rol remainder + 1\n");

  // sub
  fprintf(out, "  sec\n");
  fprintf(out, "  lda remainder + 0\n");
  fprintf(out, "  sbc value2 + 0\n");
  fprintf(out, "  tay\n");
  fprintf(out, "  lda remainder + 1\n");
  fprintf(out, "  sbc value2 + 1\n");
  fprintf(out, "  bcc #6\n");

  fprintf(out, "  sta remainder + 1\n");
  fprintf(out, "  sty remainder + 0\n");
  fprintf(out, "  inc value1 + 0\n");

  // next
  fprintf(out, "  dex\n");
  fprintf(out, "  bne #-29\n");

  // push result
  fprintf(out, "  lda value1 + 0\n");
  fprintf(out, "  sta result + 0\n");
  PUSH_LO;
  fprintf(out, "  lda value1 + 1\n");
  fprintf(out, "  sta result + 1\n");
  PUSH_HI;
  fprintf(out, "  rts\n");
}

void M6502::insert_mod_integer()
{
  fprintf(out, "mod_integer:\n");
  POP_HI;
  POP_LO;
  fprintf(out, "  lda remainder + 0\n");
  fprintf(out, "  sta result + 0\n");
  PUSH_LO;
  fprintf(out, "  lda remainder + 1\n");
  fprintf(out, "  sta result + 1\n");
  PUSH_HI;
  fprintf(out, "  rts\n");
}

void M6502::insert_neg_integer()
{
  fprintf(out, "neg_integer:\n");
  POP_HI;
  fprintf(out, "  sta result + 1\n");
  POP_LO;
  fprintf(out, "  sta result + 0\n");

  fprintf(out, "  sec\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sbc result + 0\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sbc result + 1\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  lda result + 0\n");
  PUSH_LO;
  fprintf(out, "  lda result + 1\n");
  PUSH_HI;
  fprintf(out, "  rts\n");
}

void M6502::insert_shift_left_integer()
{
  fprintf(out, "shift_left_integer:\n");
  POP_HI;
  POP_LO;
  fprintf(out, "  tax\n");

  POP_HI;
  fprintf(out, "  sta result + 1\n");
  POP_LO;
  fprintf(out, "  sta result + 0\n");

  fprintf(out, "  asl result + 0\n");
  fprintf(out, "  rol result + 1\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  bne #-7\n");
  fprintf(out, "  lda result + 0\n");
  PUSH_LO;
  fprintf(out, "  lda result + 1\n");
  PUSH_HI;
  fprintf(out, "  rts\n");
}

void M6502::insert_shift_right_integer()
{
  fprintf(out, "shift_right_integer:\n");
  POP_HI;
  POP_LO;
  fprintf(out, "  tax\n");

  POP_HI;
  fprintf(out, "  sta result + 1\n");
  POP_LO;
  fprintf(out, "  sta result + 0\n");

  fprintf(out, "  lda result + 1\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  ror result + 1\n");
  fprintf(out, "  ror result + 0\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  bne #-10\n");

  fprintf(out, "  lda result + 0\n");
  PUSH_LO;
  fprintf(out, "  lda result + 1\n");
  PUSH_HI;
  fprintf(out, "  rts\n");
}

void M6502::insert_shift_right_uinteger()
{
  fprintf(out, "shift_right_uinteger:\n");
  POP_HI;
  POP_LO;
  fprintf(out, "  tax\n");

  POP_HI;
  fprintf(out, "  sta result + 1\n");
  POP_LO;
  fprintf(out, "  sta result + 0\n");

  fprintf(out, "  lsr result + 1\n");
  fprintf(out, "  ror result + 0\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  bne #-7\n");
  fprintf(out, "  lda result + 0\n");
  PUSH_LO;
  fprintf(out, "  lda result + 1\n");
  PUSH_HI;
  fprintf(out, "  rts\n");
}

void M6502::insert_and_integer()
{
  fprintf(out, "and_integer:\n");
  POP_HI;
  fprintf(out, "  sta result + 1\n");
  POP_LO;
  fprintf(out, "  sta result + 0\n");
  POP_HI;
  fprintf(out, "  and result + 1\n");
  fprintf(out, "  sta result + 1\n");
  POP_LO;
  fprintf(out, "  and result + 0\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  lda result + 0\n");
  PUSH_LO;
  fprintf(out, "  lda result + 1\n");
  PUSH_HI;
  fprintf(out, "  rts\n");
}

void M6502::insert_or_integer()
{
  fprintf(out, "or_integer:\n");
  POP_HI;
  fprintf(out, "  sta result + 1\n");
  POP_LO;
  fprintf(out, "  sta result + 0\n");
  POP_HI;
  fprintf(out, "  ora result + 1\n");
  fprintf(out, "  sta result + 1\n");
  POP_LO;
  fprintf(out, "  ora result + 0\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  lda result + 0\n");
  PUSH_LO;
  fprintf(out, "  lda result + 1\n");
  PUSH_HI;
  fprintf(out, "  rts\n");
}

void M6502::insert_xor_integer()
{
  fprintf(out, "xor_integer:\n");
  POP_HI;
  fprintf(out, "  sta result + 1\n");
  POP_LO;
  fprintf(out, "  sta result + 0\n");
  POP_HI;
  fprintf(out, "  eor result + 1\n");
  fprintf(out, "  sta result + 1\n");
  POP_LO;
  fprintf(out, "  eor result + 0\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  lda result + 0\n");
  PUSH_LO;
  fprintf(out, "  lda result + 1\n");
  PUSH_HI;
  fprintf(out, "  rts\n");
}

void M6502::insert_push_array_length()
{
  // push_array_length
  fprintf(out, "push_array_length:\n");
  POP_HI;
  fprintf(out, "  sta result + 1\n");
  POP_LO;
  fprintf(out, "  sta result + 0\n");

  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda result + 1\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  lda address + 0\n");
  fprintf(out, "  sbc #2\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda address + 1\n");
  fprintf(out, "  sbc #0\n");
  fprintf(out, "  sta address + 1\n");

  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  ldy #1\n");
  fprintf(out, "  lda (address),y)\n");
  fprintf(out, "  sta result + 1\n");

  fprintf(out, "  lda result + 0\n");
  PUSH_LO;
  fprintf(out, "  lda result + 1\n");
  PUSH_HI;
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
  PUSH_LO;
  fprintf(out, "  ldy #1\n");
  fprintf(out, "  lda (address),y\n");
  PUSH_HI;
  fprintf(out, "  rts\n");
}

void M6502::insert_array_byte_support()
{
  // new_array byte
  fprintf(out, "new_array_byte:\n");
  POP_HI;
  fprintf(out, "  sta length + 1\n");
  POP_LO;
  fprintf(out, "  sta length + 0\n");

  fprintf(out, "  lda heap_ptr + 0\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  lda heap_ptr + 1\n");
  fprintf(out, "  sta result + 1\n");

  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda result + 1\n");
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
  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  adc #3\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  lda result + 1\n");
  fprintf(out, "  adc #0\n");
  fprintf(out, "  sta result + 1\n");

  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  and #254\n");
  fprintf(out, "  sta result + 0\n");

  fprintf(out, "  lda result + 0\n");
  PUSH_LO;
  fprintf(out, "  lda result + 1\n");
  PUSH_HI;
  fprintf(out, "  rts\n");

  // array_read_byte
  fprintf(out, "array_read_byte:\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda value2 + 0\n");
  fprintf(out, "  adc value1 + 0\n");
  fprintf(out, "  sta value2 + 0\n");
  fprintf(out, "  lda value2 + 1\n");
  fprintf(out, "  adc value1 + 1\n");
  fprintf(out, "  sta value2 + 1\n");

  fprintf(out, "  lda value2 + 0\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda value2 + 1\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta result + 0\n");

  fprintf(out, "  lda result + 0\n");
  PUSH_LO;
  // sign-extend
  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  bpl #15\n");
  fprintf(out, "  lda #0xff\n");
  fprintf(out, "  sta result + 1\n");
  PUSH_HI;
  fprintf(out, "  lda #0\n");
  fprintf(out, "  beq #11\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta result + 1\n");
  PUSH_HI;
  fprintf(out, "  rts\n");

  // array_read_byte2
  fprintf(out, "array_read_byte2:\n");
  POP_HI;
  fprintf(out, "  sta result + 1\n");
  POP_LO;
  fprintf(out, "  sta result + 0\n");

  fprintf(out, "  clc\n");
  fprintf(out, "  lda address + 0\n");
  fprintf(out, "  adc result + 0\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda address + 1\n");
  fprintf(out, "  adc result + 1\n");
  fprintf(out, "  sta address + 1\n");

  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  bmi #6\n");
  fprintf(out, "  sty result + 1\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  beq #4\n");
  fprintf(out, "  lda #0xff\n");
  fprintf(out, "  sta result + 1\n");

  fprintf(out, "  lda result + 0\n");
  PUSH_LO;
  fprintf(out, "  lda result + 1\n");
  PUSH_HI;
  fprintf(out, "  rts\n");

  // array_write_byte
  fprintf(out, "array_write_byte:\n");

  fprintf(out, "  clc\n"); 
  fprintf(out, "  lda value3 + 0\n"); 
  fprintf(out, "  adc value2 + 0\n"); 
  fprintf(out, "  sta address + 0\n"); 
  fprintf(out, "  lda value3 + 1\n"); 
  fprintf(out, "  adc value2 + 1\n"); 
  fprintf(out, "  sta address + 1\n"); 

  fprintf(out, "  ldy #0\n"); 
  fprintf(out, "  lda value1 + 0\n"); 
  fprintf(out, "  sta (address),y\n"); 
  fprintf(out, "  rts\n");
}

void M6502::insert_array_int_support()
{
  // new_array int
  fprintf(out, "new_array_int:\n");
  POP_HI;
  fprintf(out, "  sta length + 1\n");
  POP_LO;
  fprintf(out, "  sta length + 0\n");

  fprintf(out, "  lda heap_ptr + 0\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  lda heap_ptr + 1\n");
  fprintf(out, "  sta result + 1\n");

  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda result + 1\n");
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
  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  adc #3\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  lda result + 1\n");
  fprintf(out, "  adc #0\n");
  fprintf(out, "  sta result + 1\n");

  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  and #254\n");
  fprintf(out, "  sta result + 0\n");

  fprintf(out, "  lda result + 0\n");
  PUSH_LO;
  fprintf(out, "  lda result + 1\n");
  PUSH_HI;
  fprintf(out, "  rts\n");

  // array_read_int
  fprintf(out, "array_read_int:\n");
  // index * 2 for int
  fprintf(out, "  asl value1 + 0\n");
  fprintf(out, "  rol value1 + 1\n");

  fprintf(out, "  clc\n");
  fprintf(out, "  lda value2 + 0\n");
  fprintf(out, "  adc value1 + 0\n");
  fprintf(out, "  sta value2 + 0\n");
  fprintf(out, "  lda value2 + 1\n");
  fprintf(out, "  adc value1 + 1\n");
  fprintf(out, "  sta value2 + 1\n");

  fprintf(out, "  lda value2 + 0\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda value2 + 1\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  ldy #1\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta result + 1\n");

  fprintf(out, "  lda result + 0\n");
  PUSH_LO;
  fprintf(out, "  lda result + 1\n");
  PUSH_HI;
  fprintf(out, "  rts\n");

  // array_read_int2
  fprintf(out, "array_read_int2:\n");
  POP_HI;
  fprintf(out, "  sta result + 1\n");
  POP_LO;
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  asl result + 0\n");
  fprintf(out, "  rol result + 1\n");

  fprintf(out, "  clc\n");
  fprintf(out, "  lda address + 0\n");
  fprintf(out, "  adc result + 0\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda address + 1\n");
  fprintf(out, "  adc result + 1\n");
  fprintf(out, "  sta address + 1\n");

  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  ldy #1\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta result + 1\n");

  fprintf(out, "  lda result + 0\n");
  PUSH_LO;
  fprintf(out, "  lda result + 1\n");
  PUSH_HI;
  fprintf(out, "  rts\n");

  // array_write_int
  fprintf(out, "array_write_int:\n");
  fprintf(out, "  asl value2 + 0\n");
  fprintf(out, "  rol value2 + 1\n");

  fprintf(out, "  clc\n"); 
  fprintf(out, "  lda value3 + 0\n"); 
  fprintf(out, "  adc value2 + 0\n"); 
  fprintf(out, "  sta address + 0\n"); 
  fprintf(out, "  lda value3 + 1\n"); 
  fprintf(out, "  adc value2 + 1\n"); 
  fprintf(out, "  sta address + 1\n"); 

  fprintf(out, "  ldy #0\n"); 
  fprintf(out, "  lda value1 + 0\n"); 
  fprintf(out, "  sta (address),y\n"); 
  fprintf(out, "  ldy #1\n"); 
  fprintf(out, "  lda value1 + 1\n"); 
  fprintf(out, "  sta (address),y\n"); 
  fprintf(out, "  rts\n");
}

// Memory API
int M6502::memory_read8()
{
  fprintf(out, "; memory_read8\n");
  POP_HI;
  fprintf(out, "  sta address + 1\n");
  POP_LO;
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta result + 0\n");
  PUSH_LO;

  // sign-extend
  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  bpl #15\n");
  fprintf(out, "  lda #0xff\n");
  fprintf(out, "  sta result + 1\n");
  PUSH_HI;
  fprintf(out, "  lda #0\n");
  fprintf(out, "  beq #11\n");

  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta result + 1\n");
  PUSH_HI;

  return 0;
}

int M6502::memory_write8()
{
  fprintf(out, "; memory_write8\n");
  POP_HI;
  POP_LO;
  fprintf(out, "  tax\n");
  POP_HI;
  fprintf(out, "  sta address + 1\n");
  POP_LO;
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  txa\n");
  fprintf(out, "  sta (address),y\n");

  stack -= 2;

  return 0;
}

int M6502::memory_read16()
{
  fprintf(out, "; memory_read16\n");
  POP_HI;
  fprintf(out, "  sta address + 1\n");
  POP_LO;
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta result + 0\n");
  PUSH_LO;
  fprintf(out, "  iny\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta result + 1\n");
  PUSH_HI;

  return 0;
}

int M6502::memory_write16()
{
  fprintf(out, "; memory_write16\n");
  POP_HI;
  fprintf(out, "  sta result + 1\n");
  POP_LO;
  fprintf(out, "  sta result + 0\n");
  POP_HI;
  fprintf(out, "  sta address + 1\n");
  POP_LO;
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  sta (address),y\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  lda result + 1\n");
  fprintf(out, "  sta (address),y\n");

  stack -= 2;

  return 0;
}
#if 0
void M6502::close()
{
}
#endif

