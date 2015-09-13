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
  need_hires_plot(false),
  need_hires_span(false),
  need_hires_read(false),
  need_hires_blit(false),
  need_hires_palette(false),
  need_hires_set_line(false)
{
  start_org = 0x1000;
  java_stack = 0x800;
  ram_start = 0x7000;
}

AppleIIgs::~AppleIIgs()
{
  if (need_hires_enable) { insert_hires_enable(); }
  if (need_hires_plot) { insert_hires_plot(); }
  if (need_hires_span) { insert_hires_span(); }
  if (need_hires_read) { insert_hires_read(); }
  if (need_hires_blit) { insert_hires_blit(); }
  if (need_hires_palette) { insert_hires_palette(); }
  if (need_hires_set_line) { insert_hires_set_line(); }
}

int AppleIIgs::open(const char *filename)
{
  if (W65816::open(filename) != 0) { return -1; }

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

// printChar(char c) - output char to text screen
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

// hiresEnable() - enable super-hires screen
int AppleIIgs::appleiigs_hiresEnable()
{
  need_hires_enable = true;
  fprintf(out, "jsr hires_enable\n");

  return 0;
}

// hiresPlot(address, color) - plot pixel
int AppleIIgs::appleiigs_hiresPlot_II()
{
  need_hires_plot = true;
  fprintf(out, "jsr hires_plot\n");

  stack -= 2;

  return 0;
}

// hiresSpan(address, color, width) - draws horizontal line
int AppleIIgs::appleiigs_hiresSpan_III()
{
  need_hires_span = true;
  fprintf(out, "jsr hires_span\n");

  stack -= 3;

  return 0;
}

// hiresRead(address) - read pixel color
int AppleIIgs::appleiigs_hiresRead_I()
{
  need_hires_read = true;
  fprintf(out, "jsr hires_read\n");

  return 0;
}

// hiresBlit(byte[] source, int dest, int width, int length)
// draw an image, width is in *bytes* length is source.length
int AppleIIgs::appleiigs_hiresBlit_aBIII()
{
  need_hires_blit = true;
  fprintf(out, "jsr hires_blit\n");
  
  stack -= 4;
  label_count += 2;

  return 0;
}

// hiresPalette(int palette, byte[] source) - change palette
int AppleIIgs::appleiigs_hiresPalette_IaI()
{
  need_hires_palette = true;
  fprintf(out, "jsr hires_palette\n");

  stack -= 2;
  label_count += 3;

  return 0;
}

// hiresSetLine(int line, int palette) - set palette for line
int AppleIIgs::appleiigs_hiresSetLine_II()
{
  need_hires_set_line = true;
  fprintf(out, "jsr hires_set_line\n");

  stack -= 2;

  return 0;
}

// Sound API
int AppleIIgs::appleiigs_loadWaveTable_BA()
{
  return -1;
}

int AppleIIgs::appleiigs_enableOscillators_I()
{
  return -1;
}

int AppleIIgs::appleiigs_setMasterVolume_I()
{
  return -1;
}

int AppleIIgs::appleiigs_setSoundVolume_II()
{
  return -1;
}

int AppleIIgs::appleiigs_setSoundFrequency_II()
{
  return -1;
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
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_hires_plot()
{
  fprintf(out, "hires_plot:\n");
  POP();
  fprintf(out, "  tay\n");
  POP();
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda #0xe1\n");
  fprintf(out, "  sta address + 2\n");
  fprintf(out, "  tya\n");
  fprintf(out, "  sep #0x30\n");
  fprintf(out, "  sta [address]\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_hires_span()
{
  fprintf(out, "hires_span:\n");
  POP();
  fprintf(out, "  tay\n");
  fprintf(out, "  dey\n");
  POP();
  fprintf(out, "  sta value1\n");
  POP();
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda #0xe1\n");
  fprintf(out, "  sta address + 2\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  lda value1\n");
  fprintf(out, "  sta [address],y\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bpl #-5\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_hires_read()
{
  fprintf(out, "hires_read:\n");
  POP();
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda #0xe1\n");
  fprintf(out, "  sta address + 2\n");
  fprintf(out, "  lda [address]\n");
  fprintf(out, "  and #0xff\n");
  PUSH();
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_hires_blit()
{
  fprintf(out, "hires_blit:\n");
  // length
  POP();
  fprintf(out, "  sta length\n");
  // width
  POP();
  fprintf(out, "  sta value1\n");
  // dest
  POP();
  fprintf(out, "  sta address\n");
  // src
  POP();
  fprintf(out, "  sta value2\n");

  fprintf(out, "  ldy #0\n");
  fprintf(out, "  phx\n");
  fprintf(out, "  ldx #0\n");

  // read from array
  fprintf(out, "label_%d:\n", label_count + 0);
  fprintf(out, "  lda (value2),y\n");
  fprintf(out, "  sta value3\n");

  // write pixel
  fprintf(out, "  lda #0xe1\n");
  fprintf(out, "  sta address + 2\n");
  fprintf(out, "  phy\n");
  fprintf(out, "  txy\n");
  fprintf(out, "  lda value3\n");
  fprintf(out, "  sep #0x20\n");
  fprintf(out, "  sta [address],y\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "  ply\n");

  // move down a line when width is reached
  fprintf(out, "  iny\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  cpy length\n");
  fprintf(out, "  beq label_%d\n", label_count + 1);
  fprintf(out, "  cpx value1\n");
  fprintf(out, "  bne label_%d\n", label_count + 0);
  fprintf(out, "  clc\n");
  fprintf(out, "  lda address\n");
  fprintf(out, "  adc #160\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  ldx #0\n");
  fprintf(out, "  jmp label_%d\n", label_count + 0);
  fprintf(out, "label_%d:\n", label_count + 1);
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
  fprintf(out, "label_%d:\n", label_count + 0);
  fprintf(out, "  dey\n");
  fprintf(out, "  bmi label_%d\n", label_count + 1);
  fprintf(out, "  adc #32\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  jmp label_%d\n", label_count + 0);
  fprintf(out, "label_%d:\n", label_count + 1);
  fprintf(out, "  lda #0xe1\n");
  fprintf(out, "  sta address + 2\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "label_%d:\n", label_count + 2);
  fprintf(out, "  lda (value2),y \n");
  fprintf(out, "  sta [address],y\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  cpy #32\n");
  fprintf(out, "  bne label_%d\n", label_count + 2);
  fprintf(out, "  rts\n");
}

void AppleIIgs::insert_hires_set_line()
{
  fprintf(out, "hires_set_line:\n");
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

