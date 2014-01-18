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
    return cpu_##funct(java_class, generator); \
  }

int cpu_setClock16(JavaClass *java_class, Generator *generator)
{
  return generator->cpu_setClock16();
}

int cpu_nop(JavaClass *java_class, Generator *generator)
{
  return generator->cpu_nop();
}

int cpu(JavaClass *java_class, Generator *generator, char *function)
{
  CHECK_FUNC(setClock16)
  CHECK_FUNC(nop)

  return -1;
}



