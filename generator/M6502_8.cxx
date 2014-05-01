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

#include "M6502_8.h"

// ABI is:
// A
// X
// Y

// java stack from 0x80-0x9f
// vars from 0xa0-0xaf
// heap from 0xb0-0xff

#define PUSH \
  fprintf(out, "; PUSH\n"); \
  fprintf(out, "  ldy SP\n"); \
  fprintf(out, "  sta stack,y\n"); \
  fprintf(out, "  dec SP\n")

#define POP \
  fprintf(out, "; POP\n"); \
  fprintf(out, "  inc SP\n"); \
  fprintf(out, "  ldy SP\n"); \
  fprintf(out, "  lda stack,y\n")

#define LOCALS(a) (a)

M6502_8::M6502_8() :
  stack(0),
  is_main(0),
{

}

M6502_8::~M6502_8()
{
}

int M6502_8::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".65xx\n");

  return 0;
}

int M6502_8::start_init()
{
  fprintf(out, "stack equ 0x80\n");
  fprintf(out, "locals equ 0xa0\n");
  fprintf(out, "SP equ 0xa1\n");
  fprintf(out, "result equ 0xa2\n");
  fprintf(out, "length equ 0xa3\n");
  fprintf(out, "value1 equ 0xa4\n");
  fprintf(out, "value2 equ 0xa5\n");
  fprintf(out, "value3 equ 0xa6\n");
  fprintf(out, "ram_start equ 0xb0\n");
  fprintf(out, "\n");
  fprintf(out, "sei\n");
  fprintf(out, "cld\n");
  fprintf(out, "lda #0x9f\n");
  fprintf(out, "sta SP\n");
  fprintf(out, "lda #0xff\n");
  fprintf(out, "tax\n");
  fprintf(out, "txs\n");

  return 0;
}

int M6502_8::insert_static_field_define(const char *name, const char *type, int index)
{
  fprintf(out, "%s equ ram_start + %d\n", name, (index + 1) * 2);

  return 0;
}

int M6502_8::init_heap(int field_count)
{
  fprintf(out, "  ; Set up heap and static initializers\n");
  fprintf(out, "  lda #(ram_start + %d) & 0xff\n", (field_count + 1) * 2);
  fprintf(out, "  sta ram_start + 0\n");
  fprintf(out, "  lda #(ram_start + %d) >> 8\n", (field_count + 1) * 2);
  fprintf(out, "  sta ram_start + 1\n");

  return 0;
}

int M6502_8::insert_field_init_boolean(char *name, int index, int value)
{
  value = (value == 0) ? 0 : 1;
  fprintf(out, "; insert_field_init_boolean\n");
  fprintf(out, "  lda #%d\n", value & 0xff);
  fprintf(out, "  sta %s + 0\n", name);
  fprintf(out, "  lda #%d\n", value >> 8);
  fprintf(out, "  sta %s + 1\n", name);

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

int M6502_8::insert_field_init_int(char *name, int index, int value)
{
  if (value < -128 || value > 255) { return -1; }
  int16_t n = value;
  uint16_t v = (n & 0xffff);

  fprintf(out, "; insert_field_init_int\n");
  fprintf(out, "  lda #%d\n", (uint8_t)v & 0xff);
  fprintf(out, "  sta %s + 0\n", name);
  fprintf(out, "  lda #%d\n", (uint8_t)v >> 8);
  fprintf(out, "  sta %s + 1\n", name);

  return 0;
}

int M6502_8::insert_field_init(char *name, int index)
{
  fprintf(out, "; insert_field_init\n");
  fprintf(out, "  lda #_%s & 0xff\n", name);
  fprintf(out, "  sta %s + 0\n", name);
  fprintf(out, "  lda #_%s >> 8\n", name);
  fprintf(out, "  sta %s + 1\n", name);

  return 0;
}

void M6502_8::method_start(int local_count, int max_stack, int param_count, const char *name)
{
  stack = 0;

  is_main = (strcmp(name, "main") == 0) ? 1 : 0;

  fprintf(out, "%s:\n", name);

  // main() function goes here
  if (!is_main)
  {
    fprintf(out, "  lda locals\n");
    PUSH;
  }

  fprintf(out, "  lda SP\n");
  fprintf(out, "  sta locals\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #0x%02x\n", local_count);
  fprintf(out, "  sta SP\n");
}

void M6502_8::method_end(int local_count)
{
  fprintf(out, "\n");
}

int M6502_8::push_integer(int32_t n)
{
  int16_t n = b;
  uint16_t value = (n & 0xffff);

  fprintf(out, "; push_byte\n");
  fprintf(out, "  lda #0x%02x\n", value & 0xff);
  PUSH;
  stack++;

  return 0;
}

int M6502_8::push_integer_local(int index)
{
  fprintf(out, "; push_integer_local\n");
  fprintf(out, "  ldx locals\n");
  fprintf(out, "  lda stack - %d,x\n", LOCALS(index));
  PUSH;
  stack++;

  return 0;
}

int M6502_8::push_ref_local(int index)
{
  return push_integer_local(index);
}

int M6502_8::push_fake()
{
  return -1;
}

int M6502_8::push_long(int64_t n)
{
  return push_integer((int32_t)n);
}

int M6502_8::push_float(float f)
{
  printf("float is not supported right now\n");

  return -1;
}

int M6502_8::push_double(double f)
{
  printf("double is not supported right now\n");

  return -1;
}

int M6502_8::push_byte(int8_t b)
{
  return push_integer(b);
}

int M6502_8::push_short(int16_t s)
{
  return push_integer(b);
}

int M6502_8::pop_integer_local(int index)
{
  fprintf(out, "; pop_integer_local\n");
  fprintf(out, "  ldx locals\n");
  POP;
  fprintf(out, "  sta stack - %d,x\n", LOCALS(index));
  stack--;

  return 0;
}

int M6502_8::pop_ref_local(int index)
{
  return pop_integer_local(index);
}

int M6502_8::pop()
{
  fprintf(out, "; pop\n");
  POP;
  fprintf(out, "  sta result\n");
  stack--;

  return 0;
}

int M6502_8::dup()
{
  fprintf(out, "  ldx SP\n");
  fprintf(out, "  lda stack,x\n");
  PUSH;
  stack++;

  return 0;
}

int M6502_8::dup2()
{
  printf("Need to implement dup2()\n");

  return -1;
}

int M6502_8::swap()
{
  fprintf(out, "; swap\n");

  return 0;
}

int M6502_8::add_integer()
{
  fprintf(out, "; add_integer\n");
  stack--;

  return 0;
}

int M6502_8::add_integer(int const_val)
{
  return -1;
}

int M6502_8::sub_integer()
{
  fprintf(out, "; sub_integer\n");
  stack--;

  return 0;
}

int M6502_8::sub_integer(int const_val)
{
  return -1;
}

int M6502_8::mul_integer()
{
  return -1;
}

int M6502_8::mul_integer(int const_val)
{
  return -1;
}

// unsigned only for now
int M6502_8::div_integer()
{
  return -1;
}

int M6502_8::div_integer(int const_val)
{
  return -1;
}

// unsigned only for now
int M6502_8::mod_integer()
{
  return -1;
}

int M6502_8::mod_integer(int const_val)
{
  return -1;
}

int M6502_8::neg_integer()
{
  fprintf(out, "; neg_integer\n");

  return 0;
}

int M6502_8::shift_left_integer()
{
  fprintf(out, "; shift_left_integer\n");
  stack--;

  return 0;
}

int M6502_8::shift_left_integer(int const_val)
{
  return -1;
}

int M6502_8::shift_right_integer()
{
  fprintf(out, "; shift_right_integer\n");
  stack--;

  return 0;
}

int M6502_8::shift_right_integer(int const_val)
{
  return -1;
}

int M6502_8::shift_right_uinteger()
{
  fprintf(out, "; shift_right_uinteger\n");
  stack--;

  return 0;
}

int M6502_8::shift_right_uinteger(int const_val)
{
  return -1;
}

int M6502_8::and_integer()
{
  fprintf(out, "; and_integer\n");
  stack--;

  return 0;
}

int M6502_8::and_integer(int const_val)
{
  return -1;
}

int M6502_8::or_integer()
{
  fprintf(out, "; or_integer\n");
  stack--;

  return 0;
}

int M6502_8::or_integer(int const_val)
{
  return -1;
}

int M6502_8::xor_integer()
{
  fprintf(out, "; xor_integer\n");
  stack--;

  return 0;
}

int M6502_8::xor_integer(int const_val)
{
  return -1;
}

int M6502_8::inc_integer(int index, int num)
{
  uint16_t value = num & 0xffff;

  fprintf(out, "; inc_integer num = %d\n", num);
  fprintf(out, "  ldx locals\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda stack - %d,x\n", LOCALS(index));
  fprintf(out, "  adc #0x%02x\n", value & 0xff);
  fprintf(out, "  sta stack - %d,x\n", LOCALS(index));

  return 0;
}

int M6502_8::integer_to_byte()
{
  fprintf(out, "; integer_to_byte\n");

  return 0;
}

int M6502_8::jump_cond(const char *label, int cond)
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
        fprintf(out, "  lda stack,x\n");
        fprintf(out, "  cmp #0\n");
        fprintf(out, "  bne %s\n", label);
        break;
      case COND_NOT_EQUAL:
        break;
      case COND_LESS:
        if(reverse == false)
        {
        }
          else
        {
        }
        break;
      case COND_GREATER_EQUAL:
        if(reverse == false)
        {
        }
          else
        {
        }
        break;
    }

    stack--;
  }

  return 0;
}

int M6502_8::jump_cond_integer(const char *label, int cond)
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
        fprintf(out, "  lda stack - 0,x\n");
        fprintf(out, "  cmp stack - 1,x\n");
        fprintf(out, "  bne %s\n", label);
        break;
      case COND_NOT_EQUAL:
        break;
      case COND_LESS:
        if(reverse == false)
        {
        }
          else
        {
        }
        break;
      case COND_GREATER_EQUAL:
        if(reverse == false)
        {
        }
          else
        {
        }
        break;
    }

    stack -= 2;
  }

  return 0;
}

int M6502_8::return_local(int index, int local_count)
{
  fprintf(out, "; return_local\n");
  fprintf(out, "  ldx locals\n");
  fprintf(out, "  lda stack - %d,x\n", LOCALS(index));
  fprintf(out, "  sta result\n");

  fprintf(out, "  lda locals\n");
  fprintf(out, "  sta SP\n");

  if (!is_main)
  {
    POP;
    fprintf(out, "  sta locals\n");
  }

  fprintf(out, "  rts\n");

  return 0;
}

int M6502_8::return_integer(int local_count)
{
  fprintf(out, "; return_integer\n");
  POP;
  fprintf(out, "  sta result\n");
  stack--;

  fprintf(out, "  lda locals\n");
  fprintf(out, "  sta SP\n");

  if (!is_main)
  {
    POP;
    fprintf(out, "  sta locals\n");
  }

  fprintf(out, "  rts\n");

  return 0;
}

int M6502_8::return_void(int local_count)
{
  fprintf(out, "; return_void\n");
  fprintf(out, "  lda locals\n");
  fprintf(out, "  sta SP\n");

  if (!is_main)
  {
    POP;
    fprintf(out, "  sta locals\n");
  }

  fprintf(out, "  rts\n");

  return 0;
}

int M6502_8::jump(const char *name)
{
  fprintf(out, "  jmp %s\n", name);

  return 0;
}

int M6502_8::call(const char *name)
{
  fprintf(out, "  jsr %s\n", name);

  return 0;
}

int M6502_8::invoke_static_method(const char *name, int params, int is_void)
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
      fprintf(out, "  lda stack + 1 + %d,x\n", (stack - stack_vars));
      fprintf(out, "  sta stack + 1 %d,x\n", local-1);
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
    fprintf(out, "  lda result\n");
    PUSH;
    stack++;
  }

  return 0;
}

int M6502_8::put_static(const char *name, int index)
{
  if (stack > 0)
  {
    POP;
    fprintf(out, "  sta %s\n", name);
    stack--;
  }

  return 0;
}

int M6502_8::get_static(const char *name, int index)
{
  fprintf(out, "  lda %s\n", name);
  PUSH;
  stack++;

  return 0;
}

int M6502_8::brk()
{
  return -1;
}

int M6502_8::new_array(uint8_t type)
{
  return 0;
}

int M6502_8::insert_array(const char *name, int32_t *data, int len, uint8_t type)
{
  fprintf(out, "; insert_array\n");

  fprintf(out, ".align 16\n");
  return insert_db(name, data, len, TYPE_BYTE);
}

int M6502_8::insert_string(const char *name, uint8_t *bytes, int len)
{
  fprintf(out, ".align 16\n");
  fprintf(out, "dw %d\n", len);

  return insert_utf8(name, bytes, len);
}

int M6502_8::push_array_length()
{
  fprintf(out, "; push_array_length\n");

  return 0;
}

int M6502_8::push_array_length(const char *name, int field_id)
{
  fprintf(out, "; push_array_length\n");
  stack++;

  return 0;
}

int M6502_8::array_read_byte()
{
  return array_read_int();
}

int M6502_8::array_read_short()
{
  return array_read_int();
}

int M6502_8::array_read_int()
{
  get_values_from_stack(2);
  fprintf(out, "; array_read_int\n");
  stack++;

  return 0;
}

int M6502_8::array_read_byte(const char *name, int field_id)
{
  return array_read_int(name, field_id);
}

int M6502_8::array_read_short(const char *name, int field_id)
{
  return array_read_int(name, field_id);
}

int M6502_8::array_read_int(const char *name, int field_id)
{
  fprintf(out, "; array_read_int2\n");
  stack++;

  return 0;
}

int M6502_8::array_write_byte()
{
  return array_write_int();
}

int M6502_8::array_write_short()
{
  return array_write_int();
}

int M6502_8::array_write_int()
{
  get_values_from_stack(3);
  fprintf(out, "; array_write_int\n");

  return 0;
}

int M6502_8::array_write_byte(const char *name, int field_id)
{
  return array_write_int(name, field_id);
}

int M6502_8::array_write_short(const char *name, int field_id)
{
  return array_write_int(name, field_id);
}

int M6502_8::array_write_int(const char *name, int field_id)
{
  get_values_from_stack(2);
  fprintf(out, "; array_write_int2\n");

  return 0;
}

int M6502_8::get_values_from_stack(int num)
{
  fprintf(out, "; get_values_from_stack, num = %d\n", num);
  if(num > 0)
  {
    POP;
    fprintf(out, "  sta value1\n");
    stack--;
  }

  if(num > 1)
  {
    POP;
    fprintf(out, "  sta value2\n");
    stack--;
  }

  if(num > 2)
  {
    POP;
    fprintf(out, "  sta value3\n");
    stack--;
  }

  return 0;
}

// Memory API
int M6502::memory_read8()
{
  return 0;
}

int M6502::memory_write8()
{
  return 0;
}

#if 0
void M6502::close()
{
}
#endif

