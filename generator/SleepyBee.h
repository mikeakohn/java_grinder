/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2022 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_SLEEPY_BEE_H
#define JAVA_GRINDER_GENERATOR_SLEEPY_BEE_H

#include "generator/MCS51.h"

class SleepyBee : public MCS51
{
public:
  SleepyBee();
  virtual ~SleepyBee();

  virtual int open(const char *filename);

  // GPIO methods.
  virtual int ioport_setPinsAsInput_I(int port);
  virtual int ioport_setPinsAsInput_I(int port, int const_val);
  virtual int ioport_setPinsAsOutput_I(int port);
  virtual int ioport_setPinsAsOutput_I(int port, int const_val);
  virtual int ioport_setPinAsOutput_I(int port);
  virtual int ioport_setPinAsOutput_I(int port, int const_val);
  virtual int ioport_setPinAsInput_I(int port);
  virtual int ioport_setPinAsInput_I(int port, int const_val);

private:

};

#endif

