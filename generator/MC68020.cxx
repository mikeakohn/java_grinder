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
#include <string.h>
#include <stdint.h>

#include "generator/MC68020.h"

#define REG_STACK(a) (a)
#define LOCALS(i) (i * 4)

MC68020::MC68020()
{
}

MC68020::~MC68020()
{

}

int MC68020::integer_to_byte()
{
  fprintf(out, "  extb.l %s\n", top_reg());
  return 0;
}

int MC68020::array_read_byte()
{
  int index_reg;

  get_values_from_stack(&index_reg);
  get_ref_from_stack();

  if (reg < reg_max)
  {
    fprintf(out, "  move.b (d%d,a2), d%d\n", index_reg, REG_STACK(reg));
    fprintf(out, "  extb.l d%d\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  move.b (d%d,a2), d7\n", index_reg);
    fprintf(out, "  extb.l d7\n");
    fprintf(out, "  move.l d7, -(SP)\n");
    stack++;
  }

  return 0;
}

int MC68020::array_read_byte(std::string &name, int field_id)
{
  fprintf(out, "  movea.l (%s,a4), a2\n", name.c_str());

  if (stack > 0)
  {
    fprintf(out, "  add.l (SP)+, a2\n");
    fprintf(out, "  move.b (a2), d7\n");
    fprintf(out, "  extb.l d7\n");
    fprintf(out, "  move.l d7, -(SP)\n");
  }
    else
  {
    fprintf(out, "  move.b (d%d,a2), d%d\n", REG_STACK(reg-1),REG_STACK(reg-1));
    fprintf(out, "  extb.l d%d\n", REG_STACK(reg-1));
  }

  return 0;
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



