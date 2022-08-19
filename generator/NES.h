/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2022 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_NES_H
#define JAVA_GRINDER_GENERATOR_NES_H

#include "generator/M6502.h"

class NES : public M6502
{
public:
  NES();
  virtual ~NES();

  virtual int open(const char *filename);
  virtual int nes_setBackgroundPalette_II();
  virtual int nes_setSpritePalette_II();
  virtual int nes_setPattern_IaB();
  virtual int nes_setNameTable_II();
  virtual int nes_setNameTable_III();

private:
  void write_init();
  void write_interrupts();
  void write_cartridge_info();

};

#endif

