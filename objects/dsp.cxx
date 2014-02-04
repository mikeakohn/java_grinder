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

static int dsp_addABAndStoreInA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_add_ab_and_store_in_a();
}

static int dsp_addABAndStoreInB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_add_ab_and_store_in_b();
}

static int dsp_addToA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_add_to_a();
}

static int dsp_addToB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_add_to_b();
}

static int dsp_squareAndAddToA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_square_and_add_to_a();
}

static int dsp_squareAndAddToB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_square_and_add_to_b();
}

static int dsp_mulAndAddToA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_mul_and_add_to_a();
}

static int dsp_mulAndAddToB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_mul_and_add_to_b();
}

static int dsp_mulAndSubFromA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_mul_and_sub_from_a();
}

static int dsp_mulAndSubFromB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_mul_and_sub_from_b();
}

int dsp(JavaClass *java_class, Generator *generator, char *function)
{
  CHECK_FUNC(addABAndStoreInA)
  CHECK_FUNC(addABAndStoreInB)
  CHECK_FUNC(addToA)
  CHECK_FUNC(addToB)
  CHECK_FUNC(squareAndAddToA)
  CHECK_FUNC(squareAndAddToB)
  CHECK_FUNC(mulAndAddToA)
  CHECK_FUNC(mulAndAddToB)
  CHECK_FUNC(mulAndSubFromA)
  CHECK_FUNC(mulAndSubFromB)

  return -1;
}


