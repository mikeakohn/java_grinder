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

#ifndef _TI99_H
#define _TI99_H

#include "Generator.h"
#include "JavaClass.h"

int ti99(JavaClass *java_class, Generator *generator, char *method_name);
int ti99(JavaClass *java_class, Generator *generator, char *method_name, int const_val);
int ti99(JavaClass *java_class, Generator *generator, char *method_name, int const_val1, int const_val2);

#endif

