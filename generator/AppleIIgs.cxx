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
  if (W65816::open(filename) != 0) { return -1; }

  return 0;
}

int AppleIIgs::appleiigs_plotChar_IC()
{
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
    "  xce\n");

  return 0;
}


