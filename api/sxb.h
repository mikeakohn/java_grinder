/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 * Western Design Center SXB board by Joe Davisson.
 *
 */

#ifndef JAVA_GRINDER_API_SXB_H
#define JAVA_GRINDER_API_SXB_H

#include "common/JavaClass.h"
#include "generator/Generator.h"

int sxb(JavaClass *java_class, Generator *generator, const char *method_name);
int sxb(JavaClass *java_class, Generator *generator, const char *method_name, int const_val);

#endif

