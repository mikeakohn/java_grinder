/**
 *  Java Grinder
 *  Author: The Naken Crew
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2013 by The Naken Crew
 *
 */

#ifndef _TABLE_JAVA_INSTR
#define _TABLE_JAVA_INSTR

#include <stdint.h>

// This table is mostly to help with pass 1... which is needed to figure
// out where the labels should be put in the .asm files.  To help with
// debugging, I put the name of the instruction in here also.

struct table_java_instr_t
{
  const char *name;
  uint8_t normal;      // width of instruction when wide=0
  uint8_t wide;        // width of instruction when wide=1
};

#endif

