/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 * SNES by Joe Davisson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "api/snes.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->snes_##funct##sig(); \
  }

#define CHECK_FUNC_CONST_1(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->snes_##funct##sig(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->snes_##funct##sig(const_val1, const_val2); \
  }

int snes(JavaClass *java_class, Generator *generator, const char *method_name)
{
  CHECK_FUNC(setBackgroundColor,_I)

  return -1;
}

int snes(JavaClass *java_class, Generator *generator, const char *method_name, int const_val)
{
  return -1;
}

int snes(JavaClass *java_class, Generator *generator, const char *function, int const_val1, int const_val2)
{
  return -1;
}

