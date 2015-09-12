/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

#ifndef _API_SYSTEM_H
#define _API_SYSTEM_H

class API_System
{
public:
  // Memory
  virtual int memory_read8_I() { return -1; }
  virtual int memory_write8_IB() { return -1; }
  virtual int memory_read16_I() { return -1; }
  virtual int memory_write16_IS() { return -1; }

  // CPU functions
  virtual int cpu_setClock16() { return -1; }
  virtual int cpu_setClock25() { return -1; }
  virtual int cpu_setClockExternal2() { return -1; }
  virtual int cpu_nop() { return -1; }
  virtual int cpu_asm(const char *code, int len) { return -1; }
};

#endif

