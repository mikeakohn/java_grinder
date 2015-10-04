/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "AppleIIgs.h"

#define PUSH() \
  fprintf(out, "; PUSH\n"); \
  fprintf(out, "  sta stack,x\n"); \
  fprintf(out, "  dex\n"); \
  fprintf(out, "  dex\n")

#define POP() \
  fprintf(out, "; POP\n"); \
  fprintf(out, "  inx\n"); \
  fprintf(out, "  inx\n"); \
  fprintf(out, "  lda stack,x\n")

AppleIIgs::AppleIIgs() :
//  need_plot_char(false),
//  need_print_char(false),
  need_hires_enable(false),
  need_hires_clear(false),
  need_hires_update(false),
  need_hires_plot(false),
  need_hires_line(false),
  need_hires_span(false),
  need_hires_read(false),
  need_hires_sprite(false),
  need_hires_palette(false),
  need_hires_set_row(false),
  need_rnd(false)
{
  start_org = 0x1000;
  java_stack = 0x800;
  ram_start = 0x7000;
}

AppleIIgs::~AppleIIgs()
{
  if (need_hires_enable) { insert_hires_enable(); }
  if (need_hires_clear) { insert_hires_clear(); }
  if (need_hires_update) { insert_hires_update(); }
  if (need_hires_plot) { insert_hires_plot(); }
  if (need_hires_line) { insert_hires_line(); }
  if (need_hires_span) { insert_hires_span(); }
  if (need_hires_read) { insert_hires_read(); }
  if (need_hires_sprite) { insert_hires_sprite(); }
  if (need_hires_palette) { insert_hires_palette(); }
  if (need_hires_set_row) { insert_hires_set_row(); }
  if (need_rnd) { insert_rnd(); }
  insert_hires_calc_address();
  insert_glu();
}

int AppleIIgs::open(const char *filename)
{
  if (W65816::open(filename) != 0) { return -1; }

  // for line drawing
  fprintf(out, "_vars equ 0x7f00\n");
  fprintf(out, "_x1 equ _vars + 0\n");
  fprintf(out, "_y1 equ _vars + 2\n");
  fprintf(out, "_x2 equ _vars + 4\n");
  fprintf(out, "_y2 equ _vars + 6\n");
  fprintf(out, "_color equ _vars + 8\n");
  fprintf(out, "_dx equ _vars + 10\n");
  fprintf(out, "_dy equ _vars + 12\n");
  fprintf(out, "_inx equ _vars + 14\n");
  fprintf(out, "_iny equ _vars + 16\n");
  fprintf(out, "_e equ _vars + 18\n");

  // random number seed
  fprintf(out, "_seed equ _vars + 20\n");
  fprintf(out, "_bit equ _vars + 22\n");
//  fprintf(out, "rep #0x30\n");
  fprintf(out, "lda #0xace1\n");
  fprintf(out, "sta _seed\n");

  // graphics bank
  fprintf(out, "_bank equ _vars + 24\n");

  return 0;
}

int AppleIIgs::appleiigs_plotChar_IC()
{
//  need_plot_char = true;
//FIXME put into subroutine
//FIXME broked
  fprintf(out,
    "  ;; plotChar()\n"
    "ply\n"
    "pla\n"
    "sep #0x20\n"
    "sta 0x0000,y\n"
    "rep #0x20\n");

  return 0;
}

int AppleIIgs::appleiigs_printChar_C()
{
//  need_print_char = true;
//FIXME put into subroutine
  fprintf(out, ";; printChar()\n");
  POP();

  fprintf(out, "  phx\n");
  fprintf(out, "  ora #0x80\n");
  fprintf(out, "  sep #0x30\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  xce\n");
  fprintf(out, "  jsr 0xfded\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  xce\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  plx\n");

  stack--;

  return 0;
}

int AppleIIgs::appleiigs_hiresEnable_I()
{
  need_hires_enable = true;
  fprintf(out, "  jsr hires_enable\n");

  stack--;

  return 0;
}

int AppleIIgs::appleiigs_hiresClear_I()
{
  need_hires_clear = true;
  fprintf(out, "  jsr hires_clear\n");

  stack--;

  return 0;
}

int AppleIIgs::appleiigs_hiresUpdate_II()
{
  need_hires_update = true;
  fprintf(out, "  jsr hires_update\n");

  return 0;
}

int AppleIIgs::appleiigs_hiresPlot_III()
{
  need_hires_plot = true;
  fprintf(out, "  jsr hires_plot\n");

  stack -= 3;

  return 0;
}

int AppleIIgs::appleiigs_hiresLine_IIIII()
{
  need_hires_line = true;
  fprintf(out, "  jsr hires_line\n");

  stack -= 5;

  return 0;
}

int AppleIIgs::appleiigs_hiresSpan_IIII()
{
  need_hires_span = true;
  fprintf(out, "  jsr hires_span\n");

  stack -= 4;

  return 0;
}

int AppleIIgs::appleiigs_hiresRead_II()
{
  need_hires_read = true;
  fprintf(out, "  jsr hires_read\n");

  stack--;

  return 0;
}

int AppleIIgs::appleiigs_hiresSprite_aBIIII()
{
  need_hires_sprite = true;
  fprintf(out, "  jsr hires_sprite\n");
  
  stack -= 5;

  return 0;
}

int AppleIIgs::appleiigs_hiresPalette_IaI()
{
  need_hires_palette = true;
  fprintf(out, "  jsr hires_palette\n");

  stack -= 2;

  return 0;
}

int AppleIIgs::appleiigs_hiresSetRow_II()
{
  need_hires_set_row = true;
  fprintf(out, "  jsr hires_set_row\n");

  stack -= 2;

  return 0;
}

int AppleIIgs::appleiigs_rnd()
{
  need_rnd = true;
  fprintf(out, "  jsr rnd\n");
  stack++;

  return 0;
}

// Sound API
// FIXME need parameters for wavetable location/size
int AppleIIgs::appleiigs_loadWaveTable_aBI()
{
  fprintf(out, "; load_wave_table\n");
  // point to RAM
  fprintf(out, "lda #0x40\n");
  fprintf(out, "jsr set_glu_control\n");
  // set DOC RAM location
  POP();
  fprintf(out, "  sta address2\n");
  // load wavetable
  POP();
  fprintf(out, "  sta address\n");
  fprintf(out, "  dec address\n");
  fprintf(out, "  dec address\n");
  fprintf(out, "  lda (address)\n");
  fprintf(out, "  sta length\n");
  fprintf(out, "  inc address\n");
  fprintf(out, "  inc address\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "load_wave_table_%d:\n", label_count);
  fprintf(out, "  lda address2\n");
  fprintf(out, "  jsr set_glu_address\n");
  fprintf(out, "  lda (address)\n");
  fprintf(out, "  jsr set_glu_data\n");
  fprintf(out, "  inc address\n");
  fprintf(out, "  inc address2\n");
  fprintf(out, "  iny\n");
  // wavetable size
  fprintf(out, "  cpy length\n");
  fprintf(out, "  bne load_wave_table_%d\n", label_count);
  // point to DOC
  fprintf(out, "lda #0\n");
  fprintf(out, "jsr set_glu_control\n");

  stack -= 2;
  label_count++;
  return 0;
}

int AppleIIgs::appleiigs_enableOscillators_I()
{
  fprintf(out, "; enable_oscillators\n");
  // point to DOC
  fprintf(out, "lda #0\n");
  fprintf(out, "jsr set_glu_control\n");
  // enable oscillators
  fprintf(out, "lda #0xe1\n");
  fprintf(out, "jsr set_glu_address\n");
  POP();
  fprintf(out, "asl\n");
  fprintf(out, "jsr set_glu_data\n");

  stack--;
  return 0;
}

int AppleIIgs::appleiigs_setMasterVolume_I()
{
  fprintf(out, "; set_master_volume\n");
  // set DOC master volume
  POP();
  fprintf(out, "jsr set_glu_control\n");

  stack--;
  return 0;
}

int AppleIIgs::appleiigs_setSoundVolume_II()
{
  fprintf(out, "; set_sound_volume\n");
  // point to DOC
  fprintf(out, "lda #0\n");
  fprintf(out, "jsr set_glu_control\n");
  // set volume
  POP();
  fprintf(out, "tay\n");
  POP();
  fprintf(out, "sta address\n");
  fprintf(out, "clc\n");
  fprintf(out, "adc #0x40\n");
  fprintf(out, "jsr set_glu_address\n");
  fprintf(out, "tya\n");
  fprintf(out, "jsr set_glu_data\n");
  // set control register
  fprintf(out, "lda address\n");
  fprintf(out, "clc\n");
  fprintf(out, "adc #0xa0\n");
  fprintf(out, "jsr set_glu_address\n");
  fprintf(out, "lda #0\n");
  fprintf(out, "jsr set_glu_data\n");

  stack -= 2;
  return 0;
}

int AppleIIgs::appleiigs_setSoundFrequency_II()
{
  fprintf(out, "; set_sound_frequency\n");
  // point to DOC
  fprintf(out, "lda #0\n");
  fprintf(out, "jsr set_glu_control\n");
  // set voice
  POP();
  fprintf(out, "tay\n");
  POP();
  fprintf(out, "sta address\n");
  // freq lo
  fprintf(out, "clc\n");
  fprintf(out, "adc #0x00\n");
  fprintf(out, "jsr set_glu_address\n");
  fprintf(out, "tya\n");
  fprintf(out, "jsr set_glu_data\n");
  // freq hi
  fprintf(out, "lda address\n");
  fprintf(out, "clc\n");
  fprintf(out, "adc #0x20\n");
  fprintf(out, "jsr set_glu_address\n");
  fprintf(out, "tya\n");
  fprintf(out, "xba\n");
  fprintf(out, "jsr set_glu_data\n");

  stack -= 2;
  return 0;
}

int AppleIIgs::appleiigs_setWaveTable_III()
{
  fprintf(out, "; set_wave_table\n");
  // point to DOC
  fprintf(out, "lda #0\n");
  fprintf(out, "jsr set_glu_control\n");
  POP();
  fprintf(out, "sta value2\n");
  POP();
  fprintf(out, "sta value1\n");
  POP();
  fprintf(out, "sta address\n");
  // set wave table pointer
  fprintf(out, "clc\n");
  fprintf(out, "adc #0x80\n");
  fprintf(out, "jsr set_glu_address\n");
  fprintf(out, "lda value1\n");
  fprintf(out, "jsr set_glu_data\n");
  // set wave table size
  fprintf(out, "lda address\n");
  fprintf(out, "clc\n");
  fprintf(out, "adc #0xc0\n");
  fprintf(out, "asl\n");
  fprintf(out, "asl\n");
  fprintf(out, "asl\n");
  fprintf(out, "jsr set_glu_address\n");
  fprintf(out, "lda value2\n");
  fprintf(out, "jsr set_glu_data\n");

  stack -= 3;
  return 0;
}

// subroutines
void AppleIIgs::insert_hires_enable()
{
  fprintf(out, "hires_enable:\n");
  fprintf(out, "  sep #0x30\n");
  fprintf(out, "  lda.b #10000001b\n");
  fprintf(out, "  sta.l 0xe1c029\n");
  fprintf(out, "  lda.b #0\n");
  fprintf(out, "  sta.l 0xe1c034\n");
  fprintf(out, "  rep #0x30\n");
  POP();
  fprintf(out, "  sta _bank\n");
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_hires_clear()
{
  fprintf(out, "hires_clear:\n");
  fprintf(out, "  lda #0x2000\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda _bank\n");
  fprintf(out, "  sta address + 2\n");
  POP();
  fprintf(out, "  sta value1\n");
  fprintf(out, "  xba\n");
  fprintf(out, "  ora value1\n");
  fprintf(out, "  ldy #31998\n");
  fprintf(out, "hires_clear_loop:\n");
  fprintf(out, "  sta [address],y\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bpl hires_clear_loop\n");
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_hires_update()
{
  fprintf(out, "hires_update:\n");
  POP();
  fprintf(out, "  sta value1\n");
  POP();
  fprintf(out, "  phx\n");
  fprintf(out, "  tax\n");
  fprintf(out, "hires_update_loop:\n");
  fprintf(out, "  lda.l 0x022000,x\n");
  fprintf(out, "  sta.l 0xe12000,x\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  cpx value1\n");
  fprintf(out, "  bne hires_update_loop\n");
  fprintf(out, "  plx\n");
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_hires_plot()
{
  fprintf(out, "hires_plot:\n");
  POP();
  fprintf(out, "  tay\n");
  fprintf(out, "  jsr hires_calc_address\n");
  fprintf(out, "  tya\n");
  fprintf(out, "  sep #0x30\n");
  fprintf(out, "  sta [address]\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_hires_line()
{
  fprintf(out, "hires_line:\n");
  POP();
  fprintf(out, "  sta _color\n");
  POP();
  fprintf(out, "  sta _y2\n");
  POP();
  fprintf(out, "  sta _x2\n");
  POP();
  fprintf(out, "  sta _y1\n");
  POP();
  fprintf(out, "  sta _x1\n");

  fprintf(out, "  lda _x1\n");
  fprintf(out, "  cmp _x2\n");
  fprintf(out, "  bne hires_line_start\n");
  fprintf(out, "  lda _y1\n");
  fprintf(out, "  cmp _y2\n");
  fprintf(out, "  bne hires_line_start\n");
  fprintf(out, "  jmp hires_line_end\n");

  fprintf(out, "hires_line_start:\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  lda _x2\n");
  fprintf(out, "  sbc _x1\n");
  fprintf(out, "  sta _dx\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  lda _y2\n");
  fprintf(out, "  sbc _y1\n");
  fprintf(out, "  sta _dy\n");

  fprintf(out, "  lda #1\n");
  fprintf(out, "  sta _inx\n");
  fprintf(out, "  sta _iny\n");

  fprintf(out, "  lda _dx\n");
  fprintf(out, "  bpl hires_line_1\n");
  fprintf(out, "  eor #0xffff\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  adc #1\n");
  fprintf(out, "  sta _dx\n");
  fprintf(out, "  lda #-1\n");
  fprintf(out, "  sta _inx\n");
  fprintf(out, "hires_line_1:\n");
  fprintf(out, "  lda _dy\n");
  fprintf(out, "  bpl hires_line_2\n");
  fprintf(out, "  eor #0xffff\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  adc #1\n");
  fprintf(out, "  sta _dy\n");
  fprintf(out, "  lda #-1\n");
  fprintf(out, "  sta _iny\n");
  fprintf(out, "hires_line_2:\n");
  fprintf(out, "  lda _dx\n");
  fprintf(out, "  cmp _dy\n");
  fprintf(out, "  bmi hires_line_y\n");
  fprintf(out, "hires_line_x:\n");
  fprintf(out, "  asl _dy\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  lda _dy\n");
  fprintf(out, "  sbc _dx\n");
  fprintf(out, "  sta _e\n");
  fprintf(out, "  asl _dx\n");
  fprintf(out, "hires_line_x_loop:\n");
  fprintf(out, "  lda _x1\n");
  PUSH();
  fprintf(out, "  lda _y1\n");
  PUSH();
  fprintf(out, "  lda _color\n");
  PUSH();
  fprintf(out, "  jsr hires_plot\n");
  fprintf(out, "  lda _e\n");
  fprintf(out, "  bmi hires_line_x_skip\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda _y1\n");
  fprintf(out, "  adc _iny\n");
  fprintf(out, "  sta _y1\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  lda _e\n");
  fprintf(out, "  sbc _dx\n");
  fprintf(out, "  sta _e\n");
  fprintf(out, "hires_line_x_skip:\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda _x1\n");
  fprintf(out, "  adc _inx\n");
  fprintf(out, "  sta _x1\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda _e\n");
  fprintf(out, "  adc _dy\n");
  fprintf(out, "  sta _e\n");
  fprintf(out, "  lda _x1\n");
  fprintf(out, "  cmp _x2\n");
  fprintf(out, "  bne hires_line_x_loop\n");
  fprintf(out, "  jmp hires_line_end\n");
  fprintf(out, "hires_line_y:\n");
  fprintf(out, "  asl _dx\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  lda _dx\n");
  fprintf(out, "  sbc _dy\n");
  fprintf(out, "  sta _e\n");
  fprintf(out, "  asl _dy\n");
  fprintf(out, "hires_line_y_loop:\n");
  fprintf(out, "  lda _x1\n");
  PUSH();
  fprintf(out, "  lda _y1\n");
  PUSH();
  fprintf(out, "  lda _color\n");
  PUSH();
  fprintf(out, "  jsr hires_plot\n");
  fprintf(out, "  lda _e\n");
  fprintf(out, "  bmi hires_line_y_skip\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda _x1\n");
  fprintf(out, "  adc _inx\n");
  fprintf(out, "  sta _x1\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  lda _e\n");
  fprintf(out, "  sbc _dy\n");
  fprintf(out, "  sta _e\n");
  fprintf(out, "hires_line_y_skip:\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda _y1\n");
  fprintf(out, "  adc _iny\n");
  fprintf(out, "  sta _y1\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda _e\n");
  fprintf(out, "  adc _dx\n");
  fprintf(out, "  sta _e\n");
  fprintf(out, "  lda _y1\n");
  fprintf(out, "  cmp _y2\n");
  fprintf(out, "  bne hires_line_y_loop\n");
  fprintf(out, "hires_line_end:\n");
  fprintf(out, "  lda _x1\n");
  PUSH();
  fprintf(out, "  lda _y1\n");
  PUSH();
  fprintf(out, "  lda _color\n");
  PUSH();
  fprintf(out, "  jsr hires_plot\n");
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_hires_span()
{
  fprintf(out, "hires_span:\n");
  POP();
  fprintf(out, "  sta value2\n");
  POP();
  fprintf(out, "  tay\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  jsr hires_calc_address\n");
  fprintf(out, "  lda value2\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta [address],y\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bpl #-5\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_hires_read()
{
  fprintf(out, "hires_read:\n");
  fprintf(out, "  jsr hires_calc_address\n");
  fprintf(out, "  lda [address]\n");
  fprintf(out, "  and #0xff\n");
  PUSH();
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_hires_sprite()
{
  fprintf(out, "hires_sprite:\n");
  // length
  POP();
  fprintf(out, "  sta length\n");
  // width
  POP();
  fprintf(out, "  sta value1\n");
  // y, x
  fprintf(out, "  jsr hires_calc_address\n");
  // src
  POP();
  fprintf(out, "  sta value2\n");

  fprintf(out, "  ldy #0\n");
  fprintf(out, "  phx\n");
  fprintf(out, "  ldx #0\n");

  // read from array
  fprintf(out, "hires_sprite_y_loop:\n");
  fprintf(out, "  lda (value2),y\n");
  fprintf(out, "  sta value3\n");

  // write pixel
  fprintf(out, "  phy\n");
  fprintf(out, "  txy\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  lda value3\n");
  fprintf(out, "  beq hires_sprite_skip_mask\n");
  fprintf(out, "  sta [address],y\n");
  fprintf(out, "hires_sprite_skip_mask:\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  ply\n");

  // move down a line when width is reached
  fprintf(out, "  iny\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  cpy length\n");
  fprintf(out, "  beq hires_sprite_end\n");
  fprintf(out, "  cpx value1\n");
  fprintf(out, "  bne hires_sprite_y_loop\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda address\n");
  fprintf(out, "  adc #160\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  ldx #0\n");
  fprintf(out, "  jmp hires_sprite_y_loop\n");
  fprintf(out, "hires_sprite_end:\n");
  fprintf(out, "  plx\n");
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_hires_palette()
{
  fprintf(out, "hires_palette:\n");
  POP();
  fprintf(out, "  sta value2\n");
  POP();
  fprintf(out, "  tay\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda #0x9e00\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "hires_palette_1:\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bmi hires_palette_2\n");
  fprintf(out, "  adc #32\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  jmp hires_palette_1\n");
  fprintf(out, "hires_palette_2:\n");
  fprintf(out, "  lda #0xe1\n");
  fprintf(out, "  sta address + 2\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "hires_palette_3:\n");
  fprintf(out, "  lda (value2),y \n");
  fprintf(out, "  sta [address],y\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  cpy #32\n");
  fprintf(out, "  bne hires_palette_3\n");
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_hires_set_row()
{
  fprintf(out, "hires_set_row:\n");
  POP();
  fprintf(out, "  and #15\n");
  fprintf(out, "  sta value1\n");
  POP();
  fprintf(out, "  txy\n");
  fprintf(out, "  tax\n");
  fprintf(out, "  lda value1\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta.l 0xe19d00,x\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  tyx\n");
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_hires_calc_address()
{
  fprintf(out, "hires_calc_address:\n");
  POP();
  fprintf(out, "  pha\n");
  fprintf(out, "  xba\n");
  fprintf(out, "  lsr\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  xba\n");
  fprintf(out, "  lsr\n");
  fprintf(out, "  lsr\n");
  fprintf(out, "  lsr\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  adc address\n");
  fprintf(out, "  sta address\n");
  POP();
  fprintf(out, "  clc\n");
  fprintf(out, "  adc address\n");
  fprintf(out, "  adc #0x2000\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda _bank\n");
  fprintf(out, "  sta address + 2\n");
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_rnd()
{
  fprintf(out, "rnd:\n");
  fprintf(out, "  lda _seed\n");
  PUSH();
  fprintf(out, "  lda #31421\n");
  PUSH();
  fprintf(out, "  jsr mul_integer\n");
  POP();
  fprintf(out, "  clc\n");
  fprintf(out, "  adc #6927\n");
  fprintf(out, "  sta _seed\n");
  PUSH();
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_glu()
{
  fprintf(out, "set_glu_control:\n");
  fprintf(out, "  sep #0x30\n");
  fprintf(out, "  sta 0xc03c\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  rts\n");
  fprintf(out, "set_glu_address:\n");
  fprintf(out, "  sep #0x30\n");
  fprintf(out, "  sta 0xc03e\n");
  fprintf(out, "  xba\n");
  fprintf(out, "  sta 0xc03f\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  rts\n");
  fprintf(out, "set_glu_data:\n");
  fprintf(out, "  sep #0x30\n");
  fprintf(out, "  sta 0xc03d\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  rts\n");
}

