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
#if 0
int ioport_setPinsAsInput_I(JavaClass *java_class, Generator *generator);
int ioport_setPinsAsOutput_I(JavaClass *java_class, Generator *generator);
int ioport_setPinsValue_I(JavaClass *java_class, Generator *generator);
int ioport_setPinsHigh_I(JavaClass *java_class, Generator *generator);
int ioport_setPinsLow_I(JavaClass *java_class, Generator *generator);
int ioport_setPinAsOutput_I(JavaClass *java_class, Generator *generator);
int ioport_setPinAsInput_I(JavaClass *java_class, Generator *generator);
int ioport_setPinHigh_I(JavaClass *java_class, Generator *generator);
int ioport_setPinLow_I(JavaClass *java_class, Generator *generator);
int ioport_isPinInputHigh_I(JavaClass *java_class, Generator *generator);
int ioport_getPortOuputValue(JavaClass *java_class, Generator *generator);
int ioport_setPortOuputValue_I(JavaClass *java_class, Generator *generator);
#endif

#endif

