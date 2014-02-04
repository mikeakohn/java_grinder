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
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return ioport_##funct(java_class, generator, port); \
  }

static int ioport_setPinsAsInput_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinsAsInput(port);
}

static int ioport_setPinsAsOutput_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinsAsOutput(port);
}

static int ioport_setPinsValue_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinsValue(port);
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

static int ioport_setPinAsInput_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinAsInput(port);
}

static int ioport_setPinHigh_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinHigh(port);
}

static int ioport_setPinLow_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinLow(port);
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

int ioport(JavaClass *java_class, Generator *generator, char *function, int port)
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



