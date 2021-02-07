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

#include "api/joystick.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->joystick_##funct##sig(); \
  }

#define CHECK_FUNC_CONST(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->joystick_##funct##sig(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->joystick_##funct##sig(const_val1, const_val2); \
  }

int joystick(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name)
{
  CHECK_FUNC(isLeft,_I)
  CHECK_FUNC(isRight,_I)
  CHECK_FUNC(isDown,_I)
  CHECK_FUNC(isUp,_I)
  CHECK_FUNC(isButtonDown_0,_I)

  return -1;
}

int joystick(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name,
  int const_val)
{
  CHECK_FUNC_CONST(isLeft,_I)
  CHECK_FUNC_CONST(isRight,_I)
  CHECK_FUNC_CONST(isDown,_I)
  CHECK_FUNC_CONST(isUp,_I)
  CHECK_FUNC_CONST(isButtonDown_0,_I)

  return -1;
}

int joystick(
  JavaClass *java_class,
  Generator *generator,
  const char *function,
  int const_val1,
  int const_val2)
{
  return -1;
}

