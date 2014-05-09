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
#include <string.h>
#include <stdint.h>

#include "ATTINY13.h"

ATTINY13::ATTINY13()
{

}

ATTINY13::~ATTINY13()
{
}

int ATTINY13::start_init()
{
  // java stack pointer
  fprintf(out, "  ldi SP, 0xf\n");

  // java stack base locations
  fprintf(out, "stack_lo equ 0x60\n");
  fprintf(out, "stack_hi equ 0x70\n");

  // RAMEND (change to particular chip)
  fprintf(out, "RAMEND equ 0x9f\n");

  // heap
  fprintf(out, "ram_start equ 0x80\n");
  fprintf(out, "heap_ptr equ ram_start\n\n");

  // processor stack setup
  // fprintf(out, "  ldi r16, RAMEND & 0xff\n");
  // fprintf(out, "  out SPL, r16\n");
  // fprintf(out, "  ldi r16, RAMEND >> 8\n");
  // fprintf(out, "  out SPH, r16\n");

  need_farjump = 0;

  return 0;
}


#if 0
void ATTINY13::close()
{
}
#endif

