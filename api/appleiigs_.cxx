/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "JavaClass.h"
#include "appleiigs_.h"

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

int appleiigs(JavaClass *java_class, Generator *generator, char *function)
{
  CHECK_FUNC(plotChar,_IC)
  CHECK_FUNC(printChar,_C)
  CHECK_FUNC(hiresEnable,)
  CHECK_FUNC(hiresPlot,_II)
  CHECK_FUNC(hiresSpan,_III)
  CHECK_FUNC(hiresRead,_I)
  CHECK_FUNC(hiresBlit,_aBIII)
  CHECK_FUNC(hiresPalette,_IaI)
  CHECK_FUNC(hiresSetLine,_II)

  CHECK_FUNC(loadWaveTable,_BA)
  CHECK_FUNC(enableOscillators,_I)
  CHECK_FUNC(setMasterVolume,_I)
  CHECK_FUNC(setSoundVolume,_II)
  CHECK_FUNC(setSoundFrequency,_II)

  return -1;
}

int appleiigs(JavaClass *java_class, Generator *generator, char *function, int const_val)
{
  return -1;
}


