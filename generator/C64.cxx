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
  need_c64_vic_text_ascii_plot(0),
  need_c64_vic_text_read(0),
  need_c64_vic_text_string(0),
  need_c64_vic_text_fill(0),
  need_c64_vic_text_paint(0),
  need_c64_vic_text_scroll_left(0),
  need_c64_vic_text_scroll_right(0),
  need_c64_vic_text_scroll_up(0),
  need_c64_vic_text_scroll_down(0),
  need_c64_vic_text_smooth_scroll(0),
  need_c64_vic_make_color_table(0),
  need_c64_vic_color_ram_clear(0),
  need_c64_vic_copy_uppercase(0),
  need_c64_vic_copy_lowercase(0),
  need_c64_vic_copy_data_from_array(0),
  need_c64_timer_interrupt(0),
  need_c64_keyboard(0)
{
  start_org = 0x07ff;

  // Java stack pointer location. The default is zero page (32 variables max).
  // Call Grinder.largeJavaStack() at the start of a program to move it
  // to 0x200/0x300 (128 variables max, but slower).

  java_stack_lo = 0x80;
  java_stack_hi = 0xc0;

  saved_vars = 0x100;
  var_start = 0x02;

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
  if (need_c64_vic_text_ascii_plot) { insert_c64_vic_text_ascii_plot(); }
  if (need_c64_vic_text_read) { insert_c64_vic_text_read(); }
  if (need_c64_vic_text_string) { insert_c64_vic_text_string(); }
  if (need_c64_vic_text_fill) { insert_c64_vic_text_fill(); }
  if (need_c64_vic_text_paint) { insert_c64_vic_text_paint(); }
  if (need_c64_vic_text_scroll_left) { insert_c64_vic_text_scroll_left(); }
  if (need_c64_vic_text_scroll_right) { insert_c64_vic_text_scroll_right(); }
  if (need_c64_vic_text_scroll_up) { insert_c64_vic_text_scroll_up(); }
  if (need_c64_vic_text_scroll_down) { insert_c64_vic_text_scroll_down(); }
  if (need_c64_vic_text_smooth_scroll) { insert_c64_vic_text_smooth_scroll(); }
  if (need_c64_vic_make_text_table) { insert_c64_vic_make_text_table(); }
  if (need_c64_vic_make_color_table) { insert_c64_vic_make_color_table(); }
  if (need_c64_vic_color_ram_clear) { insert_c64_vic_color_ram_clear(); }
  if (need_c64_vic_copy_uppercase) { insert_c64_vic_copy_uppercase(); }
  if (need_c64_vic_copy_lowercase) { insert_c64_vic_copy_lowercase(); }
  if (need_c64_vic_copy_data_from_array) { insert_c64_vic_copy_data_from_array(); }
  if (need_c64_keyboard) { insert_c64_keyboard(); }

  insert_c64_sprite_interrupt();
}

int C64::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".6502\n");

  // heap
  fprintf(out, "ram_start equ 0x%04x\n", ram_start);
  fprintf(out, "heap_ptr equ ram_start\n");

  // java stack
  fprintf(out, ".set stack_lo=0x%04x\n", java_stack_lo);
  fprintf(out, ".set stack_hi=0x%04x\n", java_stack_hi);

  // registers and internal variables are saved here during interrupts
  fprintf(out, "saved_vars equ 0x%04x\n", saved_vars);
  fprintf(out, "var_start equ 0x%04x\n", var_start);

  // for indirection (2 bytes)
  fprintf(out, "address equ 0x02\n");

  // points to locals
  fprintf(out, "locals equ 0x04\n");

  // temp variables
  fprintf(out, "result equ 0x06\n");
  fprintf(out, "remainder equ 0x08\n");
  fprintf(out, "length equ 0x0a\n");
  fprintf(out, "value1 equ 0x0c\n");
  fprintf(out, "value2 equ 0x0e\n");
  fprintf(out, "value3 equ 0x10\n");
  fprintf(out, "value4 equ 0x12\n");

  // text/color tables
  fprintf(out, "text_table equ 0x18\n");
  fprintf(out, "color_table equ 0x4a\n");

  // keyboard buffer
  fprintf(out, "key_table equ 0x708\n");

  // sprite positions
  fprintf(out, "sprite_pos_x equ 0x748\n");
  fprintf(out, "sprite_pos_y equ 0x750\n");
  fprintf(out, "sprite_pos_x_msb equ 0x758\n");

  // timer related
  fprintf(out, "timer_enable equ 0x75a\n");

  // smooth scroll related
  fprintf(out, "scroll_x equ 0x760\n");
  fprintf(out, "scroll_y equ 0x762\n");
  fprintf(out, "scroll_w equ 0x764\n");
  fprintf(out, "scroll_x_fine equ 0x766\n");
  fprintf(out, "scroll_y_fine equ 0x768\n");
  fprintf(out, "scroll_frame equ 0x76a\n");
  fprintf(out, "scroll_src_0 equ 0x76c\n");
  fprintf(out, "scroll_src_1 equ 0x76e\n");
  fprintf(out, "scroll_dest_0 equ 0x770\n");
  fprintf(out, "scroll_dest_1 equ 0x772\n");
  fprintf(out, "scroll_page equ 0x774\n");

  // basic loader
  fprintf(out, ".org 0x%04x\n", start_org);
  fprintf(out, 
    "dw 0x0801\n"
    "dw start\n"
    "dw 2013\n"
    "db 0x9e\n"
    "db (((start / 1000) %% 10) + 0x30)\n"
    "db (((start / 100) %% 10) + 0x30)\n"
    "db (((start / 10) %% 10) + 0x30)\n"
    "db (((start / 1) %% 10) + 0x30)\n"
    "db ':'\n"
    "db 0x8f\n"
    "db \" JAVA-GRINDER \"\n"
    "db 0\n"
    "dw 0\n\n");

  fprintf(out, "start:\n");
  fprintf(out, "  sei\n");
  fprintf(out, "  cld\n");

  // reset processor stack
  fprintf(out, "  ldx #0xff\n");
  fprintf(out, "  txs\n");

  // reset java stack pointer
  fprintf(out, "  ldx #0x3f\n");  // for 0x80/0xc0

  // switch VIC-II to bank 0
  fprintf(out, "  lda #4\n");
  fprintf(out, "  sta 0xdd00\n");

  // put text screen at 0xc400 and charset at 0xc800
  fprintf(out, "  lda #0x02\n");
  fprintf(out, "  sta 0xd018\n");

  // turn off ROM chips
  fprintf(out, "  lda #53\n");
  fprintf(out, "  sta 0x0001\n");

  // reset scroll
  fprintf(out, "  lda #3\n");
  fprintf(out, "  sta scroll_frame\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta scroll_page\n");

  // sprite interrupt
  fprintf(out, "  ldy #16\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "sprite_pos_reset_loop:\n");
  fprintf(out, "  sta sprite_pos_x,y\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bpl sprite_pos_reset_loop\n");

  fprintf(out, "  lda #0x7f\n");
  fprintf(out, "  sta 0xdc0d\n");
  fprintf(out, "  sta 0xdd0d\n");

  fprintf(out, "  lda 0xdc0d\n");
  fprintf(out, "  lda 0xdd0d\n");

  fprintf(out, "  lda #1\n");
  fprintf(out, "  sta 0xd01a\n");

  fprintf(out, "  lda 0xd011\n");
  fprintf(out, "  and #0x7f\n");
  fprintf(out, "  sta 0xd011\n");

  fprintf(out, "  lda #251\n");
  fprintf(out, "  sta 0xd012\n");

  fprintf(out, "  lda #sprite_interrupt & 0xff\n");
  fprintf(out, "  sta 0xfffe\n");
  fprintf(out, "  lda #sprite_interrupt >> 8\n");
  fprintf(out, "  sta 0xffff\n");

  // timer
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta timer_enable\n");
  fprintf(out, "  cli\n");
  fprintf(out, "\n");

  return 0;
}

int C64::c64_sid_frequency1(/* value */)
{
  fprintf(out, 
    "  inx\n"
    "  lda stack_hi,x\n"
    "  sta 0xd401\n"
    "  lda stack_lo,x\n"
    "  sta 0xd400\n");

  return 0;
}

int C64::c64_sid_pulseWidth1(/* value */)
{
  fprintf(out, 
    "  inx\n"
    "  lda stack_hi,x\n"
    "  sta 0xd403\n"
    "  lda stack_lo,x\n"
    "  sta 0xd402\n");

  return 0;
}

int C64::c64_sid_waveform1(/* value */) { POKE(0xd404); return 0; }

int C64::c64_sid_adsr1(/* value */)
{
  fprintf(out, 
    "  inx\n"
    "  lda stack_hi,x\n"
    "  sta 0xd406\n"
    "  lda stack_lo,x\n"
    "  sta 0xd405\n");

  return 0;
}

int C64::c64_sid_frequency2(/* value */)
{
  fprintf(out, 
    "  inx\n"
    "  lda stack_hi,x\n"
    "  sta 0xd408\n"
    "  lda stack_lo,x\n"
    "  sta 0xd407\n");

  return 0;
}

int C64::c64_sid_pulseWidth2(/* value */)
{
  fprintf(out, 
    "  inx\n"
    "  lda stack_hi,x\n"
    "  sta 0xd40a\n"
    "  lda stack_lo,x\n"
    "  sta 0xd409\n");

  return 0;
}

int C64::c64_sid_waveform2(/* value */) { POKE(0xd40b); return 0; }

int C64::c64_sid_adsr2(/* value */)
{
  fprintf(out, 
    "  inx\n"
    "  lda stack_hi,x\n"
    "  sta 0xd40d\n"
    "  lda stack_lo,x\n"
    "  sta 0xd40c\n");

  return 0;
}

int C64::c64_sid_frequency3(/* value */)
{
  fprintf(out, 
    "  inx\n"
    "  lda stack_hi,x\n"
    "  sta 0xd40f\n"
    "  lda stack_lo,x\n"
    "  sta 0xd40e\n");

  return 0;
}

int C64::c64_sid_pulseWidth3(/* value */)
{
  fprintf(out, 
    "  inx\n"
    "  lda stack_hi,x\n"
    "  sta 0xd411\n"
    "  lda stack_lo,x\n"
    "  sta 0xd410\n");

  return 0;
}

int C64::c64_sid_waveform3(/* value */) { POKE(0xd412); return 0; }

int C64::c64_sid_adsr3(/* value */)
{
  fprintf(out, 
    "  inx\n"
    "  lda stack_hi,x\n"
    "  sta 0xd414\n"
    "  lda stack_lo,x\n"
    "  sta 0xd413\n");

  return 0;
}

int C64::c64_sid_filterCutoff(/* value */)
{
  fprintf(out, 
    "  inx\n"
    "  lda stack_hi,x\n"
    "  sta 0xd416\n"
    "  lda stack_lo,x\n"
    "  sta 0xd415\n");

  return 0;
}

int C64::c64_sid_filterResonance(/* value */) { POKE(0xd417); return 0; }
int C64::c64_sid_volume(/* value */) { POKE(0xd418); return 0; }
int C64::c64_sid_oscillator3(/* value */) { POKE(0xd41b); return 0; }
int C64::c64_sid_envelope3(/* value */) { POKE(0xd41c); return 0; }

//FIXME this should be a subroutine
int C64::c64_sid_clear()
{
  fprintf(out, 
    "; sid clear\n"
    "  ldy #0x1c\n"
    "  lda #0\n"
    "  sta 0xd400,y\n"
    "  dey\n"
    "  bpl #-6\n");

  return 0;
}

int C64::c64_vic_sprite0pos(/* x, y */)
{
  fprintf(out, 
    "; sprite0pos\n"
    "  inx\n"
    "  lda stack_lo,x\n"
    "  sta sprite_pos_y + 0\n"

    "  lda sprite_pos_x_msb\n"
    "  and #255 - 1\n"
    "  sta sprite_pos_x_msb\n"

    "  inx\n"
    "  lda stack_hi,x\n"
    "  beq #8\n"
    "  lda sprite_pos_x_msb\n"
    "  ora #1\n"
    "  sta sprite_pos_x_msb\n"
    "  lda stack_lo,x\n"
    "  sta sprite_pos_x + 0\n");

  return 0;
}

int C64::c64_vic_sprite1pos(/* x, y */)
{
  fprintf(out, 
    "; sprite1pos\n"
    "  inx\n"
    "  lda stack_lo,x\n"
    "  sta sprite_pos_y + 1\n"

    "  lda sprite_pos_x_msb\n"
    "  and #255 - 2\n"
    "  sta sprite_pos_x_msb\n"

    "  inx\n"
    "  lda stack_hi,x\n"
    "  beq #8\n"
    "  lda sprite_pos_x_msb\n"
    "  ora #2\n"
    "  sta sprite_pos_x_msb\n"
    "  lda stack_lo,x\n"
    "  sta sprite_pos_x + 1\n");

  return 0;
}

int C64::c64_vic_sprite2pos(/* x, y */)
{
  fprintf(out, 
    "; sprite2pos\n"
    "  inx\n"
    "  lda stack_lo,x\n"
    "  sta sprite_pos_y + 2\n"

    "  lda sprite_pos_x_msb\n"
    "  and #255 - 4\n"
    "  sta sprite_pos_x_msb\n"

    "  inx\n"
    "  lda stack_hi,x\n"
    "  beq #8\n"
    "  lda sprite_pos_x_msb\n"
    "  ora #4\n"
    "  sta sprite_pos_x_msb\n"
    "  lda stack_lo,x\n"
    "  sta sprite_pos_x + 2\n");

  return 0;
}

int C64::c64_vic_sprite3pos(/* x, y */)
{
  fprintf(out, 
    "; sprite3pos\n"
    "  inx\n"
    "  lda stack_lo,x\n"
    "  sta sprite_pos_y + 3\n"

    "  lda sprite_pos_x_msb\n"
    "  and #255 - 8\n"
    "  sta sprite_pos_x_msb\n"

    "  inx\n"
    "  lda stack_hi,x\n"
    "  beq #8\n"
    "  lda sprite_pos_x_msb\n"
    "  ora #8\n"
    "  sta sprite_pos_x_msb\n"
    "  lda stack_lo,x\n"
    "  sta sprite_pos_x + 3\n");

  return 0;
}

int C64::c64_vic_sprite4pos(/* x, y */)
{
  fprintf(out, 
    "; sprite4pos\n"
    "  inx\n"
    "  lda stack_lo,x\n"
    "  sta sprite_pos_y + 4\n"

    "  lda sprite_pos_x_msb\n"
    "  and #255 - 16\n"
    "  sta sprite_pos_x_msb\n"

    "  inx\n"
    "  lda stack_hi,x\n"
    "  beq #8\n"
    "  lda sprite_pos_x_msb\n"
    "  ora #16\n"
    "  sta sprite_pos_x_msb\n"
    "  lda stack_lo,x\n"
    "  sta sprite_pos_x + 4\n");

  return 0;
}

int C64::c64_vic_sprite5pos(/* x, y */)
{
  fprintf(out, 
    "; sprite5pos\n"
    "  inx\n"
    "  lda stack_lo,x\n"
    "  sta sprite_pos_y + 5\n"

    "  lda sprite_pos_x_msb\n"
    "  and #255 - 32\n"
    "  sta sprite_pos_x_msb\n"

    "  inx\n"
    "  lda stack_hi,x\n"
    "  beq #8\n"
    "  lda sprite_pos_x_msb\n"
    "  ora #32\n"
    "  sta sprite_pos_x_msb\n"
    "  lda stack_lo,x\n"
    "  sta sprite_pos_x + 5\n");

  return 0;
}

int C64::c64_vic_sprite6pos(/* x, y */)
{
  fprintf(out, 
    "; sprite6pos\n"
    "  inx\n"
    "  lda stack_lo,x\n"
    "  sta sprite_pos_y + 6\n"

    "  lda sprite_pos_x_msb\n"
    "  and #255 - 64\n"
    "  sta sprite_pos_x_msb\n"

    "  inx\n"
    "  lda stack_hi,x\n"
    "  beq #8\n"
    "  lda sprite_pos_x_msb\n"
    "  ora #64\n"
    "  sta sprite_pos_x_msb\n"
    "  lda stack_lo,x\n"
    "  sta sprite_pos_x + 6\n");

  return 0;
}

int C64::c64_vic_sprite7pos(/* x, y */)
{
  fprintf(out, 
    "; sprite7pos\n"
    "  inx\n"
    "  lda stack_lo,x\n"
    "  sta sprite_pos_y + 7\n"

    "  lda sprite_pos_x_msb\n"
    "  and #255 - 128\n"
    "  sta sprite_pos_x_msb\n"

    "  inx\n"
    "  lda stack_hi,x\n"
    "  beq #8\n"
    "  lda sprite_pos_x_msb\n"
    "  ora #128\n"
    "  sta sprite_pos_x_msb\n"
    "  lda stack_lo,x\n"
    "  sta sprite_pos_x + 7\n");

  return 0;
}

int C64::c64_vic_writeControl1(/* value */) { POKE(0xd011); return 0; }
int C64::c64_vic_readControl1() { PEEK(0xd011); return 0; }

int C64::c64_vic_waitRaster(/* line */)
{
  fprintf(out, 
    "; waitRaster\n"
    "  inx\n"
    "  lda stack_lo,x\n"
    "  cmp 0xd012\n"
    "  bne #-5\n");

  return 0;
}

int C64::c64_vic_spriteEnable(/* value */) { POKE(0xd015); return 0; }
int C64::c64_vic_writeControl2(/* value */) { POKE(0xd016); return 0; }
int C64::c64_vic_readControl2() { PEEK(0xd016); return 0; }
int C64::c64_vic_spriteExpandY(/* value */) { POKE(0xd017); return 0; }
int C64::c64_vic_writePointer(/* value */) { POKE(0xd018); return 0; }
int C64::c64_vic_readPointer() { PEEK(0xd018); return 0; }
int C64::c64_vic_writeInterruptStatus(/* value */) { POKE(0xd019); return 0; }
int C64::c64_vic_readInterruptStatus() { PEEK(0xd019); return 0; }
int C64::c64_vic_interruptControl(/* value */) { POKE(0xd01a); return 0; }
int C64::c64_vic_spritePriority(/* value */) { POKE(0xd01b); return 0; }
int C64::c64_vic_spriteMulticolorEnable(/* value */) { POKE(0xd01c); return 0; }
int C64::c64_vic_spriteExpandX(/* value */) { POKE(0xd01d); return 0; }
int C64::c64_vic_spriteCollision() { PEEK(0xd01e); return 0; }
int C64::c64_vic_dataCollision() { PEEK(0xd01f); return 0; }
int C64::c64_vic_border(/* value */) { POKE(0xd020); return 0; }
int C64::c64_vic_background(/* value */) { POKE(0xd021); return 0; }
int C64::c64_vic_multicolor1(/* value */) { POKE(0xd022); return 0; }
int C64::c64_vic_multicolor2(/* value */) { POKE(0xd023); return 0; }
int C64::c64_vic_multicolor3(/* value */) { POKE(0xd024); return 0; }
int C64::c64_vic_spriteMulticolor0(/* value */) { POKE(0xd025); return 0; }
int C64::c64_vic_spriteMulticolor1(/* value */) { POKE(0xd026); return 0; }
int C64::c64_vic_sprite0color(/* value */) { POKE(0xd027); return 0; }
int C64::c64_vic_sprite1color(/* value */) { POKE(0xd028); return 0; }
int C64::c64_vic_sprite2color(/* value */) { POKE(0xd029); return 0; }
int C64::c64_vic_sprite3color(/* value */) { POKE(0xd02a); return 0; }
int C64::c64_vic_sprite4color(/* value */) { POKE(0xd02b); return 0; }
int C64::c64_vic_sprite5color(/* value */) { POKE(0xd02c); return 0; }
int C64::c64_vic_sprite6color(/* value */) { POKE(0xd02d); return 0; }
int C64::c64_vic_sprite7color(/* value */) { POKE(0xd02e); return 0; }

int C64::c64_vic_hiresEnable()
{
  need_c64_vic_hires_enable = 1;
  fprintf(out, "  jsr hires_enable\n");
  return 0;
}

int C64::c64_vic_hiresClear(/* value */)
{
  need_c64_vic_hires_clear = 1;
  fprintf(out, "  jsr hires_clear\n");

  return 0;
}

int C64::c64_vic_hiresPlot(/* x, y, value */)
{
  need_c64_vic_hires_plot = 1;
  fprintf(out, "  jsr hires_plot\n");

  return 0;
}

int C64::c64_vic_makeHiresTables()
{
  need_c64_vic_make_hires_tables = 1;
  fprintf(out, "  jsr make_hires_tables\n");
  return 0;
}

int C64::c64_vic_textEnable(/* screen */)
{
  need_c64_vic_text_enable = 1;
  fprintf(out, "  jsr text_enable\n");
  return 0;
}

int C64::c64_vic_textClear(/* value */)
{
  need_c64_vic_text_clear = 1;
  fprintf(out, "  jsr text_clear\n");

  return 0;
}

int C64::c64_vic_textCopy()
{
  need_c64_vic_text_copy = 1;
  fprintf(out, "  jsr text_copy\n");
  return 0;
}

int C64::c64_vic_textPlot(/* x, y, value, color */)
{
  need_c64_vic_text_plot = 1;
  fprintf(out, "  jsr text_plot\n");

  return 0;
}

int C64::c64_vic_textAsciiPlot(/* x, y, value, color */)
{
  need_c64_vic_text_ascii_plot = 1;
  fprintf(out, "  jsr text_ascii_plot\n");

  return 0;
}

int C64::c64_vic_textRead(/* x, y */)
{
  need_c64_vic_text_read = 1;
  fprintf(out, "  jsr text_read\n");

  return 0;
}

int C64::c64_vic_textString(/* x, y, string, color */)
{
  need_c64_vic_text_string = 1;
  fprintf(out, "  jsr text_string\n");

  return 0;
}

int C64::c64_vic_textFill(/* x, y, w, h, value */)
{
  need_c64_vic_text_fill = 1;
  fprintf(out, "  jsr text_fill\n");

  return 0;
}

int C64::c64_vic_textPaint(/* x, y, w, h, value */)
{
  need_c64_vic_text_paint = 1;
  fprintf(out, "  jsr text_paint\n");

  return 0;
}

int C64::c64_vic_textScrollLeft()
{
  need_c64_vic_text_scroll_left = 1;
  fprintf(out, "  jsr text_scroll_left\n");

  return 0;
}

int C64::c64_vic_textScrollRight()
{
  need_c64_vic_text_scroll_right = 1;
  fprintf(out, "  jsr text_scroll_right\n");

  return 0;
}

int C64::c64_vic_textScrollUp()
{
  need_c64_vic_text_scroll_up = 1;
  fprintf(out, "  jsr text_scroll_up\n");

  return 0;
}

int C64::c64_vic_textScrollDown()
{
  need_c64_vic_text_scroll_down = 1;
  fprintf(out, "  jsr text_scroll_down\n");

  return 0;
}

int C64::c64_vic_textSmoothScroll()
{
  need_c64_vic_text_smooth_scroll = 1;
  fprintf(out, "  jsr text_smooth_scroll\n");

  return 0;
}

int C64::c64_vic_makeTextTable()
{
  need_c64_vic_make_text_table = 1;
  fprintf(out, "  jsr make_text_table\n");
  return 0;
}

int C64::c64_vic_makeColorTable()
{
  need_c64_vic_make_color_table = 1;
  fprintf(out, "  jsr make_color_table\n");
  return 0;
}

int C64::c64_vic_colorRamClear(/* value */)
{
  need_c64_vic_color_ram_clear = 1;
  fprintf(out, "  jsr color_ram_clear\n");
  return 0;
}

int C64::c64_vic_copyUppercase()
{
  need_c64_vic_copy_uppercase = 1;
  fprintf(out, "  jsr copy_uppercase\n");
  return 0;
}

int C64::c64_vic_copyLowercase()
{
  need_c64_vic_copy_lowercase = 1;
  fprintf(out, "  jsr copy_lowercase\n");
  return 0;
}

int C64::c64_vic_copyDataFromArray()
{
  need_c64_vic_copy_data_from_array = 1;
  fprintf(out, "  jsr copy_data_from_array\n");
  return 0;
}

int C64::timer_setInterval_II()
{
  fprintf(out, "; timer_setInterval\n");

  // timer B
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xdd06\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  sta 0xdd07\n");

  // timer A
  fprintf(out, "  inx\n");
  fprintf(out, "  lda stack_lo,x\n");
  fprintf(out, "  sta 0xdd04\n");
  fprintf(out, "  lda stack_hi,x\n");
  fprintf(out, "  sta 0xdd05\n");

  return 0;
}

int C64::timer_setInterval_II(int cycles, int divider)
{
  return -1;
}

int C64::timer_setListener_Z()
{
  need_c64_timer_interrupt = true;
  return -1;
}

int C64::timer_setListener_Z(int const_value)
{
  fprintf(out, "; timer_setListener\n");

  // const_value = turn interrupts on/off
  if (const_value != 0)
  {
    need_c64_timer_interrupt = true;

    // vector
    fprintf(out, "  lda #0x7f\n");
    fprintf(out, "  sta 0xdd0d\n");
    fprintf(out, "  lda 0xdd0d\n");

    fprintf(out, "  lda #timerInterrupt & 0xff\n");
    fprintf(out, "  sta 0xfffa\n");
    fprintf(out, "  lda #timerInterrupt >> 8\n");
    fprintf(out, "  sta 0xfffb\n");

    fprintf(out, "  lda #10000010b\n");
    fprintf(out, "  sta 0xdd0d\n");

    // 0 - timer on/off (start off)
    // 4 - latch
    // 5-6 - watch for timer A underflow

    // start timer A
    fprintf(out, "  lda #00000001b\n");
    fprintf(out, "  sta 0xdd0e\n");

    // start timer B
    fprintf(out, "  lda #01000001b\n");
    fprintf(out, "  sta 0xdd0f\n");

    fprintf(out, "  lda #1\n");
    fprintf(out, "  sta timer_enable\n");
  }
    else
  { 
    fprintf(out, "  lda #0\n");
    fprintf(out, "  sta timer_enable\n");

    // stop timer A
    fprintf(out, "  lda #00000000b\n");
    fprintf(out, "  sta 0xdd0e\n");

    // stop timer B
    fprintf(out, "  lda #01000000b\n");
    fprintf(out, "  sta 0xdd0f\n");
  }

  return 0;
}

int C64::timer_getValue()
{
  return -1;
}

int C64::timer_setValue_I()
{
  return -1;
}

int C64::timer_setValue_I(int const_value)
{
  return -1;
}

int C64::joystick_isRight()
{
  return -1;
}

int C64::joystick_isRight_I(int index)
{
  fprintf(out, "; joystick_isRight\n");

  if (index == 0)
  {
    fprintf(out, 
      "  lda 0xdc01\n"
      "  and #8\n"
      "  eor #8\n"
      "  sta stack_lo,x\n"
      "  lda #0\n"
      "  sta stack_hi,x\n"
      "  dex\n");
  }
    else
  {
    fprintf(out, 
      "  lda 0xdc00\n"
      "  and #8\n"
      "  eor #8\n"
      "  sta stack_lo,x\n"
      "  lda #0\n"
      "  sta stack_hi,x\n"
      "  dex\n");
  }

  return 0;
}

int C64::joystick_isLeft()
{
  return -1;
}

int C64::joystick_isLeft_I(int index)
{
  fprintf(out, "; joystick_isLeft\n");

  if (index == 0)
  {
    fprintf(out, 
      "  lda 0xdc01\n"
      "  and #4\n"
      "  eor #4\n"
      "  sta stack_lo,x\n"
      "  lda #0\n"
      "  sta stack_hi,x\n"
      "  dex\n");
  }
    else
  {
    fprintf(out, 
      "  lda 0xdc00\n"
      "  and #4\n"
      "  eor #4\n"
      "  sta stack_lo,x\n"
      "  lda #0\n"
      "  sta stack_hi,x\n"
      "  dex\n");
  }

  return 0;
}

int C64::joystick_isDown()
{
  return -1;
}

int C64::joystick_isDown_I(int index)
{
  fprintf(out, "; joystick_isDown\n");

  if (index == 0)
  {
    fprintf(out, 
      "  lda 0xdc01\n"
      "  and #2\n"
      "  eor #2\n"
      "  sta stack_lo,x\n"
      "  lda #0\n"
      "  sta stack_hi,x\n"
      "  dex\n");
  }
    else
  {
    fprintf(out, 
      "  lda 0xdc00\n"
      "  and #2\n"
      "  eor #2\n"
      "  sta stack_lo,x\n"
      "  lda #0\n"
      "  sta stack_hi,x\n"
      "  dex\n");
  }

  return 0;
}

int C64::joystick_isUp()
{
  return -1;
}

int C64::joystick_isUp_I(int index)
{
  fprintf(out, "; joystick_isUp\n");

  if (index == 0)
  {
    fprintf(out, 
      "  lda 0xdc01\n"
      "  and #1\n"
      "  eor #1\n"
      "  sta stack_lo,x\n"
      "  lda #0\n"
      "  sta stack_hi,x\n"
      "  dex\n");
  }
    else
  {
    fprintf(out, 
      "  lda 0xdc00\n"
      "  and #1\n"
      "  eor #1\n"
      "  sta stack_lo,x\n"
      "  lda #0\n"
      "  sta stack_hi,x\n"
      "  dex\n");
  }

  return 0;
}

int C64::joystick_isButtonDown_0()
{
  return -1;
}

int C64::joystick_isButtonDown_0_I(int index)
{
  fprintf(out, "; joystick_isButtonDown_0\n");

  if (index == 0)
  {
    fprintf(out, 
      "  lda 0xdc01\n"
      "  and #16\n"
      "  eor #16\n"
      "  sta stack_lo,x\n"
      "  lda #0\n"
      "  sta stack_hi,x\n"
      "  dex\n");
  }
    else
  {
    fprintf(out, 
      "  lda 0xdc00\n"
      "  and #16\n"
      "  eor #16\n"
      "  sta stack_lo,x\n"
      "  lda #0\n"
      "  sta stack_hi,x\n"
      "  dex\n");
  }

  return 0;
}

int C64::keyboard_isKeyPressed_I()
{
  need_c64_keyboard = true;

//  fprintf(out, "; keyboard_isKeyPressed_I\n");

  return -1;
}

int C64::keyboard_isKeyPressed_I(int index)
{
  need_c64_keyboard = true;

//  fprintf(out, "; keyboard_isKeyPressed_I (const)\n");

  return -1;
}

int C64::keyboard_currentKeyPressed()
{
  need_c64_keyboard = true;

  fprintf(out, "  jsr keyboard_current_key_pressed\n");

  return 0;
}

int C64::grinder_largeJavaStack()
{
  fprintf(out, 
    "; grinder_largeJavaStack\n"
    ".set stack_lo=0x200\n"
    ".set stack_hi=0x300\n"
    "  ldx #0xff\n"
    "  stx locals\n"
    "  txa\n"
    "  sec\n"
    "  sbc #0x01\n"
    "  tax\n");

  return 0;
}

int C64::return_void(int local_count)
{
  // acknowledge NMI timer interrupt
  fprintf(out, "  lda timer_enable\n");
  fprintf(out, "  beq #3\n");
  fprintf(out, "  lda 0xdd0d\n");

  return M6502::return_void(local_count);
}

// subroutines
void C64::insert_c64_vic_hires_enable()
{
  fprintf(out, 
    "hires_enable:\n"
    "  lda #8\n"
    "  sta 0xd018\n"
    "  lda 0xd011\n"
    "  ora #32\n"
    "  sta 0xd011\n"
    "  inx\n"
    "  lda stack_lo,x\n"
    "  bne hires_enable_multicolor\n"
    "  rts\n"
    "hires_enable_multicolor:\n"
    "  lda 0xd016\n"
    "  ora #16\n"
    "  sta 0xd016\n"
    "  rts\n");
}

void C64::insert_c64_vic_hires_clear()
{
  fprintf(out, 
    "hires_clear:\n"
    "  inx\n"
    "  lda stack_lo,x\n"
    "  ldy #0\n"
    "hires_clear_loop:\n"
    "  sta 0xe000,y\n"
    "  sta 0xe100,y\n"
    "  sta 0xe200,y\n"
    "  sta 0xe300,y\n"
    "  sta 0xe400,y\n"
    "  sta 0xe500,y\n"
    "  sta 0xe600,y\n"
    "  sta 0xe700,y\n"
    "  sta 0xe800,y\n"
    "  sta 0xe900,y\n"
    "  sta 0xea00,y\n"
    "  sta 0xeb00,y\n"
    "  sta 0xec00,y\n"
    "  sta 0xed00,y\n"
    "  sta 0xee00,y\n"
    "  sta 0xef00,y\n"
    "  sta 0xf000,y\n"
    "  sta 0xf100,y\n"
    "  sta 0xf200,y\n"
    "  sta 0xf300,y\n"
    "  sta 0xf400,y\n"
    "  sta 0xf500,y\n"
    "  sta 0xf600,y\n"
    "  sta 0xf700,y\n"
    "  sta 0xf800,y\n"
    "  sta 0xf900,y\n"
    "  sta 0xfa00,y\n"
    "  sta 0xfb00,y\n"
    "  sta 0xfc00,y\n"
    "  sta 0xfd00,y\n"
    "  sta 0xfe00,y\n"
    "  sta 0xfe40,y\n"
//    "  sta 0xff00,y\n"
    "  dey\n"
    "  bne hires_clear_loop\n"
    "  rts\n");
}

void C64::insert_c64_vic_hires_plot()
{
  fprintf(out, 
    "hires_plot:\n"
    // value
    "  inx\n"
    "  lda stack_lo,x\n"
    // y
    "  inx\n"
    "  ldy stack_lo,x\n"
    // address lo/hi
    "  lda 0x0400,y\n"
    "  sta address + 0\n"
    "  lda 0x0500,y\n"
    "  sta address + 1\n"
    // x
    "  inx\n"
    "  lda stack_hi,x\n"
    "  sta result + 1\n"
    "  lda stack_lo,x\n"
    "  sta result + 0\n"
    // x & 7
    "  and #7\n"
    "  sta value1 + 0\n"
    // x / 8
    "  lsr result + 1\n"
    "  ror result + 0\n"
    "  lsr result + 1\n"
    "  ror result + 0\n"
    "  lsr result + 1\n"
    "  ror result + 0\n"
    "  lda result + 0\n"
    "  tay\n"
    // col
    "  clc\n"
    "  lda 0x0600,y\n"
    "  adc address + 0\n"
    "  sta address + 0\n"
    "  lda 0x0640,y\n"
    "  adc address + 1\n"
    "  sta address + 1\n"
    // read byte
    "  ldy #0\n"
    "  lda (address),y\n"
    "  ldy value1 + 0\n"
    "  ora 0x0700,y\n"
    "  ldy #0\n"
    "  sta (address),y\n"
    "  rts\n");
}

void C64::insert_c64_vic_make_hires_tables()
{
  fprintf(out, 
    "make_hires_tables:\n"
    // row tables
    "  lda #0x00\n"
    "  sta address + 0\n"
    "  lda #0xe0\n"
    "  sta address + 1\n"
    "  ldy #0\n"
    "make_hires_tables_loop1:\n"
    "  clc\n"
    "  lda address + 0\n"
    "  sta 0x0400,y\n"
    "  adc #1\n"
    "  sta 0x0401,y\n"
    "  adc #1\n"
    "  sta 0x0402,y\n"
    "  adc #1\n"
    "  sta 0x0403,y\n"
    "  adc #1\n"
    "  sta 0x0404,y\n"
    "  adc #1\n"
    "  sta 0x0405,y\n"
    "  adc #1\n"
    "  sta 0x0406,y\n"
    "  adc #1\n"
    "  sta 0x0407,y\n"
    "  lda address + 1\n"
    "  sta 0x0500,y\n"
    "  sta 0x0501,y\n"
    "  sta 0x0502,y\n"
    "  sta 0x0503,y\n"
    "  sta 0x0504,y\n"
    "  sta 0x0505,y\n"
    "  sta 0x0506,y\n"
    "  sta 0x0507,y\n"
    "  clc\n"
    "  lda #0x40\n"
    "  adc address + 0\n"
    "  sta address + 0\n"
    "  lda #0x01\n"
    "  adc address + 1\n"
    "  sta address + 1\n"
    "  iny\n"
    "  iny\n"
    "  iny\n"
    "  iny\n"
    "  iny\n"
    "  iny\n"
    "  iny\n"
    "  iny\n"
    "  bne make_hires_tables_loop1\n"

    // col table
    "  ldy #0\n"
    "  lda #0\n"
    "make_hires_tables_loop3:\n"
    "  sta 0x0600,y\n"
    "  clc\n"
    "  adc #8\n"
    "  iny\n"
    "  bne make_hires_tables_loop3\n"

    "  ldy #40\n"
    "  lda #0\n"
    "make_hires_tables_loop4:\n"
    "  sta 0x0640,y\n"
    "  dey\n"
    "  bne make_hires_tables_loop4\n"

    "  lda #1\n"
    "  sta 0x0660\n"
    "  sta 0x0661\n"
    "  sta 0x0662\n"
    "  sta 0x0663\n"
    "  sta 0x0664\n"
    "  sta 0x0665\n"
    "  sta 0x0666\n"
    "  sta 0x0667\n"

    // bit table
    "  lda #128\n"
    "  sta 0x0700\n"
    "  lda #64\n"
    "  sta 0x0701\n"
    "  lda #32\n"
    "  sta 0x0702\n"
    "  lda #16\n"
    "  sta 0x0703\n"
    "  lda #8\n"
    "  sta 0x0704\n"
    "  lda #4\n"
    "  sta 0x0705\n"
    "  lda #2\n"
    "  sta 0x0706\n"
    "  lda #1\n"
    "  sta 0x0707\n"
    "  rts\n");
}

void C64::insert_c64_vic_text_enable()
{
  fprintf(out, 
    "text_enable:\n"
    "  inx\n"
    "  lda stack_lo,x\n"
    "  bne text_enable_1\n"
    "  lda #0x02\n"
    "  jmp text_enable_2\n"
    "text_enable_1:\n"
    "  lda #0x12\n"
    "text_enable_2:\n"
    "  sta 0xd018\n"
    "  lda 0xd011\n"
//    "  and #0xdf\n"
    "  and #0x5f\n"
    "  sta 0xd011\n"
    "  rts\n");
}

void C64::insert_c64_vic_text_clear()
{
  fprintf(out, 
    "text_clear:\n"
    "  inx\n"
    "  lda stack_lo,x\n"
    "  ldy #0\n"
    "text_clear_loop:\n"
    "  sta 0xc000,y\n"
    "  sta 0xc100,y\n"
    "  sta 0xc200,y\n"
    "  sta 0xc2e8,y\n"
    "  dey\n"
    "  bne text_clear_loop\n"
    "  rts\n");
}

void C64::insert_c64_vic_text_copy()
{
  fprintf(out, 
    "text_copy:\n"
    "  ldy #39\n"
    "text_copy_loop:\n"
    "  lda 0xc000 + 40 * 0,y\n"
    "  sta 0xc400 + 40 * 0,y\n"
    "  lda 0xc000 + 40 * 1,y\n"
    "  sta 0xc400 + 40 * 1,y\n"
    "  lda 0xc000 + 40 * 2,y\n"
    "  sta 0xc400 + 40 * 2,y\n"
    "  lda 0xc000 + 40 * 3,y\n"
    "  sta 0xc400 + 40 * 3,y\n"
    "  lda 0xc000 + 40 * 4,y\n"
    "  sta 0xc400 + 40 * 4,y\n"
    "  lda 0xc000 + 40 * 5,y\n"
    "  sta 0xc400 + 40 * 5,y\n"
    "  lda 0xc000 + 40 * 6,y\n"
    "  sta 0xc400 + 40 * 6,y\n"
    "  lda 0xc000 + 40 * 7,y\n"
    "  sta 0xc400 + 40 * 7,y\n"
    "  lda 0xc000 + 40 * 8,y\n"
    "  sta 0xc400 + 40 * 8,y\n"
    "  lda 0xc000 + 40 * 9,y\n"
    "  sta 0xc400 + 40 * 9,y\n"
    "  lda 0xc000 + 40 * 10,y\n"
    "  sta 0xc400 + 40 * 10,y\n"
    "  lda 0xc000 + 40 * 11,y\n"
    "  sta 0xc400 + 40 * 11,y\n"
    "  lda 0xc000 + 40 * 12,y\n"
    "  sta 0xc400 + 40 * 12,y\n"
    "  lda 0xc000 + 40 * 13,y\n"
    "  sta 0xc400 + 40 * 13,y\n"
    "  lda 0xc000 + 40 * 14,y\n"
    "  sta 0xc400 + 40 * 14,y\n"
    "  lda 0xc000 + 40 * 15,y\n"
    "  sta 0xc400 + 40 * 15,y\n"
    "  lda 0xc000 + 40 * 16,y\n"
    "  sta 0xc400 + 40 * 16,y\n"
    "  lda 0xc000 + 40 * 17,y\n"
    "  sta 0xc400 + 40 * 17,y\n"
    "  lda 0xc000 + 40 * 18,y\n"
    "  sta 0xc400 + 40 * 18,y\n"
    "  lda 0xc000 + 40 * 19,y\n"
    "  sta 0xc400 + 40 * 19,y\n"
    "  lda 0xc000 + 40 * 20,y\n"
    "  sta 0xc400 + 40 * 20,y\n"
    "  lda 0xc000 + 40 * 21,y\n"
    "  sta 0xc400 + 40 * 21,y\n"
    "  lda 0xc000 + 40 * 22,y\n"
    "  sta 0xc400 + 40 * 22,y\n"
    "  lda 0xc000 + 40 * 23,y\n"
    "  sta 0xc400 + 40 * 23,y\n"
    "  lda 0xc000 + 40 * 24,y\n"
    "  sta 0xc400 + 40 * 24,y\n"
    "  dey\n"
    "  bmi #3\n"
    "  jmp text_copy_loop\n"
    "  rts\n");

/*
  // about 10% slower but smaller
  fprintf(out, 
    "text_copy:\n"
    "  ldy #0\n"
    "text_copy_loop_1:\n"
    "  lda 0xc000,y\n"
    "  sta 0xc400,y\n"
    "  lda 0xc100,y\n"
    "  sta 0xc500,y\n"
    "  lda 0xc200,y\n"
    "  sta 0xc600,y\n"
    "  lda 0xc2e8,y\n"
    "  sta 0xc6e8,y\n"
    "  dey\n"
    "  bne text_copy_loop_1\n"
    "  rts\n");
*/
}

void C64::insert_c64_vic_text_plot()
{
  fprintf(out, 
    "text_plot:\n"
    "  lda stack_lo + 3,x\n" // y
    "  asl\n"
    "  tay\n"
    "  lda text_table + 0,y\n"
    "  sta address + 0\n"
    "  lda text_table + 1,y\n"
    "  sta address + 1\n"
    "  lda color_table + 0,y\n"
    "  sta value1 + 0\n"
    "  lda color_table + 1,y\n"
    "  sta value1 + 1\n"
    "  lda stack_lo + 4,x\n" // x
    "  tay\n"
    "  lda stack_lo + 2,x\n" // char
    "  sta (address),y\n"
    "  lda stack_lo + 1,x\n" // color
    "  sta (value1),y\n"
    "  inx\n"
    "  inx\n"
    "  inx\n"
    "  inx\n"
    "  rts\n");
}

void C64::insert_c64_vic_text_ascii_plot()
{
  fprintf(out, 
    "text_ascii_plot:\n"
    "  lda stack_lo + 3,x\n" // y
    "  asl\n"
    "  tay\n"
    "  lda text_table + 0,y\n"
    "  sta address + 0\n"
    "  lda text_table + 1,y\n"
    "  sta address + 1\n"
    "  lda color_table + 0,y\n"
    "  sta value1 + 0\n"
    "  lda color_table + 1,y\n"
    "  sta value1 + 1\n"
    "  lda stack_lo + 4,x\n" // x
    "  tay\n"
    "  lda stack_lo + 2,x\n" // char
    "  cmp #124\n" // ascii pipe
    "  bne text_ascii_plot_skip_1\n"
    "  lda #66\n"
    "  jmp text_ascii_plot_continue\n"
    "text_ascii_plot_skip_1:\n"
    "  cmp #95\n" // ascii underscore
    "  bne text_ascii_plot_skip_2\n"
    "  lda #100\n"
    "  jmp text_ascii_plot_continue\n"
    "text_ascii_plot_skip_2:\n"
    "  cmp #64\n" // ascii at
    "  bne text_ascii_plot_skip_3\n"
    "  lda #0\n"
    "  jmp text_ascii_plot_continue\n"
    "text_ascii_plot_skip_3:\n"
    "  cmp #96\n"
    "  bmi text_ascii_plot_continue\n"
    "  sec\n"
    "  sbc #96\n"
    "text_ascii_plot_continue:\n"
    "  sta (address),y\n"
    "  lda stack_lo + 1,x\n" // color
    "  sta (value1),y\n"
    "  inx\n"
    "  inx\n"
    "  inx\n"
    "  inx\n"
    "  rts\n");
}

void C64::insert_c64_vic_text_read()
{
  fprintf(out, 
    "text_read:\n"
    "  inx\n"
    "  lda stack_lo + 0,x\n" // y
    "  asl\n"
    "  tay\n"
    "  lda text_table + 0,y\n"
    "  sta address + 0\n"
    "  lda text_table + 1,y\n"
    "  sta address + 1\n"
    "  lda stack_lo + 1,x\n" // x
    "  tay\n"
    "  lda (address),y\n"
    "  sta stack_lo + 1,x\n"
    "  lda #0\n"
    "  sta stack_hi + 1,x\n"
    "  rts\n");
}

void C64::insert_c64_vic_text_string()
{
  fprintf(out, 
    "text_string:\n"
    "  lda stack_lo + 2,x\n"
    "  sta value2 + 0\n"
    "  lda stack_hi + 2,x\n"
    "  sta value2 + 1\n" // string pointer
    "  sec\n"
    "  lda value2 + 0\n"
    "  sbc #2\n"
    "  sta value2 + 0\n"
    "  lda value2 + 1\n"
    "  sbc #0\n"
    "  sta value2 + 1\n" // back -2 for length
    "  ldy #0\n"
    "  lda (value2),y\n"
    "  sta length\n" // string length
    "  clc\n"
    "  lda value2 + 0\n"
    "  adc #2\n"
    "  sta value2 + 0\n"
    "  lda value2 + 1\n"
    "  adc #0\n"
    "  sta value2 + 1\n" // forward +2 to string start
    "  lda stack_lo + 3,x\n" // y
    "  asl\n"
    "  tay\n"
    "  lda text_table + 0,y\n"
    "  sta address + 0\n"
    "  lda text_table + 1,y\n"
    "  sta address + 1\n" // text row
    "  lda color_table + 0,y\n"
    "  sta value1 + 0\n"
    "  lda color_table + 1,y\n"
    "  sta value1 + 1\n" // color row
    "  lda stack_lo + 4,x\n" // x
    "  tay\n" // text column
    "text_string_loop:\n"
    "  tya\n"
    "  pha\n"
    "  ldy #0\n"
    "  lda (value2),y\n"
    "  sta value3\n"
    "  pla\n"
    "  tay\n"
    "  lda value3\n" // char
    "  cmp #124\n" // ascii pipe
    "  bne text_string_skip_1\n"
    "  lda #66\n"
    "  jmp text_string_continue\n"
    "text_string_skip_1:\n"
    "  cmp #95\n" // ascii underscore
    "  bne text_string_skip_2\n"
    "  lda #100\n"
    "  jmp text_string_continue\n"
    "text_string_skip_2:\n"
    "  cmp #64\n" // ascii at
    "  bne text_string_skip_3\n"
    "  lda #0\n"
    "  jmp text_string_continue\n"
    "text_string_skip_3:\n"
    "  cmp #96\n"
    "  bmi text_string_continue\n"
    "  sec\n"
    "  sbc #96\n"
    "text_string_continue:\n"
    "  sta (address),y\n"
    "  lda stack_lo + 1,x\n" // color
    "  sta (value1),y\n"
    "  iny\n"
    "  inc value2 + 0\n"
    "  bne text_string_skip_inc_hi\n"
    "  inc value2 + 1\n" // increment string pointer
    "text_string_skip_inc_hi:\n"
    "  dec length\n"
    "  bne text_string_loop\n"
    "  inx\n"
    "  inx\n"
    "  inx\n"
    "  inx\n"
    "  rts\n");
}

void C64::insert_c64_vic_text_fill()
{
  fprintf(out, 
    "text_fill:\n"
    "  lda stack_lo + 4,x\n"
    "  asl\n"
    "  tay\n"
    "  lda text_table + 0,y\n"
    "  sta address + 0\n"
    "  lda text_table + 1,y\n"
    "  sta address + 1\n"
    "  clc\n"
    "  lda address + 0\n"
    "  adc stack_lo + 5,x\n"
    "  sta address + 0\n"
    "  lda address + 1\n"
    "  adc #0\n"
    "  sta address + 1\n"
    "  lda stack_lo + 2,x\n"
    "  sta value1\n"
    "  dec value1\n"
    "text_fill_loop_y:\n"
    "  lda stack_lo + 3,x\n"
    "  tay\n"
    "  dey\n"
    "  lda stack_lo + 1,x\n"
    "text_fill_loop_x:\n"
    "  sta (address),y\n"
    "  dey\n"
    "  bpl text_fill_loop_x\n"
    "  clc\n"
    "  lda address + 0\n"
    "  adc #40\n"
    "  sta address + 0\n"
    "  lda address + 1\n"
    "  adc #0\n"
    "  sta address + 1\n"
    "  dec value1\n"
    "  bpl text_fill_loop_y\n"
    "  inx\n"
    "  inx\n"
    "  inx\n"
    "  inx\n"
    "  inx\n"
    "  rts\n");
}

void C64::insert_c64_vic_text_paint()
{
  fprintf(out, 
    "text_paint:\n"
    "  lda stack_lo + 4,x\n"
    "  asl\n"
    "  tay\n"
    "  lda color_table + 0,y\n"
    "  sta address + 0\n"
    "  lda color_table + 1,y\n"
    "  sta address + 1\n"
    "  clc\n"
    "  lda address + 0\n"
    "  adc stack_lo + 5,x\n"
    "  sta address + 0\n"
    "  lda address + 1\n"
    "  adc #0\n"
    "  sta address + 1\n"
    "  lda stack_lo + 2,x\n"
    "  sta value1\n"
    "  dec value1\n"
    "text_paint_loop_y:\n"
    "  lda stack_lo + 3,x\n"
    "  tay\n"
    "  dey\n"
    "  lda stack_lo + 1,x\n"
    "text_paint_loop_x:\n"
    "  sta (address),y\n"
    "  dey\n"
    "  bpl text_paint_loop_x\n"
    "  clc\n"
    "  lda address + 0\n"
    "  adc #40\n"
    "  sta address + 0\n"
    "  lda address + 1\n"
    "  adc #0\n"
    "  sta address + 1\n"
    "  dec value1\n"
    "  bpl text_paint_loop_y\n"
    "  inx\n"
    "  inx\n"
    "  inx\n"
    "  inx\n"
    "  inx\n"
    "  rts\n");
}

void C64::insert_c64_vic_text_scroll_left()
{
  fprintf(out, 
    "text_scroll_left:\n"
    "  ldy #0\n"

    "text_scroll_left_loop_y:\n"
    "  lda text_table + 0,y\n"
    "  sta address + 0\n"
    "  lda text_table + 1,y\n"
    "  sta address + 1\n"
    "  lda color_table + 0,y\n"
    "  sta value1 + 0\n"
    "  lda color_table + 1,y\n"
    "  sta value1 + 1\n"
    "  tya\n"
    "  pha\n"

    "  ldy #0\n"
    "text_scroll_left_loop_x:\n"
    "  iny\n"
    "  lda (address),y\n"
    "  dey\n"
    "  sta (address),y\n"
    "  iny\n"
    "  lda (value1),y\n"
    "  dey\n"
    "  sta (value1),y\n"
    "  iny\n"
    "  cpy #39\n"
    "  bne text_scroll_left_loop_x\n"

    "  pla\n"
    "  tay\n"
    "  iny\n"
    "  iny\n"
    "  cpy #52\n"
    "  bne text_scroll_left_loop_y\n"
    "  rts\n");
}

void C64::insert_c64_vic_text_scroll_right()
{
  fprintf(out, 
    "text_scroll_right:\n"
    "  ldy #0\n"

    "text_scroll_right_loop_y:\n"
    "  lda text_table + 0,y\n"
    "  sta address + 0\n"
    "  lda text_table + 1,y\n"
    "  sta address + 1\n"
    "  lda color_table + 0,y\n"
    "  sta value1 + 0\n"
    "  lda color_table + 1,y\n"
    "  sta value1 + 1\n"
    "  tya\n"
    "  pha\n"

    "  ldy #39\n"
    "text_scroll_right_loop_x:\n"
    "  dey\n"
    "  lda (address),y\n"
    "  iny\n"
    "  sta (address),y\n"
    "  dey\n"
    "  lda (value1),y\n"
    "  iny\n"
    "  sta (value1),y\n"
    "  dey\n"
    "  cpy #1\n"
    "  bne text_scroll_right_loop_x\n"

    "  pla\n"
    "  tay\n"
    "  iny\n"
    "  iny\n"
    "  cpy #52\n"
    "  bne text_scroll_right_loop_y\n"
    "  rts\n");
}

void C64::insert_c64_vic_text_scroll_up()
{
  fprintf(out, 
    "text_scroll_up:\n"
    "  ldy #0\n"

    "text_scroll_up_loop_y:\n"
    "  lda text_table + 0,y\n"
    "  sta value1 + 0\n"
    "  lda text_table + 1,y\n"
    "  sta value1 + 1\n"
    "  lda text_table + 2,y\n"
    "  sta value2 + 0\n"
    "  lda text_table + 3,y\n"
    "  sta value2 + 1\n"
    "  lda color_table + 0,y\n"
    "  sta value3 + 0\n"
    "  lda color_table + 1,y\n"
    "  sta value3 + 1\n"
    "  lda color_table + 2,y\n"
    "  sta value4 + 0\n"
    "  lda color_table + 3,y\n"
    "  sta value4 + 1\n"
    "  tya\n"
    "  pha\n"

    "  ldy #0\n"
    "text_scroll_up_loop_x:\n"
    "  lda (value2),y\n"
    "  sta (value1),y\n"
    "  lda (value4),y\n"
    "  sta (value3),y\n"
    "  iny\n"
    "  cpy #40\n"
    "  bne text_scroll_up_loop_x\n"

    "  pla\n"
    "  tay\n"
    "  iny\n"
    "  iny\n"
    "  cpy #48\n"
    "  bne text_scroll_up_loop_y\n"
    "  rts\n");
}

void C64::insert_c64_vic_text_scroll_down()
{
  fprintf(out, 
    "text_scroll_down:\n"
    "  ldy #46\n"

    "text_scroll_down_loop_y:\n"
    "  lda text_table + 2,y\n"
    "  sta value1 + 0\n"
    "  lda text_table + 3,y\n"
    "  sta value1 + 1\n"
    "  lda text_table + 0,y\n"
    "  sta value2 + 0\n"
    "  lda text_table + 1,y\n"
    "  sta value2 + 1\n"
    "  lda color_table + 2,y\n"
    "  sta value3 + 0\n"
    "  lda color_table + 3,y\n"
    "  sta value3 + 1\n"
    "  lda color_table + 0,y\n"
    "  sta value4 + 0\n"
    "  lda color_table + 1,y\n"
    "  sta value4 + 1\n"
    "  tya\n"
    "  pha\n"

    "  ldy #0\n"
    "text_scroll_down_loop_x:\n"
    "  lda (value2),y\n"
    "  sta (value1),y\n"
    "  lda (value4),y\n"
    "  sta (value3),y\n"
    "  iny\n"
    "  cpy #40\n"
    "  bne text_scroll_down_loop_x\n"

    "  pla\n"
    "  tay\n"
    "  dey\n"
    "  dey\n"
    "  bpl text_scroll_down_loop_y\n"
    "  rts\n");
}

void C64::insert_c64_vic_text_smooth_scroll()
{
  fprintf(out, 
    "text_smooth_scroll:\n"
    "  lda scroll_frame\n"
    "  cmp #3\n"
    "  beq text_smooth_scroll_continue\n"
    "  inx\n"
    "  inx\n"
    "  inx\n"
    "  inx\n"
    "  rts\n"
    "text_smooth_scroll_continue:\n"
    "  lda stack_lo + 3,x\n" // x
    "  sta scroll_x + 0\n"
    "  and #7\n"
    "  sta scroll_x_fine\n"
    "  sec\n"
    "  lda #7\n"
    "  sbc scroll_x_fine\n"
    "  sta scroll_x_fine\n"

    "  lda stack_hi + 3,x\n"
    "  sta scroll_x + 1\n"

    "  lsr scroll_x + 1\n" // divide by 8
    "  ror scroll_x + 0\n"
    "  lsr scroll_x + 1\n"
    "  ror scroll_x + 0\n"
    "  lsr scroll_x + 1\n"
    "  ror scroll_x + 0\n"

    "  lda stack_lo + 2,x\n" // y
    "  sta scroll_y + 0\n"
    "  and #7\n"
    "  sta scroll_y_fine\n"
    "  sec\n"
    "  lda #7\n"
    "  sbc scroll_y_fine\n"
    "  sta scroll_y_fine\n"

    "  lda stack_hi + 2,x\n"
    "  sta scroll_y + 1\n"

    "  lsr scroll_y + 1\n" // divide by 8
    "  ror scroll_y + 0\n"
    "  lsr scroll_y + 1\n"
    "  ror scroll_y + 0\n"
    "  lsr scroll_y + 1\n"
    "  ror scroll_y + 0\n"

    "  lda stack_lo + 1,x\n" // w
    "  sta scroll_w + 0\n"
    "  lda stack_hi + 1,x\n"
    "  sta scroll_w + 1\n"

    "  lda stack_lo + 4,x\n"
    "  sta scroll_src_0 + 0\n"
    "  lda stack_hi + 4,x\n"
    "  sta scroll_src_0 + 1\n"

    "  ldy scroll_y\n"
    "text_smooth_scroll_loop_1:\n"
    "  clc\n"
    "  lda scroll_src_0 + 0\n"
    "  adc scroll_w + 0\n"
    "  sta scroll_src_0 + 0\n"
    "  lda scroll_src_0 + 1\n"
    "  adc scroll_w + 1\n"
    "  sta scroll_src_0 + 1\n"
    "  dey\n"
    "  bpl text_smooth_scroll_loop_1\n"

    "  clc\n"
    "  lda scroll_src_0 + 0\n"
    "  adc scroll_x + 0\n"
    "  sta scroll_src_0 + 0\n"
    "  lda scroll_src_0 + 1\n"
    "  adc scroll_x + 1\n"
    "  sta scroll_src_0 + 1\n"

    "  lda scroll_src_0 + 0\n"
    "  sta scroll_src_1 + 0\n"
    "  lda scroll_src_0 + 1\n"
    "  sta scroll_src_1 + 1\n"

    "  ldy #11\n"
    "text_smooth_scroll_loop_2:\n"
    "  clc\n"
    "  lda scroll_src_1 + 0\n"
    "  adc scroll_w + 0\n"
    "  sta scroll_src_1 + 0\n"
    "  lda scroll_src_1 + 1\n"
    "  adc scroll_w + 1\n"
    "  sta scroll_src_1 + 1\n"
    "  dey\n"
    "  bpl text_smooth_scroll_loop_2\n"

    "  lda scroll_page\n"
    "  bne text_smooth_scroll_page_1\n"

    "text_smooth_scroll_page_0:\n"
    "  clc\n"
    "  lda #0x00\n"
    "  sta scroll_dest_0 + 0\n"
    "  lda #0xc4\n"
    "  sta scroll_dest_0 + 1\n"

    "  lda #0xe0\n"
    "  sta scroll_dest_1 + 0\n"
    "  lda #0xc5\n"
    "  sta scroll_dest_1 + 1\n"

    "  jmp text_smooth_scroll_page_skip\n"

    "text_smooth_scroll_page_1:\n"

    "  lda #0x00\n"
    "  sta scroll_dest_0 + 0\n"
    "  lda #0xc0\n"
    "  sta scroll_dest_0 + 1\n"

    "  lda #0xe0\n"
    "  sta scroll_dest_1 + 0\n"
    "  lda #0xc1\n"
    "  sta scroll_dest_1 + 1\n"

    "text_smooth_scroll_page_skip:\n"

    "  lda #2\n"
    "  sta scroll_frame\n" // reset frame counter

    "  inx\n"
    "  inx\n"
    "  inx\n"
    "  inx\n"
    "  rts\n");
}

void C64::insert_c64_vic_make_text_table()
{
  fprintf(out, 
    "make_text_table:\n"
    "  lda #0x00\n"
    "  sta address + 0\n"
    "  lda #0xc0\n"
    "  sta address + 1\n"
    "  ldy #0\n"
    "make_text_table_loop:\n"
    "  lda address + 0\n"
    "  sta text_table + 0,y\n"
    "  lda address + 1\n"
    "  sta text_table + 1,y\n"
    "  clc\n"
    "  lda address + 0\n"
    "  adc #40\n"
    "  sta address + 0\n"
    "  lda address + 1\n"
    "  adc #0\n"
    "  sta address + 1\n"
    "  iny\n"
    "  iny\n"
    "  cpy #50\n"
    "  bmi make_text_table_loop\n"
    "  rts\n");
}

void C64::insert_c64_vic_make_color_table()
{
  fprintf(out, 
    "make_color_table:\n"
    "  lda #0x00\n"
    "  sta address + 0\n"
    "  lda #0xd8\n"
    "  sta address + 1\n"
    "  ldy #0\n"
    "make_color_table_loop:\n"
    "  lda address + 0\n"
    "  sta color_table + 0,y\n"
    "  lda address + 1\n"
    "  sta color_table + 1,y\n"
    "  clc\n"
    "  lda address + 0\n"
    "  adc #40\n"
    "  sta address + 0\n"
    "  lda address + 1\n"
    "  adc #0\n"
    "  sta address + 1\n"
    "  iny\n"
    "  iny\n"
    "  cpy #50\n"
    "  bmi make_color_table_loop\n"
    "  rts\n");
}

void C64::insert_c64_vic_color_ram_clear()
{
  fprintf(out, 
    "color_ram_clear:\n"
    "  inx\n"
    "  lda stack_lo,x\n"
    "  ldy #0\n"
    "color_ram_clear_loop:\n"
    "  sta 0xd800,y\n"
    "  sta 0xd900,y\n"
    "  sta 0xda00,y\n"
    "  sta 0xdae8,y\n"
    "  dey\n"
    "  bne color_ram_clear_loop\n"
    "  rts\n");
}

void C64::insert_c64_vic_copy_uppercase()
{
  fprintf(out, 
    "copy_uppercase:\n"
    "  sei\n"
    "  lda #50\n"
    "  sta 0x0001\n"
    "  ldy #0\n"
    "copy_uppercase_loop:\n"
    "  lda 0xd000,y\n"
    "  sta 0xc800,y\n"
    "  lda 0xd100,y\n"
    "  sta 0xc900,y\n"
    "  lda 0xd200,y\n"
    "  sta 0xca00,y\n"
    "  lda 0xd300,y\n"
    "  sta 0xcb00,y\n"
    "  lda 0xd400,y\n"
    "  sta 0xcc00,y\n"
    "  lda 0xd500,y\n"
    "  sta 0xcd00,y\n"
    "  lda 0xd600,y\n"
    "  sta 0xce00,y\n"
    "  lda 0xd700,y\n"
    "  sta 0xcf00,y\n"
    "  dey\n"
    "  bne copy_uppercase_loop\n"
    "  lda #53\n"
    "  sta 0x0001\n"
    "  cli\n"
    "  rts\n");
}

void C64::insert_c64_vic_copy_lowercase()
{
  fprintf(out, 
    "copy_lowercase:\n"
    "  sei\n"
    "  lda #50\n"
    "  sta 0x0001\n"
    "  ldy #0\n"
    "copy_lowercase_loop:\n"
    "  lda 0xd800,y\n"
    "  sta 0xc800,y\n"
    "  lda 0xd900,y\n"
    "  sta 0xc900,y\n"
    "  lda 0xda00,y\n"
    "  sta 0xca00,y\n"
    "  lda 0xdb00,y\n"
    "  sta 0xcb00,y\n"
    "  lda 0xdc00,y\n"
    "  sta 0xcc00,y\n"
    "  lda 0xdd00,y\n"
    "  sta 0xcd00,y\n"
    "  lda 0xde00,y\n"
    "  sta 0xce00,y\n"
    "  lda 0xdf00,y\n"
    "  sta 0xcf00,y\n"
    "  dey\n"
    "  bne copy_lowercase_loop\n"
    "  lda #53\n"
    "  sta 0x0001\n"
    "  cli\n"
    "  rts\n");
}

void C64::insert_c64_vic_copy_data_from_array()
{
  fprintf(out, 
    "copy_data_from_array:\n"
    "  lda stack_lo + 1,x\n"
    "  sta value3 + 0\n"
    "  lda stack_hi + 1,x\n"
    "  sta value3 + 1\n" // bytes
    "  lda stack_lo + 2,x\n"
    "  sta address + 0\n"
    "  lda stack_hi + 2,x\n"
    "  sta address + 1\n" // address
    "  lda stack_lo + 3,x\n"
    "  sta value1 + 0\n"
    "  lda stack_hi + 3,x\n"
    "  sta value1 + 1\n" // array
    "  ldy #0\n"
    "copy_data_from_array_loop:\n"
    "  lda (value1),y\n"
    "  sta (address),y\n"
    "  inc value1 + 0\n"
    "  bne copy_data_from_array_inc_address\n"
    "  inc value1 + 1\n"
    "copy_data_from_array_inc_address:\n"
    "  inc address + 0\n"
    "  bne copy_data_from_array_dec_array\n"
    "  inc address + 1\n"
    "copy_data_from_array_dec_array:\n"
    "  lda value3 + 0\n"
    "  bne copy_data_from_array_dec_array_skip\n"
    "  dec value3 + 1\n"
    "copy_data_from_array_dec_array_skip:\n"
    "  dec value3 + 0\n"
    "  lda value3 + 1\n"
    "  cmp #0\n"
    "  bne copy_data_from_array_loop\n"
    "  lda value3 + 0\n"
    "  cmp #0\n"
    "  bne copy_data_from_array_loop\n"
    "  inx\n"
    "  inx\n"
    "  inx\n"
    "  rts\n");
}

// shift key status is returned in the high byte
void C64::insert_c64_keyboard()
{
  fprintf(out, 
    "keyboard_current_key_pressed:\n"
    "  txa\n"
    "  pha\n"

    "  lda #0\n"
    "  ldy #63\n"
    "keyboard_table_clear:\n"
    "  sta key_table,y\n"
    "  dey\n"
    "  bpl keyboard_table_clear\n"

    "  lda #128\n"
    "  sta value1\n"
    "  ldx #0\n"
    "keyboard_loop_x:\n"
    "  sec\n"
    "  lda #255\n"
    "  sbc value1\n"
    "  sta 0xdc00\n"
    "  lda 0xdc01\n"
    "  sta value3\n"
    "  lda #128\n"
    "  sta value2\n"
    "  ldy #0\n"
    "keyboard_loop_y:\n"
    "  lda value3\n"
    "  and value2\n"
    "  bne keyboard_loop_continue\n"

    "  tya\n"
    "  asl\n"
    "  asl\n"
    "  asl\n"
    "  sta result\n"
    "  txa\n"
    "  adc result\n"
    "  sta result\n"

    "  txa\n"
    "  pha\n"
    "  ldx result\n"
    "  lda #1\n"
    "  sta key_table,x\n"
    "  pla\n"
    "  tax\n"

    "keyboard_loop_continue:\n"
    "  lda value2\n"
    "  lsr\n"
    "  sta value2\n"
    "  iny\n"
    "  cpy #8\n"
    "  bne keyboard_loop_y\n"
    "  lda value1\n"
    "  lsr\n"
    "  sta value1\n"
    "  inx\n"
    "  cpx #8\n"
    "  bne keyboard_loop_x\n"

    "  pla\n"
    "  tax\n"
    "  ldy #6\n"
    "  lda key_table,y\n"
    "  ldy #25\n"
    "  ora key_table,y\n"
    "  sta stack_hi,x\n"

    "  lda #0\n"
    "  ldy #6\n"
    "  sta key_table,y\n"
    "  ldy #25\n"
    "  sta key_table,y\n"

    "  ldy #63\n"
    "keyboard_return_loop:\n"
    "  lda key_table,y\n"
    "  cmp #1\n"
    "  beq keyboard_return\n"
    "  dey\n"
    "  bpl keyboard_return_loop\n"
    "  lda #255\n"
    "  sta stack_lo,x\n"
    "  dex\n"
    "  rts\n"
    "keyboard_return:\n"
    "  tya\n"
    "  sta stack_lo,x\n"
    "  dex\n"
    "  rts\n");
}

void C64::insert_c64_sprite_interrupt()
{
  fprintf(out, 
    "sprite_interrupt:\n"
    "  pha\n"
    "  txa\n"
    "  pha\n"
    "  tya\n"
    "  pha\n"
    "  lda value1 + 0\n"
    "  pha\n"
    "  lda value1 + 1\n"
    "  pha\n"
    "  lda value2 + 0\n"
    "  pha\n"
    "  lda value2 + 1\n"
    "  pha\n"
    "  lda sprite_pos_x + 0\n"
    "  sta 0xd000\n"
    "  lda sprite_pos_y + 0\n"
    "  sta 0xd001\n"
    "  lda sprite_pos_x + 1\n"
    "  sta 0xd002\n"
    "  lda sprite_pos_y + 1\n"
    "  sta 0xd003\n"
    "  lda sprite_pos_x + 2\n"
    "  sta 0xd004\n"
    "  lda sprite_pos_y + 2\n"
    "  sta 0xd005\n"
    "  lda sprite_pos_x + 3\n"
    "  sta 0xd006\n"
    "  lda sprite_pos_y + 3\n"
    "  sta 0xd007\n"
    "  lda sprite_pos_x + 4\n"
    "  sta 0xd008\n"
    "  lda sprite_pos_y + 4\n"
    "  sta 0xd009\n"
    "  lda sprite_pos_x + 5\n"
    "  sta 0xd00a\n"
    "  lda sprite_pos_y + 5\n"
    "  sta 0xd00b\n"
    "  lda sprite_pos_x + 6\n"
    "  sta 0xd00c\n"
    "  lda sprite_pos_y + 6\n"
    "  sta 0xd00d\n"
    "  lda sprite_pos_x + 7\n"
    "  sta 0xd00e\n"
    "  lda sprite_pos_y + 7\n"
    "  sta 0xd00f\n"

    "  lda sprite_pos_x_msb\n"
    "  sta 0xd010\n"

    "  lda scroll_frame\n"
    "  bne scroll_flip_skip\n"

    "  lda scroll_page\n"
    "  eor #1\n"
    "  sta scroll_page\n"
    "  bne scroll_flip_1\n"
    "scroll_flip_0:\n"
    "  lda 0xd018\n"
    "  and #15\n"
    "  jmp scroll_flip_end\n"
    "scroll_flip_1:\n"
    "  lda 0xd018\n"
    "  and #15\n"
    "  ora #16\n"
    "scroll_flip_end:\n"
    "  sta 0xd018\n"

    "  lda 0xd016\n"
    "  and #0xf8\n"
    "  ora scroll_x_fine\n"
    "  sta 0xd016\n"

    "  lda 0xd011\n"
    "  and #0xf8\n"
    "  ora scroll_y_fine\n"
    "  sta 0xd011\n"

    "  lda #3\n"
    "  sta scroll_frame\n"
    "  jmp scroll_skip\n"
    "scroll_flip_skip:\n"
    "  lda scroll_frame\n"
    "  cmp #2\n"
    "  bne scroll_1\n"

    "scroll_0:\n"
    "  lda scroll_src_0 + 0\n"
    "  sta value1 + 0\n"
    "  lda scroll_src_0 + 1\n"
    "  sta value1 + 1\n"
       
    "  lda scroll_dest_0 + 0\n"
    "  sta value2 + 0\n"
    "  lda scroll_dest_0 + 1\n"
    "  sta value2 + 1\n"

    "  jmp scroll_start\n"

    "scroll_1:\n"
    "  lda scroll_frame\n"
    "  cmp #1\n"
    "  bne scroll_skip\n"

    "  lda scroll_src_1 + 0\n"
    "  sta value1 + 0\n"
    "  lda scroll_src_1 + 1\n"
    "  sta value1 + 1\n"
       
    "  lda scroll_dest_1 + 0\n"
    "  sta value2 + 0\n"
    "  lda scroll_dest_1 + 1\n"
    "  sta value2 + 1\n"

    "scroll_start:\n"
    "  ldx #12\n" // row
    "scroll_loop_0_y:\n"
    "  ldy #39\n" // column
    "scroll_loop_0_x:\n"
    "  lda (value1),y\n"
    "  sta (value2),y\n"
    "  dey\n"
    "  bpl scroll_loop_0_x\n"
    "  clc\n"
    "  lda value1 + 0\n"
    "  adc scroll_w + 0\n"
    "  sta value1 + 0\n"
    "  lda value1 + 1\n"
    "  adc scroll_w + 1\n"
    "  sta value1 + 1\n"

    "  clc\n"
    "  lda value2 + 0\n"
    "  adc #40\n"
    "  sta value2 + 0\n"
    "  lda value2 + 1\n"
    "  adc #0\n"
    "  sta value2 + 1\n"

    "  dex\n"
    "  bpl scroll_loop_0_y\n"

    "  dec scroll_frame\n"

    "scroll_skip:\n"
    "  asl 0xd019\n"

    "  pla\n"
    "  sta value2 + 1\n"
    "  pla\n"
    "  sta value2 + 0\n"
    "  pla\n"
    "  sta value1 + 1\n"
    "  pla\n"
    "  sta value1 + 0\n"
    "  pla\n"
    "  tay\n"
    "  pla\n"
    "  tax\n"
    "  pla\n"
    "  rti\n");
}

