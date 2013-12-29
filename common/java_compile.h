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

#include "generator.h"
#include "java_class.h"
#include "java_stack.h"

int java_compile_method(struct java_class_t *java_class, int method_index, struct generator_t *generator, struct java_stack_t *java_stack, int stack_start_ptr);

#endif

