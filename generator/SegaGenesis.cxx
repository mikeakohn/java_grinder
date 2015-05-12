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

#include "SegaGenesis.h"

SegaGenesis::SegaGenesis()
{

}

SegaGenesis::~SegaGenesis()
{
}

int SegaGenesis::open(const char *filename)
{
  if (MC68000::open(filename) != 0) { return -1; }

  return 0;
}

int SegaGenesis::start_init()
{
  // Add any set up items (stack, registers, etc).
  //fprintf(out, ".org ???\n");
  fprintf(out, "start:\n");

  return 0;
}


