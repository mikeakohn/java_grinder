/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2023 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_API_NINTENDO64_API_H
#define JAVA_GRINDER_API_NINTENDO64_API_H

#include "common/JavaClass.h"
#include "generator/Generator.h"

int nintendo64(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name);

int nintendo64(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name,
  int const_val);

int nintendo64(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name,
  int const_val1,
  int const_val2);

int nintendo64_n64_triangle(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name);

int nintendo64_n64_rectangle(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name);

#endif

