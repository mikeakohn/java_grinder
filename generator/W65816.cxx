/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 * W65816 written by Joe Davisson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "W65816.h"

#define PUSH() \
  fprintf(out, "; PUSH\n"); \
  fprintf(out, "  sta stack,x\n"); \
  fprintf(out, "  dex\n"); \
  fprintf(out, "  dex\n")

#define POP() \
  fprintf(out, "; POP\n"); \
  fprintf(out, "  inx\n"); \
  fprintf(out, "  inx\n"); \
  fprintf(out, "  lda stack,x\n")

#define LOCALS(a) (a * 2)

// ABI is:
// A - accumulator
// X - java stack pointer
// Y - general-purpose index register

W65816::W65816() :
  stack(0),
  java_stack(0x200),
  ram_start(0x7000),
  label_count(0),
  is_main(0),

  need_swap(0),
  need_add_integer(0),
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
  need_memory_write16(0)
{
}

W65816::~W65816()
{
  if(need_swap) { insert_swap(); }
  if(need_add_integer) { insert_add_integer(); }
  if(need_sub_integer) { insert_sub_integer(); }
  if(need_mul_integer) { insert_mul_integer(); }
  if(need_div_integer) { insert_div_integer(); }
  if(need_neg_integer) { insert_neg_integer(); }
  if(need_shift_left_integer) { insert_shift_left_integer(); }
  if(need_shift_right_integer) { insert_shift_right_integer(); }
  if(need_shift_right_uinteger) { insert_shift_right_uinteger(); }
  if(need_and_integer) { insert_and_integer(); }
  if(need_or_integer) { insert_or_integer(); }
  if(need_xor_integer) { insert_xor_integer(); }
  if(need_integer_to_byte) { insert_integer_to_byte(); }
  if(need_dup) { insert_dup(); }
  if(need_dup2) { insert_dup2(); }
  if(need_push_array_length) { insert_push_array_length(); }
  if(need_push_array_length2) { insert_push_array_length2(); }
  if(need_array_byte_support) { insert_array_byte_support(); }
  if(need_array_int_support) { insert_array_int_support(); }
  if(need_memory_read8) { insert_memory_read8(); }
  if(need_memory_write8) { insert_memory_write8(); }
  if(need_memory_read16) { insert_memory_read16(); }
  if(need_memory_write16) { insert_memory_write16(); }
}

int W65816::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".65816\n");

  // java stack
  fprintf(out, "stack equ 0x%04x\n", java_stack);

  // ram start
  fprintf(out, "ram_start equ 0x%04x\n", ram_start);
  fprintf(out, "heap_ptr equ ram_start\n");

  // points to locals
  fprintf(out, "locals equ 0xc0\n");

  // temp variables
  fprintf(out, "result equ 0xc2\n");
  fprintf(out, "remainder equ 0xc4\n");
  fprintf(out, "length equ 0xc6\n");
  fprintf(out, "value1 equ 0xc8\n");
  fprintf(out, "value2 equ 0xca\n");
  fprintf(out, "value3 equ 0xcc\n");
  fprintf(out, "address equ 0xce\n");
  fprintf(out, "address2 equ 0xd0\n");

  // start
  fprintf(out, ".org 0x%04x\n", start_org);
  fprintf(out, "start:\n");
  fprintf(out, "; change to 16-bit mode\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  xce\n");
  fprintf(out, "; all 16-bit registers\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "; set up processor stack\n");
  fprintf(out, "  lda #0x8ff\n");
  fprintf(out, "  tcs\n");
  fprintf(out, "; clear java stack\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  ldx #0\n");
  fprintf(out, "clear_java_stack:\n");
  fprintf(out, "  sta stack,x\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  cpx #0x100\n");
  fprintf(out, "  bne clear_java_stack\n");
  fprintf(out, "; set up java stack pointer\n");
  fprintf(out, "  ldx #0xfe\n");

  return 0;
}

int W65816::start_init()
{
  return 0;
}

int W65816::insert_static_field_define(const char *name, const char *type, int index)
{
  fprintf(out, "; insert_static_field_define\n");
  fprintf(out, "  %s equ ram_start + %d\n", name, (index + 1) * 2);

  return 0;
}

int W65816::init_heap(int field_count)
{
  fprintf(out, "  ; Set up heap and static initializers\n");
  fprintf(out, "  lda #(ram_start + %d)\n", (field_count + 1) * 2);
  fprintf(out, "  sta ram_start\n");

  return 0;
}

int W65816::field_init_int(char *name, int index, int value)
{
  if (value < -32768 || value > 65535) { return -1; }

  fprintf(out, "; field_init_short\n");
  fprintf(out, "  lda #%d\n", (uint16_t)value);
  fprintf(out, "  sta %s\n", name);

  return 0;
}

int W65816::field_init_ref(char *name, int index)
{
  fprintf(out, "; field_init_ref\n");
  fprintf(out, "  lda #_%s\n", name);
  fprintf(out, "  sta %s\n", name);

  return 0;
}

void W65816::method_start(int local_count, int max_stack, int param_count, const char *name)
{
  stack = 0;

  is_main = (strcmp(name, "main") == 0) ? 1 : 0;

  fprintf(out, "%s:\n", name);

  // main() function goes here
  if (!is_main)
  {
    fprintf(out, "  lda locals\n");
    PUSH();
  }

  fprintf(out, "  stx locals\n");
  fprintf(out, "  txa\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #0x%04x\n", local_count * 2);
  fprintf(out, "  tax\n");
}

void W65816::method_end(int local_count)
{
  fprintf(out, "\n");
}

int W65816::push_local_var_int(int index)
{
  fprintf(out, "; push_local_var_int\n");
  fprintf(out, "  ldy locals\n");
  fprintf(out, "  lda stack - %d,y\n", LOCALS(index));
  PUSH();
  stack++;

  return 0;
}

int W65816::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int W65816::push_ref_static(const char *name, int index)
{
  fprintf(out, "; push_ref_static\n");
  fprintf(out, "  lda #_%s\n", name);
  PUSH();
  stack++;

  return 0;
}

int W65816::push_fake()
{
  fprintf(out, "; push_fake()\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  dex\n");
  stack++;
  return 0;
}

int W65816::push_int(int32_t n)
{
  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);

    return -1;
  }

  uint16_t value = (n & 0xffff);

  fprintf(out, "; push_int\n");
  fprintf(out, "  lda #0x%04x\n", value);
  PUSH();
  stack++;

  return 0;
}

int W65816::push_long(int64_t n)
{
  return push_int((int32_t)n);
}

int W65816::push_float(float f)
{
  printf("Float is not supported right now.\n");
  return -1;
}

int W65816::push_double(double f)
{
  printf("Double is not supported right now.\n");
  return -1;
}

int W65816::push_ref(char *name)
{
  fprintf(out, "; push_ref\n");
  fprintf(out, "  lda %s\n", name);
  PUSH();
  stack++;

  return 0;
}

int W65816::pop_local_var_int(int index)
{
  fprintf(out, "; pop_local_var_int\n");
  fprintf(out, "  ldy locals\n");
  POP();
  fprintf(out, "  sta stack - %d,y\n", LOCALS(index));
  stack--;

  return 0;
}

int W65816::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int W65816::pop()
{
  fprintf(out, "; pop\n");
  POP();
  stack--;

  return 0;
}

int W65816::dup()
{
  need_dup = 1;
  fprintf(out, "  jsr dup\n");
  stack++;

  return 0;
}

int W65816::dup2()
{
  need_dup2 = 1;
  fprintf(out, "  jsr dup2\n");
  stack += 2;

  return 0;
}

int W65816::swap()
{
  need_swap = 1;
  fprintf(out, "  jsr swap\n");

  return 0;
}

int W65816::add_integer()
{
  need_add_integer = 1;
  fprintf(out, "  jsr add_integer\n");
  stack--;

  return 0;
}

int W65816::add_integer(int num)
{
  fprintf(out, "; add_integer (0x%04x)\n", num);
  POP();
  fprintf(out, "  clc\n");
  fprintf(out, "  adc #0x%04x\n", num);
  PUSH();

  return 0;
}

int W65816::sub_integer()
{
  need_sub_integer = 1;
  fprintf(out, "  jsr sub_integer\n");
  stack--;

  return 0;
}

int W65816::sub_integer(int num)
{
  fprintf(out, "; add_integer (0x%04x)\n", num);
  POP();
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #0x%04x\n", num);
  PUSH();

  return 0;
}

int W65816::mul_integer()
{
  need_mul_integer = 1;
  fprintf(out, "  jsr mul_integer\n");
  stack--;

  return 0;
}

// unsigned only for now
int W65816::div_integer()
{
  need_div_integer = 1;
  fprintf(out, "  jsr div_integer\n");
  stack--;

  return 0;
}

// unsigned only for now
int W65816::mod_integer()
{
  need_div_integer = 1;
  fprintf(out, "; mod_integer\n");
  fprintf(out, "  jsr div_integer\n");
  POP();
  fprintf(out, "  lda remainder\n");
  PUSH();
  stack--;

  return 0;
}

int W65816::neg_integer()
{
  need_neg_integer = 1;
  fprintf(out, "  jsr neg_integer\n");

  return 0;
}

int W65816::shift_left_integer()
{
  need_shift_left_integer = 1;
  fprintf(out, "  jsr shift_left_integer\n");
  stack--;
  
  return 0;
}

int W65816::shift_left_integer(int num)
{
  int i;

  fprintf(out, "; shift_left_integer (0x%04x)\n", num);
  POP();

  for(i = 0; i < num; i++)
    fprintf(out, "  asl\n");

  PUSH();

  return 0;
}

int W65816::shift_right_integer()
{
  need_shift_right_integer = 1;
  fprintf(out, "  jsr shift_right_integer\n");
  stack--;

  return 0;
}

int W65816::shift_right_integer(int num)
{
  int i;

  fprintf(out, "; shift_right_integer (0x%04x)\n", num);
  POP();

  for(i = 0; i < num; i++)
  {
    fprintf(out, "  cmp #0x8000\n");
    fprintf(out, "  ror\n");
  }

  PUSH();

  return 0;
}

int W65816::shift_right_uinteger()
{
  need_shift_right_uinteger = 1;
  fprintf(out, "  jsr shift_right_uinteger\n");
  stack--;

  return 0;
}

int W65816::shift_right_uinteger(int num)
{
  int i;

  fprintf(out, "; shift_right_uinteger (0x%04x)\n", num);
  POP();

  for(i = 0; i < num; i++)
    fprintf(out, "  lsr\n");

  PUSH();

  return 0;
}

int W65816::and_integer()
{
  need_and_integer = 1;
  fprintf(out, "  jsr and_integer\n");
  stack--;

  return 0;
}

int W65816::and_integer(int num)
{
  fprintf(out, "; and_integer (0x%04x)\n", num);
  POP();
  fprintf(out, "  and #0x%04x\n", num);
  PUSH();

  return 0;
}

int W65816::or_integer()
{
  need_or_integer = 1;
  fprintf(out, "  jsr or_integer\n");
  stack--;

  return 0;
}

int W65816::or_integer(int num)
{
  fprintf(out, "; or_integer (0x%04x)\n", num);
  POP();
  fprintf(out, "  ora #0x%04x\n", num);
  PUSH();

  return 0;
}

int W65816::xor_integer()
{
  need_xor_integer = 1;
  fprintf(out, "  jsr xor_integer\n");
  stack--;

  return 0;
}

int W65816::xor_integer(int num)
{
  fprintf(out, "; xor_integer (0x%04x)\n", num);
  POP();
  fprintf(out, "  eor #0x%04x\n", num);
  PUSH();

  return 0;
}

int W65816::inc_integer(int index, int num)
{
  uint16_t value = num & 0xffff;

  fprintf(out, "; inc_integer num = %d\n", num);
  fprintf(out, "  ldy locals\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda stack - %d,y\n", LOCALS(index));
  fprintf(out, "  adc #0x%04x\n", value);
  fprintf(out, "  sta stack - %d,y\n", LOCALS(index));

  return 0;
}

int W65816::integer_to_byte()
{
  need_integer_to_byte = 1;
  fprintf(out, "  jsr integer_to_byte\n");

  return 0;
}

int W65816::integer_to_short()
{
  // integers are already shorts so do nothing
  return 0;
}

int W65816::jump_cond(const char *label, int cond, int distance)
{
  bool reverse = false;

  if(stack > 0)
  {
    fprintf(out, "; jump_cond\n");
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
        fprintf(out, "  lda stack,x\n");
        fprintf(out, "  cmp #0\n");
        fprintf(out, "  bne #3\n");
        fprintf(out, "  jmp %s\n", label);
        break;
      case COND_NOT_EQUAL:
        fprintf(out, "  lda stack,x\n");
        fprintf(out, "  cmp #0\n");
        fprintf(out, "  beq #3\n");
        fprintf(out, "  jmp %s\n", label);
        break;
      case COND_LESS:
        if(reverse == false)
        {
          fprintf(out, "  lda stack,x\n");
          fprintf(out, "  cmp #0\n");
          fprintf(out, "  bpl #3\n");
          fprintf(out, "  jmp %s\n", label);
        }
          else
        {
          fprintf(out, "  lda #0\n");
          fprintf(out, "  cmp stack,x\n");
          fprintf(out, "  bpl #3\n");
          fprintf(out, "  jmp %s\n", label);
        }
        break;
      case COND_GREATER_EQUAL:
        if(reverse == false)
        {
          fprintf(out, "  lda stack,x\n");
          fprintf(out, "  cmp #0\n");
          fprintf(out, "  bmi #3\n");
          fprintf(out, "  jmp %s\n", label);
        }
          else
        {
          fprintf(out, "  lda #0\n");
          fprintf(out, "  cmp stack,x\n");
          fprintf(out, "  bmi #3\n");
          fprintf(out, "  jmp %s\n", label);
        }
        break;
    }

    stack--;
  }

  return 0;
}

int W65816::jump_cond_integer(const char *label, int cond, int distance)
{
  bool reverse = false;

  if(stack > 1)
  {
    fprintf(out, "; jump_cond_integer\n");
    fprintf(out, "  inx\n");
    fprintf(out, "  inx\n");
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
        fprintf(out, "  lda stack - 0,x\n");
        fprintf(out, "  cmp stack - 2,x\n");
        fprintf(out, "  bne #3\n");
        fprintf(out, "  jmp %s\n", label);
        break;
      case COND_NOT_EQUAL:
        fprintf(out, "  lda stack - 0,x\n");
        fprintf(out, "  cmp stack - 2,x\n");
        fprintf(out, "  beq #3\n");
        fprintf(out, "  jmp %s\n", label);
        break;
      case COND_LESS:
        if(reverse == false)
        {
          fprintf(out, "  lda stack - 0,x\n");
          fprintf(out, "  cmp stack - 2,x\n");
          fprintf(out, "  bpl #3\n");
          fprintf(out, "  jmp %s\n", label);
        }
          else
        {
          fprintf(out, "  lda stack - 2,x\n");
          fprintf(out, "  cmp stack - 0,x\n");
          fprintf(out, "  bpl #3\n");
          fprintf(out, "  jmp %s\n", label);
        }
        break;
      case COND_GREATER_EQUAL:
        if(reverse == false)
        {
          fprintf(out, "  lda stack - 0,x\n");
          fprintf(out, "  cmp stack - 2,x\n");
          fprintf(out, "  bmi #3\n");
          fprintf(out, "  jmp %s\n", label);
        }
          else
        {
          fprintf(out, "  lda stack - 2,x\n");
          fprintf(out, "  cmp stack - 0,x\n");
          fprintf(out, "  bmi #3\n");
          fprintf(out, "  jmp %s\n", label);
        }
        break;
    }

    stack -= 2;
  }

  return 0;
}

int W65816::return_local(int index, int local_count)
{
  fprintf(out, "; return_local\n");
  fprintf(out, "  ldy locals\n");
  fprintf(out, "  lda stack - %d,y\n", LOCALS(index));
  fprintf(out, "  sta result\n");

  fprintf(out, "  ldx locals\n");

  if (!is_main)
  {
    POP();
    fprintf(out, "  sta locals\n");
  }

  fprintf(out, "  rts\n");

  return 0;
}

int W65816::return_integer(int local_count)
{
  fprintf(out, "; return_integer\n");
  POP();
  fprintf(out, "  sta result\n");
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

int W65816::return_void(int local_count)
{
  fprintf(out, "; return_void\n");
  fprintf(out, "  ldx locals\n");

  if (!is_main)
  {
    POP();
    fprintf(out, "  sta locals\n");
    fprintf(out, "  rts\n");
  }
  else
  {
    // exit to monitor/operating system
    fprintf(out, "  brk\n");
  }

  return 0;
}

int W65816::jump(const char *name, int distance)
{
  fprintf(out, "  jmp %s\n", name);

  return 0;
}

int W65816::call(const char *name)
{
  fprintf(out, "  jsr %s\n", name);

  return 0;
}

int W65816::invoke_static_method(const char *name, int params, int is_void)
{
int local;
int stack_vars = stack;

  printf("invoke_static_method() name=%s params=%d is_void=%d\n", name, params, is_void);

  fprintf(out, "; invoke_static_method\n");

  local = (params * -2);
  while(local != 0)
  {
    if(stack_vars > 0)
    {
      fprintf(out, "  txy\n");
      fprintf(out, "  lda stack + 2 + %d,y\n", (stack - stack_vars) * 2);
      fprintf(out, "  sta stack + 2 %d,y\n", local - 2);
      stack_vars--;
    }

    local += 2;
  }

  fprintf(out, "  jsr %s\n", name);

  if((stack - stack_vars) > 0)
  {
    fprintf(out, "  txa\n");
    fprintf(out, "  clc\n");
    fprintf(out, "  adc #%d\n", (stack - stack_vars) * 2);
    fprintf(out, "  tax\n");

    params -= (stack - stack_vars);
  }

  if(!is_void)
  {
    fprintf(out, "  lda result\n");
    PUSH();
    stack++;
  }

  return 0;
}

int W65816::put_static(const char *name, int index)
{
  fprintf(out, "  lda %s\n", name);
  PUSH();
  stack++;

  return 0;
}

int W65816::get_static(const char *name, int index)
{
  fprintf(out, "  lda %s\n", name);
  PUSH();
  stack++;

  return 0;
}

int W65816::brk()
{
  return -1;
}

int W65816::new_array(uint8_t type)
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

int W65816::insert_array(const char *name, int32_t *data, int len, uint8_t type)
{
  if (type == TYPE_BYTE)
  {
    return insert_db(name, data, len, TYPE_SHORT);
  }
    else
  if (type == TYPE_SHORT)
  {
    return insert_dw(name, data, len, TYPE_SHORT);
  }
    else
  if (type == TYPE_INT)
  {
    return insert_dw(name, data, len, TYPE_SHORT);
  }

  return 0;
}

int W65816::insert_string(const char *name, uint8_t *bytes, int len)
{
  fprintf(out, "dw %d\n", len);
  return insert_utf8(name, bytes, len);
}

int W65816::push_array_length()
{
  if(stack > 0)
  {
    need_push_array_length = 1;
    fprintf(out, "  jsr push_array_length\n");
  }

  return 0;
}

int W65816::push_array_length(const char *name, int field_id)
{
  need_push_array_length2 = 1;
  fprintf(out, "  lda %s\n", name);
  fprintf(out, "  jsr push_array_length2\n");
  stack++;

  return 0;
}

int W65816::array_read_byte()
{
  need_array_byte_support = 1;
  get_values_from_stack(2);
  fprintf(out, "  jsr array_read_byte\n");
  stack++;

  return 0;
}

int W65816::array_read_short()
{
  return array_read_int();
}

int W65816::array_read_int()
{
  need_array_int_support = 1;
  get_values_from_stack(2);
  fprintf(out, "  jsr array_read_int\n");
  stack++;

  return 0;
}

int W65816::array_read_byte(const char *name, int field_id)
{
  if(stack > 0)
  {
    need_array_byte_support = 1;
    fprintf(out, "  lda %s\n", name);
    fprintf(out, "  sta address\n");
    fprintf(out, "  jsr array_read_byte2\n");
  }

  return 0;
}

int W65816::array_read_short(const char *name, int field_id)
{
  return array_read_int(name, field_id);
}

int W65816::array_read_int(const char *name, int field_id)
{
  if(stack > 0)
  {
    need_array_int_support = 1;
    fprintf(out, "  lda %s\n", name);
    fprintf(out, "  sta address\n");
    fprintf(out, "  jsr array_read_int2\n");
  }

  return 0;
}

int W65816::array_write_byte()
{
  need_array_byte_support = 1;
  get_values_from_stack(3);
  fprintf(out, "  jsr array_write_byte\n");

  return 0;
}

int W65816::array_write_short()
{
  return array_write_int();
}

int W65816::array_write_int()
{
  need_array_int_support = 1;
  get_values_from_stack(3);
  fprintf(out, "  jsr array_write_int\n");

  return 0;
}

int W65816::array_write_byte(const char *name, int field_id)
{
  need_array_byte_support = 1;
  get_values_from_stack(2);
  fprintf(out, "; array_write_byte2\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda value2\n");
  fprintf(out, "  adc %s\n", name);
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda value1\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta (address)\n");
  fprintf(out, "  rep #0x30\n");

  return 0;
}

int W65816::array_write_short(const char *name, int field_id)
{
  return array_write_int(name, field_id);
}

int W65816::array_write_int(const char *name, int field_id)
{
  need_array_int_support = 1;
  get_values_from_stack(2);
  fprintf(out, "; array_write_int2\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda value2\n");
  fprintf(out, "  adc %s\n", name);
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda value1\n");
  fprintf(out, "  sta (address)\n");

  return 0;
} 

int W65816::get_values_from_stack(int num)
{
  fprintf(out, "; get_values_from_stack, num = %d\n", num);

  if(num > 0)
  {
    POP();
    fprintf(out, "  sta value1\n");
    stack--;
  }

  if(num > 1)
  {
    POP();
    fprintf(out, "  sta value2\n");
    stack--;
  }

  if(num > 2)
  {
    POP();
    fprintf(out, "  sta value3\n");
    stack--;
  }

  return 0;
}

// subroutines
void W65816::insert_swap()
{
  fprintf(out, "swap:\n");
  fprintf(out, "  txa\n");
  fprintf(out, "  tay\n");

  fprintf(out, "  lda stack,y\n");
  fprintf(out, "  sta value1,y\n");

  fprintf(out, "  lda stack - 1,y\n");
  fprintf(out, "  sta stack,y\n");

  fprintf(out, "  lda value1\n");
  fprintf(out, "  sta stack - 1,y\n");
  fprintf(out, "  rts\n");
}

void W65816::insert_add_integer()
{
  fprintf(out, "add_integer:\n");
  POP();
  fprintf(out, "  sta value1\n");
  POP();
  fprintf(out, "  clc\n");
  fprintf(out, "  adc value1\n");
  PUSH();
  fprintf(out, "  rts\n");
}

void W65816::insert_sub_integer()
{
  fprintf(out, "sub_integer:\n");
  POP();
  fprintf(out, "  sta value1\n");
  POP();
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc value1\n");
  PUSH();
  fprintf(out, "  rts\n");
}

void W65816::insert_mul_integer()
{
  fprintf(out, "mul_integer:\n");
  POP();
  fprintf(out, "  sta value2\n");
  POP();
  fprintf(out, "  sta value1\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "mul_integer_1:\n");
  fprintf(out, "  ldy value1\n");
  fprintf(out, "  beq mul_integer_done\n");
  fprintf(out, "  lsr value1\n");
  fprintf(out, "  bcc mul_integer_2\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  adc value2\n");
  fprintf(out, "mul_integer_2:\n");
  fprintf(out, "  asl value2\n");
  fprintf(out, "  bra mul_integer_1\n");
  fprintf(out, "mul_integer_done:\n");
  PUSH();
  fprintf(out, "  rts\n");
}

void W65816::insert_div_integer()
{
  fprintf(out, "div_integer:\n");
  POP();
  fprintf(out, "  tay\n");
  POP();
  fprintf(out, "  sta remainder\n");
  fprintf(out, "  tya\n");
  fprintf(out, "  stz value3\n");
  fprintf(out, "  ldy #1\n");
  fprintf(out, "div_integer_1:\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  bcs div_integer_2\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  cpy #17\n");
  fprintf(out, "  bne div_integer_1\n");
  fprintf(out, "div_integer_2:\n");
  fprintf(out, "  ror\n");
  fprintf(out, "div_integer_4:\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda remainder\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc 1,s\n");
  fprintf(out, "  bcc div_integer_3\n");
  fprintf(out, "  sta remainder\n");
  fprintf(out, "div_integer_3:\n");
  fprintf(out, "  rol value3\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  lsr\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne div_integer_4\n");
  fprintf(out, "div_integer_done:\n");
  fprintf(out, "  lda value3\n");
  PUSH();
  fprintf(out, "  rts\n");
}

void W65816::insert_neg_integer()
{
  fprintf(out, "neg_integer:\n");
  POP();
  fprintf(out, "  eor #0xffff\n");
  fprintf(out, "  inc\n");
  PUSH();
  fprintf(out, "  rts\n");
}

void W65816::insert_shift_left_integer()
{
  fprintf(out, "shift_left_integer:\n");
  POP();
  fprintf(out, "  tay\n");
  POP();
  fprintf(out, "shift_left_integer_loop:\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne shift_left_integer_loop\n");
  PUSH();
  fprintf(out, "  rts\n");
}

void W65816::insert_shift_right_integer()
{
  fprintf(out, "shift_right_integer:\n");
  POP();
  fprintf(out, "  tay\n");
  POP();
  fprintf(out, "shift_right_integer_loop:\n");
  fprintf(out, "  cmp #0x8000\n");
  fprintf(out, "  ror\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne shift_right_integer_loop\n");
  PUSH();
  fprintf(out, "  rts\n");
}

void W65816::insert_shift_right_uinteger()
{
  fprintf(out, "shift_right_uinteger:\n");
  POP();
  fprintf(out, "  tay\n");
  POP();
  fprintf(out, "shift_right_uinteger_loop:\n");
  fprintf(out, "  lsr\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne label_%d\n", label_count);
  PUSH();
  fprintf(out, "  rts\n");
}

void W65816::insert_and_integer()
{
  fprintf(out, "and_integer:\n");
  POP();
  fprintf(out, "  sta value1\n");
  POP();
  fprintf(out, "  and value1\n");
  PUSH();
  fprintf(out, "  rts\n");
}

void W65816::insert_or_integer()
{
  fprintf(out, "or_integer:\n");
  POP();
  fprintf(out, "  sta value1\n");
  POP();
  fprintf(out, "  ora value1\n");
  PUSH();
  fprintf(out, "  rts\n");
}

void W65816::insert_xor_integer()
{
  fprintf(out, "xor_integer:\n");
  POP();
  fprintf(out, "  sta value1\n");
  POP();
  fprintf(out, "  eor value1\n");
  PUSH();
  fprintf(out, "  rts\n");
}

void W65816::insert_integer_to_byte()
{
  fprintf(out, "integer_to_byte:\n");
  POP();
  fprintf(out, "  eor #128\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #128\n");
  PUSH();
  fprintf(out, "  rts\n");
}

void W65816::insert_dup()
{
  fprintf(out, "dup:\n");
  POP();
  PUSH();
  PUSH();
  fprintf(out, "  rts\n");
}

void W65816::insert_dup2()
{
  fprintf(out, "dup2:\n");
  POP();
  fprintf(out, "  sta value2\n");
  POP();
  fprintf(out, "  sta value1\n");
  PUSH();
  fprintf(out, "  lda value2\n");
  PUSH();
  fprintf(out, "  lda value1\n");
  PUSH();
  fprintf(out, "  lda value2\n");
  PUSH();
  fprintf(out, "  rts\n");
}

void W65816::insert_push_array_length()
{
  fprintf(out, "push_array_length:\n");
  POP();
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #2\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda (address)\n");
  PUSH();
  fprintf(out, "  rts\n");
}

void W65816::insert_push_array_length2()
{
  fprintf(out, "push_array_length2:\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #2\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda (address)\n");
  PUSH();
  fprintf(out, "  rts\n");
}

void W65816::insert_array_byte_support()
{
  // new_array_byte
  fprintf(out, "new_array_byte:\n");
  POP();
  fprintf(out, "  sta length\n");
  fprintf(out, "  lda heap_ptr\n");
  fprintf(out, "  sta value1\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda length\n");
  fprintf(out, "  sta (address)\n");

  fprintf(out, "  clc\n");
  fprintf(out, "  lda length\n");
  fprintf(out, "  adc #2\n");
  fprintf(out, "  sta length\n");

  fprintf(out, "  clc\n");
  fprintf(out, "  lda heap_ptr\n");
  fprintf(out, "  adc length\n");
  fprintf(out, "  sta heap_ptr\n");

  fprintf(out, "  clc\n");
  fprintf(out, "  lda value1\n");
  fprintf(out, "  adc #3\n");

  fprintf(out, "  and #0xfffe\n");
  PUSH();
  fprintf(out, "  rts\n");

  // array_read_byte
  fprintf(out, "array_read_byte:\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda value2\n");
  fprintf(out, "  adc value1\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda (address)\n");
  fprintf(out, "  eor #128\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #128\n");
  PUSH();
  fprintf(out, "  rts\n");

  // array_read_byte2
  fprintf(out, "array_read_byte2:\n");
  POP();
  fprintf(out, "  sta value1\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda address\n");
  fprintf(out, "  adc value1\n");
  fprintf(out, "  lda (address)\n");
  fprintf(out, "  eor #128\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #128\n");
  PUSH();
  fprintf(out, "  rts\n");

  // array_write_byte
  fprintf(out, "array_write_byte:\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda value3\n");
  fprintf(out, "  adc value2\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda value1\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta (address)\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  rts\n");
}

void W65816::insert_array_int_support()
{
  // new_array_int
  fprintf(out, "new_array_int:\n");
  POP();
  fprintf(out, "  sta length\n");
  fprintf(out, "  lda heap_ptr\n");
  fprintf(out, "  sta value1\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda length\n");
  fprintf(out, "  sta (address)\n");

  fprintf(out, "  asl length\n");

  fprintf(out, "  clc\n");
  fprintf(out, "  lda length\n");
  fprintf(out, "  adc #2\n");
  fprintf(out, "  sta length\n");

  fprintf(out, "  clc\n");
  fprintf(out, "  lda heap_ptr\n");
  fprintf(out, "  adc length\n");
  fprintf(out, "  sta heap_ptr\n");

  fprintf(out, "  clc\n");
  fprintf(out, "  lda value1\n");
  fprintf(out, "  adc #3\n");

  fprintf(out, "  and #0xfffe\n");
  PUSH();
  fprintf(out, "  rts\n");

  // array_read_int
  fprintf(out, "array_read_int:\n");
  fprintf(out, "  asl value1\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda value2\n");
  fprintf(out, "  adc value1\n");
  fprintf(out, "  sta value2\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda (address)\n");
  PUSH();
  fprintf(out, "  rts\n");

  // array_read_int2
  fprintf(out, "array_read_int2:\n");
  POP();
  fprintf(out, "  sta value1\n");
  fprintf(out, "  asl value1\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda address\n");
  fprintf(out, "  adc value1\n");
  fprintf(out, "  lda (address)\n");
  PUSH();
  fprintf(out, "  rts\n");

  // array_write_int
  fprintf(out, "array_write_int:\n");
  fprintf(out, "  asl value2\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda value3\n");
  fprintf(out, "  adc value2\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda value1\n");
  fprintf(out, "  sta (address)\n");
  fprintf(out, "  rts\n");
}

// Memory API
int W65816::memory_read8_I()
{
  need_memory_read8 = 1;

  fprintf(out, "  jsr memory_read8\n");

  return 0;
}

int W65816::memory_write8_IB()
{
  need_memory_write8 = 1;

  fprintf(out, "  jsr memory_write8\n");
  stack -= 2;

  return 0;
}

int W65816::memory_read16_I()
{
  need_memory_read16 = 1;

  fprintf(out, "  jsr memory_read16\n");

  return 0;
}

int W65816::memory_write16_IS()
{
  need_memory_write16 = 1;

  fprintf(out, "  jsr memory_write16\n");
  stack -= 2;

  return 0;
}

void W65816::insert_memory_read8()
{
  fprintf(out, "memory_read8:\n");
  fprintf(out, "  rts\n");
}

void W65816::insert_memory_write8()
{
  fprintf(out, "memory_write8:\n");
  POP();
  fprintf(out, "  sta value1\n");
  POP();
  fprintf(out, "  sta address\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  lda value1\n");
  fprintf(out, "  sta (address)\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  rts\n");
}

void W65816::insert_memory_read16()
{
  fprintf(out, "memory_read16:\n");
  fprintf(out, "  rts\n");
}

void W65816::insert_memory_write16()
{
  fprintf(out, "memory_write16:\n");
  fprintf(out, "  rts\n");
}

