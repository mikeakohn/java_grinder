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

#define CHECK_FUNC_TRIANGLE(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->nintendo64_n64_triangle_##funct##sig(); \
  }

#define CHECK_FUNC_RECTANGLE(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->nintendo64_n64_rectangle_##funct##sig(); \
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
  CHECK_FUNC(plot,_II)
  CHECK_FUNC(plot,_III)
  CHECK_FUNC(loadTexture,_aSII)
  CHECK_FUNC(waitForPolygon,)
  CHECK_FUNC(getAudioStatus,)
  CHECK_FUNC(setAudioDACRate,_I)
  CHECK_FUNC(setAudioBitRate,_I)
  CHECK_FUNC(playAudio,_aSI)

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

int nintendo64_n64_triangle(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name)
{
  CHECK_FUNC_TRIANGLE(Constructor,)
  CHECK_FUNC_TRIANGLE(setColor,_I)
  CHECK_FUNC_TRIANGLE(setPosition,_III)
  CHECK_FUNC_TRIANGLE(setRotation,_III)
  CHECK_FUNC_TRIANGLE(setVertex0,_III)
  CHECK_FUNC_TRIANGLE(setVertex1,_III)
  CHECK_FUNC_TRIANGLE(setVertex2,_III)
  CHECK_FUNC_TRIANGLE(setVertexes,_aS)
  CHECK_FUNC_TRIANGLE(setVertexes,_aSI)
  CHECK_FUNC_TRIANGLE(setZBuffer,_Z)
  CHECK_FUNC_TRIANGLE(draw,)

  return -1;
}

int nintendo64_n64_rectangle(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name)
{
  CHECK_FUNC_RECTANGLE(Constructor,)
  CHECK_FUNC_RECTANGLE(setColor,_I)
  CHECK_FUNC_RECTANGLE(setPosition,_II)
  CHECK_FUNC_RECTANGLE(setSize,_II)
  CHECK_FUNC_RECTANGLE(setTextureEnabled,_II)
  CHECK_FUNC_RECTANGLE(setTextureDisabled,)
  CHECK_FUNC_RECTANGLE(draw,)

  return -1;
}

