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

#ifndef MEMORY_H
#define MEMORY_H

#include "Generator.h"
#include "JavaClass.h"

int memory(JavaClass *java_class, Generator *generator, const char *method_name);
int memory(JavaClass *java_class, Generator *generator, const char *method_name, int const_val);
int memory(JavaClass *java_class, Generator *generator, const char *function, const char *const_val);

#endif

