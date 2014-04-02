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

#include "TI84.h"
#include "Z80.h"

TI84::TI84()
{
}

TI84::~TI84()
{
}

int TI84::open(const char *filename)
{
  if (Z80::open(filename) != 0) { return -1; }

  return 0;
}

int TI84::start_init()
{
  // Add any set up items (stack, registers, etc).
  //fprintf(out, ".org ???\n");
  fprintf(out, "start:\n");

  return 0;
}



