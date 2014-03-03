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

#ifndef _C64_H
#define _C64_H

#include "M6502.h"

class C64 : public M6502
{
public:
  C64();
  virtual ~C64();

  virtual int open(char *filename);

  // VIC API
  virtual int c64_vic_border();
  virtual int c64_vic_background();
  virtual int c64_vic_sprite_enable();
  virtual int c64_vic_sprite_pos();
  virtual int c64_vic_poke();
};

#endif

