/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2017 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "JavaClass.h"
#include "parallella.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->parallella_##funct##sig(); \
  }

#define CHECK_FUNC_CONST(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->parallella_##funct##sig(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->parallella_##funct##sig(const_val1, const_val2); \
  }

int parallella(JavaClass *java_class, Generator *generator, char *method_name)
{
  CHECK_FUNC(writeSharedRamByte,_IB)
  CHECK_FUNC(writeSharedRamShort,_IS)
  CHECK_FUNC(writeSharedRamInt,_II)
  CHECK_FUNC(writeSharedRamFloat,_IF)
  CHECK_FUNC(readSharedRamByte,_I)
  CHECK_FUNC(readSharedRamShort,_I)
  CHECK_FUNC(readSharedRamInt,_I)
  CHECK_FUNC(readSharedRamFloat,_I)
  CHECK_FUNC(getCoreId,)
  CHECK_FUNC(setUserInterruptListener,_Z)

  return -1;
}

int parallella(JavaClass *java_class, Generator *generator, char *method_name, int const_val)
{
  //CHECK_FUNC_CONST(readSharedRamByte,_I)
  //CHECK_FUNC_CONST(readSharedRamShort,_I)
  //CHECK_FUNC_CONST(readSharedRamInt,_I)
  CHECK_FUNC_CONST(setUserInterruptListener,_Z)

  return -1;
}

int parallella(JavaClass *java_class, Generator *generator, char *function, int const_val1, int const_val2)
{
  return -1;
}

