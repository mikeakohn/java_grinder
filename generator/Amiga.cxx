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
#include <stdint.h>

#include "generator/Amiga.h"

// NOTE: a3 points to Amiga hardware registers.

Amiga::Amiga()
{
  //start_org = 0x8000;
}

Amiga::~Amiga()
{
}

int Amiga::open(const char *filename)
{
  if (MC68000::open(filename) != 0) { return -1; }

  fprintf(out,
    "\n"
    "  ;; Hunk file header\n"
    "  dc32 0x03f3, 1\n"
  );

  return 0;
}

int Amiga::start_init()
{
  // Add any set up items (stack, registers, etc).
  MC68000::start_init();

  fprintf(out, "  ;; a3 points to custom chips.\n");
  fprintf(out, "  movea.l #0xdff000, a3\n");

  return 0;
}

int Amiga::amiga_setPalette_II()
{
  fprintf(out, "  ;; amiga_setPalette_II()\n");
  fprintf(out, "  move.w d%d, (0x180, a3)\n", reg - 1);

  reg--;

  return 0;
}

