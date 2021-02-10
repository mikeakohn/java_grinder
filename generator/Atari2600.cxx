/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2021 by Michael Kohn, Joe Davisson
 *
 * Atari 2600 written by Joe Davisson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "generator/Atari2600.h"

// http://www.alienbill.com/2600/101/docs/stella.html
// http://problemkaputt.de/2k6specs.htm

Atari2600::Atari2600() :
  need_game_draw(0),
  need_title_draw(0),
  need_set_bank(0),
  bank_index(-1)
{
  start_org = 0xf000;
  java_stack_lo = 0x80;
  java_stack_hi = 0x98;
  // not used
  ram_start = 0;
}

Atari2600::~Atari2600()
{
  if (need_set_bank) { insert_set_bank(); }

  fprintf(out, ".org 0xfffa\n");
  fprintf(out, "; NMI\n");
  fprintf(out, "dw reset\n");

  if (bank_index != 0)
  {
    fprintf(out, "; RESET\n");
    fprintf(out, "dw reset\n");
  }
    else
  {
    fprintf(out, "; RESET\n");
    fprintf(out, "dw _bank_switch\n");
  }

  fprintf(out, "; IRQ\n");
  fprintf(out, "dw reset\n");
}

int Atari2600::open(const char *filename)
{
  if (M6502_8::open(filename) != 0) { return -1; }

  fprintf(out, ".include \"atari2600.inc\"\n\n");

  insert_variables();

  fprintf(out, "; clear TIA\n");
  fprintf(out, "  sei\n");
  fprintf(out, "  cld\n");
  fprintf(out, "  ldx #0xff\n");
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

int Atari2600::finish()
{
  M6502_8::finish();

  if (need_game_draw) { insert_game_draw(); }
  if (need_title_draw) { insert_title_draw(); }

  insert_functions();

  return 0;
}

int Atari2600::atari2600_waitHsync_I()
{
  fprintf(out, "; waitLines_I()\n");
  POP();
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

  return 0;
}

int Atari2600::atari2600_waitVblank()
{
  fprintf(out, "  jsr wait_vblank\n");

  return 0;
}

int Atari2600::atari2600_startOverscan()
{
  fprintf(out, "  jsr start_overscan\n");

  return 0;
}

int Atari2600::atari2600_waitOverscan()
{
  fprintf(out, "  jsr wait_overscan\n");

  return 0;
}

int Atari2600::atari2600_setColorPlayer0_I()
{
  fprintf(out, "; setColorPlayer0_I\n");
  POP();
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
  POP();
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
  POP();
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
  POP();
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
  POP();
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
  POP();
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
  POP();
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
  POP();
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
  POP();
  fprintf(out, "  sta playfield_length\n");
  stack--;

  return 0;
}

int Atari2600::atari2600_setPlayer0Position_BB()
{
  fprintf(out, "; setPlayer0Position\n");
  POP();
  fprintf(out, "  sta player0_y\n");
  POP();
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
  POP();
  fprintf(out, "  sta player1_y\n");
  POP();
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
  POP();
  fprintf(out, "  sta missile0_y\n");
  POP();
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
  POP();
  fprintf(out, "  sta missile1_y\n");
  POP();
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
  POP();
  fprintf(out, "  sta ball_y\n");
  POP();
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
  need_game_draw = 1;
  fprintf(out, "  jsr draw\n");

  return 0;
}

int Atari2600::atari2600_isCollisionMissile0Player1()
{
  fprintf(out, "; isCollisionMissile0Player1\n");
  fprintf(out, "  lda CXM0P\n");
  fprintf(out, "  and #0x80\n");
  PUSH();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionMissile0Player0()
{
  fprintf(out, "; isCollisionMissile0Player0\n");
  fprintf(out, "  lda CXM0P\n");
  fprintf(out, "  and #0x40\n");
  PUSH();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionMissile1Player0()
{
  fprintf(out, "; isCollisionMissile1Player0\n");
  fprintf(out, "  lda CXM1P\n");
  fprintf(out, "  and #0x80\n");
  PUSH();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionMissile1Player1()
{
  fprintf(out, "; isCollisionMissile1Player1\n");
  fprintf(out, "  lda CXM1P\n");
  fprintf(out, "  and #0x40\n");
  PUSH();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionPlayer0PlayField()
{
  fprintf(out, "; isCollisionPlayer0PlayField\n");
  fprintf(out, "  lda CXP0FB\n");
  fprintf(out, "  and #0x80\n");
  PUSH();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionPlayer0Ball()
{
  fprintf(out, "; isCollisionPlayer0Ball\n");
  fprintf(out, "  lda CXP0FB\n");
  fprintf(out, "  and #0x40\n");
  PUSH();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionPlayer1PlayField()
{
  fprintf(out, "; isCollisionPlayer1PlayField\n");
  fprintf(out, "  lda CXP1FB\n");
  fprintf(out, "  and #0x80\n");
  PUSH();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionPlayer1Ball()
{
  fprintf(out, "; isCollisionPlayer1Ball\n");
  fprintf(out, "  lda CXP1FB\n");
  fprintf(out, "  and #0x40\n");
  PUSH();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionMissile0Playfield()
{
  fprintf(out, "; isCollisionMissile0Playfield\n");
  fprintf(out, "  lda CXM0FB\n");
  fprintf(out, "  and #0x80\n");
  PUSH();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionMissile0Ball()
{
  fprintf(out, "; isCollisionMissile0Ball\n");
  fprintf(out, "  lda CXM0FB\n");
  fprintf(out, "  and #0x40\n");
  PUSH();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionMissile1Playfield()
{
  fprintf(out, "; isCollisionMissile1Playfield\n");
  fprintf(out, "  lda CXM1FB\n");
  fprintf(out, "  and #0x80\n");
  PUSH();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionMissile1Ball()
{
  fprintf(out, "; isCollisionMissile1Ball\n");
  fprintf(out, "  lda CXM1FB\n");
  fprintf(out, "  and #0x40\n");
  PUSH();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionBallPlayfield()
{
  fprintf(out, "; isCollisionBallPlayfield\n");
  fprintf(out, "  lda CXBLPF\n");
  fprintf(out, "  and #0x80\n");
  PUSH();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionPlayer0Player1()
{
  fprintf(out, "; isCollisionPlayer0Player1\n");
  fprintf(out, "  lda CXPPMM\n");
  fprintf(out, "  and #0x80\n");
  PUSH();
  stack++;

  return 0;
}

int Atari2600::atari2600_isCollisionMissile0Missile1()
{
  fprintf(out, "; isCollisionMissile0Missile1\n");
  fprintf(out, "  lda CXPPMM\n");
  fprintf(out, "  and #0x40\n");
  PUSH();
  stack++;

  return 0;
}

int Atari2600::atari2600_setAudioControl0_B()
{
  fprintf(out, "; setAudioControl0_B\n");
  POP();
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
  POP();
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
  POP();
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
  POP();
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
  POP();
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
  POP();
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

int Atari2600::joystick_isRight_I()
{
  return -1;
}

int Atari2600::joystick_isRight_I(int index)
{
  fprintf(out, "; Joystick.isRight_I(index=%d)\n", index);

  if (index == 0)
  {
    fprintf(out,
      "  lda SWCHA\n"
      "  and #0x80\n"
      "  eor #0x80\n");
    PUSH();
    stack++;

    return 0;
  }
    else
  {
    fprintf(out,
      "  lda SWCHA\n"
      "  and #0x08\n"
      "  eor #0x08\n");
    PUSH();
    stack++;

    return 0;
  }

  return -1;
}

int Atari2600::joystick_isLeft_I()
{
  return -1;
}

int Atari2600::joystick_isLeft_I(int index)
{
  fprintf(out, "; isLeft_I(index=%d)\n", index);

  if (index == 0)
  {
    fprintf(out,
      "  lda SWCHA\n"
      "  and #0x40\n"
      "  eor #0x40\n");
    PUSH();
    stack++;

    return 0;
  }
    else
  {
    fprintf(out,
      "  lda SWCHA\n"
      "  and #0x04\n"
      "  eor #0x04\n");
    PUSH();
    stack++;

    return 0;
  }

  return -1;
}

int Atari2600::joystick_isDown_I()
{
  return -1;
}

int Atari2600::joystick_isDown_I(int index)
{
  fprintf(out, "; isDown_I(index=%d)\n", index);

  if (index == 0)
  {
    fprintf(out,
      "  lda SWCHA\n"
      "  and #0x20\n"
      "  eor #0x20\n");
    PUSH();
    stack++;

    return 0;
  }
    else
  {
    fprintf(out,
      "  lda SWCHA\n"
      "  and #0x02\n"
      "  eor #0x02\n");
      PUSH();
      stack++;

      return 0;
  }

  return -1;
}

int Atari2600::joystick_isUp_I()
{
  return -1;
}

int Atari2600::joystick_isUp_I(int index)
{
  fprintf(out, "; isUp_I(index=%d)\n", index);

  if (index == 0)
  {
    fprintf(out,
      "  lda SWCHA\n"
      "  and #0x10\n"
      "  eor #0x10\n");
    PUSH();
    stack++;

    return 0;
  }
    else
  {
    fprintf(out,
      "  lda SWCHA\n"
      "  and #0x01\n"
      "  eor #0x01\n");
    PUSH();
    stack++;

    return 0;
  }

  return -1;
}

int Atari2600::joystick_isButtonDown_0_I()
{
  return -1;
}

int Atari2600::joystick_isButtonDown_0_I(int index)
{
  fprintf(out, "; isButtonDown_0_I(index=%d)\n", index);

  if (index == 0)
  {
    fprintf(out,
      "  lda INPT4\n"
      "  and #0x80\n"
      "  eor #0x80\n");
    PUSH();
    stack++;

    return 0;
  }
    else
  {
    fprintf(out,
      "  lda INPT5\n"
      "  and #0x80\n"
      "  eor #0x80\n");
    PUSH();
    stack++;

    return 0;
  }

  return -1;
}

int Atari2600::atari2600_getSwitches()
{
  fprintf(out, "; getSwitches\n");
  fprintf(out, "  lda SWCHB\n");
  PUSH();
  stack++;

  return 0;
}

int Atari2600::atari2600_isSwitchResetDown()
{
  fprintf(out, "; isSwitchResetDown\n");
  fprintf(out, "  lda SWCHB\n");
  fprintf(out, "  and #0x01\n");
  fprintf(out, "  eor #0x01\n");
  PUSH();
  stack++;

  return 0;
}

int Atari2600::atari2600_isSwitchSelectDown()
{
  fprintf(out, "; isSwitchSelectDown\n");
  fprintf(out, "  lda SWCHB\n");
  fprintf(out, "  and #0x02\n");
  fprintf(out, "  eor #0x02\n");
  PUSH();
  stack++;

  return 0;
}

int Atari2600::atari2600_isSwitchColorSet()
{
  fprintf(out, "; isSwitchColorSet\n");
  fprintf(out, "  lda SWCHB\n");
  fprintf(out, "  and #0x04\n");
  PUSH();
  stack++;

  return 0;
}

int Atari2600::atari2600_isSwitchDifficultyP0Down()
{
  fprintf(out, "; isSwitchDifficultyP0Down\n");
  fprintf(out, "  lda SWCHB\n");
  fprintf(out, "  and #0x40\n");
  fprintf(out, "  eor #0x40\n");
  PUSH();
  stack++;

  return 0;
}

int Atari2600::atari2600_isSwitchDifficultyP1Down()
{
  fprintf(out, "; isSwitchDifficultyP1Down\n");
  fprintf(out, "  lda SWCHB\n");
  fprintf(out, "  and #0x80\n");
  fprintf(out, "  eor #0x80\n");
  PUSH();
  stack++;

  return 0;
}


int Atari2600::atari2600_setScore0_B()
{
  fprintf(out, "; setScore0\n");
  POP();
  fprintf(out, "  sta result\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  adc result\n");
  fprintf(out, "  sta score0\n");
  stack--;

  return 0;
}

int Atari2600::atari2600_setScore1_B()
{
  fprintf(out, "; setScore1\n");
  POP();
  fprintf(out, "  sta result\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  adc result\n");
  fprintf(out, "  sta score1\n");
  stack--;

  return 0;
}

int Atari2600::atari2600_setTitlePos_I()
{
  POP();
  fprintf(out, "  sta title_pos\n");
  stack--;

  return 0;
}

int Atari2600::atari2600_setTitleColor_I()
{
  POP();
  fprintf(out, "  sta title_color\n");
  stack--;

  return 0;
}

int Atari2600::atari2600_drawTitleScreen()
{
  need_title_draw = 1;
  fprintf(out, "  jsr title_draw\n");

  return 0;
}

int Atari2600::atari2600_setBank_B()
{
  printf("Error: setBank(byte index) can only take a constant parameter\n");

  return -1;
}

int Atari2600::atari2600_setBank_B(int value)
{
  need_set_bank = true;

  if (bank_index == -1) { bank_index = value; }

  if (bank_index != value)
  {
    printf("Error: setBank(byte index) can only be used with 1 unique constant index\n");
    return -1;
  }

  fprintf(out, "  jmp _bank_switch\n");

  return 0;
}

void Atari2600::insert_game_draw()
{
  fprintf(out, "draw:\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta PF0\n");
  fprintf(out, "  sta PF1\n");
  fprintf(out, "  sta PF2\n");
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
  fprintf(out, "  lda #00001010b\n");
  fprintf(out, "  sta CTRLPF\n");

  fprintf(out, "draw_score:\n");
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  ldy score0\n");
  fprintf(out, "  lda numbers,y\n");
  fprintf(out, "  sta PF1\n");
  fprintf(out, "  lda (playfield),y\n"); //5
  fprintf(out, "  lda (playfield),y\n"); //5
  fprintf(out, "  lda (playfield),y\n"); //5
  fprintf(out, "  lda (playfield),y\n"); //5
  fprintf(out, "  lda (playfield),y\n"); //5
  fprintf(out, "  ldy score1\n");
  fprintf(out, "  lda numbers,y\n");
  fprintf(out, "  sta PF1\n");

  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  ldy score0\n");
  fprintf(out, "  lda numbers,y\n");
  fprintf(out, "  sta PF1\n");
  fprintf(out, "  lda (playfield),y\n"); //5
  fprintf(out, "  lda (playfield),y\n"); //5
  fprintf(out, "  lda (playfield),y\n"); //5
  fprintf(out, "  lda (playfield),y\n"); //5
  fprintf(out, "  lda (playfield),y\n"); //5
  fprintf(out, "  ldy score1\n");
  fprintf(out, "  lda numbers,y\n");
  fprintf(out, "  sta PF1\n");

  fprintf(out, "  inx\n");
  fprintf(out, "  inc score0\n");
  fprintf(out, "  inc score1\n");
  fprintf(out, "  cpx #5\n");
  fprintf(out, "  bne draw_score\n");
  fprintf(out, "  lda #00001001b\n");
  fprintf(out, "  sta CTRLPF\n");

  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta PF1\n");
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  ldx #0\n");

  fprintf(out, "draw_playfield:\n");
  fprintf(out, "  txa\n"); //2
  fprintf(out, "  and #1\n"); //2
  fprintf(out, "  bne draw_playfield_skip\n"); //2
  fprintf(out, "  ldy playfield_line\n"); //3
  fprintf(out, "  cpy playfield_length\n"); //3
  fprintf(out, "  beq draw_playfield_skip\n"); //2
  fprintf(out, "  lda (playfield),y\n"); //5
  fprintf(out, "  sta WSYNC\n"); //3
  fprintf(out, "  sta PF0\n"); //3
  fprintf(out, "  iny\n"); //2
  fprintf(out, "  lda (playfield),y\n"); //5
  fprintf(out, "  sta PF1\n"); //3
  fprintf(out, "  iny\n"); //2
  fprintf(out, "  lda (playfield),y\n"); //5
  fprintf(out, "  sta PF2\n"); //3
  fprintf(out, "  inc playfield_line\n"); //5
  fprintf(out, "  inc playfield_line\n"); //5
  fprintf(out, "  inc playfield_line\n"); //5
  fprintf(out, "  db 0x2c\n"); //4
//45
  fprintf(out, "draw_playfield_skip:\n");
  fprintf(out, "  sta WSYNC\n"); //3
  fprintf(out, "draw_player0:\n");
  fprintf(out, "  cpx player0_y\n"); //3
  fprintf(out, "  bmi draw_player1\n"); //2
  fprintf(out, "  ldy player0_line\n"); //3
  fprintf(out, "  lda (player0_sprite),y\n"); //5
  fprintf(out, "  sta GRP0\n"); //3
  fprintf(out, "  beq draw_player1\n"); //2
  fprintf(out, "  inc player0_line\n"); //5
//26
  fprintf(out, "draw_player1:\n");
  fprintf(out, "  sta WSYNC\n");  //3
  fprintf(out, "  cpx player1_y\n"); //3
  fprintf(out, "  bmi draw_missile0\n"); //2
  fprintf(out, "  ldy player1_line\n"); //3
  fprintf(out, "  lda (player1_sprite),y\n"); //5
  fprintf(out, "  sta GRP1\n"); //3
  fprintf(out, "  beq draw_missile0\n"); //2
  fprintf(out, "  inc player1_line\n"); //5
//23
  fprintf(out, "draw_missile0:\n");
  fprintf(out, "  cpx missile0_y\n"); //3
  fprintf(out, "  bmi draw_missile1\n"); //2
  fprintf(out, "  ldy missile0_line\n"); //3
  fprintf(out, "  lda (missile0_sprite),y\n"); //5
  fprintf(out, "  sta ENAM0\n"); //3
  fprintf(out, "  beq draw_missile1\n"); //2
  fprintf(out, "  inc missile0_line\n"); //5
//26
  fprintf(out, "draw_missile1:\n");
  fprintf(out, "  cpx missile1_y\n"); //3
  fprintf(out, "  bmi draw_ball\n"); //2
  fprintf(out, "  ldy missile1_line\n"); //3
  fprintf(out, "  lda (missile1_sprite),y\n"); //5
  fprintf(out, "  sta ENAM1\n"); //3
  fprintf(out, "  beq draw_ball\n"); //2
  fprintf(out, "  inc missile1_line\n"); //5
//23
  fprintf(out, "draw_ball:\n");
  fprintf(out, "  sta WSYNC\n"); //3
  fprintf(out, "  cpx ball_y\n"); //3
  fprintf(out, "  bmi draw_continue\n"); //2
  fprintf(out, "  ldy ball_line\n"); //3
  fprintf(out, "  lda (ball_sprite),y\n"); //5
  fprintf(out, "  sta ENABL\n"); //3
  fprintf(out, "  beq draw_continue\n"); //2
  fprintf(out, "  inc ball_line\n"); //5
//26
  fprintf(out, "draw_continue:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  cpx #60\n");
  fprintf(out, "  bne draw_playfield\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  ldx result\n");
  fprintf(out, "  rts\n\n");
}

void Atari2600::insert_title_draw()
{
  fprintf(out, "title_draw:\n");
  fprintf(out, "  stx result\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta PF0\n");
  fprintf(out, "  sta PF1\n");
  fprintf(out, "  sta PF2\n");
  fprintf(out, "  sta ENAM0\n");
  fprintf(out, "  sta ENAM1\n");
  fprintf(out, "  sta ENABL\n");
  fprintf(out, "  sta GRP0\n");
  fprintf(out, "  sta GRP1\n");
  fprintf(out, "  sta player0_line\n");
  fprintf(out, "  sta player1_line\n");
  fprintf(out, "  lda #1\n");
  fprintf(out, "  sta VDELP0\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda title_color\n");
  fprintf(out, "  pha\n");
  fprintf(out, "title_draw_wait:\n");
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  cpy title_pos\n");
  fprintf(out, "  bne title_draw_wait\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "title_draw_loop2:\n");
  fprintf(out, "  ldx #3\n");
  fprintf(out, "title_draw_loop:\n");
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  lda title_color\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  sta COLUPF\n");
  fprintf(out, "  lda pf_left0 - 1,y\n");
  fprintf(out, "  sta PF0\n");
  fprintf(out, "  lda pf_left1 - 1,y\n");
  fprintf(out, "  sta PF1\n");
  fprintf(out, "  lda pf_left2 - 1,y\n");
  fprintf(out, "  sta PF2\n");
  fprintf(out, "  nop\n");
  fprintf(out, "  nop\n");
  fprintf(out, "  lda pf_right0 - 1,y\n");
  fprintf(out, "  sta PF0\n");
  fprintf(out, "  lda pf_right1 - 1,y\n");
  fprintf(out, "  sta PF1\n");
  fprintf(out, "  lda pf_right2 - 1,y\n");
  fprintf(out, "  sta PF2\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  bne title_draw_loop\n");
  fprintf(out, "  dec title_color\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  cpy #16\n");
  fprintf(out, "  bne title_draw_loop2\n");
  fprintf(out, "  ldx #0\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta PF0\n");
  fprintf(out, "  sta PF1\n");
  fprintf(out, "  sta PF2\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda title_pos\n");
  fprintf(out, "  adc #48\n");
  fprintf(out, "  lsr\n");
  fprintf(out, "  sta value1\n");
  fprintf(out, "  sec\n");
  fprintf(out, "  lda #96\n");
  fprintf(out, "  sbc value1\n");
  fprintf(out, "  sta value1\n");
//  fprintf(out, "  tax\n");
  fprintf(out, "title_draw_player0:\n");
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  cpx player0_y\n"); //3
  fprintf(out, "  bmi title_draw_player1\n"); //2
  fprintf(out, "  ldy player0_line\n"); //3
  fprintf(out, "  lda (player0_sprite),y\n"); //5
  fprintf(out, "  tay\n"); //3
  fprintf(out, "  eor #170\n"); //3
  fprintf(out, "  beq title_draw_player1\n"); //2
  fprintf(out, "  tya\n"); //3
  fprintf(out, "  sta GRP0\n"); //3
  fprintf(out, "  inc player0_line\n"); //5
  fprintf(out, "title_draw_player1:\n");
  fprintf(out, "  sta WSYNC\n");
  fprintf(out, "  cpx player1_y\n"); //3
  fprintf(out, "  bmi title_draw_wait2\n"); //2
  fprintf(out, "  ldy player1_line\n"); //3
  fprintf(out, "  lda (player1_sprite),y\n"); //5
  fprintf(out, "  tay\n"); //3
  fprintf(out, "  eor #170\n"); //3
  fprintf(out, "  beq title_draw_wait2\n"); //2
  fprintf(out, "  tya\n"); //3
  fprintf(out, "  sta GRP1\n"); //3
  fprintf(out, "  inc player1_line\n"); //5
  fprintf(out, "title_draw_wait2:\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  cpx value1\n");
  fprintf(out, "  bne title_draw_player0\n");
  fprintf(out, "  pla\n");
  fprintf(out, "  sta title_color\n");
  fprintf(out, "  ldx result\n");
  fprintf(out, "  rts\n");
}

void Atari2600::insert_set_bank()
{
  fprintf(out, ".org 0xfff2\n");
  fprintf(out, "_bank_switch:\n");

  if (bank_index == 0)
  {
    fprintf(out, "  lda 0x1ff8\n");
  }
    else
  if (bank_index == 1)
  {
    fprintf(out, "  lda 0x1ff9\n");
  }

  fprintf(out, "  jmp reset\n");
}

void Atari2600::insert_functions()
{
  // this scales 0-127 to 0-158
  // so that horizontal positions fit into a signed byte
  // x = (x * 5) / 4
  fprintf(out, "div15:\n");
  fprintf(out, "  sta value1 + 0\n");
  fprintf(out, "  sta value2 + 0\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta value1 + 1\n");
  fprintf(out, "  sta value2 + 1\n");
  fprintf(out, "div15_mul_5:\n");
  fprintf(out, "  asl value2 + 0\n");
  fprintf(out, "  rol value2 + 1\n");
  fprintf(out, "  asl value2 + 0\n");
  fprintf(out, "  rol value2 + 1\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda value1 + 0\n");
  fprintf(out, "  adc value2 + 0\n");
  fprintf(out, "  sta value2 + 0\n");
  fprintf(out, "  lda value1 + 1\n");
  fprintf(out, "  adc value2 + 1\n");
  fprintf(out, "  sta value2 + 1\n");
  fprintf(out, "div15_shift_right_2:\n");
  fprintf(out, "  lsr value2 + 1\n");
  fprintf(out, "  ror value2 + 0\n");
  fprintf(out, "  lsr value2 + 1\n");
  fprintf(out, "  ror value2 + 0\n");
  fprintf(out, "div15_start:\n");
  fprintf(out, "  lda value2 + 0\n");
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

  fprintf(out, "numbers:\n");
  fprintf(out, "db 00000111b\n");
  fprintf(out, "db 00000101b\n");
  fprintf(out, "db 00000101b\n");
  fprintf(out, "db 00000101b\n");
  fprintf(out, "db 00000111b\n");

  fprintf(out, "db 00000001b\n");
  fprintf(out, "db 00000001b\n");
  fprintf(out, "db 00000001b\n");
  fprintf(out, "db 00000001b\n");
  fprintf(out, "db 00000001b\n");

  fprintf(out, "db 00000111b\n");
  fprintf(out, "db 00000001b\n");
  fprintf(out, "db 00000111b\n");
  fprintf(out, "db 00000100b\n");
  fprintf(out, "db 00000111b\n");

  fprintf(out, "db 00000111b\n");
  fprintf(out, "db 00000001b\n");
  fprintf(out, "db 00000111b\n");
  fprintf(out, "db 00000001b\n");
  fprintf(out, "db 00000111b\n");

  fprintf(out, "db 00000101b\n");
  fprintf(out, "db 00000101b\n");
  fprintf(out, "db 00000111b\n");
  fprintf(out, "db 00000001b\n");
  fprintf(out, "db 00000001b\n");

  fprintf(out, "db 00000111b\n");
  fprintf(out, "db 00000100b\n");
  fprintf(out, "db 00000111b\n");
  fprintf(out, "db 00000001b\n");
  fprintf(out, "db 00000111b\n");

  fprintf(out, "db 00000111b\n");
  fprintf(out, "db 00000100b\n");
  fprintf(out, "db 00000111b\n");
  fprintf(out, "db 00000101b\n");
  fprintf(out, "db 00000111b\n");

  fprintf(out, "db 00000111b\n");
  fprintf(out, "db 00000001b\n");
  fprintf(out, "db 00000001b\n");
  fprintf(out, "db 00000001b\n");
  fprintf(out, "db 00000001b\n");

  fprintf(out, "db 00000111b\n");
  fprintf(out, "db 00000101b\n");
  fprintf(out, "db 00000111b\n");
  fprintf(out, "db 00000101b\n");
  fprintf(out, "db 00000111b\n");

  fprintf(out, "db 00000111b\n");
  fprintf(out, "db 00000101b\n");
  fprintf(out, "db 00000111b\n");
  fprintf(out, "db 00000001b\n");
  fprintf(out, "db 00000111b\n");

  fprintf(out, "db 00010111b\n");
  fprintf(out, "db 00010101b\n");
  fprintf(out, "db 00010101b\n");
  fprintf(out, "db 00010101b\n");
  fprintf(out, "db 00010111b\n");

  fprintf(out, "db 00000000b\n");
  fprintf(out, "db 00000000b\n");
  fprintf(out, "db 00000000b\n");
  fprintf(out, "db 00000000b\n");
  fprintf(out, "db 00000000b\n");
}

void Atari2600::insert_variables()
{
  fprintf(out, "; variables\n");
  fprintf(out, "  score0 equ 0xc0\n");
  fprintf(out, "  score1 equ 0xc1\n");

  fprintf(out, "  player0_y equ 0xc2\n");
  fprintf(out, "  player0_line equ 0xc3\n");
  fprintf(out, "  player0_sprite equ 0xc4\n");
  fprintf(out, "  player0_sprite_hi equ 0xc5\n");

  fprintf(out, "  player1_y equ 0xc6\n");
  fprintf(out, "  player1_line equ 0xc7\n");
  fprintf(out, "  player1_sprite equ 0xc8\n");
  fprintf(out, "  player1_sprite_hi equ 0xc9\n");

  fprintf(out, "  missile0_y equ 0xca\n");
  fprintf(out, "  missile0_line equ 0xcb\n");
  fprintf(out, "  missile0_sprite equ 0xcc\n");
  fprintf(out, "  missile0_sprite_hi equ 0xcd\n");

  fprintf(out, "  missile1_y equ 0xce\n");
  fprintf(out, "  missile1_line equ 0xcf\n");
  fprintf(out, "  missile1_sprite equ 0xd0\n");
  fprintf(out, "  missile1_sprite_hi equ 0xd1\n");

  fprintf(out, "  ball_y equ 0xd2\n");
  fprintf(out, "  ball_line equ 0xd3\n");
  fprintf(out, "  ball_sprite equ 0xd4\n");
  fprintf(out, "  ball_sprite_hi equ 0xd5\n");

  fprintf(out, "  playfield equ 0xd6\n");
  fprintf(out, "  playfield_hi equ 0xd7\n");
  fprintf(out, "  playfield_line equ 0xd8\n");
  fprintf(out, "  playfield_length equ 0xd9\n");

  fprintf(out, "  title_pos equ 0xda\n");
  fprintf(out, "  title_line_count equ 0xdb\n");
  fprintf(out, "  title_color equ 0xdc\n");
  fprintf(out, "  title_color_temp equ 0xdd\n");

  // title playfield uses the following array names:
  // pf_left0
  // pf_left1
  // pf_left2
  // pf_right0
  // pf_right1
  // pf_right2

  fprintf(out, "\n");
}

