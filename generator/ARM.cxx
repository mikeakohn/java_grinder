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
#include <stdint.h>

#include "ARM.h"

// ABI is:
// r4
// r5
// r6
// r7
// r8
// r9
// r10
// r11

ARM::ARM()
{

}

ARM::~ARM()
{

}

int ARM::open(char *filename)
{
  return 0;
}

void ARM::serial_init()
{
}

void ARM::method_start(int local_count, const char *name)
{
}

void ARM::method_end(int local_count)
{
}

int ARM::push_integer(int32_t n)
{
  return 0;
}

int ARM::push_integer_local(int index)
{
  return 0;
}

int ARM::push_long(int64_t n)
{
  return 0;
}

int ARM::push_float(float f)
{
  return 0;
}

int ARM::push_double(double f)
{
  return 0;
}

int ARM::push_byte(int8_t b)
{
  return 0;
}

int ARM::push_short(int16_t s)
{
  return 0;
}

int ARM::pop_integer_local(int index)
{
  return 0;
}

int ARM::pop()
{
  return 0;
}

int ARM::dup()
{
  return 0;
}

int ARM::dup2()
{
  return 0;
}

int ARM::swap()
{
  return 0;
}

int ARM::add_integers()
{
  return 0;
}

int ARM::sub_integers()
{
  return 0;
}

int ARM::mul_integers()
{
  return 0;
}

int ARM::div_integers()
{
  return 0;
}

int ARM::neg_integer()
{
  return 0;
}

int ARM::shift_left_integer()
{
  return 0;
}

int ARM::shift_right_integer()
{
  return 0;
}

int ARM::shift_right_uinteger()
{
  return 0;
}

int ARM::and_integer()
{
  return 0;
}

int ARM::or_integer()
{
  return 0;
}

int ARM::xor_integer()
{
  return 0;
}

int ARM::inc_integer(int index, int num)
{
  return 0;
}

int ARM::jump_cond(const char *label, int cond)
{
  return 0;
}

int ARM::jump_cond_integer(const char *label, int cond)
{
  return 0;
}

int ARM::return_local(int index, int local_count)
{
  return 0;
}

int ARM::return_integer(int local_count)
{
  return 0;
}

int ARM::return_void(int local_count)
{
  return 0;
}

int ARM::jump(const char *name)
{
  return 0;
}

int ARM::call(const char *name)
{
  return 0;
}

int ARM::brk()
{
  return 0;
}

#if 0
void ARM::close()
{
}
#endif

// GPIO functions
int ARM::ioport_setPinsAsInput() { return -1; }
int ARM::ioport_setPinsAsOutput() { return -1; }
int ARM::ioport_setPinsValue() { return -1; }
int ARM::ioport_setPinsHigh() { return -1; }
int ARM::ioport_setPinsLow() { return -1; }
int ARM::ioport_setPinAsOutput() { return -1; }
int ARM::ioport_setPinAsInput() { return -1; }
int ARM::ioport_setPinHigh() { return -1; }
int ARM::ioport_setPinLow() { return -1; }
int ARM::ioport_isPinInputHigh() { return -1; }
int ARM::ioport_getPortInputValue() { return -1; }
int ARM::ioport_setPortOutputValue() { return -1; }

