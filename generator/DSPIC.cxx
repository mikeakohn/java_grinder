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

#define __STDC_FORMAT_MACROS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "DSPIC.h"

#define LOCAL_VAR(i) (i * 2)

// ABI is:
// w0 temp
// w1 bottom of stack
// w2 ..
// w3 ..
// w4 ..
// w5 ..
// w6 ..
// w7 ..
// w8 ..
// w9 top of stack 
// w10 pointer to locals

static const char *cond_str[] = { "z", "nz", "lt", "le", "gt", "ge" };

DSPIC::DSPIC() : reg(0), stack(0)
{

}

DSPIC::~DSPIC()
{

}

int DSPIC::open(char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  // For now we only support a specific chip
  fprintf(out, ".dspic\n");
  fprintf(out, ".include \"p30f3012.inc\"\n\n");

  // Add any set up items (stack, registers, etc)
  fprintf(out, "start:\n\n");

  return 0;
}

void DSPIC::serial_init()
{
}

void DSPIC::method_start(int local_count, const char *name)
{
  reg = 0;
  stack = 0;

  // main() function goes here
  fprintf(out, "%s:\n", name);
  fprintf(out, "  mov sp, w10\n");
  fprintf(out, "  sub #0x%x, sp\n", local_count * 2);
}

void DSPIC::method_end(int local_count)
{
  //fprintf(out, "  add #0x%x, sp\n", local_count * 2);
  //fprintf(out, "  ret\n\n");
  fprintf(out, "\n");
}

int DSPIC::push_integer(int32_t n)
{
  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);
    return -1;
  }

  uint16_t value = (n & 0xffff);

  if (reg < 8)
  {
    fprintf(out, "  mov #0x%02x, w%d\n", value, (reg + 1));
    reg++;
  }
    else
  {
    fprintf(out, "  mov #0x%02x, w0\n", value);
    fprintf(out, "  push w0\n");
    stack++;
  }

  return 0;
}

int DSPIC::push_integer_local(int index)
{
  fprintf(out, "  mov [w10-#%d], w0\n", LOCAL_VAR(index));

  if (reg < 8)
  {
    fprintf(out, "  mov w0, w%d\n", (reg + 1));
    reg++;
  }
    else
  {
    fprintf(out, "  push w0\n");
    stack++;
  }

  return 0;
}

int DSPIC::push_long(int64_t n)
{
  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %" PRId64 " bigger than 16 bit.\n", n);
    return -1;
  }

  uint16_t value = (n & 0xffff);

  if (reg < 8)
  {
    fprintf(out, "  mov #0x%02x, w%d\n", value, (reg + 1));
    reg++;
  }
    else
  {
    fprintf(out, "  mov #0x%02x, w0\n", value);
    fprintf(out, "  push w0\n");
    stack++;
  }
  return 0;
}

int DSPIC::push_float(float f)
{
  printf("Error: float is not currently supported.\n");
  return -1;
}

int DSPIC::push_double(double f)
{
  printf("Error: double is not currently supported.\n");
  return -1;
}

int DSPIC::push_byte(int8_t b)
{
  uint16_t value = ((int32_t)b)&0xffff;

  if (reg < 8)
  {
    fprintf(out, "  mov #0x%02x, w%d\n", value, (reg + 1));
    reg++;
  }
    else
  {
    fprintf(out, "  mov #0x%02x, w0\n", value);
    fprintf(out, "  push w0\n");
    stack++;
  }
  return 0;
}

int DSPIC::push_short(int16_t s)
{
  uint16_t value = s;

  if (reg < 8)
  {
    fprintf(out, "  mov #0x%02x, w%d\n", value, (reg + 1));
    reg++;
  }
    else
  {
    fprintf(out, "  mov #0x%02x, w0\n", value);
    fprintf(out, "  push w0\n");
    stack++;
  }
  return 0;
}

int DSPIC::pop_integer_local(int index)
{
  return 0;
}

int DSPIC::pop()
{
  return 0;
}

int DSPIC::dup()
{
  return 0;
}

int DSPIC::dup2()
{
  return 0;
}

int DSPIC::swap()
{
  return 0;
}

int DSPIC::add_integers()
{
  if (reg < 7)
  {
    fprintf(out, "  add w%d, w%d, w%d\n", reg, reg - 1, reg - 1);
    reg--;
  }
    else
  if (reg == 7)
  {

  }
    else
  {
  }


  return 0;
}

int DSPIC::sub_integers()
{
  return 0;
}

int DSPIC::mul_integers()
{
  return 0;
}

int DSPIC::div_integers()
{
  return 0;
}

int DSPIC::neg_integer()
{
  return 0;
}

int DSPIC::shift_left_integer()
{
  return 0;
}

int DSPIC::shift_right_integer()
{
  return 0;
}

int DSPIC::shift_right_uinteger()
{
  return 0;
}

int DSPIC::and_integer()
{
  return 0;
}

int DSPIC::or_integer()
{
  return 0;
}

int DSPIC::xor_integer()
{
  return 0;
}

int DSPIC::inc_integer(int index, int num)
{
  return 0;
}

int DSPIC::jump_cond(const char *label, int cond)
{
  if (reg < 8)
  {
    fprintf(out, "  cmp #0, w%d\n", reg);
  }
    else
  {
    //fprintf(out, "  cmp #0, [sp+%d]\n", reg);
  }

  fprintf(out, "  bra %s, %s\n", cond_str[cond], label);
  return 0;
}

int DSPIC::jump_cond_integer(const char *label, int cond)
{
  if (reg < 7)
  {
    fprintf(out, "  cmp w%d, w%d, w%d\n", reg, reg - 1, reg - 1);
  }
    else
  if (reg == 7)
  {

  }
    else
  {
  }

  fprintf(out, "  bra %s, %s\n", cond_str[cond], label);

  return 0;
}

int DSPIC::return_local(int index, int local_count)
{
  fprintf(out, "  mov [w10-#%d], w0\n", LOCAL_VAR(index));
  //fprintf(out, "  add #0x%x, sp\n", local_count * 2);
  fprintf(out, "  mov w10, sp\n");
  fprintf(out, "  ret\n");

  return 0;
}

int DSPIC::return_integer(int local_count)
{
  if (reg < 8)
  {
    fprintf(out, "  mov w%d, w0\n", reg - 1 + 1);
  }
    else
  {
    fprintf(out, "  mov [sp+2], w0\n");
  }

  //fprintf(out, "  add #0x%x, sp\n", local_count * 2);
  fprintf(out, "  mov w10, sp\n");
  fprintf(out, "  ret\n");
  return 0;
}

int DSPIC::return_void(int local_count)
{
  //fprintf(out, "  add #0x%x, sp\n", local_count * 2);
  fprintf(out, "  mov w10, sp\n");
  fprintf(out, "  ret\n");

  return 0;
}

int DSPIC::jump(const char *name)
{
  fprintf(out, "  bra %s\n", name);
  return 0;
}

int DSPIC::call(const char *name)
{
  fprintf(out, "  call %s\n", name);
  return 0;
}

int DSPIC::brk()
{
  return 0;
}

void DSPIC::close()
{
}

// GPIO functions
int DSPIC::ioport_set_pins_as_input() { return -1; }
int DSPIC::ioport_set_pins_as_output() { return -1; }
int DSPIC::ioport_set_pins_value() { return -1; }
int DSPIC::ioport_set_pins_high() { return -1; }
int DSPIC::ioport_set_pins_low() { return -1; }
int DSPIC::ioport_set_pin_as_output() { return -1; }
int DSPIC::ioport_set_pin_as_input() { return -1; }
int DSPIC::ioport_set_pin_high() { return -1; }
int DSPIC::ioport_set_pin_low() { return -1; }
int DSPIC::ioport_is_pin_input_high() { return -1; }
int DSPIC::ioport_get_port_input_value() { return -1; }
int DSPIC::ioport_set_port_output_value() { return -1; }

