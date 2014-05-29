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
#include "cpu.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->cpu_##funct(); \
  }

int cpu(JavaClass *java_class, Generator *generator, char *function)
{
  CHECK_FUNC(setClock16,)
  CHECK_FUNC(setClock25,)
  CHECK_FUNC(nop,)

  return -1;
}



