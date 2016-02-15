/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "JavaClass.h"
#include "spi.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->spi_##funct##sig(port); \
  }

#define CHECK_FUNC_CONST(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->spi_##funct##sig(port, const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct, method_name) == 0) \
  { \
    return generator->spi_##funct##sig(port, const_val1, const_val2); \
  }

int spi(JavaClass *java_class, Generator *generator, char *method_name, int port)
{
  CHECK_FUNC(init,_II)
  CHECK_FUNC(init16,_II)
  CHECK_FUNC(send,_I)
  CHECK_FUNC(send16,_I)
  CHECK_FUNC(read,)
  CHECK_FUNC(isDataAvailable,)
  CHECK_FUNC(isBusy,)
  CHECK_FUNC(disable,)
  CHECK_FUNC(enable,)

  return -1;
}

int spi(JavaClass *java_class, Generator *generator, char *method_name, int port, int const_val)
{
  return -1;
}

int spi(JavaClass *java_class, Generator *generator, char *method_name, int port, int const_val1, int const_val2)
{
  CHECK_FUNC_CONST_2(init,_II)
  CHECK_FUNC_CONST_2(init16,_II)
  return -1;
}

