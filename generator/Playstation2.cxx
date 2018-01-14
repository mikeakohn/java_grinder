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
  add_dma_reset();
  add_dma_wait();
  add_screen_init_clear();
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

  // Not sure if this needs to be called on every frame draw.  I believe
  // at least part of it should be since it clears the display.
  fprintf(out,
    "  ;; Setup draw environment\n"
    "  jal _dma02_wait\n"
    "  nop\n"
    "  li $v0, D2_CHCR\n"
    "  li $v1, _screen_init_clear\n"
    "  sw $v1, 0x10($v0)         ; DMA02 ADDRESS\n"
    "  li $v1, (_screen_init_clear_end - _screen_init_clear) / 16\n"
    "  sw $v1, 0x20($v0)         ; DMA02 SIZE\n"
    "  li $v1, 0x101\n"
    "  sw $v1, ($v0)             ; start\n\n");

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

  if (strcmp(s, "Draw3DPoints") != 0 &&
      strcmp(s, "Draw3DTriangle") != 0)
  {
     printf("Error: Unknown class %s\n", object_name);
     return -1;
  }

  return 0;
}

int Playstation2::draw3d_Constructor_X(int type)
{
  fprintf(out, "  ; draw3d_Constructor_X(type=%d)\n", type);

  return -1;
}

int Playstation2::draw3d_Constructor_I(int type)
{
  // Need to allocate enough space for:
  // rx, ry, rz, 0 / x, y, z, 0  (32 bytes)
  // count, 0, 0, 0,   0, 0, 0, 0 (16 bytes)
  // 16 bytes per point (for x, y, z)
  // 16 bytes per point (for color)
  fprintf(out, "  ; draw3d_Constructor_I(type=%d)\n", type);
  fprintf(out, "  ; Align stack pointer, alloca() some memory\n");
  fprintf(out, "  ; Clear rotation and offset and set point count\n");
  fprintf(out, "  addi $at, $0, -16\n");
  fprintf(out, "  and $sp, $fp, $at\n");
  fprintf(out, "  sll $at, $t%d, 4\n", reg - 1);
  fprintf(out, "  addi $at, $at, %d\n", 32);
  fprintf(out, "  sub $sp, $sp, $at\n");
  fprintf(out, "  sq $0, 0($sp)\n");
  fprintf(out, "  sq $0, 16($sp)\n");
  fprintf(out, "  sq $t%d, 24($sp)\n", reg - 1);
  fprintf(out, "  move $t%d, $sp\n", reg - 1);

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
    "  ; playstation2_waitVsync()\n"
    "  li $v0, 8\n"
    "  sw $v0, ($v1)\n"
    "_wait_vsync_%d:\n"
    "  lw $v0, ($v1)\n"
    "  andi $v0, $v0, 8\n"
    "  beqz $v0, _wait_vsync_%d\n"
    "  nop\n", label_count, label_count);

  label_count++;

  return 0;
}

void Playstation2::add_dma_reset()
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

void Playstation2::add_dma_wait()
{
  fprintf(out,
    "_dma02_wait:\n"
    "  li $s1, D2_CHCR\n"
    "  lw $s0, ($s1)\n"
    "  andi $s0, $s0, 0x100\n"
    "  bnez $s0, _dma02_wait\n"
    "  nop\n"
    "  jr $ra\n"
    "  nop\n\n");
}

void Playstation2::add_screen_init_clear()
{
  fprintf(out,
    ".align 128\n"
    "_screen_init_clear:\n"
    "  dc64 0x100000000000800e, REG_A_D\n"
    "  dc64 0x00a0000, REG_FRAME_1            ; framebuffer width = 640/64\n"
    "  dc64 0x8c, REG_ZBUF_1              ; 0-8 Zbuffer base, 24-27 Z format (32bit)\n"
    "  dc32 27648, 30976                      ; X,Y offset\n"
    "  dc64 REG_XYOFFSET_1\n"
    "  dc16 0,639, 0,223                      ; x1,y1,x2,y2 - scissor window\n"
    "  dc64 REG_SCISSOR_1\n"
    "  dc64 1, REG_PRMODECONT                 ; refer to prim attributes\n"
    "  dc64 1, REG_COLCLAMP\n"
    "  dc64 0, REG_DTHE                       ; Dither off\n"
    "  dc64 0x70000, REG_TEST_1\n"
    "  dc64 0x30000, REG_TEST_1\n"
    "  dc64 6, REG_PRIM\n"
    "  dc64 0x3f80_0000_0000_00ff, REG_RGBAQ  ; Background RGBA\n"
    "  dc64 0x79006c00, REG_XYZ2              ; (1728.0, 1936.0, 0)\n"
    "  dc64 0x87009400, REG_XYZ2              ; (2368.0, 2160.0, 0)\n"
    "  dc64 0x70000, REG_TEST_1\n"
    "_screen_init_clear_end:\n\n");
}

