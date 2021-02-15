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

#include "api/intellivision.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->intellivision_##funct##sig(); \
  }

#define CHECK_FUNC_CONST(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->intellivision_##funct##sig(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->intellivision_##funct##sig(const_val1, const_val2); \
  }

int intellivision(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name)
{
  CHECK_FUNC(plot,_II)
  CHECK_FUNC(plot,_III)
  CHECK_FUNC(readDisplay,_II)
  CHECK_FUNC(waitForVerticalBlank,)
  CHECK_FUNC(setHorizontalDelay,_I)
  CHECK_FUNC(setVerticalDelay,_I)
  CHECK_FUNC(setVideoMode,_I)
  CHECK_FUNC(setColorStack,_II)
  CHECK_FUNC(getControllerLeft,)
  CHECK_FUNC(getControllerRight,)
  CHECK_FUNC(setSoundChannelPeriod,_II)
  CHECK_FUNC(setSoundChannelVolume,_II)
  CHECK_FUNC(setSoundEnvelopePeriod,_I)
  CHECK_FUNC(setSoundEnvelopeType,_I)
  CHECK_FUNC(setSoundNoisePeriod,_I)
  CHECK_FUNC(setSoundType,_I)

  return -1;
}

int intellivision(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name,
  int const_val)
{
  return -1;
}

int intellivision(
  JavaClass *java_class,
  Generator *generator,
  const char *function,
  int const_val1,
  int const_val2)
{
  return -1;
}

