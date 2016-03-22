/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 * SNES by Joe Davisson
 *
 */

#ifndef _API_W65C265SXB_H
#define _API_W65C265SXB_H

class API_W65C265SXB
{
public:
  // W65C265SXB
  virtual int w65c265sxb_getChar() { return -1; }
  virtual int w65c265sxb_putChar_C() { return -1; }
  virtual int w65c265sxb_getInt() { return -1; }
  virtual int w65c265sxb_putInt_I() { return -1; }
  virtual int w65c265sxb_getString() { return -1; }
  virtual int w65c265sxb_putString_X() { return -1; }
  virtual int w65c265sxb_controlTones_IIZZ() { return -1; }
};

#endif

