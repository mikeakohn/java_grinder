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

#include "MIPS64.h"

MIPS64::MIPS64()
{
  org = 0x1d001000;
  ram_start = 0x80000000;
  ram_end = 0x80003fff;
  virtual_address = 0x9d000000;
  physical_address = 0x1d000000;
}

MIPS64::~MIPS64()
{
}

int MIPS64::open(const char *filename)
{
  if (MIPS32::open(filename) != 0) { return -1; }

  return 0;
}

int MIPS64::start_init()
{
  MIPS32::start_init();

  //fprintf(out, "  li $k0, 0xbf880000\n");
  //fprintf(out, "  li $k1, 0xbf800000\n");

  return 0;
}

int MIPS64::push_float(float f)
{
  uint32_t *data = (uint32_t *)&f;
  uint32_t value = *data;

  fprintf(out, "  ; push_float(%f) -- data=0x%08x\n", f, value);

  if (value == 0)
  {
    if (reg < reg_max)
    {
      fprintf(out, "  move $t%d, $0\n", reg);
      reg++;
    }
      else
    {
      STACK_PUSH(0)
    }
  }
    else
  {
    if (reg < reg_max)
    {
      fprintf(out, "  li $t%d, 0x%04x\n", reg, value);
      reg++;
    }
      else
    {
      fprintf(out, "  li $t8, 0x%04x\n", value);
      STACK_PUSH(8)
    }
  }

  return 0;
}

