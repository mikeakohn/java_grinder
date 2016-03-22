/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 * CPC support by Carsten Dost  - https://github.com/deringenieur71
 *
 */
 
#ifndef _CPC_H
#define _CPC_H

#include "Generator.h"
#include "JavaClass.h"

int cpc(JavaClass *java_class, Generator *generator, char *method_name);

int cpc(JavaClass *java_class, Generator *generator, char *method_name, int const_val);

int cpc(JavaClass *java_class, Generator *generator, char *method_name, int const_val1, int const_val2);

#endif

