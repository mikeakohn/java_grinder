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

#ifndef _DSPIC_H
#define _DSPIC_H

#include "Generator.h"

class DSPIC : public Generator
{
  int reg;            // count number of registers are are using as stack
  int stack_count;    // count how many things we put on the stack
};

#endif

