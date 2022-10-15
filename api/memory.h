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

#ifndef JAVA_GRINDER_API_MEMORY_H
#define JAVA_GRINDER_API_MEMORY_H

#include "common/JavaClass.h"
#include "generator/Generator.h"

int memory(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name);

int memory(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name,
  int const_val);

int memory(
  JavaClass *java_class,
  Generator *generator,
  const char *function,
  const char *const_val);

#endif

