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
#include "adc.h"

#define CHECK_FUNC(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return adc_##funct(java_class, generator); \
  }

#define CHECK_FUNC_CONST_1(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return adc_##funct(java_class, generator, const_val); \
  }

static int adc_enable(JavaClass *java_class, Generator *generator)
{
  return generator->adc_enable();
}

static int adc_disable(JavaClass *java_class, Generator *generator)
{
  return generator->adc_disable();
}

static int adc_setChannel_I(JavaClass *java_class, Generator *generator)
{
  return generator->adc_setChannel_I();
}

static int adc_setChannel_I(JavaClass *java_class, Generator *generator, int channel)
{
  return generator->adc_setChannel_I(channel);
}

static int adc_read(JavaClass *java_class, Generator *generator)
{
  return generator->adc_read();
}

int adc(JavaClass *java_class, Generator *generator, char *function)
{
  CHECK_FUNC(enable)
  CHECK_FUNC(disable)
  CHECK_FUNC(setChannel_I)
  CHECK_FUNC(read)

  return -1;
}

int adc(JavaClass *java_class, Generator *generator, char *function, int const_val)
{
  CHECK_FUNC_CONST_1(setChannel_I)
  return -1;
}


