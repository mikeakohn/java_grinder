/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014 by Michael Kohn
 *
 */

#ifndef _IOPORT_H
#define _IOPORT_H

#include "Generator.h"
#include "JavaClass.h"

int ioport(JavaClass *java_class, Generator *generator, char *method_name, int port);
int ioport_setPinsAsInput_I(JavaClass *java_class, Generator *generator);
int ioport_setPinsAsOutput_I(JavaClass *java_class, Generator *generator);
int ioport_setPinsValue(JavaClass *java_class, Generator *generator);
int ioport_setPinsHight(JavaClass *java_class, Generator *generator);
int ioport_setPinsLow(JavaClass *java_class, Generator *generator);
int ioport_setPinAsOutput(JavaClass *java_class, Generator *generator);
int ioport_setPinAsInput(JavaClass *java_class, Generator *generator);
int ioport_setPinHigh(JavaClass *java_class, Generator *generator);
int ioport_setPinLow(JavaClass *java_class, Generator *generator);
int ioport_isPinInputHigh(JavaClass *java_class, Generator *generator);
int ioport_getPortOuputValue(JavaClass *java_class, Generator *generator);
int ioport_setPortOuputValue(JavaClass *java_class, Generator *generator);

#endif

