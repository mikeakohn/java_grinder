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

#include "api/dsp.h"

#define CHECK_FUNC(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return dsp_##funct(java_class, generator); \
  }

static int dsp_getA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_getA();
}

static int dsp_getB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_getB();
}

static int dsp_getUpperA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_getUpperA();
}

static int dsp_getUpperB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_getUpperB();
}

static int dsp_getLowerA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_getLowerA();
}

static int dsp_getLowerB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_getLowerB();
}

static int dsp_getRoundedA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_getRoundedA();
}

static int dsp_getRoundedB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_getRoundedB();
}

static int dsp_clearA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_clearA();
}

static int dsp_clearB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_clearB();
}

static int dsp_loadA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_loadA();
}

static int dsp_loadB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_loadB();
}

static int dsp_negA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_negA();
}

static int dsp_negB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_negB();
}

static int dsp_addABAndStoreInA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_addABAndStoreInA();
}

static int dsp_addABAndStoreInB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_addABAndStoreInB();
}

static int dsp_subABAndStoreInA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_subABAndStoreInA();
}

static int dsp_subBAAndStoreInB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_subBAAndStoreInB();
}

static int dsp_addToA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_addToA();
}

static int dsp_addToB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_addToB();
}

static int dsp_squareToA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_squareToA();
}

static int dsp_squareToB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_squareToB();
}

static int dsp_mulToA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_mulToA();
}

static int dsp_mulToB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_mulToB();
}

#if 0
static int dsp_euclideanDistanceToA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_mulToA();
}

static int dsp_euclideanDistanceToB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_mulToB();
}
#endif

static int dsp_squareAndAddToA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_squareAndAddToA();
}

static int dsp_squareAndAddToB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_squareAndAddToB();
}

static int dsp_mulAndAddToA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_mulAndAddToA();
}

static int dsp_mulAndAddToB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_mulAndAddToB();
}

static int dsp_mulAndSubFromA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_mulAndSubFromA();
}

static int dsp_mulAndSubFromB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_mulAndSubFromB();
}

#if 0
static int dsp_euclideanDistanceAndAddToA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_euclideanDistanceAndAddToA();
}

static int dsp_euclideanDistanceAndAddToB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_euclideanDistanceAndAddToB();
}
#endif

static int dsp_shiftA(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_shiftA();
}

static int dsp_shiftB(JavaClass *java_class, Generator *generator)
{
  return generator->dsp_shiftB();
}

int dsp(JavaClass *java_class, Generator *generator, const char *function)
{
  CHECK_FUNC(getA)
  CHECK_FUNC(getB)
  CHECK_FUNC(getUpperA)
  CHECK_FUNC(getUpperB)
  CHECK_FUNC(getLowerA)
  CHECK_FUNC(getLowerB)
  CHECK_FUNC(getRoundedA)
  CHECK_FUNC(getRoundedB)
  CHECK_FUNC(clearA)
  CHECK_FUNC(clearB)
  CHECK_FUNC(loadA)
  CHECK_FUNC(loadB)
  CHECK_FUNC(negA)
  CHECK_FUNC(negB)
  CHECK_FUNC(addABAndStoreInA)
  CHECK_FUNC(addABAndStoreInB)
  CHECK_FUNC(subABAndStoreInA)
  CHECK_FUNC(subBAAndStoreInB)
  CHECK_FUNC(addToA)
  CHECK_FUNC(addToB)
  CHECK_FUNC(squareToA)
  CHECK_FUNC(squareToB)
  CHECK_FUNC(mulToA)
  CHECK_FUNC(mulToB)
  //CHECK_FUNC(euclideanDistanceToA)
  //CHECK_FUNC(euclideanDistanceToB)
  CHECK_FUNC(squareAndAddToA)
  CHECK_FUNC(squareAndAddToB)
  CHECK_FUNC(mulAndAddToA)
  CHECK_FUNC(mulAndAddToB)
  CHECK_FUNC(mulAndSubFromA)
  CHECK_FUNC(mulAndSubFromB)
  //CHECK_FUNC(euclideanDistanceAndAddToA)
  //CHECK_FUNC(euclideanDistanceAndAddToB)
  CHECK_FUNC(shiftA)
  CHECK_FUNC(shiftB)

  return -1;
}


