/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "generator/TI84.h"
#include "generator/Z80.h"

#define BCALL(a) \
  save_registers(); \
  fprintf(out, "  ld iy, (save_iy)\n"); \
  fprintf(out, "  rst 0x28\n"); \
  fprintf(out, "  .db %s&0xff, %s>>8\n", #a, #a); \
  restore_registers()

TI84::TI84(int model) : model(model)
{
}

TI84::~TI84()
{
}

int TI84::open(const char *filename)
{
  if (Z80::open(filename) != 0) { return -1; }

  if (model == TI84_PLUS)
  {
    fprintf(out, ".include \"ti84plus.inc\"\n\n");
  }
    else
  {
    fprintf(out, ".include \"ti84pcse.inc\"\n\n");
  }
  fprintf(out, "save_iy equ appData\n");
  fprintf(out, "save_ix equ appData+2\n");
  fprintf(out, "ram_start equ appData+4\n");
  fprintf(out, "heap_ptr equ ram_start\n");

  return 0;
}

int TI84::start_init()
{
  fprintf(out, "\n");
  fprintf(out, ".org 0x4000\n");

  // http://wikiti.brandonw.net/index.php?title=84PCSE:OS:Applications
  if (model == TI84_PLUS)
  {
    fprintf(out,
      "  ;Header (128 bytes)\n"
      "  .db 0x80, 0x0f\n"
      "  .db 0x00, 0x00, 0x00, 0x00\n"
      "  .db 0x80, 0x12\n"
      "  .db 0x01, 0x04\n"
      "  .db 0x80, 0x21\n"
      "  .db 0x01\n"
      "  .db 0x80, 0x31\n"
      "  .db 0xa1\n"
      "  .db 0x80, 0x48\n"
      "  .db \"HelloApp\" ;make sure this is 8 bytes\n"
      "  .db 0x80, 0x81\n"
      "  .db 0x01\n"
      "  .db 0x80, 0x90\n"
      "  .db 0x03, 0x26, 0x09, 0x04\n"
      "  .db 0x1e, 0xff, 0x2b, 0x57\n"
      "  .db 0x02, 0x0d, 0x40, 0xa1, 0x6b, 0x99, 0xf6, 0x59, 0xbc, 0x67\n"
      "  .db 0xf5, 0x85, 0x9c, 0x09, 0x6c, 0x0f, 0xb4, 0x03, 0x9b, 0xc9\n"
      "  .db 0x03, 0x32, 0x2c, 0xe0, 0x03, 0x20, 0xe3, 0x2c, 0xf4, 0x2d\n"
      "  .db 0x73, 0xb4, 0x27, 0xc4, 0xa0, 0x72, 0x54, 0xb9, 0xea, 0x7c\n"
      "  .db 0x3b, 0xaa, 0x16, 0xf6, 0x77, 0x83, 0x7a, 0xee, 0x1a, 0xd4\n"
      "  .db 0x42, 0x4c, 0x6b, 0x8b, 0x13, 0x1f, 0xbb, 0x93, 0x8b, 0xfc\n"
      "  .db 0x19, 0x1c, 0x3c, 0xec, 0x4d, 0xe5, 0x75\n"
      "  .db 0x80, 0x7f\n"
      "  .db 0x00, 0x00, 0x00, 0x00\n"
      "  .db 0x00, 0x00, 0x00, 0x00\n"
      "  .db 0x00, 0x00, 0x00, 0x00\n"
      "  .db 0x00, 0x00, 0x00, 0x00\n"
      "  .db 0x00, 0x00, 0x00, 0x00\n\n");
  }
    else
  if (model == TI84_PLUS_C)
  {
    fprintf(out,
      "  ;Header (128 bytes)\n"
      "  .db 0x80, 0x0f\n"
      "  .db 0x00, 0x00, 0x00, 0x00\n"
      "  .db 0x80, 0x12\n"
      "  .db 0x01, 0x0f\n"
      "  .db 0x80, 0x21\n"
      "  .db 0x01\n"
      "  .db 0x80, 0x31\n"
      "  .db 0xa1\n"
      "  .db 0x80, 0x48\n"
      "  .db \"HelloApp\" ;make sure this is 8 bytes\n"
      "  .db 0x80, 0x81\n"
      "  .db 0x01\n"
      "  .db 0x80, 0x90\n"
      "  .db 0x03, 0x26, 0x09, 0x04\n"
      "  .db 0x1e, 0xff, 0x2b, 0x57\n"
      "  .db 0x02, 0x0d, 0x40, 0xa1, 0x6b, 0x99, 0xf6, 0x59, 0xbc, 0x67\n"
      "  .db 0xf5, 0x85, 0x9c, 0x09, 0x6c, 0x0f, 0xb4, 0x03, 0x9b, 0xc9\n"
      "  .db 0x03, 0x32, 0x2c, 0xe0, 0x03, 0x20, 0xe3, 0x2c, 0xf4, 0x2d\n"
      "  .db 0x73, 0xb4, 0x27, 0xc4, 0xa0, 0x72, 0x54, 0xb9, 0xea, 0x7c\n"
      "  .db 0x3b, 0xaa, 0x16, 0xf6, 0x77, 0x83, 0x7a, 0xee, 0x1a, 0xd4\n"
      "  .db 0x42, 0x4c, 0x6b, 0x8b, 0x13, 0x1f, 0xbb, 0x93, 0x8b, 0xfc\n"
      "  .db 0x19, 0x1c, 0x3c, 0xec, 0x4d, 0xe5, 0x75\n"
      "  .db 0x80, 0x7f\n"
      "  .db 0x00, 0x00, 0x00, 0x00\n"
      "  .db 0x00, 0x00, 0x00, 0x00\n"
      "  .db 0x00, 0x00, 0x00, 0x00\n"
      "  .db 0x00, 0x00, 0x00, 0x00\n"
      "  .db 0x00, 0x00, 0x00, 0x00\n\n");
  }

  // Add any set up items (stack, registers, etc).
  fprintf(out, "start:\n");

  return 0;
}

int TI84::ti84_clearScreen()
{
  BCALL(_ClrLCDFull);

  return 0;
}

int TI84::ti84_clearRect()
{
  fprintf(out, "  pop hl\n");
  fprintf(out, "  pop de\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  ld b,c\n");
  fprintf(out, "  pop bc\n");
  BCALL(_ClearRect);
  stack -= 4;

  return 0;
}

int TI84::ti84_drawLine()
{
  // (b,c,d,e,h)
  fprintf(out, "  ld (save_ix),ix\n");
  fprintf(out, "  ld ix,(heap_ptr)\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  ld h,c\n"); // h=param5
  fprintf(out, "  pop de\n"); // e=param4
  fprintf(out, "  pop bc\n");
  fprintf(out, "  ld d,c\n"); // d=param3

  fprintf(out, "  pop bc\n");
  fprintf(out, "  ld (ix),c\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  ld b,c\n"); // d=param1
  fprintf(out, "  ld c,(ix)\n"); // d=param2
#if 0
  fprintf(out, "  ld b,1\n");
  fprintf(out, "  ld c,2\n");
  fprintf(out, "  ld d,50\n");
  fprintf(out, "  ld e,50\n");
  fprintf(out, "  ld h,1\n");
#endif
  fprintf(out, "  ld ix,(save_ix)\n");
  BCALL(_ILine);
  stack -= 5;

  return 0;
}

int TI84::ti84_drawPoint()
{
  fprintf(out, "  pop de\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld a,l\n");
  BCALL(_IPoint);
  stack -= 3;

  return 0;
}

int TI84::ti84_fillRect()
{
  fprintf(out, "  pop hl\n");
  fprintf(out, "  pop de\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  ld b,c\n");
  fprintf(out, "  pop bc\n");
  BCALL(_FillRect);
  stack -= 4;

  return 0;
}

int TI84::ti84_print()
{
  fprintf(out, "  pop hl\n");
  BCALL(_PutS);
  stack--;

  return 0;
}

int TI84::ti84_printCenter()
{
  fprintf(out, "  pop hl\n");
  BCALL(_CenterPutS);
  stack--;

  return 0;
}

int TI84::ti84_printHL()
{
  BCALL(_DispHL);

  return 0;
}

int TI84::ti84_putc()
{
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld a,l\n");
  BCALL(_PutC);
  stack--;

  return 0;
}

int TI84::ti84_setCursorX()
{
  fprintf(out, "  pop af\n");
  fprintf(out, "  ld (curCol), a\n");
  stack--;

  return 0;
}

int TI84::ti84_setCursorY()
{
  fprintf(out, "  pop af\n");
  fprintf(out, "  ld (curRow), a\n");
  stack--;

  return 0;
}

int TI84::ti84_setDrawBGColor()
{
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld (penBGColor), hl\n");
  stack--;

  return 0;
}

int TI84::ti84_setDrawBGWhite()
{
  BCALL(_SetPenBG_White);

  return 0;
}


int TI84::ti84_setDrawColor()
{
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld (penFGColor), hl\n");
  stack--;

  return 0;
}

int TI84::ti84_setFillColor()
{
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld (fillRectColor), hl\n");
  stack--;

  return 0;
}

int TI84::ti84_setTextBGColor()
{
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld (curBGColor), hl\n");
  stack--;

  return 0;
}

int TI84::ti84_setTextColor()
{
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld (curFGColor), hl\n");
  stack--;

  return 0;
}


