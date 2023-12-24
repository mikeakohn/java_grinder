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

#ifndef JAVA_GRINDER_API_ICE_FUN_H
#define JAVA_GRINDER_API_ICE_FUN_H

#include "common/JavaClass.h"
#include "generator/Generator.h"

int ice_fun(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name);

int ice_fun(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name,
  int const_val);

int ice_fun(
  JavaClass *java_class,
  Generator *generator,
  const char *method_name,
  int const_val1,
  int const_val2);

#endif

