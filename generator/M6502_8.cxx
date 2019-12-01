/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2019 by Michael Kohn, Joe Davisson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "generator/M6502_8.h"

// ABI is:
// A - accumulator
// X - java stack index register
// Y - general-purpose index register

#define LOCALS(a) (a)

M6502_8::M6502_8() :
  stack(0),
  start_org(0xf000),
  java_stack_lo(0x80),
  java_stack_hi(0x98),
  ram_start(0xb0),
  label_count(0),
  is_main(0),

  need_swap(0),
  need_add_integer(0),
  need_sub_integer(0),
  need_neg_integer(0),
  need_shift_left_integer(0),
  need_shift_right_integer(0),
  need_shift_right_uinteger(0),
  need_and_integer(0),
  need_or_integer(0),
  need_xor_integer(0),
  need_push_array_length(0),
  need_array_read_byte(0),
  need_memory_read8(0),
  need_memory_write8(0)
{

}

M6502_8::~M6502_8()
{
}

int M6502_8::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".6502\n");

  // heap
  fprintf(out, "ram_start equ 0x%04x\n", ram_start);
  fprintf(out, "heap_ptr equ ram_start\n");

  // for indirection (2 bytes)
  fprintf(out, "address equ 0xb0\n");

  // java stack
  fprintf(out, "stack_lo equ 0x%04x\n", java_stack_lo);
  fprintf(out, "stack_hi equ 0x%04x\n", java_stack_hi);

  // points to locals
  fprintf(out, "locals equ 0xb2\n");

  // temp variables
  fprintf(out, "result equ 0xb4\n");
  fprintf(out, "length equ 0xb6\n");
  fprintf(out, "value1 equ 0xb8\n");
  fprintf(out, "value2 equ 0xba\n");

  // start
  fprintf(out, ".org 0x%04x\n", start_org);
  fprintf(out, "reset:\n");
  fprintf(out, "  sei\n");
  fprintf(out, "  cld\n");
  fprintf(out, "  lda #0xff\n");
  fprintf(out, "  tax\n");
  fprintf(out, "  txs\n");

  return 0;
}

int M6502_8::finish()
{
  if (need_swap) { insert_swap(); }
  if (need_add_integer) { insert_add_integer(); }
  if (need_sub_integer) { insert_sub_integer(); }
  if (need_neg_integer) { insert_neg_integer(); }
  if (need_shift_left_integer) { insert_shift_left_integer(); }
  if (need_shift_right_integer) { insert_shift_right_integer(); }
  if (need_shift_right_uinteger) { insert_shift_right_uinteger(); }
  if (need_and_integer) { insert_and_integer(); }
  if (need_or_integer) { insert_or_integer(); }
  if (need_xor_integer) { insert_xor_integer(); }
  if (need_push_array_length) { insert_push_array_length(); }
  if (need_array_read_byte) { insert_array_read_byte(); }
  if (need_memory_read8) { insert_memory_read8(); }
  if (need_memory_write8) { insert_memory_write8(); }

  return 0;
}

int M6502_8::start_init()
{
  return 0;
}

int M6502_8::insert_static_field_define(std::string &name, std::string &type, int index)
{
  // do nothing, using equ instead of heap to save RAM
  return 0;
}

int M6502_8::init_heap(int field_count)
{
  // do nothing, using equ instead of heap to save RAM
  return 0;
}

#if 0
int M6502_8::insert_field_init_boolean(char *name, int index, int value)
{
  value = (value == 0) ? 0 : 1;
  fprintf(out, "; insert_field_init_boolean\n");
  fprintf(out, "%s equ _%s\n", name, name);

  return 0;
}

int M6502_8::insert_field_init_byte(char *name, int index, int value)
{
  return insert_field_init_int(name, index, value);
}

int M6502_8::insert_field_init_short(char *name, int index, int value)
{
  return insert_field_init_int(name, index, value);
}
#endif

int M6502_8::field_init_int(std::string &name, int index, int value)
{
  if (value < -32768 || value > 65535) { return -1; }

  fprintf(out, "; field_init_short\n");
  fprintf(out, "%s equ _%s\n", name.c_str(), name.c_str());

  return 0;
}

int M6502_8::field_init_ref(std::string &name, int index)
{
  fprintf(out, "; field_init_ref\n");
  fprintf(out, "%s equ _%s\n", name.c_str(), name.c_str());

  return 0;
}

void M6502_8::method_start(int local_count, int max_stack, int param_count, std::string &name)
{
  stack = 0;

  is_main = (name == "main") ? 1 : 0;

  fprintf(out, "%s:\n", name.c_str());

  // main() function goes here
  if (!is_main)
  {
    fprintf(out, "  lda locals\n");
    PUSH_LO();
    fprintf(out, "  lda #0\n");
    PUSH_HI();
  }

  fprintf(out, "  stx locals\n");
  fprintf(out, "  txa\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #0x%02x\n", local_count);
  fprintf(out, "  tax\n");
}

void M6502_8::method_end(int local_count)
{
  fprintf(out, "\n");
}

int M6502_8::push_local_var_int(int index)
{
  fprintf(out, "; push_local_var_int\n");
  fprintf(out, "  ldy locals\n");
  fprintf(out, "  lda stack_lo - %d,y\n", LOCALS(index));
  PUSH_LO();
  fprintf(out, "  lda stack_hi - %d,y\n", LOCALS(index));
  PUSH_HI();
  stack++;

  return 0;
}

int M6502_8::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int M6502_8::push_ref_static(std::string &name, int index)
{
  printf("push_ref_static not supported.\n");
  return -1;
}

int M6502_8::push_fake()
{
  printf("push_fake not supported.\n");
  return -1;
}

int M6502_8::push_int(int32_t n)
{
  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);

    return -1;
  }

  uint16_t value = (n & 0xffff);
  uint16_t lo = value & 0xff;
  uint16_t hi = value >> 8;

  fprintf(out, "; push_int\n");
  fprintf(out, "  lda #0x%02x\n", lo);
  PUSH_LO();
  if (hi != lo)
  {
    fprintf(out, "  lda #0x%02x\n", hi);
  }
  PUSH_HI();
  stack++;

  return 0;
}

int M6502_8::push_long(int64_t n)
{
  return push_int((int32_t)n);
}

#if 0
int M6502_8::push_float(float f)
{
  return -1;
}
int M6502_8::push_double(double f)
{
  return -1;
}
#endif

#if 0
int M6502_8::push_byte(int8_t b)
{
  int8_t n = b;
  uint8_t value = (n & 0xff);

  fprintf(out, "; push_byte\n");
  fprintf(out, "  lda #0x%02x\n", value & 0xff);
  PUSH_LO();
  PUSH_HI();
  stack++;

  return 0;
}

int M6502_8::push_short(int16_t s)
{
  uint8_t value = (s & 0xff);

  fprintf(out, "; push_short\n");
  fprintf(out, "  lda #0x%02x\n", value & 0xff);
  PUSH_LO();
  PUSH_HI();
  stack++;

  return 0;
}
#endif

int M6502_8::push_ref(std::string &name)
{
  fprintf(out, "; push_ref\n");
  fprintf(out, "  lda #(%s & 0xff)\n", name.c_str());
  PUSH_LO();
  fprintf(out, "  lda #(%s >> 8)\n", name.c_str());
  PUSH_HI();
//  fprintf(out, "  lda %s + 0\n", name.c_str());
//  PUSH_LO();
//  fprintf(out, "  lda %s + 1\n", name.c_str());
//  PUSH_HI();
  stack++;

  return 0;
}

int M6502_8::pop_local_var_int(int index)
{
  fprintf(out, "; pop_local_var_int\n");
  fprintf(out, "  ldy locals\n");
  POP_HI();
  fprintf(out, "  sta stack_hi - %d,y\n", LOCALS(index));
  POP_LO();
  fprintf(out, "  sta stack_lo - %d,y\n", LOCALS(index));
  stack--;

  return 0;
}

int M6502_8::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int M6502_8::pop()
{
  fprintf(out, "; pop\n");
  POP();
  stack--;

  return 0;
}

int M6502_8::dup()
{
  fprintf(out, "; dup()\n");
  fprintf(out, "  lda stack_lo+1,x\n");
  fprintf(out, "  sta stack_lo+0,x\n");
  fprintf(out, "  lda stack_hi+1,x\n");
  fprintf(out, "  sta stack_hi+0,x\n");
  fprintf(out, "  dex\n");
  stack++;

  return 0;
}

int M6502_8::dup2()
{
  printf("dup2 not supported.\n");
  return -1;
}

int M6502_8::swap()
{
  need_swap = 1;
  fprintf(out, "  jsr swap\n");

  return 0;
}

int M6502_8::add_integer()
{
  need_add_integer = 1;
  fprintf(out, "  jsr add_integer\n");
  stack--;

  return 0;
}

int M6502_8::add_integer(int num)
{
#if 0
  if (num == 1)
  {
    fprintf(out, "; add_integer num = %d\n", num);
    fprintf(out, "  inc stack_lo + 1,x\n");
    fprintf(out, "  bne add_integer_%d\n", label_count);
    fprintf(out, "  inc stack_hi + 1,x\n");
    fprintf(out, "add_integer_%d:\n", label_count);

    label_count++;

    return 0;
  }
#endif

  return -1;
}

int M6502_8::sub_integer()
{
  need_sub_integer = 1;
  fprintf(out, "  jsr sub_integer\n");
  stack--;

  return 0;
}

int M6502_8::sub_integer(int num)
{
  return -1;
}

int M6502_8::mul_integer()
{
  return -1;
}

int M6502_8::div_integer()
{
  return -1;
}

int M6502_8::mod_integer()
{
  return -1;
}

int M6502_8::neg_integer()
{
  return -1;
}

int M6502_8::shift_left_integer()
{
  need_shift_left_integer = 1;
  fprintf(out, "  jsr shift_left_integer\n");
  stack--;

  return 0;
}

int M6502_8::shift_left_integer(int num)
{
#if 0
  // This should work, but probably needs some testing.
  if (num == 1)
  {
    fprintf(out, "; shift_left_integer num = %d\n", num);
    fprintf(out, "  asl stack_lo + 1,x\n");
    fprintf(out, "  rol stack_hi + 1,x\n");

    return 0;
  }
#endif

  return -1;
}

int M6502_8::shift_right_integer()
{
  need_shift_right_integer = 1;
  fprintf(out, "  jsr shift_right_integer\n");
  stack--;

  return 0;
}

int M6502_8::shift_right_integer(int num)
{
  return -1;
}

int M6502_8::shift_right_uinteger()
{
  need_shift_right_uinteger = 1;
  fprintf(out, "  jsr shift_right_uinteger\n");
  stack--;

  return 0;
}

int M6502_8::shift_right_uinteger(int num)
{
  return -1;
}

int M6502_8::and_integer()
{
  need_and_integer = 1;
  fprintf(out, "  jsr and_integer\n");
  stack--;

  return 0;
}

int M6502_8::and_integer(int num)
{
  int lo = num & 0xff;
  int hi = (num >> 8) & 0xff;

  fprintf(out, "; and_integer num = 0x%02x\n", num);

  if (lo != 0xff)
  {
    fprintf(out, "  lda #0x%02x\n", lo);
    fprintf(out, "  and stack_lo + 1,x\n");
    fprintf(out, "  sta stack_lo + 1,x\n");
  }

  if (hi == 0x00)
  {
    fprintf(out, "  lda #0x%02x\n", hi);
    fprintf(out, "  sta stack_hi + 1,x\n");
  }
    else
  if (hi != 0xff)
  {
    fprintf(out, "  lda #0x%02x\n", hi);
    fprintf(out, "  and stack_hi + 1,x\n");
    fprintf(out, "  sta stack_hi + 1,x\n");
  }

  return 0;
  //return -1;
}

int M6502_8::or_integer()
{
  need_or_integer = 1;
  fprintf(out, "  jsr or_integer\n");
  stack--;

  return 0;
}

int M6502_8::or_integer(int num)
{
#if 0
  // This should work, but needs some testing.  This could also be optimized
  // further if hi or lo are 0xff.
  int lo = num & 0xff;
  int hi = (num >> 8) & 0xff;

  fprintf(out, "; or_integer num = %d\n", num);

  if (lo != 0)
  {
    fprintf(out, "  lda #0x%02x\n", lo);
    fprintf(out, "  or stack_lo + 1,x\n");
    fprintf(out, "  sta stack_lo + 1,x\n");
  }

  if (hi != 0)
  {
    fprintf(out, "  lda #0x%02x\n", hi);
    fprintf(out, "  or stack_hi + 1,x\n");
    fprintf(out, "  sta stack_hi + 1,x\n");
  }

  return 0;
#endif

  return -1;
}

int M6502_8::xor_integer()
{
  need_xor_integer = 1;
  fprintf(out, "  jsr xor_integer\n");
  stack--;

  return 0;
}

int M6502_8::xor_integer(int num)
{
  return -1;
}

int M6502_8::inc_integer(int index, int num)
{
  uint8_t value = num & 0xff;

  fprintf(out, "; inc_integer num = %d\n", num);
  fprintf(out, "  ldy locals\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda stack_lo - %d,y\n", LOCALS(index));
  fprintf(out, "  adc #0x%02x\n", value & 0xff);
  fprintf(out, "  sta stack_lo - %d,y\n", LOCALS(index));

  return 0;
}

int M6502_8::integer_to_byte()
{
  return 0;
}

int M6502_8::integer_to_short()
{
  return 0;
}

int M6502_8::jump_cond(std::string &label, int cond, int distance)
{
  bool reverse = false;

  if (stack > 0)
  {
    fprintf(out, "; jump_cond\n");
    fprintf(out, "  inx\n");

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
        fprintf(out, "  bne #3\n");
        fprintf(out, "  jmp %s\n", label.c_str());
        break;
      case COND_NOT_EQUAL:
        fprintf(out, "  lda stack_lo - 0,x\n");
        fprintf(out, "  beq #3\n");
        fprintf(out, "  jmp %s\n", label.c_str());
        break;
      case COND_LESS:
        if(reverse == false)
        {
          fprintf(out, "  lda stack_lo - 0,x\n");
          fprintf(out, "  bpl #3\n");
          fprintf(out, "  jmp %s\n", label.c_str());
        }
          else
        {
          fprintf(out, "  lda #0\n");
          fprintf(out, "  cmp stack_lo - 0,x\n");
          fprintf(out, "  bpl #3\n");
          fprintf(out, "  jmp %s\n", label.c_str());
        }
        break;
      case COND_GREATER_EQUAL:
        if(reverse == false)
        {
          fprintf(out, "  lda stack_lo - 0,x\n");
          fprintf(out, "  bmi #3\n");
          fprintf(out, "  jmp %s\n", label.c_str());
        }
          else
        {
          fprintf(out, "  lda #0\n");
          fprintf(out, "  cmp stack_lo - 0,x\n");
          fprintf(out, "  bmi #3\n");
          fprintf(out, "  jmp %s\n", label.c_str());
        }
        break;
    }

    stack--;
  }

  return 0;
}

int M6502_8::jump_cond_integer(std::string &label, int cond, int distance)
{
  bool reverse = false;

  if (stack > 1)
  {
    fprintf(out, "; jump_cond_integer\n");
    fprintf(out, "  inx\n");
    fprintf(out, "  inx\n");

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
        fprintf(out, "  bne #3\n");
        fprintf(out, "  jmp %s\n", label.c_str());
        break;
      case COND_NOT_EQUAL:
        fprintf(out, "  lda stack_lo - 0,x\n");
        fprintf(out, "  cmp stack_lo - 1,x\n");
        fprintf(out, "  beq #3\n");
        fprintf(out, "  jmp %s\n", label.c_str());
        break;
      case COND_LESS:
        if(reverse == false)
        {
          fprintf(out, "  lda stack_lo - 0,x\n");
          fprintf(out, "  cmp stack_lo - 1,x\n");
          fprintf(out, "  bpl #3\n");
          fprintf(out, "  jmp %s\n", label.c_str());
        }
          else
        {
          fprintf(out, "  lda stack_lo - 1,x\n");
          fprintf(out, "  cmp stack_lo - 0,x\n");
          fprintf(out, "  bpl #3\n");
          fprintf(out, "  jmp %s\n", label.c_str());
        }
        break;
      case COND_GREATER_EQUAL:
        if(reverse == false)
        {
          fprintf(out, "  lda stack_lo - 0,x\n");
          fprintf(out, "  cmp stack_lo - 1,x\n");
          fprintf(out, "  bmi #3\n");
          fprintf(out, "  jmp %s\n", label.c_str());
        }
          else
        {
          fprintf(out, "  lda stack_lo - 1,x\n");
          fprintf(out, "  cmp stack_lo - 0,x\n");
          fprintf(out, "  bmi #3\n");
          fprintf(out, "  jmp %s\n", label.c_str());
        }
        break;
    }

    stack -= 2;
  }

  return 0;
}

int M6502_8::ternary(int cond, int value_true, int value_false)
{
  return -1;
}

int M6502_8::ternary(int cond, int compare, int value_true, int value_false)
{
  return -1;
}

int M6502_8::return_local(int index, int local_count)
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
    POP();
    fprintf(out, "  sta locals\n");
  }

  fprintf(out, "  rts\n");

  return 0;
}

int M6502_8::return_integer(int local_count)
{
  fprintf(out, "; return_integer\n");
  POP_HI();
  fprintf(out, "  sta result + 1\n");
  POP_LO();
  fprintf(out, "  sta result + 0\n");
  stack--;

  fprintf(out, "  ldx locals\n");

  if (!is_main)
  {
    POP();
    fprintf(out, "  sta locals\n");
  }

  fprintf(out, "  rts\n");

  return 0;
}

int M6502_8::return_void(int local_count)
{
  fprintf(out, "; return_void\n");
  fprintf(out, "  ldx locals\n");

  if (!is_main)
  {
    POP();
    fprintf(out, "  sta locals\n");
  }

  fprintf(out, "  rts\n");

  return 0;
}

int M6502_8::jump(std::string &name, int distance)
{
  fprintf(out, "  jmp %s\n", name.c_str());

  return 0;
}

int M6502_8::call(std::string &name)
{
  fprintf(out, "  jsr %s\n", name.c_str());

  return 0;
}

int M6502_8::invoke_static_method(const char *name, int params, int is_void)
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
    fprintf(out, "  txa\n");
    fprintf(out, "  clc\n");
    fprintf(out, "  adc #%d\n", (stack - stack_vars));
    fprintf(out, "  tax\n");

    params -= (stack - stack_vars);
  }

  if (!is_void)
  {
    fprintf(out, "  lda result + 0\n");
    PUSH_LO();
    fprintf(out, "  lda result + 1\n");
    PUSH_HI();
    stack++;
  }

  return 0;
}

int M6502_8::put_static(std::string &name, int index)
{
  if (stack > 0)
  {
    POP_HI();
    fprintf(out, "  sta %s + 1\n", name.c_str());
    POP_LO();
    fprintf(out, "  sta %s + 0\n", name.c_str());
    stack--;
  }

  return 0;
}

int M6502_8::get_static(std::string &name, int index)
{
  if (stack > 0)
  {
    POP_HI();
    fprintf(out, "  sta %s + 1\n", name.c_str());
    POP_LO();
    fprintf(out, "  sta %s + 0\n", name.c_str());
    stack--;
  }

  return 0;
}

int M6502_8::brk()
{
  printf("brk not supported.\n");
  return -1;
}

int M6502_8::new_array(uint8_t type)
{
  printf("new_array not supported.\n");
  return -1;
}

int M6502_8::insert_array(std::string &name, int32_t *data, int len, uint8_t type)
{
  fprintf(out, "; insert_array\n");

  if (type == TYPE_BYTE)
  {
    //fprintf(out, ".align 16\n");
    return insert_db(name, data, len, TYPE_SHORT);
  }
    else
  if (type == TYPE_SHORT)
  {
    //fprintf(out, ".align 16\n");
    return insert_dw(name, data, len, TYPE_SHORT);
  }
    else
  if (type == TYPE_INT)
  {
    //fprintf(out, ".align 16\n");
    return insert_dw(name, data, len, TYPE_SHORT);
  }

  return -1;
}

int M6502_8::insert_string(std::string &name, uint8_t *bytes, int len)
{
  printf("insert_string not supported.\n");
  return -1;
}

int M6502_8::push_array_length()
{
  need_push_array_length = 1;
  fprintf(out, "  jsr push_array_length\n");

  return 0;
}

int M6502_8::push_array_length(std::string &name, int field_id)
{
  return -1;
}

int M6502_8::array_read_byte()
{
  need_array_read_byte = 1;
  fprintf(out, "  jsr array_read_byte\n");
  stack++;

  return 0;
}

int M6502_8::array_read_short()
{
  printf("array_read_short not supported.\n");
  return -1;
}

int M6502_8::array_read_int()
{
  return array_read_byte();
}

int M6502_8::array_read_byte(std::string &name, int field_id)
{
  return -1;
}

int M6502_8::array_read_short(std::string &name, int field_id)
{
  return -1;
}

int M6502_8::array_read_int(std::string &name, int field_id)
{
  return -1;
}

int M6502_8::array_write_byte()
{
  return -1;
}

int M6502_8::array_write_short()
{
  return -1;
}

int M6502_8::array_write_int()
{
  return -1;
}

int M6502_8::array_write_byte(std::string &name, int field_id)
{
  return -1;
}

int M6502_8::array_write_short(std::string &name, int field_id)
{
  return -1;
}

int M6502_8::array_write_int(std::string &name, int field_id)
{
  return -1;
}

// Memory API
int M6502_8::memory_read8_I()
{
  need_memory_read8 = 1;
  fprintf(out, "  jsr memory_read8\n");

  return 0;
}

int M6502_8::memory_read8_I(int address)
{
  fprintf(out, "  lda 0x%04x\n", address);
  PUSH();
  stack++;

  return 0;
}

int M6502_8::memory_write8_IB()
{
  need_memory_write8 = 1;
  fprintf(out, "  jsr memory_write8\n");
  stack -= 2;

  return 0;
}

void M6502_8::insert_swap()
{
  fprintf(out, "swap:\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta result + 0,x\n");

  fprintf(out, "  lda stack_lo - 1,x\n");
  fprintf(out, "  sta stack_lo - 0,x\n");

  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  sta stack_lo - 1,x\n");
  fprintf(out, "  rts\n\n");
}

void M6502_8::insert_add_integer()
{
  fprintf(out, "add_integer:\n");
  POP();
  fprintf(out, "  sta result + 0\n");
  POP();
  fprintf(out, "  clc\n");
  fprintf(out, "  adc result + 0\n");
  PUSH();
  fprintf(out, "  rts\n\n");
}

void M6502_8::insert_sub_integer()
{
  fprintf(out, "sub_integer:\n");
  POP();
  fprintf(out, "  sta result + 0\n");
  POP();
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc result + 0\n");
  PUSH();
  fprintf(out, "  rts\n\n");
}

void M6502_8::insert_neg_integer()
{
}

void M6502_8::insert_shift_left_integer()
{
  fprintf(out, "shift_left_integer:\n");
  POP();
  fprintf(out, "  tay\n");
  POP();
  fprintf(out, "  asl\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne #-4\n");
  PUSH();
  fprintf(out, "  rts\n\n");
}

void M6502_8::insert_shift_right_integer()
{
  fprintf(out, "shift_right_integer:\n");
  POP();
  fprintf(out, "  tay\n");
  POP();
  fprintf(out, "  cmp #0x80\n");
  fprintf(out, "  ror\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne #-6\n");
  PUSH();
  fprintf(out, "  rts\n\n");
}

void M6502_8::insert_shift_right_uinteger()
{
  fprintf(out, "shift_right_uinteger:\n");
  POP();
  fprintf(out, "  tay\n");
  POP();
  fprintf(out, "  lsr\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne #-4\n");
  PUSH();
  fprintf(out, "  rts\n\n");
}

void M6502_8::insert_and_integer()
{
  fprintf(out, "and_integer:\n");
  POP();
  fprintf(out, "  sta result + 0\n");
  POP();
  fprintf(out, "  and result + 0\n");
  PUSH();
  fprintf(out, "  rts\n\n");
}

void M6502_8::insert_or_integer()
{
  fprintf(out, "or_integer:\n");
  POP();
  fprintf(out, "  sta result + 0\n");
  POP();
  fprintf(out, "  ora result + 0\n");
  PUSH();
  fprintf(out, "  rts\n\n");
}

void M6502_8::insert_xor_integer()
{
  fprintf(out, "xor_integer:\n");
  POP();
  fprintf(out, "  sta result + 0\n");
  POP();
  fprintf(out, "  eor result + 0\n");
  PUSH();
  fprintf(out, "  rts\n\n");
}

void M6502_8::insert_push_array_length()
{
  fprintf(out, "push_array_length:\n");
  POP_HI();
  fprintf(out, "  sta result + 1\n");
  POP_LO();
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
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta result + 1\n");

  fprintf(out, "  lda result + 0\n");
  PUSH_LO();
  fprintf(out, "  lda result + 1\n");
  PUSH_HI();
  fprintf(out, "  rts\n\n");
}

void M6502_8::insert_array_read_byte()
{
  fprintf(out, "array_read_byte:\n");
  POP_HI();
  fprintf(out, "  sta value1 + 1\n");
  POP_LO();
  fprintf(out, "  sta value1 + 0\n");
  POP_HI();
  fprintf(out, "  sta value2 + 1\n");
  POP_LO();
  fprintf(out, "  sta value2 + 0\n");
  
  fprintf(out, "  clc\n");
  fprintf(out, "  lda value1 + 0\n");
  fprintf(out, "  adc value2 + 0\n");
  fprintf(out, "  sta value2 + 0\n");
  fprintf(out, "  lda value1 + 1\n");
  fprintf(out, "  adc value2 + 1\n");
  fprintf(out, "  sta value2 + 1\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda (value2),y\n");
  PUSH();
  fprintf(out, "  rts\n\n");
}

void M6502_8::insert_memory_read8()
{
  fprintf(out, "memory_read8:\n");
  POP_HI();
  fprintf(out, "  sta address + 1\n");
  POP_LO();
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda (address),y\n");
  PUSH();
  fprintf(out, "  rts\n\n");
}

void M6502_8::insert_memory_write8()
{
  fprintf(out, "memory_write8:\n");
  POP();
  fprintf(out, "  sta result\n");
  POP_HI();
  fprintf(out, "  sta address + 1\n");
  POP_LO();
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda result\n");
  fprintf(out, "  sta (address),y\n");
  fprintf(out, "  rts\n\n");
}

