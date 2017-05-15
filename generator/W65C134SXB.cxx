/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2017 by Michael Kohn
 *
 * W65C134SXB by Joe Davisson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "W65C134SXB.h"

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

W65C134SXB::W65C134SXB()
{
  start_org = 0x1000;
  // FIXME - What to change this to?
  //java_stack = 0x900;
  ram_start = 0x7000;

  need_put_string = 0;
  need_put_int = 0;
}

W65C134SXB::~W65C134SXB()
{
  if(need_put_string) { insert_put_string(); }
  if(need_put_int) { insert_put_int(); }
}

int W65C134SXB::open(const char *filename)
{
  if(M6502::open(filename) != 0) { return -1; }

  fprintf(out, "put_int_result equ 0xe0\n");
  fprintf(out, "put_int_table equ 0xf0\n");
  fprintf(out, "lda #10000\n");
  fprintf(out, "sta put_int_table + 0\n");
  fprintf(out, "lda #1000\n");
  fprintf(out, "sta put_int_table + 2\n");
  fprintf(out, "lda #100\n");
  fprintf(out, "sta put_int_table + 4\n");
  fprintf(out, "lda #10\n");
  fprintf(out, "sta put_int_table + 6\n");
  fprintf(out, "lda #1\n");
  fprintf(out, "sta put_int_table + 8\n");

  fprintf(out, "; set up processor stack\n");
  fprintf(out, "  lda #0x8ff\n");
  fprintf(out, "  tcs\n");

  return 0;
}

// terminal interface API
int W65C134SXB::sxb_getChar()
{
  fprintf(out, "; getChar\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  phx\n");
  fprintf(out, "getChar_again_%d:\n", label_count);
  fprintf(out, "  jsr 0xe033\n");
  fprintf(out, "  bcs getChar_again_%d\n", label_count);
  fprintf(out, "  plx\n");
  PUSH();
  label_count++;
  stack++;

  return 0;
}

int W65C134SXB::sxb_putChar_C()
{
  POP();
  fprintf(out, "; putChar\n");
  fprintf(out, "  phx\n");
  fprintf(out, "  jsr 0xe04b\n");
  fprintf(out, "  plx\n");
  stack--;

  return 0;
}

int W65C134SXB::sxb_getInt()
{
  fprintf(out, "; getInt\n");
  return -1;
}

int W65C134SXB::sxb_putInt_I()
{
  need_put_int = 1;
  fprintf(out, "; putInt\n");
  fprintf(out, "  jsr put_int\n");
  stack--;

  return 0;
}

int W65C134SXB::sxb_getString()
{
  fprintf(out, "; getString\n");
  return -1;
}

int W65C134SXB::sxb_putString_X()
{
  need_put_string = 1;
  fprintf(out, "  jsr put_string\n");
  stack--;

  return -1;
}

void W65C134SXB::insert_put_string()
{
  fprintf(out, "put_string:\n");
  POP();
  fprintf(out, "  rts\n");
}

void W65C134SXB::insert_put_int()
{
  fprintf(out, "put_int:\n");
  fprintf(out, "  rts\n");
}

// GPIO
int W65C134SXB::ioport_setPinsAsInput_I(int port)
{
  if(port < 4 || port > 6)
    return -1;

  fprintf(out, "; ioport_setPinsAsInput\n");
  POP();
  fprintf(out, "  eor #0xffff\n");
  fprintf(out, "  and PD4 + %d\n", port - 4);
  fprintf(out, "  sta PD4 + %d\n", port - 4);
  stack--;

  return 0;
}

int W65C134SXB::ioport_setPinsAsInput_I(int port, int const_val)
{
  if(port < 4 || port > 6)
    return -1;

  fprintf(out, "; ioport_setPinsAsInput (optimized)\n");
  fprintf(out, "  lda #%d\n", const_val);
  fprintf(out, "  eor #0xffff\n");
  fprintf(out, "  and PD4 + %d\n", port - 4);
  fprintf(out, "  sta PD4 + %d\n", port - 4);

  return 0;
}

int W65C134SXB::ioport_setPinsAsOutput_I(int port)
{
  if(port < 4 || port > 6)
    return -1;

  fprintf(out, "; ioport_setPinsAsOutput\n");
  POP();
  fprintf(out, "  ora PD4 + %d\n", port - 4);
  fprintf(out, "  sta PD4 + %d\n", port - 4);
  stack--;

  return 0;
}

int W65C134SXB::ioport_setPinsAsOutput_I(int port, int const_val)
{
  if(port < 4 || port > 6)
    return -1;

  fprintf(out, "; ioport_setPinsAsOutput (optimized)\n");
  fprintf(out, "  lda #%d\n", const_val);
  fprintf(out, "  ora 0xdf24 + %d\n", port - 4);
  fprintf(out, "  sta 0xdf24 + %d\n", port - 4);

  return 0;
}

int W65C134SXB::ioport_setPinsValue_I(int port)
{
  if(port < 4 || port > 6)
    return -1;

  fprintf(out, "; ioport_setPinsValue\n");
  POP();
  fprintf(out, "  sta 0xdf20 + %d\n", port - 4);
  stack--;

  return 0;
}

int W65C134SXB::ioport_setPinsValue_I(int port, int const_val)
{
  if(port < 4 || port > 6)
    return -1;

  fprintf(out, "; ioport_setPinsValue (optimized)\n");
  fprintf(out, "  lda #%d\n", const_val);
  fprintf(out, "  sta 0xdf20 + %d\n", port - 4);

  return 0;
}

int W65C134SXB::ioport_setPinsHigh_I(int port)
{
  if(port < 4 || port > 6)
    return -1;

  fprintf(out, "; ioport_setPinsHigh\n");
  POP();
  fprintf(out, "  ora 0xdf20 + %d\n", port - 4);
  fprintf(out, "  sta 0xdf20 + %d\n", port - 4);
  stack--;

  return 0;
}

int W65C134SXB::ioport_setPinsLow_I(int port)
{
  if(port < 4 || port > 6)
    return -1;

  fprintf(out, "; ioport_setPinsLow\n");
  POP();
  fprintf(out, "  eor #0xffff\n");
  fprintf(out, "  and 0xdf20 + %d\n", port - 4);
  fprintf(out, "  sta 0xdf20 + %d\n", port - 4);
  stack--;


  return 0;
}

int W65C134SXB::ioport_setPinAsOutput_I(int port)
{
  return -1;
}

int W65C134SXB::ioport_setPinAsInput_I(int port)
{
  return -1;
}

int W65C134SXB::ioport_setPinHigh_I(int port)
{
  return -1;
}

int W65C134SXB::ioport_setPinHigh_I(int port, int const_val)
{
  if(port < 4 || port > 6)
    return -1;

  if(const_val < 0 || const_val > 7) { return -1; }

  fprintf(out, "; ioport_setPinHigh (optimized)\n");
  fprintf(out, "  lda #%d\n", (1 << const_val));
  fprintf(out, "  ora 0xdf20 + %d\n", port - 4);
  fprintf(out, "  sta 0xdf20 + %d\n", port - 4);

  return 0;
}

int W65C134SXB::ioport_setPinLow_I(int port)
{
  return -1;
}

int W65C134SXB::ioport_setPinLow_I(int port, int const_val)
{
  if(port < 4 || port > 6)
    return -1;

  if(const_val < 0 || const_val > 7) { return -1; }

  fprintf(out, "; ioport_setPinLow (optimized)\n");
  fprintf(out, "  lda #%d\n", (1 << const_val));
  fprintf(out, "  eor #0xffff\n");
  fprintf(out, "  and 0xdf20 + %d\n", port - 4);
  fprintf(out, "  sta 0xdf20 + %d\n", port - 4);

  return 0;
}

int W65C134SXB::ioport_isPinInputHigh_I(int port)
{
  return -1;
}

int W65C134SXB::ioport_getPortInputValue(int port)
{
  if(port < 4 || port > 6)
    return -1;

  fprintf(out, "; ioport_getPortInputValue\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  lda 0xdf20 + %d\n", port - 4);
  PUSH();
  stack++;

  return 0;
}

// tone generator API
int W65C134SXB::sxb_controlTones_IIZZ()
{
//  int control = (enable2 & 1) | ((enable2 & 1) << 1);

  fprintf(out, "; controlTones\n");
  POP();
  fprintf(out, "  asl\n");
  fprintf(out, "  sta value1\n");
  POP();
  fprintf(out, "  ora value1\n");
  fprintf(out, "  sta value1\n");
  POP();
  fprintf(out, "  tay\n");
  POP();
  fprintf(out, "  phx\n");
  fprintf(out, "  tax\n");
  fprintf(out, "  lda value1\n");
  fprintf(out, "  jsr 0xe009\n");
  fprintf(out, "  plx\n");
  stack -= 4;

  return 0;
}

