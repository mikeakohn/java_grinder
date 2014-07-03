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

#ifndef _ATTINY84_H
#define _ATTINY84_H

#include "AVR8.h"

class ATTINY84 : public AVR8
{
public:
  ATTINY84();
  virtual ~ATTINY84();

  virtual int start_init();
};

#endif

