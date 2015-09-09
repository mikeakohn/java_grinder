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

AppleIIgs::AppleIIgs()
{
  start_org = 0x1000;
  java_stack = 0x800;
  ram_start = 0x7000;
}

AppleIIgs::~AppleIIgs()
{
}

int AppleIIgs::open(const char *filename)
{
  if (W65816::open(filename) != 0) { return -1; }

  return 0;
}

int AppleIIgs::appleiigs_plotChar_IC()
{
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
  fprintf(out, ";; hiresEnable()\n");
  fprintf(out, "  sep #0x30\n");
  fprintf(out, "  lda.b #10000001b\n");
  fprintf(out, "  sta.l 0xe1c029\n");
  fprintf(out, "  lda.b #0\n");
  fprintf(out, "  sta.l 0xe1c034\n");
  fprintf(out, "  rep #0x30\n");

  return 0;
}

// hiresPlot(address, color) - plot pixel
int AppleIIgs::appleiigs_hiresPlot_II()
{
  fprintf(out, ";; hiresPlot()\n");
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

  stack -= 2;

  return 0;
}

// hiresSpan(address, color, width) - draws horizontal line
int AppleIIgs::appleiigs_hiresSpan_III()
{
  fprintf(out, ";; hiresSpan()\n");
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

  stack -= 3;

  return 0;
}

// hiresRead(address) - read pixel color
int AppleIIgs::appleiigs_hiresRead_I()
{
  fprintf(out, ";; hiresRead()\n");
  POP();
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda #0xe1\n");
  fprintf(out, "  sta address + 2\n");
  fprintf(out, "  lda [address]\n");
  fprintf(out, "  and #0xff\n");
  PUSH();

  return 0;
}

// hiresBlit(byte[] source, int dest, int width, int length)
// draw an image, length is source.length
int AppleIIgs::appleiigs_hiresBlit_aBIII()
{
  fprintf(out, ";; hiresBlit()\n");
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
  
  stack -= 4;
  label_count += 2;

  return 0;
}

// hiresPalette(byte[] source) - change palette
//FIXME need to be able to set all 16 palettes and choose between
int AppleIIgs::appleiigs_hiresPalette_aI()
{
  fprintf(out, ";; hiresPalette()\n");
  POP();
  fprintf(out, "  sta value2\n");
  fprintf(out, "  lda #0x9e00\n");
  fprintf(out, "  sta address\n");
  fprintf(out, "  lda #0xe1\n");
  fprintf(out, "  sta address + 2\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "label_%d:\n", label_count);
  fprintf(out, "  lda (value2),y \n");
  fprintf(out, "  sta [address],y\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  cpy #32\n");
  fprintf(out, "  bne label_%d\n", label_count);

  stack--;
  label_count++;

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

