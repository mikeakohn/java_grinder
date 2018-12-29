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

#ifndef _PLAYSTATION_2_API_H
#define _PLAYSTATION_2_API_H

#include "common/JavaClass.h"
#include "generator/Generator.h"

int playstation2(JavaClass *java_class, Generator *generator, const char *method_name);
int playstation2(JavaClass *java_class, Generator *generator, const char *method_name, int const_val);
int playstation2(JavaClass *java_class, Generator *generator, const char *method_name, int const_val1, int const_val2);

#endif

