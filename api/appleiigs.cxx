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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "api/appleiigs.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->appleiigs_##funct##sig(); \
  }

#define CHECK_FUNC_CONST_1(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->appleiigs_##funct##sig(const_val); \
  }

int appleiigs(JavaClass *java_class, Generator *generator, const char *function)
{
  CHECK_FUNC(plotChar,_IC)
  CHECK_FUNC(printChar,_C)
  CHECK_FUNC(hiresEnable,_I)
  CHECK_FUNC(hiresClear,_I)
  CHECK_FUNC(hiresUpdate,_II)
  CHECK_FUNC(hiresPlot,_III)
  CHECK_FUNC(hiresLine,_IIIII)
  CHECK_FUNC(hiresSpan,_IIII)
  CHECK_FUNC(hiresRead,_II)
  CHECK_FUNC(hiresSprite,_aBIIII)
  CHECK_FUNC(hiresPalette,_IaI)
  CHECK_FUNC(hiresSetRow,_II)
  CHECK_FUNC(rnd,)

  CHECK_FUNC(loadWaveTable,_aBI)
  CHECK_FUNC(enableOscillators,_I)
  CHECK_FUNC(setMasterVolume,_I)
  CHECK_FUNC(setSoundVolume,_II)
  CHECK_FUNC(setSoundFrequency,_II)
  CHECK_FUNC(setWaveTable,_III)

  return -1;
}

int appleiigs(JavaClass *java_class, Generator *generator, const char *function, int const_val)
{
  return -1;
}


