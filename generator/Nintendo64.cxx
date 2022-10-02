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
    ".include \"nintendo64/cp0.inc\"\n"
    ".include \"nintendo64/system.inc\"\n"
    ".include \"nintendo64/video_interface.inc\"\n\n");

  fprintf(out,
    ".org 0x8000_0000 + 1052672 - 1\n"
    ".db 0x00\n"
    ".org 0x8000_0000\n\n");

  return 0;
}

int Nintendo64::finish()
{
  ntsc_setup();
  R4000::finish();

  return 0;
}

int Nintendo64::start_init()
{
  catridge_header();
  init_system();

  fprintf(out,
    "bootcode:\n"
    ".binfile \"bootcode.bin\"\n"
    "start:\n");

  return 0;
}

int Nintendo64::nintendo64_waitVsync()
{
  return -1;
}

int Nintendo64::nintendo64_clearScreen()
{
  return -1;
}

int Nintendo64::nintendo64_setScreen_I()
{
  return -1;
}

int Nintendo64::nintendo64_plot_III()
{
  return -1;
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
    "cartridge_header:\n"
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
    "  .db \"NAKEN_ASM SAMPLE    \"\n"
    "  ;; Unknown.\n"
    "  dc32 0\n"
    "  ;; Media format.\n"
    "  db 'N', 0, 0, 0\n"
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
    "  lui $t0, 0xbfc0\n"
    "  li $t1, 8\n"
    "  sw $t1, 0x7fc($t0)\n\n");

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

void Nintendo64::ntsc_setup()
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
    "ntsc_320x240x16_end:\n");
}

