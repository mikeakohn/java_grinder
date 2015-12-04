/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 * W65C265SXB by Joe Davisson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "W65C265SXB.h"

W65C265SXB::W65C265SXB()
{
  start_org = 0x300;
  java_stack = 0x200;
  ram_start = 0x7000;
}

W65C265SXB::~W65C265SXB()
{
}

int W65C265SXB::open(const char *filename)
{
  if(W65816::open(filename) != 0) { return -1; }

  return 0;
}

// terminal interface API
int W65C265SXB::w65c265sxb_getChar()
{
  return -1;
}

int W65C265SXB::w65c265sxb_putChar_C()
{
  return -1;
}

int W65C265SXB::w65c265sxb_getInt()
{
  return -1;
}

int W65C265SXB::w65c265sxb_putInt_I()
{
  return -1;
}

int W65C265SXB::w65c265sxb_getString()
{
  return -1;
}

int W65C265SXB::w65c265sxb_putString()
{
  return -1;
}

