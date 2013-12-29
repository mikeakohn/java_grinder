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


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "dspic.h"

int dspic_init(void **context, FILE *out)
{
  // For now we only support a specific chip
  fprintf(out, ".dspic\n");
  fprintf(out, ".include \"p30f3012.inc\"\n\n");

  // Add any set up items (stack, registers, etc)
  fprintf(out, "start:\n\n");

  return 0;
}

void dspic_serial_init(void *context, FILE *out)
{
}

void dspic_method(void *context, FILE *out, char *name)
{
  // main() function goes here
  fprintf(out, "%s:\n\n", name);
}

int dspic_push_integer(void *context, FILE *out, int32_t n)
{
  return 0;
}

int dspic_push_long(void *context, FILE *out, int64_t n)
{
  return 0;
}

int dspic_push_float(void *context, FILE *out, float f)
{
  return 0;
}

int dspic_push_double(void *context, FILE *out, double f)
{
  return 0;
}

void dspic_close(void *context, FILE *out)
{
}


