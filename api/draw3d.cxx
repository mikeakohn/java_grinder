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
#include "draw3d.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->draw3d_##funct##sig(); \
  }

#define CHECK_FUNC_CONST_1(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->draw3d_##funct##sig(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->draw3d_##funct##sig(const_val1, const_val2); \
  }

int draw3d(JavaClass *java_class, Generator *generator, char *method_name)
{
  //CHECK_FUNC(Constructor,_X)
  //CHECK_FUNC(Constructor,_I)
  CHECK_FUNC(rotate512,_III)
  CHECK_FUNC(setPosition,_FFF)
  CHECK_FUNC(setPointPosition,_IFFF)
  CHECK_FUNC(setPointColor,_II)
  CHECK_FUNC(draw,)

  return -1;
}

int draw3d(JavaClass *java_class, Generator *generator, char *method_name, int const_val)
{
  return -1;
}

int draw3d(JavaClass *java_class, Generator *generator, char *function, int const_val1, int const_val2)
{
  return -1;
}

