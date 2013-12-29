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

struct generator_t;

typedef int (*init_t)(void **, FILE *);
typedef void (*serial_init_t)(void *, FILE *);
typedef void (*method_t)(void *, FILE *, char *);
typedef int (*push_integer_t)(void *, FILE *, int32_t);
typedef int (*push_long_t)(void *, FILE *, int64_t);
typedef int (*push_float_t)(void *, FILE *, float);
typedef int (*push_double_t)(void *, FILE *, double);
typedef void (*close_t)(void *, FILE *);

struct generator_t
{
  FILE *out;
  void *context;
  init_t init;
  serial_init_t serial_init;
  method_t method;
  push_integer_t push_integer;
  push_long_t push_long;
  push_float_t push_float;
  push_double_t push_double;
  close_t close;
};

enum
{
  CPU_INVALID,
  CPU_MSP430,
  CPU_DSPIC,
};

int generator_init(struct generator_t *generator, char *filename, int cpu_type);
void generator_close(struct generator_t *generator);

#endif

