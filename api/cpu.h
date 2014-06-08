/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014 by Michael Kohn
 *
 */

#ifndef _CPU_H
#define _CPU_H

#include "Generator.h"
#include "JavaClass.h"

int cpu(JavaClass *java_class, Generator *generator, char *method_name);
int cpu(JavaClass *java_class, Generator *generator, char *method_name, int const_val);

#endif

