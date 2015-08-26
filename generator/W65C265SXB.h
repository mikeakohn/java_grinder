/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 * W65C265SXB by Joe Davisson
 *
 */

#ifndef _W65C265SXB_H
#define _W65C265SXB_H

#include "W65816.h"

class W65C265SXB : public W65816
{
public:
  W65C265SXB();
  virtual ~W65C265SXB();

  virtual int open(const char *filename);

private:
};

#endif

