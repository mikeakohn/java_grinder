/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 */

#ifndef _PROPELLER_H
#define _PROPELLER_H

#include "Generator.h"
#include "JavaClass.h"

int propeller(JavaClass *java_class, Generator *generator, char *method_name);
int propeller(JavaClass *java_class, Generator *generator, char *method_name, int const_val);
int propeller(JavaClass *java_class, Generator *generator, char *method_name, int const_val1, int const_val2);

#endif

