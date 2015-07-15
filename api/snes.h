/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 * SNES by Joe Davisson
 *
 */

#ifndef _SNES_API_H
#define _SNES_API_H

#include "Generator.h"
#include "JavaClass.h"

int snes(JavaClass *java_class, Generator *generator, char *method_name);
int snes(JavaClass *java_class, Generator *generator, char *method_name, int const_val);
int snes(JavaClass *java_class, Generator *generator, char *method_name, int const_val1, int const_val2);

#endif

