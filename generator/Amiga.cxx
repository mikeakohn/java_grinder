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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "generator/Amiga.h"

#define HEAP_SIZE 250 * 1024

// NOTE: a3 points to Amiga hardware registers.

Amiga::Amiga() :
  need_set_sprite_position(false)
{
  //start_org = 0x8000;
}

Amiga::~Amiga()
{
  if (need_set_sprite_position) { add_set_sprite_position(); }

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

#if 0
  fprintf(out,
    ".align_bytes 4\n"
    "_amiga_grind_end:\n\n"
    "  ;; Hunk file end\n"
    "  dc32 0x0000_03f2\n\n"
  );
#endif

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

#if 0
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
    "  dc32 0x8000_03e9  ; hunk_code\n\n"
    "  dc32 (_amiga_grind_end - _amiga_grind_start) / 4\n"
    "_amiga_grind_start:\n\n"
  );
#endif

  fprintf(out, "  lea (6,pc), a4\n");
  fprintf(out, "  bra.w _start\n\n");

  fprintf(out, "_statics:\n");

  for (auto static_field : static_fields)
  {
    fprintf(out,
      "%s:\n"
      "  dc32 0\n",
      static_field.c_str());
  }

  if (static_fields.size() != 0) { fprintf(out, "\n"); }

  fprintf(out,
    "_start:\n"
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

int Amiga::insert_static_field_define(
  std::string &name,
  std::string &type,
  int index)
{
  static_fields.insert(name);

  return 0;
}

int Amiga::field_init_int(std::string &name, int index, int value)
{
  //fprintf(out, "  move.l #%d, (%s)\n", value, name.c_str());

  fprintf(out,
    "  lea (2,pc), a2\n"
    "  suba.l #%s-$, a2\n"
    "  move.l #0x%02x, (a2)\n",
    name.c_str(),
    value);

  return 0;
}

int Amiga::field_init_ref(std::string &name, int index)
{
  //fprintf(out, "  move.l #_%s, (%s)\n", name.c_str(), name.c_str());

  fprintf(out,
    "  ;; field_init_ref(%s, %d)\n"
    "  lea (2,pc), a2\n"
    "  adda.l #%s-$, a2\n"
    "  lea (2,pc), a1\n"
    "  adda.l #_%s-$, a1\n"
    "  move.l a1, (a2)\n",
    //"  move.l (_%s,pc), (a2)\n",
    name.c_str(), index,
    name.c_str(),
    name.c_str());

  return 0;
}

int Amiga::push_ref_static(std::string &name, int index)
{
  // REVIEW: Shouldn't this be (a2) with #%s?
  fprintf(out,
    "  ;; push_ref_static(%s, %d)\n"
    "  lea (2,pc), a2\n"
    "  adda.l #%s-$, a2\n"
    "  move.l (a2), d%s\n",
    name.c_str(), index,
    name.c_str(),
    push_reg());

  return 0;
}

int Amiga::put_static(std::string &name, int index)
{
#if 0
  if (index >= 0)
  {
    fprintf(out,
      "  ;; put_static(%s,%d)\n"
      "  lea (%d,a4), a2\n"
      "  move.l %s, (a2)\n",
      name.c_str(), index,
      index * 4,
      pop_reg());
  }
    else
#endif
  {
    fprintf(out,
      "  ;; put_static(%s,%d)\n"
      "  lea (2,pc), a2\n"
      "  adda.l #%s-$, a2\n"
      "  move.l %s, (a2)\n",
      name.c_str(), index,
      name.c_str(),
      pop_reg());
  }

  return 0;
}

int Amiga::get_static(std::string &name, int index)
{
#if 0
  if (index >= 0)
  {
    fprintf(out,
      "  ;; get_static(%s,%d)\n"
      "  lea (%d,a4), a2\n"
      "  move.l (a2), %s\n",
      name.c_str(), index,
      index * 4,
      push_reg());
  }
    else
#endif
  {
    fprintf(out,
      "  ;; get_static(%s,%d)\n"
      "  lea (2,pc), a2\n"
      "  adda.l #%s-$, a2\n"
      "  move.l (a2), %s\n",
      name.c_str(), index,
      name.c_str(),
      push_reg());
  }

  return 0;
}

int Amiga::push_ref(std::string &name)
{
  fprintf(out,
    "  ;; push_ref(%s)\n"
    "  lea (2,pc), a2\n"
    "  adda.l #%s-$, a2\n"
    "  move.l (a2), %s\n",
    name.c_str(),
    name.c_str(),
    push_reg());

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

int Amiga::amiga_setSpriteImage_IaS()
{
  fprintf(out,
    "  ;; amiga_setSpriteImage_IaS()\n"
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
  fprintf(out,
    "  ;; amiga_setSpritePosition_IIII()\n"
    "  lsl.w #8, d%d\n"
    "  lea (0,a3,d%d), a2\n"
    "  move.l d%d, d5\n"
    "  move.l d%d, d6\n"
    "  move.l d%d, d7\n"
    "  jsr (_set_sprite_position).l\n",
    reg - 4,
    reg - 4,
    reg - 3,
    reg - 2,
    reg - 1);

  need_set_sprite_position = true;

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
    "  andi.w #0xf, d%d\n"
    "  andi.w #0xf, d%d\n"
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
    "  andi.w #0x1, d%d\n"
    "  andi.w #0x7, d%d\n"
    "  andi.w #0x7, d%d\n"
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

int Amiga::amiga_setAudioData_IaB()
{
  const int channel = reg - 2;
  const int address = reg - 1;

  fprintf(out,
    "  ;; amiga_setAudioData_IaB()\n"
    "  lea (AUD0LCH,a3), a2\n"
    "  lsl.w #4, d%d\n"
    "  move.l d%d, (0,a2,d%d)\n",
    channel,
    address,
    channel);

  reg -= 2;

  return 0;
}

int Amiga::amiga_setAudioLength_II()
{
  const int channel = reg - 2;
  const int value = reg - 1;

  fprintf(out,
    "  ;; amiga_setAudioLength_II()\n"
    "  lea (AUD0LEN,a3), a2\n"
    "  lsl.w #4, d%d\n"
    "  move.w d%d, (0,a2,d%d)\n",
    channel,
    value,
    channel);

  reg -= 2;

  return 0;
}

int Amiga::amiga_setAudioPeriod_II()
{
  const int channel = reg - 2;
  const int value = reg - 1;

  fprintf(out,
    "  ;; amiga_setAudioPeriod_II()\n"
    "  lea (AUD0PER,a3), a2\n"
    "  lsl.w #4, d%d\n"
    "  move.w d%d, (0,a2,d%d)\n",
    channel,
    value,
    channel);

  reg -= 2;

  return 0;
}

int Amiga::amiga_setAudioVolume_II()
{
  const int channel = reg - 2;
  const int value = reg - 1;

  fprintf(out,
    "  ;; amiga_setAudioVolume_II()\n"
    "  lea (AUD0VOL,a3), a2\n"
    "  lsl.w #4, d%d\n"
    "  move.w d%d, (0,a2,d%d)\n",
    channel,
    value,
    channel);

  reg -= 2;

  return 0;
}

int Amiga::amiga_setAudioModulation_I()
{
  fprintf(out,
    "  ;; amiga_setAudioModulation_I()\n"
    "  or.w #0x8000, d%d\n"
    "  move.w d%d, (ADKCON,a3)\n",
    reg - 1,
    reg - 1);

  reg -= 1;

  return 0;
}

int Amiga::amiga_clearAudioModulation_I()
{
  fprintf(out,
    "  ;; amiga_clearAudioModulation_I()\n"
    "  move.w d%d, (ADKCON,a3)\n",
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

int Amiga::amiga_inVerticalBlank()
{
#if 0
  fprintf(out,
    "  ;; amiga_inVerticalBlank()\n"
    "  eor.l d%d, d%d\n"
    "  move.w (INTREQR,a3), d%d\n"
    "  lsr.w #5, d%d\n"
    "  and.w #1, d%d\n",
    reg, reg,
    reg, reg, reg);
#endif

  fprintf(out,
    "  ;; amiga_inVerticalBlank()\n"
    "  move.l (VPOSR,a3), d5\n"
    "  and.l #0x1ff00, d5\n"
    "  cmp.l #0x12f00, d5\n"
    "  move sr, d%d\n"
    "  lsr.w #2, d%d\n"
    "  and.w #1, d%d\n",
    reg, reg, reg);

  reg += 1;

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
    "  move.w d%d, (4,a2,d5)\n"
    "  move.w d%d, (6,a2,d5)\n",
    index,
    index,
    index,
    value,
    index,
    index,
    value);

  reg -= 3;

  return 0;
}

int Amiga::copper_appendSetBitplane_II()
{
  return copper_appendSetBitplane_IaB();
}

int Amiga::copper_appendSetSprite_IaC()
{
  const int object = reg - 3;
  const int index = reg - 2;
  const int value = reg - 1;

  fprintf(out, "  ;; copper_appendSetSprite_IaC()\n");

  fprintf(out,
    "  movea.l d%d, a2\n"
    "  move.l (-4,a2), d5\n"
    "  add.l #8, (-4,a2)\n",
    object);

  fprintf(out,
    "  lsl.w #2, d%d\n"
    "  add.w #SPR0PTH, d%d\n"
    "  move.w d%d, (0,a2,d5)\n"
    "  addq.w #2, d%d\n"
    "  move.w d%d, (4,a2,d5)\n"
    "  move.w d%d, (6,a2,d5)\n"
    "  swap d%d\n"
    "  move.w d%d, (2,a2,d5)\n",
    index,
    index,
    index,
    index,
    index,
    value,
    value,
    value);

  reg -= 3;

  return 0;
}

int Amiga::copper_appendEnd()
{
  const int object = reg - 1;

  fprintf(out, "  ;; copper_appendEnd()\n");

  copper_getNextIndexAndIncrement(object);

  fprintf(out, "  move.l #0xfffffffe, (0,a2,d5)\n");

  reg -= 1;

  return 0;
}

int Amiga::copper_resetIndex()
{
  const int object = reg - 1;

  fprintf(out,
    "  ;; copper_resetIndex()\n"
    "  movea.l d%d, a2\n"
    "  move.l #0, (-4,a2)\n",
    object);

  reg -= 1;

  return 0;
}

int Amiga::copper_setIndex_I()
{
  const int object = reg - 2;
  const int index = reg - 1;

  fprintf(out,
    "  ;; copper_resetIndex()\n"
    "  movea.l d%d, a2\n"
    "  move.l d%d, (-4,a2)\n",
    object,
    index);

  reg -= 2;

  return 0;
}

int Amiga::copper_run()
{
  const int object = reg - 1;

  fprintf(out,
    "  ;; copper_run()\n"
    "  move.l d%d, (COP1LCH,a3)\n"
    "  move.w #0x8080, (DMACON,a3)\n"
    "  move.w d5, (COPJMP1,a3)\n",
    object);

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

int Amiga::blitter_waitBusy()
{
  fprintf(out,
    "  ;; blitter_waitBusy()\n"
    "  tst.w (DMACONR,a3)\n"
    "label_%d:\n"
    "  btst #6, (DMACONR,a3)\n"
    "  bne.s label_%d\n",
    label_count,
    label_count);

  label_count++;

  return 0;
}

int Amiga::blitter_setSourceA_aB()
{
  const int object = reg - 2;
  const int data = reg - 1;

  fprintf(out,
    "  ;; blitter_setSourceA_aB()\n"
    "  movea.l d%d, a2\n"
    "  move.l d%d, (16, a2)\n",
    object,
    data);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setSourceB_aB()
{
  const int object = reg - 2;
  const int data = reg - 1;

  fprintf(out,
    "  ;; blitter_setSourceB_aB()\n"
    "  movea.l d%d, a2\n"
    "  move.l d%d, (12, a2)\n",
    object,
    data);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setSourceC_aB()
{
  const int object = reg - 2;
  const int data = reg - 1;

  fprintf(out,
    "  ;; blitter_setSourceC_aB()\n"
    "  movea.l d%d, a2\n"
    "  move.l d%d, (8, a2)\n",
    object,
    data);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setDestination_aB()
{
  const int object = reg - 2;
  const int data = reg - 1;

  fprintf(out,
    "  ;; blitter_setDestination_aB()\n"
    "  movea.l d%d, a2\n"
    "  move.l d%d, (20, a2)\n",
    object,
    data);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setModuloA_I()
{
  const int object = reg - 2;
  const int value = reg - 1;

  fprintf(out,
    "  ;; blitter_setModuloA_I()\n"
    "  movea.l d%d, a2\n"
    "  move.w d%d, (32, a2)\n",
    object,
    value);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setModuloB_I()
{
  const int object = reg - 2;
  const int value = reg - 1;

  fprintf(out,
    "  ;; blitter_setModuloB_I()\n"
    "  movea.l d%d, a2\n"
    "  move.w d%d, (30, a2)\n",
    object,
    value);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setModuloC_I()
{
  const int object = reg - 2;
  const int value = reg - 1;

  fprintf(out,
    "  ;; blitter_setModuloC_I()\n"
    "  movea.l d%d, a2\n"
    "  move.w d%d, (28, a2)\n",
    object,
    value);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setModuloDestination_I()
{
  const int object = reg - 2;
  const int value = reg - 1;

  fprintf(out,
    "  ;; blitter_setModuloDestination_I()\n"
    "  movea.l d%d, a2\n"
    "  move.w d%d, (34, a2)\n",
    object,
    value);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setShiftA_I()
{
  const int object = reg - 2;
  const int value = reg - 1;

  fprintf(out,
    "  ;; blitter_setShiftA_I()\n"
    "  movea.l d%d, a2\n"
    "  lsl.w #8, d%d\n"
    "  lsl.w #4, d%d\n"
    "  andi.w #0x0fff, (a2)\n"
    "  or.w d%d, (a2)\n",
    object,
    value,
    value,
    value);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setShiftB_I()
{
  const int object = reg - 2;
  const int value = reg - 1;

  fprintf(out,
    "  ;; blitter_setShiftB_I()\n"
    "  movea.l d%d, a2\n"
    "  lsl.w #8, d%d\n"
    "  lsl.w #4, d%d\n"
    "  andi.w #0x0fff, (2,a2)\n"
    "  or.w d%d, (2,a2)\n",
    object,
    value,
    value,
    value);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setChannelAMasks_II()
{
  const int object = reg - 3;
  const int first_word = reg - 2;
  const int last_word = reg - 1;

  fprintf(out,
    "  ;; blitter_setChannelAMasks_II()\n"
    "  movea.l d%d, a2\n"
    "  move.w d%d, (4, a2)\n"
    "  move.w d%d, (6, a2)\n",
    object,
    first_word,
    last_word);

  reg -= 3;

  return 0;
}

int Amiga::blitter_enableChannels_I()
{
  const int object = reg - 2;
  const int mask = reg - 1;

  fprintf(out,
    "  ;; blitter_enableChannels_I()\n"
    "  movea.l d%d, a2\n"
    "  lsl.w #8, d%d\n"
    "  andi.w #0xf0ff, (0,a2)\n"
    "  or.w d%d, (0,a2)\n",
    object,
    mask,
    mask);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setAsFillMode_I()
{
  const int object = reg - 2;
  const int options = reg - 1;

  fprintf(out,
    "  ;; blitter_setAsFillMode_I()\n"
    "  movea.l d%d, a2\n"
    "  andi.w #0xf000, (2,a2)\n"
    "  or.w d%d, (2,a2)\n",
    object,
    options);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setAsLineMode_I()
{
  const int object = reg - 2;
  const int options = reg - 1;

  fprintf(out,
    "  ;; blitter_setAsLineMode_I()\n"
    "  movea.l d%d, a2\n"
    "  andi.w #0xf0ff, (a2)\n"
    "  ori.w #0x0b00, (a2)\n"
    "  andi.w #0xf000, (2,a2)\n"
    "  or.w d%d, (2,a2)\n",
    object,
    options);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setLogicalFunction_I()
{
  const int object = reg - 2;
  const int mask = reg - 1;

  fprintf(out,
    "  ;; blitter_setLogicalFunction_I()\n"
    "  movea.l d%d, a2\n"
    "  andi.w #0xff00, (a2)\n"
    "  or.w d%d, (a2)\n",
    object,
    mask);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setSize_II()
{
  const int object = reg - 3;
  const int horizontal = reg - 2;
  const int vertical = reg - 1;

  fprintf(out,
    "  ;; blitter_setSize_II()\n"
    "  movea.l d%d, a2\n"
    "  move.w d%d, (26, a2)\n"
    "  move.w d%d, (24, a2)\n",
    object,
    horizontal,
    vertical);

  reg -= 3;

  return 0;
}

int Amiga::blitter_setLineTypeA_I()
{
  const int object = reg - 2;
  const int line_type = reg - 1;

  fprintf(out,
    "  ;; blitter_setLineTypeA_I()\n"
    "  movea.l d%d, a2\n"
    "  move.w d%d, (40, a2)\n",
    object,
    line_type);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setLineTypeB_I()
{
  const int object = reg - 2;
  const int line_type = reg - 1;

  fprintf(out,
    "  ;; blitter_setLineTypeA_I()\n"
    "  movea.l d%d, a2\n"
    "  move.w d%d, (38, a2)\n",
    object,
    line_type);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setLineTypeC_I()
{
  const int object = reg - 2;
  const int line_type = reg - 1;

  fprintf(out,
    "  ;; blitter_setLineTypeA_I()\n"
    "  movea.l d%d, a2\n"
    "  move.w d%d, (36, a2)\n",
    object,
    line_type);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setLineTexture_I()
{
  const int object = reg - 2;
  const int value = reg - 1;

  fprintf(out,
    "  ;; blitter_setLineTexture_I()\n"
    "  movea.l d%d, a2\n"
    "  andi.w #0x0fff, (2,a2)\n"
    "  or.w d%d, (2,a2)\n",
    object,
    value);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setLineOctant_I()
{
  const int object = reg - 2;
  const int value = reg - 1;

  fprintf(out,
    "  ;; blitter_setLineOctant_I()\n"
    "  movea.l d%d, a2\n"
    "  andi.w #0xffe3, (2,a2)\n"
    "  or.w d%d, (2,a2)\n",
    object,
    value);

  reg -= 2;

  return 0;
}

int Amiga::blitter_setLineStart_I()
{
  const int object = reg - 2;
  const int value = reg - 1;

  fprintf(out,
    "  ;; blitter_setLineStart_I(int value)\n"
    "  movea.l d%d, a2\n"
    "  andi.w #0x0fff, (a2)\n"
    "  or.w d%d, (a2)\n",
    object,
    value);

  reg -= 2;

  return 0;
}

int Amiga::blitter_runFill_II()
{
  const int object = reg - 3;
  const int width = reg - 2;
  const int height = reg - 1;

  fprintf(out,
    "  ;; blitter_runFill_II(int width, int height)\n"
    "  movea.l d%d, a2\n"
    "  move.l (a2), (BLTCON0,a3)\n"
    "  move.l (4,a2), (BLTAFWM,a3)\n"
    "  move.l (8,a2), (BLTCPTH,a3)\n"
    "  move.l (12,a2), (BLTBPTH,a3)\n"
    "  move.l (16,a2), (BLTAPTH,a3)\n"
    "  move.l (20,a2), (BLTDPTH,a3)\n"
    "  move.l (24,a2), (BLTSIZV,a3)\n"
    "  move.l (28,a2), (BLTCMOD,a3)\n"
    "  move.l (32,a2), (BLTAMOD,a3)\n",
    //"  move.l (36,a2), (BLTCDAT,a3)\n"
    //"  move.w (38,a2), (BLTADAT,a3)\n",
    object);

  fprintf(out,
    "  lsl.w #6, d%d\n"
    "  or.w d%d, d%d\n"
    "  move.w d%d, (BLTSIZE,a3)\n",
    height,
    height, width,
    width);

  reg -= 3;

  return 0;
}

int Amiga::blitter_drawLine_I()
{
  const int object = reg - 2;
  const int length = reg - 1;

  fprintf(out,
    "  ;; blitter_drawLine_I(int length)\n"
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
    object);

  fprintf(out,
    "  lsl.w #6, d%d\n"
    "  or.w #2, d%d\n"
    "  move.w d%d, (BLTSIZE,a3)\n",
    length,
    length,
    length);

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

int Amiga::memory_addressOf_aB()
{
  fprintf(out, "  ;; memory_addressOf_aB()\n");

  return 0;
}

int Amiga::memory_addressOf_aS()
{
  fprintf(out, "  ;; memory_addressOf_aS()\n");

  return 0;
}

int Amiga::memory_addressOf_aC()
{
  fprintf(out, "  ;; memory_addressOf_aC()\n");

  return 0;
}

int Amiga::memory_addressOf_aI()
{
  fprintf(out, "  ;; memory_addressOf_aI()\n");

  return 0;
}

int Amiga::add_array_files()
{
  struct stat statbuf;
  std::map<std::string, ArrayFiles>::iterator iter;

  for (iter = preload_arrays.begin(); iter != preload_arrays.end(); iter++)
  {
    if (stat(iter->first.c_str(), &statbuf) != 0)
    {
      printf("Error opening %s\n", iter->first.c_str());
      return -1;
    }

    fprintf(out, ".align_bytes 40\n");
    fprintf(out, "  resb 36\n");
    fprintf(out, "  dc32 %d\n",
      (int)(iter->second.type == TYPE_BYTE ?
            statbuf.st_size : statbuf.st_size / get_int_size()));

    fprintf(out, "_%s:\n", iter->second.name.c_str());
    fprintf(out, ".binfile \"%s\"\n\n", iter->first.c_str());
  }

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

int Amiga::add_set_sprite_position()
{
  fprintf(out,
    "  ;; amiga_setSpritePosition()\n"
    "_set_sprite_position:\n");

  // Clean up input first:
  // index = (index & 7) * 4  <- make sure index is 0 to 7 and multiply by 8
  fprintf(out,
    "  andi.w #0x1ff, d5\n"
    "  andi.w #0x1ff, d6\n"
    "  andi.w #0x1ff, d7\n");

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

  // Start by rolling vend (9 bit number) 8 bits left.  The upper bit
  // ends up in position 0, so rotate just the lower byte left by 1 bit.
  fprintf(out,
    "  rol.w #8, d7\n"
    "  rol.b #1, d7\n");

  // Roll vertical the same way, but the upper bit goes into bit position
  // 2 of d7.
  fprintf(out,
    "  rol.w #8, d6\n"
    "  rol.b #2, d6\n"
    "  or.b d6, d7\n"
    "  and.b #0, d6\n");

  // Copy bottom 8 bits of start horizontal to d6 and upper bit 8 to
  // d7 position 1.
  fprintf(out,
    "  or.b d5, d6\n"
    "  lsr.w #8, d5\n"
    "  or.b d5, d7\n");

  // Put d6 and d7 into SPRxPOS and SPRxCTL.
  fprintf(out,
    "  move.w d6, (a2)\n"
    "  move.w d7, (2,a2)\n"
    "  rts\n");

  return 0;
}

