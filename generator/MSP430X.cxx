/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2021 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "generator/MSP430X.h"

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

MSP430X::MSP430X(uint8_t chip_type) :
  MSP430(chip_type),
  need_set_vcore_up(false)
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
  if (need_set_vcore_up) { insert_set_vcore_up(); }
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
  need_set_vcore_up = true;

  fprintf(out,
    "  ;; Increase CPU voltage for faster MCLK\n"
    "  mov.w #1, r15\n"
    "  call #set_vcore_up\n"
    "  mov.w #2, r15\n"
    "  call #set_vcore_up\n"
    "  mov.w #3, r15\n"
    "  call #set_vcore_up\n\n"
    "  ;; Set up DCO to 25MHz\n"
    "  bis.w #SCG0, SR\n"
    "  mov.w #SELREF__REFOCLK, &UCSCTL3\n"
    "  mov.w #XT1OFF|XT2OFF, &UCSCTL6\n"
    "  mov.w #0, &UCSCTL0\n"
    "  mov.w #DCORSEL_6, &UCSCTL1\n"
    "  mov.w #762, &UCSCTL2\n"
    "  mov.w #SELA__REFOCLK|SELS__DCOCLK|SELM__DCOCLK, &UCSCTL4\n"
    "  bic.w #SCG0, SR\n"
    "wait_clock_%d:\n"
    "  bic.w #XT2OFFG|XT1LFOFFG|XT1HFOFFG|DCOFFG, &UCSCTL7\n"
    "  bic.w #OFIFG, &SFRIFG1\n"
    "  bit.w #OFIFG, &SFRIFG1\n"
    "  jnz wait_clock_%d\n",
    label_count,
    label_count);

  label_count++;

  cpu_speed = 25000000;

  return 0;
}

int MSP430X::cpu_setClockExternal2()
{
  need_set_vcore_up = true;

  fprintf(out,
    "  ;; Increase CPU voltage for faster MCLK\n"
    "  mov.w #1, r15\n"
    "  call #set_vcore_up\n"
    "  mov.w #2, r15\n"
    "  call #set_vcore_up\n"
    "  mov.w #3, r15\n"
    "  call #set_vcore_up\n\n"
    "  ;; Turn on XT2 crystal\n"
    "  bis.b #0x0c, &P5SEL\n"
    "  mov.w #SELS_5|SELM_5|SELA_2, &UCSCTL4\n"
    "  bic.w #SELREF_7, &UCSCTL3\n"
    "  bis.w #SELREF_2, &UCSCTL3\n"
    "  mov.w #0, &UCSCTL0\n"
    "  bic.w #XT2OFF, &UCSCTL6\n"
    "wait_clock:\n"
    "  bic.w #XT2OFFG|XT1LFOFFG|XT1HFOFFG|DCOFFG, &UCSCTL7\n"
    "  bic.w #OFIFG, &SFRIFG1\n"
    "  mov.w #5000, r15\n"
    "dec_again:\n"
    "  dec r15\n"
    "  jnz dec_again\n"
    "  bit.w #OFIFG, &SFRIFG1\n"
    "  jnz wait_clock\n"
    "  mov.w #0, &UCSCTL5\n\n"
    "  mov.w #SELS_5|SELM_5|SELA_2, &UCSCTL4\n");

  return 0;
}

// Timer functions
int MSP430X::timer_setInterval_II(int cycles, int divider)
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

  fprintf(out, "  ;; MSP430X::timer_setInterval_II(%d, %d)\n", cycles, divider);
  fprintf(out, "  mov.w #%d, &TA0CCR0\n", cycles);
  fprintf(out, "  mov.w #(TASSEL_2|MC_1|%d), &TA0CTL ; SMCLK, DIV1, COUNT to TA0CCR0\n", divider << 6);
  fprintf(out, "  mov.w #0, &TA0CCTL1\n");

  return 0;
}

int MSP430X::timer_setListener_Z(int const_value)
{
  fprintf(out, "  ;; MSP430X::timer_setListener_II(%d)\n", const_value);

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

int MSP430X::timer_setValue_I()
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

int MSP430X::timer_setValue_I(int const_value)
{
  fprintf(out, "  mov.w #%d, &TA0R\n", const_value);
  return 0;
}

void MSP430X::insert_set_vcore_up()
{
  // Translated from C to Assmembly from TI's msp430x5xx family PDF
  // slau208m - June 2008 - Revised February 2013, page 108

  fprintf(out, "set_vcore_up:\n");

  // Open PMM registers for write access
  fprintf(out, "  mov.b #0xa5, &PMMCTL0_H\n");
  // Make sure no flags are set for iterative sequences
/* The manual says to do this.. I see someone else not doing this.. and
   this hangs if it's done.. :(
  fprintf(out,
    "set_vcore_up_svsmhdlyifg_1:\n"
    "  bit.w #SVSMHDLYIFG, &PMMIFG\n"
    "  jz set_vcore_up_svsmhdlyifg_1\n"
    "set_vcore_up_svsmldlyifg_1:\n"
    "  bit.w #SVSMLDLYIFG, &PMMIFG\n"
    "  jz set_vcore_up_svsmldlyifg_1\n");
*/

  // Set SVS/SVM high side new level
  //SVSMHCTL = SVSHE + (SVSHRVL0 * level) + SVMHE + (SVSMHRRL0 * level);
  fprintf(out,
    "  mov.w r15, r14\n"
    "  rlam.w #4, r14\n"
    "  rlam.w #4, r14\n"
    "  mov.w r14, r13\n"
    "  bis.w #SVSHE|SVMHE, r13\n"
    "  mov.w r13, &SVSMHCTL\n");

  // Set SVM low side to new level
  //SVSMLCTL = SVSLE + SVMLE + (SVSMLRRL0 * level);
  fprintf(out,
    "  mov.w r15, r13\n"
    "  bis.w #SVSLE|SVMLE, r13\n"
    "  mov.w r13, &SVSMLCTL\n");

  // Wait till SVM is settled
  fprintf(out,
    "set_vcore_up_svsmldlyifg_2:\n"
    "  bit.w #SVSMLDLYIFG, &PMMIFG\n"
    "  jz set_vcore_up_svsmldlyifg_2\n");

  // Clear already set flags
  fprintf(out, "  bic.w #SVMLVLRIFG|SVMLIFG, &PMMIFG\n");

  // Set VCore to new level
  //PMMCTL0_L = PMMCOREV0 * level;
  fprintf(out, "  mov.b r15, &PMMCTL0_L\n");

  // Wait till new level reached
  fprintf(out,
    "  bit.w #SVMLIFG, &PMMIFG\n"
    "  jz set_vcore_up_skip_level_check\n"
    "set_vcore_up_level_check:\n"
    "  bit.w #SVMLVLRIFG, &PMMIFG\n"
    "  jz set_vcore_up_level_check\n"
    "set_vcore_up_skip_level_check:\n");

  // Set SVS/SVM low side to new level
  //SVSMLCTL = SVSLE + (SVSLRVL0 * level) + SVMLE + (SVSMLRRL0 * level);
  fprintf(out,
    "  bis.w #SVSLE|SVMLE, r14\n"
    "  mov.w r14, &SVSMLCTL\n");

  // Lock PMM registers for write access
  fprintf(out,
    "  mov.b #0x00, &PMMCTL0_H\n"
    "  ret\n\n");
}

