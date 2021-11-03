/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2021 by Michael Kohn
 *
 * V.Smile by Simon Eriksson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "generator/VSmile.h"

VSmile::VSmile()
{
  //start_org = 0x8000;
}

VSmile::~VSmile()
{
}

int VSmile::open(const char *filename)
{
  if (UNSP::open(filename) != 0) { return -1; }

  return 0;
}

