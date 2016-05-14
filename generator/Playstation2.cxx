/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "Playstation2.h"

Playstation2::Playstation2()
{
  org = 0x1d001000;
  ram_start = 0x80000000;
  ram_end = 0x80003fff;
  virtual_address = 0x9d000000;
  physical_address = 0x1d000000;
}

Playstation2::~Playstation2()
{
  // it's 6 instructions to inline and 5 instructions to call
  //if (need_spi_read) { add_spi_read(); }
}

int Playstation2::open(const char *filename)
{
  if (MIPS64::open(filename) != 0) { return -1; }

  return 0;
}

int Playstation2::start_init()
{
  MIPS64::start_init();

  //fprintf(out, "  li $k0, 0xbf880000\n");
  //fprintf(out, "  li $k1, 0xbf800000\n");

  return 0;
}

int Playstation2::playstation2_waitVsync()
{
  return 0;
}

