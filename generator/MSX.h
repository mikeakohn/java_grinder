/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 */


/*
 *   MSX support by Giovanni Nunes - https://github.com/plainspooky
 *                  Emiliano Fraga - https://github.com/efraga-msx
 */

#ifndef _MSX_H
#define _MSX_H

#include "Z80.h"

class MSX : public Z80
{
public:
  MSX();
  virtual ~MSX();
  virtual int open(const char *filename);
  virtual int start_init();
  virtual int msx_cls();
  virtual int msx_beep();
  virtual int msx_color_BBB(uint8_t foreground, uint8_t background, uint8_t border);
  virtual int msx_screen_B(uint8_t mode);
  virtual int msx_keyOn();
  virtual int msx_keyOff();
  virtual int msx_fillVRAM_III(int c, int len, int addr);
  virtual int msx_copyVRAM_III(int len, int source, int dest);
  virtual int msx_putChar_C();
  virtual int msx_setCursor_BB();

private:
  void insert_ldirvv(void);

  //uint32_t need_plot_lores : 1;
  bool need_ldirvv:1;
};

#endif
