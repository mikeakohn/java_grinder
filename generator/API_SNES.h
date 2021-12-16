/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2021 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_API_SNES_H
#define JAVA_GRINDER_GENERATOR_API_SNES_H

class API_SNES
{
public:
  // Super Nintendo
  virtual int snes_setBackgroundColor_I() { return -1; }
};

#endif

