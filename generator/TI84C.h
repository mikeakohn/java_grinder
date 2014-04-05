/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014 by Michael Kohn
 *
 */

#ifndef _TI84C_H
#define _TI84C_H

#include "Z80.h"

class TI84C : public Z80
{
public:
  TI84C();
  virtual ~TI84C();

  virtual int open(const char *filename);
  virtual int start_init();
  virtual int clearRect();
  virtual int drawHL();
  virtual int drawLine();
  virtual int drawPoint();
  virtual int drawString();
  virtual int drawStringCenter();
  virtual int fillRect();
  virtual int setCursorX();
  virtual int setCursorY();
  virtual int setDrawBGColor();
  virtual int setDrawBGWhite();
  virtual int setDrawColor();
  virtual int setTextBGColor();
  virtual int setTextColor();
};

#endif

