/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2023 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "generator/RISCVIceFun.h"

RISCVIceFun::RISCVIceFun()
{
  org = 0x4000;
  ram_start = 0;
  ram_end = 4096;
}

RISCVIceFun::~RISCVIceFun()
{
}

int RISCVIceFun::ioport_setPinsValue_I(int port)
{
  int peripheral = port == 0 ? 0x20 : 0x28;

  fprintf(out,
    "  ;; setPinsValue(port=%d)\n"
    "  li a1, 0x8000\n"
    "  sw t%d, %d(a1)\n",
    port,
    reg - 1, peripheral);

  reg -= 1;

  return 0;
}

int RISCVIceFun::ioport_setPinsValue_I(int port, int const_val)
{
  int peripheral = port == 0 ? 0x20 : 0x28;

  fprintf(out,
    "  ;; setPinsValue(port=%d, const_val=%d)\n"
    "  li a1, 0x8000\n"
    "  li a0, %d\n"
    "  sw a0, %d(a1)\n",
    port, const_val,
    const_val,
    peripheral);

  return 0;
}

int RISCVIceFun::ice_fun_setTone_I()
{
  fprintf(out,
    "  ;; ice_fun_setTone_I()\n"
    "  li a1, 0x8000\n"
    "  sw t%d, 0x24(a1)\n",
    reg - 1);

  reg -= 1;

  return 0;
}

