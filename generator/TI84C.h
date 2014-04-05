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
  virtual int ti84c_clearRect();
  virtual int ti84c_drawHL();
  virtual int ti84c_drawLine();
  virtual int ti84c_drawPoint();
  virtual int ti84c_drawString();
  virtual int ti84c_drawStringCenter();
  virtual int ti84c_fillRect();
  virtual int ti84c_setCursorX();
  virtual int ti84c_setCursorY();
  virtual int ti84c_setDrawBGColor();
  virtual int ti84c_setDrawBGWhite();
  virtual int ti84c_setDrawColor();
  virtual int ti84c_setTextBGColor();
  virtual int ti84c_setTextColor();
};

#endif

