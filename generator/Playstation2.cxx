/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "Playstation2.h"

#define DRAW3D "net/mikekohn/java_grinder/Draw3D/"
#define DRAW3D_LEN (sizeof(DRAW3D) - 1)

Playstation2::Playstation2()
{
  org = 0x100000;
  ram_start = 0x00000000;
  ram_end = 32 * 1024 * 1024;
  //virtual_address = 0x9d000000;
  //physical_address = 0x1d000000;
}

Playstation2::~Playstation2()
{
  // it's 6 instructions to inline and 5 instructions to call
  //if (need_spi_read) { add_spi_read(); }

  playstation2_addDMAReset();
}

int Playstation2::open(const char *filename)
{
  if (MIPS64::open(filename) != 0) { return -1; }

  fprintf(out,
    ".ps2_ee\n"
    ".include \"macros.inc\"\n"
    ".include \"registers_gs_gp.inc\"\n"
    ".include \"system_calls.inc\"\n"
    ".include \"registers_ee.inc\"\n"
    ".include \"registers_gs_priv.inc\"\n"
    ".include \"vif.inc\"\n\n"
    ".entry_point main\n"
    ".export start\n\n");

  return 0;
}

int Playstation2::start_init()
{
  MIPS64::start_init();

  fprintf(out,
    "  // Set stack pointer and reset DMA\n"
    "  li $sp, 0x02000000\n"
    "  jal _dma_reset\n"
    "  nop\n\n"

    "  ;; Reset GS\n"
    "  li $v1, GS_CSR\n"
    "  li $v0, 0x200\n"
    "  sd $v0, ($v1)\n\n"

    "  ;; Interrupt mask register\n"
    "  li $v1, GsPutIMR\n"
    "  li $a0, 0xff00\n"
    "  syscall\n"
    "  nop\n\n");

  fprintf(out,
    "  ;; interlace      { PS2_NONINTERLACED = 0, PS2_INTERLACED = 1 };\n"
    "  ;; videotype      { PS2_NTSC = 2, PS2_PAL = 3 };\n"
    "  ;; frame          { PS2_FRAME = 1, PS2_FIELD = 0 };\n"
    "  ;; SetGsCrt(s16 interlace, s16 pal_ntsc, s16 field);\n"
    "  li $v1, SetGsCrt\n"
    "  li $a0, 1\n"
    "  li $a1, 2\n"
    "  li $a2, 1\n"
    "  syscall\n"
    "  nop\n\n");

  // FIXME - GS_PMODE can be a settable function.
  fprintf(out,
    "  ;; Use framebuffer read circuit (1 or 2?)\n"
    "  li $v1, GS_PMODE\n"
    "  li $v0, 0xff62\n"
    "  sd $v0, ($v1)\n\n");

  // These were originally settable, but I think for now I'd rather
  // just get the drawing functions working.

  fprintf(out,
    "  ;; GS_DISPFB2 with 0x1400\n"
    "  ;;         base pointer (fbp): 0x0 (0x0)\n"
    "  ;;   frame buffer width (fbw): 10 (640)\n"
    "  ;; pixel storage format (psm): 0 (PSMCT32)\n"
    "  ;;           position x (dbx): 0 (0x0)\n"
    "  ;;           position y (dby): 0 (0x0)\n"
    "  li $v1, GS_DISPFB2\n"
    "  li $v0, 0x1400\n"
    "  sd $v0, ($v1)\n\n");

  fprintf(out,
    "  ;; GS_DISPLAY2 with 0x000d_f9ff_0182_4290\n"
    "  ;;         x position vck units (dx): 656\n"
    "  ;;      y position raster units (dy): 36\n"
    "  ;;        horiz magnification (magh): 3 (x8)\n"
    "  ;;         vert magnification (magv): 0 (x1)\n"
    "  ;;     display width - 1 in vck (dw): 2559\n"
    "  ;; display height - 1 in pixels (dh): 223\n"
    "  li $v1, GS_DISPLAY2\n"
    "  li $at, 0xdf9ff\n"
    "  dsll32 $at, $at, 0\n"
    "  li $v0, 0x0182_4290\n"
    "  or $at, $at, $v0\n"
    "  sd $at, ($v1)\n\n");

  return 0;
}

int Playstation2::new_object(const char *object_name, int field_count)
{
  //printf("Error: Here  %s field_count=%d.\n", object_name, field_count);

  if (strncmp(object_name, DRAW3D, DRAW3D_LEN) != 0)
  {
     printf("Error: Unsupported class %s\n", object_name);
     return -1;
  }

  const char *s = object_name;

  while(*s != 0) { s++; }
  while(s != object_name && *s != '/') { s--; }
  if (*s == '/') { s++; }

  if (strcmp(s, "Draw3DPoints") != 0)
  {
     printf("Error: Unknown class %s\n", object_name);
     return -1;
  }

  return 0;
}

int Playstation2::draw3d_Constructor_X(int type)
{
  fprintf(out, "  ; draw3d_Constructor_X()\n");

  return -1;
}

int Playstation2::draw3d_Constructor_I(int type)
{
  fprintf(out, "  ; draw3d_Constructor_I()\n");

  return 0;
}

int Playstation2::draw3d_rotate_III()
{
  return -1;
}

int Playstation2::draw3d_setPosition_III()
{
  return -1;
}

int Playstation2::draw3d_setPointPosition_IIII()
{
  return -1;
}

int Playstation2::draw3d_setPointColor_II()
{
  return -1;
}

int Playstation2::draw3d_draw()
{
  return -1;
}

#if 0
int Playstation2::playstation2_setVideoMode_III()
{
  fprintf(out,
    "  ;; SetGsCrt(s16 interlace, s16 pal_ntsc, s16 field);\n"
    "  li $v1, SetGsCrt\n"
    "  move $a0, $t%d\n"
    "  move $a1, $t%d\n"
    "  move $a2, $t%d\n"
    "  syscall\n"
    "  nop\n\n", reg - 3, reg - 2, reg - 1);

  reg -= 3;

  return 0;
}
#endif

#if 0
int Playstation2::playstation2_setFrameBuffer1_IIIII()
{
  return playstation2_setFrameBuffer(1);
}

int Playstation2::playstation2_setFrameBuffer2_IIIII()
{
  return playstation2_setFrameBuffer(2);
}

int Playstation2::playstation2_setDisplay1_IIIIII()
{
  return playstation2_setDisplay(1);
}

int Playstation2::playstation2_setDisplay2_IIIIII()
{
  return playstation2_setDisplay(2);
}
#endif

int Playstation2::playstation2_waitVsync()
{
  // It seems like this shouldn't be called very often
  // so might as well inline it?  Saving $ra, restoring,
  // calling, returning is almost the size of this function.
  fprintf(out,
    " li $v0, 8\n"
    " sw $v0, ($v1)\n"
    "_wait_vsync_%d:\n"
    " lw $v0, ($v1)\n"
    " andi $v0, $v0, 8\n"
    " beqz $v0, _wait_vsync_%d\n"
    " nop\n", label_count, label_count);

  label_count++;

  return 0;
}

void Playstation2::playstation2_addDMAReset()
{
  fprintf(out,
    "_dma_reset:\n"
    "  li $s0, D2_CHCR\n"
    "  sw $zero, 0x00($s0)    ; D2_CHCR\n"
    "  sw $zero, 0x30($s0)    ; D2_TADR\n"
    "  sw $zero, 0x10($s0)    ; D2_MADR\n"
    "  sw $zero, 0x50($s0)    ; D2_ASR1\n"
    "  sw $zero, 0x40($s0)    ; D2_ASR0\n\n"

    "  li $s0, D_CTRL\n"
    "  li $s1, 0xff1f\n"
    "  sw $s1, 0x10($s0)      ; DMA_STAT\n\n"

    "  sw $zero, 0x00($s0)    ; DMA_CTRL\n"
    "  sw $zero, 0x20($s0)    ; DMA_PCR\n"
    "  sw $zero, 0x30($s0)    ; DMA_SQWC\n"
    "  sw $zero, 0x50($s0)    ; DMA_RBOR\n"
    "  sw $zero, 0x40($s0)    ; DMA_RBSR\n\n"

    "  lw $s1, 0x00($s0)      ; DMA_CTRL\n"
    "  ori $s1, $s1, 1\n"
    "  nop\n"
    "  sw $s1, 0x00($s0)      ; DMA_CTRL\n"
    "  nop\n\n"

    "  jr $ra\n"
    "  nop\n\n");
}

#if 0
int Playstation2::playstation2_setFrameBuffer(int index)
{
  // FIXME - If parameters are stack based, this will fail.

  fprintf(out,
    "  ;; GS_DISPFB%d -- Will silently fail for out of range values\n"
    "  ;;         base pointer (fbp  8:0 ): 0x0 (0x0)\n"
    "  ;;   frame buffer width (fbw 14:9 ): 10 (640)\n"
    "  ;; pixel storage format (psm 19:15): 0 (PSMCT32)\n"
    "  ;;           position x (dbx 42:32): 0 (0x0)\n"
    "  ;;           position y (dby 53:43): 0 (0x0)\n"
    "  li $v1, GS_DISPFB%d\n"
    "  dsrl $t%d, $t%d, 11\n"
    "  move $v0, $t%d\n"
    "  dsrl $t%d, $t%d, 6\n"
    "  dsll $t%d, $t%d, 14\n"
    "  or $v0, $v0, $t%d\n"
    "  dsll $t%d, $t%d, 15\n"
    "  or $v0, $v0, $t%d\n"
    "  dsll32 $t%d, $t%d, 0\n"
    "  or $v0, $v0, $t%d\n"
    "  dsll32 $t%d, $t%d, 11\n"
    "  or $v0, $v0, $t%d\n"
    "  sd $v0, ($v1)\n\n",
    index, index,
    reg - 1, reg - 1,
    reg - 1,
    reg - 2, reg - 2,
    reg - 2, reg - 2,
    reg - 2,
    reg - 3, reg - 3,
    reg - 3,
    reg - 4, reg - 4,
    reg - 4,
    reg - 5, reg - 5,
    reg - 5);

  reg -= 5;

  return 0;
}

int Playstation2::playstation2_setDisplay(int index)
{
  // FIXME - If parameters are stack based, this will fail.

  fprintf(out,
    "  ;; GS_DISPLAY%d -- Bad values will silently fail\n"
    "  ;;         x position vck units (dx 11:0 ): 656\n"
    "  ;;      y position raster units (dx 22:12): 36\n"
    "  ;;        horiz magnification (magh 26:23): 3 (x8)\n"
    "  ;;         vert magnification (magv 28:27): 0 (x1)\n"
    "  ;;     display width - 1 in vck (dw 43:32): 2559\n"
    "  ;; display height - 1 in pixels (dh 54:44): 223\n"
    "  li $v1, GS_DISPLAY%d\n"
    "  move $v0, $t%d\n"
    "  dsll $t%d, $t%d, 12\n"
    "  or $v0, $v0, $t%d\n"
    "  dsll $t%d, $t%d, 23\n"
    "  or $v0, $v0, $t%d\n"
    "  dsll $t%d, $t%d, 27\n"
    "  or $v0, $v0, $t%d\n"
    "  addi $t%d, $t%d, -1\n"
    "  dsll32 $t%d, $t%d, 0\n"
    "  or $v0, $v0, $t%d\n"
    "  addi $t%d, $t%d, -1\n"
    "  dsll32 $t%d, $t%d, 11\n"
    "  or $v0, $v0, $t%d\n"
    "  sd $at, ($v1)\n\n",
    index, index,
    reg - 1,
    reg - 2, reg - 2,
    reg - 2,
    reg - 3, reg - 3,
    reg - 3,
    reg - 4, reg - 4,
    reg - 4,
    reg - 5, reg - 5,
    reg - 5, reg - 5,
    reg - 5,
    reg - 6, reg - 6,
    reg - 6, reg - 6,
    reg - 6);

  reg -= 6;

  return 0;
}
#endif

