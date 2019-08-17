/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2019 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "generator/Amiga.h"

#define HEAP_SIZE 100 * 1024

// NOTE: a3 points to Amiga hardware registers.

Amiga::Amiga()
{
  //start_org = 0x8000;
}

Amiga::~Amiga()
{
  fprintf(out,
    "  ;; Mem(100k, CHIP_MEM=2)\n"
    "  movea.l (ExecBase), a2\n"
    "  move.l #%d, d0\n"
    "  move.l #2, d1\n"
    "  jsr (FreeMem,a2)\n"
    "  movea.l d0, a5\n",
    HEAP_SIZE);

  fprintf(out,
    ".align_bytes 4\n"
    "_amiga_grind_end:\n\n"
    "  ;; Hunk file end\n"
    "  dc32 0x0000_03f2\n\n"
  );

}

int Amiga::open(const char *filename)
{
  if (MC68000::open(filename) != 0) { return -1; }

  return 0;
}

int Amiga::start_init()
{
  // Add any set up items (stack, registers, etc).
  //MC68000::start_init();

  fprintf(out,
    ".include \"amiga/hardware.inc\"\n"
    ".include \"amiga/exec.inc\"\n"
  );

  fprintf(out,
    "\n"
    "  ;; Hunk file header\n"
    "  dc32 0x0000_03f3  ; magic_cookie\n"
    "  dc32 0x0000_0000  ; name_length\n"
    "  dc32 0x0000_0001  ; table_length\n"
    "  dc32 0x0000_0000  ; first_hunk\n"
    "  dc32 0x0000_0000  ; last_hunk\n"
    "  dc32 (_amiga_grind_end - _amiga_grind_start) / 4\n"
    // FIXME: Statics need to be in chip RAM if the custom chips use them
    //"  dc32 0x0000_03e9  ; hunk_code\n\n"
    "  dc32 0x4000_03e9  ; hunk_code\n\n"
    "  dc32 (_amiga_grind_end - _amiga_grind_start) / 4\n"
    "_amiga_grind_start:\n"
  );

  fprintf(out,
    "  ;; a3 points to custom chips.\n"
    "  movea.l #0xdff000, a3\n");

  fprintf(out,
    "  ;; heap = AllocMem(100k, CHIP_MEM=2)\n"
    "  movea.l (ExecBase), a2\n"
    "  move.l #%d, d0\n"
    "  move.l #2, d1\n"
    "  jsr (AllocMem,a2)\n"
    "  movea.l d0, a5\n",
    HEAP_SIZE);

  return 0;
}

int Amiga::amiga_disableMultitasking()
{
  fprintf(out,
    "  ;; amiga_disableMultitasking()\n"
    "  movea.l (ExecBase), a2\n"
    "  jsr (Forbid,a2)\n");

  return 0;
}

int Amiga::amiga_enableMultitasking()
{
  fprintf(out,
    "  ;; amiga_enableMultitasking()\n"
    "  movea.l (ExecBase), a2\n"
    "  jsr (Permit,a2)\n");

  return 0;
}

int Amiga::amiga_disableInterrupts()
{
  fprintf(out,
    "  ;; amiga_disableInterrupts()\n"
    "  movea.l (ExecBase), a2\n"
    "  jsr (Disable,a2)\n");

  return 0;
}

int Amiga::amiga_enableInterrupts()
{
  fprintf(out,
    "  ;; amiga_enableInterrupts()\n"
    "  movea.l (ExecBase), a2\n"
    "  jsr (Enable,a2)\n");

  return 0;
}

int Amiga::amiga_setPalette_II()
{
  fprintf(out,
    "  ;; amiga_setPalette_II()\n"
    "  lea (COLOR00,a3), a2\n"
    "  lsl.w #1, d%d\n"
    "  move.w d%d, (0,a2,d%d)\n",
    reg - 2, reg - 1, reg - 2);

  reg -= 2;

  return 0;
}

int Amiga::amiga_setSpriteImage_IaI()
{
  fprintf(out,
    "  ;; amiga_setSpriteImage_IaI()\n"
    "  andi.l #0x7, d%d\n"
    "  lsl.w #2, d%d\n"
    "  add.w #SPR0PTH, d%d\n"
    "  move.l d%d, (0,a3,d%d)\n",
    reg - 2,
    reg - 2,
    reg - 2,
    reg - 1,
    reg - 2
  );

  reg -= 2;

  return 0;
}

int Amiga::amiga_setSpritePosition_IIII()
{
  // Clean up input first:
  // index = (index & 7) * 4  <- make sure index is 0 to 7 and multiply by 8

  fprintf(out,
    "  ;; amiga_setSpritePosition_IIII()\n"
    "  andi.l #0x7, d%d\n"
    "  lsl.w #3, d%d\n"
    "  andi.w #0x1ff, d%d\n"
    "  andi.w #0x1ff, d%d\n"
    "  andi.w #0x1ff, d%d\n",
    reg - 4,
    reg - 4,
    reg - 3,
    reg - 2,
    reg - 1
  );

  // Need to fit data in:
  // SPR0POS 0x140
  //   15 to 8: VSTART low  8 bits
  //    7 to 0: HSTART high 8 bits
  // SPR0CTL 0x142
  //   15 to 8: VSTOP  low  8 bits
  //         2: VSTART high 1 bit
  //         1: VSTOP  high 1 bit
  //         0: HSTART low  1 bit

  // hstart = hstart >> 1
  // d5     = hstart & 1

  fprintf(out,
    "  move.w d%d, d5\n"
    "  lsr.w #1, d5\n"
    "  andi.w #1, d%d\n",
    reg - 3,
    reg - 3
  );

  // vstart = vstart & 0xff
  // d6     = (vstart & 0x100) >> 6
  // d5     = d5 | d6

  fprintf(out,
    "  move.w d%d, d6\n"
    "  andi.w #0x100, d6\n"
    "  lsr.w #6, d6\n"
    "  andi.w #0xff, d%d\n"
    "  or.w d5, d6\n",
    reg - 2,
    reg - 2
  );

  // SPRxPOS = (vstart << 8) | hstart

  fprintf(out,
    "  addi.w #SPR0POS, d%d\n"
    "  or.w d%d, d%d\n"
    "  move.w d%d, (0,a3,d%d)\n",
    reg - 4,
    reg - 3, reg - 2,
    reg - 2, reg - 4);

  // temp |= (vstop & 0x100) >> 7
  // vstop = vstop & 0xff
  // temp |= (vstop << 8)
  // SPR0CTL = temp

  fprintf(out,
    "  move.w d%d, d6\n"
    "  andi.w #0x100, d6\n"
    "  lsr.w #7, d6\n"
    "  andi.w #0xff, d%d\n"
    "  or.w d6, d5\n"
    "  lsl.w #8, d%d\n"
    "  or.w d%d, d5\n"
    "  move.w d5, (2,a3,d%d)\n",
    reg - 1,
    reg - 1,
    reg - 1,
    reg - 1,
    reg - 1
  );

  reg -= 4;

  return 0;
}

int Amiga::amiga_setVideoMode_IBBB()
{
  // BPU2-BPU0 = shift to bit position 12
  fprintf(out,
    "  ;; amiga_setVideoMode_IBBB()\n"
    "  and.w #7, d%d\n"
    "  lsl.w #8, d%d\n"
    "  lsl.w #4, d%d\n",
    reg - 4,
    reg - 4,
    reg - 4);

  // HIRES=15, HOMOD=11, DBLPF=10
  fprintf(out,
    "  lsl.w #8, d%d\n"
    "  lsl.w #7, d%d\n"
    "  lsl.w #8, d%d\n"
    "  lsl.w #3, d%d\n"
    "  lsl.w #8, d%d\n"
    "  lsl.w #2, d%d\n",
    reg - 3,
    reg - 3,
    reg - 2,
    reg - 2,
    reg - 1,
    reg - 1);

  fprintf(out,
    "  or.w d%d, d%d\n"
    "  or.w d%d, d%d\n"
    "  or.w d%d, d%d\n"
    "  mov.w d%d, (BPLCON0,a3)\n",
    reg - 3, reg - 4,
    reg - 2, reg - 4,
    reg - 1, reg - 4,
    reg - 4);

  reg -= 4;

  return 0;
}

int Amiga::amiga_setPlayfieldScroll_II()
{
  fprintf(out,
    "  ;; amiga_setPlayfieldScroll_II()\n"
    "  and.w #0xf, d%d\n"
    "  and.w #0xf, d%d\n"
    "  lsl.w #4, d%d\n"
    "  or.w d%d, d%d\n"
    "  mov.w d%d, (BPLCON1,a3)\n",
    reg - 1,
    reg - 2,
    reg - 1,
    reg - 1, reg - 2,
    reg - 2);

  reg -= 2;

  return 0;
}

int Amiga::amiga_setPlayfieldPriority_IIB()
{
  fprintf(out,
    "  ;; amiga_setPlayfieldPriority_IIB()\n"
    "  and.w #0x7, d%d\n"
    "  and.w #0x7, d%d\n"
    "  lsl.w #3, d%d\n"
    "  lsl.w #6, d%d\n"
    "  or.w d%d, d%d\n"
    "  or.w d%d, d%d\n"
    "  mov.w d%d, (BPLCON2,a3)\n",
    reg - 2,
    reg - 3,
    reg - 2,
    reg - 1,
    reg - 3, reg - 1,
    reg - 2, reg - 1,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::copper_setWait_aIIII()
{
  fprintf(out,
    "  ;; copper_setWait_aIIII()\n");

  return 0;
}

int Amiga::copper_setColor_aIIII()
{
  return 0;
}

int Amiga::copper_setBitplane_aIIII()
{
  return 0;
}

int Amiga::copper_setMove_aIIII()
{
  return 0;
}

int Amiga::copper_setSkip_aIIII()
{
  return 0;
}

int Amiga::copper_setWaitEnd_aII()
{
  return 0;
}

