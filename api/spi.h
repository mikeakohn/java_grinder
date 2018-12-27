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

#ifndef SPI_H
#define SPI_H

#include "Generator.h"
#include "JavaClass.h"

int spi(JavaClass *java_class, Generator *generator, const char *method_name, int port);
int spi(JavaClass *java_class, Generator *generator, const char *method_name, int port, int const_val);
int spi(JavaClass *java_class, Generator *generator, const char *method_name, int port, int const_val1, int const_val2);

#endif

