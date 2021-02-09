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

#ifndef JAVA_GRINDER_API_INTELLIVISION_H
#define JAVA_GRINDER_API_INTELLIVISION_H

#include "common/JavaClass.h"
#include "generator/Generator.h"

int intellivision(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name);

int intellivision(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name,
  int const_val);

int intellivision(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name,
  int const_val1,
  int const_val2);

#endif

