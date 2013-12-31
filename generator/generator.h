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
typedef void (*method_start_t)(void *, FILE *, int, char *);
typedef void (*method_end_t)(void *, FILE *, int);
typedef int (*push_integer_t)(void *, FILE *, int32_t);
typedef int (*push_integer_local_t)(void *, FILE *, int);
typedef int (*push_long_t)(void *, FILE *, int64_t);
typedef int (*push_float_t)(void *, FILE *, float);
typedef int (*push_double_t)(void *, FILE *, double);
typedef int (*push_byte_t)(void *, FILE *, char);
typedef int (*push_short_t)(void *, FILE *, int16_t);
typedef void (*close_t)(void *, FILE *);

struct generator_t
{
  FILE *out;
  void *context;
  init_t init;
  serial_init_t serial_init;
  method_start_t method_start;
  method_end_t method_end;
  push_integer_t push_integer;
  push_integer_local_t push_integer_local;
  push_long_t push_long;
  push_float_t push_float;
  push_double_t push_double;
  push_byte_t push_byte;
  push_short_t push_short;
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

