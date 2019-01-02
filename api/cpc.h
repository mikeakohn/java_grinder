/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 * CPC support started by Carsten Dost  - https://github.com/deringenieur71
 *
 */
 
#ifndef JAVA_GRINDER_API_CPC_H
#define JAVA_GRINDER_API_CPC_H

#include "common/JavaClass.h"
#include "generator/Generator.h"

int cpc(JavaClass *java_class, Generator *generator, const char *method_name);
int cpc(JavaClass *java_class, Generator *generator, const char *method_name, int const_val);
int cpc(JavaClass *java_class, Generator *generator, const char *method_name, int const_val1, int const_val2);

#endif

