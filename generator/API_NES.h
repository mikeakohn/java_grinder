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

#ifndef JAVA_GRINDER_GENERATOR_API_NES_H
#define JAVA_GRINDER_GENERATOR_API_NES_H

class API_NES
{
public:
  // Nintendo Entertainment System.
  virtual int nes_setBackgroundPalette_II() { return -1; }
  virtual int nes_setSpritePalette_II() { return -1; }
  virtual int nes_setPattern_IaB() { return -1; }
  virtual int nes_setNameTable_II() { return -1; }
  virtual int nes_setNameTable_III() { return -1; }
};

#endif

