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

  need_put_string = 0;
}

W65C265SXB::~W65C265SXB()
{
  if(need_put_string) { insert_put_string(); }
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
  fprintf(out, "  sep #0x20\n");
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
  fprintf(out, "  sep #0x20\n");
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
  need_put_string = 1;
  fprintf(out, "  jsr put_string\n");
  stack--;

  return 0;

/*
  fprintf(out, "; putString\n");
  POP();
  fprintf(out, "  stx result\n");
  fprintf(out, "  tax\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  jsr.l 0xe04e\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  ldx result\n");
  stack--;
*/
}

void W65C265SXB::insert_put_string()
{
  fprintf(out, "put_string:\n");
  POP();
  fprintf(out, "  sta address\n");
  fprintf(out, "  dec address\n");
  fprintf(out, "  dec address\n");
  fprintf(out, "  lda (address)\n");
  fprintf(out, "  tay\n");
  fprintf(out, "  inc address\n");
  fprintf(out, "  inc address\n");
  fprintf(out, "put_string_loop:\n");
  fprintf(out, "  lda (address)\n");
  fprintf(out, "  beq put_string_end\n");
  fprintf(out, "  cmp #'\\n'\n");
  fprintf(out, "  bne put_string_skip\n");
  fprintf(out, "  lda #13\n");
  fprintf(out, "put_string_skip:\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  jsr.l 0xe04b\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  inc address\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne put_string_loop\n");
  fprintf(out, "put_string_end:\n");
  fprintf(out, "  rts\n");
}

// 0xdf24 - port 4 direction
// 0xdf25 - port 5 direction
// 0xdf26 - port 6 direction
// 0xdf20 - port 4 data
// 0xdf21 - port 5 data
// 0xdf22 - port 6 data

// GPIO
int W65C265SXB::ioport_setPinsAsInput(int port)
{
  if(port < 4 || port > 6)
    return -1;

  fprintf(out, "; ioport_setPinsAsInput\n");
  POP();
  fprintf(out, "  eor #0xffff\n");
  fprintf(out, "  and 0xdf24 + %d\n", port - 4);
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta 0xdf24 + %d\n", port - 4);
  fprintf(out, "  rep #0x30\n");
  stack--;

  return 0;
}

int W65C265SXB::ioport_setPinsAsInput(int port, int const_val)
{
  if(port < 4 || port > 6)
    return -1;

  fprintf(out, "; ioport_setPinsAsInput (optimized)\n");
  fprintf(out, "  lda #%d\n", const_val);
  fprintf(out, "  eor #0xffff\n");
  fprintf(out, "  and 0xdf24 + %d\n", port - 4);
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta 0xdf24 + %d\n", port - 4);
  fprintf(out, "  rep #0x30\n");

  return 0;
}

int W65C265SXB::ioport_setPinsAsOutput(int port)
{
  if(port < 4 || port > 6)
    return -1;

  fprintf(out, "; ioport_setPinsAsOutput\n");
  POP();
  fprintf(out, "  ora 0xdf24 + %d\n", port - 4);
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta 0xdf24 + %d\n", port - 4);
  fprintf(out, "  rep #0x30\n");
  stack--;

  return 0;
}

int W65C265SXB::ioport_setPinsAsOutput(int port, int const_val)
{
  if(port < 4 || port > 6)
    return -1;

  fprintf(out, "; ioport_setPinsAsOutput (optimized)\n");
  fprintf(out, "  lda #%d\n", const_val);
  fprintf(out, "  ora 0xdf24 + %d\n", port - 4);
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta 0xdf24 + %d\n", port - 4);
  fprintf(out, "  rep #0x30\n");

  return 0;
}

int W65C265SXB::ioport_setPinsValue(int port)
{
  if(port < 4 || port > 6)
    return -1;

  fprintf(out, "; ioport_setPinsValue\n");
  POP();
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta 0xdf20 + %d\n", port - 4);
  fprintf(out, "  rep #0x30\n");
  stack--;

  return 0;
}

int W65C265SXB::ioport_setPinsValue(int port, int const_val)
{
  if(port < 4 || port > 6)
    return -1;

  fprintf(out, "; ioport_setPinsValue (optimized)\n");
  fprintf(out, "  lda #%d\n", const_val);
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta 0xdf20 + %d\n", port - 4);
  fprintf(out, "  rep #0x30\n");


  return 0;
}

int W65C265SXB::ioport_setPinsHigh(int port)
{
  if(port < 4 || port > 6)
    return -1;

  fprintf(out, "; ioport_setPinsHigh\n");
  POP();
  fprintf(out, "  ora 0xdf20 + %d\n", port - 4);
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta 0xdf20 + %d\n", port - 4);
  fprintf(out, "  rep #0x30\n");
  stack--;

  return 0;
}

int W65C265SXB::ioport_setPinsLow(int port)
{
  if(port < 4 || port > 6)
    return -1;

  fprintf(out, "; ioport_setPinsLow\n");
  POP();
  fprintf(out, "  eor #0xffff\n");
  fprintf(out, "  and 0xdf20 + %d\n", port - 4);
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta 0xdf20 + %d\n", port - 4);
  fprintf(out, "  rep #0x30\n");
  stack--;


  return 0;
}

int W65C265SXB::ioport_setPinAsOutput(int port)
{
  return -1;
}

int W65C265SXB::ioport_setPinAsInput(int port)
{
  return -1;
}

int W65C265SXB::ioport_setPinHigh(int port)
{
  return -1;
}

int W65C265SXB::ioport_setPinHigh(int port, int const_val)
{
  if(port < 4 || port > 6)
    return -1;

  if(const_val < 0 || const_val > 7) { return -1; }

  fprintf(out, "; ioport_setPinHigh (optimized)\n");
  fprintf(out, "  lda #%d\n", (1 << const_val));
  fprintf(out, "  ora 0xdf20 + %d\n", port - 4);
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta 0xdf20 + %d\n", port - 4);
  fprintf(out, "  rep #0x30\n");

  return 0;
}

int W65C265SXB::ioport_setPinLow(int port)
{
  return -1;
}

int W65C265SXB::ioport_setPinLow(int port, int const_val)
{
  if(port < 4 || port > 6)
    return -1;

  if(const_val < 0 || const_val > 7) { return -1; }

  fprintf(out, "; ioport_setPinLow (optimized)\n");
  fprintf(out, "  lda #%d\n", (1 << const_val));
  fprintf(out, "  eor #0xffff\n");
  fprintf(out, "  and 0xdf20 + %d\n", port - 4);
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta 0xdf20 + %d\n", port - 4);
  fprintf(out, "  rep #0x30\n");

  return 0;
}

int W65C265SXB::ioport_isPinInputHigh(int port)
{
  return -1;
}

int W65C265SXB::ioport_getPortInputValue(int port)
{
  return -1;
}

