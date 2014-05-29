/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "JavaClass.h"
#include "timer.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->timer_##funct(); \
  }

#define CHECK_FUNC_CONST_1(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->timer_##funct(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->timer_##funct(const_val1, const_val2); \
  }

int timer(JavaClass *java_class, Generator *generator, char *function)
{
  CHECK_FUNC(setInterval,II)
  CHECK_FUNC(setListener,Z)
  CHECK_FUNC(getValue,)
  CHECK_FUNC(setValue,I)

  return -1;
}

int timer(JavaClass *java_class, Generator *generator, char *function, int const_val)
{
  CHECK_FUNC_CONST_1(setListener,Z)
  CHECK_FUNC_CONST_1(setValue,I)
  return -1;
}

int timer(JavaClass *java_class, Generator *generator, char *function, int const_val1, int const_val2)
{
  CHECK_FUNC_CONST_2(setInterval,II)
  return -1;
}



