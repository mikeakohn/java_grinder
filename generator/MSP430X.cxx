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
#include <stdint.h>

#include "MSP430X.h"

// ABI is:
// r4 top of stack
// r5
// r6
// r7
// r8
// r9
// r10
// r11 top of stack
// r12 points to locals
// r15 is temp

#define REG_STACK(a) (a + 4)
#define LOCALS(a) ((a * 2) + 2)

MSP430X::MSP430X(uint8_t chip_type) : MSP430(chip_type)
{

}

MSP430X::~MSP430X()
{

}

int MSP430X::shift_left_integer()
{
  if (stack > 0)
  {
    fprintf(out, "  pop r15\n");
    stack--;
  }
    else
  if (reg > 0)
  {
    fprintf(out, "  mov.w r%d, r15\n", REG_STACK(reg-1));
    reg--;
  }

  fprintf(out, "  repeat r15\n");

  if (stack > 0)
  {
    fprintf(out, "  rlam.w #1, @sp\n");
  }
    else
  {
    fprintf(out, "  rlam.w #1, r%d\n", REG_STACK(reg-1));
  }

  return 0;
}

int MSP430X::shift_right_integer()
{
  if (stack > 0)
  {
    fprintf(out, "  pop r15\n");
    stack--;
  }
    else
  if (reg > 0)
  {
    fprintf(out, "  mov.w r%d, r15\n", REG_STACK(reg-1));
    reg--;
  }

  fprintf(out, "  repeat r15\n");

  if (stack > 0)
  {
    fprintf(out, "  rram.w #1, @sp\n");
  }
    else
  {
    fprintf(out, "  rram.w #1, r%d\n", REG_STACK(reg-1));
  }

  return 0;
}

int MSP430X::shift_right_uinteger()
{
  if (stack > 0)
  {
    fprintf(out, "  pop r15\n");
    stack--;
  }
    else
  if (reg > 0)
  {
    fprintf(out, "  mov.w r%d, r15\n", REG_STACK(reg-1));
    reg--;
  }

  fprintf(out, "  repeat r15\n");

  if (stack > 0)
  {
    fprintf(out, "  rrum.w #1, @sp\n");
  }
    else
  {
    fprintf(out, "  rrum.w #1, r%d\n", REG_STACK(reg-1));
  }

  return 0;
}


