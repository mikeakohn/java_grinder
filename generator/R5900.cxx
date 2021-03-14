/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2021 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "generator/R5900.h"

R5900::R5900()
{
}

R5900::~R5900()
{
}

int R5900::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".ps2_ee\n");

  // Set where RAM starts / ends
  //fprintf(out, "  ram_start equ 0x%x\n", ram_start);
  //fprintf(out, "  ram_end equ 0x%x\n", ram_end);

  return 0;
}

int R5900::start_init()
{
  // Add any set up items (stack, registers, etc).
  fprintf(out, ".org 0x%x\n", org);
  fprintf(out, "start:\n");

  return 0;
}

