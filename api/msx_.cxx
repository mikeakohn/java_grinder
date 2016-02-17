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

/*
 *   MSX support by Giovanni Nunes - https://github.com/plainspooky
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "JavaClass.h"
#include "msx_.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->msx_##funct##sig(); \
  }

#define CHECK_FUNC_CONST(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->msx_##funct##sig(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->msx_##funct##sig(const_val1, const_val2); \
  }

int msx(JavaClass *java_class, Generator *generator, char *method_name)
{
  //CHECK_FUNC(someFunction,_I)
  CHECK_FUNC(beep,_I)
  CHECK_FUNC(color,_I)
  CHECK_FUNC(cls,_I)
  CHECK_FUNC(screen,_I)
  CHECK_FUNC(keyOn,_I)
  CHECK_FUNC(keyOff,_I)
  CHECK_FUNC(fillVRAM,_I)
  CHECK_FUNC(copyVRAM,_I)
  CHECK_FUNC(putChar,_C)
  CHECK_FUNC(setCursor,_I)

  return -1;
}

int msx(JavaClass *java_class, Generator *generator, char *method_name, int const_val)
{
  //CHECK_FUNC_CONST(someFunction,_I)

  return -1;
}

int msx(JavaClass *java_class, Generator *generator, char *function, int const_val1, int const_val2)
{
  //CHECK_FUNC_CONST_2(someFunction,_II)

  return -1;
}
