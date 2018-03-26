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

#ifndef _IOPORT_H
#define _IOPORT_H

#include "Generator.h"
#include "JavaClass.h"

int ioport(JavaClass *java_class, Generator *generator, char *method_name, int port);
int ioport(JavaClass *java_class, Generator *generator, char *method_name, int port, int const_val);

#endif

