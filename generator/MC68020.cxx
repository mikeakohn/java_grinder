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
#include <string.h>
#include <stdint.h>

#include "MC68020.h"

#define REG_STACK(a) (a)
#define LOCALS(i) (i * 4)

MC68020::MC68020()
{
}

MC68020::~MC68020()
{

}

int MC68020::mul_integer()
{
  return stack_alu("muls");
}

int MC68020::mul_integer(int num)
{
  fprintf(out, "  muls.l #%d, d%d\n", num, REG_STACK(reg-1));
  return 0;
}

int MC68020::div_integer()
{
  return stack_alu("divs");
}

int MC68020::div_integer(int num)
{
  fprintf(out, "  divs.l #%d, d%d\n", num, REG_STACK(reg-1));
  return 0;
}

int MC68020::mod_integer()
{
  if (stack == 0)
  {
    fprintf(out, "  divs.l d%d, d%d:d%d\n", REG_STACK(reg-1), REG_STACK(reg-2), REG_STACK(reg-1));
    reg--;
  }
    else
  {
    return -1;
  }

  return 0;
}

int MC68020::mod_integer(int num)
{
  if (stack == 0)
  {
    fprintf(out, "  divs.l #%d, r%d:r%d\n", num, REG_STACK(reg-2), REG_STACK(reg-1));
  }
    else
  {
    return -1;
  }

  return 0;
}



