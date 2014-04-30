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

#ifndef _ATTINY85_H
#define _ATTINY85_H

#include "AVR8.h"

class ATTINY85 : public AVR8
{
public:
  ATTINY85();
  virtual ~ATTINY85();

  virtual int start_init();
};

#endif

