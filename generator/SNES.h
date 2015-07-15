/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 * SNES by Joe Davisson
 *
 */

#ifndef _SNES_H
#define _SNES_H

#include "W65816.h"

class SNES : public W65816
{
public:
  SNES();
  virtual ~SNES();

  virtual int open(const char *filename);

private:
};

#endif

