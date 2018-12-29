/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "api/math_.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->math_##funct##sig(); \
  }

#define CHECK_FUNC_CONST(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->math_##funct##sig(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->math_##funct##sig(const_val1, const_val2); \
  }

int math(JavaClass *java_class, Generator *generator, const char *method_name)
{
  CHECK_FUNC(abs,_I)
  CHECK_FUNC(xor,_I)
  CHECK_FUNC(min,_II)
  CHECK_FUNC(max,_II)
  CHECK_FUNC(sin512,_I)
  CHECK_FUNC(cos512,_I)

  return -1;
}

#if 0
int math(JavaClass *java_class, Generator *generator, const char *method_name, int const_val)
{
  //CHECK_FUNC_CONST(stopCog,_I)

  return -1;
}

int math(JavaClass *java_class, Generator *generator, const char *function, int const_val1, int const_val2)
{
  //CHECK_FUNC_CONST_2(someFunction,_II)

  return -1;
}
#endif

