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

#define LOCALS(a) ((a * 2) + 2)

M6502::M6502() :
  stack(0),
  is_main(0)
{

}

M6502::~M6502()
{
  fprintf(out, "result:\n");
  fprintf(out, "dw 0\n");
  fprintf(out, "temp:\n");
  fprintf(out, "dw 0\n");
  fprintf(out, "temp2:\n");
  fprintf(out, "dw 0\n");
  fprintf(out, "locals:\n");
  fprintf(out, "db 0\n");
}

int M6502::open(char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  // assumes c64 for now (basic loader)
  fprintf(out, ".65xx\n");

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
  fprintf(out, "  cld\n\n");
  //fprintf(out, "ram_start:\n");

  return 0;
}

int M6502::start_init()
{
  return 0;
}

int M6502::insert_static_field_define(const char *name, const char *type, int index)
{
/*
  fprintf(out, "%s equ ram_start+%d\n", name, (index + 1) * 2);
*/
  return -1;
}

int M6502::init_heap(int field_count)
{
/*
  fprintf(out, "  ;; Set up heap and static initializers\n");
  fprintf(out, "  lda ram_start+%d\n", ((field_count + 1) * 2) & 0xff);
  fprintf(out, "  sta ram_start + 0\n");
  fprintf(out, "  lda ram_start+%d\n", ((field_count + 1) * 2) >> 8);
  fprintf(out, "  sta ram_start + 1\n");
  return 0;
*/
  return -1;
}

int M6502::insert_field_init_boolean(char *name, int index, int value)
{
  value = (value == 0) ? 0 : 1;
  //fprintf(out, "  mov #%d, &ram_start+%d ; %s\n", value, (index + 1) * 2, name);
  fprintf(out, "  lda #%d\n", value & 0xff);
  fprintf(out, "  sta %s\n", name + 0);
  fprintf(out, "  lda #%d\n", value >> 8);
  fprintf(out, "  sta %s\n", name + 1);
  return 0;
}

int M6502::insert_field_init_byte(char *name, int index, int value)
{
  return -1;
}

int M6502::insert_field_init_short(char *name, int index, int value)
{
  return -1;
}

int M6502::insert_field_init_int(char *name, int index, int value)
{
  return -1;
}

int M6502::insert_field_init(char *name, int index)
{
  return -1;
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

  fprintf(out, "  lda #0x%02x\n", value & 0xff);
  fprintf(out, "  pha\n");
  fprintf(out, "  lda #0x%02x\n", value >> 8);
  fprintf(out, "  pha\n");
  stack++;

  return 0;
}

int M6502::push_integer_local(int index)
{

  fprintf(out, "  lda 0x100 + (locals - %d) + 0\n", LOCALS(index));
  fprintf(out, "  pha\n");
  fprintf(out, "  lda 0x100 + (locals - %d) + 1\n", LOCALS(index));
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
  printf("long is not supported right now\n");
  return -1;
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
  fprintf(out, "  lda #0x%02x\n", b);
  fprintf(out, "  pha\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  pha\n");
  stack++;

  return 0;
}

int M6502::push_short(int16_t s)
{
  uint16_t value = (s & 0xffff);

  fprintf(out, "  lda #0x%02x\n", value & 0xff);
  fprintf(out, "  pha\n");
  fprintf(out, "  lda #0x%02x\n", value >> 8);
  fprintf(out, "  pha\n");
  stack++;

  return 0;
}

int M6502::pop_integer_local(int index)
{
  fprintf(out, "  pla\n");
  fprintf(out, "  sta 0x100 + (locals - %d) + 1\n", LOCALS(index));
  fprintf(out, "  pla\n");
  fprintf(out, "  sta 0x100 + (locals - %d) + 0\n", LOCALS(index));
  stack--;
  return 0;
}

int M6502::pop_ref_local(int index)
{
  return pop_integer_local(index);
}

int M6502::pop()
{
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta result + 0\n");
  stack--;

  return 0;
}

int M6502::dup()
{
//FIXME: slow
  fprintf(out, "  pla\n");
  fprintf(out, "  sta temp + 1\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta temp + 0\n");
  fprintf(out, "  lda temp + 0\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda temp + 1\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda temp + 0\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda temp + 1\n");
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
//FIXME: slow
  fprintf(out, "  pla\n");
  fprintf(out, "  sta temp + 1\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta temp + 0\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta temp2 + 1\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta temp2 + 0\n");
  fprintf(out, "  lda temp2 + 0\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda temp2 + 1\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda temp + 0\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda temp + 1\n");
  fprintf(out, "  pha\n");
  return 0;
}

int M6502::add_integers()
{
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
  fprintf(out, "  txa\n");
  fprintf(out, "  adc result + 1\n");
  fprintf(out, "  sta result + 1\n");

  return 0;
}

int M6502::sub_integers()
{
  return -1;
}

int M6502::mul_integers()
{
  return -1;
}

int M6502::div_integers()
{
  return -1;
}

int M6502::mod_integers()
{
  return -1;
}

int M6502::neg_integer()
{
  return -1;
}

int M6502::shift_left_integer()
{
  return -1;
}

int M6502::shift_right_integer()
{
  return -1;
}

int M6502::shift_right_uinteger()
{
  return -1;
}

int M6502::and_integer()
{
  return -1;
}

int M6502::or_integer()
{
  return -1;
}

int M6502::xor_integer()
{
  return -1;
}

int M6502::inc_integer(int index, int num)
{
  return -1;
}

int M6502::jump_cond(const char *label, int cond)
{
  return -1;
}

int M6502::jump_cond_integer(const char *label, int cond)
{
  return -1;
}

int M6502::return_local(int index, int local_count)
{
  fprintf(out, "  lda 0x100 + locals - %d + 0\n", LOCALS(index));
  fprintf(out, "  sta result + 0\n");
  fprintf(out, "  lda 0x100 + locals - %d + 1\n", LOCALS(index));
  fprintf(out, "  sta result + 1\n");

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

int M6502::return_integer(int local_count)
{
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
    fprintf(out, "  sta locals\n");
  }

  fprintf(out, "  rts\n");

  return 0;
}

int M6502::return_void(int local_count)
{
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

int M6502::jump(const char *name)
{
  return -1;
}

int M6502::call(const char *name)
{
  return -1;
}

int M6502::invoke_static_method(const char *name, int params, int is_void)
{
int local;
int stack_vars = stack;

  printf("invoke_static_method() name=%s params=%d is_void=%d\n", name, params, is_void);

  local = (params * -2);
  while(local != 0)
  {
    // Copy parameters onto the stack so they are local variables in
    // the called method.  Start with -4 because the return address will
    // be at 0 and r12 will be at 2.

    if (stack_vars > 0)
    {
      //fprintf(out, "  mov.w %d(SP), %d(SP)\n", stack_vars, local-4);
      fprintf(out, "  tsx\n");

      fprintf(out, "  lda 0x100 + %d + 0,x\n", (stack_vars - 1) * 2);
      fprintf(out, "  sta 0x100 + %d + 0,x\n", local-4);
      fprintf(out, "  lda 0x100 + %d + 1,x\n", (stack_vars - 1) * 2);
      fprintf(out, "  sta 0x100 + %d + 1,x\n", local-4);
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
    fprintf(out, "  sec\n");
    fprintf(out, "  sbc #%d\n", (stack - stack_vars) * 2);
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
  return -1;
}

int M6502::get_static(const char *name, int index)
{
  return -1;
}

int M6502::brk()
{
  return -1;
}

int M6502::new_array(uint8_t type)
{
  return -1;
}

int M6502::insert_array(const char *name, int32_t *data, int len, uint8_t type)
{
  return -1;
}

int M6502::push_array_length()
{
  return -1;
}

int M6502::push_array_length(const char *name, int field_id)
{
  return -1;
}

int M6502::array_read_byte()
{
  return -1;
}

int M6502::array_read_short()
{
  return -1;
}

int M6502::array_read_int()
{
  return -1;
}

int M6502::array_read_byte(const char *name, int field_id)
{
  return -1;
}

int M6502::array_read_short(const char *name, int field_id)
{
  return -1;
}

int M6502::array_read_int(const char *name, int field_id)
{
  return -1;
}

int M6502::array_write_byte()
{
  return -1;
}

int M6502::array_write_short()
{
  return -1;
}

int M6502::array_write_int()
{
  return -1;
}

int M6502::array_write_byte(const char *name, int field_id)
{
  return -1;
}

int M6502::array_write_short(const char *name, int field_id)
{
  return -1;
}

int M6502::array_write_int(const char *name, int field_id)
{
  return -1;
}


#if 0
void M6502::close()
{
}
#endif

