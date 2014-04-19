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

#ifndef _EXECUTE_STATIC_H
#define _EXECUTE_STATIC_H

#include "Generator.h"
#include "JavaClass.h"

int execute_static(JavaClass *java_class, int method_id, Generator *generator, bool do_arrays, JavaClass *parent_class=NULL);

#endif

