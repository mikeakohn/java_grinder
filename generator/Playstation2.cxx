/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2017 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "Playstation2.h"

Playstation2::Playstation2()
{
  org = 0x100000;
  ram_start = 0x80000000;
  ram_end = 0x80003fff;
  virtual_address = 0x9d000000;
  physical_address = 0x1d000000;
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

  fprintf(out, ".ps2_ee\n");
  fprintf(out, ".include \"playstation2.inc\"\n");

  return 0;
}

int Playstation2::start_init()
{
  MIPS64::start_init();

  fprintf(out,
    "  // Set stack pointer and reset DMA\n"
    "  li $sp, 0x02000000\n"
    "  jal dma_reset\n"
    "  nop\n\n"

    "  ;; Reset GS\n"
    "  li $v1, GS_CSR\n"
    "  li $v0, 0x200\n"
    "  sd $v0, ($v1)\n\n"

    "  ;; Interrupt mask register\n"
    "  li $v1, GsPutIMR\n"
    "  li $a0, 0xff00\n"
    "  syscall\n"
    "  nop\n\n"

    "  ;; Use framebuffer read circuit (1 or 2?)\n"
    "  li $v1, GS_PMODE\n"
    "  li $v0, 0xff62\n"
    "  sd $v0, ($v1)\n\n");

  // FIXME - GS_PMODE can be a settable function.

  return 0;
}

int Playstation2::playstation2_setVideoMode_III()
{
  return -1;
}

int Playstation2::playstation2_setVideoMode_III(int interlaced, int video_type, int frame)
{
  fprintf(out,
    "  ;; SetGsCrt(s16 interlace, s16 pal_ntsc, s16 field);\n"
    "  li $v1, SetGsCrt\n"
    "  li $a0, %d\n"
    "  li $a1, %d\n"
    "  li $a2, %d\n"
    "  syscall\n"
    "  nop\n\n", interlaced, video_type, frame);

  return 0;
}

int Playstation2::playstation2_setFrameBuffer1_IIIII()
{
  return -1;
}

int Playstation2::playstation2_setFrameBuffer1_IIIII(int base, int width, int pixel_format, int position_x, int position_y)
{
  fprintf(out,
    ";; GS_DISPFB2 with 0x1400\n"
    ";;         base pointer (fbp): 0x0 (0x0)\n"
    ";;   frame buffer width (fbw): 10 (640)\n"
    ";; pixel storage format (psm): 0 (PSMCT32)\n"
    ";;           position x (dbx): 0 (0x0)\n"
    ";;           position y (dby): 0 (0x0)\n"
    "li $v1, GS_DISPFB2\n"
    "li $v0, 0x1400\n"
    "sd $v0, ($v1)\n\n");

  return 0;
}

int Playstation2::playstation2_setDisplay1_IIIIII()
{
  return -1;
}

int Playstation2::playstation2_setDisplay1_IIIIII(int dx, int dy, int magh, int magv, int dw, int dh)
{
  fprintf(out,
    ";; GS_DISPLAY2 with 0x000d_f9ff_0182_4290\n"
    ";;         x position vck units (dx): 656\n"
    ";;      y position raster units (dx): 36\n"
    ";;        horiz magnification (magh): 3 (x8)\n"
    ";;         vert magnification (magv): 0 (x1)\n"
    ";;     display width - 1 in vck (dw): 2559\n"
    ";; display height - 1 in pixels (dh): 223\n"
    "li $v1, GS_DISPLAY2\n"
    "li $at, 0xdf9ff\n"
    "dsll32 $at, $at, 0\n"
    "li $v0, 0x0182_4290\n"
    "or $at, $at, $v0\n"
    "sd $at, ($v1)\n\n");

  return 0;
}

int Playstation2::playstation2_waitVsync()
{
  // It seems like this shouldn't be called very often
  // so might as well inline it?  Saving $ra, restoring,
  // calling, returning is almost the size of this function.
  fprintf(out,
    "_wait_vsync_%d:\n"
    "li $v0, 8\n"
    "sw $v0, ($v1)\n"
  "_wait_vsync_%d:\n"
    "lw $v0, ($v1)\n"
    "andi $v0, $v0, 8\n"
    "beqz $v0, _wait_vsync_%d\n"
    "nop\n", label_count, label_count, label_count);

  label_count++;

  return 0;
}

void Playstation2::playstation2_addDMAReset()
{
  fprintf(out,
    "_dma_reset:\n"
    "li $s0, D2_CHCR\n"
    "sw $zero, 0x00($s0)    ; D2_CHCR\n"
    "sw $zero, 0x30($s0)    ; D2_TADR\n"
    "sw $zero, 0x10($s0)    ; D2_MADR\n"
    "sw $zero, 0x50($s0)    ; D2_ASR1\n"
    "sw $zero, 0x40($s0)    ; D2_ASR0\n\n"

    "li $s0, D_CTRL\n"
    "li $s1, 0xff1f\n"
    "sw $s1, 0x10($s0)      ; DMA_STAT\n\n"

    "sw $zero, 0x00($s0)    ; DMA_CTRL\n"
    "sw $zero, 0x20($s0)    ; DMA_PCR\n"
    "sw $zero, 0x30($s0)    ; DMA_SQWC\n"
    "sw $zero, 0x50($s0)    ; DMA_RBOR\n"
    "sw $zero, 0x40($s0)    ; DMA_RBSR\n\n"

    "lw $s1, 0x00($s0)      ; DMA_CTRL\n"
    "ori $s1, $s1, 1\n"
    "nop\n"
    "sw $s1, 0x00($s0)      ; DMA_CTRL\n"
    "nop\n\n"

    "jr $ra\n"
    "nop\n\n");
}

