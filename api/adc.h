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

#ifndef _ADC_H
#define _ADC_H

#include "Generator.h"
#include "JavaClass.h"

int adc(JavaClass *java_class, Generator *generator, char *method_name);
int adc(JavaClass *java_class, Generator *generator, char *method_name, int const_val);

#endif

