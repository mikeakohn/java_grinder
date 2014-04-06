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

#ifndef _API_TI84C_H
#define _API_TI84C_H

class API_TI84C
{
public:
  // TI-84 plus C Color Calculator
  virtual int ti84c_clearRect() { return -1; }
  virtual int ti84c_clearScreen() { return -1; }
  virtual int ti84c_drawLine() { return -1; }
  virtual int ti84c_drawPoint() { return -1; }
  virtual int ti84c_print() { return -1; }
  virtual int ti84c_printCenter() { return -1; }
  virtual int ti84c_printHL() { return -1; }
  virtual int ti84c_putc() { return -1; }
  virtual int ti84c_fillRect() { return -1; }
  virtual int ti84c_setCursorX() { return -1; }
  virtual int ti84c_setCursorY() { return -1; }
  virtual int ti84c_setDrawBGColor() { return -1; }
  virtual int ti84c_setDrawBGWhite() { return -1; }
  virtual int ti84c_setDrawColor() { return -1; }
  virtual int ti84c_setFillColor() { return -1; }
  virtual int ti84c_setTextBGColor() { return -1; }
  virtual int ti84c_setTextColor() { return -1; }
};

#endif

