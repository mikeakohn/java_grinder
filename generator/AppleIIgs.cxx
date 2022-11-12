/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2019 by Michael Kohn, Joe Davisson
 *
 * Apple IIgs written by Joe Davisson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "generator/AppleIIgs.h"

AppleIIgs::AppleIIgs() :
//  need_plot_char(0),
//  need_print_char(0),
  need_hires_enable(0),
  need_hires_clear(0),
  need_hires_update(0),
//  need_hires_plot(0),
  need_hires_line(0),
  need_hires_span(0),
  need_hires_read(0),
  need_hires_sprite(0),
  need_hires_palette(0),
  need_hires_set_row(0),
  need_rnd(0)
{
  start_org = 0x1000;
  ram_start = 0x7000;
}

AppleIIgs::~AppleIIgs()
{
}

int AppleIIgs::open(const char *filename)
{
  if (W65816::open(filename) != 0) { return -1; }

  // hires row table
  fprintf(out, "_row equ 0x7d00\n");

  // for line drawing
  fprintf(out, "_vars equ 0x7f00\n"); // was 0x7f00
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
  fprintf(out, "lda #0xace1\n");
  fprintf(out, "sta _seed\n");

  // graphics bank
  fprintf(out, "_bank equ _vars + 24\n");

  // turn off interrupts?
//  fprintf(out, "  sei\n");
//  fprintf(out, "  cld\n");

  fprintf(out, "; set up processor stack\n");
  fprintf(out, "  lda #0x1ff\n");
  fprintf(out, "  tcs\n");

  // fill row table
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda #0x2000\n");
  fprintf(out, "fill_row_table_loop:\n");
  fprintf(out, "  sta _row,y\n");
  fprintf(out, "  adc #160\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  cpy #400\n");
  fprintf(out, "  bmi fill_row_table_loop\n");

  // shadowing off
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  lda 0xc035\n");
  fprintf(out, "  ora.b #14\n");
  fprintf(out, "  sta 0xc035\n");
  fprintf(out, "  rep #0x30\n");

  return 0;
}

int AppleIIgs::finish()
{
  if (need_hires_enable) { insert_hires_enable(); }
  if (need_hires_clear) { insert_hires_clear(); }
  if (need_hires_update) { insert_hires_update(); }
//  if (need_hires_plot) { insert_hires_plot(); }
  if (need_hires_line) { insert_hires_line(); }
  if (need_hires_span) { insert_hires_span(); }
  if (need_hires_read) { insert_hires_read(); }
  if (need_hires_sprite) { insert_hires_sprite(); }
  if (need_hires_palette) { insert_hires_palette(); }
  if (need_hires_set_row) { insert_hires_set_row(); }
  if (need_rnd) { insert_rnd(); }
  insert_hires_calc_address();
  insert_glu();

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
  fprintf(out, "  pla\n");
  fprintf(out, "  phx\n");
  fprintf(out, "  ora #0x80\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  xce\n");
  fprintf(out, "  jsr 0xfded\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  xce\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  plx\n");

  return 0;
}

int AppleIIgs::appleiigs_hiresEnable_I()
{
  need_hires_enable = true;
  fprintf(out, "  jsr hires_enable\n");

  return 0;
}

int AppleIIgs::appleiigs_hiresClear_I()
{
  need_hires_clear = true;
  fprintf(out, "  jsr hires_clear\n");

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
//  fprintf(out, "fuck_%d:\n", label_count);
//  fprintf(out, "  jmp fuck_%d\n", label_count++);

  fprintf(out, "  lda _bank\n");
  fprintf(out, "  sta address2\n");
  fprintf(out, "  pla \n");
  fprintf(out, "  sta _color\n");
  fprintf(out, "  pla \n");
  fprintf(out, "  sta _y1\n");
  fprintf(out, "  pla \n");
  fprintf(out, "  sta _x1\n");
  hires_plot();
  return 0;
}

int AppleIIgs::appleiigs_hiresLine_IIIII()
{
  need_hires_line = true;
  fprintf(out, "  jsr hires_line\n");

  return 0;
}

int AppleIIgs::appleiigs_hiresSpan_IIII()
{
  need_hires_span = true;
  fprintf(out, "  jsr hires_span\n");

  return 0;
}

int AppleIIgs::appleiigs_hiresRead_II()
{
  need_hires_read = true;
  fprintf(out, "  jsr hires_read\n");

  return 0;
}

int AppleIIgs::appleiigs_hiresSprite_aBIIII()
{
  need_hires_sprite = true;
  fprintf(out, "  jsr hires_sprite\n");
  
  return 0;
}

int AppleIIgs::appleiigs_hiresPalette_IaI()
{
  need_hires_palette = true;
  fprintf(out, "  jsr hires_palette\n");

  return 0;
}

int AppleIIgs::appleiigs_hiresSetRow_II()
{
  need_hires_set_row = true;
  fprintf(out, "  jsr hires_set_row\n");

  return 0;
}

int AppleIIgs::appleiigs_rnd()
{
  need_rnd = true;
  fprintf(out, "  jsr rnd\n");

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
  fprintf(out, "  pla\n");
  fprintf(out, "  sta address2\n");
  // load wavetable
  fprintf(out, "  pla\n");
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
  fprintf(out, "  pla\n");
  fprintf(out, "asl\n");
  fprintf(out, "jsr set_glu_data\n");

  return 0;
}

int AppleIIgs::appleiigs_setMasterVolume_I()
{
  fprintf(out, "; set_master_volume\n");
  // set DOC master volume
  fprintf(out, "  pla\n");
  fprintf(out, "jsr set_glu_control\n");

  return 0;
}

int AppleIIgs::appleiigs_setSoundVolume_II()
{
  fprintf(out, "; set_sound_volume\n");
  // point to DOC
  fprintf(out, "lda #0\n");
  fprintf(out, "jsr set_glu_control\n");
  // set volume
  fprintf(out, "  pla\n");
  fprintf(out, "tay\n");
  fprintf(out, "  pla\n");
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

  return 0;
}

// formula is:
// SR = 894886 / (OSC + 2)
// value = (131072 * Hz) / SR
//
// where SR = scan rate, OSC = number of enabled oscillators
int AppleIIgs::appleiigs_setSoundFrequency_II()
{
  fprintf(out, "; set_sound_frequency\n");
  // point to DOC
  fprintf(out, "lda #0\n");
  fprintf(out, "jsr set_glu_control\n");
  // set voice
  fprintf(out, "pla\n");
  fprintf(out, "tay\n");
  fprintf(out, "pla\n");
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

  return 0;
}

int AppleIIgs::appleiigs_setWaveTable_III()
{
  fprintf(out, "; set_wave_table\n");
  // point to DOC
  fprintf(out, "lda #0\n");
  fprintf(out, "jsr set_glu_control\n");
  fprintf(out, "pla\n");
  fprintf(out, "sta value2\n");
  fprintf(out, "pla\n");
  fprintf(out, "sta value1\n");
  fprintf(out, "pla\n");
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
  fprintf(out, "jsr set_glu_address\n");
  fprintf(out, "lda value2\n");
  fprintf(out, "asl\n");
  fprintf(out, "asl\n");
  fprintf(out, "asl\n");
  fprintf(out, "jsr set_glu_data\n");

  return 0;
}

// subroutines
void AppleIIgs::insert_hires_enable()
{
  fprintf(out, "hires_enable:\n");
  save_return();
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  lda.b #10000001b\n");
  fprintf(out, "  sta 0xc029\n");
  fprintf(out, "  lda.b #0\n");
  fprintf(out, "  sta 0xc034\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta _bank\n");
  restore_return();
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_hires_clear()
{
/*
  fprintf(out, "hires_clear:\n");
  SAVE();
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  lda _bank\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  plb\n");
  fprintf(out, "  rep #0x30\n");
  POP();
  fprintf(out, "  sta value1\n");
  fprintf(out, "  xba\n");
  fprintf(out, "  ora value1\n");
  fprintf(out, "  ldx #31998\n");
  fprintf(out, "hires_clear_loop:\n");
  fprintf(out, "  sta 0x2000,x\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  bne hires_clear_loop\n");

  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  lda #0x00\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  plb\n");
  fprintf(out, "  rep #0x30\n");
  restore_return();
  fprintf(out, "  rts\n");
*/


  fprintf(out, "hires_clear:\n");
  save_return();
  fprintf(out, "  lda #0x2000\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda _bank\n");
  fprintf(out, "  sta address2\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta value1\n");
  fprintf(out, "  xba\n");
  fprintf(out, "  ora value1\n");
  fprintf(out, "  ldy #31998\n");
  fprintf(out, "hires_clear_loop:\n");
  fprintf(out, "  sta [address],y\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bpl hires_clear_loop\n");
  restore_return();
  fprintf(out, "  rts\n");

}

void AppleIIgs::insert_hires_update()
{
  fprintf(out, "hires_update:\n");
  save_return();
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  lda 0xc035\n");
  fprintf(out, "  and.b #241\n");
  fprintf(out, "  sta 0xc035\n");
  fprintf(out, "  lda.b #0x01\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  plb\n");
  fprintf(out, "  rep #0x30\n");

  fprintf(out, "  pla\n");
  fprintf(out, "  sta value1\n");
  fprintf(out, "  plx\n");
  fprintf(out, "hires_update_loop:\n");
  fprintf(out, "  lda 0x2000,x\n");
  fprintf(out, "  sta 0x2000,x\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  cpx value1\n");
  fprintf(out, "  bmi hires_update_loop\n");

  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  lda 0xc035\n");
  fprintf(out, "  ora.b #14\n");
  fprintf(out, "  sta 0xc035\n");
  fprintf(out, "  lda.b #0x00\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  plb\n");
  fprintf(out, "  rep #0x30\n");
  restore_return();
  fprintf(out, "  rts\n");
}

/*
void AppleIIgs::insert_hires_plot()
{
  fprintf(out, "hires_plot:\n");
  save_return();
  POP();
  fprintf(out, "  tay\n");
  fprintf(out, "  jsr hires_calc_address\n");
  fprintf(out, "  tya\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta [address]\n");
  fprintf(out, "  rep #0x30\n");
  restore_return();
  fprintf(out, "  rts\n");
}
*/

void AppleIIgs::insert_hires_line()
{
  fprintf(out, "hires_line:\n");
  save_return();
  fprintf(out, "  lda _bank\n");
  fprintf(out, "  sta address2\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta _color\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta _y2\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta _x2\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta _y1\n");
  fprintf(out, "  pla\n");
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
  hires_plot();
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
  hires_plot();
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
  hires_plot();
  restore_return();
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_hires_span()
{
  fprintf(out, "hires_span:\n");
  save_return();
  fprintf(out, "  pla\n");
  fprintf(out, "  sta _color\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  inc\n");
  fprintf(out, "  sta _x2\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  tax\n");
  fprintf(out, "  lda _row,x\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tay\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  lda _color\n");
  fprintf(out, "hires_span_loop:\n");
  fprintf(out, "  sta [address],y\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  cpy _x2\n");
  fprintf(out, "  bmi hires_span_loop\n");
  fprintf(out, "  rep #0x30\n");
  restore_return();
  fprintf(out, "  rts\n");

/*
  fprintf(out, "hires_span:\n");
  save_return();
  POP();
  fprintf(out, "  sta value2\n");
  POP();
  fprintf(out, "  tay\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  jsr hires_calc_address\n");
  fprintf(out, "  lda value2\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "hires_span_loop:\n");
  fprintf(out, "  sta [address],y\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bpl hires_span_loop\n");
  fprintf(out, "  rep #0x30\n");
  restore_return();
  fprintf(out, "  rts\n");
*/
}

void AppleIIgs::insert_hires_read()
{
  fprintf(out, "hires_read:\n");
  save_return();
  fprintf(out, "  jsr hires_calc_address\n");
  fprintf(out, "  lda [address]\n");
  fprintf(out, "  and #0xff\n");
  fprintf(out, "  pha\n");
  restore_return();
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_hires_sprite()
{
  fprintf(out, "hires_sprite:\n");
  save_return();
  // length
  fprintf(out, "  pla\n");
  fprintf(out, "  sta length\n");
  // width
  fprintf(out, "  pla\n");
  fprintf(out, "  sta value1\n");
  // y, x
  fprintf(out, "  jsr hires_calc_address\n");
  // src
  fprintf(out, "  pla\n");
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
  restore_return();
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_hires_palette()
{
  fprintf(out, "hires_palette:\n");
  save_return();
  fprintf(out, "  lda #0x9e00\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda _bank\n");
  fprintf(out, "  sta address2\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta value2\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tay\n");
  fprintf(out, "  clc\n");
  fprintf(out, "hires_palette_1:\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bmi hires_palette_2\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda address\n");
  fprintf(out, "  adc #32\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  jmp hires_palette_1\n");
  fprintf(out, "hires_palette_2:\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "hires_palette_3:\n");
  fprintf(out, "  lda (value2),y \n");
  fprintf(out, "  sta [address],y\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  cpy #32\n");
  fprintf(out, "  bmi hires_palette_3\n");
  restore_return();
  fprintf(out, "  rts\n");

/*
  fprintf(out, "hires_palette:\n");
  save_return();
  POP();
  fprintf(out, "  sta value2\n");
  POP();
  fprintf(out, "  tay\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda #0x9e00\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda #0xe1\n");
  fprintf(out, "  sta address2\n");
//  fprintf(out, "hires_palette_1:\n");
//  fprintf(out, "  dey\n");
//  fprintf(out, "  bmi hires_palette_2\n");
//  fprintf(out, "  adc #32\n");
//  fprintf(out, "  sta address\n");
//  fprintf(out, "  jmp hires_palette_1\n");
  fprintf(out, "hires_palette_2:\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "hires_palette_3:\n");
  fprintf(out, "  lda (value2),y \n");
  fprintf(out, "  sta [address],y\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  cpy #32\n");
  fprintf(out, "  bne hires_palette_3\n");
  restore_return();
  fprintf(out, "  rts\n");
*/
}

void AppleIIgs::insert_hires_set_row()
{
  fprintf(out, "hires_set_row:\n");
  save_return();
  fprintf(out, "  pla\n");
  fprintf(out, "  and #15\n");
  fprintf(out, "  sta value1\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  tax\n");
  fprintf(out, "  lda value1\n");
  fprintf(out, "  sep #0x20\n");
//  fprintf(out, "  lda.l 0xe19d00,x\n");
//  fprintf(out, "  ora.b value1\n");
  fprintf(out, "  lda.b value1\n");
  fprintf(out, "  sta.l 0xe19d00,x\n");
  fprintf(out, "  rep #0x30\n");
  restore_return();
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_hires_calc_address()
{
  fprintf(out, "hires_calc_address:\n");
  save_return();
  fprintf(out, "  pla\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  tax\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  adc _row,x\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda _bank\n");
  fprintf(out, "  sta address2\n");
  restore_return();
  fprintf(out, "  rts\n");
}

/*
  static int seed = 12345;

  public static int rnd()
  {
    seed ^= seed << 9;
    seed ^= seed >> 5;
    seed ^= seed << 3;

    return seed;
  }
*/

void AppleIIgs::insert_rnd()
{
  fprintf(out, "rnd:\n");
  save_return();
  fprintf(out, "  lda _seed\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  lda #31421\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  jsr mul_integer\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  adc #6927\n");
  fprintf(out, "  sta _seed\n");
  fprintf(out, "  pha\n");
  restore_return();
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_glu()
{
  fprintf(out, "set_glu_control:\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta 0xc03c\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  rts\n");

  fprintf(out, "set_glu_address:\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta 0xc03e\n");
  fprintf(out, "  xba\n");
  fprintf(out, "  sta 0xc03f\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  rts\n");

  fprintf(out, "set_glu_data:\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta 0xc03d\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  rts\n");
}

void AppleIIgs::hires_plot()
{
  fprintf(out, "; hires_plot\n");
  fprintf(out, "  lda _y1\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  tax\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda _x1\n");
  fprintf(out, "  adc _row,x\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda _color\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta [address]\n");
  fprintf(out, "  rep #0x30\n");
}


