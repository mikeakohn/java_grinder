/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2013 by Michael Kohn
 *
 */

#ifndef _GENERATOR_H
#define _GENERATOR_H

struct generator_t;

typedef int (*init_t)(void **);
typedef void (*close_t)(void *);

struct generator_t
{
  FILE *out;
  void *context;
  init_t init;
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

