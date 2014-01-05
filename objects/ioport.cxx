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

int ioport_setPinsAsInput_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinsAsInput();
}

int ioport_setPinsAsOutput_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinsAsOutput();
}

int ioport_setPinsValue_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinsValue();
}

int ioport_setPinsHigh_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinsHigh();
}

int ioport_setPinsLow_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinsLow();
}

int ioport_setPinAsOutput_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinAsOutput();
}

int ioport_setPinAsInput_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinAsInput();
}

int ioport_setPinHigh_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinHigh();
}

int ioport_setPinLow_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPinLow();
}

int ioport_isPinInputHigh_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_isPinInputHigh();
}

int ioport_getPortInputValue(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_getPortInputValue();
}

int ioport_setPortOutputValue_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->ioport_setPortOutputValue();
}

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
  CHECK_FUNC(setPortOutputValue_I)

  return -1;
}



