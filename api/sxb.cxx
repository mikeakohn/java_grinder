/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 * Western Design Center SXB board by Joe Davisson.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "JavaClass.h"
#include "sxb.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->sxb_##funct##sig(); \
  }

#define CHECK_FUNC_CONST_1(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->sxb_##funct##sig(const_val); \
  }

int sxb(JavaClass *java_class, Generator *generator, char *method_name)
{
  CHECK_FUNC(getChar,)
  CHECK_FUNC(putChar,_C)
  CHECK_FUNC(getInt,)
  CHECK_FUNC(putInt,_I)
  CHECK_FUNC(getString,)
  CHECK_FUNC(putString,_X)
  CHECK_FUNC(controlTones,_IIZZ)

  return -1;
}

int sxb(JavaClass *java_class, Generator *generator, char *method_name, int const_val)
{
  return -1;
}


