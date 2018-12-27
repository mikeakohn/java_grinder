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

#ifndef CPU_H
#define CPU_H

#include "Generator.h"
#include "JavaClass.h"

int cpu(JavaClass *java_class, Generator *generator, const char *method_name);
int cpu(JavaClass *java_class, Generator *generator, const char *method_name, int const_val);

#endif

