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
#include "appleiigs_.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->appleiigs_##funct##sig(); \
  }

#define CHECK_FUNC_CONST_1(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->appleiigs_##funct##sig(const_val); \
  }

int appleiigs(JavaClass *java_class, Generator *generator, char *function)
{
  CHECK_FUNC(plotChar,_IC)
  CHECK_FUNC(printChar,_C)
  CHECK_FUNC(hiresEnable,)
  CHECK_FUNC(hiresPlot,_II)
  CHECK_FUNC(hiresRead,_I)
  return -1;
}

int appleiigs(JavaClass *java_class, Generator *generator, char *function, int const_val)
{
  return -1;
}


