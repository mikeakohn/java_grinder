/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "AppleIIgs.h"

AppleIIgs::AppleIIgs()
{
}

AppleIIgs::~AppleIIgs()
{
}

int AppleIIgs::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".65816\n");

  // stack location
  fprintf(out, "stack equ 0x100\n");

  // ram start
//FIXME this is not correct
  fprintf(out, "ram_start equ 0x9500\n");
  fprintf(out, "heap_ptr equ ram_start\n");

  // points to locals
  fprintf(out, "locals equ 0x00\n");

  // temp variables
  fprintf(out, "result equ 0x02\n");
  fprintf(out, "remainder equ 0x04\n");
  fprintf(out, "length equ 0x06\n");
  fprintf(out, "value1 equ 0x08\n");
  fprintf(out, "value2 equ 0x10\n");
  fprintf(out, "value3 equ 0x12\n");

  // start
  fprintf(out, ".org 0x9100\n");
  fprintf(out, "; change to 16-bit mode\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  xce\n");
  fprintf(out, "; all 16-bit registers\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "; set up stack\n");
  fprintf(out, "  lda #0x1FF\n");
  fprintf(out, "  tcs\n");
  fprintf(out, "; set up direct-page\n");
  fprintf(out, "  pea 0x0000\n");

  return 0;
}

int AppleIIgs::appleiigs_plotChar_IC()
{
//FIXME broked
  fprintf(out,
    "  ;; plotChar()\n"
    "ply\n"
    "pla\n"
    "sep #0x20\n"
    "sta 0x0000,y\n"
    "rep #0x20\n");

  return 0;
}

int AppleIIgs::appleiigs_printChar_C()
{
  fprintf(out,
    "  ;; printChar()\n"
    "  pla\n"
    "  sec\n"
    "  xce\n"
    "  jsr 0xfded\n"
    "  clc\n"
    "  xce\n"
    "  rep #0x30\n");

  return 0;
}

