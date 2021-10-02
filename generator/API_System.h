/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2021 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_API_SYSTEM_H
#define JAVA_GRINDER_GENERATOR_API_SYSTEM_H

class API_System
{
public:
  // Memory
  virtual int memory_initHeap_I() { return -1; }
  virtual int memory_read8_I() { return -1; }
  virtual int memory_read8_I(int address) { return -1; }
  virtual int memory_write8_IB() { return -1; }
  virtual int memory_read16_I() { return -1; }
  virtual int memory_read16_I(int address) { return -1; }
  virtual int memory_write16_IS() { return -1; }
  virtual int memory_allocStackBytes_I() { return -1; }
  virtual int memory_allocStackShorts_I() { return -1; }
  virtual int memory_allocStackInts_I() { return -1; }
  virtual int memory_preloadByteArray_X(const char *array_name) { return -1; }
  virtual int memory_preloadIntArray_X(const char *array_name) { return -1; }
  virtual int memory_clearArray_aB() { return -1; }
  virtual int memory_clearArray_aS() { return -1; }
  virtual int memory_clearArray_aI() { return -1; }
  virtual int memory_addressOf_aB() { return -1; }
  virtual int memory_addressOf_aS() { return -1; }
  virtual int memory_addressOf_aC() { return -1; }
  virtual int memory_addressOf_aI() { return -1; }

  // CPU functions
  virtual int cpu_setClock1() { return -1; }
  virtual int cpu_setClock2() { return -1; }
  virtual int cpu_setClock4() { return -1; }
  virtual int cpu_setClock8() { return -1; }
  virtual int cpu_setClock16() { return -1; }
  virtual int cpu_setClock25() { return -1; }
  virtual int cpu_setClockExternal2() { return -1; }
  virtual int cpu_nop() { return -1; }
  virtual int cpu_asm_X(const char *code, int len) { return -1; }
  virtual int cpu_getCycleCount() { return -1; }
};

#endif

