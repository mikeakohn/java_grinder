/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 * SNES by Joe Davisson
 *
 */

#ifndef _SNES_H
#define _SNES_H

#include "generator/W65816.h"

class SNES : public W65816
{
public:
  SNES();
  virtual ~SNES();

  virtual int open(const char *filename);
  virtual int snes_setBackgroundColor_I();

private:
  void write_interrupts();
  void write_cartridge_info();
};

#endif

