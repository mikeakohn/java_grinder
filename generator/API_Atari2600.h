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

#ifndef _API_ATARI_2600_H
#define _API_ATARI_2600_H

class API_Atari2600
{
public:
  virtual int atari2600_waitHsync() { return -1; }
  virtual int atari2600_vsyncOn() { return -1; }
  virtual int atari2600_vsyncOff() { return -1; }
  virtual int atari2600_vblankOn() { return -1; }
  virtual int atari2600_vblankOff() { return -1; }
  virtual int atari2600_setColorPlayer0() { return -1; }
  virtual int atari2600_setColorPlayer0(int color) { return -1; }
  virtual int atari2600_setColorPlayer1() { return -1; }
  virtual int atari2600_setColorPlayer1(int color) { return -1; }
  virtual int atari2600_setColorPlayfield() { return -1; }
  virtual int atari2600_setColorPlayfield(int color) { return -1; }
  virtual int atari2600_setColorBackground() { return -1; }
  virtual int atari2600_setColorBackground(int color) { return -1; }
};

#endif

