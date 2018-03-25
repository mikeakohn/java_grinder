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

#include "Math.h"
#include "Playstation2.h"

#define DRAW3D "net/mikekohn/java_grinder/Draw3D/"
#define DRAW3D_LEN (sizeof(DRAW3D) - 1)

Playstation2::Playstation2()
{
  org = 0x100000;
  ram_start = 0x00000000;
  ram_end = 32 * 1024 * 1024;
  physical_address = 0x0;
}

Playstation2::~Playstation2()
{
  add_dma_reset();
  add_dma_wait();
  add_screen_init_clear();
  add_draw3d_object_constructor();
  add_primitive_gif_tag();
  add_vu1_code();
  Math::add_sin_table(out);
  Math::add_cos_table(out);
}

int Playstation2::open(const char *filename)
{
  if (R5900::open(filename) != 0) { return -1; }

  fprintf(out,
    ".include \"macros.inc\"\n"
    ".include \"registers_gs_gp.inc\"\n"
    ".include \"system_calls.inc\"\n"
    ".include \"registers_ee.inc\"\n"
    ".include \"registers_gs_priv.inc\"\n"
    ".include \"vif.inc\"\n\n"
    ".entry_point start\n"
    ".export start\n\n");

  return 0;
}

int Playstation2::start_init()
{
  R5900::start_init();

  fprintf(out,
    "  ;; Set stack pointer and reset DMA\n"
    "  li $sp, 0x%x\n",
    ram_end);

  //fprintf("  li $sp, _constant_pool + 256\n");

  fprintf(out,
    "  ;; Reset DMA\n"
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
    "  li $a2, 0\n"
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
    "  li $at, 0x1bf_9ff\n"
    "  dsll32 $at, $at, 0\n"
    "  li $v0, 0x0182_4290\n"
    "  or $at, $at, $v0\n"
    "  sd $at, ($v1)\n\n");

  add_copy_vu1_code();

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

int Playstation2::draw3d_object_Constructor_X(int type)
{
  fprintf(out, "  ;; draw3d_object_Constructor_X(type=%d)\n", type);

  return -1;
}

int Playstation2::draw3d_object_Constructor_I(int type)
{
  const int reg_point_count = reg - 1;
  //const int reg_object_dup = reg - 2;
  const int reg_object_ref = reg - 3;

  fprintf(out, "  ;; draw3d_object_Constructor_I(type=%d)\n", type);
  fprintf(out, "  li $a0, %d\n", type);
  fprintf(out, "  move $a1, $t%d\n", reg_point_count);
  fprintf(out, "  move $s0, $ra\n");
  fprintf(out, "  jal _draw3d_object_constructor\n");
  fprintf(out, "  nop\n");
  fprintf(out, "  move $ra, $s0\n");
  fprintf(out, "  move $t%d, $v0\n", reg_object_ref);

  reg -= 2;

  return 0;
}

int Playstation2::draw3d_object_rotateX512_I()
{
  int object = reg - 2;
  int x = reg - 1;

  fprintf(out,
    "  ;; draw3d_object_rotate512_III()\n"
    "  andi $t%d, $t%d, 511\n"
    "  sll $t%d, $t%d, 2\n"
    "  li $v1, _sin_table_512\n"
    "  addu $v1, $v1, $t%d\n"
    "  lw $v0, ($v1)\n"
    "  sw $v0, 0($t%d)\n"
    "  li $v1, _cos_table_512\n"
    "  addu $v1, $v1, $t%d\n"
    "  lw $v0, ($v1)\n"
    "  sw $v0, 4($t%d)\n"
    "  ori $v1, $0, 1\n"
    "  sw $v1, 52($t%d)\n",
    x, x,
    x, x,
    x,
    object,
    x,
    object,
    object);

  reg -= 2;

  return 0;
}

int Playstation2::draw3d_object_rotateY512_I()
{
  int object = reg - 2;
  int y = reg - 1;

  fprintf(out,
    "  ;; draw3d_object_rotate512_III()\n"
    "  andi $t%d, $t%d, 511\n"
    "  sll $t%d, $t%d, 2\n"
    "  li $v1, _sin_table_512\n"
    "  addu $v1, $v1, $t%d\n"
    "  lw $v0, ($v1)\n"
    "  sw $v0, 8($t%d)\n"
    "  li $v1, _cos_table_512\n"
    "  addu $v1, $v1, $t%d\n"
    "  lw $v0, ($v1)\n"
    "  sw $v0, 12($t%d)\n"
    "  ori $v1, $0, 1\n"
    "  sw $v1, 56($t%d)\n",
    y, y,
    y, y,
    y,
    object,
    y,
    object,
    object);

  reg -= 2;

  return 0;
}

int Playstation2::draw3d_object_rotateZ512_I()
{
  int object = reg - 2;
  int z = reg - 1;

  fprintf(out,
    "  ;; draw3d_object_rotate512_III()\n"
    "  andi $t%d, $t%d, 511\n"
    "  sll $t%d, $t%d, 2\n"
    "  li $v1, _sin_table_512\n"
    "  addu $v1, $v1, $t%d\n"
    "  lw $v0, ($v1)\n"
    "  sw $v0, 16($t%d)\n"
    "  li $v1, _cos_table_512\n"
    "  addu $v1, $v1, $t%d\n"
    "  lw $v0, ($v1)\n"
    "  sw $v0, 20($t%d)\n"
    "  ori $v1, $0, 1\n"
    "  sw $v1, 60($t%d)\n",
    z, z,
    z, z,
    z,
    object,
    z,
    object,
    object);

  reg -= 2;

  return 0;
}

int Playstation2::draw3d_object_setPosition_FFF()
{
  int object = reg - 4;
  int x = reg - 3;
  int y = reg - 2;
  int z = reg - 1;

  fprintf(out,
    "  ;; draw3d_object_setPosition_FFF()\n"
    "  sw $t%d, 32($t%d)\n"
    "  sw $t%d, 36($t%d)\n"
    "  sw $t%d, 40($t%d)\n",
    x, object,
    y, object,
    z, object);

  reg -= 4;

  return 0;
}

int Playstation2::draw3d_object_setPoint_IFFF()
{
  int object = reg - 5;
  int index = reg - 4;
  int x = reg - 3;
  int y = reg - 2;
  int z = reg - 1;

  fprintf(out,
    "  ;; draw3d_object_setPointPosition_IFFF()\n"
    "  sll $t%d, $t%d, 5\n"
    "  addiu $t%d, $t%d, 128\n"
    "  addu $t%d, $t%d, $t%d\n"
    "  sw $t%d, 0($t%d)\n"
    "  sw $t%d, 4($t%d)\n"
    "  sw $t%d, 8($t%d)\n",
    index, index,
    object, object,
    object, object, index,
    x, object,
    y, object,
    z, object);

  reg -= 5;

  return 0;
}

int Playstation2::draw3d_object_setPointColor_II()
{
  int object = reg - 3;
  int index = reg - 2;
  int color = reg - 1;

  fprintf(out,
    "  ;; draw3d_object_setPointColor_II()\n"
    "  sll $t%d, $t%d, 5\n"
    "  addiu $t%d, $t%d, 112\n"
    "  addu $t%d, $t%d, $t%d\n"
    "  sw $t%d, 0($t%d)\n",
    index, index,
    object, object,
    object, object, index,
    color, object);

  reg -= 3;

  return 0;
}

int Playstation2::draw3d_object_setPoints_aF()
{
  int array = reg - 1;
  int object = reg - 2;

  fprintf(out,
    "  ;; draw3d_object_setPoints_aF()\n"
    "  lw $t8, -4($t%d)\n"
    //"  sll $t8, $t8, 5\n"
    "  addiu $t%d, $t%d, 128\n"
    "_set_points_%d:\n"
    "  lw $at, 0($t%d)\n"
    "  sw $at, 0($t%d)\n"
    "  lw $at, 4($t%d)\n"
    "  sw $at, 4($t%d)\n"
    "  lw $at, 8($t%d)\n"
    "  sw $at, 8($t%d)\n"
    "  addiu $t%d, $t%d, 12\n"
    "  addiu $t%d, $t%d, 32\n"
    "  addiu $t8, $t8, -3\n"
    "  bne $t8, $0, _set_points_%d\n"
    "  nop\n",
    array,
    object, object,
    label_count,
    array,
    object,
    array,
    object,
    array,
    object,
    array, array,
    object, object,
    label_count);

  label_count++;
  reg -= 2;

  return 0;
}

int Playstation2::draw3d_object_setPointColors_aI()
{
  int array = reg - 1;
  int object = reg - 2;

  fprintf(out,
    "  ;; draw3d_object_setPointColors_aI()\n"
    "  lw $t8, -4($t%d)\n"
    //"  sll $t8, $t8, 5\n"
    "  addiu $t%d, $t%d, 112\n"
    "_set_point_colors_%d:\n"
    "  lw $at, 0($t%d)\n"
    "  sw $at, 0($t%d)\n"
    "  addiu $t%d, $t%d, 4\n"
    "  addiu $t%d, $t%d, 32\n"
    "  addiu $t8, $t8, -1\n"
    "  bne $t8, $0, _set_point_colors_%d\n"
    "  nop\n",
    array,
    object, object,
    label_count,
    array,
    object,
    array, array,
    object, object,
    label_count);

  label_count++;
  reg -= 2;

  return 0;
}

int Playstation2::draw3d_object_disableGouraudShading()
{
  int object = reg - 1;

  fprintf(out,
    "  ;; disableGouraudShading()\n"
    "  lw $t8, 80($t%d)\n"
    "  andi $t8, $t8, 0xfff7\n"
    "  sw $t8, 80($t%d)\n",
    object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_object_enableGouraudShading()
{
  int object = reg - 1;

  fprintf(out,
    "  ;; enableGouraudShading()\n"
    "  lw $t8, 80($t%d)\n"
    "  ori $t8, $t8, 0x8\n"
    "  sw $t8, 80($t%d)\n",
    object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_object_disableFogging()
{
  int object = reg - 1;

  fprintf(out,
    "  ;; disableFogging()\n"
    "  lw $t8, 80($t%d)\n"
    "  andi $t8, $t8, 0xffdf\n"
    "  sw $t8, 80($t%d)\n",
    object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_object_enableFogging()
{
  int object = reg - 1;

  fprintf(out,
    "  ;; enableFogging()\n"
    "  lw $t8, 80($t%d)\n"
    "  ori $t8, $t8, 0x20\n"
    "  sw $t8, 80($t%d)\n",
    object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_object_disableTexture()
{
  int object = reg - 1;

  fprintf(out,
    "  ;; disableTexture()\n"
    "  lw $t8, 80($t%d)\n"
    "  andi $t8, $t8, 0xffef\n"
    "  sw $t8, 80($t%d)\n",
    object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_object_enableTexture()
{
  int object = reg - 1;

  fprintf(out,
    "  ;; enableTexture()\n"
    "  lw $t8, 80($t%d)\n"
    "  ori $t8, $t8, 0x10\n"
    "  sw $t8, 80($t%d)\n",
    object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_object_disableAlphaBlending()
{
  int object = reg - 1;

  fprintf(out,
    "  ;; disableAlphaBlending()\n"
    "  lw $t8, 80($t%d)\n"
    "  andi $t8, $t8, 0xffbf\n"
    "  sw $t8, 80($t%d)\n",
    object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_object_enableAlphaBlending()
{
  int object = reg - 1;

  fprintf(out,
    "  ;; enableAlphaBlending()\n"
    "  lw $t8, 80($t%d)\n"
    "  ori $t8, $t8, 0x40\n"
    "  sw $t8, 80($t%d)\n",
    object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_object_disableAntialiasing()
{
  int object = reg - 1;

  fprintf(out,
    "  ;; disableAntialiasing()\n"
    "  lw $t8, 80($t%d)\n"
    "  andi $t8, $t8, 0xff7f\n"
    "  sw $t8, 80($t%d)\n",
    object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_object_enableAntialiasing()
{
  int object = reg - 1;

  fprintf(out,
    "  ;; enableAntialiasing()\n"
    "  lw $t8, 80($t%d)\n"
    "  ori $t8, $t8, 0x80\n"
    "  sw $t8, 80($t%d)\n",
    object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_object_draw()
{
  int object = reg - 1;

  // This could be done with DMA.  Not sure what's better.
  fprintf(out,
    "  ;; draw3d_object_draw()\n"
    "  ;; Copy GIF packet to VU1's data memory segment.\n"
    "  li $v0, VU1_VU_MEM\n"
    "  move $v1, $t%d\n"
    "  lw $a1, 48($v1)\n"
    "  sll $a1, $a1, 1\n"
    "  addiu $a1, $a1, 7\n"
    "_repeat_vu1_data_copy_%d:\n"
    "  lq $a0, ($v1)\n"
    "  sq $a0, ($v0)\n"
    "  addiu $v1, $v1, 16\n"
    "  addiu $v0, $v0, 16\n"
    "  addiu $a1, $a1, -1\n"
    "  bnez $a1, _repeat_vu1_data_copy_%d\n"
    "  nop\n\n",
    object,
    label_count,
    label_count);

  fprintf(out,
    "  ;; Start the VU1 with a VIF packet\n"
    "  li $v0, D1_CHCR\n"
    "  li $v1, vu1_start\n"
    "  sw $v1, 0x10($v0)         ; DMA01 ADDRESS\n"
    "  li $v1, 1                 ; Length is only 1 qword\n"
    "  sw $v1, 0x20($v0)         ; DMA01 SIZE\n"
    "  li $v1, 0x101\n"
    "  sw $v1, ($v0)             ; start\n\n");

  label_count++;

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_texture_Constructor_II()
{
  return -1;
}

int Playstation2::draw3d_texture_setPixel_II()
{
  return -1;
}

int Playstation2::draw3d_texture_setPixels_aI()
{
  return -1;
}

int Playstation2::draw3d_texture_upload()
{
  return -1;
}

int Playstation2::playstation2_clearScreen()
{
  // Not sure if this needs to be called on every frame draw.  I believe
  // at least part of it should be since it clears the display.
  fprintf(out,
    "  ;; Draw a black square over the entire screen\n"
    "  move $at, $ra\n"
    "  jal _dma02_wait\n"
    "  nop\n"
    "  move $ra, $at\n"
    "  li $v0, D2_CHCR\n"
    "  li $v1, _screen_init_clear\n"
    "  sw $v1, 0x10($v0)         ; DMA02 ADDRESS\n"
    "  li $v1, (_screen_init_clear_end - _screen_init_clear) / 16\n"
    "  sw $v1, 0x20($v0)         ; DMA02 SIZE\n"
    "  li $v1, 0x101\n"
    "  sw $v1, ($v0)             ; start\n\n");

  return 0;
}

int Playstation2::playstation2_waitVsync()
{
  // It seems like this shouldn't be called very often
  // so might as well inline it?  Saving $ra, restoring,
  // calling, returning is almost the size of this function.
  fprintf(out,
    "  ;; playstation2_waitVsync()\n"
    "  li $v0, GS_CSR\n"
    "  li $v1, 8\n"
    "  sw $v1, ($v0)\n"
    "_wait_vsync_%d:\n"
    "  lw $v1, ($v0)\n"
    "  andi $v1, $v1, 8\n"
    "  beqz $v1, _wait_vsync_%d\n"
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
    "  addiu $v1, $v1, 16\n"
    "  addiu $v0, $v0, 16\n"
    "  addiu $a1, $a1, -1\n"
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
    "  dc64 GIF_TAG(14, 0, 0, 0, FLG_PACKED, 1), REG_A_D\n"
    "  dc64 0x00a0000, REG_FRAME_1            ; framebuffer width = 640/64\n"
    "  dc64 0x8c, REG_ZBUF_1              ; 0-8 Zbuffer base, 24-27 Z format (32bit)\n"
    "  dc64 SETREG_XYOFFSET(1728 << 4, 1936 << 4), REG_XYOFFSET_1\n"
    "  dc64 SETREG_SCISSOR(0,639,0,447), REG_SCISSOR_1\n"
    "  dc64 1, REG_PRMODECONT                 ; refer to prim attributes\n"
    "  dc64 1, REG_COLCLAMP\n"
    "  dc64 0, REG_DTHE                       ; Dither off\n"
    "  dc64 0x70000, REG_TEST_1\n"
    "  dc64 0x30000, REG_TEST_1\n"
    "  dc64 6, REG_PRIM\n"
    "  dc64 0x3f80_0000_0000_0000, REG_RGBAQ  ; Background RGBA\n"
    "  dc64 SETREG_XYZ2(1728 << 4, 1936 << 4, 0), REG_XYZ2\n"
    "  dc64 SETREG_XYZ2(2368 << 4, 2384 << 4, 0), REG_XYZ2\n"
    "  dc64 0x70000, REG_TEST_1\n"
    "_screen_init_clear_end:\n\n");
}

void Playstation2::add_primitive_gif_tag()
{
  fprintf(out,
    ".align 128\n"
    "_primitive_gif_tag:\n"
    "  dc64 GIF_TAG(1, 0, 0, 0, FLG_PACKED, 1), REG_A_D\n"
    "  dc64 SETREG_PRIM(0, 1, 0, 0, 0, 0, 0, 0, 0), REG_PRIM\n"
    "  dc64 GIF_TAG(0, 1, 0, 0, FLG_PACKED, 2), (REG_A_D|(REG_XYZ2<<4))\n\n");
    //"  dc64 GIF_TAG(0, 1, 0, 0, FLG_PACKED, 1), REG_A_D\n"
    //"  dc64 SETREG_PRIM(0, 1, 0, 0, 0, 0, 0, 0, 1), REG_PRIM\n\n");
}

void Playstation2::add_draw3d_object_constructor()
{
  // Need to allocate enough space for:
  //  0: sin512(rx), cos512(rx), sin512(ry), sin512(ry)
  // 16: sin512(rz), cos(rz), 0, 0
  // 32: x, y, z, 0
  // 48: count, do_rot_x, do_rot_y, do_rot_z
  // 64: 16 byte GIF tag
  // 80: 16 byte primitive info
  // 96: 16 byte GIF tag
  //112: 16 bytes per point (for color)   REG_A_D
  //   : 16 bytes per point (for x, y, z) REG_XYZ2
  fprintf(out,
    "_draw3d_object_constructor:\n"
    "  ;; _draw3d_object_constructor(type, point_count)\n"
    "  ;; Align stack pointer, alloca() some memory\n"
    "  ;; Clear rotation and offset and set point count\n"
    "  addiu $at, $0, -16\n"
    "  and $sp, $sp, $at\n");

  // Allocated memory is number of points * 32 + size of header.
  fprintf(out,
    "  sll $at, $a1, 5\n"
    "  addiu $at, $at, 112\n"
    "  subu $sp, $sp, $at\n");

  // Set top of reg stack (return value) to allocated memory.
  fprintf(out, "  move $v0, $sp\n");

  // Clear out members of the structure (rotation, xyz position, etc).
  fprintf(out,
    "  sq $0, 0($sp)\n"
    "  sq $0, 16($sp)\n"
    "  sq $0, 32($sp)\n"
    "  sq $0, 48($sp)\n");

  // Save point count in the struct
  fprintf(out, "  sw $a1, 48($sp)\n");

  // Copy the default primitive tag to the new structure.
  fprintf(out,
    "  li $t8, _primitive_gif_tag\n"
    "  lq $t9, 0($t8)\n"
    "  sq $t9, 64($sp)\n"
    "  lq $t9, 16($t8)\n"
    "  or $t9, $t9, $a0\n"
    "  sq $t9, 80($sp)\n"
    "  lq $t9, 32($t8)\n"
    "  or $t9, $t9, $a1\n"
    "  sq $t9, 96($sp)\n");

  // a0 = count
  //fprintf(out, "  move $a0, $t%d\n", reg_point_count);

  // Set points and colors
  fprintf(out,
    "  addiu $t8, $sp, 112\n"
    "  lui $t9, 0x3f80\n"
    "  dsll32 $t9, $t9, 0\n"
    "  li $a2, REG_RGBAQ\n"
    "_draw3d_object_constructor_l0:\n"
    "  sd $t9, 0($t8)\n"
    "  sd $a2, 8($t8)\n"
    "  sq $0, 16($t8)\n"
    "  addiu $t8, $t8, 32\n"
    "  addiu $a0, $a0, -1\n"
    "  bne $a0, $0, _draw3d_object_constructor_l0\n"
    "  nop\n");

  fprintf(out,
    "  jr $ra\n"
    "  nop\n\n");

  label_count++;
}

