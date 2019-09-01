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
  virtual int init_heap(int field_count);
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

  virtual int copper_Constructor_I();
  virtual int copper_appendInstruction_I();
  virtual int copper_appendWait_II();
  virtual int copper_appendSkip_II();
  virtual int copper_appendSetColor_II();
  virtual int copper_appendSetBitplane_II();
  virtual int copper_appendEnd();
  virtual int copper_resetIndex();
  virtual int copper_setIndex_I();
  virtual int copper_run();
  virtual int copper_stop();
  virtual int copper_getArrayAsInt();
  virtual int copper_getArrayAsChar();

  virtual int blitter_Constructor();
  virtual int blitter_setSourceA_aB();
  virtual int blitter_setSourceB_aB();
  virtual int blitter_setSourceC_aB();
  virtual int blitter_setDestination_aB();
  virtual int blitter_setModuloA_I();
  virtual int blitter_setModuloB_I();
  virtual int blitter_setModuloC_I();
  virtual int blitter_setModuloDestination_I();
  virtual int blitter_setShiftA_I();
  virtual int blitter_setShiftB_I();
  virtual int blitter_setChannelAMasks_II();
  virtual int blitter_enableChannels_I();
  virtual int blitter_setAsFillMode_I();
  virtual int blitter_setAsLineMode_I();
  virtual int blitter_setLogicalFunction_I();
  virtual int blitter_setSize_II();
  virtual int blitter_setLineTypeA_I();
  virtual int blitter_setLineTypeB_I();
  virtual int blitter_setLineTypeC_I();
  virtual int blitter_setLineTexture_I();
  virtual int blitter_setLineStart_I();
  virtual int blitter_runFill_II();
  virtual int blitter_drawLine_I();

  virtual int memory_clearArray_aB();
  virtual int memory_clearArray_aS();
  virtual int memory_clearArray_aI();

private:
  int copper_getNextIndexAndIncrement(int reg);
};

#endif

