/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2023 by Michael Kohn
 *
 * W65816 written by Joe Davisson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "generator/W65816.h"

#define LOCALS(a) ((a * 2) + 1)

// ABI is:
// A - accumulator
// X - java stack pointer
// Y - general-purpose index register

W65816::W65816() :
  stack(0),
  ram_start(0x7000),
  label_count(0),
  is_main(false),

  need_mul_integer(0),
  need_div_integer(0),
  need_push_array_length(0),
  need_push_array_length2(0),
  need_array_byte_support(0),
  need_array_int_support(0)
//  need_memory_read8(0),
//  need_memory_write8(0),
//  need_memory_read16(0),
//  need_memory_write16(0)
{
}

W65816::~W65816()
{
  if(need_mul_integer) { insert_mul_integer(); }
  if(need_div_integer) { insert_div_integer(); }
  if(need_push_array_length) { insert_push_array_length(); }
  if(need_push_array_length2) { insert_push_array_length2(); }
  if(need_array_byte_support) { insert_array_byte_support(); }
  if(need_array_int_support) { insert_array_int_support(); }
//  if(need_memory_read8) { insert_memory_read8(); }
//  if(need_memory_write8) { insert_memory_write8(); }
//  if(need_memory_read16) { insert_memory_read16(); }
//  if(need_memory_write16) { insert_memory_write16(); }
}

int W65816::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".65816\n");

  // ram start
  fprintf(out, "ram_start equ 0x%04x\n", ram_start);
  fprintf(out, "heap_ptr equ ram_start\n");

  // points to locals
  fprintf(out, "locals equ 0xc0\n");

  // internal variables
  fprintf(out, "result equ 0xc2\n");
  fprintf(out, "remainder equ 0xc4\n");
  fprintf(out, "length equ 0xc6\n");
  fprintf(out, "value1 equ 0xc8\n");
  fprintf(out, "value2 equ 0xca\n");
  fprintf(out, "value3 equ 0xcc\n");
  fprintf(out, "value4 equ 0xce\n");
  fprintf(out, "address equ 0xd0\n");
  fprintf(out, "address2 equ 0xd2\n");

  // mini-stack for saving return addresses, can go anywhere
  fprintf(out, "return_index equ 0xd4\n");
  fprintf(out, "return_stack equ 0x100\n");

  // start
  fprintf(out, ".org 0x%04x\n", start_org);
  fprintf(out, "start:\n");
  fprintf(out, "; change to 16-bit mode\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  xce\n");
  fprintf(out, "; all 16-bit registers\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  stz return_index\n");
  fprintf(out, "; stack pointer\n");
  fprintf(out, "  lda #0x1ff\n");
  fprintf(out, "  tcs\n");

  return 0;
}

int W65816::start_init()
{
  return 0;
}

int W65816::insert_static_field_define(
  std::string &name,
  std::string &type,
  int index)
{
  fprintf(out, "; insert_static_field_define\n");
  fprintf(out, "  %s equ ram_start + %d\n", name.c_str(), (index + 1) * 2);

  return 0;
}

int W65816::init_heap(int field_count)
{
  fprintf(out, "  ; Set up heap and static initializers\n");
  fprintf(out, "  lda #(ram_start + %d)\n", (field_count + 1) * 2);
  fprintf(out, "  sta ram_start\n");

  return 0;
}

int W65816::field_init_int(std::string &name, int index, int value)
{
  if (value < -32768 || value > 65535) { return -1; }

  fprintf(out, "; field_init_short\n");
  fprintf(out, "  lda #%d\n", (uint16_t)value);
  fprintf(out, "  sta %s\n", name.c_str());

  return 0;
}

int W65816::field_init_ref(std::string &name, int index)
{
  fprintf(out, "; field_init_ref\n");
  fprintf(out, "  lda #_%s\n", name.c_str());
  fprintf(out, "  sta %s\n", name.c_str());

  return 0;
}

void W65816::method_start(
  int local_count,
  int max_stack,
  int param_count,
  std::string &name)
{
  stack = 0;

  is_main = name == "main";

  fprintf(out, "%s:\n", name.c_str());

  // main() function goes here
  if (!is_main)
  {
    save_return();
    fprintf(out, "  lda locals\n");
    fprintf(out, "  pha\n");
  }

  fprintf(out, "  tsc\n");
  fprintf(out, "  sta locals\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #0x%04x\n", local_count * 2);
  fprintf(out, "  tcs\n");
}

void W65816::method_end(int local_count)
{
  fprintf(out, "\n");
}

int W65816::push_local_var_int(int index)
{
  fprintf(out, "; push_local_var_int\n");
  fprintf(out, "  lda locals\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #%d\n", LOCALS(index));
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda (address)\n");
  fprintf(out, "  pha\n");
  stack++;

  return 0;
}

int W65816::push_local_var_ref(int index)
{
  fprintf(out, "; push_local_var_ref\n");
  return push_local_var_int(index);
}

int W65816::push_ref_static(std::string &name, int index)
{
  fprintf(out, "; push_ref_static\n");
  fprintf(out, "  lda #_%s\n", name.c_str());
  fprintf(out, "  pha\n");
  stack++;

  return 0;
}

int W65816::push_fake()
{
/*
  fprintf(out, "; push_fake\n");
  fprintf(out, "  pha\n");
  stack++;

  return 0;
*/

  return -1;
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
  fprintf(out, "  pha\n");
  stack++;

  return 0;
}

int W65816::push_long(int64_t n)
{
  return push_int((int32_t)n);
}

#if 0
int W65816::push_float(float f)
{
  return -1;
}

int W65816::push_double(double f)
{
  return -1;
}
#endif

int W65816::push_ref(std::string &name, int index)
{
  fprintf(out, "; push_ref\n");
  fprintf(out, "  lda %s\n", name.c_str());
  fprintf(out, "  pha\n");
  stack++;

  return 0;
}

int W65816::pop_local_var_int(int index)
{
  fprintf(out, "; pop_local_var_int\n");
  fprintf(out, "  lda locals\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #%d\n", LOCALS(index));
  fprintf(out, "  sta address\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta (address)\n");
  stack--;

  return 0;
}

int W65816::pop_local_var_ref(int index)
{
  fprintf(out, "; pop_local_var_ref\n");
  return pop_local_var_int(index);
}

int W65816::pop()
{
  fprintf(out, "; pop\n");
  fprintf(out, "  pla\n");
  stack--;

  return 0;
}

int W65816::dup()
{
  fprintf(out, "; dup\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  pha\n");
  stack++;

  return 0;
}

int W65816::dup2()
{
  fprintf(out, "; dup2\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta value2\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta value1\n");

  //fprintf(out, "  lda value1\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda value2\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda value1\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda value2\n");
  fprintf(out, "  pha\n");
  stack += 2;

  return 0;
}

int W65816::swap()
{
  fprintf(out, "; swap\n");
  fprintf(out, "  lda 1,s\n");
  fprintf(out, "  sta value1\n");

  fprintf(out, "  lda 3,s\n");
  fprintf(out, "  sta 1,s\n");

  fprintf(out, "  lda value1\n");
  fprintf(out, "  sta 3,s\n");

  return 0;
}

int W65816::add_integer()
{
  fprintf(out, "; add_integer\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda 1,s\n");
  fprintf(out, "  adc 3,s\n");
  fprintf(out, "  sta 3,s\n");
  fprintf(out, "  pla\n");
  stack--;

  return 0;
}

int W65816::add_integer(int num)
{
  fprintf(out, "; add_integer (const)\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda #0x%04x\n", num);
  fprintf(out, "  adc 1,s\n");
  fprintf(out, "  sta 1,s\n");

  return 0;
}

int W65816::sub_integer()
{
  fprintf(out, "; sub_integer\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  lda 3,s\n");
  fprintf(out, "  sbc 1,s\n");
  fprintf(out, "  sta 3,s\n");
  fprintf(out, "  pla\n");
  stack--;

  return 0;
}

int W65816::sub_integer(int num)
{
  fprintf(out, "; sub_integer (const)\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  lda 1,s\n");
  fprintf(out, "  sbc #0x%04x\n", num);
  fprintf(out, "  sta 1,s\n");

  return 0;
}

int W65816::mul_integer()
{
  need_mul_integer = 1;
  fprintf(out, "  jsr mul_integer\n");
  stack--;

  return 0;
}

// unsigned only
int W65816::div_integer()
{
  need_div_integer = 1;
  fprintf(out, "  jsr div_integer\n");
  stack--;

  return 0;
}

// unsigned only
int W65816::mod_integer()
{
  need_div_integer = 1;
  fprintf(out, "; mod_integer\n");
  fprintf(out, "  jsr div_integer\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  lda remainder\n");
  fprintf(out, "  pha\n");
  stack--;

  return 0;
}

int W65816::neg_integer()
{
  fprintf(out, "; neg_integer\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sbc (1,s)\n");
  fprintf(out, "  sta (1,s)\n");

  return 0;
}

int W65816::shift_left_integer()
{
  fprintf(out, "; shift_left_integer\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tay\n");
  fprintf(out, "  pla\n");
  fprintf(out, "shift_left_integer_loop_%d:\n", label_count);
  fprintf(out, "  asl\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne shift_left_integer_loop_%d\n", label_count++);
  fprintf(out, "  pha\n");
  stack--;
  
  return 0;
}

int W65816::shift_left_integer(int num)
{
  int i;

  fprintf(out, "; shift_left_integer (0x%04x)\n", num);
  fprintf(out, "  pla\n");

  for(i = 0; i < num; i++)
    fprintf(out, "  asl\n");

  fprintf(out, "  pha\n");

  return 0;
}

int W65816::shift_right_integer()
{
  fprintf(out, "; shift_right_integer\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tay\n");
  fprintf(out, "  pla\n");
  fprintf(out, "shift_right_integer_loop_%d:\n", label_count);
  fprintf(out, "  cmp #0x8000\n");
  fprintf(out, "  ror\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne shift_right_integer_loop_%d\n", label_count++);
  fprintf(out, "  pha\n");
  stack--;

  return 0;
}

int W65816::shift_right_integer(int num)
{
  int i;

  fprintf(out, "; shift_right_integer (0x%04x)\n", num);
  fprintf(out, "  pla\n");

  for(i = 0; i < num; i++)
  {
    fprintf(out, "  cmp #0x8000\n");
    fprintf(out, "  ror\n");
  }

  fprintf(out, "  pha\n");

  return 0;
}

int W65816::shift_right_uinteger()
{
  fprintf(out, "; shift_right_uinteger\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tay\n");
  fprintf(out, "  pla\n");
  fprintf(out, "shift_right_uinteger_loop_%d:\n", label_count);
  fprintf(out, "  lsr\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne shift_right_uinteger_loop_%d\n", label_count++);
  fprintf(out, "  pha\n");
  stack--;

  return 0;
}

int W65816::shift_right_uinteger(int num)
{
  int i;

  fprintf(out, "; shift_right_uinteger (0x%04x)\n", num);
  fprintf(out, "  pla\n");

  for(i = 0; i < num; i++)
    fprintf(out, "  lsr\n");

  fprintf(out, "  pha\n");

  return 0;
}

int W65816::and_integer()
{
  fprintf(out, "; and_integer\n");
  fprintf(out, "  lda 1,s\n");
  fprintf(out, "  and 3,s\n");
  fprintf(out, "  sta 3,s\n");
  fprintf(out, "  pla\n");
  stack--;

  return 0;
}

int W65816::and_integer(int num)
{
  fprintf(out, "; and_integer (const)\n");
  fprintf(out, "  lda #0x%04x\n", num);
  fprintf(out, "  and 1,s\n");
  fprintf(out, "  sta 1,s\n");

  return 0;
}

int W65816::or_integer()
{
  fprintf(out, "; or_integer\n");
  fprintf(out, "  lda 1,s\n");
  fprintf(out, "  ora 3,s\n");
  fprintf(out, "  sta 3,s\n");
  fprintf(out, "  pla\n");
  stack--;

  return 0;
}

int W65816::or_integer(int num)
{
  fprintf(out, "; or_integer (const)\n");
  fprintf(out, "  lda #0x%04x\n", num);
  fprintf(out, "  ora 1,s\n");
  fprintf(out, "  sta 1,s\n");

  return 0;
}

int W65816::xor_integer()
{
  fprintf(out, "; xor_integer\n");
  fprintf(out, "  lda 1,s\n");
  fprintf(out, "  eor 3,s\n");
  fprintf(out, "  sta 3,s\n");
  fprintf(out, "  pla\n");
  stack--;

  return 0;
}

int W65816::xor_integer(int num)
{
  fprintf(out, "; xor_integer (const)\n");
  fprintf(out, "  lda #0x%04x\n", num);
  fprintf(out, "  eor 1,s\n");
  fprintf(out, "  sta 1,s\n");

  return 0;
}

int W65816::inc_integer(int index, int num)
{
  uint16_t value = num & 0xffff;

  fprintf(out, "; inc_integer num = %d\n", num);

  if(num == 1)
  {
    fprintf(out, "  lda locals\n");
    fprintf(out, "  sec\n");
    fprintf(out, "  sbc #%d\n", LOCALS(index));
    fprintf(out, "  sta address\n");
    fprintf(out, "  lda (address)\n");
    fprintf(out, "  inc\n");
    fprintf(out, "  sta (address)\n");
  }
  else
  {
    fprintf(out, "  lda locals\n");
    fprintf(out, "  sec\n");
    fprintf(out, "  sbc #%d\n", LOCALS(index));
    fprintf(out, "  sta address\n");
    fprintf(out, "  lda (address)\n");
    fprintf(out, "  clc\n");
    fprintf(out, "  adc #0x%04x\n", value & 0xffff);
    fprintf(out, "  sta (address)\n");
  }

  return 0;
}

int W65816::integer_to_byte()
{
  fprintf(out, "; integer_to_byte\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  eor #128\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #128\n");
  fprintf(out, "  pha\n");

  return 0;
}

int W65816::integer_to_short()
{
  // integers are already shorts so do nothing
  return 0;
}

int W65816::jump_cond(std::string &label, int cond, int distance)
{
  bool reverse = false;

  fprintf(out, "; jump_cond\n");

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
      fprintf(out, "  pla\n");
      fprintf(out, "  cmp #0\n");
      fprintf(out, "  bne jump_cond_skip_2_%d\n", label_count);
      fprintf(out, "  jmp %s\n", label.c_str());
      break;
    case COND_NOT_EQUAL:
      fprintf(out, "  pla\n");
      fprintf(out, "  cmp #0\n");
      fprintf(out, "  beq jump_cond_skip_2_%d\n", label_count);
      fprintf(out, "  jmp %s\n", label.c_str());
      break;
    case COND_LESS:
      if(reverse == false)
      {
        fprintf(out, "  pla\n");
        fprintf(out, "  cmp #0\n");
        fprintf(out, "  bpl jump_cond_skip_2_%d\n", label_count);
        fprintf(out, "  jmp %s\n", label.c_str());
      }
        else
      {
        fprintf(out, "  lda #0\n");
        fprintf(out, "  cmp 1,s\n");
        fprintf(out, "  bpl jump_cond_skip_%d\n", label_count);
        fprintf(out, "  pla\n");
        fprintf(out, "  jmp %s\n", label.c_str());
      }
      break;
    case COND_GREATER_EQUAL:
      if(reverse == false)
      {
        fprintf(out, "  pla\n");
        fprintf(out, "  cmp #0\n");
        fprintf(out, "  bmi jump_cond_skip_2_%d\n", label_count);
        fprintf(out, "  jmp %s\n", label.c_str());
      }
        else
      {
        fprintf(out, "  lda #0\n");
        fprintf(out, "  cmp 1,s\n");
        fprintf(out, "  bmi jump_cond_skip_%d\n", label_count);
        fprintf(out, "  pla\n");
        fprintf(out, "  jmp %s\n", label.c_str());
      }
      break;
  }

  fprintf(out, "jump_cond_skip_%d:\n", label_count);
  fprintf(out, "  pla\n");
  fprintf(out, "jump_cond_skip_2_%d:\n", label_count);

  label_count++;
  stack--;

  return 0;
}

int W65816::jump_cond_zero(std::string &label, int cond, int distance)
{
  fprintf(out, "; jump_cond_zero\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  cmp #0\n");

  if (cond == COND_EQUAL)
  {
    fprintf(out, "  bne jump_cond_zero_skip_%d\n", label_count);
    fprintf(out, "  jmp %s\n", label.c_str());
  } 
    else
  if (cond == COND_NOT_EQUAL)
  {
    fprintf(out, "  beq jump_cond_zero_skip_%d\n", label_count);
    fprintf(out, "  jmp %s\n", label.c_str());
  }

  fprintf(out, "jump_cond_zero_skip_%d:\n", label_count++);
  stack--;

  return 0;
}

int W65816::jump_cond_integer(std::string &label, int cond, int distance)
{
  bool reverse = false;

  fprintf(out, "; jump_cond_integer\n");

  if (cond == COND_LESS_EQUAL)
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

  switch (cond)
  {
    case COND_EQUAL:
      fprintf(out, "  pla\n");
      fprintf(out, "  cmp 1,s\n");
      fprintf(out, "  bne jump_cond_integer_skip_2_%d\n", label_count);
      fprintf(out, "  pla\n");
      fprintf(out, "  jmp %s\n", label.c_str());
      break;
    case COND_NOT_EQUAL:
      fprintf(out, "  pla\n");
      fprintf(out, "  cmp 1,s\n");
      fprintf(out, "  beq jump_cond_integer_skip_2_%d\n", label_count);
      fprintf(out, "  pla\n");
      fprintf(out, "  jmp %s\n", label.c_str());
      break;
    case COND_LESS:
      if(reverse == false)
      {
        fprintf(out, "  lda 3,s\n");
        fprintf(out, "  cmp 1,s\n");
        fprintf(out, "  bpl jump_cond_integer_skip_%d\n", label_count);
        fprintf(out, "  pla\n");
        fprintf(out, "  pla\n");
        fprintf(out, "  jmp %s\n", label.c_str());
      }
        else
      {
        fprintf(out, "  pla\n");
        fprintf(out, "  cmp 1,s\n");
        fprintf(out, "  bpl jump_cond_integer_skip_2_%d\n", label_count);
        fprintf(out, "  pla\n");
        fprintf(out, "  jmp %s\n", label.c_str());
      }
      break;
    case COND_GREATER_EQUAL:
      if(reverse == false)
      {
        fprintf(out, "  lda 3,s\n");
        fprintf(out, "  cmp 1,s\n");
        fprintf(out, "  bmi jump_cond_integer_skip_%d\n", label_count);
        fprintf(out, "  pla\n");
        fprintf(out, "  pla\n");
        fprintf(out, "  jmp %s\n", label.c_str());
      }
        else
      {
        fprintf(out, "  pla\n");
        fprintf(out, "  cmp 1,s\n");
        fprintf(out, "  bmi jump_cond_integer_skip_2_%d\n", label_count);
        fprintf(out, "  pla\n");
        fprintf(out, "  jmp %s\n", label.c_str());
      }
      break;
  }

  fprintf(out, "jump_cond_integer_skip_%d:\n", label_count);
  fprintf(out, "  pla\n");
  fprintf(out, "jump_cond_integer_skip_2_%d:\n", label_count);
  fprintf(out, "  pla\n");

  label_count++;
  stack -= 2;

  return 0;
}

int W65816::jump_cond_integer(std::string &label, int cond, int const_val, int distance)
{
  bool reverse = false;

  fprintf(out, "; jump_cond_integer (const)\n");

  if (cond == COND_LESS_EQUAL)
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

  switch (cond)
  {
    case COND_EQUAL:
      fprintf(out, "  pla\n");
      fprintf(out, "  cmp #0x%04x\n", const_val);
      fprintf(out, "  bne jump_cond_integer_const_skip_2_%d\n", label_count);
      fprintf(out, "  jmp %s\n", label.c_str());
      break;
    case COND_NOT_EQUAL:
      fprintf(out, "  pla\n");
      fprintf(out, "  cmp #0x%04x\n", const_val);
      fprintf(out, "  beq jump_cond_integer_const_skip_2_%d\n", label_count);
      fprintf(out, "  jmp %s\n", label.c_str());
      break;
    case COND_LESS:
      if(reverse == false)
      {
        fprintf(out, "  pla\n");
        fprintf(out, "  cmp #0x%04x\n", const_val);
        fprintf(out, "  bpl jump_cond_integer_const_skip_2_%d\n", label_count);
        fprintf(out, "  jmp %s\n", label.c_str());
      }
        else
      {
        fprintf(out, "  lda #0x%04x\n", const_val);
        fprintf(out, "  cmp 1,s\n");
        fprintf(out, "  bpl jump_cond_integer_const_skip_%d\n", label_count);
        fprintf(out, "  pla\n");
        fprintf(out, "  jmp %s\n", label.c_str());
      }
      break;
    case COND_GREATER_EQUAL:
      if(reverse == false)
      {
        fprintf(out, "  pla\n");
        fprintf(out, "  cmp #0x%04x\n", const_val);
        fprintf(out, "  bmi jump_cond_integer_const_skip_2_%d\n", label_count);
        fprintf(out, "  jmp %s\n", label.c_str());
      }
        else
      {
        fprintf(out, "  lda #0x%04x\n", const_val);
        fprintf(out, "  cmp 1,s\n");
        fprintf(out, "  bmi jump_cond_integer_const_skip_%d\n", label_count);
        fprintf(out, "  pla\n");
        fprintf(out, "  jmp %s\n", label.c_str());
      }
      break;
  }

  fprintf(out, "jump_cond_integer_const_skip_%d:\n", label_count);
  fprintf(out, "  pla\n");
  fprintf(out, "jump_cond_integer_const_skip_2_%d:\n", label_count);

  label_count++;
  stack--;

  return 0;
}

int W65816::ternary(int cond, int value_true, int value_false)
{
  return -1;
}

int W65816::ternary(int cond, int compare, int value_true, int value_false)
{
  return -1;
}

int W65816::return_local(int index, int local_count)
{
  fprintf(out, "; return_local\n");
  fprintf(out, "  lda locals\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #%d\n", LOCALS(index));
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda (address)\n");
  fprintf(out, "  sta result\n");
  fprintf(out, "  lda locals\n");
  fprintf(out, "  tcs\n");

  if (!is_main)
  {
    fprintf(out, "  pla\n");
    fprintf(out, "  sta locals\n");
  }
 
  restore_return();
  fprintf(out, "  rts\n");

  return 0;
}

int W65816::return_integer(int local_count)
{
  fprintf(out, "; return_integer\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result\n");
  stack--;

  fprintf(out, "  lda locals\n");
  fprintf(out, "  tcs\n");

  if (!is_main)
  {
    fprintf(out, "  pla\n");
    fprintf(out, "  sta locals\n");
  }

  restore_return();
  fprintf(out, "  rts\n");

  return 0;

}

int W65816::return_void(int local_count)
{
  fprintf(out, "; return_void\n");
  fprintf(out, "  lda locals\n");
  fprintf(out, "  tcs\n");

  if (!is_main)
  {
    fprintf(out, "  pla\n");
    fprintf(out, "  sta locals\n");
    restore_return();
    fprintf(out, "  rts\n");
  }
  else
  {
    // exit to monitor/operating system
    fprintf(out, "  brk\n");
  }

  return 0;
}

int W65816::jump(std::string &name, int distance)
{
  fprintf(out, "  jmp %s\n", name.c_str());

  return 0;
}

int W65816::call(std::string &name)
{
  fprintf(out, "; call\n");
  fprintf(out, "  jsr %s\n", name.c_str());

  return 0;
}

int W65816::invoke_static_method(const char *name, int params, int is_void)
{
  int local;
  int stack_vars = stack;

  // printf("invoke_static_method() name=%s params=%d is_void=%d\n", name, params, is_void);

  fprintf(out, "; invoke_static_method\n");
  fprintf(out, "  tsc\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  adc #2\n");
  fprintf(out, "  tay\n");

  local = (params * -2);

  while(local != 0)
  {
    if(stack_vars > 0)
    {
      fprintf(out, "; param\n");
      fprintf(out, "  tya\n");
      fprintf(out, "  clc\n");
      fprintf(out, "  adc #%d\n", (stack - stack_vars) * 2);
      fprintf(out, "  sta address\n");
      fprintf(out, "  lda (address)\n");
      fprintf(out, "  pha\n");

      fprintf(out, "  tya\n");
      fprintf(out, "  sec\n");
      fprintf(out, "  sbc #%d\n", -local + 2);
      fprintf(out, "  sta address\n");
      fprintf(out, "  pla\n");
      fprintf(out, "  sta (address)\n");

      stack_vars--;
    }

    local += 2;
  }

  fprintf(out, "  jsr %s\n", name);

  if((stack - stack_vars) > 0)
  {
    fprintf(out, "  tsc\n");
    fprintf(out, "  clc\n");
    fprintf(out, "  adc #%d\n", (stack - stack_vars) * 2);
    fprintf(out, "  tcs\n");

    params -= (stack - stack_vars);
  }

  if(!is_void)
  {
    fprintf(out, "  lda result\n");
    fprintf(out, "  pha\n");
    stack++;
  }

  return 0;
}

int W65816::put_static(std::string &name, int index)
{
  fprintf(out, "  lda %s\n", name.c_str());
  fprintf(out, "  pha\n");
  stack++;

  return 0;
}

int W65816::get_static(std::string &name, int index)
{
  fprintf(out, "  lda %s\n", name.c_str());
  fprintf(out, "  pha\n");
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

int W65816::insert_array(std::string &name, int32_t *data, int len, uint8_t type)
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

int W65816::insert_string(std::string &name, uint8_t *bytes, int len)
{
  fprintf(out, "dw %d\n", len);
  return insert_utf8(name, bytes, len);
}

int W65816::push_array_length()
{
  need_push_array_length = 1;
  fprintf(out, "  jsr push_array_length\n");

  return 0;
}

int W65816::push_array_length(std::string &name, int field_id)
{
  need_push_array_length2 = 1;
  fprintf(out, "  lda %s\n", name.c_str());
  fprintf(out, "  jsr push_array_length2\n");
  stack++;

  return 0;
}

int W65816::array_read_byte()
{
  need_array_byte_support = 1;

  fprintf(out, "; array_read_byte\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  tay\n");
  fprintf(out, "  lda (1,s),y\n");
  fprintf(out, "  and #0xff\n");
  fprintf(out, "  eor #0x80\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #0x80\n");
  fprintf(out, "  tay\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tya\n");
  fprintf(out, "  pha\n");
  stack--;

  return 0;
}

int W65816::array_read_short()
{
  return array_read_int();
}

int W65816::array_read_int()
{
  need_array_int_support = 1;

  fprintf(out, "; array_read_int\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  tay\n");
  fprintf(out, "  lda (1,s),y\n");
  fprintf(out, "  tay\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tya\n");
  fprintf(out, "  pha\n");
  stack--;

  return 0;
}

int W65816::array_read_byte(std::string &name, int field_id)
{
  return -1;
}

int W65816::array_read_short(std::string &name, int field_id)
{
  return array_read_int(name, field_id);
}

int W65816::array_read_int(std::string &name, int field_id)
{
  return -1;
}

int W65816::array_write_byte()
{
  need_array_byte_support = 1;

  fprintf(out, "; array_write_byte\n");
  fprintf(out, "  lda 3,s\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  tay\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta (3,s),y\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  pla\n");
  stack -= 3;

  return 0;
}

int W65816::array_write_short()
{
  return array_write_int();
}

int W65816::array_write_int()
{
  need_array_int_support = 1;

  fprintf(out, "; array_write_int\n");
  fprintf(out, "  lda 3,s\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  tay\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta (3,s),y\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  pla\n");
  stack -= 3;

  return 0;
}

int W65816::array_write_byte(std::string &name, int field_id)
{
  return -1;
}

int W65816::array_write_short(std::string &name, int field_id)
{
  return array_write_int(name, field_id);
}

int W65816::array_write_int(std::string &name, int field_id)
{
  return -1;
} 

void W65816::insert_mul_integer()
{
#if 1
  // Booth's algorithm
  fprintf(out, "mul_integer:\n");
  save_return();
  fprintf(out, "  pla\n");
  fprintf(out, "  sta value2\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta value1\n");
  fprintf(out, "  stz value4\n");
  fprintf(out, "  stz result\n");
  fprintf(out, "  stz value3\n");
  fprintf(out, "  bit value1\n");
  fprintf(out, "  bpl mul_integer_2\n");
  fprintf(out, "  lda #-1\n");
  fprintf(out, "  sta value3\n");
  fprintf(out, "  jmp mul_integer_2\n");
  fprintf(out, "mul_integer_6:\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda value1\n");
  fprintf(out, "  adc result\n");
  fprintf(out, "  sta result\n");
  fprintf(out, "  lda value3\n");
  fprintf(out, "  adc value4\n");
  fprintf(out, "  sta value4\n");
  fprintf(out, "mul_integer_1:\n");
  fprintf(out, "  asl value1\n");
  fprintf(out, "  rol value3\n");
  fprintf(out, "mul_integer_2:\n");
  fprintf(out, "  bit value2\n");
  fprintf(out, "  ror value2\n");
  fprintf(out, "  bcs mul_integer_5\n");
  fprintf(out, "  bne mul_integer_1\n");
  fprintf(out, "  jmp mul_integer_4\n");
  fprintf(out, "mul_integer_5:\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  lda result\n");
  fprintf(out, "  sbc value1\n");
  fprintf(out, "  sta result\n");
  fprintf(out, "  lda value4\n");
  fprintf(out, "  sbc value3\n");
  fprintf(out, "  sta value4\n");
  fprintf(out, "mul_integer_3:\n");
  fprintf(out, "  asl value1\n");
  fprintf(out, "  rol value3\n");
  fprintf(out, "  bit value2\n");
  fprintf(out, "  ror value2\n");
  fprintf(out, "  bcc mul_integer_6\n");
  fprintf(out, "  lda #-1\n");
  fprintf(out, "  cmp value2\n");
  fprintf(out, "  bne mul_integer_3\n");
  fprintf(out, "mul_integer_4:\n");
  fprintf(out, "  lda result\n");
  fprintf(out, "  pha\n");
  restore_return();
  fprintf(out, "  rts\n");
#else
  // based on example in WDC's manual
  fprintf(out, "mul_integer:\n");
  save_return();
  fprintf(out, "  pla\n");
  fprintf(out, "  sta value2\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta value1\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "mul_integer_1:\n");
  fprintf(out, "  ldx value1\n");
  fprintf(out, "  beq mul_integer_done\n");
  fprintf(out, "  lsr value1\n");
  fprintf(out, "  bcc mul_integer_2\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  adc value2\n");
  fprintf(out, "mul_integer_2:\n");
  fprintf(out, "  asl value2\n");
  fprintf(out, "  bra mul_integer_1\n");
  fprintf(out, "mul_integer_done:\n");
  fprintf(out, "  pha\n");
  restore_return();
  fprintf(out, "  rts\n");
#endif
}

void W65816::insert_div_integer()
{
  // based on example in WDC's manual
  fprintf(out, "div_integer:\n");
  save_return();
  fprintf(out, "  pla\n");
  fprintf(out, "  tay\n");
  fprintf(out, "  pla\n");
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
  fprintf(out, "  pha\n");
  restore_return();
  fprintf(out, "  rts\n");
}

void W65816::insert_push_array_length()
{
  fprintf(out, "push_array_length:\n");
  save_return();
  fprintf(out, "  pla\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #2\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda (address)\n");
  fprintf(out, "  pha\n");
  restore_return();
  fprintf(out, "  rts\n");
}

void W65816::insert_push_array_length2()
{
  fprintf(out, "push_array_length2:\n");
  save_return();
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #2\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda (address)\n");
  fprintf(out, "  pha\n");
  restore_return();
  fprintf(out, "  rts\n");
}

void W65816::insert_array_byte_support()
{
  // new_array_byte
  fprintf(out, "new_array_byte:\n");
  save_return();
  fprintf(out, "  pla\n");
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
  fprintf(out, "  pha\n");
  restore_return();
  fprintf(out, "  rts\n");

  // array_read_byte
/*
  fprintf(out, "array_read_byte:\n");
  save_return();
  fprintf(out, "  clc\n");
  fprintf(out, "  lda value2\n");
  fprintf(out, "  adc value1\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda (address)\n");
  fprintf(out, "  eor #0x80\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #0x80\n");
  fprintf(out, "  pha\n");
  restore_return();
  fprintf(out, "  rts\n");

  // array_read_byte2
  fprintf(out, "array_read_byte2:\n");
  save_return();
  fprintf(out, "  pla\n");
  fprintf(out, "  sta value1\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda address\n");
  fprintf(out, "  adc value1\n");
  fprintf(out, "  lda (address)\n");
  fprintf(out, "  eor #128\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #128\n");
  fprintf(out, "  pha\n");
  restore_return();
  fprintf(out, "  rts\n");

  // array_write_byte
  fprintf(out, "array_write_byte:\n");
  save_return();
  fprintf(out, "  clc\n");
  fprintf(out, "  lda value3\n");
  fprintf(out, "  adc value2\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda value1\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta (address)\n");
  fprintf(out, "  rep #0x30\n");
  restore_return();
  fprintf(out, "  rts\n");
*/
}

void W65816::insert_array_int_support()
{
  // new_array_int
  fprintf(out, "new_array_int:\n");
  save_return();
  fprintf(out, "  pla\n");
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
  fprintf(out, "  pha\n");
  restore_return();
  fprintf(out, "  rts\n");


/*
  fprintf(out, "array_read_int:\n");
  save_return();
  fprintf(out, "  pla\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  tay\n");
  fprintf(out, "  lda (1,s),y\n");
  fprintf(out, "  tay\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tya\n");
  fprintf(out, "  pha\n");
  restore_return();
  fprintf(out, "  rts\n");
*/

  // array_read_int
/*
  fprintf(out, "array_read_int:\n");
  save_return();
  fprintf(out, "  asl value1\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda value2\n");
  fprintf(out, "  adc value1\n");
  fprintf(out, "  sta value2\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda (address)\n");
  fprintf(out, "  pha\n");
  restore_return();
  fprintf(out, "  rts\n");
*/

  // array_read_int2
/*
  fprintf(out, "array_read_int2:\n");
  save_return();
  fprintf(out, "  pla\n");
  fprintf(out, "  sta value1\n");
  fprintf(out, "  asl value1\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda address\n");
  fprintf(out, "  adc value1\n");
  fprintf(out, "  lda (address)\n");
  fprintf(out, "  pha\n");
  restore_return();
  fprintf(out, "  rts\n");
*/

  // array_write_int
/*
  fprintf(out, "array_write_int:\n");
  save_return();
  fprintf(out, "  lda 3,s\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  tay\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta (3,s),y\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  pla\n");
  restore_return();
  fprintf(out, "  rts\n");
*/
/*
  fprintf(out, "array_write_int:\n");
  save_return();
  fprintf(out, "  asl value2\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda value3\n");
  fprintf(out, "  adc value2\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda value1\n");
  fprintf(out, "  sta (address)\n");
  restore_return();
  fprintf(out, "  rts\n");
*/
}

// Memory API
int W65816::memory_read8_I()
{
  fprintf(out, "; memory_read8\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda (address)\n");
  fprintf(out, "  and #0xff\n");
  fprintf(out, "  eor #0x80\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #0x80\n");
  fprintf(out, "  pha\n");

  return 0;
}

int W65816::memory_write8_IB()
{
  fprintf(out, "; memory_write8\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tax\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  txa\n");
  fprintf(out, "  sta (address)\n");
  fprintf(out, "  rep #0x30\n");
  stack -= 2;

  return 0;
}

int W65816::memory_read16_I()
{
  fprintf(out, "; memory_read16\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda (address)\n");
  fprintf(out, "  pha\n");

  return 0;
}

int W65816::memory_write16_IS()
{
  fprintf(out, "; memory_write16\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tax\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  txa\n");
  fprintf(out, "  sta (address)\n");
  stack -= 2;

  return 0;
}

// called at the beginning of a method
void W65816::save_return()
{
  fprintf(out, "; save_return\n");
  fprintf(out, "  ldx return_index\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  stx return_index\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta return_stack,x\n");
}

// called just before "rts" at the end of a method
void W65816::restore_return()
{
  fprintf(out, "; restore_return\n");
  fprintf(out, "  ldx return_index\n");
  fprintf(out, "  lda return_stack,x\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  dec return_index\n");
  fprintf(out, "  dec return_index\n");
}

