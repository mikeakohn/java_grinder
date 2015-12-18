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

#ifndef _ATARI_2600_H
#define _ATARI_2600_H

#include "M6502.h"

class Atari2600 : public M6502
{
public:
  Atari2600();
  virtual ~Atari2600();

  virtual int open(const char *filename);
  virtual int atari2600_waitHsync();

private:
  void print_tia_definitions();
};

#endif

