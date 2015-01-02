/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

#ifndef _TIMER_H
#define _TIMER_H

#include "Generator.h"
#include "JavaClass.h"

int timer(JavaClass *java_class, Generator *generator, char *method_name);
int timer(JavaClass *java_class, Generator *generator, char *method_name, int const_val);
int timer(JavaClass *java_class, Generator *generator, char *method_name, int const_val1, int const_val2);

#endif

