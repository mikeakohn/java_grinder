/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

#ifndef _W65C265SXB_H
#define _W65C265SXB_H

#include "Generator.h"
#include "JavaClass.h"

int w65c265sxb(JavaClass *java_class, Generator *generator, char *method_name);
int w65c265sxb(JavaClass *java_class, Generator *generator, char *method_name, int const_val);

#endif

