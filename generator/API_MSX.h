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

#ifndef _API_MSX_H
#define _API_MSX_H

class API_MSX
{
public:
  // MSX
  virtual int msx_beep_I() { return -1; }
  virtual int msx_color_I() { return -1; }
  virtual int msx_cls_I() { return -1; }
  virtual int msx_screen_I( ) { return -1; }
  virtual int msx_keyOn_I() { return -1; }
  virtual int msx_keyOff_I() { return -1; }
  virtual int msx_fillVRAM_I() { return -1; }
  virtual int msx_copyVRAM_I() { return -1; }
  virtual int msx_putChar_C() { return -1; }
  virtual int msx_setCursor_I() { return -1; }
};

#endif
