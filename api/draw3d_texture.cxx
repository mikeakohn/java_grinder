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
#include "draw3d_texture.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->draw3d_texture_##funct##sig(); \
  }

#define CHECK_FUNC_CONST_1(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->draw3d_texture_##funct##sig(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->draw3d_texture_##funct##sig(const_val1, const_val2); \
  }

#define CHECK_FUNC_16(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->draw3d_texture16_##funct##sig(); \
  }

#define CHECK_FUNC_24(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->draw3d_texture24_##funct##sig(); \
  }

#define CHECK_FUNC_32(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->draw3d_texture32_##funct##sig(); \
  }

int draw3d_texture(JavaClass *java_class, Generator *generator, const char *method_name)
{
  CHECK_FUNC(enableTransparency,)
  CHECK_FUNC(enableTransparencyOnBlack,)
  CHECK_FUNC(disableTransparency,)
  CHECK_FUNC(upload,)

  return -1;
}

int draw3d_texture(JavaClass *java_class, Generator *generator, const char *method_name, int const_val)
{
  return -1;
}

int draw3d_texture(JavaClass *java_class, Generator *generator, const char *function, int const_val1, int const_val2)
{
  return -1;
}

int draw3d_texture16(JavaClass *java_class, Generator *generator, const char *method_name)
{
  CHECK_FUNC_16(setPixel,_II)
  CHECK_FUNC_16(setPixels,_IaS)
  CHECK_FUNC_16(setPixelsRLE16,_IaB)

  return -1;
}

int draw3d_texture24(JavaClass *java_class, Generator *generator, const char *method_name)
{
  CHECK_FUNC_24(setPixel,_II)
  CHECK_FUNC_24(setPixels,_IaI)

  return -1;
}

int draw3d_texture32(JavaClass *java_class, Generator *generator, const char *method_name)
{
  CHECK_FUNC_32(setPixel,_II)
  CHECK_FUNC_32(setPixels,_IaI)

  return -1;
}

