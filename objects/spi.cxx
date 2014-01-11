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
#include "spi.h"

#define CHECK_FUNC(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return spi_##funct(java_class, generator, port); \
  }

int spi_init_III(JavaClass *java_class, Generator *generator, int port)
{
  return generator->spi_init(port);
}

int spi_send_B(JavaClass *java_class, Generator *generator, int port)
{
  return generator->spi_send(port);
}

int spi_read(JavaClass *java_class, Generator *generator, int port)
{
  return generator->spi_read(port);
}

int spi_isDataAvailable(JavaClass *java_class, Generator *generator, int port)
{
  return generator->spi_isDataAvailable(port);
}

int spi_isSendReady(JavaClass *java_class, Generator *generator, int port)
{
  return generator->spi_isSendReady(port);
}

int spi(JavaClass *java_class, Generator *generator, char *function, int port)
{
  CHECK_FUNC(init_III)
  CHECK_FUNC(send_B)
  CHECK_FUNC(read)
  CHECK_FUNC(isDataAvailable)
  CHECK_FUNC(isSendReady)

  return -1;
}



