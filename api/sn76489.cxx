/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2022 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "api/sn76489.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->sn76489_##funct##sig(); \
  }

#define CHECK_FUNC_CONST(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->sn78489_##funct##sig(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->sn78489_##funct##sig(const_val1, const_val2); \
  }

int sn76489(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name)
{
  CHECK_FUNC(setSoundFreq,_II)
  CHECK_FUNC(setSoundVolume,_II)

  return -1;
}

int sn76489(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name,
  int const_val)
{
  //CHECK_FUNC_CONST(setSoundFreq,_II)
  //CHECK_FUNC_CONST(setSoundVolume,_II)

  return -1;
}

int sn76489(
  JavaClass *java_class,
  Generator *generator,
  const char *function,
  int const_val1,
  int const_val2)
{

  return -1;
}

