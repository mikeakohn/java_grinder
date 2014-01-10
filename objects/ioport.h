/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014 by Michael Kohn
 *
 */

#ifndef _IOPORT_H
#define _IOPORT_H

#include "Generator.h"
#include "JavaClass.h"

int ioport(JavaClass *java_class, Generator *generator, char *method_name, int port);

#endif

