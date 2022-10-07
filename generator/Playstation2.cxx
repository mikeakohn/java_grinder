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
#include <string.h>
#include <stdint.h>

#include "generator/Math.h"
#include "generator/Playstation2.h"

//#define USE_SPU2_DMA

#define DRAW3D "net/mikekohn/java_grinder/draw3d/"
#define DRAW3D_LEN (sizeof(DRAW3D) - 1)

#define KERNEL_ENTER \
    "  ; Enter kernel mode\n" \
    "  mfc0 $s1, $12\n" \
    "  li $at, 0xffffffe7\n" \
    "  and $at, $s1, $at\n" \
    "  mtc0 $at, $12\n" \
    "  sync.p\n"

#define KERNEL_EXIT \
    "  ; Exit kernel mode\n" \
    "  mtc0 $s1, $12\n" \
    "  sync.p\n"

#define DMA02_WAIT \
    ".scope\n" \
    "  li $v0, D2_CHCR\n" \
    "_dma02_wait:\n" \
    "  lw $v1, ($v0)\n" \
    "  andi $v1, $v1, 0x100\n" \
    "  bnez $v1, _dma02_wait\n" \
    "  nop\n" \
    ".ends\n"

#if 0
#define FLUSH_CACHE \
    "  ;; Flush cache\n" \
    "  lui $a0, 0\n" \
    "  li $v1, FlushCache\n" \
    "  syscall\n"
#endif
//#define FLUSH_CACHE

Playstation2::Playstation2() :
  need_draw3d_texture16(false),
  need_draw3d_texture24(false),
  need_draw3d_texture32(false),
  need_spu_functions(false)
{
  org = 0x100000;
  ram_start = 0x00000000;
  ram_end = 32 * 1024 * 1024;
  physical_address = 0x0;
  preload_array_align = 128;
}

Playstation2::~Playstation2()
{
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

int Playstation2::finish()
{
  add_dma_functions();
  add_misc_functions();
  add_screen_init_clear();
  add_draw3d_object_constructor();
  add_draw3d_object_with_texture_constructor();
  if (need_draw3d_texture16) { add_draw3d_texture_constructor(16); }
  if (need_draw3d_texture24) { add_draw3d_texture_constructor(24); }
  if (need_draw3d_texture32) { add_draw3d_texture_constructor(32); }
  if (need_spu_functions) { add_spu_functions(); }
  add_draw3d_texture16_setPixelsRLE16_IaB();
  add_draw3d_object_draw();
  add_draw3d_texture_upload();
  add_primitive_gif_tag();
  add_texture_gif_tag();
  add_vu0_code();
  add_vu1_code();
  add_strings();
  Math::add_sin_table(out);
  Math::add_cos_table(out);

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

    "  ;; Reset VIF0\n"
    "  li $v0, VIF0_STAT\n"
    "  li $v1, 0xf\n"
    "  sw $v1, 0x10($v0)     ; FBRST\n\n"

    "  ;; Reset VIF1\n"
    "  li $v0, VIF1_STAT\n"
    "  li $v1, 0xf\n"
    "  sw $v1, 0x10($v0)     ; FBRST\n\n"

    "  ;; Interrupt mask register\n"
    "  li $v1, _GsPutIMR\n"
    "  li $a0, 0xff00\n"
    "  syscall\n"
    "  nop\n\n");

  fprintf(out,
    "  ;; interlace      { PS2_NONINTERLACED = 0, PS2_INTERLACED = 1 };\n"
    "  ;; videotype      { PS2_NTSC = 2, PS2_PAL = 3 };\n"
    "  ;; frame          { PS2_FRAME = 1, PS2_FIELD = 0 };\n"
    "  ;; _SetGsCrt(s16 interlace, s16 pal_ntsc, s16 field);\n"
    "  li $v1, _SetGsCrt\n"
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
    "  ;; pixel storage format (psm): 1 (PSMCT24)\n"
    "  ;;           position x (dbx): 0 (0x0)\n"
    "  ;;           position y (dby): 0 (0x0)\n"
    "  li $v1, GS_DISPFB2\n"
    "  li $v0, SETREG_DISPFB(0, 10, FMT_PSMCT24, 0, 0)\n"
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

  fprintf(out,
    "  ;; Set context 1 and 2 buffer pointers and such\n"
    "  jal _dma02_wait\n"
    "  nop\n"
    "  li $v0, D2_CHCR\n"
    "  li $v1, _screen_init\n"
    "  sw $v1, 0x10($v0)         ; DMA02 ADDRESS\n"
    "  li $v1, (_screen_init_end - _screen_init) / 16\n"
    "  sw $v1, 0x20($v0)         ; DMA02 SIZE\n"
    "  li $v1, 0x101\n"
    "  sw $v1, ($v0)             ; start\n\n");

  add_copy_vu1_code();

  return 0;
}

int Playstation2::new_object(std::string &object_name, int field_count)
{
  fprintf(out, "  ;; new_object(%s, field_count=%d)\n",
    object_name.c_str(), field_count);

  if (strncmp(object_name.c_str(), DRAW3D, DRAW3D_LEN) != 0)
  {
     printf("Error: Unsupported class %s\n", object_name.c_str());
     return -1;
  }

  const char *s = object_name.c_str();

  while (*s != 0) { s++; }
  while (s != object_name.c_str() && *s != '/') { s--; }
  if (*s == '/') { s++; }

  if (strcmp(s, "Points") != 0 &&
      strcmp(s, "Line") != 0 &&
      strcmp(s, "LineStrip") != 0 &&
      strcmp(s, "Triangle") != 0 &&
      strcmp(s, "TriangleStrip") != 0 &&
      strcmp(s, "TriangleFan") != 0 &&
      strcmp(s, "Sprite") != 0 &&
      strcmp(s, "TriangleStripWithTexture") != 0 &&
      strcmp(s, "TriangleFanWithTexture") != 0 &&
      strcmp(s, "SpriteWithTexture") != 0 &&
      strcmp(s, "Texture16") != 0 &&
      strcmp(s, "Texture24") != 0 &&
      strcmp(s, "Texture32") != 0)
  {
     printf("Error: Unknown class %s\n", object_name.c_str());
     return -1;
  }

  reg++;

  return 0;
}

int Playstation2::draw3d_object_Constructor_X(int type, bool with_texture)
{
  fprintf(out, "  ;; draw3d_object_Constructor_X(type=%d)\n", type);

  return -1;
}

int Playstation2::draw3d_object_Constructor_I(int type, bool with_texture)
{
  const int point_count = reg - 1;
  //const int object_dup = reg - 2;
  const int object = reg - 3;

  fprintf(out, "  ;; draw3d_object_Constructor_I(type=%d)\n", type);
  fprintf(out, "  li $a0, %d\n", type);
  fprintf(out, "  move $a1, $t%d\n", point_count);
  fprintf(out, "  move $s0, $ra\n");

  if (with_texture == false)
  {
    fprintf(out, "  jal _draw3d_object_constructor\n");
  }
    else
  {
    fprintf(out, "  jal _draw3d_object_with_texture_constructor\n");
  }

  fprintf(out, "  nop\n");
  fprintf(out, "  move $ra, $s0\n");
  fprintf(out, "  move $t%d, $v0\n", object);

  reg -= 2;

  return 0;
}

int Playstation2::draw3d_object_setContext_I()
{
  const int object = reg - 2;
  const int value = reg - 1;

  fprintf(out,
    "  ;; draw3d_object_setContext_I()\n"
    "  andi $t%d, $t%d, 1\n"
    "  sll $t%d, $t%d, 9\n"
    "  lw $t8, 96($t%d)\n"
    "  andi $t8, $t8, 0xfdff\n"
    "  or $t8, $t8, $t%d\n"
    "  sw $t8, 96($t%d)\n",
    value, value,
    value, value,
    object,
    value,
    object);

  reg -= 2;

  return 0;
}

int Playstation2::draw3d_object_rotateX512_I()
{
  const int object = reg - 2;
  const int x = reg - 1;

  fprintf(out,
    "  ;; draw3d_object_rotateX512_I()\n"
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
    "  lw $v1, 68($t%d)\n"
    "  ori $v1, $v1, 1\n"
    "  sw $v1, 68($t%d)\n",
    x, x,
    x, x,
    x,
    object,
    x,
    object,
    object,
    object);

  reg -= 2;

  return 0;
}

int Playstation2::draw3d_object_rotateY512_I()
{
  const int object = reg - 2;
  const int y = reg - 1;

  fprintf(out,
    "  ;; draw3d_object_rotateY512_I()\n"
    "  andi $t%d, $t%d, 511\n"
    "  sll $t%d, $t%d, 2\n"
    "  li $v1, _sin_table_512\n"
    "  addu $v1, $v1, $t%d\n"
    "  lw $v0, ($v1)\n"
    "  sw $v0, 24($t%d)\n"
    "  li $v1, _cos_table_512\n"
    "  addu $v1, $v1, $t%d\n"
    "  lw $v0, ($v1)\n"
    "  sw $v0, 28($t%d)\n"
    "  ori $v1, $0, 1\n"
    "  lw $v1, 68($t%d)\n"
    "  ori $v1, $v1, 2\n"
    "  sw $v1, 68($t%d)\n",
    y, y,
    y, y,
    y,
    object,
    y,
    object,
    object,
    object);

  reg -= 2;

  return 0;
}

int Playstation2::draw3d_object_rotateZ512_I()
{
  const int object = reg - 2;
  const int z = reg - 1;

  fprintf(out,
    "  ;; draw3d_object_rotateZ512_I()\n"
    "  andi $t%d, $t%d, 511\n"
    "  sll $t%d, $t%d, 2\n"
    "  li $v1, _sin_table_512\n"
    "  addu $v1, $v1, $t%d\n"
    "  lw $v0, ($v1)\n"
    "  sw $v0, 32($t%d)\n"
    "  li $v1, _cos_table_512\n"
    "  addu $v1, $v1, $t%d\n"
    "  lw $v0, ($v1)\n"
    "  sw $v0, 36($t%d)\n"
    "  ori $v1, $0, 1\n"
    "  lw $v1, 68($t%d)\n"
    "  ori $v1, $v1, 4\n"
    "  sw $v1, 68($t%d)\n",
    z, z,
    z, z,
    z,
    object,
    z,
    object,
    object,
    object);

  reg -= 2;

  return 0;
}

int Playstation2::draw3d_object_setPosition_FFF()
{
  const int object = reg - 4;
  const int x = reg - 3;
  const int y = reg - 2;
  const int z = reg - 1;

  fprintf(out,
    "  ;; draw3d_object_setPosition_FFF()\n"
    "  sw $t%d, 48($t%d)\n"
    "  sw $t%d, 52($t%d)\n"
    "  sw $t%d, 56($t%d)\n",
    x, object,
    y, object,
    z, object);

  reg -= 4;

  return 0;
}

int Playstation2::draw3d_object_setPoint_IFFF()
{
  const int object = reg - 5;
  const int index = reg - 4;
  const int x = reg - 3;
  const int y = reg - 2;
  const int z = reg - 1;

  fprintf(out,
    "  ;; draw3d_object_setPoint_IFFF()\n"
    "  li $at, 1\n"
    "  sw $at, -12($t%d)\n"
    "  sll $t%d, $t%d, 5\n"
    "  addiu $t%d, $t%d, 144\n"
    "  addu $t%d, $t%d, $t%d\n"
    "  sw $t%d, 0($t%d)\n"
    "  sw $t%d, 4($t%d)\n"
    "  sw $t%d, 8($t%d)\n",
    object,
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
  const int object = reg - 3;
  const int index = reg - 2;
  const int color = reg - 1;

  fprintf(out,
    "  ;; draw3d_object_setPointColor_II()\n"
    "  li $at, 1\n"
    "  sw $at, -12($t%d)\n"
    "  sll $t%d, $t%d, 5\n"
    "  addiu $t%d, $t%d, 128\n"
    "  addu $t%d, $t%d, $t%d\n"
    "  sw $t%d, 0($t%d)\n",
    object,
    index, index,
    object, object,
    object, object, index,
    color, object);

  reg -= 3;

  return 0;
}

int Playstation2::draw3d_object_setPoints_aF()
{
  const int array = reg - 1;
  const int object = reg - 2;

  fprintf(out,
    "  ;; draw3d_object_setPoints_aF()\n"
    "  li $at, 1\n"
    "  sw $at, -12($t%d)\n"
    "  lw $t8, -4($t%d)\n"
    "  addiu $t%d, $t%d, 144\n"
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
    object,
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
  const int array = reg - 1;
  const int object = reg - 2;

  fprintf(out,
    "  ;; draw3d_object_setPointColors_aI()\n"
    "  li $at, 1\n"
    "  sw $at, -12($t%d)\n"
    "  lw $t8, -4($t%d)\n"
    "  addiu $t%d, $t%d, 128\n"
    "_set_point_colors_%d:\n"
    "  lw $at, 0($t%d)\n"
    "  sw $at, 0($t%d)\n"
    "  addiu $t%d, $t%d, 4\n"
    "  addiu $t%d, $t%d, 32\n"
    "  addiu $t8, $t8, -1\n"
    "  bne $t8, $0, _set_point_colors_%d\n"
    "  nop\n",
    object,
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

int Playstation2::draw3d_object_with_texture_setPoint_IFFF()
{
  const int object = reg - 5;
  const int index = reg - 4;
  const int x = reg - 3;
  const int y = reg - 2;
  const int z = reg - 1;

  fprintf(out,
    "  ;; draw3d_object_with_texture_setPoint_IFFF()\n"
    "  li $at, 1\n"
    "  sw $at, -12($t%d)\n"
    "  sll $at, $t%d, 5\n"
    "  sll $t%d, $t%d, 4\n"
    "  addu $t%d, $t%d, $at\n"
    "  addiu $t%d, $t%d, 160\n"
    "  addu $t%d, $t%d, $t%d\n"
    "  sw $t%d, 0($t%d)\n"
    "  sw $t%d, 4($t%d)\n"
    "  sw $t%d, 8($t%d)\n",
    object,
    index,
    index, index,
    index, index,
    object, object,
    object, object, index,
    x, object,
    y, object,
    z, object);

  reg -= 5;

  return 0;
}

int Playstation2::draw3d_object_with_texture_setPointColor_II()
{
  const int object = reg - 3;
  const int index = reg - 2;
  const int color = reg - 1;

  fprintf(out,
    "  ;; draw3d_object_with_texture_setPointColor_II()\n"
    "  li $at, 1\n"
    "  sw $at, -12($t%d)\n"
    "  sll $at, $t%d, 5\n"
    "  sll $t%d, $t%d, 4\n"
    "  addu $t%d, $t%d, $at\n"
    "  addiu $t%d, $t%d, 128\n"
    "  addu $t%d, $t%d, $t%d\n"
    "  sw $t%d, 0($t%d)\n",
    object,
    index,
    index, index,
    index, index,
    object, object,
    object, object, index,
    color, object);

  reg -= 3;

  return 0;
}

int Playstation2::draw3d_object_with_texture_setPoints_aF()
{
  const int array = reg - 1;
  const int object = reg - 2;

  fprintf(out,
    "  ;; draw3d_object_with_texture_setPoints_aF()\n"
    "  li $at, 1\n"
    "  sw $at, -12($t%d)\n"
    "  lw $t8, -4($t%d)\n"
    "  addiu $t%d, $t%d, 160\n"
    "_set_points_%d:\n"
    "  lw $at, 0($t%d)\n"
    "  sw $at, 0($t%d)\n"
    "  lw $at, 4($t%d)\n"
    "  sw $at, 4($t%d)\n"
    "  lw $at, 8($t%d)\n"
    "  sw $at, 8($t%d)\n"
    "  addiu $t%d, $t%d, 12\n"
    "  addiu $t%d, $t%d, 48\n"
    "  addiu $t8, $t8, -3\n"
    "  bne $t8, $0, _set_points_%d\n"
    "  nop\n",
    object,
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

int Playstation2::draw3d_object_with_texture_setPointColors_aI()
{
  const int array = reg - 1;
  const int object = reg - 2;

  fprintf(out,
    "  ;; draw3d_object_with_texture_setPointColors_aI()\n"
    "  li $at, 1\n"
    "  sw $at, -12($t%d)\n"
    "  lw $t8, -4($t%d)\n"
    "  addiu $t%d, $t%d, 128\n"
    "_set_point_colors_%d:\n"
    "  lw $at, 0($t%d)\n"
    "  sw $at, 0($t%d)\n"
    "  addiu $t%d, $t%d, 4\n"
    "  addiu $t%d, $t%d, 48\n"
    "  addiu $t8, $t8, -1\n"
    "  bne $t8, $0, _set_point_colors_%d\n"
    "  nop\n",
    object,
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

int Playstation2::draw3d_object_setTextureCoord_IFF()
{
  const int object = reg - 4;
  const int index = reg - 3;
  const int coord_s = reg - 2;
  const int coord_t = reg - 1;

  fprintf(out,
    "  ;; draw3d_object_setTextureCoord_IFF()\n"
    "  li $at, 1\n"
    "  sw $at, -12($t%d)\n"
    "  sll $at, $t%d, 5\n"
    "  sll $t%d, $t%d, 4\n"
    "  addu $at, $at, $t%d\n"
    "  addiu $t%d, $t%d, 144\n"
    "  addu $t%d, $t%d, $at\n"
    "  sw $t%d, 0($t%d)\n"
    "  sw $t%d, 4($t%d)\n",
    object,
    index,
    index, index,
    index,
    object, object,
    object, object,
    coord_s, object,
    coord_t, object);

  reg -= 4;

  return 0;
}

int Playstation2::draw3d_object_setTextureCoords_aF()
{
  const int array = reg - 1;
  const int object = reg - 2;

  fprintf(out,
    "  ;; draw3d_object_setTextureCoords_aF()\n"
    "  li $at, 1\n"
    "  sw $at, -12($t%d)\n"
    "  lw $t8, -4($t%d)\n"
    "  addiu $t%d, $t%d, 144\n"
    "_set_point_colors_%d:\n"
    "  lw $at, 0($t%d)\n"
    "  sw $at, 0($t%d)\n"
    "  lw $at, 4($t%d)\n"
    "  sw $at, 4($t%d)\n"
    "  addiu $t%d, $t%d, 8\n"
    "  addiu $t%d, $t%d, 48\n"
    "  addiu $t8, $t8, -2\n"
    "  bne $t8, $0, _set_point_colors_%d\n"
    "  nop\n",
    object,
    array,
    object, object,
    label_count,
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

int Playstation2::draw3d_object_disableGouraudShading()
{
  const int object = reg - 1;

  fprintf(out,
    "  ;; disableGouraudShading()\n"
    "  lw $t8, 96($t%d)\n"
    "  andi $t8, $t8, 0xfff7\n"
    "  sw $t8, 96($t%d)\n",
    object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_object_enableGouraudShading()
{
  const int object = reg - 1;

  fprintf(out,
    "  ;; enableGouraudShading()\n"
    "  lw $t8, 96($t%d)\n"
    "  ori $t8, $t8, 0x8\n"
    "  sw $t8, 96($t%d)\n",
    object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_object_disableFogging()
{
  const int object = reg - 1;

  fprintf(out,
    "  ;; disableFogging()\n"
    "  lw $t8, 96($t%d)\n"
    "  andi $t8, $t8, 0xffdf\n"
    "  sw $t8, 96($t%d)\n",
    object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_object_enableFogging()
{
  const int object = reg - 1;

  fprintf(out,
    "  ;; enableFogging()\n"
    "  lw $t8, 96($t%d)\n"
    "  ori $t8, $t8, 0x20\n"
    "  sw $t8, 96($t%d)\n",
    object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_object_disableTexture()
{
  const int object = reg - 1;

  fprintf(out,
    "  ;; disableTexture()\n"
    "  lw $t8, 96($t%d)\n"
    "  andi $t8, $t8, 0xffef\n"
    "  sw $t8, 96($t%d)\n",
    object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_object_enableTexture()
{
  const int object = reg - 1;

  fprintf(out,
    "  ;; enableTexture()\n"
    "  lw $t8, 96($t%d)\n"
    "  ori $t8, $t8, 0x10\n"
    "  sw $t8, 96($t%d)\n",
    object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_object_disableAlphaBlending()
{
  const int object = reg - 1;

  fprintf(out,
    "  ;; disableAlphaBlending()\n"
    "  lw $t8, 96($t%d)\n"
    "  andi $t8, $t8, 0xffbf\n"
    "  sw $t8, 96($t%d)\n",
    object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_object_enableAlphaBlending()
{
  const int object = reg - 1;

  fprintf(out,
    "  ;; enableAlphaBlending()\n"
    "  lw $t8, 96($t%d)\n"
    "  ori $t8, $t8, 0x40\n"
    "  sw $t8, 96($t%d)\n",
    object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_object_disableAntialiasing()
{
  const int object = reg - 1;

  fprintf(out,
    "  ;; disableAntialiasing()\n"
    "  lw $t8, 96($t%d)\n"
    "  andi $t8, $t8, 0xff7f\n"
    "  sw $t8, 96($t%d)\n",
    object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_object_enableAntialiasing()
{
  const int object = reg - 1;

  fprintf(out,
    "  ;; enableAntialiasing()\n"
    "  lw $t8, 96($t%d)\n"
    "  ori $t8, $t8, 0x80\n"
    "  sw $t8, 96($t%d)\n",
    object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_object_draw()
{
  const int object = reg - 1;

  fprintf(out,
    "  ;; draw3d_object_draw()\n"
    "  move $a0, $t%d\n"
    "  move $s0, $ra\n"
    "  jal _draw3d_object_draw\n"
    "  nop\n"
    "  move $ra, $s0\n",
    object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_texture_Constructor_II(int size)
{
  const int height = reg - 1;
  const int width = reg - 2;
  //const int object_dup = reg - 3;
  const int object = reg - 4;

  fprintf(out, "  ;; draw3d_texture_Constructor_II()\n");
  fprintf(out, "  move $a0, $t%d\n", width);
  fprintf(out, "  move $a1, $t%d\n", height);
  fprintf(out, "  move $s0, $ra\n");

  switch (size)
  {
    case 16:
      fprintf(out, "  jal _draw3d_texture16_constructor\n");
      need_draw3d_texture16 = true;
      break;
    case 24:
      fprintf(out, "  jal _draw3d_texture24_constructor\n");
      need_draw3d_texture24 = true;
      break;
    case 32:
      fprintf(out, "  jal _draw3d_texture32_constructor\n");
      need_draw3d_texture32 = true;
      break;
  }

  fprintf(out, "  nop\n");
  fprintf(out, "  move $ra, $s0\n");
  fprintf(out, "  move $t%d, $v0\n", object);

  reg -= 3;

  return 0;
}

int Playstation2::draw3d_texture16_setPixel_II()
{
  const int color = reg - 1;
  const int index = reg - 2;
  const int object = reg - 3;

  fprintf(out,
    "  ;; draw3d_texture16_setPixel_II()\n"
    "  li $at, 1\n"
    "  sw $at, -12($t%d)\n"
    "  addiu $t%d, $t%d, 208\n"
    "  sll $t%d, $t%d, 1\n"
    "  addu $t%d, $t%d, $t%d\n"
    "  sh $t%d, 0($t%d)\n",
    object,
    object, object,
    index, index,
    object, object, index,
    color, object);

  reg -= 3;

  return 0;
}

int Playstation2::draw3d_texture16_setPixels_IaS()
{
  const int array = reg - 1;
  const int index = reg - 2;
  const int object = reg - 3;

  fprintf(out,
    "  ;; draw3d_texture16_setPixel_IaI()\n"
    "  li $at, 1\n"
    "  sw $at, -12($t%d)\n"
    "  sll $t%d, $t%d, 1\n"
    "  addiu $t%d, $t%d, 208\n"
    "  addu $t%d, $t%d, $t%d\n"
    "  lw $t9, -4($t%d)\n"
    "draw3d_texture16_setPixels_%d:\n"
    "  lh $at, 0($t%d)\n"
    "  sh $at, 0($t%d)\n"
    "  addiu $t%d, $t%d, 2\n"
    "  addiu $t%d, $t%d, 2\n"
    "  addiu $t9, $t9, -1\n"
    "  bne $t9, $0, draw3d_texture16_setPixels_%d\n"
    "  nop\n",
    object,
    index, index,
    object, object,
    object, object, index,
    array,
    label_count,
    array,
    object,
    array, array,
    object, object,
    label_count);

  label_count++;
  reg -= 3;

  return 0;
}

int Playstation2::draw3d_texture16_setPixelsRLE16_IaB()
{
  const int reg_image_array = reg - 1;
  const int index = reg - 2;
  const int object = reg - 3;

  fprintf(out,
    "  ;; draw3d_texture16_setPixelsRLE16_IaB()\n"
    "  move $a0, $t%d\n"
    "  move $a1, $t%d\n"
    "  move $a2, $t%d\n"
    "  move $s0, $ra\n"
    "  jal _draw3d_texture16_setPixelsRLE16_IaB\n"
    "  nop\n"
    "  move $ra, $s0\n",
    object,
    index,
    reg_image_array);

  reg -= 3;

  return 0;
}

int Playstation2::draw3d_texture24_setPixel_II()
{
  const int color = reg - 1;
  const int index = reg - 2;
  const int object = reg - 3;

  fprintf(out,
    "  ;; draw3d_texture24_setPixel_II()\n"
    "  li $at, 1\n"
    "  sw $at, -12($t%d)\n"
    "  addiu $t%d, $t%d, 208\n"
    "  sll $at, $t%d, 1\n"
    "  addu $t%d, $t%d, $at\n"
    "  addu $t%d, $t%d, $t%d\n"
    "  sb $t%d, 0($t%d)\n"
    "  srl $t%d, $t%d, 8\n"
    "  sb $t%d, 1($t%d)\n"
    "  srl $t%d, $t%d, 8\n"
    "  sb $t%d, 2($t%d)\n",
    object,
    object, object,
    index,
    index, index,
    object, object, index,
    color, object,
    color, color,
    color, object,
    color, color,
    color, object);

  reg -= 3;

  return 0;
}

int Playstation2::draw3d_texture24_setPixels_IaI()
{
  const int array = reg - 1;
  const int index = reg - 2;
  const int object = reg - 3;

  // FIXME: This should probably be a function, not inline
  fprintf(out,
    "  ;; draw3d_texture24_setPixel_IaI()\n"
    "  li $at, 1\n"
    "  sw $at, -12($t%d)\n"
    "  sll $at, $t%d, 1\n"
    "  addu $t%d, $t%d, $at\n"
    "  addiu $t%d, $t%d, 208\n"
    "  addu $t%d, $t%d, $t%d\n"
    "  lw $t9, -4($t%d)\n"
    "draw3d_texture24_setPixels_%d:\n"
    "  lb $at, 0($t%d)\n"
    "  sb $at, 0($t%d)\n"
    "  sb $at, 1($t%d)\n"
    "  lb $at, 1($t%d)\n"
    "  lb $at, 2($t%d)\n"
    "  sb $at, 2($t%d)\n"
    "  addiu $t%d, $t%d, 3\n"
    "  addiu $t%d, $t%d, 3\n"
    "  addiu $t9, $t9, -1\n"
    "  bne $t9, $0, draw3d_texture24_setPixels_%d\n"
    "  nop\n",
    object,
    index,
    index, index,
    object, object,
    object, object, index,
    array,
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
  reg -= 3;

  return 0;
}
int Playstation2::draw3d_texture32_setPixel_II()
{
  const int color = reg - 1;
  const int index = reg - 2;
  const int object = reg - 3;

  fprintf(out,
    "  ;; draw3d_texture32_setPixel_II()\n"
    "  li $at, 1\n"
    "  sw $at, -12($t%d)\n"
    "  addiu $t%d, $t%d, 208\n"
    "  sll $t%d, $t%d, 2\n"
    "  addu $t%d, $t%d, $t%d\n"
    "  sw $t%d, 0($t%d)\n",
    object,
    object, object,
    index, index,
    object, object, index,
    color, object);

  reg -= 3;

  return 0;
}

int Playstation2::draw3d_texture32_setPixels_IaI()
{
  const int array = reg - 1;
  const int index = reg - 2;
  const int object = reg - 3;

  fprintf(out,
    "  ;; draw3d_texture32_setPixel_IaI()\n"
    "  li $at, 1\n"
    "  sw $at, -12($t%d)\n"
    "  sll $t%d, $t%d, 2\n"
    "  addiu $t%d, $t%d, 208\n"
    "  addu $t%d, $t%d, $t%d\n"
    "  lw $t9, -4($t%d)\n"
    "draw3d_texture32_setPixels_%d:\n"
    "  lw $at, 0($t%d)\n"
    "  sw $at, 0($t%d)\n"
    "  addiu $t%d, $t%d, 4\n"
    "  addiu $t%d, $t%d, 4\n"
    "  addiu $t9, $t9, -1\n"
    "  bne $t9, $0, draw3d_texture32_setPixels_%d\n"
    "  nop\n",
    object,
    index, index,
    object, object,
    object, object, index,
    array,
    label_count,
    array,
    object,
    array, array,
    object, object,
    label_count);

  label_count++;
  reg -= 3;

  return 0;
}

int Playstation2::draw3d_texture_enableTransparency()
{
  const int object = reg - 1;

  fprintf(out,
    "  ;; enableTransparency()\n"
    "  li $at, 1\n"
    "  sw $at, -12($t%d)\n"
    "  lw $t8, 64($t%d)\n"
    "  andi $t8, $t8, 0x7fff\n"
    "  sw $t8, 64($t%d)\n",
    object, object, object);

  fprintf(out,
    "  lw $t8, 100($t%d)\n"
    "  ori $t8, $t8, 0x0004\n"
    "  sw $t8, 100($t%d)\n"
    "  sw $t8, 116($t%d)\n",
    object, object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_texture_enableTransparencyOnBlack()
{
  const int object = reg - 1;

  fprintf(out,
    "  ;; enableTransparency()\n"
    "  li $at, 1\n"
    "  sw $at, -12($t%d)\n"
    "  lw $t8, 64($t%d)\n"
    "  ori $t8, $t8, 0x8000\n"
    "  sw $t8, 64($t%d)\n",
    object, object, object);

  fprintf(out,
    "  lw $t8, 100($t%d)\n"
    "  ori $t8, $t8, 0x0004\n"
    "  sw $t8, 100($t%d)\n"
    "  sw $t8, 116($t%d)\n",
    object, object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_texture_disableTransparency()
{
  const int object = reg - 1;

  // NOTE: TEX0 sits at offsets 96 and 112, but the TCC bit is
  // in the upper 32 bits.  Both values should always be the same.

  fprintf(out,
    "  ;; disableTransparency()\n"
    "  li $at, 1\n"
    "  sw $at, -12($t%d)\n"
    "  lw $t8, 100($t%d)\n"
    "  andi $t8, $t8, 0xfffb\n"
    "  sw $t8, 100($t%d)\n"
    "  sw $t8, 116($t%d)\n",
    object, object, object, object);

  reg -= 1;

  return 0;
}

int Playstation2::draw3d_texture_upload()
{
  const int object = reg - 1;

  fprintf(out,
    "  ;; draw3d_texture_upload()\n"
    "  move $a0, $t%d\n"
    "  move $s0, $ra\n"
    "  jal _draw3d_texture_upload\n"
    "  nop\n"
    "  move $ra, $s0\n",
    object);

  reg -= 1;

  return 0;
}

int Playstation2::playstation2_clearContext_I()
{
  const int context = reg - 1;

  fprintf(out,
    "  ;; playstation2_clearContext_I()\n"
    "  move $a0, $t%d\n"
    "  move $s0, $ra\n"
    "  jal _clear_context\n"
    "  nop\n"
    "  move $ra, $s0\n",
    context);

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

int Playstation2::playstation2_showContext_I()
{
  const int value = reg - 1;

  fprintf(out,
    "  ;; showContext_I()\n"
    "  move $a0, $t%d\n"
    "  move $s0, $ra\n"
    "  jal _show_context\n"
    "  nop\n"
    "  move $ra, $s0\n",
    value);

  reg -= 1;

  return 0;
}

int Playstation2::playstation2_vu0UploadCode_aB()
{
  const int array = reg - 1;

  fprintf(out,
    "  ;; playstation2_vu0UploadCode_aB()\n"
    "  move $a0, $t%d\n"
    "  move $s0, $ra\n"
    "  jal _vu0_upload_code\n"
    "  nop\n"
    "  move $ra, $s0\n",
    array);

  reg--;

  return 0;
}

int Playstation2::playstation2_vu0UploadData_IaB()
{
  const int array = reg - 1;
  const int index = reg - 2;

  fprintf(out,
    "  ;; playstation2_vu0UploadData_IaB()\n"
    "  move $a0, $t%d\n"
    "  move $a1, $t%d\n"
    "  move $s0, $ra\n"
    "  jal _vu0_upload_data_bytes\n"
    "  nop\n"
    "  move $ra, $s0\n",
    index,
    array);

  reg -= 2;

  return 0;
}

int Playstation2::playstation2_vu0UploadData_IaI()
{
  const int array = reg - 1;
  const int index = reg - 2;

  fprintf(out,
    "  ;; playstation2_vu0UploadData_IaI()\n"
    "  move $a0, $t%d\n"
    "  move $a1, $t%d\n"
    "  move $s0, $ra\n"
    "  jal _vu0_upload_data_words\n"
    "  nop\n"
    "  move $ra, $s0\n",
    index,
    array);

  reg -= 2;

  return 0;
}

int Playstation2::playstation2_vu0UploadData_IaF()
{
  const int array = reg - 1;
  const int index = reg - 2;

  fprintf(out,
    "  ;; playstation2_vu0UploadData_IaF()\n"
    "  move $a0, $t%d\n"
    "  move $a1, $t%d\n"
    "  move $s0, $ra\n"
    "  jal _vu0_upload_data_words\n"
    "  nop\n"
    "  move $ra, $s0\n",
    index,
    array);

  reg -= 2;

  return 0;
}

int Playstation2::playstation2_vu0DownloadData_IaB()
{
  const int array = reg - 1;
  const int index = reg - 2;

  fprintf(out,
    "  ;; playstation2_vu0DownloadData_IaB()\n"
    "  move $a0, $t%d\n"
    "  move $a1, $t%d\n"
    "  move $s0, $ra\n"
    "  jal _vu0_download_data_bytes\n"
    "  nop\n"
    "  move $ra, $s0\n",
    index,
    array);

  reg -= 2;

  return 0;
}

int Playstation2::playstation2_vu0DownloadData_IaI()
{
  const int array = reg - 1;
  const int index = reg - 2;

  fprintf(out,
    "  ;; playstation2_vu0DownloadData_IaI()\n"
    "  move $a0, $t%d\n"
    "  move $a1, $t%d\n"
    "  move $s0, $ra\n"
    "  jal _vu0_download_data_words\n"
    "  nop\n"
    "  move $ra, $s0\n",
    index,
    array);

  reg -= 2;

  return 0;
}

int Playstation2::playstation2_vu0DownloadData_IaF()
{
  const int array = reg - 1;
  const int index = reg - 2;

  fprintf(out,
    "  ;; playstation2_vu0DownloadData_IaF()\n"
    "  move $a0, $t%d\n"
    "  move $a1, $t%d\n"
    "  move $s0, $ra\n"
    "  jal _vu0_download_data_words\n"
    "  nop\n"
    "  move $ra, $s0\n",
    index,
    array);

  reg -= 2;

  return 0;
}

int Playstation2::playstation2_vu0Start()
{
#if 0
  fprintf(out,
    "  ;; Send data (VIF_UNPACK) and start VU1 (VIF_MSCAL)\n"
    "  li $v0, D0_CHCR\n"
    "  li $v1, _vif_vu0_start\n"
    "  sw $v1, 0x10($v0)                   ; DMA00 ADDRESS\n"
    "  li $v1, ((_vif_vu0_end - _vif_vu0_start) / 16)\n"
    "  sw $v1, 0x20($v0)                   ; DMA00 SIZE\n"
    "  li $v1, 0x101\n"
    "  sw $v1, ($v0)                       ; start\n");
#endif

  fprintf(out,
    "  ;; vu0Start()\n"
    "  vcallms 0\n");

  return 0;
}

int Playstation2::playstation2_vu0Stop()
{
#if 0
  fprintf(out,
    "  ;; vu0Stop()\n"
    "  li $at, 0xf\n"
    "  ctc2 $at, 28\n");
#endif

  fprintf(out,
    "  ;; vu0Stop()\n"
    "  li $v0, VIF0_FBRST\n"
    "  li $v1, 0xf\n"
    "  sw $v1, 0($v0)\n");

  return 0;
}

int Playstation2::playstation2_vu0IsRunning()
{
  fprintf(out,
    "  ;; vu0IsRunning()\n"
    "  cfc2 $at, 29\n"
    "  andi $t%d, $at, 0x01\n",
    reg);

#if 0
  fprintf(out,
    "  ;; vu0IsRunning()\n"
    "  li $v1, VIF0_STAT\n"
    "  lw $v0, ($v1)\n"
    "  andi $t%d, $v0, 0x04\n",
    reg);
#endif


  reg++;

  return 0;
}

int Playstation2::playstation2_performanceCountEnable_I()
{
  fprintf(out,
    "  ;; performanceCountDisable()\n"
    "  li $at, 0x8000_001c\n"
    "  sll $t%d, $t%d, 5\n"
    "  or $at, $at, $t%d\n"
    "  mtpc $at, 0\n"
    "  sync.p\n",
    reg - 1, reg - 1,
    reg - 1);

  reg -= 1;

  return 0;
}

int Playstation2::playstation2_performanceCountDisable()
{
  fprintf(out,
    "  ;; performanceCountDisable()\n"
    "  li $at, 0\n"
    "  mtpc $at, 0\n"
    "  sync.p\n");

  return 0;
}

int Playstation2::playstation2_performanceCountGet()
{
  fprintf(out,
    "  ;; getPerformanceCount()\n"
    "  mfpc $t%d, 0\n",
    reg);

  reg++;
  return 0;
}

int Playstation2::playstation2_performanceCountReset()
{
  fprintf(out,
    "  ;; performanceCountReset()\n"
    "  li $at, 0\n"
    "  mtpc $at, 0\n");

  return 0;
}

int Playstation2::playstation2_randomInit_I()
{
  fprintf(out,
    "  ;; randomInit()\n"
    "  qmtc2 $t%d, $vf01\n"
    "  vrinit R, $vf01x\n",
    reg - 1);

  reg--;

  return 0;
}

int Playstation2::playstation2_randomGet()
{
  fprintf(out,
    "  ;; randomGet()\n"
    "  vrget.x $vf01x, R\n"
    "  qmfc2 $t%d, $vf01\n"
    "  li $at, 0x007fffff\n"
    "  and $t%d, $t%d, $at\n",
    reg,
    reg, reg);

  reg++;

  return 0;
}

int Playstation2::playstation2_randomNext()
{
  fprintf(out,
    "  ;; randomNext()\n"
    "  vrnext.x $vf01x, R\n"
    "  qmfc2 $t%d, $vf01\n"
    "  li $at, 0x007fffff\n"
    "  and $t%d, $t%d, $at\n",
    reg,
    reg, reg);

  reg++;

  return 0;
}

int Playstation2::playstation2_spuInit()
{
  fprintf(out,
    "  ;; playstation2_spuInit()\n"
    "  move $s0, $ra\n"
    "  jal _spu_init\n"
    "  nop\n"
    "  move $ra, $s0\n");

  need_spu_functions = true;

  return 0;
}

int Playstation2::playstation2_spuUploadSoundData_aB()
{
  const int array = reg - 1;

  fprintf(out,
    "  ;; playstation2_spuUploadSoundData_aB()\n"
    "  move $a0, $t%d\n"
    "  move $s0, $ra\n"
    "  jal _upload_sound_data\n"
    "  nop\n"
    "  move $ra, $s0\n",
    array);

  need_spu_functions = true;

  reg -= 1;

  return 0;
}

int Playstation2::playstation2_spuSetVolume_II()
{
  const int value = reg - 1;
  const int voice = reg - 2;

  fprintf(out,
    "  ;; playstation2_spuSetVolume_II()\n"
    KERNEL_ENTER
    "  li $v1, 0xbf90_0000\n"
    "  sll $t%d, $t%d, 4\n"
    "  addu $v1, $v1, $t%d\n"
    "  sh $t%d, 0x0000($v1)\n"
    "  sh $t%d, 0x0002($v1)\n"
    KERNEL_EXIT,
    voice, voice,
    voice,
    value,
    value);

  reg -= 2;

  return 0;
}

int Playstation2::playstation2_spuSetMasterVolume_I()
{
  const int value = reg - 1;

  fprintf(out,
    "  ;; playstation2_spuSetMasterVolume_I() - MVOLL/MVOLR\n"
    KERNEL_ENTER
    "  li $v1, 0xbf90_0000\n"
    "  sh $t%d, 0x0760($v1)\n"
    "  sh $t%d, 0x0762($v1)\n"
    KERNEL_EXIT,
    value, value);

  reg -= 1;

  return 0;
}

int Playstation2::playstation2_spuSetPitch_II()
{
  const int value = reg - 1;
  const int voice = reg - 2;

  fprintf(out,
    "  ;; playstation2_spuSetPitch_II() - PITCH\n"
    KERNEL_ENTER
    "  li $v1, 0xbf90_0000\n"
    "  sll $t%d, $t%d, 4\n"
    "  addu $v1, $v1, $t%d\n"
    "  sh $t%d, 0x0004($v1)\n"
    KERNEL_EXIT,
    voice, voice,
    voice,
    value);

  reg -= 2;

  return 0;
}

int Playstation2::playstation2_spuKeyOn_I()
{
  const int value = reg - 1;

  fprintf(out,
    "  ;; playstation2_spuKeyOn_I() - KON0/KON1\n"
    KERNEL_ENTER
    "  li $v1, 0xbf90_0000\n"
    "  li $at, 1\n"
    "  sllv $at, $at, $t%d\n"
    "  sh $at, 0x01a0($v1)\n"
    KERNEL_EXIT,
    value);

  reg -= 1;

  return 0;
}

int Playstation2::playstation2_spuKeyOff_I()
{
  const int value = reg - 1;

  fprintf(out,
    "  ;; playstation2_spuKeyOn_I() - KOF0/KOF1\n"
    KERNEL_ENTER
    "  li $v1, 0xbf90_0000\n"
    "  li $at, 1\n"
    "  sllv $at, $at, $t%d\n"
    "  sh $at, 0x01a4($v1)\n"
    KERNEL_EXIT,
    value);

  reg -= 1;

  return 0;
}

int Playstation2::math_sin512_I()
{
  const int value = reg - 1;

  fprintf(out,
    "  ;; math_sin512_I()\n"
    "  andi $t%d, $t%d, 511\n"
    "  sll $t%d, $t%d, 2\n"
    "  li $v1, _sin_table_512\n"
    "  addu $v1, $v1, $t%d\n"
    "  lwu $t%d, ($v1)\n",
    value, value,
    value, value,
    value,
    value);

  return 0;
}

int Playstation2::math_cos512_I()
{
  const int value = reg - 1;

  fprintf(out,
    "  ;; math_cos512_I()\n"
    "  andi $t%d, $t%d, 511\n"
    "  sll $t%d, $t%d, 2\n"
    "  li $v1, _cos_table_512\n"
    "  addu $v1, $v1, $t%d\n"
    "  lwu $t%d, ($v1)\n",
    value, value,
    value, value,
    value,
    value);

  return 0;
}

int Playstation2::push_registers()
{
  fprintf(out,
    "  move $s0, $sp\n"
    "  addiu $at, $0, -16\n"
    "  and $sp, $sp, $at\n"
    "  addiu $sp, $0, -16\n"
    "  sw $gp, 12($sp)\n"
    "  sw $fp, 8($sp)\n"
    "  sw $ra, 4($sp)\n"
    "  sw $s0, 0($sp)\n");

  return 0;
}

int Playstation2::pop_registers()
{
  fprintf(out,
    "  lwu $gp, 12($sp)\n"
    "  lwu $fp, 8($sp)\n"
    "  lwu $ra, 4($sp)\n"
    "  lwu $s0, 0($sp)\n"
    "  move $sp, $s0\n");

  return 0;
}

void Playstation2::add_dma_functions()
{
  fprintf(out,
    ".align 32\n"
    "_dma_reset:\n"
    "  ;; VIF0\n"
    "  li $v1, D0_CHCR\n"
    "  sw $zero, 0x00($v1)    ; D0_CHCR\n"
    "  sw $zero, 0x30($v1)    ; D0_TADR\n"
    "  sw $zero, 0x10($v1)    ; D0_MADR\n"
    "  sw $zero, 0x50($v1)    ; D0_ASR1\n"
    "  sw $zero, 0x40($v1)    ; D0_ASR0\n\n"

    "  ;; VIF1\n"
    "  li $v1, D1_CHCR\n"
    "  sw $zero, 0x00($v1)    ; D1_CHCR\n"
    "  sw $zero, 0x30($v1)    ; D1_TADR\n"
    "  sw $zero, 0x10($v1)    ; D1_MADR\n"
    "  sw $zero, 0x50($v1)    ; D1_ASR1\n"
    "  sw $zero, 0x40($v1)    ; D1_ASR0\n\n"

    "  ;; GIF\n"
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

  fprintf(out,
    "_dma00_wait:\n"
    "  li $v1, D0_CHCR\n"
    "_dma00_wait_loop:\n"
    "  lw $v0, ($v1)\n"
    "  andi $v0, $v0, 0x100\n"
    "  bnez $v0, _dma00_wait_loop\n"
    "  nop\n"
    "  jr $ra\n"
    "  nop\n\n"

    "_dma01_wait:\n"
    "  li $v1, D0_CHCR\n"
    "_dma01_wait_loop:\n"
    "  lw $v0, ($v1)\n"
    "  andi $v0, $v0, 0x100\n"
    "  bnez $v0, _dma01_wait_loop\n"
    "  nop\n"
    "  jr $ra\n"
    "  nop\n\n"

    "_dma02_wait:\n"
    "  li $v1, D2_CHCR\n"
    "_dma02_wait_loop:\n"
    "  lw $v0, ($v1)\n"
    "  andi $v0, $v0, 0x100\n"
    "  bnez $v0, _dma02_wait_loop\n"
    "  nop\n"
    "  jr $ra\n"
    "  nop\n\n"

    "_dma06_wait:\n"
    "  li $v1, D2_CHCR\n"
    "_dma06_wait_loop:\n"
    "  lw $v0, ($v1)\n"
    "  andi $v0, $v0, 0x100\n"
    "  bnez $v0, _dma06_wait_loop\n"
    "  nop\n"
    "  jr $ra\n"
    "  nop\n\n");
}

void Playstation2::add_misc_functions()
{
  fprintf(out,
    "_show_context:\n"
    "  andi $a0, $a0, 1\n"
    "  li $v0, GS_DISPFB2\n"
    "  li $v1, (10 << 9) | (FMT_PSMCT24 << 15)\n"
    "  beq $a0, $0, _show_context_1\n"
    "  nop\n"
    "  ori $v1, $v1, 210\n"
    "_show_context_1:\n"
    "  sd $v1, ($v0)\n"
    "  jr $ra\n"
    "  nop\n\n");

  fprintf(out,
    "_clear_context:\n"
    "  andi $a0, $a0, 1\n"
    DMA02_WAIT
    "  li $v1, _screen_init_clear_1\n"
    "  beq $a0, $0, _clear_context_1\n"
    "  nop\n"
    "  li $v1, _screen_init_clear_2\n"
    "_clear_context_1:\n"
    "  li $v0, D2_CHCR\n"
    "  sw $v1, 0x10($v0)         ; DMA02 ADDRESS\n"
    "  li $v1, (_screen_init_clear_1_end - _screen_init_clear_1) / 16\n"
    "  sw $v1, 0x20($v0)         ; DMA02 SIZE\n"
    "  li $v1, 0x101\n"
    "  sw $v1, ($v0)             ; start\n"
    "  jr $ra\n"
    "  nop\n\n");
}

void Playstation2::add_draw3d_object_draw()
{
  fprintf(out,
    "  ;; _draw3d_object_draw()\n"
    "  ;; Copy GIF packet to VU1's data memory segment.\n"
    "_draw3d_object_draw:\n");

  // Clear cache for this object.
  // Find the number of quadwords (16 byte) elements in this object.
  // Since there are 4 elements per 64 byte cache line, figure out how
  // many cache lines need to be flushed by taking (count + 3) / 4.
  // The addresses at object + 16 to object + 96 need to be flushed
  // if the object has changed.
  // Not sure why all the sync.l's are needed, but it fails to work
  // without them.
  fprintf(out,
    "  ; Flush cache\n"
    "  sync.l\n"
    "  cache dhwbin, 0($a0)\n"
    "  sync.l\n"
    "  cache dhwbin, 64($a0)\n"
    "  sync.l\n"
    "  cache dhwbin, 128($a0)\n"
    "  sync.l\n"
    "  cache dhwbin, 192($a0)\n"
    "  sync.l\n"
    "  lw $v0, -12($a0)\n"
    "  beqz $v0, _draw3d_object_draw_skip_cache_flush\n"
    "  nop\n"
    "  sw $0, -12($a0)\n"
    "  lw $v0, -16($a0)\n"
    "  addiu $v1, $a0, 128\n"
    "  addiu $v0, $v0, 1\n"
    "  srl $v0, $v0, 2\n"
    "_draw3d_object_draw_cache_flush:\n"
    "  sync.l\n"
    "  cache dhwbin, 0($v1)\n"
    "  sync.l\n"
    "  addiu $v0, $v0, -1\n"
    "  addiu $v1, $v1, 64\n"
    "  bnez $v0, _draw3d_object_draw_cache_flush\n"
    "  nop\n"
    "_draw3d_object_draw_skip_cache_flush:\n");

  fprintf(out,
    "  ;; Wait till DMA01 is free\n"
    "  li $v0, D1_CHCR\n"
    "_draw_dma01_wait:\n"
    "  lw $v1, ($v0)\n"
    "  andi $v1, $v1, 0x100\n"
    "  bnez $v1, _draw_dma01_wait\n"
    "  nop\n" \
    "  ;; Start the VU1 with a VIF packet\n"
    "  sw $a0, 0x10($v0)         ; DMA01 ADDRESS\n"
    "  lw $v1, -16($a0)          ; Length of VIF data\n"
    "  sw $v1, 0x20($v0)         ; DMA01 SIZE\n"
    "  li $v1, 0x101\n"
    "  sw $v1, ($v0)             ; start\n"
    "  jr $ra\n"
    "  nop\n\n");
}

void Playstation2::add_draw3d_texture_upload()
{
  fprintf(out,
    "  ;; _draw3d_texture_upload()\n"
    "_draw3d_texture_upload:\n");

  // Texture flush
  fprintf(out,
    DMA02_WAIT
    "  li $v1, _texture_flush\n"
    "  sw $v1, 0x10($v0)        ; DMA02 ADDRESS\n"
    "  li $v1, (_texture_flush_end - _texture_flush) / 16\n"
    "  sw $v1, 0x20($v0)         ; DMA02 SIZE\n"
    "  li $v1, 0x101\n"
    "  sw $v1, ($v0)             ; start\n");

  // Clear cache for this object.
  // Find the number of quadwords (16 byte) elements in this object.
  // Since there are 4 elements per 64 byte cache line, figure out how
  // many cache lines need to be flushed by taking (count + 3) / 4.
  fprintf(out,
    "  ; Flush cache\n"
    "  lw $v0, -12($a0)\n"
    "  beqz $v0, _draw3d_texture_upload_skip_cache_flush\n"
    "  nop\n"
    "  sw $0, -12($a0)\n"
    "  lw $v0, -16($a0)\n"
    "  addiu $v0, $v0, 7\n"
    "  move $v1, $a0\n"
    "  srl $v0, $v0, 2\n"
    "_draw3d_texture_upload_cache_flush:\n"
    "  addiu $v0, $v0, -1\n"
    "  sync.l\n"
    "  cache dhwbin, 0($v1)\n"
    "  sync.l\n"
    "  addiu $v1, $v1, 64\n"
    "  bnez $v0, _draw3d_texture_upload_cache_flush\n"
    "  nop\n"
    "_draw3d_texture_upload_skip_cache_flush:\n");

  fprintf(out,
    DMA02_WAIT
    "  sw $a0, 0x10($v0)        ; DMA02 ADDRESS\n"
    "  lw $v1, -16($a0)\n"
    "  sw $v1, 0x20($v0)         ; DMA02 SIZE\n"
    "  li $v1, 0x101\n"
    "  sw $v1, ($v0)             ; start\n"
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
    "_screen_init:\n"
    "  dc64 GIF_TAG(13, 1, 0, 0, FLG_PACKED, 1), REG_A_D\n"
    "  dc64 SETREG_FRAME(0, 10, FMT_PSMCT24, 0), REG_FRAME_1\n"
    "  dc64 SETREG_FRAME(210, 10, FMT_PSMCT24, 0), REG_FRAME_2\n"
    "  dc64 SETREG_ZBUF(420, FMT_PSMZ16S, 0), REG_ZBUF_1\n"
    "  dc64 SETREG_ZBUF(420, FMT_PSMZ16S, 0), REG_ZBUF_2\n"
    "  dc64 SETREG_XYOFFSET(1000 << 4, 1000 << 4), REG_XYOFFSET_1\n"
    "  dc64 SETREG_XYOFFSET(1000 << 4, 1000 << 4), REG_XYOFFSET_2\n"
    "  dc64 SETREG_SCISSOR(0,639,0,447), REG_SCISSOR_1\n"
    "  dc64 SETREG_SCISSOR(0,639,0,447), REG_SCISSOR_2\n"
    "  dc64 1, REG_PRMODECONT\n"
    "  dc64 1, REG_COLCLAMP\n"
    "  dc64 0, REG_DTHE\n"
    "  dc64 SETREG_ALPHA(ALPHA_SRC, ALPHA_FB, ALPHA_SRC, ALPHA_FB, 0x80), REG_ALPHA_1\n"
    "  dc64 SETREG_ALPHA(ALPHA_SRC, ALPHA_FB, ALPHA_SRC, ALPHA_FB, 0x80), REG_ALPHA_2\n"
    "_screen_init_end:\n\n");

  fprintf(out,
    ".align 128\n"
    "_screen_init_clear_1:\n"
    "  dc64 GIF_TAG(6, 1, 0, 0, FLG_PACKED, 1), REG_A_D\n"
    "  dc64 SETREG_TEST(0,ATST_NEVER,0x00,AFAIL_KEEP,0,0,1,ZTST_ALWAYS), REG_TEST_1\n"
    "  dc64 SETREG_PRIM(PRIM_SPRITE, 0, 0, 0, 0, 0, 0, 0, 0), REG_PRIM\n"
    "  dc64 SETREG_RGBAQ(0, 0, 0, 0x80, 1.0), REG_RGBAQ\n"
    "  dc64 SETREG_XYZ2(1000 << 4, 1000 << 4, 0), REG_XYZ2\n"
    "  dc64 SETREG_XYZ2(1640 << 4, 1448 << 4, 0), REG_XYZ2\n"
    "  dc64 SETREG_TEST(0,ATST_ALWAYS,0x00,AFAIL_KEEP,0,0,1,ZTST_GREATER), REG_TEST_1\n"
    "_screen_init_clear_1_end:\n\n");

  fprintf(out,
    ".align 128\n"
    "_screen_init_clear_2:\n"
    "  dc64 GIF_TAG(6, 1, 0, 0, FLG_PACKED, 1), REG_A_D\n"
    "  dc64 SETREG_TEST(0,ATST_NEVER,0x00,AFAIL_KEEP,0,0,1,ZTST_ALWAYS), REG_TEST_2\n"
    "  dc64 SETREG_PRIM(PRIM_SPRITE, 0, 0, 0, 0, 0, 0, 1, 0), REG_PRIM\n"
    "  dc64 SETREG_RGBAQ(0, 0, 0, 0x80, 1.0), REG_RGBAQ\n"
    "  dc64 SETREG_XYZ2(1000 << 4, 1000 << 4, 0), REG_XYZ2\n"
    "  dc64 SETREG_XYZ2(1640 << 4, 1448 << 4, 0), REG_XYZ2\n"
    "  dc64 SETREG_TEST(0,ATST_ALWAYS,0x00,AFAIL_KEEP,0,0,1,ZTST_GREATER), REG_TEST_2\n"
    "_screen_init_clear_2_end:\n\n");

  fprintf(out,
    ".align 128\n"
    "_texture_flush:\n"
    "  dc64 GIF_TAG(1, 1, 0, 0, FLG_PACKED, 1), REG_A_D\n"
    "  dc64 0, REG_TEXFLUSH\n"
    "_texture_flush_end:\n\n");

  //fprintf(out,
  //  "_current_context:\n"
  //  "  dc32 _screen_init_clear_1\n");
}

void Playstation2::add_primitive_gif_tag()
{
  fprintf(out,
    ".align 128\n"
    "_primitive_gif_tag:\n"
    "  dc64 GIF_TAG(1, 0, 0, 0, FLG_PACKED, 1), REG_A_D\n"
    "  dc64 SETREG_PRIM(0, 1, 0, 0, 0, 0, 0, 0, 0), REG_PRIM\n"
    "  dc64 GIF_TAG(0, 1, 0, 0, FLG_PACKED, 2), (REG_A_D|(REG_XYZ2<<4))\n\n");

  fprintf(out,
    ".align 128\n"
    "_primitive_with_texture_gif_tag:\n"
    "  dc64 GIF_TAG(1, 0, 0, 0, FLG_PACKED, 1), REG_A_D\n"
    "  dc64 SETREG_PRIM(0, 0, 1, 0, 0, 0, 0, 0, 0), REG_PRIM\n"
    "  dc64 GIF_TAG(0, 1, 0, 0, FLG_PACKED, 3), (REG_A_D|(REG_A_D<<4)|(REG_XYZ2<<8))\n\n");
}

void Playstation2::add_texture_gif_tag()
{
  // 4M = 0x400_000
  // 640*480*3 = 921600
  // 0x400_000 - 921600 = 0x31f000

  // 640*224*4 = 573,440
  // zbuf = 140 * 20488 = 286,720 ?
  // 640*224*2 = 286,720
  // Context 1 FB = 0
  // Context 1 Z  = 573440
  // Context 2 FB = 1146880
  // Context 2 Z  = 1720320
  // Textures     = 2293760 (currently using 2867200 / 64 / 4)

  fprintf(out,
    ".align 128\n"
    "_texture16_gif_tag:\n"
    "  dc64 GIF_TAG(11, 0, 0, 0, FLG_PACKED, 1), REG_A_D\n"
    "  dc64 SETREG_BITBLTBUF(0, 0, 0, 0x2bc0, 0, FMT_PSMCT16), REG_BITBLTBUF\n"
    "  dc64 SETREG_TRXREG(0, 0), REG_TRXREG\n"
    "  dc64 SETREG_TRXPOS(0, 0, 0, 0, DIR_UL_LR), REG_TRXPOS\n"
    "  dc64 SETREG_TEXA(0x80, 0, 0), REG_TEXA\n"
    "  dc64 SETREG_TRXDIR(XDIR_HOST_TO_LOCAL), REG_TRXDIR\n"
    "  dc64 SETREG_TEX0(0x2bc0, 0, FMT_PSMCT16, 0, 0, 0, TEX_MODULATE, 0, 0, 0, 0, 0), REG_TEX0_1\n"
    "  dc64 SETREG_TEX0(0x2bc0, 0, FMT_PSMCT16, 0, 0, 0, TEX_MODULATE, 0, 0, 0, 0, 0), REG_TEX0_2\n"
    "  dc64 SETREG_TEX1(0, 0, FILTER_NEAREST, 0, 0, 0, 0), REG_TEX1_1\n"
    "  dc64 SETREG_TEX1(0, 0, FILTER_NEAREST, 0, 0, 0, 0), REG_TEX1_2\n"
    "  dc64 SETREG_TEX2(FMT_PSMCT16, 0, 0, 0, 0, 0), REG_TEX2_1\n"
    "  dc64 SETREG_TEX2(FMT_PSMCT16, 0, 0, 0, 0, 0), REG_TEX2_2\n"
    "  dc64 GIF_TAG(0, 1, 0, 0, FLG_IMAGE, 1), REG_A_D\n\n");

  fprintf(out,
    ".align 128\n"
    "_texture24_gif_tag:\n"
    "  dc64 GIF_TAG(11, 0, 0, 0, FLG_PACKED, 1), REG_A_D\n"
    "  dc64 SETREG_BITBLTBUF(0, 0, 0, 0x2bc0, 0, FMT_PSMCT24), REG_BITBLTBUF\n"
    "  dc64 SETREG_TRXREG(0, 0), REG_TRXREG\n"
    "  dc64 SETREG_TRXPOS(0, 0, 0, 0, DIR_UL_LR), REG_TRXPOS\n"
    "  dc64 SETREG_TEXA(0, 0, 0), REG_TEXA\n"
    "  dc64 SETREG_TRXDIR(XDIR_HOST_TO_LOCAL), REG_TRXDIR\n"
    "  dc64 SETREG_TEX0(0x2bc0, 0, FMT_PSMCT24, 0, 0, 0, TEX_MODULATE, 0, 0, 0, 0, 0), REG_TEX0_1\n"
    "  dc64 SETREG_TEX0(0x2bc0, 0, FMT_PSMCT24, 0, 0, 0, TEX_MODULATE, 0, 0, 0, 0, 0), REG_TEX0_2\n"
    "  dc64 SETREG_TEX1(0, 0, FILTER_NEAREST, 0, 0, 0, 0), REG_TEX1_1\n"
    "  dc64 SETREG_TEX1(0, 0, FILTER_NEAREST, 0, 0, 0, 0), REG_TEX1_2\n"
    "  dc64 SETREG_TEX2(FMT_PSMCT24, 0, 0, 0, 0, 0), REG_TEX2_1\n"
    "  dc64 SETREG_TEX2(FMT_PSMCT24, 0, 0, 0, 0, 0), REG_TEX2_2\n"
    "  dc64 GIF_TAG(0, 1, 0, 0, FLG_IMAGE, 1), REG_A_D\n\n");

  fprintf(out,
    ".align 128\n"
    "_texture32_gif_tag:\n"
    "  dc64 GIF_TAG(11, 0, 0, 0, FLG_PACKED, 1), REG_A_D\n"
    "  dc64 SETREG_BITBLTBUF(0, 0, 0, 0x2bc0, 0, FMT_PSMCT32), REG_BITBLTBUF\n"
    "  dc64 SETREG_TRXREG(0, 0), REG_TRXREG\n"
    "  dc64 SETREG_TRXPOS(0, 0, 0, 0, DIR_UL_LR), REG_TRXPOS\n"
    "  dc64 SETREG_TEXA(0, 0, 0), REG_TEXA\n"
    "  dc64 SETREG_TRXDIR(XDIR_HOST_TO_LOCAL), REG_TRXDIR\n"
    "  dc64 SETREG_TEX0(0x2bc0, 0, FMT_PSMCT32, 0, 0, 0, TEX_MODULATE, 0, 0, 0, 0, 0), REG_TEX0_1\n"
    "  dc64 SETREG_TEX0(0x2bc0, 0, FMT_PSMCT32, 0, 0, 0, TEX_MODULATE, 0, 0, 0, 0, 0), REG_TEX0_2\n"
    "  dc64 SETREG_TEX1(0, 0, FILTER_NEAREST, 0, 0, 0, 0), REG_TEX1_1\n"
    "  dc64 SETREG_TEX1(0, 0, FILTER_NEAREST, 0, 0, 0, 0), REG_TEX1_2\n"
    "  dc64 SETREG_TEX2(FMT_PSMCT32, 0, 0, 0, 0, 0), REG_TEX2_1\n"
    "  dc64 SETREG_TEX2(FMT_PSMCT32, 0, 0, 0, 0, 0), REG_TEX2_2\n"
    "  dc64 GIF_TAG(0, 1, 0, 0, FLG_IMAGE, 1), REG_A_D\n\n");
}

void Playstation2::add_strings()
{
  fprintf(out,
    "_audsrv_irx:\n"
    "  db \"host:audsrv.irx\", 0\n\n");
    //"  db \"host0:audsrv.irx\", 0\n\n");
}

void Playstation2::add_draw3d_texture16_setPixelsRLE16_IaB()
{
  // a0 = object
  // a1 = index
  // a2 = array

  fprintf(out,
    "_draw3d_texture16_setPixelsRLE16_IaB:\n"
    "  ;; draw3d_texture16_setPixelRLE16_IaB()\n"
    "  li $at, 1\n"
    "  sw $at, -12($a0)\n"
    "  sll $a1, $a1, 1\n"
    "  addiu $a0, $a0, 208\n"
    "  addu $a0, $a0, $a1\n"
    "  lw $t9, -4($a2)\n"
    "_draw3d_textureRLE16_setPixels_0:\n"
    "  lbu $t8, 0($a2)\n"
    "  lbu $v0, 1($a2)\n"
    "  lbu $v1, 2($a2)\n"
    "  sll $v1, $v1, 8\n"
    "  or $v0, $v0, $v1\n"
    "_draw3d_textureRLE16_setPixels_loop_0:\n"
    "  sh $v0, 0($a0)\n"
    "  addiu $a0, $a0, 2\n"
    "  addiu $t8, $t8, -1\n"
    "  bne $t8, $0, _draw3d_textureRLE16_setPixels_loop_0\n"
    "  nop\n"
    "  addiu $t9, $t9, -3\n"
    "  addiu $a2, $a2, 3\n"
    "  bgtz $t9, _draw3d_textureRLE16_setPixels_0\n"
    "  nop\n"
    "  jr $ra\n"
    "  nop\n\n");
}

void Playstation2::add_draw3d_object_constructor()
{
  // Need to allocate enough space for:
  // -16: size of packet (in 16 byte blocks)
  // -12: cache flush object flag
  //   0: VIF_FLUSH, VIF_STMOD, VIF_STCYCL, VIF_UNPACK_V4_32
  //  16: sin512(rx), cos512(rx), sin512(ry), sin512(ry)
  //  32: sin512(rz), cos(rz), 0, 0
  //  48: x, y, z, 0
  //  64: count, do_rot_xyz, vertex_size, 0
  //  80: 16 byte GIF tag
  //  96: 16 byte primitive info
  // 112: 16 byte GIF tag
  // 128: 16 bytes per point (for color)   REG_A_D
  //    : 16 bytes per point (for x, y, z) REG_XYZ2
  //    : VIF_MSCAL, VIF_NOP, VIF_NOP, VIF_NOP
  fprintf(out,
    "_draw3d_object_constructor:\n"
    "  ;; _draw3d_object_constructor(type, point_count)\n"
    "  ;; Align stack pointer, alloca() some memory\n"
    "  ;; Clear rotation and offset and set point count\n"
    "  addiu $at, $0, -16\n"
    "  and $sp, $sp, $at\n");

  // Allocated memory is:
  // vertex_count * 32 + size_of_header + 32 + 16
  fprintf(out,
    "  sll $at, $a1, 5\n"
    "  addiu $at, $at, 160\n"
    "  subu $sp, $sp, $at\n");

  // Set top of reg stack (return value) to allocated memory.
  // 16 bytes before the allocated memory is the size of the
  // DMA transfer.
  fprintf(out, "  addiu $v0, $sp, 16\n");

  // Setup starting part VIF packet
  fprintf(out,
    "  li $t8, (VIF_FLUSH << 24)\n"
    "  sw $t8, 0($v0)\n"
    "  li $t8, (VIF_STMOD << 24)\n"
    "  sw $t8, 4($v0)\n"
    "  li $t8, (VIF_STCYCL << 24)|(1 << 8)|1\n"
    "  sw $t8, 8($v0)\n"
    "  li $t8, (VIF_UNPACK_V4_32 << 24)\n"
    "  addiu $t9, $at, -48\n"
    "  sll $t9, $t9, 12\n"
    "  or $t8, $t8, $t9\n"
    "  sw $t8, 12($v0)\n");

  // Setup ending part VIF packet
  fprintf(out,
    "  li $t8, (VIF_MSCAL << 24)\n"
    "  addu $t9, $sp, $at\n"
    "  sw $t8, -16($t9)\n"
    "  sw $0, -12($t9)\n"
    "  sw $0, -8($t9)\n"
    "  sw $0, -4($t9)\n");

  // Store the size of the packet (size / 16).
  fprintf(out,
    "  sll $at, $a1, 1\n"
    "  addiu $at, $at, 9\n"
    "  sw $at, -16($v0)\n");

  // Put 1 (true) into cache-flush flag.
  fprintf(out,
    "  li $at, 1\n"
    "  sw $at, -12($v0)\n");

  // Clear out members of the structure (rotation, xyz position, etc).
  fprintf(out,
    "  sq $0, 16($v0)\n"
    "  sq $0, 32($v0)\n"
    "  sq $0, 48($v0)\n"
    "  sq $0, 64($v0)\n");

  // Save point count in the struct
  fprintf(out, "  sw $a1, 64($v0)\n");

  // Copy the default primitive tag to the new structure.
  fprintf(out,
    "  li $t8, _primitive_gif_tag\n"
    "  lq $t9, 0($t8)\n"
    "  sq $t9, 80($v0)\n"
    "  lq $t9, 16($t8)\n"
    "  or $t9, $t9, $a0\n"
    "  sq $t9, 96($v0)\n"
    "  lq $t9, 32($t8)\n"
    "  or $t9, $t9, $a1\n"
    "  sq $t9, 112($v0)\n");

  // Set vertex size (2 for no texture and 3 for texture)
  fprintf(out,
    "  li $t9, 2\n"
    "  sw $t9, 72($v0)\n");

  // a1 = count
  //fprintf(out, "  move $a1, $t%d\n", point_count);

  // Set points and colors
  fprintf(out,
    "  addiu $t8, $v0, 128\n"
    "  lui $t9, 0x3f80\n"
    "  dsll32 $t9, $t9, 0\n"
    "  li $a2, REG_RGBAQ\n"
    "_draw3d_object_constructor_l0:\n"
    "  sd $t9, 0($t8)\n"
    "  sd $a2, 8($t8)\n"
    "  sq $0, 16($t8)\n"
    "  addiu $t8, $t8, 32\n"
    "  addiu $a1, $a1, -1\n"
    "  bne $a1, $0, _draw3d_object_constructor_l0\n"
    "  nop\n");

  fprintf(out,
    "  jr $ra\n"
    "  nop\n\n");
}

void Playstation2::add_draw3d_object_with_texture_constructor()
{
  // Need to allocate enough space for:
  // -16: size of packet (in 16 byte blocks)
  // -12: cache flush object flag
  //   0: VIF_FLUSH, VIF_STMOD, VIF_STCYCL, VIF_UNPACK_V4_32
  //  16: sin512(rx), cos512(rx), sin512(ry), sin512(ry)
  //  32: sin512(rz), cos(rz), 0, 0
  //  48: x, y, z, 0
  //  64: count, do_rot_xyz, vertex_size, 0
  //  80: 16 byte GIF tag
  //  96: 16 byte primitive info
  // 112: 16 byte GIF tag
  // 128: 16 bytes per point (for color)   REG_A_D
  //    : 16 bytes per point (for st)      REG_ST    [texture coords]
  //    : 16 bytes per point (for x, y, z) REG_XYZ2
  //    : VIF_MSCAL, VIF_NOP, VIF_NOP, VIF_NOP
  fprintf(out,
    "_draw3d_object_with_texture_constructor:\n"
    "  ;; _draw3d_object_with_texture_constructor(type, point_count)\n"
    "  ;; Align stack pointer, alloca() some memory\n"
    "  ;; Clear rotation and offset and set point count\n"
    "  addiu $at, $0, -16\n"
    "  and $sp, $sp, $at\n");

  // Allocated memory is:
  // vertex_count * 48 + size_of_header + 32 + 16
  fprintf(out,
    "  sll $at, $a1, 5\n"
    "  sll $t9, $a1, 4\n"
    "  addu $at, $at, $t9\n"
    "  addiu $at, $at, 160\n"
    "  subu $sp, $sp, $at\n");

  // Set top of reg stack (return value) to allocated memory.
  fprintf(out, "  addiu $v0, $sp, 16\n");

  // Setup starting part VIF packet
  fprintf(out,
    "  li $t8, (VIF_FLUSH << 24)\n"
    "  sw $t8, 0($v0)\n"
    "  li $t8, (VIF_STMOD << 24)\n"
    "  sw $t8, 4($v0)\n"
    "  li $t8, (VIF_STCYCL << 24)|(1 << 8)|1\n"
    "  sw $t8, 8($v0)\n"
    "  li $t8, (VIF_UNPACK_V4_32 << 24)\n"
    "  addiu $t9, $at, -48\n"
    "  sll $t9, $t9, 12\n"
    "  or $t8, $t8, $t9\n"
    "  sw $t8, 12($v0)\n");

  // Setup ending part VIF packet
  fprintf(out,
    "  li $t8, (VIF_MSCAL << 24)\n"
    "  addu $t9, $sp, $at\n"
    "  sw $t8, -16($t9)\n"
    "  sw $0, -12($t9)\n"
    "  sw $0, -8($t9)\n"
    "  sw $0, -4($t9)\n");

  // Store the size of the packet (size / 16).
  fprintf(out,
    "  sll $at, $a1, 1\n"
    "  addu $at, $at, $a1\n"
    "  addiu $at, $at, 9\n"
    "  sw $at, -16($v0)\n");

  // Put 1 (true) into cache-flush flag.
  fprintf(out,
    "  li $at, 1\n"
    "  sw $at, -12($v0)\n");

  // Clear out members of the structure (rotation, xyz position, etc).
  fprintf(out,
    "  sq $0, 16($v0)\n"
    "  sq $0, 32($v0)\n"
    "  sq $0, 48($v0)\n"
    "  sq $0, 64($v0)\n");

  // Save point count in the struct
  fprintf(out, "  sw $a1, 64($v0)\n");

  // Copy the default primitive tag to the new structure.
  fprintf(out,
    "  li $t8, _primitive_with_texture_gif_tag\n"
    "  lq $t9, 0($t8)\n"
    "  sq $t9, 80($v0)\n"
    "  lq $t9, 16($t8)\n"
    "  or $t9, $t9, $a0\n"
    "  sq $t9, 96($v0)\n"
    "  lq $t9, 32($t8)\n"
    "  or $t9, $t9, $a1\n"
    "  sq $t9, 112($v0)\n");

  // Set vertex size (2 for no texture and 3 for texture)
  fprintf(out,
    "  li $t9, 3\n"
    "  sw $t9, 72($v0)\n");

  // a1 = count
  //fprintf(out, "  move $a1, $t%d\n", point_count);

  // Set points and colors
  fprintf(out,
    "  addiu $t8, $v0, 128\n"
    "  lui $t9, 0x3f80\n"
    "  dsll32 $t9, $t9, 0\n"
    "  li $a2, REG_RGBAQ\n"
    "  li $a3, REG_ST\n"
    "_draw3d_object_texture_constructor_l0:\n"
    "  sd $t9,  0($t8)\n"
    "  sd $a2,  8($t8)\n"
    "  sd $0,  16($t8)\n"
    "  sd $a3, 24($t8)\n"
    "  sq $0,  32($t8)\n"
    "  addiu $t8, $t8, 48\n"
    "  addiu $a1, $a1, -1\n"
    "  bne $a1, $0, _draw3d_object_texture_constructor_l0\n"
    "  nop\n");

  fprintf(out,
    "  jr $ra\n"
    "  nop\n\n");
}

void Playstation2::add_draw3d_texture_constructor(int bit_size)
{
  int shift_size = 1;

  // Need to allocate enough space for:
  // -16: size of data
  // -12: cache full texture flag
  //   0: 16 byte GIF tag (for texture info)
  //  16: BITBLTBUF
  //  32: TRXREG
  //  48: TRXPOS
  //  64: TEXA
  //  80: TRXDIR
  //  96: TEX0_1
  // 112: TEX0_2
  // 128: TEX1_1
  // 144: TEX1_2
  // 160: TEX2_1
  // 176: TEX2_2
  // 192: 16 byte GIF tag (for actual image)
  // 208: (width * height * 2) bytes of image
  fprintf(out,
    "_draw3d_texture%d_constructor:\n"
    "  ;; _draw3d_texture%d_constructor(type, point_count)\n"
    "  ;; Align stack pointer, alloca() some memory\n"
    "  ;; Clear rotation and offset and set point count\n"
    "  addiu $at, $0, -16\n"
    "  and $sp, $sp, $at\n",
    bit_size, bit_size);

  // If bit_size is 16, then bytes = (width * height * 2)
  // If bit_size is 24, then bytes = (width * height * 2) + (width * height)
  // If bit_size is 32, then bytes = (width * height * 4)
  if (bit_size == 32) { shift_size = 2; }

  // Allocated memory is (width * height * [2/3/4]) + 208 + 16.
  fprintf(out,
    "  multu $a0, $a1\n"
    "  mflo $t9\n"
    "  sll $a3, $t9, %d\n",
    shift_size);

  if (bit_size == 24)
  {
    fprintf(out, "  addu $a3, $a3, $t9\n");
  }

  fprintf(out,
    "  addiu $sp, $sp, -224\n"
    "  subu $sp, $sp, $a3\n"
    "  and $sp, $sp, $at\n");

  // Set top of reg stack (return value) to allocated memory.
  // 16 bytes before the allocated memory is the size of the
  // DMA transfer.
  fprintf(out, "  addiu $v0, $sp, 16\n");

  // Copy default GIF packet to new memory.
  fprintf(out,
    "  li $t8, _texture%d_gif_tag\n"
    "  ori $t9, $0, 13\n"
    "  move $v1, $v0\n"
    "_draw3d_texture%d_constructor_l0:\n"
    "  lq $at, 0($t8)\n"
    "  sq $at, 0($v1)\n"
    "  addiu $t9, $t9, -1\n"
    "  addiu $t8, $t8, 16\n"
    "  addiu $v1, $v1, 16\n"
    "  bne $t9, $0, _draw3d_texture%d_constructor_l0\n"
    "  nop\n",
    bit_size, bit_size, bit_size);

  // Update TRXREG in GIF packet with width, height
  fprintf(out,
    "  sw $a0, 32($v0)\n"
    "  sw $a1, 36($v0)\n");

  // Update BITBLTBUF in GIF packet with width / 64 field.
  fprintf(out,
    "  srl $at, $a0, 6\n"
    "  sb $at, 22($v0)\n");

  // Update TEX0_1/2 in GIF packet for log2(width) / log2(height).
  fprintf(out,
    "  li $t9, 30\n"
    "  ld $t8, 96($v0)\n"
    "  dsll $at, $at, 14\n"
    "  or $t8, $t8, $at\n"
    "  plzcw $at, $a0\n"
    "  andi $at, $at, 0xff\n"
    "  subu $at, $t9, $at\n"
    "  dsll $at, $at, 26\n"
    "  or $t8, $t8, $at\n"
    "  plzcw $at, $a1\n"
    "  andi $at, $at, 0xff\n"
    "  subu $at, $t9, $at\n"
    "  dsll $at, $at, 30\n"
    "  or $t8, $t8, $at\n"
    "  sd $t8, 96($v0)\n"
    "  sd $t8, 112($v0)\n");

  // Update image GIF packet quadword count.
  fprintf(out,
    "  addiu $a3, $a3, 15\n"
    "  addiu $at, $0, -16\n"
    "  and $a3, $a3, $at\n"
    "  srl $a3, $a3, 4\n"
    "  ori $at, $a3, 0x8000\n"
    "  sh $at, 192($v0)\n");

  // Save the size of the GIF packets to the memory-chunk -16
  fprintf(out,
    "  addiu $a3, $a3, 13\n"
    "  sw $a3, -16($v0)\n");

  // Put 1 (true) into cache-flush flag.
  fprintf(out,
    "  li $at, 1\n"
    "  sw $at, -12($v0)\n");

  fprintf(out,
    "  jr $ra\n"
    "  nop\n\n");
}

void Playstation2::add_spu_functions()
{
  fprintf(out,
    "  ;; spu_init()\n"
    "_spu_init:\n"
    KERNEL_ENTER
    "  li $v1, 0xbf90_0000\n"
    "  ;; SPU2 attributes Core 0/1 ATTR\n"
#ifdef USE_SPU2_DMA
    "  li $v0, 0xc022\n"
    "  sh $v0, 0x019a($v1)\n"
    "  sh $v0, 0x059a($v1)\n"
#else
    "  li $v0, 0xc012\n"
    "  sh $v0, 0x019a($v1)\n"
    "  sh $v0, 0x059a($v1)\n"
#endif
    //"  ; SPU Voice 0 Start Address Core 0, Voice 0 - SSA\n"
    //"  li $v0, 0x2800\n"
    //"  sh $v0, 0x1c2($v1)\n"
    //"  sh $zero, 0x1c0($v1)\n"
    "  ; SPU Volume Control MVOLL,MVOLR Core 1\n"
    "  li $v0, 0x3fff\n"
    "  sh $v0, 0x788($v1)\n"
    "  sh $v0, 0x78a($v1)\n"

    // Shouldn't be needed since "sound data volume" isn't used.
    "  ; SPU Volume Control BVOLL,BVOLR Core 0\n"
    "  li $v0, 0x7fff\n"
    "  sh $v0, 0x76c($v1)\n"
    "  sh $v0, 0x76e($v1)\n"
    "  sh $v0, 0x794($v1)\n"
    "  sh $v0, 0x796($v1)\n"

    "  ; SPU Volume Control AVOLL,AVOLR Core 0/1\n"
    "  li $v0, 0x7fff\n"
    "  sh $v0, 0x768($v1)\n"
    "  sh $v0, 0x76a($v1)\n"
    "  sh $v0, 0x790($v1)\n"
    "  sh $v0, 0x792($v1)\n"
    "  ; SPU Mixer Control MMIX Core 0/1\n"
    "  li $v0, 0xc00\n"
    "  sh $v0, 0x198($v1)\n"
    "  li $v0, 0x00c\n"
    "  sh $v0, 0x598($v1)\n"
    "  ; SPU ADSR1 06 Core 0 - Voice 0\n"
    //"  li $v0, 0x00ff\n"
    "  li $v0, 0x0000\n"
    "  sh $v0, 0x0006($v1)\n"
    "  ; SPU ADSR2 08 Core 0 - Voice 0\n"
    //"  li $v0, 0x1fc0\n"
    "  li $v0, 0x0000\n"
    "  sh $v0, 0x0008($v1)\n"

    "  ; SPU Mixer Control VMIXL,VMIXR Core 0/1\n"
    "  li $v0, 0x1\n"
    "  sh $v0, 0x188($v1)\n"
    "  sh $zero, 0x18a($v1)\n"
    "  sh $v0, 0x190($v1)\n"
    "  sh $zero, 0x192($v1)\n"
    "  sh $v0, 0x588($v1)\n"
    "  sh $zero, 0x58a($v1)\n"
    "  sh $v0, 0x590($v1)\n"
    "  sh $zero, 0x592($v1)\n"

    "  ; SPU Mixer Control VMIXEL,VMIXER Core 0/1 (set to 0)\n"
    "  sh $zero, 0x18c($v1)\n"
    "  sh $zero, 0x18e($v1)\n"
    "  sh $zero, 0x194($v1)\n"
    "  sh $zero, 0x196($v1)\n"
    "  sh $zero, 0x58c($v1)\n"
    "  sh $zero, 0x58e($v1)\n"
    "  sh $zero, 0x594($v1)\n"
    "  sh $zero, 0x596($v1)\n"

    "  ; SPU PMON set to 0.\n"
    "  sh $zero, 0x0180($v1)\n"
    "  sh $zero, 0x0182($v1)\n"

    "  ; SPU NON set to 0.\n"
    "  sh $zero, 0x0184($v1)\n"
    "  sh $zero, 0x0186($v1)\n"

    "  ; SPU IRQA should be set to 0.\n"
    "  sh $zero, 0x019c($v1)\n"
    "  sh $zero, 0x019e($v1)\n"

    "  ;; Work address\n"
    "  li $at, 0x003f\n"
    "  sh $at, 0x033c($v1)\n"
    "  li $at, 0xffff\n"
    "  sh $at, 0x02e0($v1)\n"
    "  sh $at, 0x02e2($v1)\n"

    "  ; SPU2 KEYOFF to all 24 voices\n"
    "  li $v0, 0xffff\n"
    "  sh $v0, 0x01a4($v1)\n"
    "  sh $v0, 0x01a6($v1)\n"
    "  ; DMA Primary Control Register (0x1f80_10f0)\n"
    "  ; Enable DMA 4 (SPU2)\n"
    "  li $v1, 0xbf80_10f0\n"
    "  lw $v0, 0($v1)\n"
    "  li $at, 0x0008_0000\n"
    "  or $v0, $v0, $at\n"
    "  sw $v0, 0($v1)\n"
    KERNEL_EXIT
    "  jr $ra\n"
    "  nop\n\n");

#ifdef USE_SPU2_DMA
  fprintf(out,
    "  ;; upload_sound_data(byte[] data)\n"
    "_upload_sound_data:\n"
    KERNEL_ENTER
    "  ; SPU Voice 0 Transfer Address Core 0\n"
    "  li $v1, 0xbf90_0000\n"
    "  li $at, 0\n"
    "  sh $at, 0x1aa($v1)\n"
    "  sh $zero, 0x1a8($v1)\n"
    "  ; Upload data to IOP\n"
    "  li $v1, 0xbc01_0000 + 0x10000\n"
    "  lw $a1, -4($a0)\n"
    "  srl $at, $a1, 2\n"
    "_upload_sound_data_loop:\n"
    "  lw $v0, ($a0)\n"
    "  sw $v0, ($v1)\n"
    "  addiu $a0, $a0, 4\n"
    "  addiu $v1, $v1, 4\n"
    "  addiu $at, $at, -1\n"
    "  bnez $at, _upload_sound_data_loop\n"
    "  nop\n");

  fprintf(out,
    "  ;; Upload from IOP to SPU2 with DMA\n"
    "  ;; DMA 4 D_MADR (0x1f80_10c0) source address\n"
    "  ;; DMA 4 D_BCRL (0x1f80_10c4) block control\n"
    "  ;; DMA 4 D_CHCR (0x1f80_10c8) channel control\n"
    //"  li $v1, 0x1f80_10c0\n"
    "  li $v1, 0xbf80_10c0\n"
    "  li $at, 0x2_0000\n"
    "  sw $at, 0($v1)\n"
    "  ;; Block Size 16, Words * Blocks = $a1 / 16 ($a1 bytes total)\n"
    "  ;; DMA send sound data.\n"
    "  sll $a1, $a1, 12\n"
    "  ori $a1, $a1, 4\n"
    "  sw $a1, 4($v1)\n"
    "  li $at, 0x0100_0201\n"
    "  sw $at, 8($v1)\n"
    "  li $at, 0x0100_0000\n"
    "_wait_spu_dma:\n"
    "  lw $v0, 8($v1)\n"
    "  and $v0, $v0, $at\n"
    "  bnez $v0, _wait_spu_dma\n"
    "  nop\n"
    KERNEL_EXIT
    "  jr $ra\n"
    "  nop\n");

#else

  fprintf(out,
    "  ;; upload_sound_data(byte[] data)\n"
    "_upload_sound_data:\n"
    KERNEL_ENTER
    "  li $v1, 0xbf90_0000\n"
    "  ; Need to set transfer mode to 0 in order to set TSA/TSA\n"
    "  li $t9, 0xc002\n"
    "  sh $t9, 0x019a($v1)\n"
    "  ; SPU Voice 0 Transfer Address Core 0 - TSA\n"
    "  li $at, 0x2800\n"
    "  sh $zero, 0x1a8($v1)\n"
    "  sh $at, 0x1aa($v1)\n"
    "  ; SPU Voice 0 Start Address Core 0, Voice 0 - SSA\n"
    "  sh $zero, 0x1c0($v1)\n"
    "  sh $at, 0x1c2($v1)\n"
    "  ; Upload data to IOP\n"
    "  lw $a1, -4($a0)\n"
    "  srl $at, $a1, 1\n"
    "_upload_sound_data_loop:\n"
    "  ; Load the FIFO with thirty-two 16 bit sound samples\n"
    "  li $a3, 32\n"
    "_upload_sound_block_loop:\n"
    "  lh $v0, ($a0)\n"
    "  sh $v0, 0x01ac($v1)\n"
    "  addiu $a0, $a0, 2\n"
    "  addiu $a3, $a3, -1\n"
    "  bnez $a3, _upload_sound_block_loop\n"
    "  nop\n"
    "  addiu $at, $at, -32\n"
    "  ; Trigger the FIFO to flush (setting the transfer bits to 1\n"
    "  li $t9, 0xc012\n"
    "  sh $t9, 0x019a($v1)\n"
    "  ; Wait until the SPU2 FIFO transfer status bit is 0\n"
    "_upload_sound_busy:\n"
    "  lh $t9, 0x0344($v1)\n"
    "  andi $t9, $t9, 0x0400\n"
    "  bnez $t9, _upload_sound_busy\n"
    "  nop\n"
    "  bgtz $at, _upload_sound_data_loop\n"
    "  nop\n"

    KERNEL_EXIT
    "  jr $ra\n"
    "  nop\n");

#endif
}

