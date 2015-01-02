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
#include "JavaCompiler.h"
#include "java_lang_string.h"

#define CHECK_FUNC(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return string_##funct(java_class, generator, field_name, field_id); \
  }

static int string_length(JavaClass *java_class, Generator *generator, char *field_name, int field_id)
{
  //generator->push_array_length(field_name, field_id);
  generator->push_array_length();
  return 0;
}

static int string_charAt_I(JavaClass *java_class, Generator *generator, char *field_name, int field_id)
{
  //generator->array_read_byte(field_name, field_id);
  generator->array_read_byte();
  return 0;
}

int java_lang_string(JavaClass *java_class, Generator *generator, char *function, char *field_name, int field_id)
{
  CHECK_FUNC(length)
  CHECK_FUNC(charAt_I)

  return -1;
}

