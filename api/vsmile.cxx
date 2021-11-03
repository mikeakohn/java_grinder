/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2021 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "api/vsmile.h"

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

int vsmile(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name)
{
  //CHECK_FUNC(disableMultitasking,)
  //CHECK_FUNC(enableMultitasking,)

  return -1;
}

int vsmile(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name,
  int const_val)
{
  return -1;
}

int vsmile(
  JavaClass *java_class,
  Generator *generator,
  const char *function,
  int const_val1,
  int const_val2)
{
  return -1;
}

