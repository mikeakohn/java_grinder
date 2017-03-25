/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2017 by Michael Kohn
 *
 */

#ifndef _MIPS64_H
#define _MIPS64_H

#include "MIPS32.h"

class MIPS64 : public MIPS32
{
public:
  MIPS64();
  virtual ~MIPS64();

  virtual int open(const char *filename);
  virtual int start_init();

private:
};

#endif

