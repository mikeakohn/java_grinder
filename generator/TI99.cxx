/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "TI99.h"

TI99::TI99()
{
}

TI99::~TI99()
{
}

int TI99::open(const char *filename)
{
  if (TMS9900::open(filename) != 0) { return -1; }

  fprintf(out, ".include \"ti99.inc\"\n\n");
  fprintf(out, "ram_start equ appData+4\n");
  fprintf(out, "heap_ptr equ ram_start\n");

  return 0;
}

int TI99::start_init()
{
  fprintf(out, "\n");
  fprintf(out, ".org 0x4000\n");

  // Add any set up items (stack, registers, etc).
  fprintf(out, "start:\n");

  return 0;
}

int TI99::ti99_print()
{
  return -1;
}

int TI99::ti99_printChar()
{
  return -1;
}

int TI99::ti99_printChar(int c)
{
  return -1;
}

int TI99::ti99_setCursor()
{
  return -1;
}

int TI99::ti99_setCursor(int x, int y)
{
  return -1;
}




