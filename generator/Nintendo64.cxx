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

#include "generator/Nintendo64.h"

Nintendo64::Nintendo64()
{
  org = 0x80000000;
  ram_start = 0x00000000;
  ram_end = 32 * 1024 * 1024;
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
  insert_rsp_code();
  R4000::finish();

  return 0;
}

int Nintendo64::start_init()
{
  catridge_header();

  fprintf(out,
    "bootcode:\n"
    ".binfile \"bootcode.bin\"\n"
    "start:\n");

  init_system();

  fprintf(out,
    "  ;; $k0 points to struct of\n"
    "  ;; uint32_t current_screen\n"
    "  ;; uint8_t  screen_id\n"
    "  li $k0, 320 * 240 * 2 * 2 * 2\n"
    "  li $t0, KSEG1 | 0x0010_0000\n"
    "  li $t1, 1\n"
    "  sw $t0, 0($k0)\n"
    "  sb $t1, 4($k0)\n");

  fprintf(out,
    "  ;; $k1 points to DMEM\n"
    "  li $k1, KSEG1 | RSP_DMEM\n");

  fprintf(out,
    "  ;; Make sure RSP is halted.\n"
    "  li $a0, KSEG1 | RSP_CPU_BASE_REG\n"
    "  li $t0, 0x02\n"
    "  sw $t0, RSP_CPU_STATUS($a0)\n\n");

  fprintf(out,
    "  ;; Copy RDP instructions from ROM to RSP data memory.\n"
    "  ;; Must be done 32 bits at a time, not 64 bit.\n"
    "_setup_rdp:\n"
    "  li $a0, KSEG1 | RSP_DMEM\n"
    "  li $a1, dp_setup\n"
    "  li $t1, (_dp_list_end - _dp_setup) / 4\n"
    "  sw $0, 0($a0)\n"
    "  addiu $a0, $a0, 56\n"
    "_setup_rdp_loop:\n"
    "  lw $t2, 0($a1)\n"
    "  sw $t2, 0($a0)\n"
    "  addiu $a1, $a1, 4\n"
    "  addiu $a0, $a0, 4\n"
    "  addiu $t1, $t1, -1\n"
    "  bne $t1, $0, _setup_rdp_loop\n"
    "  nop\n");

  fprintf(out,
    "  ;; Copy RSP code from ROM to RSP instruction memory.\n"
    "  ;; Must be done 32 bits at a time, not 64 bit.\n"
    "_setup_rsp:\n"
    "  li $a1, _rsp_code_start\n"
    "  li $t1, (_rsp_code_end - _rsp_code_start) / 4\n"
    "  li $a0, KSEG1 | RSP_IMEM\n"
    "_setup_rsp_loop:\n"
    "  lw $t2, 0($a1)\n"
    "  sw $t2, 0($a0)\n"
    "  addiu $a1, $a1, 4\n"
    "  addiu $a0, $a0, 4\n"
    "  addiu $t1, $t1, -1\n"
    "  bne $t1, $0, _setup_rsp_loop\n"
    "  nop\n\n");

  fprintf(out,
    "  ;; Reset RSP PC and clear halt to start it.\n"
    "  li $a0, KSEG1 | RSP_PC\n"
    "  sw $0, 0($a0)\n"
    "  li $a0, KSEG1 | RSP_CPU_BASE_REG\n"
    "  li $t0, 0x01\n"
    "  sw $t0, RSP_CPU_STATUS($a0)\n\n"
    "  li $t8, 56\n"
    "  li $t9, _dp_setup_end - _dp_setup\n"
    "  jal send_command\n"
    "  nop\n");

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
    "  li $t8, (319 << 16) | 240\n"
    "  li $t9, 0xffff\n"
    "  sw $0,   8($k1)\n"
    "  sw $t8, 16($k1)\n"
    "  sw $0,  48($k1)\n"
    "  li $t8, 4 << 24\n"
    "  sw $t8, 0($k1)\n"
    "_reset_z_buffer_wait_%d:\n"
    "  lw $t8, 0($k1)\n"
    "  bne $t8, $0, _reset_z_buffer_wait_%d\n"
    "  nop\n",
    label_count,
    label_count);

  label_count += 1;

  return 0;
}

int Nintendo64::nintendo64_resetZBuffer()
{
  fprintf(out,
    "  ;; nintendo64_resetZBuffer()\n"
    "  li $at, 320 * 240 * 2\n"
    "  li $a0, KSEG1 | VI_BASE\n"
    "  lw $t8, 0($k0)\n"
    "  addu $t8, $t8, $at\n"
    "  sw $t8, VI_DRAM_ADDR_REG($a0)\n");

  fprintf(out,
    "  li $t8, (319 << 16) | 240\n"
    "  li $t9, 0xffff\n"
    "  sw $0,   8($k1)\n"
    "  sw $t8, 16($k1)\n"
    "  sw $t9, 48($k1)\n"
    "  li $t8, 4 << 24\n"
    "  sw $t8, 0($k1)\n"
    "_reset_z_buffer_wait_%d:\n"
    "  lw $t8, 0($k1)\n"
    "  bne $t8, $0, _reset_z_buffer_wait_%d\n"
    "  nop\n",
    label_count,
    label_count);

  fprintf(out,
    "  lw $t8, 0($k0)\n"
    "  sw $t8, VI_DRAM_ADDR_REG($a0)\n");

  label_count += 1;

  return 0;
}

int Nintendo64::nintendo64_setScreen_I()
{
  // Note: After the bne is a li $a0, KSEG1 | VI_BASE in the delay slot.
  // setScreen(0): Display screen 0, draw on screen 1.
  // setScreen(1): Display screen 1, draw on screen 0.

  fprintf(out,
    "  ;; nintendo64_setScreen_I()\n"
    "  bne $t%d, $0, _set_screen_not_0%d\n"
    "  li $a0, KSEG1 | VI_BASE\n"
    "  li $t8, KSEG1 | 0x0010_0000 + (320 * 2 * 2)\n"
    "  li $t9, 0x0010_0000\n"
    "  b _set_screen_finish_%d\n"
    "  nop\n"
    "_set_screen_not_0%d:\n"
    "  li $t8, KSEG1 | 0x0010_0000\n"
    "  li $t9, 0x0010_0000 + (320 * 2 * 2)\n"
    "_set_screen_finish_%d:\n"
    "  sw $t8, 0($k0)\n"
    "  sw $t9, VI_DRAM_ADDR_REG($a0)\n",
    reg - 1, label_count,
    label_count,
    label_count,
    label_count);

  label_count += 1;
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

int Nintendo64::nintendo64_n64_triangle_Constructor()
{
  return -1;
}

int Nintendo64::nintendo64_n64_triangle_setColor_I()
{
  return -1;
}

int Nintendo64::nintendo64_n64_triangle_setPosition_III()
{
  return -1;
}

int Nintendo64::nintendo64_n64_triangle_setRotation_III() 
{
  return -1;
}

int Nintendo64::nintendo64_n64_triangle_setVertex0_III() 
{
  return -1;
}

int Nintendo64::nintendo64_n64_triangle_setVertex1_III() 
{
  return -1;
}

int Nintendo64::nintendo64_n64_triangle_setVertex2_III() 
{
  return -1;
}

int Nintendo64::nintendo64_n64_triangle_setVertexes_bS() 
{
  return -1;
}

int Nintendo64::nintendo64_n64_triangle_draw() 
{
  return -1;
}

int Nintendo64::nintendo64_n64_rectangle_Constructor() 
{
  return -1;
}

int Nintendo64::nintendo64_n64_rectangle_setColor_I() 
{
  return -1;
}

int Nintendo64::nintendo64_n64_rectangle_setPosition_II()
{
  return -1;
}

int Nintendo64::nintendo64_n64_rectangle_setSize_II()
{
  return -1;
}

int Nintendo64::nintendo64_n64_rectangle_setTexture_aBII()
{
  return -1;
}

int Nintendo64::nintendo64_n64_rectangle_draw()
{
  return -1;
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
    "  sw $t0, PIF_RAM+0x3c($a0)\n");

  fprintf(out,
    "  ;; Enable interrupts.\n"
    "  mfc0 $t0, CP0_STATUS\n"
    "  ori $t0, $t0, 1\n"
    "  mtc0 $t0, CP0_STATUS\n\n");

  fprintf(out,
    "setup_video:\n"
    "  li $t0, ntsc_320x240x16\n"
    "  li $t1, (ntsc_320x240x16_end - ntsc_320x240x16) / 8\n"
    "setup_video_loop:\n"
    "  lw $a0, 0($t0)\n"
    "  lw $t2, 4($t0)\n"
    "  sw $t2, 0($a0)\n"
    "  addiu $t0, $t0, 8\n"
    "  addiu $t1, $t1, -1\n"
    "  bne $t1, $0, setup_video_loop\n"
    "  nop\n\n");
}

void Nintendo64::insert_ntsc_setup()
{
  fprintf(out,
    ";; NTSC values found in the Reality Coprocessor.pdf\n"
    ";; VI_CONTROL_REG:                   0 0011 0010 0000 1110\n"
    ";; VI_TIMING_REG:  0000 0011 1110 0101 0010 0010 0011 1001\n"
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
    "  .dc64 (DP_OP_SET_Z_IMAGE << 56) | (0x10_0000 + (320 * 240 * 2))\n"
    "  .dc64 (DP_OP_SET_SCISSOR << 56) | ((320 << 2) << 12) | (240 << 2)\n"
    "  .dc64 (DP_OP_SYNC_PIPE << 56)\n"
    "  .dc64 (DP_OP_SET_OTHER_MODES << 56) | (1 << 55) | (3 << 52)\n"
    "_dp_setup_end:\n");

}

void Nintendo64::insert_rsp_code()
{
  fprintf(out,
    "_rsp_code_start:\n"
    ".binfile \"rsp.bin\"\n"
    "_rsp_code_end:\n");
}

