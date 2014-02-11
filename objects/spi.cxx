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

#define CHECK_FUNC_CONST_2(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return spi_##funct(java_class, generator, port, const_val1, const_val2); \
  }

static int spi_init_II(JavaClass *java_class, Generator *generator, int port)
{
  return generator->spi_init(port);
}

static int spi_init_II(JavaClass *java_class, Generator *generator, int port, int const_val1, int const_val2)
{
  return generator->spi_init(port, const_val1, const_val2);
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
  return generator->spi_isDataAvailable(port);
}

static int spi_isBusy(JavaClass *java_class, Generator *generator, int port)
{
  return generator->spi_isBusy(port);
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

int spi(JavaClass *java_class, Generator *generator, char *function, int port, int const_val)
{
  return -1;
}

int spi(JavaClass *java_class, Generator *generator, char *function, int port, int const_val1, int const_val2)
{
  CHECK_FUNC_CONST_2(init_II)
  return -1;
}

