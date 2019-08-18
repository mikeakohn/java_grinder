/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2019 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_AMIGA_H
#define JAVA_GRINDER_GENERATOR_AMIGA_H

#include "generator/MC68000.h"

class Amiga : public MC68000
{
public:
  Amiga();
  virtual ~Amiga();

  virtual int open(const char *filename);
  virtual int start_init();
  virtual int new_object(std::string &object_name, int field_count);
  virtual int amiga_disableMultitasking();
  virtual int amiga_enableMultitasking();
  virtual int amiga_disableInterrupts();
  virtual int amiga_enableInterrupts();
  virtual int amiga_setPalette_II();
  virtual int amiga_setSpriteImage_IaI();
  virtual int amiga_setSpritePosition_IIII();
  virtual int amiga_setVideoMode_IBBB();
  virtual int amiga_setPlayfieldScroll_II();
  virtual int amiga_setPlayfieldPriority_IIB();

#if 0
  virtual int copper_setWait_aIIII();
  virtual int copper_setColor_aIIII();
  virtual int copper_setBitplane_aIIII();
  virtual int copper_setMove_aIIII();
  virtual int copper_setSkip_aIIII();
  virtual int copper_setWaitEnd_aII();
#endif
  virtual int copper_Constructor_I();
  virtual int copper_appendInstruction_I();
  virtual int copper_appendWait_II();
  virtual int copper_appendSkip_II();
  virtual int copper_appendSetColor_II();
  virtual int copper_appendSetBitplane_II();
  virtual int copper_appendEnd();
  virtual int copper_resetIndex();
  virtual int copper_setIndex_I();

private:
  int copper_getNextIndexAndIncrement(int reg);
};

#endif

