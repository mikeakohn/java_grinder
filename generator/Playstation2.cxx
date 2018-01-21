/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
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
  virtual_address = 0x0;
  physical_address = 0x0;
}

Playstation2::~Playstation2()
{
  add_dma_reset();
  add_dma_wait();
  add_copy_vu1_code();
  add_screen_init_clear();
  add_primitive_gif_tag();
  add_vu1_code();
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
  fprintf(out, "  ;; new_object(%s, field_count=%d)\n", object_name, field_count);

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
  fprintf(out, "  ;; draw3d_Constructor_X(type=%d)\n", type);

  return -1;
}

int Playstation2::draw3d_Constructor_I(int type)
{
  // Need to allocate enough space for:
  //  0: rx, ry, rz, 0
  // 16: x, y, z, 0
  // 32: count, 0, 0, 0
  // 48: 16 byte GIF tag
  // 64: 16 byte primitive info
  //   : 16 bytes per point (for x, y, z)
  //   : 16 bytes per point (for color)
  fprintf(out, "  ;; draw3d_Constructor_I(type=%d)\n", type);
  fprintf(out, "  ;; Align stack pointer, alloca() some memory\n");
  fprintf(out, "  ;; Clear rotation and offset and set point count\n");
  fprintf(out, "  addi $at, $0, -16\n");
  fprintf(out, "  and $sp, $sp, $at\n");
  fprintf(out, "  sll $at, $t%d, 4\n", reg - 1);
  fprintf(out, "  addi $at, $at, 64\n");
  fprintf(out, "  sub $sp, $sp, $at\n");
  fprintf(out, "  sq $0, 0($sp)\n");
  fprintf(out, "  sq $0, 16($sp)\n");
  fprintf(out, "  li $v0, _primitive_gif_tag\n");
  fprintf(out, "  lq $v1, 0($v0)\n");
  fprintf(out, "  sll $at, $t%d, 1\n", reg - 1);
  fprintf(out, "  addi $at, $at, 1\n");
  fprintf(out, "  or $v1, $v1, $at\n");
  fprintf(out, "  sq $v1, 48($sp)\n");
  fprintf(out, "  lq $v1, 16($v0)\n");
  fprintf(out, "  ori $v1, $v1, %d\n", type);
  fprintf(out, "  sq $v1, 64($sp)\n");
  fprintf(out, "  sw $t%d, 32($sp)\n", reg - 1);
  fprintf(out, "  move $a0, $t%d\n", reg - 1);
  //fprintf(out, "  move $t%d, $sp\n", reg - 1);
  fprintf(out, "  move $t%d, $sp\n", reg - 2);
  fprintf(out, "  move $t%d, $sp\n", reg - 3);

  // Set points and colors
  fprintf(out,
    "  move $v0, $sp\n"
    "  addi $v0, $sp, 80\n"
    "  li $a3, 0x3f80_0000\n"
    "  dsll32 $a3, $a3, 0\n"
    "  li $a2, REG_RGBAQ\n"
    "  li $a1, REG_XYZ2\n"
    "_const_object_%d:\n"
    "  ld $a3, 0($v0)\n"
    "  ld $a2, 8($v0)\n"
    "  ld $0, 16($v0)\n"
    "  ld $a1, 24($v0)\n"
    "  addi $v0, $v0, 32\n"
    "  addi $a0, $a0, -1\n"
    "  bne $a0, $0, _const_object_%d\n"
    "  nop\n", label_count, label_count);

  label_count++;
  reg -= 2;

  return 0;
}

int Playstation2::draw3d_rotate_III()
{
  int object = reg - 4;
  int x = reg - 3;
  int y = reg - 2;
  int z = reg - 1;

  fprintf(out,
    "  ;; draw3d_rotate_III()\n"
    "  lw $t%d, 16($t%d)\n"
    "  lw $t%d, 20($t%d)\n"
    "  lw $t%d, 24($t%d)\n",
    x, object,
    y, object,
    z, object);

  reg -= 4;

  return 0;
}

int Playstation2::draw3d_setPosition_III()
{
  int object = reg - 4;
  int x = reg - 3;
  int y = reg - 2;
  int z = reg - 1;

  fprintf(out,
    "  ;; draw3d_setPosition_III()\n"
    "  lw $t%d, 16($t%d)\n"
    "  lw $t%d, 20($t%d)\n"
    "  lw $t%d, 24($t%d)\n",
    x, object,
    y, object,
    z, object);

  reg -= 4;

  return 0;
}

int Playstation2::draw3d_setPointPosition_IIII()
{
  int object = reg - 5;
  int index = reg - 4;
  int x = reg - 3;
  int y = reg - 2;
  int z = reg - 1;

  fprintf(out,
    "  ;; draw3d_setPointPosition_IIII()\n"
    "  sll $t%d, $t%d, 5\n"
    "  add $t%d, $t%d, $t%d\n"
    "  lw $t%d, 96($t%d)\n"
    "  lw $t%d, 128($t%d)\n"
    "  lw $t%d, 104($t%d)\n",
    index, index,
    object, object, index,
    x, object,
    y, object,
    z, object);

  reg -= 5;

  return 0;
}

int Playstation2::draw3d_setPointColor_II()
{
  int object = reg - 3;
  int index = reg - 2;
  int color = reg - 1;

  fprintf(out,
    "  ;; draw3d_setPointColor_II()\n"
    "  sll $t%d, $t%d, 5\n"
    "  add $t%d, $t%d, $t%d\n"
    "  lw $t%d, 80($t%d)\n",
    index, index,
    object, object, index,
    color, object);

  reg -= 3;

  return 0;
}

int Playstation2::draw3d_draw()
{
  int object = reg - 1;

  fprintf(out,
    "  ;; draw3d_draw()\n"
    "  ;; This an be done with DMA, but trying it with the main CPU\n"
    "  ;; for now.  Copy GIF packet to VU1's data memory segment.\n"
    "  li $v0, VU1_VU_MEM\n"
    "  move $a1, $t%d\n"
    "  sll $a1, $a1, 5\n"
    "  addi $a1, $a1, 80\n"
    "_repeat_vu1_data_copy_%d:\n"
    "  lq $a0, ($t%d)\n"
    "  sq $a0, ($v0)\n"
    "  addi $t%d, $t%d, 16\n"
    "  addi $v0, $v0, 16\n"
    "  addi $a1, $a1, -1\n"
    "  bnez $a1, _repeat_vu1_data_copy_%d\n"
    "  nop\n",
    object,
    label_count,
    object, object, object,
    label_count);

  label_count++;

  reg -= 1;

  return 0;
}

int Playstation2::playstation2_waitVsync()
{
  // It seems like this shouldn't be called very often
  // so might as well inline it?  Saving $ra, restoring,
  // calling, returning is almost the size of this function.
  fprintf(out,
    "  ;; playstation2_waitVsync()\n"
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
    "  li $v1, D2_CHCR\n"
    "  sw $zero, 0x00($v1)    ; D2_CHCR\n"
    "  sw $zero, 0x30($v1)    ; D2_TADR\n"
    "  sw $zero, 0x10($v1)    ; D2_MADR\n"
    "  sw $zero, 0x50($v1)    ; D2_ASR1\n"
    "  sw $zero, 0x40($v1)    ; D2_ASR0\n\n"

    "  li $v1, D_CTRL\n"
    "  li $v0, 0xff1f\n"
    "  sw $v0, 0x10($v1)      ; DMA_STAT\n\n"

    "  sw $zero, 0x00($v1)    ; DMA_CTRL\n"
    "  sw $zero, 0x20($v1)    ; DMA_PCR\n"
    "  sw $zero, 0x30($v1)    ; DMA_SQWC\n"
    "  sw $zero, 0x50($v1)    ; DMA_RBOR\n"
    "  sw $zero, 0x40($v1)    ; DMA_RBSR\n\n"

    "  lw $v0, 0x00($v1)      ; DMA_CTRL\n"
    "  ori $v0, $v0, 1\n"
    "  nop\n"
    "  sw $v0, 0x00($v1)      ; DMA_CTRL\n"
    "  nop\n\n"

    "  jr $ra\n"
    "  nop\n\n");
}

void Playstation2::add_dma_wait()
{
  fprintf(out,
    "_dma02_wait:\n"
    "  li $v1, D2_CHCR\n"
    "  lw $v0, ($v1)\n"
    "  andi $v0, $v0, 0x100\n"
    "  bnez $v0, _dma02_wait\n"
    "  nop\n"
    "  jr $ra\n"
    "  nop\n\n");
}

void Playstation2::add_copy_vu1_code()
{
  fprintf(out,
    "  ;; add_copy_vu1_code()\n"
    "  li $v0, VU1_MICRO_MEM\n"
    "  li $a1, (_rotation_vu1_end - _rotation_vu1) / 16\n"
    "  li $v1, _rotation_vu1\n"
    "_repeat_vu1_prog_copy_%d:\n"
    "  lq $a0, ($v1)\n"
    "  sq $a0, ($v0)\n"
    "  addi $v1, $v1, 16\n"
    "  addi $v0, $v0, 16\n"
    "  addi $a1, $a1, -1\n"
    "  bnez $a1, _repeat_vu1_prog_copy_%d\n"
    "  nop\n",
    label_count, label_count
  );

  label_count++;
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

void Playstation2::add_primitive_gif_tag()
{
  fprintf(out,
    "_primitive_gif_tag:\n"
    "  dc64 GIF_TAG(0, 1, 0, 0, FLG_PACKED, 1, 0x0), REG_A_D\n"
    "  dc64 SETREG_PRIM(0, 1, 0, 0, 0, 0, 0, 0, 1), REG_PRIM\n\n");
}

void Playstation2::add_vu1_code()
{
  fprintf(out,
  ".ps2_ee_vu1\n"
  ".org 0\n"
  "_rotation_vu1:\n"
  "  nop isub vi0, vi0, vi0\n"
  "  nop iaddiu vi0, vi0, 48\n"
  "  nop nop\n"
  "  nop xgkick vi0\n"
  "  nop nop\n"
  "  nop[E] nop\n"
  "_rotation_vu1_end:\n\n");
}

