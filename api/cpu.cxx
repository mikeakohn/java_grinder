/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
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

#define CHECK_FUNC_CONST_1(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return cpu_##funct(java_class, generator, const_val); \
  }

static int cpu_asm(JavaClass *java_class, Generator *generator, int const_index)
{
  constant_utf8_t *constant_utf8 = (constant_utf8_t *)java_class->get_constant(const_index);

  return generator->cpu_asm((const char *)constant_utf8->bytes, constant_utf8->length);
}

int cpu(JavaClass *java_class, Generator *generator, char *function)
{
  CHECK_FUNC(setClock16,)
  CHECK_FUNC(setClock25,)
  CHECK_FUNC(setClockExternal2,)
  CHECK_FUNC(nop,)

  return -1;
}

int cpu(JavaClass *java_class, Generator *generator, char *function, int const_val)
{
  CHECK_FUNC_CONST_1(asm,_Ljava/lang/String;)
  return -1;
}



