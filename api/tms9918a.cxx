/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2023 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "api/tms9918a.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->tms9918a_##funct##sig(); \
  }

#define CHECK_FUNC_CONST(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->tms9918a_##funct##sig(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->tms9918a_##funct##sig(const_val1, const_val2); \
  }

int tms9918a(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name)
{
  CHECK_FUNC(initDisplay,)
  CHECK_FUNC(setGraphicsMode,_I)
  CHECK_FUNC(setPattern,_IaB)
  CHECK_FUNC(setColor,_II)
  CHECK_FUNC(setTextBackdropColor,_I)
  CHECK_FUNC(clearScreen,)
  CHECK_FUNC(print,_X)
  CHECK_FUNC(printChar,_C)
  CHECK_FUNC(setCursor,_II)
  CHECK_FUNC(plot,_III)
  CHECK_FUNC(setSpriteVisible,_IZ)
  CHECK_FUNC(setSpriteImage,_IaB)
  CHECK_FUNC(setSpritePos,_III)
  CHECK_FUNC(setSpriteColor,_II)
  CHECK_FUNC(setSpriteSize,_I)

  return -1;
}

int tms9918a(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name,
  int const_val)
{
  CHECK_FUNC_CONST(setGraphicsMode,_I)
  CHECK_FUNC_CONST(printChar,_C)

  return -1;
}

int tms9918a(
  JavaClass *java_class,
  Generator *generator,
  const char *function,
  int const_val1,
  int const_val2)
{
  CHECK_FUNC_CONST_2(setCursor,_II)

  return -1;
}

