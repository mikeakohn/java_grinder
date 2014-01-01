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

#ifndef _GENERATOR_H
#define _GENERATOR_H

class generator
{
public:
  generator();
  virtual ~generator();

  int open(char *filename);
  void label(char *name);

  virtual int init();
  virtual void serial_init();
  virtual void method_start(int local_count, char *name);
  virtual void method_end(int local_count);
  virtual int push_integer(int32_t n);
  virtual int push_integer_local(int index);
  virtual int push_long(int64_t n);
  virtual int push_float(float f);
  virtual int push_double(double f);
  virtual int push_byte(char b);
  virtual int push_short(int16_t s);
  virtual void close();

private:
  FILE *out;
};

enum
{
  CPU_INVALID,
  CPU_MSP430,
  CPU_DSPIC,
};

#endif

