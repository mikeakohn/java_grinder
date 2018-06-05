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
#include "memory.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->memory_##funct##sig(); \
  }

#define CHECK_FUNC_CONST(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->memory_##funct##sig(const_val); \
  }

#define CHECK_FUNC_STRING(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->memory_##funct##sig(const_val); \
  }

int memory(JavaClass *java_class, Generator *generator, char *function)
{
  CHECK_FUNC(read8,_I)
  CHECK_FUNC(write8,_IB)
  CHECK_FUNC(read16,_I)
  CHECK_FUNC(write16,_IS)
  CHECK_FUNC(allocStackBytes,_I)
  CHECK_FUNC(allocStackShorts,_I)
  CHECK_FUNC(allocStackInts,_I)

  return -1;
}

int memory(JavaClass *java_class, Generator *generator, char *function, int const_val)
{
  CHECK_FUNC_CONST(read8,_I)
  CHECK_FUNC_CONST(read16,_I)

  return -1;
}

int memory(JavaClass *java_class, Generator *generator, char *function, const char *const_val)
{
  CHECK_FUNC_STRING(preloadByteArray,_X)
  CHECK_FUNC_STRING(preloadIntArray,_X)

  return -1;
}

