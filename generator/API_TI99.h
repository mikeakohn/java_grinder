/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 */

#ifndef _API_TI99_H
#define _API_TI99_H

class API_TI99
{
public:
  // TI99/4A
  virtual int ti99_print_X() { return -1; }
  virtual int ti99_printChar_C() { return -1; }
  virtual int ti99_printChar_C(int c) { return -1; }
  virtual int ti99_setCursor_II() { return -1; }
  virtual int ti99_setCursor_II(int x, int y) { return -1; }
  virtual int ti99_setGraphicsMode_I() { return -1; }
  virtual int ti99_setGraphicsMode_I(int mode) { return -1; }
  virtual int ti99_clearScreen() { return -1; }
  virtual int ti99_plot_III() { return -1; }
  virtual int ti99_setColors() { return -1; }
  virtual int ti99_setSoundFreq_II() { return -1; }
  virtual int ti99_setSoundVolume_II() { return -1; }
  virtual int ti99_setSpriteVisible_IZ() { return -1; }
  virtual int ti99_setSpriteImage_IaB() { return -1; }
  virtual int ti99_setSpritePos_III() { return -1; }
  virtual int ti99_setSpriteColor_II() { return -1; }
  virtual int ti99_setSpriteSize_I() { return -1; }
};

#endif

