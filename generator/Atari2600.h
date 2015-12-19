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

#ifndef _ATARI_2600_H
#define _ATARI_2600_H

#include "M6502.h"

class Atari2600 : public M6502
{
public:
  Atari2600();
  virtual ~Atari2600();

  virtual int open(const char *filename);
  virtual int atari2600_waitHsync();
  virtual int atari2600_startVblank();
  virtual int atari2600_waitVblank();
  virtual int atari2600_startOverscan();
  virtual int atari2600_waitOverscan();
  virtual int atari2600_setColorPlayer0();
  virtual int atari2600_setColorPlayer0(int color);
  virtual int atari2600_setColorPlayer1();
  virtual int atari2600_setColorPlayer1(int color);
  virtual int atari2600_setColorPlayfield();
  virtual int atari2600_setColorPlayfield(int color);
  virtual int atari2600_setColorBackground();
  virtual int atari2600_setColorBackground(int color);

  virtual int atari2600_resetPlayer0();
  virtual int atari2600_resetPlayer1();
  virtual int atari2600_resetMissile0();
  virtual int atari2600_resetMissile1();
  virtual int atari2600_resetBall();
  virtual int atari2600_applyHorizontalMotion();
  virtual int atari2600_clearMotionRegisters();
  virtual int atari2600_clearCollisionLatches();

private:
  void print_tia_definitions();
  void print_pia_definitions();
};

#endif

