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
  // Looks like most of the MSP430F55xx line are using this value, but
  // if other chips are added this can move to the switch/case.
  ram_start = 0x2400;
  vector_timer = 0xffea;

  switch(chip_type)
  {
    case MSP430F5529:
      // There are 4 sections of flash, and 4 2k sections of RAM.
      // Not sure how to get to this RAM easily.
      flash_start = 0x4400;
      stack_start = 0x4400;
      include_file = "msp430f5529.inc";
      break;
    default:
      break;
  }
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

  fprintf(out, "  rpt r15\n");

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

int MSP430X::shift_left_integer(int count)
{
  if (stack != 0) { return -1; }
  if (count >= 1 && count <= 4)
  {
    fprintf(out, "  rlam.w #%d, r%d\n", count, REG_STACK(reg-1));
    return 0;
  }

  if (count >= 1 && count <= 16)
  {
    fprintf(out, "  rpt #%d\n", count);
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

  fprintf(out, "  rpt r15\n");

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

int MSP430X::shift_right_integer(int count)
{
  if (stack != 0) { return -1; }
  if (count >= 1 && count <= 4)
  {
    fprintf(out, "  rram.w #%d, r%d\n", count, REG_STACK(reg-1));
    return 0;
  }

  if (count >= 1 && count <= 16)
  {
    fprintf(out, "  rpt #%d\n", count);
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

  fprintf(out, "  rpt r15\n");

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

int MSP430X::shift_right_uinteger(int count)
{
  if (stack != 0) { return -1; }
  if (count >= 1 && count <= 4)
  {
    fprintf(out, "  rrum.w #%d, r%d\n", count, REG_STACK(reg-1));
    return 0;
  }

  if (count >= 1 && count <= 16)
  {
    fprintf(out, "  rpt #%d\n", count);
    fprintf(out, "  rrum.w #1, r%d\n", REG_STACK(reg-1));
  }

  return 0;
}

// CPU functions
int MSP430X::cpu_setClock25()
{
  fprintf(out, "  ;; Set up DCO to 25MHz\n");
  fprintf(out, "  bis.w #SCG0, SR\n");
  fprintf(out, "  mov.w #SELREF__REFOCLK, &UCSCTL3\n");
  fprintf(out, "  mov.w #XT1OFF|XT2OFF, &UCSCTL6\n");
  fprintf(out, "  mov.w #0, &UCSCTL0\n");
  fprintf(out, "  mov.w #DCORSEL_6, &UCSCTL1\n");
  fprintf(out, "  mov.w #762, &UCSCTL2\n");
  fprintf(out, "  mov.w #SELA__REFOCLK|SELS__DCOCLK|SELM__DCOCLK, &UCSCTL4\n");
  fprintf(out, "  bic.w #SCG0, SR\n");
  fprintf(out, "wait_clock_%d:\n", label_count);
  fprintf(out, "  bic.w #XT2OFFG|XT1LFOFFG|XT1HFOFFG|DCOFFG, &UCSCTL7\n");
  fprintf(out, "  bic.w #SFRIFG1, &OFIFG\n");
  fprintf(out, "  bit.w #SFRIFG1, &OFIFG\n");
  fprintf(out, "  jnz wait_clock_%d\n", label_count);

  label_count++;

  return 0;
}

// Timer functions
int MSP430X::timer_setInterval(int cycles, int divider)
{
  if (cycles > 0xffff)
  {
    printf("** Error cycles is more than 16 bit\n"); 
    return -1;
  }

  if (divider == 1) { divider = 0; }
  else if (divider == 2) { divider = 1; }
  else if (divider == 4) { divider = 2; }
  else if (divider == 8) { divider = 3; }
  else { printf("** divider must be 1,2,4,8\n"); return -1; }

  fprintf(out, "  mov.w #%d, &TA0CCR0\n", cycles);
  fprintf(out, "  mov.w #(TASSEL_2|MC_1|%d), &TA0CTL ; SMCLK, DIV1, COUNT to TA0CCR0\n", divider << 6);
  fprintf(out, "  mov.w #0, &TA0CCTL1\n");

  return 0;
}

int MSP430X::timer_setListener(int const_value)
{
  if (const_value != 0)
  {
    need_timer_interrupt = true;
    fprintf(out, "  bis.w #CCIE, &TA0CCTL0\n");
    fprintf(out, "  eint\n");
  }
    else
  {
    fprintf(out, "  bic.w #CCIE, &TA0CCTL0\n");
  }
  return 0;
}

int MSP430X::timer_getValue()
{
  if (reg < reg_max)
  {
    fprintf(out, "  mov.w &TA0R, r%d\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  mov.w &TA0R, r15\n");
    fprintf(out, "  push r15\n");
    stack++;
  }

  return 0;
}

int MSP430X::timer_setValue()
{
  if (stack != 0)
  {
    fprintf(out, "  pop r15\n");
    fprintf(out, "  mov.w r15, &TA0R\n");
    stack--;
  }
    else
  {
    fprintf(out, "  mov.w r%d, &TA0R\n", REG_STACK(reg-1));
    reg--;
  }

  return 0;
}

int MSP430X::timer_setValue(int const_value)
{
  fprintf(out, "  mov.w #%d, &TA0R\n", const_value);
  return 0;
}

