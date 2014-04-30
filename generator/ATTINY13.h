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

#ifndef _ATTINY13_H
#define _ATTINY13_H

#include "AVR8.h"

class ATTINY13 : public AVR8
{
public:
  ATTINY13();
  virtual ~ATTINY13();

  virtual int start_init();
};

#endif

