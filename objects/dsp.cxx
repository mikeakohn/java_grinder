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

static int dsp_clearA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_clear_a();
}

static int dsp_clearB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_clear_b();
}

static int dsp_loadA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_load_a();
}

static int dsp_loadB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_load_b();
}

static int dsp_negA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_neg_a();
}

static int dsp_negB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_neg_b();
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

static int dsp_squareToA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_square_to_a();
}

static int dsp_squareToB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_square_to_b();
}

static int dsp_mulToA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_mul_to_a();
}

static int dsp_mulToB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_mul_to_b();
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
  CHECK_FUNC(clearA)
  CHECK_FUNC(clearB)
  CHECK_FUNC(loadA)
  CHECK_FUNC(loadB)
  CHECK_FUNC(negA)
  CHECK_FUNC(negB)
  CHECK_FUNC(addABAndStoreInA)
  CHECK_FUNC(addABAndStoreInB)
  CHECK_FUNC(addToA)
  CHECK_FUNC(addToB)
  CHECK_FUNC(squareToA)
  CHECK_FUNC(squareToB)
  CHECK_FUNC(mulToA)
  CHECK_FUNC(mulToB)
  CHECK_FUNC(squareAndAddToA)
  CHECK_FUNC(squareAndAddToB)
  CHECK_FUNC(mulAndAddToA)
  CHECK_FUNC(mulAndAddToB)
  CHECK_FUNC(mulAndSubFromA)
  CHECK_FUNC(mulAndSubFromB)

  return -1;
}


