/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2022 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "generator/Nintendo64.h"

#define N643D "net/mikekohn/java_grinder/n64/"
#define N643D_LEN (sizeof(N643D) - 1)

// Screen 0: 0x10_0000 (0x10_0000)
// Screen 1: 0x12_5800 (0x10_0000 + (320 * 240 * 2))
// Z Buffer: 0x14_b000 (0x10_0000 + (320 * 240 * 4))
// $k1 points to DMEM
// $k0 points to struct of:
//  0: uint32_t current_screen (with KSEG1)
//  4: uint32_t current_screen (for DMEM)
//  8: uint8_t  screen_id
//     uint8_t  reserved[3]
// 12: uint32_t reserved
// 16: uint16_t texture[]

Nintendo64::Nintendo64()
{
  org = 0x80000000;
  ram_start = 0x80000000;
  ram_end = 0x80200000;
}

Nintendo64::~Nintendo64()
{
}

int Nintendo64::open(const char *filename)
{
  if (R4000::open(filename) != 0) { return -1; }

  fprintf(out,
    ".include \"nintendo64/rdp.inc\"\n"
    ".include \"nintendo64/rsp.inc\"\n"
    ".include \"nintendo64/system.inc\"\n"
    ".include \"nintendo64/video_interface.inc\"\n\n");

  fprintf(out,
    ".high_address 0x8010_ffff\n"
    ".org 0x8000_0000\n\n");

  return 0;
}

int Nintendo64::finish()
{
  insert_ntsc_setup();
  insert_rsp_data();
  insert_run_rdp_screen_setup();
  insert_clear_screen();
  insert_reset_z_buffer();
  insert_set_screen();
  insert_triangle_constructor();
  insert_rectangle_constructor();
  insert_rectangle_draw();
  insert_triangle_draw();
  insert_triangle_vertex_copy();
  insert_load_texture();
  insert_cos_table();
  insert_rsp_code();
  R4000::finish();

  return 0;
}

int Nintendo64::start_init()
{
  catridge_header();

  fprintf(out,
    "bootcode:\n"
    "  ;; This was downloaded from Peter Lemon's git repo. It seems like it's\n"
    "  ;; needed to initialize the hardware.\n"
    ".binfile \"bootcode.bin\"\n\n"
    "start:\n");

  init_system();

  fprintf(out,
    "  ;; $k0 points to struct of\n"
    "  ;;  0: uint32_t current_screen (with KSEG1)\n"
    "  ;;  4: uint32_t current_screen (for DMEM)\n"
    "  ;;  8: uint8_t  screen_id\n"
    "  ;;     uint8_t  reserved[3]\n"
    "  ;; 12: uint32_t reserved\n"
    "  ;; 16: uint16_t texture[]\n"
    "  li $k0, 320 * 240 * 2 * 2 * 2\n"
    "  li $t0, KSEG1 | 0x0010_0000\n"
    "  li $t2, 0x0010_0000\n"
    "  addu $k0, $k0, $t0\n"
    "  li $t1, 1\n"
    "  sw $t0, 0($k0)\n"
    "  sw $t2, 4($k0)\n"
    "  sb $t1, 8($k0)\n\n");

  fprintf(out,
    "  ;; $k1 points to DMEM\n"
    "  li $k1, KSEG1 | RSP_DMEM\n\n");

  rsp_halt();
  rsp_copy_code();
  rsp_copy_cos_table();
  rdp_copy_instructions();
  rsp_start();

  fprintf(out,
    "  jal _run_rdp_screen_setup\n"
    "  nop\n\n");

  return 0;
}

int Nintendo64::new_object(std::string &object_name, int field_count)
{
  fprintf(out, "  ;; new_object(%s, field_count=%d)\n",
    object_name.c_str(), field_count);

  if (strncmp(object_name.c_str(), N643D, N643D_LEN) != 0)
  {
     printf("Error: Unsupported class %s\n", object_name.c_str());
     return -1;
  }

  const char *s = object_name.c_str();

  while (*s != 0) { s++; }
  while (s != object_name.c_str() && *s != '/') { s--; }
  if (*s == '/') { s++; }

  if (strcmp(s, "Triangle") != 0 &&
      strcmp(s, "Rectangle") != 0)
  {
     printf("Error: Unknown class %s\n", object_name.c_str());
     return -1;
  }

  reg++;

  return 0;
}

int Nintendo64::nintendo64_waitVsync()
{
  fprintf(out,
    "  ;; nintendo64_waitVsync()\n"
    "  li $a0, KSEG1 | VI_BASE\n"
    "  li $t0, 512\n"
    "wait_for_vblank_loop_%d:\n"
    "  lw $t1, VI_V_CURRENT_LINE_REG($a0)\n"
    "  bne $t0, $t1, wait_for_vblank_loop_%d\n"
    "  nop\n",
    label_count,
    label_count);

  label_count++;

  return 0;
}

int Nintendo64::nintendo64_clearScreen()
{
  fprintf(out,
    "  ;; nintendo64_clearScreen()\n"
    "  move $s0, $ra\n"
    "  jal _clear_screen\n"
    "  nop\n"
    "  move $ra, $s0\n");

  return 0;
}

int Nintendo64::nintendo64_resetZBuffer()
{
  fprintf(out,
    "  ;; nintendo64_resetZBuffer()\n"
    "  move $s0, $ra\n"
    "  jal _reset_z_buffer\n"
    "  nop\n"
    "  move $ra, $s0\n");

  return 0;
}

int Nintendo64::nintendo64_setScreen_I()
{
  // setScreen(0): Display screen 0, draw on screen 1.
  // setScreen(1): Display screen 1, draw on screen 0.
  fprintf(out,
    "  ;; nintendo64_setScreen_I()\n"
    "  move $s0, $ra\n"
    "  move $a0, $t%d\n"
    "  jal _set_screen\n"
    "  nop\n"
    "  move $ra, $s0\n",
    reg - 1);

  reg -= 1;

  return 0;
}

int Nintendo64::nintendo64_plot_III()
{
  // x = x * 2;
  // y = y * 320 * 2;
  fprintf(out,
    "  ;; nintendo64_plot_III()\n"
    "  li $at, 640\n"
    "  sll $t%d, $t%d, 1\n"
    "  multu $t%d, $at\n"
    "  mflo $t%d\n"
    "  addu $t%d, $t%d, $t%d\n"
    "  lw $a0, ($k0)\n"
    "  addu $a0, $a0, $t%d\n"
    "  sh $t%d, ($a0)\n",
    reg - 3, reg - 3,
    reg - 2,
    reg - 2,
    reg - 3, reg - 3, reg - 2,
    reg - 3,
    reg - 1);

  reg -= 3;

  return 0;
}

int Nintendo64::nintendo64_loadTexture_aSII()
{
  const int array = reg - 3;
  const int width = reg - 2;
  const int height = reg - 1;

  fprintf(out,
    "  ;; nintendo64_loadTexture_aSII()\n"
    "  move $a0, $t%d\n"
    "  move $a1, $t%d\n"
    "  move $a2, $t%d\n"
    "  jal _load_texture\n"
    "  nop\n",
    array,
    width,
    height);

  reg -= 3;

  return 0;
}

#if 0
int Nintendo64::nintendo64_waitForPolygon()
{
  fprintf(out,
    "  ;; nintendo64_waitForPolygon()\n"
    "_wait_for_polygon_%d:\n"
    "  lw $at, 0($k1)\n"
    "  bne $at, $0, _wait_for_polygon_%d\n"
    "  nop\n",
    label_count,
    label_count);

  label_count++;

  return 0;
}
#endif

int Nintendo64::nintendo64_n64_triangle_Constructor()
{

  fprintf(out,
    "  ;; nintendo64_n64_triangle_Constructor()\n"
    "  move $s0, $ra\n"
    "  jal _triangle_constructor\n"
    "  nop\n"
    "  move $t%d, $v0\n"
    "  move $ra, $s0\n",
    reg - 2);

  reg -= 1;

  return 0;
}

int Nintendo64::nintendo64_n64_triangle_setColor_I()
{
  const int object = reg - 2;
  const int color = reg - 1;

  fprintf(out,
    "  ;; nintendo64_n64_triangle_setColor_I()\n"
    "  sw $t%d, 40($t%d)\n",
    color, object);

  reg -= 2;

  return 0;
}

int Nintendo64::nintendo64_n64_triangle_setPosition_III()
{
  const int object = reg - 4;
  const int x = reg - 3;
  const int y = reg - 2;
  const int z = reg - 1;

  fprintf(out,
    "  ;; nintendo64_n64_triangle_setPosition_III()\n"
    "  sh $t%d, 32($t%d)\n"
    "  sh $t%d, 34($t%d)\n"
    "  sh $t%d, 36($t%d)\n",
    x, object,
    y, object,
    z, object);

  reg -= 4;

  return 0;
}

int Nintendo64::nintendo64_n64_triangle_setRotation_III()
{
  const int object = reg - 4;
  const int rx = reg - 3;
  const int ry = reg - 2;
  const int rz = reg - 1;

  fprintf(out,
    "  ;; nintendo64_n64_triangle_setRotation_III()\n"
    "  sh $t%d, 24($t%d)\n"
    "  sh $t%d, 26($t%d)\n"
    "  sh $t%d, 28($t%d)\n",
    rx, object,
    ry, object,
    rz, object);

  reg -= 4;

  return 0;
}

int Nintendo64::nintendo64_n64_triangle_setVertex0_III()
{
  const int object = reg - 4;
  const int x = reg - 3;
  const int y = reg - 2;
  const int z = reg - 1;

  fprintf(out,
    "  ;; nintendo64_n64_triangle_setVertex0_III()\n"
    "  sh $t%d, 0($t%d)\n"
    "  sh $t%d, 2($t%d)\n"
    "  sh $t%d, 4($t%d)\n",
    x, object,
    y, object,
    z, object);

  reg -= 4;

  return 0;
}

int Nintendo64::nintendo64_n64_triangle_setVertex1_III()
{
  const int object = reg - 4;
  const int x = reg - 3;
  const int y = reg - 2;
  const int z = reg - 1;

  fprintf(out,
    "  ;; nintendo64_n64_triangle_setVertex1_III()\n"
    "  sh $t%d,  8($t%d)\n"
    "  sh $t%d, 10($t%d)\n"
    "  sh $t%d, 12($t%d)\n",
    x, object,
    y, object,
    z, object);

  reg -= 4;

  return 0;
}

int Nintendo64::nintendo64_n64_triangle_setVertex2_III()
{
  const int object = reg - 4;
  const int x = reg - 3;
  const int y = reg - 2;
  const int z = reg - 1;

  fprintf(out,
    "  ;; nintendo64_n64_triangle_setVertex2_III()\n"
    "  sh $t%d, 16($t%d)\n"
    "  sh $t%d, 18($t%d)\n"
    "  sh $t%d, 20($t%d)\n",
    x, object,
    y, object,
    z, object);

  reg -= 4;

  return 0;
}

int Nintendo64::nintendo64_n64_triangle_setVertexes_aS()
{
  const int object = reg - 2;
  const int array = reg - 1;

  fprintf(out,
    "  ;; nintendo64_n64_setVertexes_bS()\n"
    "  move $s0, $ra\n"
    "  move $a0, $t%d\n"
    "  move $a1, $t%d\n"
    "  jal _triangle_vertex_copy\n"
    "  nop\n"
    "  move $ra, $s0\n",
    object,
    array);

  reg -= 2;

  return 0;
}

int Nintendo64::nintendo64_n64_triangle_setVertexes_aSI()
{
  const int object = reg - 3;
  const int array = reg - 2;
  const int offset = reg - 1;

  fprintf(out,
    "  ;; nintendo64_n64_setVertexes_bS()\n"
    "  move $s0, $ra\n"
    "  move $a0, $t%d\n"
    "  move $a1, $t%d\n"
    "  sll $t%d, $t%d, 1\n"
    "  addu $a1, $a1, $t%d\n"
    "  jal _triangle_vertex_copy\n"
    "  nop\n"
    "  move $ra, $s0\n",
    object,
    array,
    offset, offset,
    offset);

  reg -= 3;

  return 0;
}

int Nintendo64::nintendo64_n64_triangle_setZBuffer_Z()
{
  const int object = reg - 2;
  const int enabled = reg - 1;

  fprintf(out,
    "  ;; nintendo64_n64_setZBuffer_Z()\n"
    "  sb $t%d, 44($t%d)\n",
    enabled, object);

  reg -= 2;

  return 0;
}

int Nintendo64::nintendo64_n64_triangle_draw()
{
  fprintf(out,
    "  ;; nintendo64_n64_triangle_draw()\n"
    "  move $a0, $t%d\n"
    "  move $s0, $ra\n"
    "  jal _triangle_draw\n"
    "  nop\n"
    "  move $ra, $s0\n",
    reg - 1);

  reg -= 1;

  return 0;
}

int Nintendo64::nintendo64_n64_rectangle_Constructor()
{
  fprintf(out,
    "  ;; nintendo64_n64_rectangle_Constructor()\n"
    "  move $s0, $ra\n"
    "  jal _rectangle_constructor\n"
    "  nop\n"
    "  move $t%d, $v0\n"
    "  move $ra, $s0\n",
    reg - 2);

  reg -= 1;

  return 0;
}

int Nintendo64::nintendo64_n64_rectangle_setColor_I()
{
  const int object = reg - 2;
  const int color = reg - 1;

  fprintf(out,
    "  ;; nintendo64_n64_rectangle_setColor_I()\n"
    "  sw $t%d, 8($t%d)\n",
    color, object);

  reg -= 2;

  return 0;
}

int Nintendo64::nintendo64_n64_rectangle_setPosition_II()
{
  const int object = reg - 3;
  const int x = reg - 2;
  const int y = reg - 1;

  fprintf(out,
    "  ;; nintendo64_n64_rectangle_setPosition_II()\n"
    "  sll $t%d, $t%d, 2\n"
    "  sll $t%d, $t%d, 2\n"
    "  sh $t%d, 0($t%d)\n"
    "  sh $t%d, 2($t%d)\n",
    x, x,
    y, y,
    x, object,
    y, object);

  reg -= 3;

  return 0;
}

int Nintendo64::nintendo64_n64_rectangle_setSize_II()
{
  const int object = reg - 3;
  const int width = reg - 2;
  const int height = reg - 1;

  fprintf(out,
    "  ;; nintendo64_n64_rectangle_setSize_II()\n"
    "  sll $t%d, $t%d, 2\n"
    "  sll $t%d, $t%d, 2\n"
    "  sh $t%d, 4($t%d)\n"
    "  sh $t%d, 6($t%d)\n",
    width, width,
    height, height,
    width, object,
    height, object);

  reg -= 3;

  return 0;
}

int Nintendo64::nintendo64_n64_rectangle_setTextureEnabled_II()
{
  const int object = reg - 3;
  const int width = reg - 2;
  const int height = reg - 1;

  fprintf(out,
    "  ;; nintendo64_n64_rectangle_setTextureEnabled_II()\n"
    "  sh $t%d, 12($t%d)\n"
    "  sh $t%d, 14($t%d)\n",
    width, object,
    height, object);

  reg -= 3;

  return 0;
}

int Nintendo64::nintendo64_n64_rectangle_setTextureDisabled()
{
  const int object = reg - 2;

  fprintf(out,
    "  ;; nintendo64_n64_rectangle_setTextureDisabled()\n"
    "  sw $0, 12($t%d)\n",
    object);

  reg -= 1;

  return 0;
}

int Nintendo64::nintendo64_n64_rectangle_draw()
{
  const int object = reg - 1;

  fprintf(out,
    "  ;; nintendo64_n64_rectangle_draw()\n"
    "  move $a0, $t%d\n"
    "  move $s0, $ra\n"
    "  jal _rectangle_draw\n"
    "  nop\n"
    "  move $ra, $s0\n",
    object);

  reg--;

  return 0;
}

void Nintendo64::catridge_header()
{
  fprintf(out,
    "_cartridge_header:\n"
    "  .db 0x80\n"
    "  .db 0x37\n"
    "  .db 0x12\n"
    "  .db 0x40\n"
    "  ;; Clock rate.\n"
    "  .dc32 0x000f\n"
    "  ;; Vector\n"
    "  .dc32 start\n"
    "  .dc32 0x1444\n"
    "  ;; Complement checksum and checksum.\n"
    "  .db \"CRC1\"\n"
    "  .db \"CRC2\"\n"
    "  ;; Unused.\n"
    "  .dc32 0, 0\n"
    "  ;; Program title.\n"
    "  .db \"JAVA GRINDER               \"\n"
    "  ;; Developer ID code.\n"
    "  .db 0\n"
    //"  ;; Media format.\n"
    //"  db 'N', 0, 0, 0\n"
    "  ;; Cartridge ID code.\n"
    "  .dc16 0x0000\n"
    "  ;; Country code (D=Germany, E=USA, J=Japan, P=Europe, U=Australia)\n"
    "  .db 'E'\n"
    "  ;; Version (00 = 1.0).\n"
    "  .db 0\n\n");
}

void Nintendo64::init_system()
{
  fprintf(out,
    "  ;; Exception vector location in 32 bit mode is 0xbfc0_0000.\n"
    "  ;; 0x1fc0_0000 to 0x1fc0_07bf is PIF Boot ROM.\n"
    "  li $a0, PIF_BASE\n"
    "  li $t0, 8\n"
    "  sw $t0, PIF_RAM+0x3c($a0)\n\n");

  fprintf(out,
    "  ;; Enable interrupts.\n"
    "  mfc0 $t0, CP0_STATUS\n"
    "  ori $t0, $t0, 1\n"
    "  mtc0 $t0, CP0_STATUS\n\n");

  fprintf(out,
    "_setup_vi:\n"
    "  li $t0, ntsc_320x240x16\n"
    "  li $t1, (ntsc_320x240x16_end - ntsc_320x240x16) / 8\n"
    "_setup_vi_loop:\n"
    "  lw $a0, 0($t0)\n"
    "  lw $t2, 4($t0)\n"
    "  sw $t2, 0($a0)\n"
    "  addiu $t0, $t0, 8\n"
    "  addiu $t1, $t1, -1\n"
    "  bne $t1, $0, _setup_vi_loop\n"
    "  nop\n\n");
}

void Nintendo64::rsp_halt()
{
  fprintf(out,
    "  ;; Make sure RSP is halted.\n"
    "  li $a0, KSEG1 | RSP_CPU_BASE_REG\n"
    "  li $t0, 0x02\n"
    "  sw $t0, RSP_CPU_STATUS($a0)\n\n");
}

void Nintendo64::rsp_start()
{
  fprintf(out,
    "  ;; Reset RSP PC and clear halt to start it.\n"
    "  li $a0, KSEG1 | RSP_PC\n"
    "  sw $0, 0($a0)\n"
    "  li $a0, KSEG1 | RSP_CPU_BASE_REG\n"
    "  li $t0, 0x01\n"
    "  sw $t0, RSP_CPU_STATUS($a0)\n\n");
}

void Nintendo64::rsp_copy_code()
{
  fprintf(out,
    "  ;; Copy RSP code from ROM to RSP instruction memory.\n"
    "  ;; Must be done 32 bits at a time, not 64 bit.\n"
    "_setup_rsp_mem:\n"
    "  li $t1, (_rsp_code_end - _rsp_code_start) / 4\n"
    "  li $a1, _rsp_code_start\n"
    "  li $a0, KSEG1 | RSP_IMEM\n"
    "_setup_rsp_mem_loop:\n"
    "  lw $t2, 0($a1)\n"
    "  sw $t2, 0($a0)\n"
    "  addiu $a1, $a1, 4\n"
    "  addiu $a0, $a0, 4\n"
    "  addiu $t1, $t1, -1\n"
    "  bne $t1, $0, _setup_rsp_mem_loop\n"
    "  nop\n\n");
}

void Nintendo64::rsp_copy_cos_table()
{
  fprintf(out,
    ";; Copy cos() table from ROM to RSP DMEM\n"
    "  li $t1, (_cos_table_end - _cos_table_start) / 4\n"
    "  li $a1, _cos_table_start\n"
    "  li $a0, KSEG1 | RSP_DMEM + 1024\n"
    "_copy_cos_table_loop:\n"
    "  lw $t2, 0($a1)\n"
    "  sw $t2, 0($a0)\n"
    "  addiu $a1, $a1, 4\n"
    "  addiu $a0, $a0, 4\n"
    "  addiu $t1, $t1, -1\n"
    "  bne $t1, $0, _copy_cos_table_loop\n"
    "  nop\n\n");
}

void Nintendo64::rdp_copy_instructions()
{
  fprintf(out,
    "  ;; Copy RDP instructions from ROM to RSP data memory.\n"
    "_setup_rdp_mem:\n"
    "  li $a0, KSEG1 | RSP_DMEM\n"
    "  li $a1, _dp_setup\n"
    "  li $t1, (_dp_setup_end - _dp_setup) / 4\n"
    "  sw $0, 0($a0)\n"
    "  addiu $a0, $a0, 56\n"
    "_setup_rdp_mem_loop:\n"
    "  lw $t2, 0($a1)\n"
    "  sw $t2, 0($a0)\n"
    "  addiu $a1, $a1, 4\n"
    "  addiu $a0, $a0, 4\n"
    "  addiu $t1, $t1, -1\n"
    "  bne $t1, $0, _setup_rdp_mem_loop\n"
    "  nop\n\n");
}

void Nintendo64::insert_ntsc_setup()
{
  fprintf(out,
    ";; NTSC values found in the Reality Coprocessor.pdf\n"
    ";; VI_CONTROL_REG:                   0 0011 0010 0000 1110\n"
    ";; VI_TIMING_REG:  0000 0011 1110 0101 0010 0010 0011 1001\n"
    ".align 32\n"
    "ntsc_320x240x16:\n"
    "  .dc32 KSEG1 | VI_BASE | VI_CONTROL_REG,     0x0000_320e\n"
    "  .dc32 KSEG1 | VI_BASE | VI_DRAM_ADDR_REG,   0xa010_0000\n"
    "  .dc32 KSEG1 | VI_BASE | VI_H_WIDTH_REG,     0x0000_0140\n"
    "  .dc32 KSEG1 | VI_BASE | VI_V_INTR_REG,      0x0000_0200\n"
    "  .dc32 KSEG1 | VI_BASE | VI_TIMING_REG,      0x03e5_2239\n"
    "  .dc32 KSEG1 | VI_BASE | VI_V_SYNC_REG,      0x0000_020d\n"
    "  .dc32 KSEG1 | VI_BASE | VI_H_SYNC_REG,      0x0000_0c15\n"
    "  .dc32 KSEG1 | VI_BASE | VI_H_SYNC_LEAP_REG, 0x0c15_0c15\n"
    "  .dc32 KSEG1 | VI_BASE | VI_H_VIDEO_REG,     0x006c_02ec\n"
    "  .dc32 KSEG1 | VI_BASE | VI_V_VIDEO_REG,     0x0025_01ff\n"
    "  .dc32 KSEG1 | VI_BASE | VI_V_BURST_REG,     0x000e_0204\n"
    "  .dc32 KSEG1 | VI_BASE | VI_X_SCALE_REG,     0x0000_0200\n"
    "  .dc32 KSEG1 | VI_BASE | VI_Y_SCALE_REG,     0x0000_0400\n"
    "ntsc_320x240x16_end:\n\n");
}

void Nintendo64::insert_rsp_data()
{
  fprintf(out,
    ".align_bits 64\n"
    "_dp_setup:\n"
    "  .dc64 (DP_OP_SET_COLOR_IMAGE << 56) | (2 << 51) | (319 << 32) | 0x10_0000\n"
    "  .dc64 (DP_OP_SET_Z_IMAGE << 56) | (0x10_0000 + (320 * 240 * 4))\n"
    "  .dc64 (DP_OP_SET_SCISSOR << 56) | ((320 << 2) << 12) | (240 << 2)\n"
    "  .dc64 (DP_OP_SYNC_PIPE << 56)\n"
    "  .dc64 (DP_OP_SET_OTHER_MODES << 56) | (1 << 55) | (3 << 52)\n"
    "_dp_setup_end:\n\n");
}

void Nintendo64::insert_run_rdp_screen_setup()
{
  fprintf(out,
    "_run_rdp_screen_setup:\n"
    "  ;; Signal RSP code to start.\n"
    "  li $a0, KSEG1 | RSP_DMEM\n"
    "  li $t0, 1 << 24\n"
    "  sw $t0, 0($a0)\n"
    "_run_rdp_screen_setup_wait_for_rsp:\n"
    "  lb $t0, 0($a0)\n"
    "  bne $t0, $0, _run_rdp_screen_setup_wait_for_rsp\n"
    "  nop\n"
    "  jr $ra\n"
    "  nop\n\n");
}

void Nintendo64::insert_clear_screen()
{
  fprintf(out,
    "_clear_screen:\n"
    "  li $t0, 8 << 24\n"
    "  sw $t0, 0($k1)\n"
    "_clear_screen_wait:\n"
    "  lw $t0, 0($k1)\n"
    "  bne $t0, $0, _clear_screen_wait\n"
    "  nop\n"
    "  jr $ra\n"
    "  nop\n\n");
}

void Nintendo64::insert_reset_z_buffer()
{
  fprintf(out,
    "_reset_z_buffer:\n"
    "  li $t0, 9 << 24\n"
    "  sw $t0, 0($k1)\n"
    "_reset_z_buffer_wait:\n"
    "  lw $t0, 0($k1)\n"
    "  bne $t0, $0, _reset_z_buffer_wait\n"
    "  nop\n"
    "  jr $ra\n"
    "  nop\n");
}

void Nintendo64::insert_set_screen()
{
  // Note: After the bne is a li $a0, KSEG1 | VI_BASE in the delay slot.

  fprintf(out,
    "_set_screen:\n"
    "  li $a1, KSEG1\n"
    "  bne $a0, $0, _set_screen_not_0\n"
    "  li $at, KSEG1 | VI_BASE\n"
    "  li $t8, 0x0010_0000 + (320 * 240 * 2)\n"
    "  li $t9, 0x0010_0000\n"
    "  b _set_screen_finish\n"
    "  nop\n"
    "_set_screen_not_0:\n"
    "  li $t8, 0x0010_0000\n"
    "  li $t9, 0x0010_0000 + (320 * 240 * 2)\n");

  fprintf(out,
    "_set_screen_finish:\n"
    "  or $a1, $a1, $t8\n"
    "  sw $a1, 0($k0)\n"
    "  sw $t9, VI_DRAM_ADDR_REG($at)\n"
    "  sw $t8, 60($k1)\n");

  fprintf(out,
    "  li $t0, 1 << 24\n"
    "  sw $t0, 0($k1)\n"
    "_set_screen_wait:\n"
    "  lw $t0, 0($k1)\n"
    "  bne $t0, $0, _set_screen_wait\n"
    "  nop\n"
    "  jr $ra\n"
    "  nop\n\n");
}

void Nintendo64::insert_triangle_constructor()
{
  // struct
  // {
  //    0: uint16_t x0, y0, z0, reserved;
  //    8: uint16_t x1, y1, z1, reserved;
  //   16: uint16_t x2, y2, z2, reserved;
  //   24: uint16_t rx, ry, rz, reserved;
  //   32: uint16_t dx, dy, dz, reserved;
  //   40: uint32_t color;
  //   44: uint8_t do_zbuffer;
  //       uint8_t reserved, reserved, reserved;
  // };

  fprintf(out,
    "_triangle_constructor:\n"
    "  addiu $sp, $sp, -12 * 4\n"
    "  move $v0, $sp\n"
    "  sw $0,  0($v0)\n"
    "  sw $0,  4($v0)\n"
    "  sw $0,  8($v0)\n"
    "  sw $0, 12($v0)\n"
    "  sw $0, 16($v0)\n"
    "  sw $0, 20($v0)\n"
    "  sw $0, 24($v0)\n"
    "  sw $0, 28($v0)\n"
    "  sw $0, 32($v0)\n"
    "  sw $0, 40($v0)\n"
    "  sw $0, 44($v0)\n"
    "  jr $ra\n"
    "  nop\n\n");
}

void Nintendo64::insert_rectangle_constructor()
{
  // struct
  // {
  //    0: uint16_t x, y;
  //    4: uint16_t width, height;
  //    8: uint32_t color;
  //   12: uint16_t texture_width, texture_height;
  // };

  fprintf(out,
    "_rectangle_constructor:\n"
    "  addiu $sp, $sp, -4 * 4\n"
    "  move $v0, $sp\n"
    "  sw $0,  0($v0)\n"
    "  sw $0,  4($v0)\n"
    "  sw $0,  8($v0)\n"
    "  sw $0, 12($v0)\n"
    "  jr $ra\n"
    "  nop\n\n");
}

void Nintendo64::insert_triangle_draw()
{
  fprintf(out,
    "_triangle_draw:\n"
    "  ;; Make sure RSP is not busy.\n"
    "_draw_triangle_wait_rsp_busy:\n"
    "  lw $at, 0($k1)\n"
    "  bne $at, $0, _draw_triangle_wait_rsp_busy\n"
    "  nop\n"
    "  move $a1, $a0\n"
    "  move $a2, $k1\n"
    "  li $a3, 11\n"
    "_triangle_draw_memcpy:\n"
    "  lw $t8, 0($a1)\n"
    "  sw $t8, 8($a2)\n"
    "  addiu $a1, $a1, 4\n"
    "  addiu $a2, $a2, 4\n"
    "  addiu $a3, $a3, -1\n"
    "  bne $a3, $0, _triangle_draw_memcpy\n"
    "  nop\n"
    "  ;; Copy do_zbuffer boolean to RSP.\n"
    "  lb $t8, 44($a0)\n"
    "  sll $t8, $t8, 8\n"
    "  ;; Set command to draw_triangle.\n"
    "  li $at, 4 << 24\n"
    "  or $at, $at, $t8\n"
    "  sw $at, 0($k1)\n"
    "  jr $ra\n"
    "  nop\n\n");
}

void Nintendo64::insert_rectangle_draw()
{
  fprintf(out,
    "_rectangle_draw:\n"
    "  ;; Make sure RSP is not busy.\n"
    "_rectangle_draw_wait_rsp_busy:\n"
    "  lw $at, 0($k1)\n"
    "  bne $at, $0, _rectangle_draw_wait_rsp_busy\n"
    "  nop\n"
    "  lw $at,  0($a0)\n"
    "  sw $at,  8($k1)\n"
    "  lh $at,  0($a0)\n"
    "  lh $s1,  2($a0)\n"
    "  lh $s2,  4($a0)\n"
    "  lh $s3,  6($a0)\n"
    "  addu $s2, $s2, $at\n"
    "  addu $s3, $s3, $s1\n"
    "  sll $s2, $s2, 16\n"
    "  or $s2, $s2, $s3\n"
    "  sw $s2, 16($k1)\n"
    "  lw $at,  8($a0)\n"
    "  sw $at, 48($k1)\n"
    "  lw $t8, 12($a0)\n"
    "  bne $t8, $0, _rectangle_draw_with_texture\n"
    "  nop\n");

  fprintf(out,
    "  ;; Set command to draw_rectangle.\n"
    "  li $at, 5 << 24\n"
    "  sw $at, 0($k1)\n"
    "  jr $ra\n"
    "  nop\n");

  fprintf(out,
    "_rectangle_draw_with_texture:\n"
    "  ;; Set command to draw_rectangle with texture.\n"
    "  sw $t8, 40($k1)\n"
    "  li $at, 6 << 24\n"
    "  sw $at, 0($k1)\n"
    "  jr $ra\n"
    "  nop\n\n");
}

void Nintendo64::insert_triangle_vertex_copy()
{
  fprintf(out,
    "_triangle_vertex_copy:\n"
    "  ;; X0, Y0, Z0\n"
    "  lh $t8,  0($a1)\n"
    "  lh $t9,  2($a1)\n"
    "  lh $at,  4($a1)\n"
    "  sh $t8,  0($a0)\n"
    "  sh $t9,  2($a0)\n"
    "  sh $at,  4($a0)\n"
    //"  sh $0,   6($a0)\n"
    "  ;; X1, Y1, Z1\n"
    "  lh $t8,  6($a1)\n"
    "  lh $t9,  8($a1)\n"
    "  lh $at, 10($a1)\n"
    "  sh $t8,  8($a0)\n"
    "  sh $t9, 10($a0)\n"
    "  sh $at, 12($a0)\n"
    //"  sh $0,  14($a0)\n"
    "  ;; X2, Y2, Z2\n"
    "  lh $t8, 12($a1)\n"
    "  lh $t9, 14($a1)\n"
    "  lh $at, 16($a1)\n"
    "  sh $t8, 16($a0)\n"
    "  sh $t9, 18($a0)\n"
    "  sh $at, 20($a0)\n"
    //"  sh $0,  22($a0)\n"
    "  jr $ra\n"
    "  nop\n\n");
}

void Nintendo64::insert_load_texture()
{
  fprintf(out,
    "_load_texture:\n"
    "  ;; Make sure RSP is not busy.\n"
    "_load_texture_wait_rsp_busy:\n"
    "  lw $at, 0($k1)\n"
    "  bne $at, $0, _load_texture_wait_rsp_busy\n"
    "  nop\n"
    "  ;; Copy texture to video RAM.\n"
    "  addiu $a3, $k0, 16\n"
    "  multu $a1, $a2\n"
    "  mflo $t8\n"
    //"  move $t9, $a3\n"
    "_load_texture_memcpy:\n"
    "  lh $at, 0($a0)\n"
    "  sh $at, 0($a3)\n"
    "  addiu $a0, $a0, 2\n"
    "  addiu $a3, $a3, 2\n"
    "  addiu $t8, $t8, -1\n"
    "  bne $t8, $0, _load_texture_memcpy\n"
    "  nop\n"
    "  ;; Copy setup to DMEM.\n"
    "  sll $a1, $a1, 16\n"
    "  or $a1, $a1, $a2\n"
    "  sw $a1, 40($k1)\n"
    "  li $a3, 0x0010_0000 + (320 * 240 * 2 * 2 * 2) + 16\n"
    "  sw $a3, 48($k1)\n");

  fprintf(out,
    "  ;; Set command to setup_texture.\n"
    "  li $at, 7 << 24\n"
    "  sw $at, 0($k1)\n"
    //"_load_texture_wait:\n"
    //"  lw $at, 0($k1)\n"
    //"  bne $at, $0, _load_texture_wait\n"
    //"  nop\n"
    "  jr $ra\n"
    "  nop\n\n");
}

void Nintendo64::insert_cos_table()
{
  uint32_t value;
  int i;

  fprintf(out, "_cos_table_start:\n");

  for (i = 0; i < 512; i++)
  {
    float c = M_PI * 2;
    float d = cos(((float)i / 512) * c);

    if (d >= 0)
    {
      value = (int)(65536 * d);
    }
      else
    {
      value = (int)(65536 * -d);
      value ^= 0xffffffff;
      value += 1;
    }

    if ((i % 6) == 0)
    {
      if (i != 0) { fprintf(out, "\n"); }
      fprintf(out, "  .dc32");
    }

    fprintf(out, " 0x%08x,", value);
  }

  fprintf(out, "\n_cos_table_end:\n\n");
}

void Nintendo64::insert_rsp_code()
{
  fprintf(out,
    "_rsp_code_start:\n"
    ".binfile \"rsp.bin\"\n"
    "_rsp_code_end:\n\n");
}

