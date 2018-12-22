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
#include "adc.h"
  //if (strncmp(#funct#sig, function, sizeof(#funct#sig)-1) == 0)
  //if (strncmp(#funct#sig, function, sizeof(#funct#sig)-1) == 0)

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->adc_##funct##sig(); \
  }

#define CHECK_FUNC_CONST_1(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->adc_##funct##sig(const_val); \
  }

#if 0
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
#endif

int adc(JavaClass *java_class, Generator *generator, const char *function)
{
  CHECK_FUNC(enable,)
  CHECK_FUNC(disable,)
  CHECK_FUNC(setChannel,_I)
  CHECK_FUNC(read,)

  return -1;
}

int adc(JavaClass *java_class, Generator *generator, const char *function, int const_val)
{
  CHECK_FUNC_CONST_1(setChannel,_I)
  return -1;
}


