/**
 *  Java Grinder
 *  Author: Joe Davisson
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2019 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_API_AMIGA_H
#define JAVA_GRINDER_GENERATOR_API_AMIGA_H

class API_Amiga
{
public:
  virtual int amiga_disableMultitasking() { return -1; }
  virtual int amiga_enableMultitasking() { return -1; }
  virtual int amiga_disableInterrupts() { return -1; }
  virtual int amiga_enableInterrupts() { return -1; }
  virtual int amiga_setPalette_II() { return -1; }
  virtual int amiga_setSpriteImage_IaI() { return -1; }
  virtual int amiga_setSpritePosition_IIII() { return -1; }
};

#endif

