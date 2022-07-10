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

#ifndef JAVA_GRINDER_API_INVOKE_VIRTUAL_H
#define JAVA_GRINDER_API_INVOKE_VIRTUAL_H

#include "common/JavaClass.h"
#include "generator/Generator.h"

int invoke_virtual(
  JavaClass *java_class,
  int method_id,
  int field_id,
  Generator *generator);

int invoke_virtual(
  JavaClass *java_class,
  int method_id,
  Generator *generator);

#endif

