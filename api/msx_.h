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
 
/*
 *   MSX support by Giovanni Nunes - https://github.com/plainspooky
 *                  Emiliano Fraga - https://github.com/efraga-msx
 */

#ifndef MSX_H
#define MSX_H

#include "Generator.h"
#include "JavaClass.h"

int msx(JavaClass *java_class, Generator *generator, const char *method_name);

//int msx(JavaClass *java_class, Generator *generator, const char *method_name, char const_val);

int msx(JavaClass *java_class, Generator *generator, const char *method_name, int const_val);

int msx(JavaClass *java_class, Generator *generator, const char *method_name, int const_val1, int const_val2);

#endif
