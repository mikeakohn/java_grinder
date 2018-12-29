/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 */

#ifndef _AMIGA_H
#define _AMIGA_H

#include "generator/MC68000.h"

class Amiga : public MC68000
{
public:
  Amiga();
  virtual ~Amiga();

  virtual int open(const char *filename);
  virtual int start_init();
  virtual int amiga_setPalette_II();

private:
};

#endif

