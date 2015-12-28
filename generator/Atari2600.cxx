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
// http://problemkaputt.de/2k6specs.htm

Atari2600::Atari2600()
{
  start_org = 0xf000;
  java_stack_lo = 0x80;
  java_stack_hi = 0x98;
  ram_start = 0xb0;
}

Atari2600::~Atari2600()
{
  insert_atari_2600_functions();

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

  insert_atari_2600_variables();

  fprintf(out, "; clear TIA\n");
  fprintf(out, "  sei\n");
  fprintf(out, "  cld\n");
  fprintf(out, "  ldx #0xff\n");
  fprintf(out, "  stx seed\n");
  fprintf(out, "  txs\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  ldx #127\n");
  fprintf(out, "_clear_tia:\n");
  fprintf(out, "  sta 0,x\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  bne _clear_tia\n");

  fprintf(out, "; set java stack pointer (x register)\n");
  fprintf(out, "  ldx #23\n\n");

  return 0;
}

int Atari2600::atari2600_waitHsync_I()
{
  fprintf(out, "; waitLines_I()\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  tay\n");
  fprintf(out, "_wait_sync_%d:\n", label_count);
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne _wait_sync_%d\n", label_count);

  label_count++;
  stack--;

  return 0;
}

int Atari2600::atari2600_waitHsync_I(int lines)
{
  if (lines == 1) { return atari2600_waitHsync(); }

  fprintf(out, "; waitHsync_I(%d)\n", lines);
  fprintf(out, "  ldy #0x%02x\n", lines);
  fprintf(out, "_wait_sync_%d:\n", label_count);
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne _wait_sync_%d\n", label_count);

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
  fprintf(out, "  jsr start_vblank\n");
/*
  fprintf(out, "  lda #0x02\n");
  fprintf(out, "  sta VSYNC\n");
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  lda #0x00\n");
  fprintf(out, "  sta VSYNC\n");

  fprintf(out, "  lda #43\n");
  fprintf(out, "  sta TIM64T\n");
*/
  return 0;
}

int Atari2600::atari2600_waitVblank()
{
  fprintf(out, "  jsr wait_vblank\n");

/*
  fprintf(out, "_vblank_wait_%d:\n", label_count);
  fprintf(out, "  lda INTIM\n");
  fprintf(out, "  bne _vblank_wait_%d\n", label_count);

  fprintf(out, "  lda #0x00\n");
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  sta VBLANK\n");

  label_count++;
*/
  return 0;
}

int Atari2600::atari2600_startOverscan()
{
  fprintf(out, "  jsr start_overscan\n");
/*
  fprintf(out, "  lda #35\n");
  fprintf(out, "  sta TIM64T\n");
  fprintf(out, "  lda #0x02\n");
  fprintf(out, "  sta VBLANK\n");
*/
  return 0;
}

int Atari2600::atari2600_waitOverscan()
{
  fprintf(out, "  jsr wait_overscan\n");
/*
  fprintf(out, "_overscan_wait_%d:\n", label_count);
  fprintf(out, "  lda INTIM\n");
  fprintf(out, "  bne _overscan_wait_%d\n", label_count);

  label_count++;
*/
  return 0;
}

int Atari2600::atari2600_setColorPlayer0_I()
{
  fprintf(out, "; setColorPlayer0_I\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  sta COLUP0\n");
  stack--;

  return 0;
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
  fprintf(out, "; setColorPlayer1_I\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  sta COLUP1\n");
  stack--;

  return 0;
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
  stack--;

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
  stack--;

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
  fprintf(out, "sta HMCLR\n");
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

int Atari2600::atari2600_setPlayfieldMode_I()
{
  fprintf(out, "; setPlayfieldMode_I\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  sta CTRLPF\n");
  stack--;

  return 0;
}

int Atari2600::atari2600_setPlayfieldMode_I(int mode)
{
  fprintf(out, "; setPlayfieldMode_I\n");
  fprintf(out, "  lda #0x%02x\n", (uint8_t)mode);
  fprintf(out, "  sta CTRLPF\n");
  return 0;
}

int Atari2600::atari2600_setPlayfield0_B()
{
  fprintf(out, "; setPlayfield0_B\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  sta PF0\n");
  stack--;

  return 0;
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
  fprintf(out, "; setPlayfield1_B\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  sta PF1\n");
  stack--;

  return 0;
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
  fprintf(out, "; setPlayfield2_B\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  sta PF2\n");
  stack--;

  return 0;
}

int Atari2600::atari2600_setPlayfield2_B(int value)
{
  fprintf(out, "; setPlayfield2_B\n");
  fprintf(out, "  lda #0x%02x\n", (uint8_t)value);
  fprintf(out, "  sta PF2\n");

  return 0;
}

int Atari2600::atari2600_setPlayfieldData_aB()
{
  fprintf(out, "; setPlayfieldData\n");
  POP_HI();
  fprintf(out, "  sta playfield + 1\n");
  POP_LO();
  fprintf(out, "  sta playfield + 0\n");
  stack--;

  return 0;
}

int Atari2600::atari2600_setPlayfieldLength_B()
{
  fprintf(out, "; setPlayfieldLength\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  sta playfield_length\n");
  stack--;

  return 0;
}

int Atari2600::atari2600_setPlayer0Position_BB()
{
  fprintf(out, "; setPlayer0Position\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  sta player0_y\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  stx result\n");
  fprintf(out, "  ldx #0\n");
  fprintf(out, "  jsr div15\n");
  fprintf(out, "  ldx result\n");
  stack -= 2;

  return 0;
}

int Atari2600::atari2600_setPlayer1Position_BB()
{
  fprintf(out, "; setPlayer1Position\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  sta player1_y\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  stx result\n");
  fprintf(out, "  ldx #1\n");
  fprintf(out, "  jsr div15\n");
  fprintf(out, "  ldx result\n");
  stack -= 2;

  return 0;
}

int Atari2600::atari2600_setMissile0Position_BB()
{
  fprintf(out, "; setMissile0Position\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  sta missile0_y\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  stx result\n");
  fprintf(out, "  ldx #2\n");
  fprintf(out, "  jsr div15\n");
  fprintf(out, "  ldx result\n");
  stack -= 2;

  return 0;
}

int Atari2600::atari2600_setMissile1Position_BB()
{
  fprintf(out, "; setMissile1Position\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  sta missile1_y\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  stx result\n");
  fprintf(out, "  ldx #3\n");
  fprintf(out, "  jsr div15\n");
  fprintf(out, "  ldx result\n");
  stack -= 2;

  return 0;
}

int Atari2600::atari2600_setBallPosition_BB()
{
  fprintf(out, "; setBallPosition\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  sta ball_y\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  stx result\n");
  fprintf(out, "  ldx #4\n");
  fprintf(out, "  jsr div15\n");
  fprintf(out, "  ldx result\n");
  stack -= 2;

  return 0;
}

int Atari2600::atari2600_setPlayer0Sprite_aB()
{
  fprintf(out, "; setPlayer0Sprite\n");
  POP_HI();
  fprintf(out, "  sta player0_sprite + 1\n");
  POP_LO();
  fprintf(out, "  sta player0_sprite + 0\n");
  stack--;

  return 0;
}

int Atari2600::atari2600_setPlayer1Sprite_aB()
{
  fprintf(out, "; setPlayer1Sprite\n");
  POP_HI();
  fprintf(out, "  sta player1_sprite + 1\n");
  POP_LO();
  fprintf(out, "  sta player1_sprite + 0\n");
  stack--;

  return 0;
}

int Atari2600::atari2600_setMissile0Sprite_aB()
{
  fprintf(out, "; setMissile0Sprite\n");
  POP_HI();
  fprintf(out, "  sta missile0_sprite + 1\n");
  POP_LO();
  fprintf(out, "  sta missile0_sprite + 0\n");
  stack--;

  return 0;
}

int Atari2600::atari2600_setMissile1Sprite_aB()
{
  fprintf(out, "; setMissile1Sprite\n");
  POP_HI();
  fprintf(out, "  sta missile1_sprite + 1\n");
  POP_LO();
  fprintf(out, "  sta missile1_sprite + 0\n");
  stack--;

  return 0;
}

int Atari2600::atari2600_setBallSprite_aB()
{
  fprintf(out, "; setBallSprite\n");
  POP_HI();
  fprintf(out, "  sta ball_sprite + 1\n");
  POP_LO();
  fprintf(out, "  sta ball_sprite + 0\n");
  stack--;

  return 0;
}

int Atari2600::atari2600_drawScreen()
{
  fprintf(out, "  jsr draw\n");

  return 0;
}

int Atari2600::atari2600_isCollisionMissile0Player1()
{
  fprintf(out, "; isCollisionMissile0Player1\n");
  fprintf(out, "  lda CXM0P\n");
  fprintf(out, "  and #0x80\n");
  PUSH_LO();
  fprintf(out, "  lda #0\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionMissile0Player0()
{
  fprintf(out, "; isCollisionMissile0Player0\n");
  fprintf(out, "  lda CXM0P\n");
  fprintf(out, "  and #0x40\n");
  PUSH_LO();
  fprintf(out, "  lda #0\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionMissile1Player0()
{
  fprintf(out, "; isCollisionMissile1Player0\n");
  fprintf(out, "  lda CXM1P\n");
  fprintf(out, "  and #0x80\n");
  PUSH_LO();
  fprintf(out, "  lda #0\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionMissile1Player1()
{
  fprintf(out, "; isCollisionMissile1Player1\n");
  fprintf(out, "  lda CXM1P\n");
  fprintf(out, "  and #0x40\n");
  PUSH_LO();
  fprintf(out, "  lda #0\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionPlayer0PlayField()
{
  fprintf(out, "; isCollisionPlayer0PlayField\n");
  fprintf(out, "  lda CXP0FB\n");
  fprintf(out, "  and #0x80\n");
  PUSH_LO();
  fprintf(out, "  lda #0\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionPlayer0Ball()
{
  fprintf(out, "; isCollisionPlayer0Ball\n");
  fprintf(out, "  lda CXP0FB\n");
  fprintf(out, "  and #0x40\n");
  PUSH_LO();
  fprintf(out, "  lda #0\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionPlayer1PlayField()
{
  fprintf(out, "; isCollisionPlayer1PlayField\n");
  fprintf(out, "  lda CXP1FB\n");
  fprintf(out, "  and #0x80\n");
  PUSH_LO();
  fprintf(out, "  lda #0\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionPlayer1Ball()
{
  fprintf(out, "; isCollisionPlayer1Ball\n");
  fprintf(out, "  lda CXP1FB\n");
  fprintf(out, "  and #0x40\n");
  PUSH_LO();
  fprintf(out, "  lda #0\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionMissile0Playfield()
{
  fprintf(out, "; isCollisionMissile0Playfield\n");
  fprintf(out, "  lda CXM0FB\n");
  fprintf(out, "  and #0x80\n");
  PUSH_LO();
  fprintf(out, "  lda #0\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionMissile0Ball()
{
  fprintf(out, "; isCollisionMissile0Ball\n");
  fprintf(out, "  lda CXM0FB\n");
  fprintf(out, "  and #0x40\n");
  PUSH_LO();
  fprintf(out, "  lda #0\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionMissile1Playfield()
{
  fprintf(out, "; isCollisionMissile1Playfield\n");
  fprintf(out, "  lda CXM1FB\n");
  fprintf(out, "  and #0x80\n");
  PUSH_LO();
  fprintf(out, "  lda #0\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionMissile1Ball()
{
  fprintf(out, "; isCollisionMissile1Ball\n");
  fprintf(out, "  lda CXM1FB\n");
  fprintf(out, "  and #0x40\n");
  PUSH_LO();
  fprintf(out, "  lda #0\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionBallPlayfield()
{
  fprintf(out, "; isCollisionBallPlayfield\n");
  fprintf(out, "  lda CXBLPF\n");
  fprintf(out, "  and #0x80\n");
  PUSH_LO();
  fprintf(out, "  lda #0\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionPlayer0Player1()
{
  fprintf(out, "; isCollisionPlayer0Player1\n");
  fprintf(out, "  lda CXPPMM\n");
  fprintf(out, "  and #0x80\n");
  PUSH_LO();
  fprintf(out, "  lda #0\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionMissile0Missile1()
{
  fprintf(out, "; isCollisionMissile0Missile1\n");
  fprintf(out, "  lda CXPPMM\n");
  fprintf(out, "  and #0x40\n");
  PUSH_LO();
  fprintf(out, "  lda #0\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_setAudioControl0_B()
{
  fprintf(out, "; setAudioControl0_B\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  sta AUDC0\n");
  stack--;

  return 0;
}

int Atari2600::atari2600_setAudioControl0_B(int value)
{
  fprintf(out, "; setAudioControl0_B\n");
  fprintf(out, "  lda #0x%02x\n", value & 0xff);
  fprintf(out, "  sta AUDC0\n");

  return 0;
}

int Atari2600::atari2600_setAudioControl1_B()
{
  fprintf(out, "; setAudioControl1_B\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  sta AUDC1\n");
  stack--;

  return 0;
}

int Atari2600::atari2600_setAudioControl1_B(int value)
{
  fprintf(out, "; setAudioControl1_B\n");
  fprintf(out, "  lda #0x%02x\n", value & 0xff);
  fprintf(out, "  sta AUDC1\n");

  return 0;
}

int Atari2600::atari2600_setAudioFrequency0_B()
{
  fprintf(out, "; setAudioFrequency0_B\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  sta AUDF0\n");
  stack--;

  return 0;
}

int Atari2600::atari2600_setAudioFrequency0_B(int value)
{
  fprintf(out, "; setAudioFrequency0_B\n");
  fprintf(out, "  lda #0x%02x\n", value & 0xff);
  fprintf(out, "  sta AUDF0\n");

  return 0;
}

int Atari2600::atari2600_setAudioFrequency1_B()
{
  fprintf(out, "; setAudioFrequency1_B\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  sta AUDF1\n");
  stack--;

  return 0;
}

int Atari2600::atari2600_setAudioFrequency1_B(int value)
{
  fprintf(out, "; setAudioFrequency1_B\n");
  fprintf(out, "  lda #0x%02x\n", value & 0xff);
  fprintf(out, "  sta AUDF1\n");

  return 0;
}

int Atari2600::atari2600_setAudioVolume0_B()
{
  fprintf(out, "; setAudioVolume0_B\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  sta AUDV0\n");
  stack--;

  return 0;
}

int Atari2600::atari2600_setAudioVolume0_B(int value)
{
  fprintf(out, "; setAudioVolume0_B\n");
  fprintf(out, "  lda #0x%02x\n", value & 0xff);
  fprintf(out, "  sta AUDV0\n");

  return 0;
}

int Atari2600::atari2600_setAudioVolume1_B()
{
  fprintf(out, "; setAudioVolume1_B\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  sta AUDV1\n");
  stack--;

  return 0;
}

int Atari2600::atari2600_setAudioVolume1_B(int value)
{
  fprintf(out, "; setAudioVolume1_B\n");
  fprintf(out, "  lda #0x%02x\n", value & 0xff);
  fprintf(out, "  sta AUDV1\n");

  return 0;
}

int Atari2600::atari2600_isJoystick0Right()
{
  fprintf(out, "; isJoystick0Right\n");
  fprintf(out, "  lda SWCHA\n");
  fprintf(out, "  and #0x80\n");
  fprintf(out, "  eor #0x80\n");
  PUSH_LO();
  fprintf(out, "  lda #0x00\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isJoystick0Left()
{
  fprintf(out, "; isJoystick0Left\n");
  fprintf(out, "  lda SWCHA\n");
  fprintf(out, "  and #0x40\n");
  fprintf(out, "  eor #0x40\n");
  PUSH_LO();
  fprintf(out, "  lda #0x00\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isJoystick0Down()
{
  fprintf(out, "; isJoystick0Down\n");
  fprintf(out, "  lda SWCHA\n");
  fprintf(out, "  and #0x20\n");
  fprintf(out, "  eor #0x20\n");
  PUSH_LO();
  fprintf(out, "  lda #0x00\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isJoystick0Up()
{
  fprintf(out, "; isJoystick0Up\n");
  fprintf(out, "  lda SWCHA\n");
  fprintf(out, "  and #0x10\n");
  fprintf(out, "  eor #0x10\n");
  PUSH_LO();
  fprintf(out, "  lda #0x00\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isJoystick0ButtonDown()
{
  fprintf(out, "; isJoystick0ButtonDown\n");
  fprintf(out, "  lda INPT4\n");
  fprintf(out, "  and #0x80\n");
  fprintf(out, "  eor #0x80\n");
  PUSH_LO();
  fprintf(out, "  lda #0x00\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isJoystick1Right()
{
  fprintf(out, "; isJoystick1Right\n");
  fprintf(out, "  lda SWCHA\n");
  fprintf(out, "  and #0x08\n");
  fprintf(out, "  eor #0x08\n");
  PUSH_LO();
  fprintf(out, "  lda #0x00\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isJoystick1Left()
{
  fprintf(out, "; isJoystick1Left\n");
  fprintf(out, "  lda SWCHA\n");
  fprintf(out, "  and #0x04\n");
  fprintf(out, "  eor #0x04\n");
  PUSH_LO();
  fprintf(out, "  lda #0x00\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isJoystick1Down()
{
  fprintf(out, "; isJoystick1Down\n");
  fprintf(out, "  and #0x02\n");
  fprintf(out, "  eor #0x02\n");
  PUSH_LO();
  fprintf(out, "  lda #0x00\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isJoystick1Up()
{
  fprintf(out, "; isJoystick1Up\n");
  fprintf(out, "  and #0x01\n");
  fprintf(out, "  eor #0x01\n");
  PUSH_LO();
  fprintf(out, "  lda #0x00\n");
  PUSH_HI();
  stack++;

  return 0;
}

int Atari2600::atari2600_isJoystick1ButtonDown()
{
  fprintf(out, "; isJoystick1ButtonDown\n");
  fprintf(out, "  lda INPT5\n");
  fprintf(out, "  and #0x80\n");
  fprintf(out, "  eor #0x80\n");
  PUSH_LO();
  fprintf(out, "  lda #0x00\n");
  PUSH_HI();
  stack++;

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

  fprintf(out, "  CXM0P equ 0x00\n");
  fprintf(out, "  CXM1P equ 0x01\n");
  fprintf(out, "  CXP0FB equ 0x02\n");
  fprintf(out, "  CXP1FB equ 0x03\n");
  fprintf(out, "  CXM0FB equ 0x04\n");
  fprintf(out, "  CXM1FB equ 0x05\n");
  fprintf(out, "  CXBLPF equ 0x06\n");
  fprintf(out, "  CXPPMM equ 0x07\n");
  fprintf(out, "  INPT0 equ 0x08\n");
  fprintf(out, "  INPT1 equ 0x09\n");
  fprintf(out, "  INPT2 equ 0x0a\n");
  fprintf(out, "  INPT3 equ 0x0b\n");
  fprintf(out, "  INPT4 equ 0x0c\n");
  fprintf(out, "  INPT5 equ 0x0d\n\n");
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
  fprintf(out, "  T1024T equ 0x297\n\n");
}

void Atari2600::insert_atari_2600_functions()
{
  fprintf(out, "draw:\n");
//  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta ENAM0\n");
  fprintf(out, "  sta ENAM1\n");
  fprintf(out, "  sta ENABL\n");
  fprintf(out, "  sta GRP0\n");
  fprintf(out, "  sta GRP1\n");
  fprintf(out, "  sta player0_line\n");
  fprintf(out, "  sta player1_line\n");
  fprintf(out, "  sta missile0_line\n");
  fprintf(out, "  sta missile1_line\n");
  fprintf(out, "  sta ball_line\n");
  fprintf(out, "  sta playfield_line\n");
  fprintf(out, "  stx result\n");
  fprintf(out, "  ldx #0\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "draw_playfield:\n");
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  txa\n");
  fprintf(out, "  and #3\n");
  fprintf(out, "  bne draw_player0\n");
  fprintf(out, "  ldy playfield_line\n");
  fprintf(out, "  cpy playfield_length\n");
  fprintf(out, "  beq draw_player0\n");
  fprintf(out, "  lda (playfield),y\n");
  fprintf(out, "  sta PF0\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  lda (playfield),y\n");
  fprintf(out, "  sta PF1\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  lda (playfield),y\n");
  fprintf(out, "  sta PF2\n");
  fprintf(out, "  inc playfield_line\n");
  fprintf(out, "  inc playfield_line\n");
  fprintf(out, "  inc playfield_line\n");
  fprintf(out, "draw_player0:\n");
  fprintf(out, "  cpx player0_y\n");
  fprintf(out, "  bmi draw_player1\n");
  fprintf(out, "  ldy player0_line\n");
  fprintf(out, "  lda (player0_sprite),y\n");
  fprintf(out, "  sta GRP0\n");
  fprintf(out, "  beq draw_player1\n");
  fprintf(out, "  inc player0_line\n");
  fprintf(out, "draw_player1:\n");
  fprintf(out, "  cpx player1_y\n");
  fprintf(out, "  bmi draw_missile0\n");
  fprintf(out, "  ldy player1_line\n");
  fprintf(out, "  lda (player1_sprite),y\n");
  fprintf(out, "  sta GRP1\n");
  fprintf(out, "  beq draw_missile0\n");
  fprintf(out, "  inc player1_line\n");
  fprintf(out, "draw_missile0:\n");
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  cpx missile0_y\n");
  fprintf(out, "  bmi draw_missile1\n");
  fprintf(out, "  ldy missile0_line\n");
  fprintf(out, "  lda (missile0_sprite),y\n");
  fprintf(out, "  sta ENAM0\n");
  fprintf(out, "  beq draw_missile1\n");
  fprintf(out, "  inc missile0_line\n");
  fprintf(out, "draw_missile1:\n");
  fprintf(out, "  cpx missile1_y\n");
  fprintf(out, "  bmi draw_ball\n");
  fprintf(out, "  ldy missile1_line\n");
  fprintf(out, "  lda (missile1_sprite),y\n");
  fprintf(out, "  sta ENAM1\n");
  fprintf(out, "  beq draw_ball\n");
  fprintf(out, "  inc missile1_line\n");
  fprintf(out, "draw_ball:\n");
  fprintf(out, "  cpx ball_y\n");
  fprintf(out, "  bmi draw_continue\n");
  fprintf(out, "  ldy ball_line\n");
  fprintf(out, "  lda (ball_sprite),y\n");
  fprintf(out, "  sta ENABL\n");
  fprintf(out, "  beq draw_continue\n");
  fprintf(out, "  inc ball_line\n");
  fprintf(out, "draw_continue:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  cpx #96\n");
  fprintf(out, "  bne draw_playfield\n");
  fprintf(out, "  ldx result\n");
  fprintf(out, "  rts\n\n");

  fprintf(out, "div15:\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "div15_loop:\n");
  fprintf(out, "  sbc #15\n");
  fprintf(out, "  bcs div15_loop\n");
  fprintf(out, "  eor #7\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  sta HMP0,x\n");
  fprintf(out, "  sta RESP0,x\n");
  fprintf(out, "  rts\n\n");

  fprintf(out, "rand:\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  lda seed\n");
  fprintf(out, "  sbc #77\n");
  fprintf(out, "  sta seed\n");
  fprintf(out, "  lda seed\n");
  fprintf(out, "  rts\n\n");

  fprintf(out, "start_vblank:\n");
  fprintf(out, "  lda #0x02\n");
  fprintf(out, "  sta VSYNC\n");
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  lda #0x00\n");
  fprintf(out, "  sta VSYNC\n");
  fprintf(out, "  lda #43\n");
  fprintf(out, "  sta TIM64T\n");
  fprintf(out, "  rts\n\n");

  fprintf(out, "wait_vblank:\n");
  fprintf(out, "  lda INTIM\n");
  fprintf(out, "  bne wait_vblank\n");
  fprintf(out, "  lda #0x00\n");
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  sta HMOVE\n");
  fprintf(out, "  sta VBLANK\n");
  fprintf(out, "  rts\n\n");

  fprintf(out, "start_overscan:\n");
  fprintf(out, "  lda #35\n");
  fprintf(out, "  sta TIM64T\n");
  fprintf(out, "  lda #0x02\n");
  fprintf(out, "  sta VBLANK\n");
  fprintf(out, "  rts\n\n");

  fprintf(out, "wait_overscan:\n");
  fprintf(out, "  lda INTIM\n");
  fprintf(out, "  bne wait_overscan\n");
  fprintf(out, "  rts\n\n");
}

void Atari2600::insert_atari_2600_variables()
{
  fprintf(out, "; variables\n");
  fprintf(out, "  player0_y equ 0xd1\n");
  fprintf(out, "  player0_line equ 0xd2\n");
  fprintf(out, "  player0_sprite equ 0xd3\n");
  fprintf(out, "  player0_sprite_hi equ 0xd4\n");

  fprintf(out, "  player1_y equ 0xd5\n");
  fprintf(out, "  player1_line equ 0xd6\n");
  fprintf(out, "  player1_sprite equ 0xd7\n");
  fprintf(out, "  player1_sprite_hi equ 0xd8\n");

  fprintf(out, "  missile0_y equ 0xd9\n");
  fprintf(out, "  missile0_line equ 0xda\n");
  fprintf(out, "  missile0_sprite equ 0xdb\n");
  fprintf(out, "  missile0_sprite_hi equ 0xdc\n");

  fprintf(out, "  missile1_y equ 0xdd\n");
  fprintf(out, "  missile1_line equ 0xde\n");
  fprintf(out, "  missile1_sprite equ 0xdf\n");
  fprintf(out, "  missile1_sprite_hi equ 0xf0\n");

  fprintf(out, "  ball_y equ 0xf1\n");
  fprintf(out, "  ball_line equ 0xf2\n");
  fprintf(out, "  ball_sprite equ 0xf3\n");
  fprintf(out, "  ball_sprite_hi equ 0xf4\n");

  fprintf(out, "  playfield equ 0xf5\n");
  fprintf(out, "  playfield_hi equ 0xf6\n");
  fprintf(out, "  playfield_line equ 0xf7\n");
  fprintf(out, "  playfield_length equ 0xf8\n");

  fprintf(out, "  seed equ 0xf9\n");
  fprintf(out, "\n");
}

