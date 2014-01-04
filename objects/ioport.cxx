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
  return -1;
}

int ioport_setPinsAsOutput_I(JavaClass *java_class, Generator *generator, int port)
{
  return -1;
}

int ioport_setPinsValue_I(JavaClass *java_class, Generator *generator, int port)
{
  return -1;
}

int ioport_setPinsHight_I(JavaClass *java_class, Generator *generator, int port)
{
  return -1;
}

int ioport_setPinsLow_I(JavaClass *java_class, Generator *generator, int port)
{
  return -1;
}

int ioport_setPinAsOutput_I(JavaClass *java_class, Generator *generator, int port)
{
  return -1;
}

int ioport_setPinAsInput_I(JavaClass *java_class, Generator *generator, int port)
{
  return -1;
}

int ioport_setPinHigh_I(JavaClass *java_class, Generator *generator, int port)
{
  return -1;
}

int ioport_setPinLow_I(JavaClass *java_class, Generator *generator, int port)
{
  return -1;
}

int ioport_isPinInputHigh_I(JavaClass *java_class, Generator *generator, int port)
{
  return -1;
}

int ioport_getPortOuputValue(JavaClass *java_class, Generator *generator, int port)
{
  return -1;
}

int ioport_setPortOuputValue_I(JavaClass *java_class, Generator *generator, int port)
{
  return -1;
}

int ioport(JavaClass *java_class, Generator *generator, char *function, int port)
{
  CHECK_FUNC(setPinsAsInput_I)
  CHECK_FUNC(setPinsAsOutput_I)
  CHECK_FUNC(setPinsValue_I)
  CHECK_FUNC(setPinsHight_I)
  CHECK_FUNC(setPinsLow_I)
  CHECK_FUNC(setPinAsOutput_I)
  CHECK_FUNC(setPinAsInput_I)
  CHECK_FUNC(setPinHigh_I)
  CHECK_FUNC(setPinLow_I)
  CHECK_FUNC(isPinInputHigh_I)
  CHECK_FUNC(getPortOuputValue)
  CHECK_FUNC(setPortOuputValue_I)

  return -1;
}



