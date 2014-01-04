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

#ifndef _COMPILE_H
#define _COMPILE_H

#include "Generator.h"
#include "JavaClass.h"

#define GET_PC_INT16(a) ((int16_t)(((uint16_t)bytes[pc+a+0])<<8|bytes[pc+a+1]))
#define GET_PC_UINT16(a) (((uint16_t)bytes[pc+a+0])<<8|bytes[pc+a+1])
#define GET_PC_INT32(a) ((int32_t)((uint32_t)bytes[pc+a+0])<<24| \
                        ((uint32_t)bytes[pc+a+1])<<16|\
                        ((uint32_t)bytes[pc+a+2])<<8|\
                         bytes[pc+a+3])
#define GET_PC_UINT32(a) (((uint32_t)bytes[pc+a+0])<<24| \
                         ((uint32_t)bytes[pc+a+1])<<16|\
                         ((uint32_t)bytes[pc+a+2])<<8|\
                          bytes[pc+a+3])

int compile_method(JavaClass *java_class, int method_id, Generator *generator);

#endif

