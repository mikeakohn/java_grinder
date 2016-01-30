/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 */

#ifndef _API_PROPELLER_H
#define _API_PROPELLER_H

class API_Propeller
{
public:
  virtual int propeller_setClock_I() { return -1; }
  virtual int propeller_getCogId() { return -1; }
  virtual int propeller_stopCog_I() { return -1; }
  virtual int propeller_stopCog_I(int value) { return -1; }
};

#endif

