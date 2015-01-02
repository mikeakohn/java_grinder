/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

#ifndef _API_TI84_H
#define _API_TI84_H

class API_TI84
{
public:
  // TI-84 plus / TI-84 plus C
  virtual int ti84_clearRect() { return -1; }
  virtual int ti84_clearScreen() { return -1; }
  virtual int ti84_drawLine() { return -1; }
  virtual int ti84_drawPoint() { return -1; }
  virtual int ti84_print() { return -1; }
  virtual int ti84_printCenter() { return -1; }
  virtual int ti84_printHL() { return -1; }
  virtual int ti84_putc() { return -1; }
  virtual int ti84_fillRect() { return -1; }
  virtual int ti84_setCursorX() { return -1; }
  virtual int ti84_setCursorY() { return -1; }
  virtual int ti84_setDrawBGColor() { return -1; }
  virtual int ti84_setDrawBGWhite() { return -1; }
  virtual int ti84_setDrawColor() { return -1; }
  virtual int ti84_setFillColor() { return -1; }
  virtual int ti84_setTextBGColor() { return -1; }
  virtual int ti84_setTextColor() { return -1; }
};

#endif

