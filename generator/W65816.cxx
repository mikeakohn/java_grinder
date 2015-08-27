/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 * W65816 written by Joe Davisson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "W65816.h"

#define LOCALS(a) (a * 2)

W65816::W65816() :
  stack(0),
  is_main(0),
  need_mul_integer(0),
  need_div_integer(0)
{
}

W65816::~W65816()
{
  if(need_mul_integer) { insert_mul_integer(); }
  if(need_div_integer) { insert_mul_integer(); }

//FIXME for testing
  fprintf(out, "; exit to monitor\n");
  fprintf(out, "  lda #0xABCD\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  rtl\n");
}

int W65816::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".65816\n");

  // stack location
  fprintf(out, "stack equ 0x100\n");

  // ram start
  fprintf(out, "ram_start equ 0x7000\n");
  fprintf(out, "heap_ptr equ ram_start\n");

  // points to locals
  fprintf(out, "locals equ 0x00\n");

  // temp variables
  fprintf(out, "result equ 0x02\n");
  fprintf(out, "remainder equ 0x04\n");
  fprintf(out, "length equ 0x06\n");
  fprintf(out, "value1 equ 0x08\n");
  fprintf(out, "value2 equ 0x10\n");
  fprintf(out, "value3 equ 0x12\n");

  // start
  fprintf(out, ".org 0x2000\n");
  fprintf(out, "; change to 16-bit mode\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  xce\n");
  fprintf(out, "; all 16-bit registers\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "; set up stack\n");
  fprintf(out, "  lda #0x1FF\n");
  fprintf(out, "  tcs\n");
  fprintf(out, "; set up direct-page\n");
  fprintf(out, "  pea 0x0000\n");

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

int W65816::insert_field_init_boolean(char *name, int index, int value)
{
  fprintf(out, "; insert_field_init_boolean\n");
  fprintf(out, "  lda #%d\n", value);
  fprintf(out, "  sta #%s\n", name);

  return 0;
}

int W65816::insert_field_init_byte(char *name, int index, int value)
{
  if (value < -128 || value > 255) { return -1; }
  int16_t n = value;
  uint16_t v = (n & 0xffff);

  fprintf(out, "; insert_field_init_byte\n");
  fprintf(out, "  lda #%d\n", (uint8_t)v);
  fprintf(out, "  sta %s\n", name);

  return 0;
}

int W65816::insert_field_init_short(char *name, int index, int value)
{
  if (value < -32768 || value > 65535) { return -1; }

  fprintf(out, "; insert_field_init_short\n");
  fprintf(out, "  lda #%d\n", (uint16_t)value);
  fprintf(out, "  sta %s\n", name);

  return 0;
}

int W65816::insert_field_init_int(char *name, int index, int value)
{
  return insert_field_init_short(name, index, value);
}

int W65816::insert_field_init(char *name, int index)
{
  fprintf(out, "; insert_field_init\n");
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
    fprintf(out, "  pha\n");
  }

  fprintf(out, "  tsc\n");
  fprintf(out, "  sta locals\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #0x%04x\n", local_count);
  fprintf(out, "  tcs\n");
}

void W65816::method_end(int local_count)
{
  fprintf(out, "\n");
}

int W65816::push_integer(int32_t n)
{
  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);

    return -1;
  }

  uint16_t value = (n & 0xffff);

  fprintf(out, "; push_integer\n");
  fprintf(out, "  lda #0x%04x\n", value);
  fprintf(out, "  pha\n");
  stack++;

  return 0;
}

int W65816::push_integer_local(int index)
{
  fprintf(out, "; push_integer_local\n");
  fprintf(out, "  ldx locals\n");
  fprintf(out, "  lda #stack - %d,x\n", LOCALS(index));
  fprintf(out, "  pha\n");
  stack++;

  return 0;
}

int W65816::push_ref_static(const char *name, int index)
{
  fprintf(out, "; push_ref_static\n");
  fprintf(out, "  lda #_%s\n", name);
  fprintf(out, "  pha\n");
  stack++;

  return 0;
}

int W65816::push_ref_local(int index)
{
  return push_integer_local(index);
}

int W65816::push_fake()
{
  return -1;
}

int W65816::push_long(int64_t n)
{
  return push_integer((int32_t)n);
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

int W65816::push_byte(int8_t b)
{
  int16_t n = b;
  uint16_t value = (n & 0xffff);

  fprintf(out, "; push_byte\n");
  fprintf(out, "  lda #0x%04x\n", value);
  fprintf(out, "  pha\n");
  stack++;

  return 0;
}

int W65816::push_short(int16_t s)
{
  uint16_t value = (s & 0xffff);

  fprintf(out, "; push_short\n");
  fprintf(out, "  lda #0x%04x\n", value);
  fprintf(out, "  pha\n");
  stack++;

  return 0;
}

int W65816::push_ref(char *name)
{
  fprintf(out, "; push_ref\n");
  fprintf(out, "  lda %s\n", name);
  fprintf(out, "  pha\n");
  stack++;

  return 0;
}

int W65816::pop_integer_local(int index)
{
  fprintf(out, "; pop_integer_local\n");
  fprintf(out, "  ldx locals\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta stack - %d,x\n", LOCALS(index));
  stack--;

  return 0;
}

int W65816::pop_ref_local(int index)
{
  return pop_integer_local(index);
}

int W65816::pop()
{
  fprintf(out, "; pop\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result\n");
  stack--;

  return 0;
}

int W65816::dup()
{
  fprintf(out, "; dup\n");
  fprintf(out, "  tsx\n");
  fprintf(out, "  lda stack,x\n");
  fprintf(out, "  pha\n");

  stack++;

  return 0;
}

int W65816::dup2()
{
  printf("Need to implement dup2()\n");
  return -1;
}

int W65816::swap()
{
  fprintf(out, "; swap\n");
  fprintf(out, "  tsx\n");
  // x ^= y
  fprintf(out, "  lda stack,x\n");
  fprintf(out, "  eor stack - 1,x\n");
  fprintf(out, "  sta stack,x\n");
  // y ^= x
  fprintf(out, "  lda stack - 1,x\n");
  fprintf(out, "  eor stack,x\n");
  fprintf(out, "  sta stack - 1,x\n");
  // x ^= y
  fprintf(out, "  lda stack,x\n");
  fprintf(out, "  eor stack - 1,x\n");
  fprintf(out, "  sta stack,x\n");

  return 0;
/*
  fprintf(out, "; swap\n");
  fprintf(out, "  tsx\n");
  fprintf(out, "  lda stack,x\n");
  fprintf(out, "  sta value,x\n");

  fprintf(out, "  lda stack - 1,x\n");
  fprintf(out, "  sta stack,x\n");

  fprintf(out, "  lda value1\n");
  fprintf(out, "  sta stack - 1,x\n");

  return 0;
*/
}

int W65816::add_integer()
{
  fprintf(out, "; add_integer\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  adc result\n");
  fprintf(out, "  sta result\n");
  fprintf(out, "  pha\n");
  stack--;

  return 0;
}

int W65816::add_integer(int const_val)
{
  return -1;
}

int W65816::sub_integer()
{
  fprintf(out, "; sub_integer\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc result\n");
  fprintf(out, "  sta result\n");
  fprintf(out, "  pha\n");
  stack--;

  return 0;
}

int W65816::sub_integer(int const_val)
{
  return -1;
}

int W65816::mul_integer()
{
//  fprintf(out, "  jsr mul_integer\n");
//  stack--;
  return -1;
}

int W65816::div_integer()
{
//  fprintf(out, "  jsr div_integer\n");
//  stack--;
  return -1;
}

int W65816::mod_integer()
{
  return -1;
}

int W65816::neg_integer()
{
  fprintf(out, "; neg_integer\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result\n");

  fprintf(out, "  sec\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sbc result\n");
  fprintf(out, "  sta result\n");
  fprintf(out, "  pha\n");

  return 0;
}

int W65816::shift_left_integer()
{
  return -1;
}

int W65816::shift_left_integer(int const_val)
{
  return -1;
}

int W65816::shift_right_integer()
{
  return -1;
}

int W65816::shift_right_integer(int num)
{
  return -1;
}

int W65816::shift_right_uinteger()
{
  return -1;
}

int W65816::shift_right_uinteger(int num)
{
  return -1;
}

int W65816::and_integer()
{
  fprintf(out, "; and_integer\n");
  stack--;
  return 0;
}

int W65816::and_integer(int num)
{
  return -1;
}

int W65816::or_integer()
{
  fprintf(out, "; or_integer\n");
  stack--;
  return 0;
}

int W65816::or_integer(int num)
{
  return -1;
}

int W65816::xor_integer()
{
  fprintf(out, "; xor_integer\n");
  stack--;
  return 0;
}

int W65816::xor_integer(int num)
{
  return -1;
}

int W65816::inc_integer(int index, int num)
{
  uint16_t value = num & 0xffff;

  fprintf(out, "; inc_integer num = %d\n", num);
  fprintf(out, "  ldx locals\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda stack - %d,x\n", LOCALS(index));
  fprintf(out, "  adc #0x%04x\n", value);
  fprintf(out, "  sta stack - %d,x\n", LOCALS(index));

  return 0;
}

int W65816::integer_to_byte()
{
  // (x ^ 128) - 128
  fprintf(out, "integer_to_byte:\n");
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

int W65816::jump_cond(const char *label, int cond, int distance)
{
  return -1;
}

int W65816::jump_cond_integer(const char *label, int cond, int distance)
{
  return -1;
}

int W65816::return_local(int index, int local_count)
{
  fprintf(out, "; return_local\n");
  fprintf(out, "  ldx locals\n");
  fprintf(out, "  lda stack - %d,x\n", LOCALS(index));
  fprintf(out, "  sta result\n");

  fprintf(out, "  txs\n");

  if (!is_main)
  {
    fprintf(out, "  pla\n");
    fprintf(out, "  sta locals\n");
  }

  fprintf(out, "  rts\n");

  return 0;
}

int W65816::return_integer(int local_count)
{
  fprintf(out, "; return_integer\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result\n");
  stack--;

  fprintf(out, "  ldx locals\n");
  fprintf(out, "  txs\n");

  if (!is_main)
  {
    fprintf(out, "  pla\n");
    fprintf(out, "  sta locals\n");
  }

  fprintf(out, "  rts\n");

  return 0;

}

int W65816::return_void(int local_count)
{
  fprintf(out, "; return_void\n");
  fprintf(out, "  ldx locals\n");
  fprintf(out, "  txs\n");

  if (!is_main)
  {
    fprintf(out, "  pla\n");
    fprintf(out, "  sta locals\n");
  }

  fprintf(out, "  rts\n");

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
  return -1;
}

int W65816::put_static(const char *name, int index)
{
  return -1;
}

int W65816::get_static(const char *name, int index)
{
  return -1;
}

int W65816::brk()
{
  return -1;
}

int W65816::new_array(uint8_t type)
{
  return -1;
}

int W65816::insert_array(const char *name, int32_t *data, int len, uint8_t type)
{
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

int W65816::insert_string(const char *name, uint8_t *bytes, int len)
{
  return -1;
}

int W65816::push_array_length()
{
  return -1;
}

int W65816::push_array_length(const char *name, int field_id)
{
  return -1;
}

int W65816::array_read_byte()
{
  return -1;
}

int W65816::array_read_short()
{
  return -1;
}

int W65816::array_read_int()
{
  return -1;
}

int W65816::array_read_byte(const char *name, int field_id)
{
  return -1;
}

int W65816::array_read_short(const char *name, int field_id)
{
  return -1;
}

int W65816::array_read_int(const char *name, int field_id)
{
  return -1;
}

int W65816::array_write_byte()
{
  return -1;
}

int W65816::array_write_short()
{
  return -1;
}

int W65816::array_write_int()
{
  return -1;
}

int W65816::array_write_byte(const char *name, int field_id)
{
  return -1;
}

int W65816::array_write_short(const char *name, int field_id)
{
  return -1;
}

int W65816::array_write_int(const char *name, int field_id)
{
  return -1;
}

// subroutines
void W65816::insert_mul_integer()
{
}

void W65816::insert_div_integer()
{
}

