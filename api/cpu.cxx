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

#include "JavaClass.h"
#include "cpu.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->cpu_##funct##sig(); \
  }

#define CHECK_FUNC_CONST(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->cpu_##funct##sig(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->cpu_##funct##sig(const_val1, const_val2); \
  }

#define CHECK_FUNC_CONST_STRING(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return cpu_##funct(java_class, generator, const_val); \
  }

static int cpu_asm(JavaClass *java_class, Generator *generator, int const_index)
{
  constant_utf8_t *constant_utf8 = (constant_utf8_t *)java_class->get_constant(const_index);

  return generator->cpu_asm_X((const char *)constant_utf8->bytes, constant_utf8->length);
}

int cpu(JavaClass *java_class, Generator *generator, char *method_name)
{
  CHECK_FUNC(setClock8,)
  CHECK_FUNC(setClock16,)
  CHECK_FUNC(setClock25,)
  CHECK_FUNC(setClockExternal2,)
  CHECK_FUNC(nop,)
  CHECK_FUNC(getCycleCount,)

  return -1;
}

int cpu(JavaClass *java_class, Generator *generator, char *method_name, int const_val)
{
  CHECK_FUNC_CONST_STRING(asm,_X)
  return -1;
}

