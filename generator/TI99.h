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

#ifndef _TI99_H
#define _TI99_H

#include "TMS9900.h"

class TI99 : public TMS9900
{
public:
  TI99();
  virtual ~TI99();

  virtual int open(const char *filename);
  virtual int start_init();
  virtual int ti99_print();
  virtual int ti99_printChar();
  virtual int ti99_printChar(int c);
  virtual int ti99_setCursor();
  virtual int ti99_setCursor(int x, int y);
};

#endif

