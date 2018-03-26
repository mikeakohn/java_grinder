/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 * Western Design Center SXB board by Joe Davisson.
 *
 */

#ifndef _SXB_H
#define _SXB_H

#include "Generator.h"
#include "JavaClass.h"

int sxb(JavaClass *java_class, Generator *generator, char *method_name);
int sxb(JavaClass *java_class, Generator *generator, char *method_name, int const_val);

#endif

