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

#ifndef _DRAW3D_API_H
#define _DRAW3D_API_H

#include "Generator.h"
#include "JavaClass.h"

int draw3d(JavaClass *java_class, Generator *generator, char *method_name);
int draw3d(JavaClass *java_class, Generator *generator, char *method_name, int const_val);
int draw3d(JavaClass *java_class, Generator *generator, char *method_name, int const_val1, int const_val2);

#endif

