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

