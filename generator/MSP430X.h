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

#ifndef _MSP430X_H
#define _MSP430X_H

#include "Generator.h"
#include "MSP430.h"

class MSP430X : public MSP430
{
public:
  MSP430X();
  virtual ~MSP430X();

  virtual int shift_left_integer();
  virtual int shift_right_integer();
  virtual int shift_right_uinteger();
};

#endif

