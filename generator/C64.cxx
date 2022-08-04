/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn, Joe Davisson
 *
 * C64 written by Joe Davisson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "generator/C64.h"

#define POKE(dst) \
  fprintf(out, "  inx\n"); \
  fprintf(out, "  lda stack_lo,x\n"); \
  fprintf(out, "  sta 0x%04x\n", dst)

#define PEEK(src) \
  fprintf(out, "  lda 0x%04x\n", src); \
  fprintf(out, "  sta stack_lo,x\n"); \
  fprintf(out, "  lda #0\n"); \
  fprintf(out, "  sta stack_hi,x\n"); \
  fprintf(out, "  dex\n")

C64::C64() :
  label_count(0),

  need_c64_vic_hires_enable(0),
  need_c64_vic_hires_clear(0),
  need_c64_vic_hires_plot(0),
  need_c64_vic_make_hires_tables(0),
  need_c64_vic_text_enable(0),
  need_c64_vic_text_clear(0),
  need_c64_vic_text_copy(0),
  need_c64_vic_text_plot(0),
  need_c64_vic_text_read(0),
  need_c64_vic_make_color_table(0),
  need_c64_vic_color_ram_clear(0),
  need_c64_vic_copy_uppercase(0),
  need_c64_vic_copy_lowercase(0)
{
  start_org = 0x07ff;

  // java stack pointer location
  java_stack_lo = 0x80;
  java_stack_hi = 0xc0;
  // java_stack_lo = 0x200;
  // java_stack_hi = 0x300;

  // heap location (for new arrays)
  ram_start = 0xa000;
}

C64::~C64()
{
  if (need_c64_vic_hires_enable) { insert_c64_vic_hires_enable(); }
  if (need_c64_vic_hires_clear) { insert_c64_vic_hires_clear(); }
  if (need_c64_vic_hires_plot) { insert_c64_vic_hires_plot(); }
  if (need_c64_vic_make_hires_tables) { insert_c64_vic_make_hires_tables(); }
  if (need_c64_vic_text_enable) { insert_c64_vic_text_enable(); }
  if (need_c64_vic_text_clear) { insert_c64_vic_text_clear(); }
  if (need_c64_vic_text_copy) { insert_c64_vic_text_copy(); }
  if (need_c64_vic_text_plot) { insert_c64_vic_text_plot(); }
  if (need_c64_vic_text_read) { insert_c64_vic_text_read(); }
  if (need_c64_vic_make_text_table) { insert_c64_vic_make_text_table(); }
  if (need_c64_vic_make_color_table) { insert_c64_vic_make_color_table(); }
  if (need_c64_vic_color_ram_clear) { insert_c64_vic_color_ram_clear(); }
  if (need_c64_vic_copy_uppercase) { insert_c64_vic_copy_uppercase(); }
  if (need_c64_vic_copy_lowercase) { insert_c64_vic_copy_lowercase(); }
}

int C64::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".6502\n");

  // heap
  fprintf(out, "  ram_start equ 0x%04x\n", ram_start);
  fprintf(out, "  heap_ptr equ ram_start\n");

  // for indirection (2 bytes)
  fprintf(out, "  address equ 0x02\n");

  // java stack
  fprintf(out, "  stack_lo equ 0x%04x\n", java_stack_lo);
  fprintf(out, "  stack_hi equ 0x%04x\n", java_stack_hi);

  // points to locals
  fprintf(out, "  locals equ 0x04\n");

  // temp variables
  fprintf(out, "  result equ 0x0a\n");
  fprintf(out, "  remainder equ 0x0c\n");
  fprintf(out, "  length equ 0x0e\n");
  fprintf(out, "  value1 equ 0x10\n");
  fprintf(out, "  value2 equ 0x12\n");
  fprintf(out, "  value3 equ 0x14\n");
  fprintf(out, "  temp1 equ 0x16\n");

  // text/color tables
  fprintf(out, "  text_table equ 0x18\n");
  fprintf(out, "  color_table equ 0x4a\n");

  // sprites
  fprintf(out, "  sprite_msb_set equ 0x06\n");
  fprintf(out, "  sprite_msb_clear equ 0x07\n");
  fprintf(out, "  sprite_x equ 0x08\n");
  fprintf(out, "  sprite_y equ 0x09\n");

  // basic loader
  fprintf(out, ".org 0x%04x\n", start_org);
  fprintf(out, "dw 0x0801\n");
  fprintf(out, "dw start\n");
  fprintf(out, "dw 2013\n");
  fprintf(out, "db 0x9e\n");
  fprintf(out, "db (((start / 1000) %% 10) + 0x30)\n");
  fprintf(out, "db (((start / 100) %% 10) + 0x30)\n");
  fprintf(out, "db (((start / 10) %% 10) + 0x30)\n");
  fprintf(out, "db (((start / 1) %% 10) + 0x30)\n");
  fprintf(out, "db ':'\n");
  fprintf(out, "db 0x8f\n");
  fprintf(out, "db \" JAVA-GRINDER \"\n");
  fprintf(out, "db 0\n");
  fprintf(out, "dw 0\n\n");

  fprintf(out, "start:\n");
  fprintf(out, "  sei\n");
  fprintf(out, "  cld\n");

  // reset processor stack
  fprintf(out, "  ldx #0xff\n");
  fprintf(out, "  txs\n");

  // reset java stack pointer
  // fprintf(out, "  ldx #0xff\n");  // for 0x200/0x300
  fprintf(out, "  ldx #0x3f\n");  // for 0x80/0xc0

  // clear_stack (so it looks nice in monitor)
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "clear_stack_loop:\n");
  fprintf(out, "  sta 0x80,y\n");
  fprintf(out, "  sta 0xc0,y\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  cpy #0x40\n");
  fprintf(out, "  bne clear_stack_loop\n");

  // switch VIC-II to bank 0
  fprintf(out, "  lda #4\n");
  fprintf(out, "  sta 0xdd00\n");

  // put text screen at 0xc400 and charset at 0xc800
  fprintf(out, "  lda #0x12\n");
  fprintf(out, "  sta 0xd018\n");

  // turn off ROM chips
  fprintf(out, "  lda #53\n");
  fprintf(out, "  sta 0x0001\n");
  fprintf(out, "\n");

  return 0;
}

int C64::c64_sid_voice1_frequency(/* value */)
{
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  sta 0xd401\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd400\n");

  return 0;
}

int C64::c64_sid_voice1_pulse_width(/* value */)
{
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  sta 0xd403\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd402\n");

  return 0;
}

int C64::c64_sid_voice1_waveform(/* value */) { POKE(0xd404); return 0; }

int C64::c64_sid_voice1_adsr(/* value */)
{
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  sta 0xd406\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd405\n");

  return 0;
}

int C64::c64_sid_voice2_frequency(/* value */)
{
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  sta 0xd408\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd407\n");

  return 0;
}

int C64::c64_sid_voice2_pulse_width(/* value */)
{
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  sta 0xd40a\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd409\n");

  return 0;
}

int C64::c64_sid_voice2_waveform(/* value */) { POKE(0xd40b); return 0; }

int C64::c64_sid_voice2_adsr(/* value */)
{
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  sta 0xd40d\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd40c\n");

  return 0;
}

int C64::c64_sid_voice3_frequency(/* value */)
{
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  sta 0xd40f\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd40e\n");

  return 0;
}

int C64::c64_sid_voice3_pulse_width(/* value */)
{
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  sta 0xd411\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd410\n");

  return 0;
}

int C64::c64_sid_voice3_waveform(/* value */) { POKE(0xd412); return 0; }

int C64::c64_sid_voice3_adsr(/* value */)
{
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  sta 0xd414\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd413\n");

  return 0;
}

int C64::c64_sid_filter_cutoff(/* value */)
{
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  sta 0xd416\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd415\n");

  return 0;
}

int C64::c64_sid_filter_resonance(/* value */) { POKE(0xd417); return 0; }
int C64::c64_sid_volume(/* value */) { POKE(0xd418); return 0; }
int C64::c64_sid_voice3_oscillator(/* value */) { POKE(0xd41b); return 0; }
int C64::c64_sid_voice3_envelope(/* value */) { POKE(0xd41c); return 0; }

int C64::c64_sid_clear()
{
  fprintf(out, "; sid clear\n");
  fprintf(out, "  ldy #0x1c\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta 0xd400,y\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bpl #-6\n");

  return 0;
}

int C64::c64_vic_sprite0pos(/* x, y */)
{
  fprintf(out, "; sprite0pos\n");

  // y
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd001\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 1\n");
  fprintf(out, "  sta 0xd010\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #1\n");
  fprintf(out, "  sta 0xd010\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd000\n");

  return 0;
}

int C64::c64_vic_sprite1pos(/* x, y */)
{
  fprintf(out, "; sprite1pos\n");

  // y
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd003\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 2\n");
  fprintf(out, "  sta 0xd010\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #2\n");
  fprintf(out, "  sta 0xd010\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd002\n");

  return 0;
}

int C64::c64_vic_sprite2pos(/* x, y */)
{
  fprintf(out, "; sprite2pos\n");

  // y
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd005\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 4\n");
  fprintf(out, "  sta 0xd010\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #4\n");
  fprintf(out, "  sta 0xd010\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd004\n");

  return 0;
}

int C64::c64_vic_sprite3pos(/* x, y */)
{
  fprintf(out, "; sprite3pos\n");

  // y
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd007\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 8\n");
  fprintf(out, "  sta 0xd010\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #8\n");
  fprintf(out, "  sta 0xd010\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd006\n");

  return 0;
}

int C64::c64_vic_sprite4pos(/* x, y */)
{
  fprintf(out, "; sprite4pos\n");

  // y
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd009\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 16\n");
  fprintf(out, "  sta 0xd010\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #16\n");
  fprintf(out, "  sta 0xd010\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd008\n");

  return 0;
}

int C64::c64_vic_sprite5pos(/* x, y */)
{
  fprintf(out, "; sprite5pos\n");

  // y
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd00b\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 32\n");
  fprintf(out, "  sta 0xd010\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #32\n");
  fprintf(out, "  sta 0xd010\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd00a\n");

  return 0;
}

int C64::c64_vic_sprite6pos(/* x, y */)
{
  fprintf(out, "; sprite6pos\n");

  // y
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd00d\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 64\n");
  fprintf(out, "  sta 0xd010\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #64\n");
  fprintf(out, "  sta 0xd010\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd00c\n");

  return 0;
}

int C64::c64_vic_sprite7pos(/* x, y */)
{
  fprintf(out, "; sprite7pos\n");

  // y
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd00f\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 128\n");
  fprintf(out, "  sta 0xd010\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #128\n");
  fprintf(out, "  sta 0xd010\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xd00e\n");

  return 0;
}

int C64::c64_vic_write_control1(/* value */) { POKE(0xd011); return 0; }
int C64::c64_vic_read_control1() { PEEK(0xd011); return 0; }

int C64::c64_vic_wait_raster(/* line */)
{
  fprintf(out, "; wait_raster\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  cmp 0xd012\n");
  fprintf(out, "  bne #-5\n");

  return 0;
}

int C64::c64_vic_sprite_enable(/* value */) { POKE(0xd015); return 0; }
int C64::c64_vic_write_control2(/* value */) { POKE(0xd016); return 0; }
int C64::c64_vic_read_control2() { PEEK(0xd016); return 0; }
int C64::c64_vic_sprite_expandy(/* value */) { POKE(0xd017); return 0; }
int C64::c64_vic_write_pointer(/* value */) { POKE(0xd018); return 0; }
int C64::c64_vic_read_pointer() { PEEK(0xd018); return 0; }
int C64::c64_vic_write_interrupt_status(/* value */) { POKE(0xd019); return 0; }
int C64::c64_vic_read_interrupt_status() { PEEK(0xd019); return 0; }
int C64::c64_vic_interrupt_control(/* value */) { POKE(0xd01a); return 0; }
int C64::c64_vic_sprite_priority(/* value */) { POKE(0xd01b); return 0; }
int C64::c64_vic_sprite_multicolor_enable(/* value */) { POKE(0xd01c); return 0; }
int C64::c64_vic_sprite_expandx(/* value */) { POKE(0xd01d); return 0; }
int C64::c64_vic_sprite_collision() { PEEK(0xd01e); return 0; }
int C64::c64_vic_data_collision() { PEEK(0xd01f); return 0; }
int C64::c64_vic_border(/* value */) { POKE(0xd020); return 0; }
int C64::c64_vic_background(/* value */) { POKE(0xd021); return 0; }
int C64::c64_vic_multi1(/* value */) { POKE(0xd022); return 0; }
int C64::c64_vic_multi2(/* value */) { POKE(0xd023); return 0; }
int C64::c64_vic_multi3(/* value */) { POKE(0xd024); return 0; }
int C64::c64_vic_sprite_multicolor0(/* value */) { POKE(0xd025); return 0; }
int C64::c64_vic_sprite_multicolor1(/* value */) { POKE(0xd026); return 0; }
int C64::c64_vic_sprite0color(/* value */) { POKE(0xd027); return 0; }
int C64::c64_vic_sprite1color(/* value */) { POKE(0xd028); return 0; }
int C64::c64_vic_sprite2color(/* value */) { POKE(0xd029); return 0; }
int C64::c64_vic_sprite3color(/* value */) { POKE(0xd02a); return 0; }
int C64::c64_vic_sprite4color(/* value */) { POKE(0xd02b); return 0; }
int C64::c64_vic_sprite5color(/* value */) { POKE(0xd02c); return 0; }
int C64::c64_vic_sprite6color(/* value */) { POKE(0xd02d); return 0; }
int C64::c64_vic_sprite7color(/* value */) { POKE(0xd02e); return 0; }

int C64::c64_vic_hires_enable()
{
  need_c64_vic_hires_enable = 1;
  fprintf(out, "  jsr hires_enable\n");
  return 0;
}

int C64::c64_vic_hires_clear(/* value */)
{
  need_c64_vic_hires_clear = 1;
  fprintf(out, "  jsr hires_clear\n");

  return 0;
}

int C64::c64_vic_hires_plot(/* x, y, value */)
{
  need_c64_vic_hires_plot = 1;
  fprintf(out, "  jsr hires_plot\n");

  return 0;
}

int C64::c64_vic_make_hires_tables()
{
  need_c64_vic_make_hires_tables = 1;
  fprintf(out, "  jsr make_hires_tables\n");
  return 0;
}

int C64::c64_vic_text_enable()
{
  need_c64_vic_text_enable = 1;
  fprintf(out, "  jsr text_enable\n");
  return 0;
}

int C64::c64_vic_text_clear(/* value */)
{
  need_c64_vic_text_clear = 1;
  fprintf(out, "  jsr text_clear\n");

  return 0;
}

int C64::c64_vic_text_copy()
{
  need_c64_vic_text_copy = 1;
  fprintf(out, "  jsr text_copy\n");
  return 0;
}

int C64::c64_vic_text_plot(/* x, y, value, color */)
{
  need_c64_vic_text_plot = 1;
  fprintf(out, "  jsr text_plot\n");

  return 0;
}

int C64::c64_vic_text_read(/* x, y */)
{
  need_c64_vic_text_read = 1;
  fprintf(out, "  jsr text_read\n");

  return 0;
}

int C64::c64_vic_make_text_table()
{
  need_c64_vic_make_text_table = 1;
  fprintf(out, "  jsr make_text_table\n");
  return 0;
}

int C64::c64_vic_make_color_table()
{
  need_c64_vic_make_color_table = 1;
  fprintf(out, "  jsr make_color_table\n");
  return 0;
}

int C64::c64_vic_color_ram_clear(/* value */)
{
  need_c64_vic_color_ram_clear = 1;
  fprintf(out, "  jsr color_ram_clear\n");
  return 0;
}

int C64::c64_vic_copy_uppercase()
{
  need_c64_vic_copy_uppercase = 1;
  fprintf(out, "  jsr copy_uppercase\n");
  return 0;
}

int C64::c64_vic_copy_lowercase()
{
  need_c64_vic_copy_lowercase = 1;
  fprintf(out, "  jsr copy_lowercase\n");
  return 0;
}

void C64::insert_c64_vic_hires_enable()
{
  fprintf(out, "hires_enable:\n");
  fprintf(out, "  lda #8\n");
  fprintf(out, "  sta 0xd018\n");
  fprintf(out, "  lda 0xd011\n");
  fprintf(out, "  ora #32\n");
  fprintf(out, "  sta 0xd011\n");
  fprintf(out, "  rts\n");
}

void C64::insert_c64_vic_hires_clear()
{
  fprintf(out, "hires_clear:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "hires_clear_loop:\n");
  fprintf(out, "  sta 0xe000,y\n");
  fprintf(out, "  sta 0xe100,y\n");
  fprintf(out, "  sta 0xe200,y\n");
  fprintf(out, "  sta 0xe300,y\n");
  fprintf(out, "  sta 0xe400,y\n");
  fprintf(out, "  sta 0xe500,y\n");
  fprintf(out, "  sta 0xe600,y\n");
  fprintf(out, "  sta 0xe700,y\n");
  fprintf(out, "  sta 0xe800,y\n");
  fprintf(out, "  sta 0xe900,y\n");
  fprintf(out, "  sta 0xea00,y\n");
  fprintf(out, "  sta 0xeb00,y\n");
  fprintf(out, "  sta 0xec00,y\n");
  fprintf(out, "  sta 0xed00,y\n");
  fprintf(out, "  sta 0xee00,y\n");
  fprintf(out, "  sta 0xef00,y\n");
  fprintf(out, "  sta 0xf000,y\n");
  fprintf(out, "  sta 0xf100,y\n");
  fprintf(out, "  sta 0xf200,y\n");
  fprintf(out, "  sta 0xf300,y\n");
  fprintf(out, "  sta 0xf400,y\n");
  fprintf(out, "  sta 0xf500,y\n");
  fprintf(out, "  sta 0xf600,y\n");
  fprintf(out, "  sta 0xf700,y\n");
  fprintf(out, "  sta 0xf800,y\n");
  fprintf(out, "  sta 0xf900,y\n");
  fprintf(out, "  sta 0xfa00,y\n");
  fprintf(out, "  sta 0xfb00,y\n");
  fprintf(out, "  sta 0xfc00,y\n");
  fprintf(out, "  sta 0xfd00,y\n");
  fprintf(out, "  sta 0xfe00,y\n");
  fprintf(out, "  sta 0xff00,y\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne hires_clear_loop\n");
  fprintf(out, "  rts\n");
}

void C64::insert_c64_vic_hires_plot()
{
  fprintf(out, "hires_plot:\n");
  // value
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo,x\n");
  // y
  fprintf(out, "  inx\n");
  fprintf(out, "  ldy stack_lo,x\n");
  // address lo/hi
  fprintf(out, "  lda 0x0400,y\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda 0x0500,y\n");
  fprintf(out, "  sta address + 1\n");
  // x
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  sta result + 1\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta result + 0\n");
  // x & 7
  fprintf(out, "  and #7\n");
  fprintf(out, "  sta value1 + 0\n");
  // x / 8
  fprintf(out, "  lsr result + 1\n");
  fprintf(out, "  ror result + 0\n");
  fprintf(out, "  lsr result + 1\n");
  fprintf(out, "  ror result + 0\n");
  fprintf(out, "  lsr result + 1\n");
  fprintf(out, "  ror result + 0\n");
  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  tay\n");
  // col
  fprintf(out, "  clc\n");
  fprintf(out, "  lda 0x0600,y\n");
  fprintf(out, "  adc address + 0\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda 0x0640,y\n");
  fprintf(out, "  adc address + 1\n");
  fprintf(out, "  sta address + 1\n");
  // read byte
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  ldy value1 + 0\n");
  fprintf(out, "  ora 0x0700,y\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  sta (address),y\n");

  fprintf(out, "  rts\n");
}

void C64::insert_c64_vic_make_hires_tables()
{
  fprintf(out, "make_hires_tables:\n");
  // row tables
  fprintf(out, "  lda #0x00\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda #0xe0\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "make_hires_tables_loop1:\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda address + 0\n");
  fprintf(out, "  sta 0x0400,y\n");
  fprintf(out, "  adc #1\n");
  fprintf(out, "  sta 0x0401,y\n");
  fprintf(out, "  adc #1\n");
  fprintf(out, "  sta 0x0402,y\n");
  fprintf(out, "  adc #1\n");
  fprintf(out, "  sta 0x0403,y\n");
  fprintf(out, "  adc #1\n");
  fprintf(out, "  sta 0x0404,y\n");
  fprintf(out, "  adc #1\n");
  fprintf(out, "  sta 0x0405,y\n");
  fprintf(out, "  adc #1\n");
  fprintf(out, "  sta 0x0406,y\n");
  fprintf(out, "  adc #1\n");
  fprintf(out, "  sta 0x0407,y\n");
  fprintf(out, "  lda address + 1\n");
  fprintf(out, "  sta 0x0500,y\n");
  fprintf(out, "  sta 0x0501,y\n");
  fprintf(out, "  sta 0x0502,y\n");
  fprintf(out, "  sta 0x0503,y\n");
  fprintf(out, "  sta 0x0504,y\n");
  fprintf(out, "  sta 0x0505,y\n");
  fprintf(out, "  sta 0x0506,y\n");
  fprintf(out, "  sta 0x0507,y\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda #0x40\n");
  fprintf(out, "  adc address + 0\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda #0x01\n");
  fprintf(out, "  adc address + 1\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  bne make_hires_tables_loop1\n");

  // col table
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "make_hires_tables_loop3:\n");
  fprintf(out, "  sta 0x0600,y\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  adc #8\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  bne make_hires_tables_loop3\n");

  fprintf(out, "  ldy #40\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "make_hires_tables_loop4:\n");
  fprintf(out, "  sta 0x0640,y\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne make_hires_tables_loop4\n");

  fprintf(out, "  lda #1\n");
  fprintf(out, "  sta 0x0660\n");
  fprintf(out, "  sta 0x0661\n");
  fprintf(out, "  sta 0x0662\n");
  fprintf(out, "  sta 0x0663\n");
  fprintf(out, "  sta 0x0664\n");
  fprintf(out, "  sta 0x0665\n");
  fprintf(out, "  sta 0x0666\n");
  fprintf(out, "  sta 0x0667\n");

  // bit table
  fprintf(out, "  lda #128\n");
  fprintf(out, "  sta 0x0700\n");
  fprintf(out, "  lda #64\n");
  fprintf(out, "  sta 0x0701\n");
  fprintf(out, "  lda #32\n");
  fprintf(out, "  sta 0x0702\n");
  fprintf(out, "  lda #16\n");
  fprintf(out, "  sta 0x0703\n");
  fprintf(out, "  lda #8\n");
  fprintf(out, "  sta 0x0704\n");
  fprintf(out, "  lda #4\n");
  fprintf(out, "  sta 0x0705\n");
  fprintf(out, "  lda #2\n");
  fprintf(out, "  sta 0x0706\n");
  fprintf(out, "  lda #1\n");
  fprintf(out, "  sta 0x0707\n");
  fprintf(out, "  rts\n");
}

void C64::insert_c64_vic_text_enable()
{
  fprintf(out, "text_enable:\n");
  fprintf(out, "  lda #2\n");
  fprintf(out, "  sta 0xd018\n");
  fprintf(out, "  lda 0xd011\n");
  fprintf(out, "  and #223\n");
  fprintf(out, "  sta 0xd011\n");
  fprintf(out, "  rts\n");
}

void C64::insert_c64_vic_text_clear()
{
  fprintf(out, "text_clear:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "text_clear_loop:\n");
  fprintf(out, "  sta 0xc000,y\n");
  fprintf(out, "  sta 0xc100,y\n");
  fprintf(out, "  sta 0xc200,y\n");
  fprintf(out, "  sta 0xc2e8,y\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne text_clear_loop\n");
  fprintf(out, "  rts\n");
}

void C64::insert_c64_vic_text_copy()
{
  fprintf(out, "text_copy:\n");
  fprintf(out, "  ldy #39\n");
  fprintf(out, "text_copy_loop:\n");
  fprintf(out, "  lda 0xc000 + 40 * 0,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 0,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 1,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 1,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 2,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 2,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 3,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 3,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 4,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 4,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 5,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 5,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 6,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 6,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 7,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 7,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 8,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 8,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 9,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 9,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 10,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 10,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 11,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 11,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 12,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 12,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 13,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 13,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 14,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 14,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 15,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 15,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 16,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 16,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 17,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 17,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 18,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 18,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 19,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 19,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 20,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 20,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 21,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 21,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 22,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 22,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 23,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 23,y\n");
  fprintf(out, "  lda 0xc000 + 40 * 24,y\n");
  fprintf(out, "  sta 0xc400 + 40 * 24,y\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bmi #3\n");
  fprintf(out, "  jmp text_copy_loop\n");
  fprintf(out, "  rts\n");

/*
  // about 10% slower version but smaller
  fprintf(out, "text_copy:\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "text_copy_loop_1:\n");
  fprintf(out, "  lda 0xc000,y\n");
  fprintf(out, "  sta 0xc400,y\n");
  fprintf(out, "  lda 0xc100,y\n");
  fprintf(out, "  sta 0xc500,y\n");
  fprintf(out, "  lda 0xc200,y\n");
  fprintf(out, "  sta 0xc600,y\n");
  fprintf(out, "  lda 0xc2e8,y\n");
  fprintf(out, "  sta 0xc6e8,y\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne text_copy_loop_1\n");
  fprintf(out, "  rts\n");
*/
}

void C64::insert_c64_vic_text_plot()
{
  fprintf(out, "text_plot:\n");
  fprintf(out, "  lda stack_lo + 3,x\n"); // y
  fprintf(out, "  asl\n");
  fprintf(out, "  tay\n");
  fprintf(out, "  lda text_table + 0,y\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda text_table + 1,y\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  lda color_table + 0,y\n");
  fprintf(out, "  sta temp1 + 0\n");
  fprintf(out, "  lda color_table + 1,y\n");
  fprintf(out, "  sta temp1 + 1\n");
  fprintf(out, "  lda stack_lo + 4,x\n"); // x
  fprintf(out, "  tay\n");
  fprintf(out, "  lda stack_lo + 2,x\n"); // char
  fprintf(out, "  sta (address),y\n");
  fprintf(out, "  lda stack_lo + 1,x\n"); // color
  fprintf(out, "  sta (temp1),y\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  rts\n");
}

void C64::insert_c64_vic_text_read()
{
  fprintf(out, "text_read:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo + 0,x\n"); // y
  fprintf(out, "  asl\n");
  fprintf(out, "  tay\n");
  fprintf(out, "  lda text_table + 0,y\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda text_table + 1,y\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  lda stack_lo + 1,x\n"); // x
  fprintf(out, "  tay\n");
  fprintf(out, "  lda stack_lo + 1,x\n"); // char
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  sta stack_lo + 1,x\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta stack_hi + 1,x\n");
  fprintf(out, "  rts\n");
}

void C64::insert_c64_vic_make_text_table()
{
  fprintf(out, "make_text_table:\n");
  fprintf(out, "  lda #0x00\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda #0xc0\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "make_text_table_loop:\n");
  fprintf(out, "  lda address + 0\n");
  fprintf(out, "  sta text_table + 0,y\n");
  fprintf(out, "  lda address + 1\n");
  fprintf(out, "  sta text_table + 1,y\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda address + 0\n");
  fprintf(out, "  adc #40\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda address + 1\n");
  fprintf(out, "  adc #0\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  cpy #50\n");
  fprintf(out, "  bmi make_text_table_loop\n");
  fprintf(out, "  rts\n");
}

void C64::insert_c64_vic_make_color_table()
{
  fprintf(out, "make_color_table:\n");
  fprintf(out, "  lda #0x00\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda #0xd8\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "make_color_table_loop:\n");
  fprintf(out, "  lda address + 0\n");
  fprintf(out, "  sta color_table + 0,y\n");
  fprintf(out, "  lda address + 1\n");
  fprintf(out, "  sta color_table + 1,y\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda address + 0\n");
  fprintf(out, "  adc #40\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda address + 1\n");
  fprintf(out, "  adc #0\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  cpy #50\n");
  fprintf(out, "  bmi make_color_table_loop\n");
  fprintf(out, "  rts\n");
}

void C64::insert_c64_vic_color_ram_clear()
{
  fprintf(out, "color_ram_clear:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "color_ram_clear_loop:\n");
  fprintf(out, "  sta 0xd800,y\n");
  fprintf(out, "  sta 0xd900,y\n");
  fprintf(out, "  sta 0xda00,y\n");
  fprintf(out, "  sta 0xdae8,y\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne color_ram_clear_loop\n");
  fprintf(out, "  rts\n");
}

void C64::insert_c64_vic_copy_uppercase()
{
  fprintf(out, "copy_uppercase:\n");
  fprintf(out, "  lda #50\n");
  fprintf(out, "  sta 0x0001\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "copy_uppercase_loop:\n");
  fprintf(out, "  lda 0xd000,y\n");
  fprintf(out, "  sta 0xc800,y\n");
  fprintf(out, "  lda 0xd100,y\n");
  fprintf(out, "  sta 0xc900,y\n");
  fprintf(out, "  lda 0xd200,y\n");
  fprintf(out, "  sta 0xca00,y\n");
  fprintf(out, "  lda 0xd300,y\n");
  fprintf(out, "  sta 0xcb00,y\n");
  fprintf(out, "  lda 0xd400,y\n");
  fprintf(out, "  sta 0xcc00,y\n");
  fprintf(out, "  lda 0xd500,y\n");
  fprintf(out, "  sta 0xcd00,y\n");
  fprintf(out, "  lda 0xd600,y\n");
  fprintf(out, "  sta 0xce00,y\n");
  fprintf(out, "  lda 0xd700,y\n");
  fprintf(out, "  sta 0xcf00,y\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne copy_uppercase_loop\n");
  fprintf(out, "  lda #53\n");
  fprintf(out, "  sta 0x0001\n");
  fprintf(out, "  rts\n");
}

void C64::insert_c64_vic_copy_lowercase()
{
  fprintf(out, "copy_lowercase:\n");
  fprintf(out, "  lda #50\n");
  fprintf(out, "  sta 0x0001\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "copy_lowercase_loop:\n");
  fprintf(out, "  lda 0xd800,y\n");
  fprintf(out, "  sta 0xc800,y\n");
  fprintf(out, "  lda 0xd900,y\n");
  fprintf(out, "  sta 0xc900,y\n");
  fprintf(out, "  lda 0xda00,y\n");
  fprintf(out, "  sta 0xca00,y\n");
  fprintf(out, "  lda 0xdb00,y\n");
  fprintf(out, "  sta 0xcb00,y\n");
  fprintf(out, "  lda 0xdc00,y\n");
  fprintf(out, "  sta 0xcc00,y\n");
  fprintf(out, "  lda 0xdd00,y\n");
  fprintf(out, "  sta 0xcd00,y\n");
  fprintf(out, "  lda 0xde00,y\n");
  fprintf(out, "  sta 0xce00,y\n");
  fprintf(out, "  lda 0xdf00,y\n");
  fprintf(out, "  sta 0xcf00,y\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne copy_lowercase_loop\n");
  fprintf(out, "  lda #53\n");
  fprintf(out, "  sta 0x0001\n");
  fprintf(out, "  rts\n");
}

#if 0
void C64::close()
{
}
#endif

