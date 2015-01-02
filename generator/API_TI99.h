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

#ifndef _API_TI99_H
#define _API_TI99_H

class API_TI99
{
public:
  // TI99/4A
  virtual int ti99_print() { return -1; }
  virtual int ti99_printChar() { return -1; }
  virtual int ti99_printChar(int c) { return -1; }
  virtual int ti99_setCursor() { return -1; }
  virtual int ti99_setCursor(int x, int y) { return -1; }
  //virtual int ti99_setTextColor() { return -1; }
  virtual int ti99_setGraphicsMode() { return -1; }
  virtual int ti99_setGraphicsMode(int mode) { return -1; }
  virtual int ti99_clearScreen() { return -1; }
  virtual int ti99_plot() { return -1; }
};

#endif

