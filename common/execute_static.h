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

#ifndef JAVA_GRINDER_COMMON_EXECUTE_STATIC_H
#define JAVA_GRINDER_COMMON_EXECUTE_STATIC_H

#include "common/JavaClass.h"
#include "generator/Generator.h"

int execute_static(JavaClass *java_class, int method_id, Generator *generator, bool do_arrays, bool verbose, JavaClass *parent_class=NULL);

#endif

