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

#include "JavaClass.h"
#include "propeller_.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->propeller_##funct##sig(); \
  }

#define CHECK_FUNC_CONST(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->propeller_##funct##sig(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->propeller_##funct##sig(const_val1, const_val2); \
  }

int propeller(JavaClass *java_class, Generator *generator, const char *method_name)
{
  CHECK_FUNC(setClock_I,)
  CHECK_FUNC(getCogId,)
  CHECK_FUNC(stopCog_I,)
  CHECK_FUNC(waitCount,_II)
  CHECK_FUNC(waitPinsEqual,_II)
  CHECK_FUNC(waitPinsNotEqual,_II)

  return -1;
}

int propeller(JavaClass *java_class, Generator *generator, const char *method_name, int const_val)
{
  CHECK_FUNC_CONST(waitCount,_II)
  CHECK_FUNC_CONST(waitPinsEqual,_II)
  CHECK_FUNC_CONST(waitPinsNotEqual,_II)

  return -1;
}

int propeller(JavaClass *java_class, Generator *generator, const char *function, int const_val1, int const_val2)
{
  //CHECK_FUNC_CONST_2(someFunction,_II)

  return -1;
}

