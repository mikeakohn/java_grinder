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
  fprintf(out, ".org 0xfffa\n");
  fprintf(out, "; NMI\n");
  fprintf(out, "dw reset\n");
  fprintf(out, "; RESET\n");
  fprintf(out, "dw reset\n");
  fprintf(out, "; IRQ\n");
  fprintf(out, "dw reset\n");
}

int Atari2600::open(const char *filename)
{
  if (M6502::open(filename) != 0) { return -1; }

  print_tia_definitions();
  print_pia_definitions();

  // Is this needed?
  fprintf(out, "; clear TIA\n");
  fprintf(out, "  sei\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  ldx #127\n");
  fprintf(out, "_clear_tia:\n");
  fprintf(out, "  sta 0,x\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  bne _clear_tia\n");

  fprintf(out, "; set java stack pointer (x register)\n");
  fprintf(out, "  ldx #0x0F\n");

  return 0;
}

int Atari2600::atari2600_waitHsync_I()
{
  return -1;
}

int Atari2600::atari2600_waitHsync_I(int lines)
{
  if (lines == 1) { return atari2600_waitHsync(); }

  fprintf(out, "; waitLines(%d)\n", lines);
  fprintf(out, "  ldx #0x%02x\n", lines);
  fprintf(out, "_wait_lines_%d:\n", label_count);
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  bne _wait_lines_%d\n", label_count);

  label_count++;

  return 0;
}

int Atari2600::atari2600_waitHsync()
{
  fprintf(out, "  sta WSYNC\n");

  return 0;
}

int Atari2600::atari2600_startVblank()
{
  fprintf(out, "  lda #0x02\n");
  fprintf(out, "  sta VSYNC\n");
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  lda #0x00\n");
  fprintf(out, "  sta VSYNC\n");

  fprintf(out, "  lda #43\n");
  fprintf(out, "  sta TIM64T\n");

  return 0;
}

int Atari2600::atari2600_waitVblank()
{
  fprintf(out, "_vblank_wait_%d:\n", label_count);
  fprintf(out, "  lda INTIM\n");
  fprintf(out, "  bne _vblank_wait_%d\n", label_count);

  fprintf(out, "  lda #0x00\n");
  //fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  sta VBLANK\n");

  label_count++;

  return 0;
}

int Atari2600::atari2600_startOverscan()
{
  fprintf(out, "  lda #0x02\n");
  fprintf(out, "  sta VBLANK\n");

  return 0;
}

int Atari2600::atari2600_waitOverscan()
{
  fprintf(out, "  ldx #30\n");
  fprintf(out, "_overscan_wait_%d:\n", label_count);
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  bne _overscan_wait_%d\n", label_count);

  label_count++;

  return 0;
}

int Atari2600::atari2600_setColorPlayer0_I()
{
  return -1;
}

int Atari2600::atari2600_setColorPlayer0_I(int color)
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

int Atari2600::atari2600_setColorPlayer1_I()
{
  return -1;
}

int Atari2600::atari2600_setColorPlayer1_I(int color)
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

int Atari2600::atari2600_setColorPlayfield_I()
{
  fprintf(out, "; setColorPlayfield_I\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  sta COLUPF\n");
  return 0;
}

int Atari2600::atari2600_setColorPlayfield_I(int color)
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

int Atari2600::atari2600_setColorBackground_I()
{
  fprintf(out, "; setColorBackground_I\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  sta COLUBK\n");
  return 0;
}

int Atari2600::atari2600_setColorBackground_I(int color)
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

int Atari2600::atari2600_resetPlayer0()
{
  fprintf(out, "sta RESP0\n");
  return 0;
}

int Atari2600::atari2600_resetPlayer1()
{
  fprintf(out, "sta RESP1\n");
  return 0;
}

int Atari2600::atari2600_resetMissile0()
{
  fprintf(out, "sta RESM0\n");
  return 0;
}

int Atari2600::atari2600_resetMissile1()
{
  fprintf(out, "sta RESM1\n");
  return 0;
}

int Atari2600::atari2600_resetBall()
{
  fprintf(out, "sta RESBL\n");
  return 0;
}

int Atari2600::atari2600_resetMissile0ToPlayer0On()
{
  fprintf(out, "lda #0x02\n");
  fprintf(out, "sta RESMP0\n");
  return 0;
}

int Atari2600::atari2600_resetMissile1ToPlayer1Off()
{
  fprintf(out, "lda #0x00\n");
  fprintf(out, "sta RESMP1\n");
  return 0;
}

int Atari2600::atari2600_resetMissile0ToPlayer0Off()
{
  fprintf(out, "lda #0x00\n");
  fprintf(out, "sta RESMP0\n");
  return 0;
}

int Atari2600::atari2600_resetMissile1ToPlayer1On()
{
  fprintf(out, "lda #0x02\n");
  fprintf(out, "sta RESMP1\n");
  return 0;
}

int Atari2600::atari2600_applyHorizontalMotion()
{
  fprintf(out, "sta HMOVE\n");
  return 0;
}

int Atari2600::atari2600_clearMotionRegisters()
{
  fprintf(out, "sta HCLR\n");
  return 0;
}

int Atari2600::atari2600_clearCollisionLatches()
{
  fprintf(out, "sta CXCLR\n");
  return 0;
}

int Atari2600::atari2600_enableMissile0()
{
  fprintf(out, "  lda #0x02\n");
  fprintf(out, "  sta ENAM0\n");
  return 0;
}

int Atari2600::atari2600_enableMissile1()
{
  fprintf(out, "  lda #0x02\n");
  fprintf(out, "  sta ENAM1\n");
  return 0;
}

int Atari2600::atari2600_enableBall()
{
  fprintf(out, "  lda #0x02\n");
  fprintf(out, "  sta ENABL\n");
  return 0;
}

int Atari2600::atari2600_disableMissile0()
{
  fprintf(out, "  lda #0x00\n");
  fprintf(out, "  sta ENAM0\n");
  return 0;
}

int Atari2600::atari2600_disableMissile1()
{
  fprintf(out, "  lda #0x00\n");
  fprintf(out, "  sta ENAM1\n");
  return 0;
}

int Atari2600::atari2600_disableBall()
{
  fprintf(out, "  lda #0x00\n");
  fprintf(out, "  sta ENABL\n");
  return 0;
}

int Atari2600::atari2600_enablePlayer0Reflect()
{
  fprintf(out, "  lda #0x08\n");
  fprintf(out, "  sta REFP0\n");
  return 0;
}

int Atari2600::atari2600_enablePlayer1Reflect()
{
  fprintf(out, "  lda #0x08\n");
  fprintf(out, "  sta REFP1\n");
  return 0;
}

int Atari2600::atari2600_enablePlayfieldReflect()
{
  fprintf(out, "  lda CTRLPF\n");
  fprintf(out, "  ora #0x01\n");
  fprintf(out, "  sta CTRLPF\n");
  return 0;
}

int Atari2600::atari2600_disablePlayer0Reflect()
{
  fprintf(out, "  lda #0x00\n");
  fprintf(out, "  sta REFP0\n");
  return 0;
}

int Atari2600::atari2600_disablePlayer1Reflect()
{
  fprintf(out, "  lda #0x00\n");
  fprintf(out, "  sta REFP1\n");
  return 0;
}

int Atari2600::atari2600_disablePlayfieldReflect()
{
  fprintf(out, "  lda CTRLPF\n");
  fprintf(out, "  and #0xfe\n");
  fprintf(out, "  sta CTRLPF\n");
  return 0;
}

int Atari2600::atari2600_enablePlayfieldScoreColor()
{
  fprintf(out, "  lda CTRLPF\n");
  fprintf(out, "  ora #0x02\n");
  fprintf(out, "  sta CTRLPF\n");
  return 0;
}

int Atari2600::atari2600_disablePlayfieldScoreColor()
{
  fprintf(out, "  lda CTRLPF\n");
  fprintf(out, "  and #0xfd\n");
  fprintf(out, "  sta CTRLPF\n");
  return 0;
}

int Atari2600::atari2600_setPlayfield0_B()
{
  return -1;
}

int Atari2600::atari2600_setPlayfield0_B(int value)
{
  fprintf(out, "; setPlayfield0_B\n");
  fprintf(out, "  lda #0x%02x\n", (uint8_t)value);
  fprintf(out, "  sta PF0\n");

  return 0;
}

int Atari2600::atari2600_setPlayfield1_B()
{
  return -1;
}

int Atari2600::atari2600_setPlayfield1_B(int value)
{
  fprintf(out, "; setPlayfield1_B\n");
  fprintf(out, "  lda #0x%02x\n", (uint8_t)value);
  fprintf(out, "  sta PF1\n");

  return 0;
}

int Atari2600::atari2600_setPlayfield2_B()
{
  return -1;
}

int Atari2600::atari2600_setPlayfield2_B(int value)
{
  fprintf(out, "; setPlayfield2_B\n");
  fprintf(out, "  lda #0x%02x\n", (uint8_t)value);
  fprintf(out, "  sta PF2\n");

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

void Atari2600::print_pia_definitions()
{
  fprintf(out, "  SWCHA equ 0x280\n");
  fprintf(out, "  SWACNT equ 0x281\n");
  fprintf(out, "  SWCHB equ 0x282\n");
  fprintf(out, "  SWBCNT equ 0x283\n");
  fprintf(out, "  INTIM equ 0x284\n");
  fprintf(out, "  TIMINT equ 0x285\n");
  fprintf(out, "  TIM1T equ 0x294\n");
  fprintf(out, "  TIM8T equ 0x295\n");
  fprintf(out, "  TIM64T equ 0x296\n");
  fprintf(out, "  T1024T equ 0x297\n");
}

