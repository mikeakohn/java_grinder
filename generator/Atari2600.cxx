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

#include "Atari2600.h"

// http://www.alienbill.com/2600/101/docs/stella.html

Atari2600::Atari2600()
{
  start_org = 0xf000;
  java_stack_lo = 0x80;
  java_stack_hi = 0x90;
  ram_start = 0xa0;
}

Atari2600::~Atari2600()
{
}

int Atari2600::open(const char *filename)
{
  if (M6502::open(filename) != 0) { return -1; }

  fprintf(out, "; set java stack pointer (x register)\n");
  fprintf(out, "  ldx #0x0F\n");
  print_tia_definitions();

  return 0;
}

int Atari2600::atari2600_waitHsync()
{
  fprintf(out, "  sta WSYNC\n");

  return 0;
}

int Atari2600::atari2600_vsyncOn()
{
  fprintf(out, "  lda #0x02\n");
  fprintf(out, "  sta VSYNC\n");

  return 0;
}

int Atari2600::atari2600_vsyncOff()
{
  fprintf(out, "  lda #0x00\n");
  fprintf(out, "  sta VSYNC\n");

  return 0;
}

int Atari2600::atari2600_vblankOn()
{
  fprintf(out, "  lda #0x02\n");
  fprintf(out, "  sta VBLANK\n");

  return 0;
}

int Atari2600::atari2600_vblankOff()
{
  fprintf(out, "  lda #0x00\n");
  fprintf(out, "  sta VBLANK\n");

  return 0;
}

int Atari2600::atari2600_setColorPlayer0()
{
  return -1;
}

int Atari2600::atari2600_setColorPlayer0(int color)
{
  if (color < 0 || color > 255)
  {
    printf("Error: Color index out of range\n");
    return -1;
  }

  fprintf(out, "  lda #0x%02x\n", color);
  fprintf(out, "  sta COLUP0\n");

  return 0;
}

int Atari2600::atari2600_setColorPlayer1()
{
  return -1;
}

int Atari2600::atari2600_setColorPlayer1(int color)
{
  if (color < 0 || color > 255)
  {
    printf("Error: Color index out of range\n");
    return -1;
  }

  fprintf(out, "  lda #0x%02x\n", color);
  fprintf(out, "  sta COLUP1\n");

  return 0;
}

int Atari2600::atari2600_setColorPlayfield()
{
  return -1;
}

int Atari2600::atari2600_setColorPlayfield(int color)
{
  if (color < 0 || color > 255)
  {
    printf("Error: Color index out of range\n");
    return -1;
  }

  fprintf(out, "  lda #0x%02x\n", color);
  fprintf(out, "  sta COLUPF\n");

  return 0;
}

int Atari2600::atari2600_setColorBackground()
{
  return -1;
}

int Atari2600::atari2600_setColorBackground(int color)
{
  if (color < 0 || color > 255)
  {
    printf("Error: Color index out of range\n");
    return -1;
  }

  fprintf(out, "  lda #0x%02x\n", color);
  fprintf(out, "  sta COLUBK\n");

  return 0;
}

void Atari2600::print_tia_definitions()
{
  fprintf(out, "  VSYNC equ 0x00\n");
  fprintf(out, "  VBLANK equ 0x01\n");
  fprintf(out, "  WSYNC equ 0x02\n");
  fprintf(out, "  RSYNC equ 0x03\n");
  fprintf(out, "  NUSIZ0 equ 0x04\n");
  fprintf(out, "  NUSIZ1 equ 0x05\n");
  fprintf(out, "  COLUP0 equ 0x06\n");
  fprintf(out, "  COLUP1 equ 0x07\n");
  fprintf(out, "  COLUPF equ 0x08\n");
  fprintf(out, "  COLUBK equ 0x09\n");
  fprintf(out, "  CTRLPF equ 0x0A\n");
  fprintf(out, "  REFP0 equ 0x0B\n");
  fprintf(out, "  REFP1 equ 0x0C\n");
  fprintf(out, "  PF0 equ 0x0D\n");
  fprintf(out, "  PF1 equ 0x0E\n");
  fprintf(out, "  PF2 equ 0x0F\n");
  fprintf(out, "  RESP0 equ 0x10\n");
  fprintf(out, "  RESP1 equ 0x11\n");
  fprintf(out, "  RESM0 equ 0x12\n");
  fprintf(out, "  RESM1 equ 0x13\n");
  fprintf(out, "  RESBL equ 0x14\n");
  fprintf(out, "  AUDC0 equ 0x15\n");
  fprintf(out, "  AUDC1 equ 0x16\n");
  fprintf(out, "  AUDF0 equ 0x17\n");
  fprintf(out, "  AUDF1 equ 0x18\n");
  fprintf(out, "  AUDV0 equ 0x19\n");
  fprintf(out, "  AUDV1 equ 0x1A\n");
  fprintf(out, "  GRP0 equ 0x1B\n");
  fprintf(out, "  GRP1 equ 0x1C\n");
  fprintf(out, "  ENAM0 equ 0x1D\n");
  fprintf(out, "  ENAM1 equ 0x1E\n");
  fprintf(out, "  ENABL equ 0x1F\n");
  fprintf(out, "  HMP0 equ 0x20\n");
  fprintf(out, "  HMP1 equ 0x21\n");
  fprintf(out, "  HMM0 equ 0x22\n");
  fprintf(out, "  HMM1 equ 0x23\n");
  fprintf(out, "  HMBL equ 0x24\n");
  fprintf(out, "  VDELP0 equ 0x25\n");
  fprintf(out, "  VDELP1 equ 0x26\n");
  fprintf(out, "  VDELBL equ 0x27\n");
  fprintf(out, "  RESMP0 equ 0x28\n");
  fprintf(out, "  RESMP1 equ 0x29\n");
  fprintf(out, "  HMOVE equ 0x2A\n");
  fprintf(out, "  HMCLR equ 0x2B\n");
  fprintf(out, "  CXCLR equ 0x2C\n");
}

