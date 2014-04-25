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
#include <string.h>
#include <stdint.h>

#include "AVR8.h"

// ABI is:
// r0 result0
// r1 result1
// r2 remainder0
// r3 remainder1
// r4 length0 
// r5 length1
// r6
// r7
// r8
// r9
// r10 zero
// r11 one
// r12 two
// r13 three
// r14 ff
// r15
// r16 value10
// r17 value11
// r18 value20
// r19 value21
// r20 value30
// r21 value31
// r22 temp
// r23 temp2
// r24 locals
// r25 SP
// r26 XL
// r27 XH
// r28 YL
// r29 YH
// r30 ZL
// r31 ZH

#define PUSH_LO(a) \
  fprintf(out, "; PUSH_LO\n"); \
  fprintf(out, "  ldi YL, stack_lo\n"); \
  fprintf(out, "  add YL, SP\n"); \
  fprintf(out, "  st Y, %s\n", a)

#define PUSH_HI(a) \
  fprintf(out, "; PUSH_HI\n"); \
  fprintf(out, "  ldi YL, stack_hi\n"); \
  fprintf(out, "  add YL, SP\n"); \
  fprintf(out, "  st Y, %s\n", a); \
  fprintf(out, "  dec SP\n")

#define POP_HI(a) \
  fprintf(out, "; POP_HI\n"); \
  fprintf(out, "  inc SP\n"); \
  fprintf(out, "  ldi YL, stack_hi\n"); \
  fprintf(out, "  add YL, SP\n"); \
  fprintf(out, "  ld %s, Y\n", a)

#define POP_LO(a) \
  fprintf(out, "; POP_LO\n"); \
  fprintf(out, "  ldi YL, stack_lo\n"); \
  fprintf(out, "  add YL, SP\n"); \
  fprintf(out, "  ld %s, Y\n", a)

#define LOCALS(a) (a)

AVR8::AVR8() :
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
  need_inc_integer(0),
  need_integer_to_byte(0),
  need_jump_cond(0),
  need_jump_cond_integer(0),
  need_push_integer_local(0),
  need_pop_integer_local(0),
  need_dup(0),
  need_push_array_length(0),
  need_push_array_length2(0),
  need_array_byte_support(0),
  need_array_int_support(0)
{

}

AVR8::~AVR8()
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
  if(need_inc_integer) { insert_inc_integer(); }
  if(need_integer_to_byte) { insert_integer_to_byte(); }
  if(need_jump_cond) { insert_jump_cond(); }
  if(need_jump_cond_integer) { insert_jump_cond_integer(); }
  if(need_push_integer_local) { insert_push_integer_local(); }
  if(need_pop_integer_local) { insert_pop_integer_local(); }
  if(need_dup) { insert_dup(); }
  if(need_push_array_length) { insert_push_array_length(); }
  if(need_push_array_length2) { insert_push_array_length2(); }
  if(need_array_byte_support) { insert_array_byte_support(); }
  if(need_array_int_support) { insert_array_int_support(); }
}

int AVR8::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".avr8\n\n");

  // defines
  fprintf(out, "result0 equ r0\n");
  fprintf(out, "result1 equ r1\n");
  fprintf(out, "remainder0 equ r2\n");
  fprintf(out, "remainder1 equ r3\n");
  fprintf(out, "length0 equ r4\n");
  fprintf(out, "length1 equ r5\n");
  fprintf(out, "zero equ r10\n");
  fprintf(out, "one equ r11\n");
  fprintf(out, "two equ r12\n");
  fprintf(out, "three equ r13\n");
  fprintf(out, "ff equ r14\n");
  fprintf(out, "value10 equ r16\n");
  fprintf(out, "value11 equ r17\n");
  fprintf(out, "value20 equ r18\n");
  fprintf(out, "value21 equ r19\n");
  fprintf(out, "value30 equ r20\n");
  fprintf(out, "value31 equ r21\n");
  fprintf(out, "temp equ r22\n");
  fprintf(out, "temp2 equ r23\n");
  fprintf(out, "locals equ r24\n");
  fprintf(out, "SP equ r25\n");
  fprintf(out, "XL equ r26\n");
  fprintf(out, "XH equ r27\n");
  fprintf(out, "YL equ r28\n");
  fprintf(out, "YH equ r29\n");
  fprintf(out, "ZL equ r30\n");
  fprintf(out, "ZH equ r31\n");
  fprintf(out, "SPL equ 0x3d\n");
  fprintf(out, "SPH equ 0x3e\n");

  // java stack base locations
  fprintf(out, "stack_lo equ 0x60\n");
  fprintf(out, "stack_hi equ 0x70\n");

  // RAMEND (change to particular chip)
  fprintf(out, "RAMEND equ 0x9f\n");

  // heap
  fprintf(out, "ram_start equ 0x80\n");
  fprintf(out, "heap_ptr equ ram_start\n\n");

  // startup
  fprintf(out, ".org 0x0000\n");
  fprintf(out, "  rjmp start\n");
  fprintf(out, "start:\n");

  // constants
  fprintf(out, "  ldi temp, 0\n");
  fprintf(out, "  mov zero, temp\n");
  fprintf(out, "  ldi temp, 1\n");
  fprintf(out, "  mov one, temp\n");
  fprintf(out, "  ldi temp, 2\n");
  fprintf(out, "  mov two, temp\n");
  fprintf(out, "  ldi temp, 3\n");
  fprintf(out, "  mov three, temp\n");
  fprintf(out, "  ldi temp, 0xff\n");
  fprintf(out, "  mov ff, temp\n");

  // processor stack setup
//  fprintf(out, "  ldi r16, RAMEND & 0xff\n");
//  fprintf(out, "  out SPL, r16\n");
//  fprintf(out, "  ldi r16, RAMEND >> 8\n");
//  fprintf(out, "  out SPH, r16\n");

  // java stack setup
  fprintf(out, "  ldi SP, 0xf\n");
  fprintf(out, "  ldi XL, 0\n");
  fprintf(out, "  ldi XH, 0\n");
  fprintf(out, "  ldi YL, 0\n");
  fprintf(out, "  ldi YH, 0\n");
  fprintf(out, "  ldi ZL, 0\n");
  fprintf(out, "  ldi ZH, 0\n");

  return 0;
}

int AVR8::start_init()
{
  return 0;
}

int AVR8::insert_static_field_define(const char *name, const char *type, int index)
{
  fprintf(out, "%s equ ram_start + %d\n", name, (index + 1) * 2);

  return 0;
}

int AVR8::init_heap(int field_count)
{
  fprintf(out, "  ; Set up heap and static initializers\n");
  fprintf(out, "  ldi temp, (ram_start + %d) & 0xff\n", (field_count + 1) * 2);
  fprintf(out, "  sts ram_start + 0, temp\n");
  fprintf(out, "  ldi temp, (ram_start + %d) >> 8\n", (field_count + 1) * 2);
  fprintf(out, "  sts ram_start + 1, temp\n");

  return 0;
}

int AVR8::insert_field_init_boolean(char *name, int index, int value)
{
  value = (value == 0) ? 0 : 1;
  fprintf(out, "; insert_field_init_boolean\n");
  fprintf(out, "  ldi temp, %d\n", value & 0xff);
  fprintf(out, "  sts %s + 0, temp\n", name);
  fprintf(out, "  ldi temp, %d\n", value >> 8);
  fprintf(out, "  sts %s + 1, temp\n", name);

  return 0;
}

int AVR8::insert_field_init_byte(char *name, int index, int value)
{
  if (value < -128 || value > 255) { return -1; }
  int16_t n = value;
  uint16_t v = (n & 0xffff);

  fprintf(out, "; insert_field_init_byte\n");
  fprintf(out, "  ldi temp, %d\n", (uint8_t)v & 0xff);
  fprintf(out, "  sts %s + 0, temp\n", name);
  fprintf(out, "  ldi temp, %d\n", (uint8_t)v >> 8);
  fprintf(out, "  sts %s + 1, temp\n", name);

  return 0;
}

int AVR8::insert_field_init_short(char *name, int index, int value)
{
  if (value < -32768 || value > 65535) { return -1; }

  fprintf(out, "; insert_field_init_short\n");
  fprintf(out, "  ldi temp, %d\n", value & 0xff);
  fprintf(out, "  sts %s + 0, temp\n", name);
  fprintf(out, "  ldi temp, %d\n", value >> 8);
  fprintf(out, "  sts %s + 1, temp\n", name);

  return 0;
}

int AVR8::insert_field_init_int(char *name, int index, int value)
{
  return insert_field_init_short(name, index, value);
}

int AVR8::insert_field_init(char *name, int index)
{
  fprintf(out, "; insert_field_init\n");
  fprintf(out, "  ldi temp, (_%s * 2) & 0xff\n", name);
  fprintf(out, "  sts %s + 0, temp\n", name);
  fprintf(out, "  ldi temp, (_%s * 2) >> 8\n", name);
  fprintf(out, "  sts %s + 1, temp\n", name);

  return 0;
}

void AVR8::method_start(int local_count, int max_stack, int param_count, const char *name)
{
  stack = 0;

  is_main = (strcmp(name, "main") == 0) ? 1 : 0;

  fprintf(out, "%s:\n", name);

  // main() function goes here
  if (!is_main)
  {
    PUSH_LO("locals");
    PUSH_HI("zero");
  }

  fprintf(out, "  mov locals, SP\n");
  fprintf(out, "  ldi temp, 0x%02x\n", local_count);
  fprintf(out, "  sub SP, temp\n");
}

void AVR8::method_end(int local_count)
{
  fprintf(out, "\n");
}

int AVR8::push_integer(int32_t n)
{
  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);

    return -1;
  }

  uint16_t value = (n & 0xffff);

  fprintf(out, "; push_integer\n");
  fprintf(out, "  ldi temp, 0x%02x\n", value & 0xff);
  PUSH_LO("temp");
  fprintf(out, "  ldi temp, 0x%02x\n", value >> 8);
  PUSH_HI("temp");
  stack++;

  return 0;
}

int AVR8::push_integer_local(int index)
{
  need_push_integer_local = 1;

  fprintf(out, "  ldi temp2, %d\n", LOCALS(index));
  fprintf(out, "  rcall push_integer_local\n");
  stack++;

  return 0;
}

int AVR8::push_ref_local(int index)
{
  return push_integer_local(index);
}

int AVR8::push_fake()
{
  return -1;
}

int AVR8::push_long(int64_t n)
{
  return push_integer((int32_t)n);
}

int AVR8::push_float(float f)
{
  printf("float is not supported right now\n");

  return -1;
}

int AVR8::push_double(double f)
{
  printf("double is not supported right now\n");

  return -1;
}

int AVR8::push_byte(int8_t b)
{
  int16_t n = b;
  uint16_t value = (n & 0xffff);

  fprintf(out, "; push_byte\n");
  fprintf(out, "  ldi temp, 0x%02x\n", value & 0xff);
  PUSH_LO("temp");
  fprintf(out, "  ldi temp, 0x%02x\n", value >> 8);
  PUSH_HI("temp");
  stack++;

  return 0;
}

int AVR8::push_short(int16_t s)
{
  uint16_t value = (s & 0xffff);

  fprintf(out, "; push_short\n");
  fprintf(out, "  ldi temp, 0x%02x\n", value & 0xff);
  PUSH_LO("temp");
  fprintf(out, "  ldi temp, 0x%02x\n", value >> 8);
  PUSH_HI("temp");
  stack++;

  return 0;
}

int AVR8::pop_integer_local(int index)
{
  need_pop_integer_local = 1;

  fprintf(out, "  ldi temp2, %d\n", LOCALS(index));
  fprintf(out, "  rcall pop_integer_local\n");
  stack--;

  return 0;
}

int AVR8::pop_ref_local(int index)
{
  return pop_integer_local(index);
}

int AVR8::pop()
{
  fprintf(out, "; pop\n");
  POP_HI("result1");
  POP_LO("result0");
  stack--;

  return 0;
}

int AVR8::dup()
{
  need_dup = 1;

  fprintf(out, "  rcall dup\n");
  stack++;

  return 0;
}

int AVR8::dup2()
{
  printf("Need to implement dup2()\n");

  return -1;
}

int AVR8::swap()
{
  need_swap = 1;
  fprintf(out, "  rcall swap\n");

  return 0;
}

int AVR8::add_integer()
{
  need_add_integer = 1;
  fprintf(out, "  rcall add_integer\n");
  stack--;

  return 0;
}

int AVR8::add_integer(int const_val)
{
  return -1;
}

int AVR8::sub_integer()
{
  need_sub_integer = 1;
  fprintf(out, "  rcall sub_integer\n");
  stack--;

  return 0;
}

int AVR8::sub_integer(int const_val)
{
  return -1;
}

int AVR8::mul_integer()
{
  need_mul_integer = 1;
  fprintf(out, "  rcall mul_integer\n");
  stack--;

  return 0;
}

int AVR8::mul_integer(int const_val)
{
  return -1;
}

// unsigned only for now
int AVR8::div_integer()
{
  need_div_integer = 1;
  fprintf(out, "  rcall div_integer\n");
  stack--;

  return 0;
}

int AVR8::div_integer(int const_val)
{
  return -1;
}

// unsigned only for now
int AVR8::mod_integer()
{
  need_div_integer = 1;
  need_mod_integer = 1;
  fprintf(out, "  rcall div_integer\n");
  fprintf(out, "  rcall mod_integer\n");
  stack--;

  return 0;
}

int AVR8::mod_integer(int const_val)
{
  return -1;
}

int AVR8::neg_integer()
{
  need_neg_integer = 1;
  fprintf(out, "  rcall neg_integer\n");

  return 0;
}

int AVR8::shift_left_integer()
{
  need_shift_left_integer = 1;
  fprintf(out, "  rcall shift_left_integer\n");
  stack--;

  return 0;
}

int AVR8::shift_left_integer(int const_val)
{
  return -1;
}

int AVR8::shift_right_integer()
{
  need_shift_right_integer = 1;
  fprintf(out, "  rcall shift_right_integer\n");
  stack--;

  return 0;
}

int AVR8::shift_right_integer(int const_val)
{
  return -1;
}

int AVR8::shift_right_uinteger()
{
  need_shift_right_uinteger = 1;
  fprintf(out, "  rcall shift_right_uinteger\n");
  stack--;

  return 0;
}

int AVR8::shift_right_uinteger(int const_val)
{
  return -1;
}

int AVR8::and_integer()
{
  need_and_integer = 1;
  fprintf(out, "  rcall and_integer\n");
  stack--;

  return 0;
}

int AVR8::and_integer(int const_val)
{
  return -1;
}

int AVR8::or_integer()
{
  need_or_integer = 1;
  fprintf(out, "  rcall or_integer\n");
  stack--;

  return 0;
}

int AVR8::or_integer(int const_val)
{
  return -1;
}

int AVR8::xor_integer()
{
  need_xor_integer = 1;
  fprintf(out, "  rcall xor_integer\n");
  stack--;

  return 0;
}

int AVR8::xor_integer(int const_val)
{
  return -1;
}

int AVR8::inc_integer(int index, int num)
{
  need_inc_integer = 1;
  uint16_t value = num & 0xffff;

  fprintf(out, "  ldi value10, 0x%02x\n", value & 0xff);
  fprintf(out, "  ldi value11, 0x%02x\n", value >> 8);
  fprintf(out, "  ldi temp, %d\n", LOCALS(index));
  fprintf(out, "  rcall inc_integer\n");

  return 0;
}

int AVR8::integer_to_byte()
{
  need_integer_to_byte = 1;
  fprintf(out, "  rcall integer_to_byte\n");

  return 0;
}

int AVR8::jump_cond(const char *label, int cond)
{
  bool reverse = false;

  char label_skip[16];
  char label_jump[16];

  if (stack > 0)
  {
    need_jump_cond = 1;
    sprintf(label_skip, "label_%d", label_count++);
    sprintf(label_jump, "label_%d", label_count++);
    fprintf(out, "  rcall jump_cond\n");

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
        fprintf(out, "  cp value10, zero\n");
        fprintf(out, "  brne %s\n", label_skip);
        fprintf(out, "  cp value11, zero\n");
        fprintf(out, "  brne %s\n", label_skip);
        fprintf(out, "  rjmp %s\n", label);
        fprintf(out, "%s:\n", label_skip);
        break;
      case COND_NOT_EQUAL:
        fprintf(out, "  cp value10, zero\n");
        fprintf(out, "  brne %s\n", label_jump);
        fprintf(out, "  cp value11, zero\n");
        fprintf(out, "  breq %s\n", label_skip);
        fprintf(out, "%s:\n", label_jump);
        fprintf(out, "  rjmp %s\n", label);
        fprintf(out, "%s:\n", label_skip);
        break;
      case COND_LESS:
        if(reverse == false)
        {
          fprintf(out, "  cp value10, zero\n");
          fprintf(out, "  cpc value11, zero\n");
          fprintf(out, "  brge %s\n", label_skip);
          fprintf(out, "  rjmp %s\n", label);
          fprintf(out, "%s:\n", label_skip);
        }
          else
        {
          fprintf(out, "  cp zero, value10\n");
          fprintf(out, "  cpc zero, value11\n");
          fprintf(out, "  brge %s\n", label_skip);
          fprintf(out, "  rjmp %s\n", label);
          fprintf(out, "%s:\n", label_skip);
        }
        break;
      case COND_GREATER_EQUAL:
        if(reverse == false)
        {
          fprintf(out, "  cp value10, zero\n");
          fprintf(out, "  cpc value11, zero\n");
          fprintf(out, "  brlt %s\n", label_skip);
          fprintf(out, "  rjmp %s\n", label);
          fprintf(out, "%s:\n", label_skip);
        }
          else
        {
          fprintf(out, "  cp zero, value10\n");
          fprintf(out, "  cpc zero, value11\n");
          fprintf(out, "  brlt %s\n", label_skip);
          fprintf(out, "  rjmp %s\n", label);
          fprintf(out, "%s:\n", label_skip);
        }
        break;
    }

    stack--;
  }

  return 0;
}

int AVR8::jump_cond_integer(const char *label, int cond)
{
  bool reverse = false;

  char label_skip[16];
  char label_jump[16];

  if (stack > 1)
  {
    need_jump_cond_integer = 1;
    sprintf(label_skip, "label_%d", label_count++);
    sprintf(label_jump, "label_%d", label_count++);
    fprintf(out, "  rcall jump_cond_integer\n");

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
        fprintf(out, "  cp value10, value20\n");
        fprintf(out, "  brne %s\n", label_skip);
        fprintf(out, "  cp value11, value21\n");
        fprintf(out, "  brne %s\n", label_skip);
        fprintf(out, "  rjmp %s\n", label);
        fprintf(out, "%s:\n", label_skip);
        break;
      case COND_NOT_EQUAL:
        fprintf(out, "  cp value10, value20\n");
        fprintf(out, "  brne %s\n", label_jump);
        fprintf(out, "  cp value11, value21\n");
        fprintf(out, "  breq %s\n", label_skip);
        fprintf(out, "%s:\n", label_jump);
        fprintf(out, "  rjmp %s\n", label);
        fprintf(out, "%s:\n", label_skip);
        break;
      case COND_LESS:
        if(reverse == false)
        {
          fprintf(out, "  cp value10, value20\n");
          fprintf(out, "  cpc value11, value21\n");
          fprintf(out, "  brge %s\n", label_skip);
          fprintf(out, "  rjmp %s\n", label);
          fprintf(out, "%s:\n", label_skip);
        }
          else
        {
          fprintf(out, "  cp value20, value10\n");
          fprintf(out, "  cpc value21, value11\n");
          fprintf(out, "  brge %s\n", label_skip);
          fprintf(out, "  rjmp %s\n", label);
          fprintf(out, "%s:\n", label_skip);
        }
        break;
      case COND_GREATER_EQUAL:
        if(reverse == false)
        {
          fprintf(out, "  cp value10, value20\n");
          fprintf(out, "  cpc value11, value21\n");
          fprintf(out, "  brlt %s\n", label_skip);
          fprintf(out, "  rjmp %s\n", label);
          fprintf(out, "%s:\n", label_skip);
        }
          else
        {
          fprintf(out, "  cp value20, value10\n");
          fprintf(out, "  cpc value21, value11\n");
          fprintf(out, "  brlt %s\n", label_skip);
          fprintf(out, "  rjmp %s\n", label);
          fprintf(out, "%s:\n", label_skip);
        }
        break;
    }

    stack -= 2;
  }

  return 0;
}

int AVR8::return_local(int index, int local_count)
{
  fprintf(out, "; return_local\n");
  fprintf(out, "  ldi YL, stack_lo - %d\n", LOCALS(index));
  fprintf(out, "  add YL, locals\n");
  fprintf(out, "  ld result0, Y\n");
  fprintf(out, "  ldi YL, stack_hi - %d\n", LOCALS(index));
  fprintf(out, "  add YL, locals\n");
  fprintf(out, "  ld result1, Y\n");
  fprintf(out, "  mov SP, locals\n");

  if (!is_main)
  {
    POP_HI("locals");
    POP_LO("locals");
  }

  fprintf(out, "  ret\n\n");

  return 0;
}

int AVR8::return_integer(int local_count)
{
  fprintf(out, "; return_integer\n");
  POP_HI("result1");
  POP_LO("result0");
  stack--;

  fprintf(out, "  mov SP, locals\n");

  if (!is_main)
  {
    POP_HI("locals");
    POP_LO("locals");
  }

  fprintf(out, "  ret\n\n");

  return 0;
}

int AVR8::return_void(int local_count)
{
  fprintf(out, "; return_void\n");
  fprintf(out, "  mov SP, locals\n");

  if (!is_main)
  {
    POP_HI("locals");
    POP_LO("locals");
    fprintf(out, "  ret\n\n");
  }
    else
  {
    // stop the cpu when program ends
    fprintf(out, "  break\n");
  }

  return 0;
}

int AVR8::jump(const char *name)
{
  fprintf(out, "  rjmp %s\n", name);

  return 0;
}

int AVR8::call(const char *name)
{
  fprintf(out, "  rcall %s\n", name);

  return 0;
}

int AVR8::invoke_static_method(const char *name, int params, int is_void)
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
      fprintf(out, "  ldi YL, stack_lo + 1 + %d\n", (stack - stack_vars));
      fprintf(out, "  add YL, SP\n");
      fprintf(out, "  ld temp, Y\n");
      fprintf(out, "  ldi YL, stack_lo + 1 %d\n", local - 1);
      fprintf(out, "  add YL, SP\n");
      fprintf(out, "  st Y, temp\n");
      fprintf(out, "  ldi YL, stack_hi + 1 + %d\n", (stack - stack_vars));
      fprintf(out, "  add YL, SP\n");
      fprintf(out, "  ld temp, Y\n");
      fprintf(out, "  ldi YL, stack_hi + 1 %d\n", local - 1);
      fprintf(out, "  add YL, SP\n");
      fprintf(out, "  st Y, temp\n");
      stack_vars--;
    }

    local++;
  }

  fprintf(out, "  rcall %s\n", name);

  if ((stack - stack_vars) > 0)
  {
    fprintf(out, "  ldi temp, %d\n", (stack - stack_vars));
    fprintf(out, "  add SP, temp\n");

    params -= (stack - stack_vars);
  }

  if (!is_void)
  {
    PUSH_LO("result0");
    PUSH_HI("result1");
    stack++;
  }

  return 0;
}

int AVR8::put_static(const char *name, int index)
{
  if (stack > 0)
  {
    POP_HI("temp");
    fprintf(out, "  sts %s + 1, temp\n", name);
    POP_LO("temp");
    fprintf(out, "  sts %s + 0, temp\n", name);
    stack--;
  }

  return 0;
}

int AVR8::get_static(const char *name, int index)
{
  fprintf(out, "  lds temp, %s + 0\n", name);
  PUSH_LO("temp");
  fprintf(out, "  lds temp, %s + 1\n", name);
  PUSH_HI("temp");
  stack++;

  return 0;
}

int AVR8::brk()
{
  return -1;
}

int AVR8::new_array(uint8_t type)
{
  if (stack > 0)
  {
    if (type == TYPE_SHORT || type == TYPE_CHAR || type == TYPE_INT)
    {
      need_array_int_support = 1;
      fprintf(out, "  rcall new_array_int\n");
    }
      else
    {
      need_array_byte_support = 1;
      fprintf(out, "  rcall new_array_byte\n");
    }
  }

  return 0;
}

int AVR8::insert_array(const char *name, int32_t *data, int len, uint8_t type)
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

int AVR8::insert_string(const char *name, uint8_t *bytes, int len)
{
  fprintf(out, ".align 16\n");
  fprintf(out, "dw %d\n", len);

  return insert_utf8(name, bytes, len);
}

int AVR8::push_array_length()
{
  if (stack > 0)
  {
    need_push_array_length = 1;
    fprintf(out, "  rcall push_array_length\n");
  }

  return 0;
}

int AVR8::push_array_length(const char *name, int field_id)
{
  need_push_array_length2 = 1;
  fprintf(out, "  lds XL, %s + 0\n", name);
  fprintf(out, "  lds XH, %s + 1\n", name);
  fprintf(out, "  rcall push_array_length2\n");
  stack++;

  return 0;
}

int AVR8::array_read_byte()
{
  get_values_from_stack(2);
  fprintf(out, "  rcall array_read_byte\n");
  stack++;

  return 0;
}

int AVR8::array_read_short()
{
  return array_read_int();
}

int AVR8::array_read_int()
{
  get_values_from_stack(2);
  fprintf(out, "  rcall array_read_int\n");
  stack++;

  return 0;
}

int AVR8::array_read_byte(const char *name, int field_id)
{
  need_array_byte_support = 1;
  if (stack > 0)
  {
    fprintf(out, "  lds ZL, %s + 0\n", name);
    fprintf(out, "  lds ZH, %s + 1\n", name);
    fprintf(out, "  rcall array_read_byte2\n");
    stack++;
  }

  return 0;
}

int AVR8::array_read_short(const char *name, int field_id)
{
  return array_read_int(name, field_id);
}

int AVR8::array_read_int(const char *name, int field_id)
{
  need_array_int_support = 1;

  if (stack > 0)
  {
    fprintf(out, "  lds ZL, %s + 0\n", name);
    fprintf(out, "  lds ZH, %s + 1\n", name);
    fprintf(out, "  rcall array_read_int2\n");
    stack++;
  }

  return 0;
}

int AVR8::array_write_byte()
{
  get_values_from_stack(3);
  fprintf(out, "  rcall array_write_byte\n");

  return 0;
}

int AVR8::array_write_short()
{
  return array_write_int();
}

int AVR8::array_write_int()
{
  get_values_from_stack(3);
  fprintf(out, "  rcall array_write_int\n");

  return 0;
}

int AVR8::array_write_byte(const char *name, int field_id)
{
  get_values_from_stack(2);
  fprintf(out, "; array_write_byte2\n");

  return 0;
}

int AVR8::array_write_short(const char *name, int field_id)
{
  return array_write_int(name, field_id);
}

int AVR8::array_write_int(const char *name, int field_id)
{
  get_values_from_stack(2);
  fprintf(out, "; array_write_int2\n");

  return 0;
}

int AVR8::get_values_from_stack(int num)
{
  fprintf(out, "; get_values_from_stack, num = %d\n", num);
  if(num > 0)
  {
    POP_HI("value11");
    POP_LO("value10");
    stack--;
  }

  if(num > 1)
  {
    POP_HI("value21");
    POP_LO("value20");
    stack--;
  }

  if(num > 2)
  {
    POP_HI("value31");
    POP_LO("value30");
    stack--;
  }

  return 0;
}

// subroutines
void AVR8::insert_swap()
{
  fprintf(out, "swap:\n");
  fprintf(out, "  ldi YL, stack_lo\n");
  fprintf(out, "  add YL, SP\n");
  fprintf(out, "  ld value10, Y-\n");
  fprintf(out, "  ld value20, Y-\n");
  fprintf(out, "  ldi YL, stack_hi\n");
  fprintf(out, "  add YL, SP\n");
  fprintf(out, "  ld value11, Y-\n");
  fprintf(out, "  ld value21, Y\n");
  fprintf(out, "  mov value30, value20\n");
  fprintf(out, "  mov value31, value21\n");
  fprintf(out, "  mov value20, value10\n");
  fprintf(out, "  mov value21, value11\n");
  fprintf(out, "  mov value10, value30\n");
  fprintf(out, "  mov value11, value31\n");
  fprintf(out, "  st Y+, value21\n");
  fprintf(out, "  st Y, value11\n");
  fprintf(out, "  ldi YL, stack_lo\n");
  fprintf(out, "  add YL, SP\n");
  fprintf(out, "  st Y-, value10\n");
  fprintf(out, "  st Y, value20\n");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_add_integer()
{
  fprintf(out, "add_integer:\n");
  POP_HI("value11");
  POP_LO("value10");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "  add result0, value10\n");
  fprintf(out, "  adc result1, value11\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_sub_integer()
{
  fprintf(out, "sub_integer:\n");
  POP_HI("value11");
  POP_LO("value10");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "  sub result0, value10\n");
  fprintf(out, "  sbc result1, value11\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_mul_integer()
{
  fprintf(out, "mul_integer:\n");
  POP_HI("value21");
  POP_LO("value20");
  POP_HI("value11");
  POP_LO("value10");
  fprintf(out, "  mov result0, zero\n");
  fprintf(out, "  mov result1, zero\n");
  fprintf(out, "  ldi temp, 16\n");
  fprintf(out, "mul_integer_loop:\n");
  fprintf(out, "  lsl result0\n");
  fprintf(out, "  rol result1\n");
  fprintf(out, "  lsl value10\n");
  fprintf(out, "  rol value11\n");
  fprintf(out, "  brcc mul_integer_next\n");
  fprintf(out, "  add result0, value20\n");
  fprintf(out, "  adc result1, value21\n");
  fprintf(out, "mul_integer_next:\n");
  fprintf(out, "  dec temp\n");
  fprintf(out, "  brne mul_integer_loop\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_div_integer()
{
  fprintf(out, "div_integer:\n");
  POP_HI("value11");
  POP_LO("value10");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "  ldi temp, 0\n");
  fprintf(out, "  mov remainder0, temp\n");
  fprintf(out, "  mov remainder1, temp\n");
  fprintf(out, "  ldi temp, 16\n");
  fprintf(out, "div_integer_loop:\n");
  fprintf(out, "  lsl result0\n");
  fprintf(out, "  rol result1\n");
  fprintf(out, "  rol remainder0\n");
  fprintf(out, "  rol remainder1\n");
  fprintf(out, "  cp remainder0, value10\n");
  fprintf(out, "  cpc remainder1, value11\n");
  fprintf(out, "  brcs div_integer_next\n");
  fprintf(out, "  sub remainder0, value10\n");
  fprintf(out, "  sbc remainder1, value11\n");
  fprintf(out, "  inc result0\n");
  fprintf(out, "div_integer_next:\n");
  fprintf(out, "  dec temp\n");
  fprintf(out, "  brne div_integer_loop\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_mod_integer()
{
  fprintf(out, "mod_integer:\n");
  fprintf(out, "  mov result0, remainder0\n");
  PUSH_LO("result0");
  fprintf(out, "  mov result1, remainder1\n");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_neg_integer()
{
  fprintf(out, "neg_integer:\n");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "  ldi temp, 0\n");
  fprintf(out, "  sub temp, result0\n");
  fprintf(out, "  sbc temp, result1\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_shift_left_integer()
{
  fprintf(out, "shift_left_integer:\n");
  POP_HI("temp");
  POP_LO("temp");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "shift_left_integer_loop:\n");
  fprintf(out, "  lsl result0\n");
  fprintf(out, "  rol result1\n");
  fprintf(out, "  dec temp\n");
  fprintf(out, "  brne shift_left_integer_loop\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_shift_right_integer()
{
  fprintf(out, "shift_right_integer:\n");
  POP_HI("temp");
  POP_LO("temp");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "  mov temp2, result1\n");
  fprintf(out, "shift_right_integer_loop:\n");
  fprintf(out, "  lsl temp2\n");
  fprintf(out, "  ror result1\n");
  fprintf(out, "  ror result0\n");
  fprintf(out, "  mov temp2, result0\n");
  fprintf(out, "  dec temp\n");
  fprintf(out, "  brne shift_right_integer_loop\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_shift_right_uinteger()
{
  fprintf(out, "shift_right_uinteger:\n");
  POP_HI("temp");
  POP_LO("temp");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "shift_right_uinteger_loop:\n");
  fprintf(out, "  lsr result1\n");
  fprintf(out, "  ror result0\n");
  fprintf(out, "  dec temp\n");
  fprintf(out, "  brne shift_right_uinteger_loop\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_and_integer()
{
  fprintf(out, "and_integer:\n");
  POP_HI("value11");
  POP_LO("value10");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "  and result0, value10\n");
  fprintf(out, "  and result1, value11\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_or_integer()
{
  fprintf(out, "or_integer:\n");
  POP_HI("value11");
  POP_LO("value10");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "  or result0, value10\n");
  fprintf(out, "  or result1, value11\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_xor_integer()
{
  fprintf(out, "xor_integer:\n");
  POP_HI("value11");
  POP_LO("value10");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "  eor result0, value10\n");
  fprintf(out, "  eor result1, value11\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_inc_integer()
{
  fprintf(out, "inc_integer:\n");
  fprintf(out, "  ldi YL, stack_lo\n");
  fprintf(out, "  sub YL, temp\n");
  fprintf(out, "  add YL, locals\n");
  fprintf(out, "  ld value20, Y\n");
  fprintf(out, "  mov temp2, YL\n");
  fprintf(out, "  ldi YL, stack_hi\n");
  fprintf(out, "  sub YL, temp\n");
  fprintf(out, "  add YL, locals\n");
  fprintf(out, "  ld value21, Y\n");
  fprintf(out, "  add value20, value10\n");
  fprintf(out, "  adc value21, value11\n");
  fprintf(out, "  st Y, value21\n");
  fprintf(out, "  mov YL, temp2\n");
  fprintf(out, "  st Y, value20\n");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_integer_to_byte()
{
  fprintf(out, "integer_to_byte:\n");
  POP_HI("result1");
  POP_LO("result0");
  PUSH_LO("result0");
  fprintf(out, "  cp result0, zero\n");
  fprintf(out, "  brpl integer_to_byte_skip\n");
  fprintf(out, "  mov result1, ff\n");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
  fprintf(out, "integer_to_byte_skip:\n");
  fprintf(out, "  mov result1, zero\n");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_jump_cond()
{
  fprintf(out, "jump_cond:\n");
  fprintf(out, "  inc SP\n");

  // value10
  fprintf(out, "  ldi YL, stack_lo\n");
  fprintf(out, "  add YL, SP\n");
  fprintf(out, "  ld value10, Y\n");
  // value11
  fprintf(out, "  ldi YL, stack_hi\n");
  fprintf(out, "  add YL, SP\n");
  fprintf(out, "  ld value11, Y\n");

  fprintf(out, "  ret\n\n");
}

void AVR8::insert_jump_cond_integer()
{
  fprintf(out, "jump_cond_integer:\n");
  fprintf(out, "  inc SP\n");
  fprintf(out, "  inc SP\n");

  // value10
  fprintf(out, "  ldi YL, stack_lo\n");
  fprintf(out, "  add YL, SP\n");
  fprintf(out, "  ld value10, Y\n");
  // value20
  fprintf(out, "  dec YL\n");
  fprintf(out, "  ld value20, Y\n");
  // value11
  fprintf(out, "  ldi YL, stack_hi\n");
  fprintf(out, "  add YL, SP\n");
  fprintf(out, "  ld value11, Y\n");
  // value21
  fprintf(out, "  dec YL\n");
  fprintf(out, "  ld value21, Y\n");

  fprintf(out, "  ret\n\n");
}

void AVR8::insert_push_integer_local()
{
  fprintf(out, "push_integer_local:\n");
  fprintf(out, "  ldi YL, stack_lo\n");
  fprintf(out, "  sub YL, temp2\n");
  fprintf(out, "  add YL, locals\n");
  fprintf(out, "  ld temp, Y\n");
  PUSH_LO("temp");
  fprintf(out, "  ldi YL, stack_hi\n");
  fprintf(out, "  sub YL, temp2\n");
  fprintf(out, "  add YL, locals\n");
  fprintf(out, "  ld temp, Y\n");
  PUSH_HI("temp");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_pop_integer_local()
{
  fprintf(out, "pop_integer_local:\n");
  POP_HI("temp");
  fprintf(out, "  ldi YL, stack_hi\n");
  fprintf(out, "  sub YL, temp2\n");
  fprintf(out, "  add YL, locals\n");
  fprintf(out, "  st Y, temp\n");
  POP_LO("temp");
  fprintf(out, "  ldi YL, stack_lo\n");
  fprintf(out, "  sub YL, temp2\n");
  fprintf(out, "  add YL, locals\n");
  fprintf(out, "  st Y, temp\n");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_dup()
{
  fprintf(out, "dup:\n");
  fprintf(out, "  mov YL, stack_lo\n");
  fprintf(out, "  add YL, SP\n");
  fprintf(out, "  ld temp, YL\n");
  PUSH_LO("temp");
  fprintf(out, "  mov YL, stack_hi\n");
  fprintf(out, "  add YL, SP\n");
  fprintf(out, "  ld temp, YL\n");
  PUSH_HI("temp");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_push_array_length()
{
  // push_array_length
  fprintf(out, "push_array_length:\n");
  POP_HI("XH");
  POP_LO("XL");
  fprintf(out, "  sub XL, two\n");
  fprintf(out, "  sbc XH, zero\n");
  fprintf(out, "  ld result0, X+\n");
  fprintf(out, "  ld result1, X+\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_push_array_length2()
{
  fprintf(out, "push_array_length2:\n");
  fprintf(out, "  sub XL, two\n");
  fprintf(out, "  sbc XH, zero\n");
  fprintf(out, "  ld result0, X+\n");
  fprintf(out, "  ld result1, X+\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_array_byte_support()
{
  // new_array byte
  fprintf(out, "new_array_byte:\n");
  POP_HI("length1");
  POP_LO("length0");
  fprintf(out, "  lds result0, heap_ptr + 0\n");
  fprintf(out, "  lds result1, heap_ptr + 1\n");
  fprintf(out, "  mov XL, result0\n");
  fprintf(out, "  mov XH, result1\n");
  fprintf(out, "  st X+, length0\n");
  fprintf(out, "  st X+, length1\n");
  fprintf(out, "  add length0, two\n");
  fprintf(out, "  adc length1, zero\n");
  fprintf(out, "  lds temp, heap_ptr + 0\n");
  fprintf(out, "  lds temp2, heap_ptr + 1\n");
  fprintf(out, "  add temp, length0\n");
  fprintf(out, "  adc temp2, length1\n");
  fprintf(out, "  sts heap_ptr + 0, temp\n");
  fprintf(out, "  sts heap_ptr + 1, temp2\n");
  fprintf(out, "  add result0, three\n");
  fprintf(out, "  adc result1, zero\n");
  fprintf(out, "  ldi temp, 254\n");
  fprintf(out, "  and result0, temp\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");

  // array_read_byte
  fprintf(out, "array_read_byte:\n");
  fprintf(out, "  add value20, value10\n");
  fprintf(out, "  adc value21, value11\n");
  fprintf(out, "  mov XL, value20\n");
  fprintf(out, "  mov XH, value21\n");
  fprintf(out, "  ld result0, X\n");
  PUSH_LO("result0");
  fprintf(out, "  cp result0, zero\n");
  fprintf(out, "  brpl array_read_byte_skip\n");
  fprintf(out, "  mov result1, ff\n");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
  fprintf(out, "array_read_byte_skip:\n");
  fprintf(out, "  mov result1, zero\n");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");

  // array_read_byte2
  fprintf(out, "array_read_byte2:\n");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "  add ZL, result0\n");
  fprintf(out, "  adc ZH, result1\n");
  fprintf(out, "  lpm result0, Z\n");
  PUSH_LO("result0");
  fprintf(out, "  cp result0, zero\n");
  fprintf(out, "  brpl array_read_byte2_skip\n");
  fprintf(out, "  mov result1, ff\n");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
  fprintf(out, "array_read_byte2_skip:\n");
  fprintf(out, "  mov result1, zero\n");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");

  // array_write_byte
  fprintf(out, "array_write_byte:\n");
  fprintf(out, "  add value30, value20\n");
  fprintf(out, "  adc value31, value21\n");
  fprintf(out, "  mov XL, value30\n");
  fprintf(out, "  mov XH, value31\n");
  fprintf(out, "  st X, value10\n");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_array_int_support()
{
  // new_array int
  fprintf(out, "new_array_int:\n");
  POP_HI("length1");
  POP_LO("length0");
  fprintf(out, "  lds result0, heap_ptr + 0\n");
  fprintf(out, "  lds result1, heap_ptr + 1\n");
  fprintf(out, "  mov XL, result0\n");
  fprintf(out, "  mov XH, result1\n");
  fprintf(out, "  st X+, length0\n");
  fprintf(out, "  st X+, length1\n");
  fprintf(out, "  lsl length0\n");
  fprintf(out, "  rol length1\n");
  fprintf(out, "  add length0, two\n");
  fprintf(out, "  adc length1, zero\n");
  fprintf(out, "  lds temp, heap_ptr + 0\n");
  fprintf(out, "  lds temp2, heap_ptr + 1\n");
  fprintf(out, "  add temp, length0\n");
  fprintf(out, "  adc temp2, length1\n");
  fprintf(out, "  sts heap_ptr + 0, temp\n");
  fprintf(out, "  sts heap_ptr + 1, temp2\n");
  fprintf(out, "  add result0, three\n");
  fprintf(out, "  adc result1, zero\n");
  fprintf(out, "  ldi temp, 254\n");
  fprintf(out, "  and result0, temp\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");

  // array_read_int
  fprintf(out, "array_read_int:\n");
  fprintf(out, "  lsl value10\n");
  fprintf(out, "  rol value11\n");
  fprintf(out, "  add value20, value10\n");
  fprintf(out, "  adc value21, value11\n");
  fprintf(out, "  mov XL, value20\n");
  fprintf(out, "  mov XH, value21\n");
  fprintf(out, "  ld result0, X+\n");
  fprintf(out, "  ld result1, X+\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");

  // array_read_int2
  fprintf(out, "array_read_int2:\n");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "  lsl result0\n");
  fprintf(out, "  rol result1\n");
  fprintf(out, "  add ZL, result0\n");
  fprintf(out, "  adc ZH, result1\n");
  fprintf(out, "  lpm result0, Z+\n");
  fprintf(out, "  lpm result1, Z+\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");

  // array_write_int
  fprintf(out, "array_write_int:\n");
  fprintf(out, "  lsl value20\n");
  fprintf(out, "  rol value21\n");
  fprintf(out, "  add value30, value20\n");
  fprintf(out, "  adc value31, value21\n");
  fprintf(out, "  mov XL, value30\n");
  fprintf(out, "  mov XH, value31\n");
  fprintf(out, "  st X+, value10\n");
  fprintf(out, "  st X+, value11\n");
  fprintf(out, "  ret\n\n");
}

// Memory API
int AVR8::memory_read8()
{
  fprintf(out, "; memory_read8\n");
  POP_HI("XH");
  POP_LO("XL");
  fprintf(out, "  ld result0, X\n");
  PUSH_LO("result0");
  fprintf(out, "  cp result0, zero\n");
  fprintf(out, "  brpl memory_read8_zero_%d\n", label_count);
  fprintf(out, "  mov result1, ff\n");
  fprintf(out, "  rjmp memory_read8_end_%d\n", label_count);
  fprintf(out, "memory_read8_zero_%d:\n", label_count);
  fprintf(out, "  mov result1, zero\n");
  fprintf(out, "memory_read8_end_%d:\n", label_count++);
  PUSH_HI("result1");

  return 0;
}

int AVR8::memory_write8()
{
  fprintf(out, "; memory_write8\n");
  POP_HI("temp");
  POP_LO("temp");
  POP_HI("XH");
  POP_LO("XL");
  fprintf(out, "  st X, temp\n");

  stack -= 2;

  return 0;
}

// IOPort API
int AVR8::ioport_setPinsAsInput(int port)
{
/*
  if(port == 0)
  {
    fprintf(out, "; ioport_setPinsAsInput\n");
    POP_HI("temp");
    POP_LO("temp");
    fprintf(out, "  in temp2, 0x16\n");
    fprintf(out, "  and temp2, ~temp\n");
    fprintf(out, "  out 0x17, temp2\n");

    stack--;
  }
*/
  return 0;
}

int AVR8::ioport_setPinsAsOutput(int port)
{
  if(port == 0)
  {
    fprintf(out, "; ioport_setPinsAsOutput\n");
    POP_HI("temp");
    POP_LO("temp");
//    fprintf(out, "  in temp2, 0x16\n");
//    fprintf(out, "  or temp2, temp\n");
//    fprintf(out, "  out 0x17, temp2\n");
    fprintf(out, "  out 0x17, temp\n");

    stack--;
  }

  return 0;
}

int AVR8::ioport_setPinsValue(int port)
{
  if(port == 0)
  {
    fprintf(out, "; ioport_setPinsValue\n");
    POP_HI("temp");
    POP_LO("temp");
    fprintf(out, "  out 0x18, temp\n");

    stack--;
  }

  return 0;
}

#if 0
void AVR8::close()
{
}
#endif

