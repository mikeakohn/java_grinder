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

#include "MIPS.h"

// ABI is:
// r4
// r5
// r6
// r7
// r8
// r9
// r10
// r11

MIPS::MIPS()
{

}

MIPS::~MIPS()
{

}

int MIPS::open(char *filename)
{
  return 0;
}

#if 0
void MIPS::serial_init()
{
}
#endif

void MIPS::method_start(int local_count, const char *name)
{
}

void MIPS::method_end(int local_count)
{
}

int MIPS::push_integer(int32_t n)
{
  return -1;
}

int MIPS::push_integer_local(int index)
{
  return -1;
}

int MIPS::push_long(int64_t n)
{
  return -1;
}

int MIPS::push_float(float f)
{
  return -1;
}

int MIPS::push_double(double f)
{
  return -1;
}

int MIPS::push_byte(int8_t b)
{
  return -1;
}

int MIPS::push_short(int16_t s)
{
  return -1;
}

int MIPS::pop_integer_local(int index)
{
  return -1;
}

int MIPS::pop()
{
  return -1;
}

int MIPS::dup()
{
  return -1;
}

int MIPS::dup2()
{
  return -1;
}

int MIPS::swap()
{
  return -1;
}

int MIPS::add_integers()
{
  return -1;
}

int MIPS::sub_integers()
{
  return -1;
}

int MIPS::mul_integers()
{
  return -1;
}

int MIPS::div_integers()
{
  return -1;
}

int MIPS::neg_integer()
{
  return -1;
}

int MIPS::shift_left_integer()
{
  return -1;
}

int MIPS::shift_right_integer()
{
  return -1;
}

int MIPS::shift_right_uinteger()
{
  return -1;
}

int MIPS::and_integer()
{
  return -1;
}

int MIPS::or_integer()
{
  return -1;
}

int MIPS::xor_integer()
{
  return -1;
}

int MIPS::inc_integer(int index, int num)
{
  return -1;
}

int MIPS::jump_cond(const char *label, int cond)
{
  return -1;
}

int MIPS::jump_cond_integer(const char *label, int cond)
{
  return -1;
}

int MIPS::return_local(int index, int local_count)
{
  return -1;
}

int MIPS::return_integer(int local_count)
{
  return -1;
}

int MIPS::return_void(int local_count)
{
  return -1;
}

int MIPS::jump(const char *name)
{
  return -1;
}

int MIPS::call(const char *name)
{
  return -1;
}

int MIPS::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int MIPS::brk()
{
  return -1;
}

#if 0
void MIPS::close()
{
}
#endif

// GPIO functions
int MIPS::ioport_setPinsAsInput(int port) { return -1; }
int MIPS::ioport_setPinsAsOutput(int port) { return -1; }
int MIPS::ioport_setPinsValue(int port) { return -1; }
int MIPS::ioport_setPinsHigh(int port) { return -1; }
int MIPS::ioport_setPinsLow(int port) { return -1; }
int MIPS::ioport_setPinAsOutput(int port) { return -1; }
int MIPS::ioport_setPinAsInput(int port) { return -1; }
int MIPS::ioport_setPinHigh(int port) { return -1; }
int MIPS::ioport_setPinLow(int port) { return -1; }
int MIPS::ioport_isPinInputHigh(int port) { return -1; }
int MIPS::ioport_getPortInputValue(int port) { return -1; }
//int MIPS::ioport_setPortOutputValue(int port) { return -1; }

