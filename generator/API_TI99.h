/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2023 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_API_TI99_H
#define JAVA_GRINDER_GENERATOR_API_TI99_H

class API_TI99
{
public:
  // TI99/4A
  virtual int tms9918a_print_X() { return -1; }
  virtual int tms9918a_printChar_C() { return -1; }
  virtual int tms9918a_printChar_C(int c) { return -1; }
  virtual int tms9918a_setCursor_II() { return -1; }
  virtual int tms9918a_setCursor_II(int x, int y) { return -1; }
  virtual int tms9918a_setGraphicsMode_I() { return -1; }
  virtual int tms9918a_setGraphicsMode_I(int mode) { return -1; }
  virtual int tms9918a_clearScreen() { return -1; }
  virtual int tms9918a_plot_III() { return -1; }
  virtual int tms9918a_initDisplay() { return -1; }
  virtual int tms9918a_setSpriteVisible_IZ() { return -1; }
  virtual int tms9918a_setSpriteImage_IaB() { return -1; }
  virtual int tms9918a_setSpritePos_III() { return -1; }
  virtual int tms9918a_setSpriteColor_II() { return -1; }
  virtual int tms9918a_setSpriteSize_I() { return -1; }

  virtual int sn76489_setSoundFreq_II() { return -1; }
  virtual int sn76489_setSoundVolume_II() { return -1; }
};

#endif

