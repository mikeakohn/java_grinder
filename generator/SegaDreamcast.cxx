/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2020 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "generator/Math.h"
#include "generator/SegaDreamcast.h"

SegaDreamcast::SegaDreamcast()
{
  //org = 0x0c010000;
  org = 0x8c010000;
  ram_start = 0x00000000;
  ram_end = 32 * 1024 * 1024;
  //physical_address = 0x0;
  preload_array_align = 128;
}

SegaDreamcast::~SegaDreamcast()
{
}

int SegaDreamcast::open(const char *filename)
{
  if (SH4::open(filename) != 0) { return -1; }

  fprintf(out,
    ".include \"powervr.inc\"\n"
    ".entry_point start\n"
    ".export start\n\n");

  return 0;
}

int SegaDreamcast::finish()
{
  SH4::finish();

  //Math::add_sin_table(out);
  //Math::add_cos_table(out);

  return 0;
}

int SegaDreamcast::start_init()
{
  SH4::start_init();

  init_system();
  init_video();

  return 0;
}

int SegaDreamcast::new_object(std::string &object_name, int field_count)
{
  fprintf(out, "  ;; new_object(%s, field_count=%d)\n",
    object_name.c_str(), field_count);


  reg++;

  return 0;
}

void SegaDreamcast::init_system()
{
  fprintf(out,
    "  ; First, make sure to run in the P2 area (privileged mode, no cache).\n"
    "  mov.l _setup_cache_addr, r0\n"
    "  mov.l _p2_mask, r1\n"
    "  or r1, r0\n"
    "  jmp @r0\n"
    "  nop\n\n");

  fprintf(out,
    "_setup_cache:\n"
    "  ; Now that the SH4 is in P2, it's safe to enable the cache.\n"
    "  mov.l _ccr_addr, r0\n"
    "  mov.w _ccr_data, r1\n"
    "  mov.l r1, @r0\n"
    "  ; After changing CCR, eight instructions must be executed before it's\n"
    "  ; safe to enter a cached area such as P1.\n"
    "  mov.l _setup_done_address, r0 ; 1\n"
    "  mov #0, r1          ; 2\n"
    "  nop                 ; 3\n"
    "  nop                 ; 4\n"
    "  nop                 ; 5\n"
    "  nop                 ; 6\n"
    "  nop                 ; 7\n"
    "  nop                 ; 8\n"
    "  jmp @r0             ; go\n"
    "  mov r1, r0\n\n");

  fprintf(out,
    ".align 32\n"
    "_p2_mask:\n"
    "  .dc32 0xa0000000\n"
    "_setup_cache_addr:\n"
    "  .dc32 _setup_cache\n"
    "_setup_done_address:\n"
    "  .dc32 _setup_done\n"
    "_ccr_addr:\n"
    "  .dc32 0xff00001c\n"
    "_ccr_data:\n"
    "  .dc16 0x090d\n\n");

  fprintf(out, "_setup_done:\n");
}

void SegaDreamcast::init_video()
{
  const char *options[] =
  {
    "sync_cfg",
    "display_sync_load",
    "display_sync_width",
    "display_mode",
    "hborder",
    "vborder",
    "hposition",
    "vposition",
    "display_size",
    "video_config",
    "unknown_1",
    "unknown_2",
    "display_memory_1",
    "display_memory_2",
    "border_color",
    NULL
  };

  fprintf(out, "  ; Setup video\n");

  for (int n = 0; options[n] != NULL; n++)
  {
    fprintf(out,
      "  mov.l _%s, r1\n"
      "  mov.l _%s_value, r2\n"
      "  mov.l r2, @r1\n\n",
      options[n], options[n]);
  }

  fprintf(out,
    "  ;; Put some stuff in the frame buffer.\n"
    "  mov.l _frame_buffer, r1\n"
    "  mov.l _test_color, r2\n"
    "  mov.l _pixel_count, r0\n"
    "_frame_buffer_loop:\n"
    "  mov.l r2, @r1\n"
    "  add #4, r1\n"
    "  add #-1, r0\n"
    "  cmp/eq #0, r0\n"
    "  bf _frame_buffer_loop\n"
    "  nop\n\n");

  fprintf(out,
    "  .align 32\n"
    "_border_color:\n"
    "  .dc32 POWERVR_BORDER_COL\n"
    "_border_color_value:\n"
    "  .dc32 127\n"
    "_sync_cfg:\n"
    "  .dc32 POWERVR_SYNC_CFG\n"
    "_sync_cfg_value:\n"
    "  .dc32 POWERVR_SYNC_ENABLE | POWERVR_VIDEO_NTSC | POWERVR_VIDEO_INTERLACE\n"
    "_display_mode:\n"
    "  .dc32 POWERVR_FB_DISPLAY_CFG\n"
    "_display_mode_value:\n"
    "  ;; 8: Threshold when display mode is ARGB8888.\n"
    "  ;; 2: Pixel mode 3 = RGB0888 (4 bytes / pixel)\n"
    "  ;; 1: Line double.\n"
    "  ;; 0: Enable.\n"
    "  .dc32 (0xff << 8) | (3 << 2) | 1\n"
    "_display_memory_1:\n"
    "  .dc32 POWERVR_FB_DISPLAY_ADDR1\n"
    "_display_memory_1_value:\n"
    "  .dc32 0\n"
    "_display_memory_2:\n"
    "  .dc32 POWERVR_FB_DISPLAY_ADDR2\n"
    "_display_memory_2_value:\n"
    "  .dc32 640 * 240 * 4\n"
    "_frame_buffer:\n"
    "  .dc32 0xa500_0000\n"
    "_test_color:\n"
    "  .dc32 0xff00ff\n"
    "_pixel_count:\n"
    "  .dc32 640 * 240\n"
    "_display_sync_load:\n"
    "  .dc32 POWERVR_SYNC_LOAD\n"
    "_display_sync_load_value:\n"
    "  .dc32 (524 << 16) | 857\n"
    "_display_sync_width:\n"
    "  .dc32 POWERVR_SYNC_WIDTH\n"
    "_display_sync_width_value:\n"
    "  .dc32 (0x1f << 22) | (364 << 12) | (0x06 << 8) | 0x3f\n"
    "_hborder:\n"
    "  .dc32 POWERVR_HBORDER\n"
    "_hborder_value:\n"
    "  .dc32 (126 << 16) | 837\n"
    "_vborder:\n"
    "  .dc32 POWERVR_VBORDER\n"
    "_vborder_value:\n"
    "  .dc32 (36 << 16) | 516\n"
    "_hposition:\n"
    "  .dc32 POWERVR_HPOS\n"
    "_hposition_value:\n"
    "  .dc32 164\n"
    "_vposition:\n"
    "  .dc32 POWERVR_VPOS\n"
    "_vposition_value:\n"
    "  .dc32 (18 << 16) | 18\n"
    "_display_size:\n"
    "  .dc32 POWERVR_FB_DISPLAY_SIZE\n"
    "_display_size_value:\n"
    "  .dc32 (641 << 20) | (239 << 10) | 639\n"
    "_video_config:\n"
    "  .dc32 POWERVR_VIDEO_CFG\n"
    "_video_config_value:\n"
    "  .dc32 (0x16 << 16)\n"
    "_unknown_1:\n"
    "  .dc32 0xa05f8110\n"
    "_unknown_1_value:\n"
    "  .dc32 0x00093f39\n"
    "_unknown_2:\n"
    "  .dc32 0xa05f8114\n"
    "_unknown_2_value:\n"
    "  .dc32 0x00200000\n\n");
}

