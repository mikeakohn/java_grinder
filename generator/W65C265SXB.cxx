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

#define PUSH() \
  fprintf(out, "; PUSH\n"); \
  fprintf(out, "  sta stack,x\n"); \
  fprintf(out, "  dex\n"); \
  fprintf(out, "  dex\n")

#define POP() \
  fprintf(out, "; POP\n"); \
  fprintf(out, "  inx\n"); \
  fprintf(out, "  inx\n"); \
  fprintf(out, "  lda stack,x\n")

#define LOCALS(a) (a * 2)

W65C265SXB::W65C265SXB()
{
  start_org = 0x1000;
  java_stack = 0x900;
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
  fprintf(out, "; getChar\n");
  fprintf(out, "  sep #0x30\n");
  fprintf(out, "  jsr.l 0xe036\n");
  fprintf(out, "  rep #0x30\n");
  PUSH();
  stack++;

  return 0;
}

int W65C265SXB::w65c265sxb_putChar_C()
{
  POP();
  fprintf(out, "; putChar\n");
  fprintf(out, "  sep #0x30\n");
  fprintf(out, "  jsr.l 0xe04b\n");
  fprintf(out, "  rep #0x30\n");
  stack--;

  return 0;
}

int W65C265SXB::w65c265sxb_getInt()
{
  fprintf(out, "; getInt\n");
  return -1;
}

int W65C265SXB::w65c265sxb_putInt_I()
{
  fprintf(out, "; putInt\n");
  return -1;
}

int W65C265SXB::w65c265sxb_getString()
{
  fprintf(out, "; getString\n");
  return -1;
}

int W65C265SXB::w65c265sxb_putString()
{
  fprintf(out, "; putString\n");
  return -1;
}

