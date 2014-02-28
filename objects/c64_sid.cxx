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
#include "cpu.h"

#define CHECK_FUNC(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return c64_sid_##funct(java_class, generator); \
  }

static int c64_sid_setVolume(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_setVolume();
}

int c64_sid(JavaClass *java_class, Generator *generator, char *function)
{
  CHECK_FUNC(setVolume)

  return -1;
}



