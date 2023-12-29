/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2023 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_API_INVOKE_STATIC_H
#define JAVA_GRINDER_API_INVOKE_STATIC_H

#include "common/JavaClass.h"
#include "generator/Generator.h"

int invoke_static(
  JavaClass *java_class,
  int method_id,
  Generator *generator);

int invoke_static(
  JavaClass *java_class,
  int method_id,
  Generator *generator,
  int *const_vals,
  int const_count);

int invoke_static(
  JavaClass *java_class,
  int method_id,
  Generator *generator,
  const char *const_val);

#endif

