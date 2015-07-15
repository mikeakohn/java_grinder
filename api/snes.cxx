/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 * SNES by Joe Davisson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "JavaClass.h"
#include "snes.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->snes_##funct(); \
  }

#define CHECK_FUNC_CONST(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->snes_##funct(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->snes_##funct(const_val1, const_val2); \
  }

int snes(JavaClass *java_class, Generator *generator, char *method_name)
{
  return -1;
}

int snes(JavaClass *java_class, Generator *generator, char *method_name, int const_val)
{
  return -1;
}

int snes(JavaClass *java_class, Generator *generator, char *function, int const_val1, int const_val2)
{
  return -1;
}

