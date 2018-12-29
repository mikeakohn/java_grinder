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

#include "api/trs80_coco.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->trs80_coco_##funct##sig(); \
  }

#define CHECK_FUNC_CONST(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->trs80_coco_##funct##sig(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->trs80_coco_##funct##sig(const_val1, const_val2); \
  }

int trs80_coco(JavaClass *java_class, Generator *generator, const char *method_name)
{
  CHECK_FUNC(clearScreenLores,)
  CHECK_FUNC(clearScreenMidres,)
  //CHECK_FUNC(setBackgroundColor,_I)
  CHECK_FUNC(setText,_II)
  CHECK_FUNC(setGraphicsMode,)
  CHECK_FUNC(setTextMode,)
  CHECK_FUNC(print,_X)

  CHECK_FUNC(plotLores,_III)
  CHECK_FUNC(plotMidres,_III)

  CHECK_FUNC(initVideoFlags,)
  CHECK_FUNC(waitForHsync,)
  CHECK_FUNC(waitForVsync,)

  CHECK_FUNC(initSound,)
  CHECK_FUNC(setSound,_aB)

  return -1;
}

int trs80_coco(JavaClass *java_class, Generator *generator, const char *method_name, int const_val)
{
  //CHECK_FUNC_CONST(setBackgroundColor,_I)

  return -1;
}

int trs80_coco(JavaClass *java_class, Generator *generator, const char *function, int const_val1, int const_val2)
{
  //CHECK_FUNC_CONST_2(someFunction,_II)

  return -1;
}

