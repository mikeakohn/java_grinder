/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 */

#ifndef _PLAYSTATION_2_API_H
#define _PLAYSTATION_2_API_H

#include "Generator.h"
#include "JavaClass.h"

int playstation2(JavaClass *java_class, Generator *generator, char *method_name);
int playstation2(JavaClass *java_class, Generator *generator, char *method_name, int const_val);
int playstation2(JavaClass *java_class, Generator *generator, char *method_name, int const_val1, int const_val2);

#endif

