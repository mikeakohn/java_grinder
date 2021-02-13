/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2021 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_API_JAVA_LANG_STRING_H
#define JAVA_GRINDER_API_JAVA_LANG_STRING_H

#include "common/JavaClass.h"
#include "generator/Generator.h"

int java_lang_string(
  JavaClass *java_class,
  Generator *generator,
  const char *function,
  const char *field_name,
  int field_id);

int java_lang_string(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name);

#endif

