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
typedef void (*close_t)(void *, FILE *);
typedef void (*serial_init_t)(void *, FILE *);
typedef void (*main_t)(void *, FILE *);

struct generator_t
{
  FILE *out;
  void *context;
  init_t init;
  close_t close;
  serial_init_t serial_init;
  main_t main;
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

