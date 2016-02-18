/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 */
 
/*
 *   MSX support by Giovanni Nunes - https://github.com/plainspooky
 *                  Emiliano Fraga - https://github.com/efraga-msx
 */

#ifndef _MSX_H
#define _MSX_H

#include "Generator.h"
#include "JavaClass.h"

int msx(JavaClass *java_class, Generator *generator, char *method_name);

//int msx(JavaClass *java_class, Generator *generator, char *method_name, char const_val);

int msx(JavaClass *java_class, Generator *generator, char *method_name, int const_val);

int msx(JavaClass *java_class, Generator *generator, char *method_name, int const_val1, int const_val2);

#endif
