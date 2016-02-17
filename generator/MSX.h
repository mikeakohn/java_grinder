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
  virtual int msx_beep_I();
  virtual int msx_color_I(int foreground,int background,int border);
  virtual int msx_cls_I();
  virtual int msx_screen_I(int mode);
  virtual int msx_keyOn_I();
  virtual int msx_keyOff_I();
  virtual int msx_fillVRAM_I(int c,int len,int addr);
  virtual int msx_copyVRAM_I(int len,int source,int dest);
  virtual int msx_putChar_C(char c);
  virtual int msx_setCursor_I(int column,int line);

private:
  void insert_ldirvv(void);

  //uint32_t need_plot_lores : 1;
  bool need_ldirvv:1;
};

#endif
