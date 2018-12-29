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

#include "api/draw3d_object.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->draw3d_object_##funct##sig(); \
  }

#define CHECK_FUNC_CONST_1(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->draw3d_object_##funct##sig(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->draw3d_object_##funct##sig(const_val1, const_val2); \
  }

#define CHECK_FUNC_WITH_TEXTURE(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->draw3d_object_with_texture_##funct##sig(); \
  }

int draw3d_object(JavaClass *java_class, Generator *generator, const char *method_name)
{
  CHECK_FUNC(setContext,_I)
  CHECK_FUNC(rotateX512,_I)
  CHECK_FUNC(rotateY512,_I)
  CHECK_FUNC(rotateZ512,_I)
  CHECK_FUNC(setPosition,_FFF)
  CHECK_FUNC(setPoint,_IFFF)
  CHECK_FUNC(setPointColor,_II)
  CHECK_FUNC(setPoints,_aF)
  CHECK_FUNC(setPointColors,_aI)
  CHECK_FUNC(setTextureCoord,_IFF)
  CHECK_FUNC(setTextureCoords,_aF)
  CHECK_FUNC(disableGouraudShading,)
  CHECK_FUNC(enableGouraudShading,)
  CHECK_FUNC(disableFogging,)
  CHECK_FUNC(enableFogging,)
  CHECK_FUNC(disableTexture,)
  CHECK_FUNC(enableTexture,)
  CHECK_FUNC(disableAlphaBlending,)
  CHECK_FUNC(enableAlphaBlending,)
  CHECK_FUNC(disableAntialiasing,)
  CHECK_FUNC(enableAntialiasing,)
  CHECK_FUNC(draw,)

  return -1;
}

int draw3d_object(JavaClass *java_class, Generator *generator, const char *method_name, int const_val)
{
  return -1;
}

int draw3d_object(JavaClass *java_class, Generator *generator, const char *function, int const_val1, int const_val2)
{
  return -1;
}

int draw3d_object_with_texture(JavaClass *java_class, Generator *generator, const char *method_name)
{
  CHECK_FUNC_WITH_TEXTURE(setPoint,_IFFF)
  CHECK_FUNC_WITH_TEXTURE(setPointColor,_II)
  CHECK_FUNC_WITH_TEXTURE(setPoints,_aF)
  CHECK_FUNC_WITH_TEXTURE(setPointColors,_aI)

  return -1;
}

