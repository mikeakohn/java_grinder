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

int amiga(JavaClass *java_class, Generator *generator, const char *method_name)
{
  CHECK_FUNC(disableMultitasking,)
  CHECK_FUNC(enableMultitasking,)
  CHECK_FUNC(disableInterrupts,)
  CHECK_FUNC(enableInterrupts,)
  CHECK_FUNC(setPalette,_II)
  CHECK_FUNC(setSpriteImage,_IaI)
  CHECK_FUNC(setSpritePosition,_IIII)
  CHECK_FUNC(setVideoMode,_IBBB)
  CHECK_FUNC(setPlayfieldScroll,_II)
  CHECK_FUNC(setPlayfieldPriority,_IIB)
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
  CHECK_FUNC_COPPER(setWait,_aIIII)
  CHECK_FUNC_COPPER(setColor,_aIIII)
  CHECK_FUNC_COPPER(setBitplane,_aIIII)
  CHECK_FUNC_COPPER(setMove,_aIIII)
  CHECK_FUNC_COPPER(setSkip,_aIIII)
  CHECK_FUNC_COPPER(setWaitEnd,_aII)

  return -1;
}

