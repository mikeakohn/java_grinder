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
  virtual int clearRect() { return -1; }
  virtual int drawHL() { return -1; }
  virtual int drawLine() { return -1; }
  virtual int drawPoint() { return -1; }
  virtual int drawString() { return -1; }
  virtual int drawStringCenter() { return -1; }
  virtual int fillRect() { return -1; }
  virtual int setCursorX() { return -1; }
  virtual int setCursorY() { return -1; }
  virtual int setDrawBGColor() { return -1; }
  virtual int setDrawBGWhite() { return -1; }
  virtual int setDrawColor() { return -1; }
  virtual int setTextBGColor() { return -1; }
  virtual int setTextColor() { return -1; }
};

#endif

