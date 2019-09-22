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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "api/amiga.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->amiga_##funct##sig(); \
  }

#define CHECK_FUNC_CONST_1(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->amiga_##funct##sig(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->amiga_##funct##sig(const_val1, const_val2); \
  }

#define CHECK_FUNC_COPPER(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->copper_##funct##sig(); \
  }

#define CHECK_FUNC_BLITTER(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->blitter_##funct##sig(); \
  }

int amiga(JavaClass *java_class, Generator *generator, const char *method_name)
{
  CHECK_FUNC(disableMultitasking,)
  CHECK_FUNC(enableMultitasking,)
  CHECK_FUNC(disableInterrupts,)
  CHECK_FUNC(enableInterrupts,)
  CHECK_FUNC(setPalette,_II)
  CHECK_FUNC(setSpriteImage,_IaS)
  CHECK_FUNC(setSpritePosition,_IIII)
  CHECK_FUNC(setVideoMode,_I)
  CHECK_FUNC(setPlayfieldScroll,_II)
  CHECK_FUNC(setPlayfieldPriority,_IIZ)
  CHECK_FUNC(setBitplaneModuloEven,_I)
  CHECK_FUNC(setBitplaneModuloOdd,_I)
  CHECK_FUNC(setDisplayWindowStart,_II)
  CHECK_FUNC(setDisplayWindowStop,_II)
  CHECK_FUNC(setDisplayBitplaneStart,_I)
  CHECK_FUNC(setDisplayBitplaneStop,_I)
  CHECK_FUNC(setAudioData,_IaB)
  CHECK_FUNC(setAudioLength,_II)
  CHECK_FUNC(setAudioPeriod,_II)
  CHECK_FUNC(setAudioVolume,_II)
  CHECK_FUNC(setAudioModulation,_I)
  CHECK_FUNC(clearAudioModulation,_I)
  CHECK_FUNC(setDMA,_I)
  CHECK_FUNC(clearDMA,_I)

  return -1;
}

int amiga(JavaClass *java_class, Generator *generator, const char *method_name, int const_val)
{
  return -1;
}

int amiga(JavaClass *java_class, Generator *generator, const char *function, int const_val1, int const_val2)
{
  return -1;
}

int copper(JavaClass *java_class, Generator *generator, const char *method_name)
{
  CHECK_FUNC_COPPER(appendInstruction,_I)
  CHECK_FUNC_COPPER(appendWait,_II)
  CHECK_FUNC_COPPER(appendSkip,_II)
  CHECK_FUNC_COPPER(appendSetColor,_II)
  CHECK_FUNC_COPPER(appendSetBitplane,_IaB)
  CHECK_FUNC_COPPER(appendSetSprite,_IaC)
  CHECK_FUNC_COPPER(appendEnd,)
  CHECK_FUNC_COPPER(resetIndex,)
  CHECK_FUNC_COPPER(setIndex,_I)
  CHECK_FUNC_COPPER(run,)
  CHECK_FUNC_COPPER(stop,)

  return -1;
}

int blitter(JavaClass *java_class, Generator *generator, const char *method_name)
{
  CHECK_FUNC_BLITTER(waitBusy,)
  CHECK_FUNC_BLITTER(setSourceA,_aB)
  CHECK_FUNC_BLITTER(setSourceB,_aB)
  CHECK_FUNC_BLITTER(setSourceC,_aB)
  CHECK_FUNC_BLITTER(setDestination,_aB)
  CHECK_FUNC_BLITTER(setModuloA,_I)
  CHECK_FUNC_BLITTER(setModuloB,_I)
  CHECK_FUNC_BLITTER(setModuloC,_I)
  CHECK_FUNC_BLITTER(setModuloDestination,_I)
  CHECK_FUNC_BLITTER(setShiftA,_I)
  CHECK_FUNC_BLITTER(setShiftB,_I)
  CHECK_FUNC_BLITTER(setChannelAMasks,_II)
  CHECK_FUNC_BLITTER(enableChannels,_I)
  CHECK_FUNC_BLITTER(setAsFillMode,_I)
  CHECK_FUNC_BLITTER(setAsLineMode,_I)
  CHECK_FUNC_BLITTER(setLogicalFunction,_I)
  CHECK_FUNC_BLITTER(setSize,_II)
  CHECK_FUNC_BLITTER(setLineTypeA,_I)
  CHECK_FUNC_BLITTER(setLineTypeB,_I)
  CHECK_FUNC_BLITTER(setLineTypeC,_I)
  CHECK_FUNC_BLITTER(setLineTexture,_I)
  CHECK_FUNC_BLITTER(setLineStart,_I)
  CHECK_FUNC_BLITTER(runFill,_II)
  CHECK_FUNC_BLITTER(drawLine,_I)

  return -1;
}

