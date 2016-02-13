/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "JavaClass.h"
#include "JavaCompiler.h"
#include "java_lang_string.h"

// FIXME: This should be changed to just the "pushed" versions

#define CHECK_FUNC(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return string_##funct(java_class, generator); \
  }

#define CHECK_FUNC_PUSHED(funct,sig,gen_function) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->gen_function(); \
  }

static int string_length(JavaClass *java_class, Generator *generator)
{
  generator->push_array_length();
  return 0;
}

static int string_charAt_I(JavaClass *java_class, Generator *generator)
{
  generator->array_read_byte();
  return 0;
}

int java_lang_string(JavaClass *java_class, Generator *generator, char *function, char *field_name, int field_id)
{
  CHECK_FUNC(length)
  CHECK_FUNC(charAt_I)

  return -1;
}

int java_lang_string(JavaClass *java_class, Generator *generator, char *method_name)
{
  CHECK_FUNC_PUSHED(length,,push_array_length)
  CHECK_FUNC_PUSHED(charAt,_I,array_read_byte)

  return -1;
}


