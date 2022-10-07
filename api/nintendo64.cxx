/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2022 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "api/nes.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->nintendo64_##funct##sig(); \
  }

#define CHECK_FUNC_CONST_1(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->nintendo64_##funct##sig(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->nintendo64_##funct##sig(const_val1, const_val2); \
  }

int nintendo64(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name)
{
  CHECK_FUNC(waitVsync,)
  CHECK_FUNC(clearScreen,)
  CHECK_FUNC(resetZBuffer,)
  CHECK_FUNC(setScreen,_I)
  CHECK_FUNC(plot,_III)
  CHECK_FUNC(waitForPolygon,)

  CHECK_FUNC(n64_triangle_Constructor,)
  CHECK_FUNC(n64_triangle_setColor,_I)
  CHECK_FUNC(n64_triangle_setPosition,_III)
  CHECK_FUNC(n64_triangle_setRotation,_III)
  CHECK_FUNC(n64_triangle_setVertex0,_III)
  CHECK_FUNC(n64_triangle_setVertex1,_III)
  CHECK_FUNC(n64_triangle_setVertex2,_III)
  CHECK_FUNC(n64_triangle_setVertexes,_bS)
  CHECK_FUNC(n64_triangle_setZBuffer,_Z)
  CHECK_FUNC(n64_triangle_draw,)

  CHECK_FUNC(n64_rectangle_Constructor,)
  CHECK_FUNC(n64_rectangle_setColor,_I)
  CHECK_FUNC(n64_rectangle_setPosition,_II)
  CHECK_FUNC(n64_rectangle_setSize,_II)
  CHECK_FUNC(n64_rectangle_setTexture,_aBII)
  CHECK_FUNC(n64_rectangle_draw,)

  return -1;
}

int nintendo64(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name,
  int const_val)
{
  return -1;
}

int nintendo64(
  JavaClass *java_class,
  Generator *generator,
  const char *function,
  int const_val1,
  int const_val2)
{
  return -1;
}

