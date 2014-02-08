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

static int spi_init_II(JavaClass *java_class, Generator *generator, int port)
{
  return generator->spi_init(port);
}

static int spi_send_I(JavaClass *java_class, Generator *generator, int port)
{
  return generator->spi_send(port);
}

static int spi_read(JavaClass *java_class, Generator *generator, int port)
{
  return generator->spi_read(port);
}

static int spi_isDataAvailable(JavaClass *java_class, Generator *generator, int port)
{
  return generator->spi_is_data_available(port);
}

static int spi_isBusy(JavaClass *java_class, Generator *generator, int port)
{
  return generator->spi_is_busy(port);
}

static int spi_disable(JavaClass *java_class, Generator *generator, int port)
{
  return generator->spi_disable(port);
}

static int spi_enable(JavaClass *java_class, Generator *generator, int port)
{
  return generator->spi_enable(port);
}

int spi(JavaClass *java_class, Generator *generator, char *function, int port)
{
  CHECK_FUNC(init_II)
  CHECK_FUNC(send_I)
  CHECK_FUNC(read)
  CHECK_FUNC(isDataAvailable)
  CHECK_FUNC(isBusy)
  CHECK_FUNC(disable)
  CHECK_FUNC(enable)

  return -1;
}


