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

#include "MSP430.h"

int MSP430::open(char *filename)
{
  return 0;
}

void MSP430::serial_init()
{
}

void MSP430::method_start(int local_count, const char *name)
{
}

void MSP430::method_end(int local_count)
{
}

int MSP430::push_integer(int32_t n)
{
  return 0;
}

int MSP430::push_integer_local(int index)
{
  return 0;
}

int MSP430::push_long(int64_t n)
{
  return 0;
}

int MSP430::push_float(float f)
{
  return 0;
}

int MSP430::push_double(double f)
{
  return 0;
}

int MSP430::push_byte(char b)
{
  return 0;
}

int MSP430::push_short(int16_t s)
{
  return 0;
}

int MSP430::pop_integer_local(int index)
{
  return 0;
}

int MSP430::pop()
{
  return 0;
}

int MSP430::dup()
{
  return 0;
}

int MSP430::dup2()
{
  return 0;
}

int MSP430::swap()
{
  return 0;
}

int MSP430::add_integers()
{
  return 0;
}

int MSP430::sub_integers()
{
  return 0;
}

int MSP430::mul_integers()
{
  return 0;
}

int MSP430::div_integers()
{
  return 0;
}

int MSP430::neg_integer()
{
  return 0;
}

int MSP430::shift_left_integer()
{
  return 0;
}

int MSP430::shift_right_integer()
{
  return 0;
}

int MSP430::shift_right_uinteger()
{
  return 0;
}

int MSP430::and_integer()
{
  return 0;
}

int MSP430::or_integer()
{
  return 0;
}

int MSP430::xor_integer()
{
  return 0;
}

int MSP430::inc_integer(int index, int num)
{
  return 0;
}

int MSP430::jump_cond(const char *label, int cond)
{
  return 0;
}

int MSP430::jump_cond_integer(const char *label, int cond)
{
  return 0;
}

int MSP430::return_local(int index, int local_count)
{
  return 0;
}

int MSP430::return_integer(int local_count)
{
  return 0;
}

int MSP430::return_void(int local_count)
{
  return 0;
}

int MSP430::jump(const char *name)
{
  return 0;
}

int MSP430::call(const char *name)
{
  return 0;
}

int MSP430::brk()
{
  return 0;
}

void MSP430::close()
{
}


