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

#ifndef _TIMER_H
#define _TIMER_H

#include "common/JavaClass.h"
#include "generator/Generator.h"

int timer(JavaClass *java_class, Generator *generator, const char *method_name);
int timer(JavaClass *java_class, Generator *generator, const char *method_name, int const_val);
int timer(JavaClass *java_class, Generator *generator, const char *method_name, int const_val1, int const_val2);

#endif

