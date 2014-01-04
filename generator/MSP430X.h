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

