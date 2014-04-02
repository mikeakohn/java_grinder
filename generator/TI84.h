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
  TI84();
  virtual ~TI84();

  virtual int open(const char *filename);
  virtual int start_init();
};

#endif

