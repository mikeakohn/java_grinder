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

#ifndef _TI84_H
#define _TI84_H

#include "Z80.h"

class TI84 : public Z80
{
public:
  TI84(int model);
  virtual ~TI84();

  virtual int open(const char *filename);
  virtual int start_init();
  virtual int ti84_clearScreen();
  virtual int ti84_clearRect();
  virtual int ti84_drawLine();
  virtual int ti84_drawPoint();
  virtual int ti84_print();
  virtual int ti84_printCenter();
  virtual int ti84_printHL();
  virtual int ti84_putc();
  virtual int ti84_fillRect();
  virtual int ti84_setCursorX();
  virtual int ti84_setCursorY();
  virtual int ti84_setDrawBGColor();
  virtual int ti84_setDrawBGWhite();
  virtual int ti84_setDrawColor();
  virtual int ti84_setFillColor();
  virtual int ti84_setTextBGColor();
  virtual int ti84_setTextColor();

private:
  int model;
};

enum
{
  TI84_PLUS,
  TI84_PLUS_C,
};

#endif

