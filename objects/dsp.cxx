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
#include "dsp.h"

#define CHECK_FUNC(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return dsp_##funct(java_class, generator); \
  }

int dsp_addABAndStoreInA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_add_ab_and_store_in_a();
}

int dsp_addABAndStoreInB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_add_ab_and_store_in_b();
}

int dsp(JavaClass *java_class, Generator *generator, char *function)
{
  CHECK_FUNC(addABAndStoreInA)
  CHECK_FUNC(addABAndStoreInB)

  return -1;
}


