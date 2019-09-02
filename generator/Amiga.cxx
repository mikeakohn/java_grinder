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

#define HEAP_SIZE 250 * 1024

// NOTE: a3 points to Amiga hardware registers.

Amiga::Amiga()
{
  //start_org = 0x8000;
}

Amiga::~Amiga()
{
#if 0
  fprintf(out,
    "  ;; Mem(100k, CHIP_MEM=2)\n"
    "  move.l a6, (a7)-\n"
    "  movea.l (ExecBase), a6\n"
    "  move.l #%d, d0\n"
    "  move.l #2, d1\n"
    "  jsr (FreeMem,a6)\n"
    "  movea.l (a7)+, a6\n",
    HEAP_SIZE);
#endif

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
    // 0x4000 = flags, 0x03e9 = code
    //"  dc32 0x0000_03e9  ; hunk_code\n\n"
    "  dc32 0x4000_03e9  ; hunk_code\n\n"
    "  dc32 (_amiga_grind_end - _amiga_grind_start) / 4\n"
    "_amiga_grind_start:\n"
  );

  fprintf(out,
    "  ;; a3 points to custom chips.\n"
    "  movea.l #0xdff000, a3\n");

  return 0;
}

int Amiga::init_heap(int field_count)
{
  fprintf(out, "  ;; Setup heap and static initializers\n");

  fprintf(out,
    "  ;; heap = AllocMem(100k, CHIP_MEM=2)\n"
    "  move.l a6, -(a7)\n"
    "  movea.l (ExecBase), a6\n"
    "  move.l #%d, d0\n"
    "  moveq #2, d1\n"
    "  jsr (AllocMem,a6)\n"
    "  movea.l (a7)+, a6\n"
    "  movea.l d0, a5\n",
    HEAP_SIZE);

  return 0;
}

int Amiga::new_object(std::string &object_name, int field_count)
{
  fprintf(out, "  ; new_object(object_name=%s field_count=%d)\n",
    object_name.c_str(), field_count);

  if (object_name != "net/mikekohn/java_grinder/amiga/Copper" &&
      object_name != "net/mikekohn/java_grinder/amiga/Blitter")
  {
     printf("Error: Unknown class %s\n", object_name.c_str());
     return -1;
  }

#if 0
  fprintf(out,
    "  move.l #0, (a5)\n"
    "  lsl.l #2, d%d\n"
    "  add.l #%d, a5\n"
    "  add.l d%d, a5\n",
    reg - 1,
    field_count * 4,
    reg - 1);

  reg -= 2;
#endif

  reg++;

  return 0;
}

int Amiga::amiga_disableMultitasking()
{
  fprintf(out,
    "  ;; amiga_disableMultitasking()\n"
    "  move.l a6, -(a7)\n"
    "  movea.l (ExecBase), a6\n"
    "  jsr (Forbid,a6)\n"
    "  movea.l (a7)+, a6\n");

  return 0;
}

int Amiga::amiga_enableMultitasking()
{
  fprintf(out,
    "  ;; amiga_enableMultitasking()\n"
    "  move.l a6, -(a7)\n"
    "  movea.l (ExecBase), a6\n"
    "  jsr (Permit,a6)\n"
    "  movea.l (a7)+, a6\n");

  return 0;
}

int Amiga::amiga_disableInterrupts()
{
  fprintf(out,
    "  ;; amiga_disableInterrupts()\n"
    "  move.l a6, -(a7)\n"
    "  movea.l (ExecBase), a6\n"
    "  jsr (Disable,a6)\n"
    "  movea.l (a7)+, a6\n");

  return 0;
}

int Amiga::amiga_enableInterrupts()
{
  fprintf(out,
    "  ;; amiga_enableInterrupts()\n"
    "  move.l a6, -(a7)\n"
    "  movea.l (ExecBase), a6\n"
    "  jsr (Enable,a6)\n"
    "  movea.l (a7)+, a6\n");

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

int Amiga::amiga_setVideoMode_I()
{
  fprintf(out,
    "  ;; amiga_setVideoMode_I()\n"
    "  move.w d%d, (BPLCON0,a3)\n",
    reg - 1);

  reg -= 1;

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
    "  move.w d%d, (BPLCON1,a3)\n",
    reg - 1,
    reg - 2,
    reg - 1,
    reg - 1, reg - 2,
    reg - 2);

  reg -= 2;

  return 0;
}

int Amiga::amiga_setPlayfieldPriority_IIZ()
{
  fprintf(out,
    "  ;; amiga_setPlayfieldPriority_IIZ()\n"
    "  and.w #0x1, d%d\n"
    "  and.w #0x7, d%d\n"
    "  and.w #0x7, d%d\n"
    "  lsl.w #3, d%d\n"
    "  lsl.w #6, d%d\n"
    "  or.w d%d, d%d\n"
    "  or.w d%d, d%d\n"
    "  move.w d%d, (BPLCON2,a3)\n",
    reg - 1,
    reg - 2,
    reg - 3,
    reg - 2,
    reg - 1,
    reg - 3, reg - 1,
    reg - 2, reg - 1,
    reg - 1);

  reg -= 3;

  return 0;
}

int Amiga::amiga_setBitplaneModuloEven_I()
{
  fprintf(out,
    "  ;; amiga_setBitplaneModuloEven_I()\n"
    "  move.w d%d, (BPL1MOD,a3)\n",
    reg - 1);

  reg -= 1;

  return 0;
}

int Amiga::amiga_setBitplaneModuloOdd_I()
{
  fprintf(out,
    "  ;; amiga_BitplaneModuloOdd_I()\n"
    "  move.w d%d, (BPL2MOD,a3)\n",
    reg - 1);

  reg -= 1;

  return 0;
}

int Amiga::amiga_setDisplayWindowStart_II()
{
  const int horizontal = reg - 2;
  const int vertical = reg - 1;

  fprintf(out,
    "  ;; amiga_setDisplayWindowStart_II()\n"
    "  lsl.w #8, d%d\n"
    "  or.w d%d, d%d\n"
    "  move.w d%d, (DIWSTRT,a3)\n",
    vertical,
    vertical, horizontal,
    horizontal);

  reg -= 2;

  return 0;
}

int Amiga::amiga_setDisplayWindowStop_II()
{
  const int horizontal = reg - 2;
  const int vertical = reg - 1;

  fprintf(out,
    "  ;; amiga_setDisplayWindowStop_II()\n"
    "  lsl.w #8, d%d\n"
    "  or.w d%d, d%d\n"
    "  move.w d%d, (DIWSTOP,a3)\n",
    vertical,
    vertical, horizontal,
    horizontal);

  reg -= 2;

  return 0;
}

int Amiga::amiga_setDisplayBitplaneStart_I()
{
  fprintf(out,
    "  ;; amiga_setDisplayBitplaneStart_I()\n"
    "  move.w d%d, (DDFSTRT,a3)\n",
    reg - 1);

  reg -= 1;

  return 0;
}

int Amiga::amiga_setDisplayBitplaneStop_I()
{
  fprintf(out,
    "  ;; amiga_setDisplayBitplaneStop_I()\n"
    "  move.w d%d, (DDFSTOP,a3)\n",
    reg - 1);

  reg -= 1;

  return 0;
}

int Amiga::amiga_setDMA_I()
{
  fprintf(out,
    "  ;; amiga_setDMA_I()\n"
    "  or.w #0x8000, d%d\n"
    "  move.w d%d, (DMACON,a3)\n",
    reg - 1,
    reg - 1);

  reg -= 1;

  return 0;
}

int Amiga::amiga_clearDMA_I()
{
  fprintf(out,
    "  ;; amiga_clearDMA_I()\n"
    "  move.w d%d, (DMACON,a3)\n",
    reg - 1);

  reg -= 1;

  return 0;
}

int Amiga::copper_Constructor_I()
{
  fprintf(out,
    "  ;; copper_Constructor_I()\n");

  // Push address of Copper onto the stack.
  fprintf(out,
    "  move.l a5, d%d\n"
    "  add.l #4, d%d\n",
    reg - 3,
    reg - 3);

  // Allocate memory from the heap.
  fprintf(out,
    "  move.l #0, (a5)\n"
    "  lsl.l #2, d%d\n"
    "  add.l #4, a5\n"
    "  add.l d%d, a5\n",
    reg - 1,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::copper_appendInstruction_I()
{
  const int object = reg - 2;
  const int value = reg - 1;

  fprintf(out, "  ;; copper_appendInstruction_I()\n");

  copper_getNextIndexAndIncrement(object);

  fprintf(out,
    "  move.l d%d, (0,a2,d5)\n",
    value);

  reg -= 2;

  return 0;
}

int Amiga::copper_appendWait_II()
{
  const int object = reg - 3;
  const int horizontal = reg - 2;
  const int vertical = reg - 1;

  fprintf(out, "  ;; copper_appendWait_II()\n");

  copper_getNextIndexAndIncrement(object);

  fprintf(out,
    "  and.l #0x7f, d%d\n"
    "  and.l #0xff, d%d\n"
    "  lsl.l #1, d%d\n"
    "  lsl.l #8, d%d\n"
    "  or.l d%d, d%d\n"
    "  or.l #1, d%d\n"
    "  swap d%d\n"
    "  or.w #0xfffe, d%d\n"
    "  move.l d%d, (0,a2,d5)\n",
    horizontal,
    vertical,
    horizontal,
    vertical,
    horizontal, vertical,
    vertical,
    vertical,
    vertical,
    vertical);

  reg -= 3;

  return 0;
}

int Amiga::copper_appendSkip_II()
{
  const int object = reg - 3;
  const int horizontal = reg - 2;
  const int vertical = reg - 1;

  fprintf(out, "  ;; copper_appendWait_II()\n");

  copper_getNextIndexAndIncrement(object);

  fprintf(out,
    "  and.l #0x7f, d%d\n"
    "  and.l #0xff, d%d\n"
    "  lsl.l #1, d%d\n"
    "  lsl.l #8, d%d\n"
    "  or.l d%d, d%d\n"
    "  or.l #1, d%d\n"
    "  swap d%d\n"
    "  or.w #0xffff, d%d\n"
    "  move.l d%d, (0,a2,d5)\n",
    horizontal,
    vertical,
    horizontal,
    vertical,
    horizontal, vertical,
    vertical,
    vertical,
    vertical,
    vertical);

  reg -= 3;

  return 0;
}

int Amiga::copper_appendSetColor_II()
{
  const int object = reg - 3;
  const int index = reg - 2;
  const int value = reg - 1;

  fprintf(out, "  ;; copper_appendSetColor_II()\n");

  copper_getNextIndexAndIncrement(object);

  fprintf(out,
    "  lsl.w #1, d%d\n"
    "  add.w #COLOR00, d%d\n"
    "  move.w d%d, (0,a2,d5)\n"
    "  move.w d%d, (2,a2,d5)\n",
    index,
    index,
    index,
    value);

  reg -= 3;

  return 0;
}

int Amiga::copper_appendSetBitplane_IaB()
{
  const int object = reg - 3;
  const int index = reg - 2;
  const int value = reg - 1;

  fprintf(out, "  ;; copper_appendSetBitplane_IaB()\n");

  fprintf(out,
    "  movea.l d%d, a2\n"
    "  move.l (-4,a2), d5\n"
    "  add.l #8, (-4,a2)\n",
    object);

  fprintf(out,
    "  lsl.w #2, d%d\n"
    "  add.w #BPL1PTH, d%d\n"
    "  move.w d%d, (0,a2,d5)\n"
    "  move.l d%d, (2,a2,d5)\n"
    "  addq.w #2, d%d\n"
    //"  swap d%d\n"
    "  move.w d%d, (4,a2,d5)\n"
    "  move.w d%d, (6,a2,d5)\n",
    index,
    index,
    index,
    value,
    index,
    //value,
    index,
    value);

  reg -= 3;

  return 0;
}

int Amiga::copper_appendEnd()
{
  const int object = reg - 1;

  fprintf(out, "  ;; copper_appendEnd()\n");

  copper_getNextIndexAndIncrement(object);

  fprintf(out, "  move.l #0xfffffffe, (0,a3,d5)\n");

  reg -= 1;

  return 0;
}

int Amiga::copper_resetIndex()
{
  fprintf(out,
    "  ;; copper_resetIndex()\n"
    "  movea.l d%d, a2\n"
    "  move.l #0, (-4,a2)\n",
    reg - 1);

  reg -= 1;

  return 0;
}

int Amiga::copper_setIndex_I()
{
  fprintf(out,
    "  ;; copper_resetIndex()\n"
    "  movea.l d%d, a2\n"
    "  move.l d%d, (-4,a2)\n",
    reg - 2,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::copper_run()
{
  fprintf(out,
    "  ;; copper_run()\n"
    "  move.l d%d, (COP1LCH,a3)\n"
    "  move.w #0x8080, (DMACON,a3)\n"
    "  move.w d5, (COPJMP1,a3)\n",
    reg - 1);

  reg -= 1;

  return 0;
}

int Amiga::copper_stop()
{
  fprintf(out,
    "  ;; copper_run()\n"
    "  move.w #0x0080, (DMACON,a3)\n");

  reg -= 1;

  return 0;
}

int Amiga::copper_getArrayAsInt()
{
  fprintf(out, "  ;; copper_getArrayAsInt()\n");

  return 0;
}

int Amiga::copper_getArrayAsChar()
{
  fprintf(out, "  ;; copper_getArrayAsChar()\n");

  return 0;
}

int Amiga::blitter_Constructor()
{
  fprintf(out,
    "  ;; blitter_Constructor()\n");

  // Push address of Blitter onto the stack.
  fprintf(out, "  move.l a5, d%d\n", reg - 2);

  // Structure is:
  //  0: uint16_t control_0;
  //  2: uint16_t control_1;
  //  4: uint16_t mask_word_first;
  //  6: uint16_t mask_word_last;
  //  8: uint32_t source_c;
  // 12: uint32_t source_b;
  // 16: uint32_t source_a;
  // 20: uint32_t source_d;
  // 24: uint16_t size_v;
  // 26: uint16_t size_h;
  // 28: uint16_t modulo_c;
  // 30: uint16_t modulo_b;
  // 32: uint16_t modulo_a;
  // 34: uint16_t modulo_d;
  // 36: uint16_t source_data_c;
  // 38: uint16_t source_data_b;
  // 40: uint16_t source_data_a;

  // Allocate memory from the heap.
  fprintf(out,
    "  move.l #0, (a5)\n"
    "  add.l #44, a5\n");

  reg -= 1;

  return 0;
}

int Amiga::blitter_setSourceA_aB()
{
  fprintf(out,
    "  ;; blitter_setSourceA_aB()\n"
    "  movea.l d%d, a2\n"
    "  move.l d%d, (16, a2)\n",
    reg - 2,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setSourceB_aB()
{
  fprintf(out,
    "  ;; blitter_setSourceB_aB()\n"
    "  movea.l d%d, a2\n"
    "  move.l d%d, (12, a2)\n",
    reg - 2,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setSourceC_aB()
{
  fprintf(out,
    "  ;; blitter_setSourceC_aB()\n"
    "  movea.l d%d, a2\n"
    "  move.l d%d, (8, a2)\n",
    reg - 2,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setDestination_aB()
{
  fprintf(out,
    "  ;; blitter_setDestination_aB()\n"
    "  movea.l d%d, a2\n"
    "  move.l d%d, (20, a2)\n",
    reg - 2,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setModuloA_I()
{
  fprintf(out,
    "  ;; blitter_setModuloA_I()\n"
    "  movea.l d%d, a2\n"
    "  move.w d%d, (32, a2)\n",
    reg - 2,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setModuloB_I()
{
  fprintf(out,
    "  ;; blitter_setModuloB_I()\n"
    "  movea.l d%d, a2\n"
    "  move.w d%d, (30, a2)\n",
    reg - 2,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setModuloC_I()
{
  fprintf(out,
    "  ;; blitter_setModuloC_I()\n"
    "  movea.l d%d, a2\n"
    "  move.w d%d, (28, a2)\n",
    reg - 2,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setModuloDestination_I()
{
  fprintf(out,
    "  ;; blitter_setModuloDestination_I()\n"
    "  movea.l d%d, a2\n"
    "  move.w d%d, (34, a2)\n",
    reg - 2,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setShiftA_I()
{
  fprintf(out,
    "  ;; blitter_setShiftA_I()\n"
    "  movea.l d%d, a2\n"
    "  lsl.w #8, d%d\n"
    "  lsl.w #4, d%d\n"
    "  and.w 0x0fff, (a2)\n"
    "  or.w d%d, (a2)\n",
    reg - 2,
    reg - 1,
    reg - 1,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setShiftB_I()
{
  fprintf(out,
    "  ;; blitter_setShiftB_I()\n"
    "  movea.l d%d, a2\n"
    "  lsl.w #8, d%d\n"
    "  lsl.w #4, d%d\n"
    "  and.w 0x0fff, (2,a2)\n"
    "  or.w d%d, (2,a2)\n",
    reg - 2,
    reg - 1,
    reg - 1,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setChannelAMasks_II()
{
  fprintf(out,
    "  ;; blitter_setChannelAMasks_II()\n"
    "  movea.l d%d, a2\n"
    "  move.w d%d, (4, a2)\n"
    "  move.w d%d, (6, a2)\n",
    reg - 3,
    reg - 2,
    reg - 1);

  reg -= 3;

  return 0;
}

int Amiga::blitter_enableChannels_I()
{
  fprintf(out,
    "  ;; blitter_enableChannels_I()\n"
    "  movea.l d%d, a2\n"
    "  lsl.w #8, d%d\n"
    "  and.w 0xf0ff, (2,a2)\n"
    "  or.w d%d, (2,a2)\n",
    reg - 2,
    reg - 1,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setAsFillMode_I()
{
  fprintf(out,
    "  ;; blitter_setAsFillMode_I()\n"
    "  movea.l d%d, a2\n"
    "  and.w 0xf000, (2,a2)\n"
    "  or.w d%d, (2,a2)\n",
    reg - 2,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setAsLineMode_I()
{
  fprintf(out,
    "  ;; blitter_setAsLineMode_I()\n"
    "  movea.l d%d, a2\n"
    "  and.w 0xf0ff, (a2)\n"
    "  or.w 0x0b00, (a2)\n"
    "  and.w 0xf000, (2,a2)\n"
    "  or.w d%d, (2,a2)\n",
    reg - 2,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setLogicalFunction_I()
{
  fprintf(out,
    "  ;; blitter_setLogicalFunction_I()\n"
    "  movea.l d%d, a2\n"
    "  and.w 0xff00, (a2)\n"
    "  or.w d%d, (a2)\n",
    reg - 2,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setSize_II()
{
  fprintf(out,
    "  ;; blitter_setSize_II()\n"
    "  movea.l d%d, a2\n"
    "  move.w d%d, (26, a2)\n"
    "  move.w d%d, (24, a2)\n",
    reg - 3,
    reg - 2,
    reg - 1);

  reg -= 3;

  return 0;
}

int Amiga::blitter_setLineTypeA_I()
{
  fprintf(out,
    "  ;; blitter_setLineTypeA_I()\n"
    "  movea.l d%d, a2\n"
    "  move.w d%d, (40, a2)\n",
    reg - 2,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setLineTypeB_I()
{
  fprintf(out,
    "  ;; blitter_setLineTypeA_I()\n"
    "  movea.l d%d, a2\n"
    "  move.w d%d, (38, a2)\n",
    reg - 2,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setLineTypeC_I()
{
  fprintf(out,
    "  ;; blitter_setLineTypeA_I()\n"
    "  movea.l d%d, a2\n"
    "  move.w d%d, (36, a2)\n",
    reg - 2,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setLineTexture_I()
{
  fprintf(out,
    "  ;; blitter_setLineTexture_I()\n"
    "  movea.l d%d, a2\n"
    "  and.w 0x0fff, (2,a2)\n"
    "  or.w d%d, (2,a2)\n",
    reg - 2,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setLineStart_I()
{
  fprintf(out,
    "  ;; blitter_setLineTexture_I()\n"
    "  movea.l d%d, a2\n"
    "  and.w 0x0fff, (a2)\n"
    "  or.w d%d, (a2)\n",
    reg - 2,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::blitter_runFill_II()
{
  fprintf(out,
    "  ;; blitter_runFill_II()\n"
    "  movea.l d%d, a2\n"
    "  move.l (a2), (BLTCON0,a3)\n"
    "  move.l (4,a2), (BLTAFWM,a3)\n"
    "  move.l (8,a2), (BLTCPTH,a3)\n"
    "  move.l (12,a2), (BLTBPTH,a3)\n"
    "  move.l (16,a2), (BLTAPTH,a3)\n"
    "  move.l (20,a2), (BLTDPTH,a3)\n"
    "  move.l (24,a2), (BLTDPTH,a3)\n"
    "  move.l (28,a2), (BLTCMOD,a3)\n"
    "  move.l (32,a2), (BLTAMOD,a3)\n"
    "  move.l (36,a2), (BLTCDAT,a3)\n"
    "  move.w (38,a2), (BLTADAT,a3)\n",
    reg - 2);

  fprintf(out,
    "  lsl.w #6, d%d\n"
    "  or.w d%d, d%d\n"
    "  move.w d%d, (BLTSIZE,a3)\n",
    reg - 1,
    reg - 1,
    reg - 2,
    reg - 2);

  reg -= 3;

  return 0;
}

int Amiga::blitter_drawLine_I()
{
  fprintf(out,
    "  ;; blitter_drawLine_I()\n"
    "  movea.l d%d, a2\n"
    "  move.l (a2), (BLTCON0,a3)\n"
    "  move.l (4,a2), (BLTAFWM,a3)\n"
    "  move.l (8,a2), (BLTCPTH,a3)\n"
    "  move.l (12,a2), (BLTBPTH,a3)\n"
    "  move.l (16,a2), (BLTAPTH,a3)\n"
    "  move.l (20,a2), (BLTDPTH,a3)\n"
    "  move.l (24,a2), (BLTDPTH,a3)\n"
    "  move.l (28,a2), (BLTCMOD,a3)\n"
    "  move.l (32,a2), (BLTAMOD,a3)\n"
    "  move.l (36,a2), (BLTCDAT,a3)\n"
    "  move.w (38,a2), (BLTADAT,a3)\n",
    reg - 2);

  fprintf(out,
    "  lsl.w #6, d%d\n"
    "  or.w #2, d%d\n"
    "  move.w d%d, (BLTSIZE,a3)\n",
    reg - 1,
    reg - 1,
    reg - 1);

  reg -= 2;

  return 0;
}

int Amiga::memory_clearArray_aB()
{
  fprintf(out,
    "  ;; memory_clearArray_aB()\n"
    "  movea.l d%d, a2\n"
    "  move.l d5, (-4,a2)\n"
    "_clear_array_loop_%d:\n"
    "  move.b #0, (a2)+\n"
    "  subq.l #1, d5\n"
    "  bne.s _clear_array_loop_%d\n",
    reg - 1,
    label_count,
    label_count);

  label_count++;
  reg -= 1;

  return 0;
}

int Amiga::memory_clearArray_aS()
{
  fprintf(out,
    "  ;; memory_clearArray_aB()\n"
    "  movea.l d%d, a2\n"
    "  move.l d5, (-4,a2)\n"
    "_clear_array_loop_%d:\n"
    "  move.w #0, (a2)+\n"
    "  subq.l #1, d5\n"
    "  bne.s _clear_array_loop_%d\n",
    reg - 1,
    label_count,
    label_count);

  label_count++;
  reg -= 1;

  return 0;
}

int Amiga::memory_clearArray_aI()
{
  fprintf(out,
    "  ;; memory_clearArray_aB()\n"
    "  movea.l d%d, a2\n"
    "  move.l d5, (-4,a2)\n"
    "_clear_array_loop_%d:\n"
    "  move.l #0, (a2)+\n"
    "  subq.l #1, d5\n"
    "  bne.s _clear_array_loop_%d\n",
    reg - 1,
    label_count,
    label_count);

  label_count++;
  reg -= 1;

  return 0;
}

int Amiga::copper_getNextIndexAndIncrement(int reg)
{
  fprintf(out,
    "  movea.l d%d, a2\n"
    "  move.l (-4,a2), d5\n"
    "  add.l #4, (-4,a2)\n",
    reg);

  return 0;
}
