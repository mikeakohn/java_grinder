/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 * Western Design Center SXB boards by Joe Davisson
 *
 */

#ifndef API_SXB_H
#define API_SXB_H

class API_SXB
{
public:
  // SXB
  virtual int sxb_getChar() { return -1; }
  virtual int sxb_putChar_C() { return -1; }
  virtual int sxb_getInt() { return -1; }
  virtual int sxb_putInt_I() { return -1; }
  virtual int sxb_getString() { return -1; }
  virtual int sxb_putString_X() { return -1; }
  virtual int sxb_controlTones_IIZZ() { return -1; }
};

#endif

