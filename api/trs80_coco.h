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

#ifndef _TRS80_COCO_H
#define _TRS80_COCO_H

#include "Generator.h"
#include "JavaClass.h"

int trs80_coco(JavaClass *java_class, Generator *generator, char *method_name);
int trs80_coco(JavaClass *java_class, Generator *generator, char *method_name, int const_val);
int trs80_coco(JavaClass *java_class, Generator *generator, char *method_name, int const_val1, int const_val2);

#endif

