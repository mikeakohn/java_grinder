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
#include "ti99_.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->ti99_##funct(); \
  }

#define CHECK_FUNC_CONST(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->ti99_##funct(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->ti99_##funct(const_val1, const_val2); \
  }

int ti99(JavaClass *java_class, Generator *generator, char *method_name)
{
  CHECK_FUNC(print,_Ljava/lang/String;)
  CHECK_FUNC(printChar,_C)
  CHECK_FUNC(setCursor,_II)
  //CHECK_FUNC(setTextColor,_III)
  CHECK_FUNC(setGraphicsMode,_I)
  CHECK_FUNC(clearScreen,)
  CHECK_FUNC(plot,_III)
  CHECK_FUNC(setColors,)
  CHECK_FUNC(setSoundFreq,_II)
  CHECK_FUNC(setSoundVolume,_II)
  CHECK_FUNC(setSpriteVisible,_II)
  CHECK_FUNC(setSpriteImage,_IB)
  CHECK_FUNC(setSpritePos,_III)
  CHECK_FUNC(setSpriteColor,_I)

  return -1;
}

int ti99(JavaClass *java_class, Generator *generator, char *method_name, int const_val)
{
  CHECK_FUNC_CONST(printChar,_C)
  CHECK_FUNC_CONST(setGraphicsMode,_I)
  //CHECK_FUNC_CONST(setSoundFreq,_II)
  //CHECK_FUNC_CONST(setSoundVolume,_II)

  return -1;
}

int ti99(JavaClass *java_class, Generator *generator, char *function, int const_val1, int const_val2)
{
  CHECK_FUNC_CONST_2(setCursor,_II)

  return -1;
}

