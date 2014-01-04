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

#ifndef _INVOKE_H
#define _INVOKE_H

#include "Generator.h"
#include "JavaClass.h"

int invoke_virtual(JavaClass *java_class, int method_id, int field_id, Generator *generator);
int invoke_static(JavaClass *java_class, int method_id, Generator *generator);

#endif

