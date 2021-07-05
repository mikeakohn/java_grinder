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

#include "api/i2c.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->i2c_##funct##sig(); \
  }

#define CHECK_FUNC_CONST(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->i2c_##funct##sig(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->i2c_##funct##sig(const_val1, const_val2); \
  }

int i2c(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name)
{
  CHECK_FUNC(init,_I)
  CHECK_FUNC(enable,)
  CHECK_FUNC(disable,)
  CHECK_FUNC(start,)
  CHECK_FUNC(stop,)
  CHECK_FUNC(read,)
  CHECK_FUNC(write,_I)

  return -1;
}

int i2c(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name,
  int const_val)
{
  CHECK_FUNC_CONST(init,_I)
  return -1;
}

#if 0
int i2c(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name,
  int const_val1,
  int const_val2)
{
  return -1;
}
#endif

