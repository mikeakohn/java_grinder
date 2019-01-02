/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_API_PROPELLER_H
#define JAVA_GRINDER_GENERATOR_API_PROPELLER_H

class API_Propeller
{
public:
  virtual int propeller_setClock_I() { return -1; }
  virtual int propeller_getCogId() { return -1; }
  virtual int propeller_stopCog_I() { return -1; }
  virtual int propeller_waitPinsEqual_II() { return -1; }
  virtual int propeller_waitPinsEqual_II(int mask) { return -1; }
  virtual int propeller_waitPinsNotEqual_II() { return -1; }
  virtual int propeller_waitPinsNotEqual_II(int mask) { return -1; }
  virtual int propeller_waitCount_II() { return -1; }
  virtual int propeller_waitCount_II(int delay) { return -1; }
};

#endif

