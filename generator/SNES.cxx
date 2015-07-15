/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 * SNES by Joe Davisson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "SNES.h"

SNES::SNES()
{
}

SNES::~SNES()
{
}

int SNES::open(const char *filename)
{
  if (W65816::open(filename) != 0) { return -1; }

  return 0;
}


