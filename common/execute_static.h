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

#ifndef EXECUTE_STATIC_H
#define EXECUTE_STATIC_H

#include "Generator.h"
#include "JavaClass.h"

int execute_static(JavaClass *java_class, int method_id, Generator *generator, bool do_arrays, bool verbose, JavaClass *parent_class=NULL);

#endif

