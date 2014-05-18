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
#include "timer.h"

#define CHECK_FUNC(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return timer_##funct(java_class, generator); \
  }

#define CHECK_FUNC_CONST_1(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return timer_##funct(java_class, generator, const_val); \
  }

#define CHECK_FUNC_CONST_2(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return timer_##funct(java_class, generator, const_val1, const_val2); \
  }

static int timer_setInterval_II(JavaClass *java_class, Generator *generator)
{
  return generator->timer_setInterval();
}

static int timer_setInterval_II(JavaClass *java_class, Generator *generator, int const_val1, int const_val2)
{
  return generator->timer_setInterval(const_val1, const_val2);
}

static int timer_setListener_Z(JavaClass *java_class, Generator *generator)
{
  return generator->timer_setListener();
}

static int timer_setListener_Z(JavaClass *java_class, Generator *generator, int const_val)
{
  return generator->timer_setListener(const_val);
}

static int timer_getValue(JavaClass *java_class, Generator *generator)
{
  return generator->timer_getValue();
}

static int timer_setValue_I(JavaClass *java_class, Generator *generator)
{
  return generator->timer_setValue();
}

static int timer_setValue_I(JavaClass *java_class, Generator *generator, int const_val)
{
  return generator->timer_setValue(const_val);
}

int timer(JavaClass *java_class, Generator *generator, char *function)
{
  CHECK_FUNC(setInterval_II)
  CHECK_FUNC(setListener_Z)
  CHECK_FUNC(getValue)
  CHECK_FUNC(setValue_I)

  return -1;
}

int timer(JavaClass *java_class, Generator *generator, char *function, int const_val)
{
  CHECK_FUNC_CONST_1(setListener_Z)
  CHECK_FUNC_CONST_1(setValue_I)
  return -1;
}

int timer(JavaClass *java_class, Generator *generator, char *function, int const_val1, int const_val2)
{
  CHECK_FUNC_CONST_2(setInterval_II)
  return -1;
}



