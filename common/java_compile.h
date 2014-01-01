/**
 *  Java Grinder
 *  Author: The Naken Crew
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2013 by The Naken Crew
 *
 */

#ifndef _JAVA_COMPILE_H
#define _JAVA_COMPILE_H

#include "Generator.h"
#include "JavaClass.h"
#include "JavaStack.h"

int java_compile_method(JavaClass *class, int method_index, struct generator_t *generator, JavaStack *stack, int stack_start_ptr);

#endif

