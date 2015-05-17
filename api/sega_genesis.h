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

#ifndef _SEGA_GENESIS_H
#define _SEGA_GENESIS_H

#include "Generator.h"
#include "JavaClass.h"

int sega_genesis(JavaClass *java_class, Generator *generator, char *method_name);
int sega_genesis(JavaClass *java_class, Generator *generator, char *method_name, int const_val);
int sega_genesis(JavaClass *java_class, Generator *generator, char *method_name, int const_val1, int const_val2);

#endif

