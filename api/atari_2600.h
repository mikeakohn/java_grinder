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

#ifndef _ATARI_2600_H
#define _ATARI_2600_H

#include "Generator.h"
#include "JavaClass.h"

int atari_2600(JavaClass *java_class, Generator *generator, char *method_name);
int atari_2600(JavaClass *java_class, Generator *generator, char *method_name, int const_val);
int atari_2600(JavaClass *java_class, Generator *generator, char *method_name, int const_val1, int const_val2);

#endif

