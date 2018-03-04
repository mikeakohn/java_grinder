/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 * SNES by Joe Davisson
 *
 */

#ifndef _API_SNES_H
#define _API_SNES_H

class API_SNES
{
public:
  // Super Nintendo
  virtual int snes_setBackgroundColor_I() { return -1; }
};

#endif

