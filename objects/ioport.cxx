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
#include "compile.h"
#include "ioport.h"

#define CHECK_FUNC(funct) \
  if (strncmp(#funct, method_name, sizeof(#funct)-1) == 0) \
  { \
    return ioport_##funct(java_class, generator, port); \
  }

#define CHECK_FUNC_CONST(funct) \
  if (strncmp(#funct, method_name, sizeof(#funct)-1) == 0) \
  { \
    return ioport_##funct(java_class, generator, port, const_val); \
  }

static int ioport_setPinsAsInput_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinsAsInput(port);
}

static int ioport_setPinsAsOutput_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinsAsOutput(port);
}

static int ioport_setPinsAsOutput_I(JavaClass *java_class, Generator *generator, int port, int const_val)
{
  return generator->ioport_setPinsAsOutput(port, const_val);
}

static int ioport_setPinsValue_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinsValue(port);
}

static int ioport_setPinsValue_I(JavaClass *java_class, Generator *generator, int port, int const_val)
{
  return generator->ioport_setPinsValue(port, const_val);
}

static int ioport_setPinsHigh_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinsHigh(port);
}

static int ioport_setPinsLow_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinsLow(port);
}

static int ioport_setPinAsOutput_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinAsOutput(port);
}

static int ioport_setPinAsOutput_I(JavaClass *java_class, Generator *generator, int port, int const_val)
{
  return generator->ioport_setPinAsOutput(port, const_val);
}

static int ioport_setPinAsInput_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinAsInput(port);
}

static int ioport_setPinAsInput_I(JavaClass *java_class, Generator *generator, int port, int const_val)
{
  return generator->ioport_setPinAsInput(port, const_val);
}

static int ioport_setPinHigh_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinHigh(port);
}

static int ioport_setPinHigh_I(JavaClass *java_class, Generator *generator, int port, int const_val)
{
  return generator->ioport_setPinHigh(port, const_val);
}

static int ioport_setPinLow_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinLow(port);
}

static int ioport_setPinLow_I(JavaClass *java_class, Generator *generator, int port, int const_val)
{
  return generator->ioport_setPinLow(port, const_val);
}

static int ioport_isPinInputHigh_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_isPinInputHigh(port);
}

static int ioport_getPortInputValue(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_getPortInputValue(port);
}

#if 0
static int ioport_setPortOutputValue_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPortOutputValue(port);
}
#endif

int ioport(JavaClass *java_class, Generator *generator, char *method_name, int port)
{
  CHECK_FUNC(setPinsAsInput_I)
  CHECK_FUNC(setPinsAsOutput_I)
  CHECK_FUNC(setPinsValue_I)
  CHECK_FUNC(setPinsHigh_I)
  CHECK_FUNC(setPinsLow_I)
  CHECK_FUNC(setPinAsOutput_I)
  CHECK_FUNC(setPinAsInput_I)
  CHECK_FUNC(setPinHigh_I)
  CHECK_FUNC(setPinLow_I)
  CHECK_FUNC(isPinInputHigh_I)
  CHECK_FUNC(getPortInputValue)
  //CHECK_FUNC(setPortOutputValue_I)

  return -1;
}

int ioport(JavaClass *java_class, Generator *generator, char *method_name, int port, int const_val)
{
  CHECK_FUNC_CONST(setPinsAsOutput_I)
  CHECK_FUNC_CONST(setPinsValue_I)
  CHECK_FUNC_CONST(setPinAsOutput_I)
  CHECK_FUNC_CONST(setPinAsInput_I)
  CHECK_FUNC_CONST(setPinHigh_I)
  CHECK_FUNC_CONST(setPinLow_I)
  return -1;
}


