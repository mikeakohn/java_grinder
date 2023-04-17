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

#include "api/cpu.h"

#define CHECK_FUNC(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return grinder_##funct(java_class, generator); \
  }

static int grinder_largeJavaStack(JavaClass *java_class, Generator *generator)
{
  return generator->grinder_largeJavaStack();
}

int grinder(JavaClass *java_class, Generator *generator, const char *function)
{
  CHECK_FUNC(largeJavaStack)

  return -1;
}

