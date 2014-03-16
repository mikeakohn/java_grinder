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
#include "memory.h"

#define CHECK_FUNC(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return memory_##funct(java_class, generator); \
  }

static int memory_read8_I(JavaClass *java_class, Generator *generator)
{
  return generator->memory_read8();
}

static int memory_write8_IB(JavaClass *java_class, Generator *generator)
{
  return generator->memory_write8();
}

static int memory_read16_I(JavaClass *java_class, Generator *generator)
{
  return generator->memory_read16();
}

static int memory_write16_IS(JavaClass *java_class, Generator *generator)
{
  return generator->memory_write16();
}

int memory(JavaClass *java_class, Generator *generator, char *function)
{
  CHECK_FUNC(read8_I)
  CHECK_FUNC(write8_IB)
  CHECK_FUNC(read16_I)
  CHECK_FUNC(write16_IS)

  return -1;
}

int memory(JavaClass *java_class, Generator *generator, char *function, int const_val)
{
  return -1;
}



