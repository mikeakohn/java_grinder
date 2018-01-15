/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "JavaClass.h"
#include "playstation_2.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->playstation2_##funct##sig(); \
  }

#define CHECK_FUNC_CONST_1(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->playstation2_##funct##sig(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->playstation2_##funct##sig(const_val1, const_val2); \
  }

int playstation2(JavaClass *java_class, Generator *generator, char *method_name)
{
  //CHECK_FUNC(setVideoMode,_III)
  //CHECK_FUNC(setFrameBuffer1,_IIIII)
  //CHECK_FUNC(setFrameBuffer2,_IIIII)
  //CHECK_FUNC(setDisplay1,_IIIIII)
  //CHECK_FUNC(setDisplay2,_IIIIII)
  CHECK_FUNC(waitVsync,)

  return -1;
}

int playstation2(JavaClass *java_class, Generator *generator, char *method_name, int const_val)
{
  return -1;
}

int playstation2(JavaClass *java_class, Generator *generator, char *function, int const_val1, int const_val2)
{
  return -1;
}

