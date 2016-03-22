/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 * CPC support by Carsten Dost  - https://github.com/deringenieur71
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "CPC.h"

CPC::CPC()
{
  //start_org = 0x4000;
  //ram_start = 0xc000;
  //ram_end = 0xdfff;
}

CPC::~CPC()
{
  //if (need_plot_lores) { add_plot_lores(); }
}

int CPC::open(const char *filename)
{
  if (Z80::open(filename) != 0) { return -1; }

  fprintf(out, "ram_start equ 0xc000\n");
  fprintf(out, "heap_ptr equ ram_start\n");
  fprintf(out, "save_iy equ heap_ptr\n");

  return 0;
}

int CPC::cpc_getFunction_IC()
{
  return -1;
}

