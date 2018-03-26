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
#include "ioport.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->ioport_##funct##sig(port); \
  }

#define CHECK_FUNC_CONST(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->ioport_##funct##sig(port, const_val); \
  }

int ioport(JavaClass *java_class, Generator *generator, char *method_name, int port)
{
  CHECK_FUNC(setPinsAsInput,_I)
  CHECK_FUNC(setPinsAsOutput,_I)
  CHECK_FUNC(setPinsValue,_I)
  CHECK_FUNC(setPinsHigh,_I)
  CHECK_FUNC(setPinsLow,_I)
  CHECK_FUNC(setPinAsOutput,_I)
  CHECK_FUNC(setPinAsInput,_I)
  CHECK_FUNC(setPinHigh,_I)
  CHECK_FUNC(setPinLow,_I)
  CHECK_FUNC(isPinInputHigh,_I)
  CHECK_FUNC(getPortInputValue,)
  //CHECK_FUNC(setPortOutputValue,I)
  CHECK_FUNC(setPinsResistorEnable,_I)

  return -1;
}

int ioport(JavaClass *java_class, Generator *generator, char *method_name, int port, int const_val)
{
  CHECK_FUNC_CONST(setPinsAsInput,_I)
  CHECK_FUNC_CONST(setPinsAsOutput,_I)
  CHECK_FUNC_CONST(setPinsValue,_I)
  CHECK_FUNC_CONST(setPinsHigh,_I)
  CHECK_FUNC_CONST(setPinsLow,_I)
  CHECK_FUNC_CONST(setPinAsOutput,_I)
  CHECK_FUNC_CONST(setPinAsInput,_I)
  CHECK_FUNC_CONST(setPinHigh,_I)
  CHECK_FUNC_CONST(setPinLow,_I)
  CHECK_FUNC_CONST(setPinsResistorEnable,_I)
  return -1;
}


