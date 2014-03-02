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

//FIXME: large blocks of 6502 code should be placed into subroutines

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "M6502.h"

// ABI is:
// A
// X
// Y

#define LOCALS(a) ((a * 2) + 1)

#define POKE(dst) \
  fprintf(out, "  pla\n"); \
  fprintf(out, "  pla\n"); \
  fprintf(out, "  sta 0x%04x\n", dst)

M6502::M6502() :
  stack(0),
  is_main(0)
{

}

M6502::~M6502()
{
  // test that prints stack and heap on screen
  fprintf(out, "print:\n");
  fprintf(out, "  ldx #0\n");
  fprintf(out, "print_stack_loop:\n");
  fprintf(out, "  lda 0x100,x\n");
  fprintf(out, "  sta 1024,x\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  bne print_stack_loop\n");

  fprintf(out, "  ldx #0\n");
  fprintf(out, "print_heap_loop:\n");
  fprintf(out, "  lda heap_ptr,x\n");
  fprintf(out, "  sta 1344,x\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  bne print_heap_loop\n");
  fprintf(out, "print_end:\n");
  fprintf(out, "  jmp print_end\n");

  fprintf(out, "result:\n");
  fprintf(out, "dw 0\n");
  fprintf(out, "remainder:\n");
  fprintf(out, "dw 0\n");
  fprintf(out, "length:\n");
  fprintf(out, "dw 0\n");
  fprintf(out, "temp:\n");
  fprintf(out, "dw 0\n");
  fprintf(out, "value1:\n");
  fprintf(out, "dw 0\n");
  fprintf(out, "value2:\n");
  fprintf(out, "dw 0\n");
  fprintf(out, "value3:\n");
  fprintf(out, "dw 0\n");
  fprintf(out, "sprite_msb_set:\n");
  fprintf(out, "db 0\n");
  fprintf(out, "sprite_msb_clear:\n");
  fprintf(out, "db 0\n");
  fprintf(out, "sprite_x:\n");
  fprintf(out, "dw 0\n");
  fprintf(out, "sprite_y:\n");
  fprintf(out, "db 0\n");
  fprintf(out, "locals:\n");
  fprintf(out, "db 0\n");
}

int M6502::open(char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  // assumes c64 for now (basic loader)
  fprintf(out, ".65xx\n");
  fprintf(out, "ram_start equ 0x8000\n");
  fprintf(out, "heap_ptr equ ram_start\n");
  // for indirection
  fprintf(out, "address equ 251\n");

  fprintf(out, ".org 0x07ff\n\n");

  fprintf(out, "dw 0x0801\n");
  fprintf(out, "dw start\n");
  fprintf(out, "dw 2013\n");
  fprintf(out, "db 0x9e\n");
  fprintf(out, "db (((start / 1000) %% 10) + 0x30)\n");
  fprintf(out, "db (((start / 100) %% 10) + 0x30)\n");
  fprintf(out, "db (((start / 10) %% 10) + 0x30)\n");
  fprintf(out, "db (((start / 1) %% 10) + 0x30)\n");
  fprintf(out, "db ':'\n");
  fprintf(out, "db 0x8f\n");
  fprintf(out, "db \" MORTIS\"\n");
  fprintf(out, "db 0\n");
  fprintf(out, "dw 0\n\n");

  fprintf(out, "start:\n");
  fprintf(out, "  sei\n");
  fprintf(out, "  cld\n");
  fprintf(out, "  ldx #0xff\n");
  fprintf(out, "  txs\n\n");

  return 0;
}

int M6502::start_init()
{
  return 0;
}

int M6502::insert_static_field_define(const char *name, const char *type, int index)
{
  fprintf(out, "%s equ ram_start+%d\n", name, (index + 1) * 2);

  return 0;
}

int M6502::init_heap(int field_count)
{
  fprintf(out, "  ; Set up heap and static initializers\n");
  fprintf(out, "  lda #(ram_start+%d) & 0xff\n", (field_count + 1) * 2);
  fprintf(out, "  sta ram_start + 0\n");
  fprintf(out, "  lda #(ram_start+%d) >> 8\n", (field_count + 1) * 2);
  fprintf(out, "  sta ram_start + 1\n");

  return 0;
}

int M6502::insert_field_init_boolean(char *name, int index, int value)
{
  value = (value == 0) ? 0 : 1;
  fprintf(out, "  lda #%d\n", value & 0xff);
  fprintf(out, "  sta %s\n", name + 0);
  fprintf(out, "  lda #%d\n", value >> 8);
  fprintf(out, "  sta %s\n", name + 1);

  return 0;
}

int M6502::insert_field_init_byte(char *name, int index, int value)
{
  if (value < -128 || value > 255) { return -1; }

  fprintf(out, "  lda #%d\n", (int8_t)value & 0xff);
  fprintf(out, "  sta %s\n", name + 0);
  fprintf(out, "  lda #%d\n", (int8_t)value >> 8);
  fprintf(out, "  sta %s\n", name + 1);

  return 0;
}

int M6502::insert_field_init_short(char *name, int index, int value)
{
  if (value < -32768 || value > 65535) { return -1; }

  fprintf(out, "  lda #%d\n", value & 0xff);
  fprintf(out, "  sta %s\n", name + 0);
  fprintf(out, "  lda #%d\n", value >> 8);
  fprintf(out, "  sta %s\n", name + 1);

  return 0;
}

int M6502::insert_field_init_int(char *name, int index, int value)
{
  return insert_field_init_short(name, index, value);
}

int M6502::insert_field_init(char *name, int index)
{
  fprintf(out, "  lda #_%s & 0xff\n", name);
  fprintf(out, "  sta %s\n", name + 0);
  fprintf(out, "  lda #_%s >> 8\n", name);
  fprintf(out, "  sta %s\n", name + 1);

  return 0;
}

void M6502::method_start(int local_count, const char *name)
{
  stack = 0;

  is_main = (strcmp(name, "main") == 0) ? 1 : 0;

  fprintf(out, "%s:\n", name);

  // main() function goes here
  if (!is_main)
  {
    fprintf(out, "  lda locals\n");
    fprintf(out, "  pha\n");
    fprintf(out, "  pha\n");
  }

  fprintf(out, "  tsx\n");
  fprintf(out, "  stx locals\n");

  fprintf(out, "  txa\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sbc #0x%02x\n", local_count * 2);
  fprintf(out, "  tax\n");
  fprintf(out, "  txs\n");
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
  fprintf(out, "  pha\n");
  fprintf(out, "  lda #0x%02x\n", value >> 8);
  fprintf(out, "  pha\n");
  stack++;

  return 0;
}

int M6502::push_integer_local(int index)
{
  fprintf(out, "; push_integer_local\n");
  fprintf(out, "  ldx locals\n");
  fprintf(out, "  lda 0x101 - %d,x\n", LOCALS(index));
  fprintf(out, "  pha\n");
  fprintf(out, "  lda 0x100 - %d,x\n", LOCALS(index));
  fprintf(out, "  pha\n");
  stack++;

  return 0;
}

int M6502::push_ref_local(int index)
{
  return push_integer_local(index);
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
  uint16_t value = (b & 0xffff);

  fprintf(out, "; push_byte\n");
  fprintf(out, "  lda #0x%02x\n", value & 0xff);
  fprintf(out, "  pha\n");
  fprintf(out, "  lda #0x%02x\n", value >> 8);
  fprintf(out, "  pha\n");
  stack++;

  return 0;
}

int M6502::push_short(int16_t s)
{
  uint16_t value = (s & 0xffff);

  fprintf(out, "; push_short\n");
  fprintf(out, "  lda #0x%02x\n", value & 0xff);
  fprintf(out, "  pha\n");
  fprintf(out, "  lda #0x%02x\n", value >> 8);
  fprintf(out, "  pha\n");
  stack++;

  return 0;
}

int M6502::pop_integer_local(int index)
{
  fprintf(out, "; pop_integer_local\n");
  fprintf(out, "  ldx locals\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta 0x100 - %d,x\n", LOCALS(index));
  fprintf(out, "  pla\n");
  fprintf(out, "  sta 0x101 - %d,x\n", LOCALS(index));
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
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 0\n");
  stack--;

  return 0;
}

int M6502::dup()
{
  fprintf(out, "  tsx\n");
  fprintf(out, "  lda 0x101,x\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda 0x100,x\n");
  fprintf(out, "  pha\n");
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
//FIXME untested
  fprintf(out, "; swap\n");
  fprintf(out, "  tsx\n");
  fprintf(out, "  lda 0x101,x\n");
  fprintf(out, "  sta temp + 0,x\n");
  fprintf(out, "  lda 0x100,x\n");
  fprintf(out, "  sta temp + 1,x\n");

  fprintf(out, "  lda 0x103,x\n");
  fprintf(out, "  sta 0x101,x\n");
  fprintf(out, "  lda 0x102,x\n");
  fprintf(out, "  sta 0x100,x\n");

  fprintf(out, "  lda temp + 0\n");
  fprintf(out, "  sta 0x103,x\n");
  fprintf(out, "  lda temp + 1\n");
  fprintf(out, "  sta 0x102,x\n");

  return 0;
}

int M6502::add_integer()
{
  fprintf(out, "; add_integers\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tax\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  adc result + 0\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  txa\n");
  fprintf(out, "  adc result + 1\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pha\n");

  return 0;
}

int M6502::sub_integer()
{
  fprintf(out, "; sub_integers\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tax\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sbc result + 0\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  txa\n");
  fprintf(out, "  sbc result + 1\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pha\n");

  return 0;
}

int M6502::mul_integer()
{
  fprintf(out, "; mul_integers\n");
  // load values
  fprintf(out, "  pla\n");
  fprintf(out, "  sta value2 + 1\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta value2 + 0\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta value1 + 1\n");
  fprintf(out, "  pla\n");
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
  fprintf(out, "  bcc #19\n");

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
  fprintf(out, "  bne #-36\n");

  // push result
  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda result + 1\n");
  fprintf(out, "  pha\n");

  return 0;
}

// unsigned only
int M6502::div_integer()
{
  fprintf(out, "; div_integers\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta value2 + 1\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta value2 + 0\n");

  fprintf(out, "  pla\n");
  fprintf(out, "  sta value1 + 1\n");
  fprintf(out, "  pla\n");
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
  fprintf(out, "  bcc #9\n");

  fprintf(out, "  sta remainder + 1\n");
  fprintf(out, "  sty remainder + 0\n");
  fprintf(out, "  inc value1 + 0\n");

  // skip
  fprintf(out, "  dex\n");
  fprintf(out, "  bne #-40\n");

  // push result
  fprintf(out, "  lda value1 + 0\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda value1 + 1\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pha\n");

  return 0;
}

// unsigned only
int M6502::mod_integer()
{
  // div_integer has the remainder built-in
  div_integer();

  fprintf(out, "  lda remainder + 0\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda remainder + 1\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pha\n");

  return 0;
}

int M6502::neg_integer()
{
  fprintf(out, "; neg_integer\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 0\n");

  fprintf(out, "  sec\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sbc result + 0\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sbc result + 1\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda result + 1\n");
  fprintf(out, "  pha\n");

  return 0;
}

int M6502::shift_left_integer()
{
  fprintf(out, "; shift_left_integer\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tax\n");

  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 0\n");

  fprintf(out, "  asl result + 0\n");
  fprintf(out, "  rol result + 1\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  bne #-9\n");
  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda result + 1\n");
  fprintf(out, "  pha\n");

  return 0;
}

int M6502::shift_right_integer()
{
  fprintf(out, "; shift_right_integer\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tax\n");

  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 0\n");

  fprintf(out, "  lda result + 1\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  ror result + 1\n");
  fprintf(out, "  ror result + 0\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  bne #-13\n");
  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda result + 1\n");
  fprintf(out, "  pha\n");

  return 0;
}

int M6502::shift_right_uinteger()
{
  fprintf(out, "; shift_right_uinteger\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tax\n");

  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 0\n");

  fprintf(out, "  lsr result + 1\n");
  fprintf(out, "  ror result + 0\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  bne #-9\n");
  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda result + 1\n");
  fprintf(out, "  pha\n");

  return 0;
}

int M6502::and_integer()
{
  fprintf(out, "; and_integers\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tax\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  and result + 0\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  txa\n");
  fprintf(out, "  and result + 1\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pha\n");

  return 0;
}

int M6502::or_integer()
{
  fprintf(out, "; and_integers\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tax\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  ora result + 0\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  txa\n");
  fprintf(out, "  ora result + 1\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pha\n");

  return 0;
}

int M6502::xor_integer()
{
  fprintf(out, "; and_integers\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tax\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  eor result + 0\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  txa\n");
  fprintf(out, "  eor result + 1\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pha\n");

  return 0;
}

int M6502::inc_integer(int index, int num)
{
  uint8_t lo = num & 0xff;
  uint8_t hi = lo > 0x7f ? 0xff : 0x00;

  fprintf(out, "; inc_integer num = %d\n", num);
  fprintf(out, "  ldx locals\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda 0x101 - %d,x\n", LOCALS(index));
  fprintf(out, "  adc #0x%02x\n", lo);
  fprintf(out, "  sta 0x101 - %d,x\n", LOCALS(index));
  fprintf(out, "  lda 0x100 - %d,x\n", LOCALS(index));
  fprintf(out, "  adc #0x%02x\n", hi);
  fprintf(out, "  sta 0x100 - %d,x\n", LOCALS(index));

  return 0;
}

int M6502::jump_cond(const char *label, int cond)
{
  bool reverse = false;

  if (stack > 0)
  {
    fprintf(out, "; jump_cond\n");
    fprintf(out, "  tsx\n");
    fprintf(out, "  txa\n");
    fprintf(out, "  clc\n");
    fprintf(out, "  adc #2\n");
    fprintf(out, "  tax\n");
    fprintf(out, "  txs\n");

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
        fprintf(out, "  lda 0x101 -2,x\n");
        fprintf(out, "  cmp #0\n");
        fprintf(out, "  bne #10\n");
        fprintf(out, "  lda 0x102 -2,x\n");
        fprintf(out, "  cmp #0\n");
        fprintf(out, "  bne #3\n");
        fprintf(out, "  jmp %s\n", label);
        break;
      case COND_NOT_EQUAL:
        fprintf(out, "  lda 0x101 -2,x\n");
        fprintf(out, "  cmp #0\n");
        fprintf(out, "  beq #10\n");
        fprintf(out, "  lda 0x102 -2,x\n");
        fprintf(out, "  cmp #0\n");
        fprintf(out, "  beq #3\n");
        fprintf(out, "  jmp %s\n", label);
        break;
      case COND_LESS:
        if(reverse == false)
        {
          fprintf(out, "  lda 0x102 -2,x\n");
          fprintf(out, "  cmp #0\n");
          fprintf(out, "  lda 0x101 -2,x\n");
          fprintf(out, "  sbc #0\n");
          fprintf(out, "  bvc #2\n");
          fprintf(out, "  eor #0x80\n");
          fprintf(out, "  bpl #3\n");
          fprintf(out, "  jmp %s\n", label);
        }
          else
        {
          fprintf(out, "  lda #0\n");
          fprintf(out, "  cmp 0x102 -2,x\n");
          fprintf(out, "  lda #0\n");
          fprintf(out, "  sbc 0x101 -2,x\n");
          fprintf(out, "  bvc #2\n");
          fprintf(out, "  eor #0x80\n");
          fprintf(out, "  bpl #3\n");
          fprintf(out, "  jmp %s\n", label);
        }
        break;
      case COND_GREATER_EQUAL:
        if(reverse == false)
        {
          fprintf(out, "  lda 0x102 -2,x\n");
          fprintf(out, "  cmp #0\n");
          fprintf(out, "  lda 0x101 -2,x\n");
          fprintf(out, "  sbc #0\n");
          fprintf(out, "  bvc #2\n");
          fprintf(out, "  eor #0x80\n");
          fprintf(out, "  bmi #3\n");
          fprintf(out, "  jmp %s\n", label);
        }
          else
        {
          fprintf(out, "  lda #0\n");
          fprintf(out, "  cmp 0x102 -2,x\n");
          fprintf(out, "  lda #0\n");
          fprintf(out, "  sbc 0x101 -2,x\n");
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
    fprintf(out, "  tsx\n");
    fprintf(out, "  txa\n");
    fprintf(out, "  clc\n");
    fprintf(out, "  adc #4\n");
    fprintf(out, "  tax\n");
    fprintf(out, "  txs\n");

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
        fprintf(out, "  lda 0x101 -2,x\n");
        fprintf(out, "  cmp 0x101 -4,x\n");
        fprintf(out, "  bne #11\n");
        fprintf(out, "  lda 0x102 -2,x\n");
        fprintf(out, "  cmp 0x102 -4,x\n");
        fprintf(out, "  bne #3\n");
        fprintf(out, "  jmp %s\n", label);
        break;
      case COND_NOT_EQUAL:
        fprintf(out, "  lda 0x101 -2,x\n");
        fprintf(out, "  cmp 0x101 -4,x\n");
        fprintf(out, "  beq #11\n");
        fprintf(out, "  lda 0x102 -2,x\n");
        fprintf(out, "  cmp 0x102 -4,x\n");
        fprintf(out, "  beq #3\n");
        fprintf(out, "  jmp %s\n", label);
        break;
      case COND_LESS:
        if(reverse == false)
        {
          fprintf(out, "  lda 0x102 -2,x\n");
          fprintf(out, "  cmp 0x102 -4,x\n");
          fprintf(out, "  lda 0x101 -2,x\n");
          fprintf(out, "  sbc 0x101 -4,x\n");
          fprintf(out, "  bvc #2\n");
          fprintf(out, "  eor #0x80\n");
          fprintf(out, "  bpl #3\n");
          fprintf(out, "  jmp %s\n", label);
        }
          else
        {
          fprintf(out, "  lda 0x102 -4,x\n");
          fprintf(out, "  cmp 0x102 -2,x\n");
          fprintf(out, "  lda 0x101 -4,x\n");
          fprintf(out, "  sbc 0x101 -2,x\n");
          fprintf(out, "  bvc #2\n");
          fprintf(out, "  eor #0x80\n");
          fprintf(out, "  bpl #3\n");
          fprintf(out, "  jmp %s\n", label);
        }
        break;
      case COND_GREATER_EQUAL:
        if(reverse == false)
        {
          fprintf(out, "  lda 0x102 -2,x\n");
          fprintf(out, "  cmp 0x102 -4,x\n");
          fprintf(out, "  lda 0x101 -2,x\n");
          fprintf(out, "  sbc 0x101 -4,x\n");
          fprintf(out, "  bvc #2\n");
          fprintf(out, "  eor #0x80\n");
          fprintf(out, "  bmi #3\n");
          fprintf(out, "  jmp %s\n", label);
        }
          else
        {
          fprintf(out, "  lda 0x102 -4,x\n");
          fprintf(out, "  cmp 0x102 -2,x\n");
          fprintf(out, "  lda 0x101 -4,x\n");
          fprintf(out, "  sbc 0x101 -2,x\n");
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
  fprintf(out, "  lda 0x101 - %d, x\n", LOCALS(index));
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  lda 0x100 - %d, x\n", LOCALS(index));
  fprintf(out, "  sta result + 1\n");

  fprintf(out, "  ldx locals\n");
  fprintf(out, "  txs\n");

  if (!is_main)
  {
    fprintf(out, "  pla\n");
    fprintf(out, "  pla\n");
    fprintf(out, "  sta locals\n");
  }

  fprintf(out, "  rts\n");

  return 0;
}

int M6502::return_integer(int local_count)
{
  fprintf(out, "; return_integer\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 0\n");
  stack--;

  fprintf(out, "  ldx locals\n");
  fprintf(out, "  txs\n");

  if (!is_main)
  {
    fprintf(out, "  pla\n");
    fprintf(out, "  pla\n");
    fprintf(out, "  sta locals\n");
  }

  fprintf(out, "  rts\n");

  return 0;
}

int M6502::return_void(int local_count)
{
  fprintf(out, "; return_void\n");
  fprintf(out, "  ldx locals\n");
  fprintf(out, "  txs\n");

  if (!is_main)
  {
    fprintf(out, "  pla\n");
    fprintf(out, "  pla\n");
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

  local = (params * -2);
  while(local != 0)
  {
    // Copy parameters onto the stack so they are local variables in
    // the called method.  Start with -4 because the return address will
    // be at 0 and r12 will be at 2.

    if (stack_vars > 0)
    {
      fprintf(out, "  tsx\n");
      fprintf(out, "  lda 0x102 + %d,x\n", (stack - stack_vars) * 2);
      fprintf(out, "  sta 0x102 %d,x\n", local-4);
      fprintf(out, "  lda 0x101 + %d,x\n", (stack - stack_vars) * 2);
      fprintf(out, "  sta 0x101 %d,x\n", local-4);
      stack_vars--;
    }

    local += 2;
  }

  // Make the call
  fprintf(out, "  jsr %s\n", name);

  // Pop all params off the Java stack
  if ((stack - stack_vars) > 0)
  {
    fprintf(out, "  tsx\n");
    fprintf(out, "  txa\n");
    fprintf(out, "  clc\n");
    fprintf(out, "  adc #%d\n", (stack - stack_vars) * 2);
    fprintf(out, "  tax\n");
    fprintf(out, "  txs\n");

    params -= (stack - stack_vars);
  }

  if (!is_void)
  {
    // REVIEW - This looks wrong
    fprintf(out, "  lda result + 0\n");
    fprintf(out, "  pha\n");
    fprintf(out, "  lda result + 1\n");
    fprintf(out, "  pha\n");
    stack++;
  }

  return 0;
}

int M6502::put_static(const char *name, int index)
{
  if (stack > 0)
  {
    fprintf(out, "  pla\n");
    fprintf(out, "  sta result + 1\n");
    fprintf(out, "  sta %s\n", name + 1);
    fprintf(out, "  pla\n");
    fprintf(out, "  sta result + 0\n");
    fprintf(out, "  sta %s\n", name + 0);
    stack--;
  }

  return 0;
}

int M6502::get_static(const char *name, int index)
{
  fprintf(out, "  pla\n");
  fprintf(out, "  sta %s\n", name + 1);
  fprintf(out, "  pla\n");
  fprintf(out, "  sta %s\n", name + 0);
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
    fprintf(out, "; new_array\n");
    fprintf(out, "  pla\n");
    fprintf(out, "  sta length + 1\n");
    fprintf(out, "  pla\n");
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

    if (type == TYPE_SHORT || type == TYPE_CHAR || type == TYPE_INT)
    {
      fprintf(out, "  asl length + 0\n");
      fprintf(out, "  rol length + 1\n");
    }
      else
    {
      fprintf(out, "  lda length + 0\n");
      fprintf(out, "  sta temp + 0\n");
      fprintf(out, "  lda length + 1\n");
      fprintf(out, "  sta temp + 1\n");

      fprintf(out, "  lda temp + 0\n");
      fprintf(out, "  and #1\n");
      fprintf(out, "  sta temp + 0\n");

      fprintf(out, "  clc\n");
      fprintf(out, "  lda length + 0\n");
      fprintf(out, "  adc temp + 0\n");
      fprintf(out, "  sta length + 0\n");
      fprintf(out, "  lda length + 1\n");
      fprintf(out, "  adc temp + 1\n");
      fprintf(out, "  sta length + 1\n");
    }

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
    fprintf(out, "  adc #2\n");
    fprintf(out, "  sta result + 0\n");
    fprintf(out, "  lda result + 1\n");
    fprintf(out, "  adc #0\n");
    fprintf(out, "  sta result + 1\n");

    fprintf(out, "  lda result + 0\n");
    fprintf(out, "  pha\n");
    fprintf(out, "  lda result + 1\n");
    fprintf(out, "  pha\n");
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

  return 0;
}

int M6502::push_array_length()
{
  fprintf(out, "; push_array_length\n");

  if (stack > 0)
  {
    fprintf(out, "  pla\n");
    fprintf(out, "  sta result + 1\n");
    fprintf(out, "  pla\n");
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
    fprintf(out, "  pha\n");
    fprintf(out, "  lda result + 1\n");
    fprintf(out, "  pha\n");
  }

  return 0;
}

int M6502::push_array_length(const char *name, int field_id)
{
  fprintf(out, "; push_array_length2\n");

  fprintf(out, "  lda %s + 0\n", name);
  fprintf(out, "  sta temp + 0\n");
  fprintf(out, "  lda %s + 1\n", name);
  fprintf(out, "  sta temp + 1\n");

  fprintf(out, "  sec\n");
  fprintf(out, "  lda temp + 0\n");
  fprintf(out, "  sbc #2\n");
  fprintf(out, "  sta temp + 0\n");
  fprintf(out, "  lda temp + 1\n");
  fprintf(out, "  sbc #0\n");
  fprintf(out, "  sta temp + 1\n");

  fprintf(out, "  lda temp + 0\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda temp + 1\n");
  fprintf(out, "  pha\n");

  stack++;

  return 0;
}

int M6502::array_read_byte()
{
  fprintf(out, "; array_read_byte\n");

  get_values_from_stack(2);

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
  fprintf(out, "  pha\n");
  // sign-extend
  fprintf(out, "  bpl #10\n");
  fprintf(out, "  lda #0xff\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  beq #6\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pha\n");

  stack++;

  return 0;
}

int M6502::array_read_short()
{
  return array_read_int();
}

int M6502::array_read_int()
{
  fprintf(out, "; array_read_int\n");

  get_values_from_stack(2);

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
  fprintf(out, "  pha\n");
  fprintf(out, "  lda result + 1\n");
  fprintf(out, "  pha\n");

  stack++;

  return 0;
}

int M6502::array_read_byte(const char *name, int field_id)
{
  if (stack > 0)
  {
    fprintf(out, "; array_read_byte2\n");

    fprintf(out, "  lda %s & 0xff\n", name);
    fprintf(out, "  sta address + 0\n");
    fprintf(out, "  lda %s >> 8\n", name);
    fprintf(out, "  sta address + 1\n");

    fprintf(out, "  pla\n");
    fprintf(out, "  sta result + 1\n");
    fprintf(out, "  pla\n");
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
    fprintf(out, "  bmi #7\n");
    fprintf(out, "  sty result + 1\n");
    fprintf(out, "  lda #0\n");
    fprintf(out, "  beq #5\n");
    fprintf(out, "  lda #0xff\n");
    fprintf(out, "  sta result + 1\n");

    fprintf(out, "  lda result + 0\n");
    fprintf(out, "  pha\n");
    fprintf(out, "  lda result + 1\n");
    fprintf(out, "  pha\n");
  }

  return 0;
}

int M6502::array_read_short(const char *name, int field_id)
{
  return array_read_short(name, field_id);
}

int M6502::array_read_int(const char *name, int field_id)
{
  if (stack > 0)
  {
    fprintf(out, "; array_read_int2\n");

    fprintf(out, "  lda %s & 0xff\n", name);
    fprintf(out, "  sta address + 0\n");
    fprintf(out, "  lda %s >> 8\n", name);
    fprintf(out, "  sta address + 1\n");

    fprintf(out, "  pla\n");
    fprintf(out, "  sta result + 1\n");
    fprintf(out, "  pla\n");
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
    fprintf(out, "  ldy #1\n");
    fprintf(out, "  lda (address),y\n");
    fprintf(out, "  sta result + 1\n");

    fprintf(out, "  lda result + 0\n");
    fprintf(out, "  pha\n");
    fprintf(out, "  lda result + 1\n");
    fprintf(out, "  pha\n");
  }

  return 0;
}

int M6502::array_write_byte()
{
  fprintf(out, "; array_write_byte\n");

  get_values_from_stack(3);

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

  return 0;
}

int M6502::array_write_short()
{
  return array_write_int();
}

int M6502::array_write_int()
{
  fprintf(out, "; array_write_int\n");

  get_values_from_stack(3);

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

  return 0;
}

int M6502::array_write_byte(const char *name, int field_id)
{
  get_values_from_stack(2);

  fprintf(out, "; array_write_byte2\n");
  fprintf(out, "  clc\n"); 
  fprintf(out, "  lda value2 + 0\n"); 
  fprintf(out, "  adc %s + 0\n", name); 
  //fprintf(out, "  sta value2 + 0\n"); 
  fprintf(out, "  sta address + 0\n"); 
  fprintf(out, "  lda value2 + 1\n"); 
  fprintf(out, "  adc %s + 1\n", name); 
  //fprintf(out, "  sta value2 + 1\n"); 
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
  //fprintf(out, "  sta value2 + 0\n"); 
  fprintf(out, "  sta address + 0\n"); 
  fprintf(out, "  lda value2 + 1\n"); 
  fprintf(out, "  adc %s + 1\n", name); 
  //fprintf(out, "  sta value2 + 1\n"); 
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
    fprintf(out, "  pla\n");
    fprintf(out, "  sta value1 + 1\n");
    fprintf(out, "  pla\n");
    fprintf(out, "  sta value1 + 0\n");
    stack--;
  }

  if(num > 1)
  {
    fprintf(out, "  pla\n");
    fprintf(out, "  sta value2 + 1\n");
    fprintf(out, "  pla\n");
    fprintf(out, "  sta value2 + 0\n");
    stack--;
  }

  if(num > 2)
  {
    fprintf(out, "  pla\n");
    fprintf(out, "  sta value3 + 1\n");
    fprintf(out, "  pla\n");
    fprintf(out, "  sta value3 + 0\n");
    stack--;
  }

  return 0;
}

// Memory API
int M6502::memory_read8()
{
  fprintf(out, "; memory_read8\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  pha\n");
  // sign-extend
  fprintf(out, "  bpl #10\n");
  fprintf(out, "  lda #0xff\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  beq #6\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pha\n");

  return 0;
}

int M6502::memory_write8()
{
  fprintf(out, "; memory_write8\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tax\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  txa\n");
  fprintf(out, "  sta (address),y\n");

  stack -= 2;

  return 0;
}

// C64 API
int M6502::c64_vic_border(/* color */) { POKE(0xd020); return 0; }

int M6502::c64_vic_background(/* color */) { POKE(0xd021); return 0; }

int M6502::c64_vic_sprite_enable(/* num */ ) { POKE(0xd015); return 0; }

// slow but convenient :)
int M6502::c64_vic_sprite_pos(/* sprite, x, y */ )
{
  fprintf(out, "; c64_vic_sprite_pos\n");
  // y
  fprintf(out, "  pla\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta sprite_y\n");

  // x
  fprintf(out, "  pla\n");
  fprintf(out, "  sta sprite_x + 1\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta sprite_x + 0\n");

  // sprite
  fprintf(out, "  pla\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tay\n");
  fprintf(out, "  sta sprite_msb_set\n");
  fprintf(out, "  tax\n");
  fprintf(out, "  beq #6\n");
  fprintf(out, "  asl sprite_msb_set\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  bne #-8\n");

  // vic sprite position offset (sprite * 2)
  fprintf(out, "  tya\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  tay\n");

  // set xy pos
  fprintf(out, "  lda sprite_msb_set\n");
  fprintf(out, "  eor #0xff\n");
  fprintf(out, "  sta sprite_msb_clear\n");

  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and sprite_msb_clear\n");
  fprintf(out, "  sta 0xd010\n");
  fprintf(out, "  lda sprite_x + 1\n");
  fprintf(out, "  and sprite_msb_set\n");
  fprintf(out, "  ora 0xd010\n");
  fprintf(out, "  sta 0xd010\n");

  fprintf(out, "  lda sprite_x + 0\n");
  fprintf(out, "  sta 0xd000,y\n");

  fprintf(out, "  lda sprite_y\n");
  fprintf(out, "  sta 0xd001,y\n");
  
  return 0;
}

int M6502::c64_vic_poke_reg(/* dest, value */ )
{
  // value
  fprintf(out, "  pla\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tay\n");

  // reg
  fprintf(out, "  pla\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tax\n");

  fprintf(out, "  tya\n");
  fprintf(out, "  sta 0xd000,x\n");

  return 0;
}

#if 0
void M6502::close()
{
}
#endif

