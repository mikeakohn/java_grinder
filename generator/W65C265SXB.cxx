/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2017 by Michael Kohn
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
  need_put_int = 0;
}

W65C265SXB::~W65C265SXB()
{
  if(need_put_string) { insert_put_string(); }
  if(need_put_int) { insert_put_int(); }
}

int W65C265SXB::open(const char *filename)
{
  if(W65816::open(filename) != 0) { return -1; }

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
int W65C265SXB::sxb_getChar()
{
  fprintf(out, "; getChar\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  phx\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "getChar_again_%d:\n", label_count);
  fprintf(out, "  jsr.l 0xe033\n");
  fprintf(out, "  bcs getChar_again_%d\n", label_count);
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  plx\n");
  PUSH();
  label_count++;
  stack++;

  return 0;
}

int W65C265SXB::sxb_putChar_C()
{
  POP();
  fprintf(out, "; putChar\n");
  fprintf(out, "  phx\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  jsr.l 0xe04b\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  plx\n");
  stack--;

  return 0;
}

int W65C265SXB::sxb_getInt()
{
  fprintf(out, "; getInt\n");
  return -1;
}

int W65C265SXB::sxb_putInt_I()
{
  need_put_int = 1;
  fprintf(out, "; putInt\n");
  fprintf(out, "  jsr put_int\n");
  stack--;

  return 0;
}

int W65C265SXB::sxb_getString()
{
  fprintf(out, "; getString\n");
  return -1;
}

int W65C265SXB::sxb_putString_X()
{
  need_put_string = 1;
  fprintf(out, "  jsr put_string\n");
  stack--;

  return 0;
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
  fprintf(out, "  phx\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  jsr.l 0xe04b\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  plx\n");
  fprintf(out, "  inc address\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne put_string_loop\n");
  fprintf(out, "put_string_end:\n");
  fprintf(out, "  rts\n");
}

void W65C265SXB::insert_put_int()
{
  fprintf(out, "put_int:\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta value1\n");
  fprintf(out, "  sta value2\n");
  fprintf(out, "  lda #'0'\n");
  fprintf(out, "  sta put_int_result + 0\n");
  fprintf(out, "  sta put_int_result + 2\n");
  fprintf(out, "  sta put_int_result + 4\n");
  fprintf(out, "  sta put_int_result + 6\n");
  fprintf(out, "  sta put_int_result + 8\n");
  POP();
  fprintf(out, "  bne put_int_check_sign\n");
  fprintf(out, "  lda #'0'\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  jsr.l 0xe04b\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  rts\n");
  fprintf(out, "put_int_check_sign:\n");
  fprintf(out, "  stx result\n");
  fprintf(out, "  bpl put_int_start\n");
  fprintf(out, "  eor #0xffff\n");
  fprintf(out, "  inc\n");
  fprintf(out, "  ldx #1\n");
  fprintf(out, "  stx value1\n");
  fprintf(out, "put_int_start:\n");
  fprintf(out, "  ldx #0\n");
  fprintf(out, "  sec\n");
  fprintf(out, "put_int_loop:\n");
  fprintf(out, "  cmp put_int_table,x\n");
  fprintf(out, "  bmi put_int_loop2\n");
  fprintf(out, "  inc value2\n");
  fprintf(out, "put_int_loop2:\n");
  fprintf(out, "  cmp put_int_table,x\n");
  fprintf(out, "  bmi put_int_continue\n");
  fprintf(out, "  sbc put_int_table,x\n");
  fprintf(out, "  inc put_int_result,x\n");
  fprintf(out, "  bra put_int_loop2\n");
  fprintf(out, "put_int_continue:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  cpx #10\n");
  fprintf(out, "  bne put_int_loop\n");
  fprintf(out, "  lda value1\n");
  fprintf(out, "  beq put_int_skip_minus\n");
  fprintf(out, "  lda #'-'\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  jsr.l 0xe04b\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "put_int_skip_minus:\n");
  fprintf(out, "  lda #5\n");
  fprintf(out, "  sta value1\n");
  fprintf(out, "  ldx #0\n");
  fprintf(out, "put_int_print_loop:\n");
  fprintf(out, "  lda value2\n");
  fprintf(out, "  cmp value1\n");
  fprintf(out, "  bmi put_int_print_continue\n");
  fprintf(out, "  lda put_int_result,x\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  jsr.l 0xe04b\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "put_int_print_continue:\n");
  fprintf(out, "  dec value1\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  cpx #10\n");
  fprintf(out, "  bne put_int_print_loop\n");
  fprintf(out, "  ldx result\n");
  fprintf(out, "  rts\n");
}

// 0xdf24 - port 4 direction
// 0xdf25 - port 5 direction
// 0xdf26 - port 6 direction
// 0xdf20 - port 4 data
// 0xdf21 - port 5 data
// 0xdf22 - port 6 data

// GPIO
int W65C265SXB::ioport_setPinsAsInput_I(int port)
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

int W65C265SXB::ioport_setPinsAsInput_I(int port, int const_val)
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

int W65C265SXB::ioport_setPinsAsOutput_I(int port)
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

int W65C265SXB::ioport_setPinsAsOutput_I(int port, int const_val)
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

int W65C265SXB::ioport_setPinsValue_I(int port)
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

int W65C265SXB::ioport_setPinsValue_I(int port, int const_val)
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

int W65C265SXB::ioport_setPinsHigh_I(int port)
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

int W65C265SXB::ioport_setPinsLow_I(int port)
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

int W65C265SXB::ioport_setPinAsOutput_I(int port)
{
  return -1;
}

int W65C265SXB::ioport_setPinAsInput_I(int port)
{
  return -1;
}

int W65C265SXB::ioport_setPinHigh_I(int port)
{
  return -1;
}

int W65C265SXB::ioport_setPinHigh_I(int port, int const_val)
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

int W65C265SXB::ioport_setPinLow_I(int port)
{
  return -1;
}

int W65C265SXB::ioport_setPinLow_I(int port, int const_val)
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

int W65C265SXB::ioport_isPinInputHigh_I(int port)
{
  return -1;
}

int W65C265SXB::ioport_getPortInputValue(int port)
{
  if(port < 4 || port > 6)
    return -1;

  fprintf(out, "; ioport_getPortInputValue\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  lda 0xdf20 + %d\n", port - 4);
  fprintf(out, "  rep #0x30\n");
  PUSH();
  stack++;

  return 0;
}

// tone generator API
int W65C265SXB::sxb_controlTones_IIZZ()
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
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  lda.b value1\n");
  fprintf(out, "  jsr.l 0xe009\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  plx\n");
  stack -= 4;

  return 0;
}

