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
  virtual int amiga_setVideoMode_IBBB() { return -1; }
  virtual int amiga_setPlayfieldScroll_II() { return -1; }
  virtual int amiga_setPlayfieldPriority_IIB() { return -1; }

  virtual int copper_Constructor_I() { return -1; }
  virtual int copper_appendInstruction_I() { return -1; }
  virtual int copper_appendWait_II() { return -1; }
  virtual int copper_appendSkip_II() { return -1; }
  virtual int copper_appendSetColor_II() { return -1; }
  virtual int copper_appendSetBitplane_II() { return -1; }
  virtual int copper_appendEnd() { return -1; }
  virtual int copper_resetIndex() { return -1; }
  virtual int copper_setIndex_I() { return -1; }
  virtual int copper_run() { return -1; }
  virtual int copper_stop() { return -1; }
  virtual int copper_getArrayAsInt() { return -1; }
  virtual int copper_getArrayAsChar() { return -1; }
};

#endif

