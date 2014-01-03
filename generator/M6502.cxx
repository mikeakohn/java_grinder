/**
 *  Java Grinder
 *  Author: The Naken Crew
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2013 by The Naken Crew
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "M6502.h"

// ABI is:
// A
// X
// Y

int M6502::open(char *filename)
{
  return 0;
}

void M6502::serial_init()
{
}

void M6502::method_start(int local_count, const char *name)
{
}

void M6502::method_end(int local_count)
{
}

int M6502::push_integer(int32_t n)
{
  return 0;
}

int M6502::push_integer_local(int index)
{
  return 0;
}

int M6502::push_long(int64_t n)
{
  return 0;
}

int M6502::push_float(float f)
{
  return 0;
}

int M6502::push_double(double f)
{
  return 0;
}

int M6502::push_byte(char b)
{
  return 0;
}

int M6502::push_short(int16_t s)
{
  return 0;
}

int M6502::pop_integer_local(int index)
{
  return 0;
}

int M6502::pop()
{
  return 0;
}

int M6502::dup()
{
  return 0;
}

int M6502::dup2()
{
  return 0;
}

int M6502::swap()
{
  return 0;
}

int M6502::add_integers()
{
  return 0;
}

int M6502::sub_integers()
{
  return 0;
}

int M6502::mul_integers()
{
  return 0;
}

int M6502::div_integers()
{
  return 0;
}

int M6502::neg_integer()
{
  return 0;
}

int M6502::shift_left_integer()
{
  return 0;
}

int M6502::shift_right_integer()
{
  return 0;
}

int M6502::shift_right_uinteger()
{
  return 0;
}

int M6502::and_integer()
{
  return 0;
}

int M6502::or_integer()
{
  return 0;
}

int M6502::xor_integer()
{
  return 0;
}

int M6502::inc_integer(int index, int num)
{
  return 0;
}

int M6502::jump_cond(const char *label, int cond)
{
  return 0;
}

int M6502::jump_cond_integer(const char *label, int cond)
{
  return 0;
}

int M6502::return_local(int index, int local_count)
{
  return 0;
}

int M6502::return_integer(int local_count)
{
  return 0;
}

int M6502::return_void(int local_count)
{
  return 0;
}

int M6502::jump(const char *name)
{
  return 0;
}

int M6502::call(const char *name)
{
  return 0;
}

int M6502::brk()
{
  return 0;
}

void M6502::close()
{
}


