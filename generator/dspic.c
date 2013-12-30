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
#include <inttypes.h>

#include "dspic.h"

int dspic_init(void **context, FILE *out)
{
  struct dspic_t *dspic = (struct dspic_t *)malloc(sizeof(struct dspic_t));
  *context = dspic;

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
  struct dspic_t *dspic = (struct dspic_t *)context;
  dspic->reg = 0;
  dspic->stack_count = 0;

  // main() function goes here
  fprintf(out, "%s:\n\n", name);
}

int dspic_push_integer(void *context, FILE *out, int32_t n)
{
  struct dspic_t *dspic = (struct dspic_t *)context;

  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);
    return -1;
  }

  uint16_t value = (n & 0xffff);

  if (dspic->reg < 8)
  {
    fprintf(out, "  mov #0x%02x, w%d\n", value, (dspic->reg + 1));
    dspic->reg++;
  }
    else
  {
    fprintf(out, "  mov #0x%02x, w0\n", value);
    fprintf(out, "  push w0\n");
  }

  return 0;
}

int dspic_push_long(void *context, FILE *out, int64_t n)
{
  struct dspic_t *dspic = (struct dspic_t *)context;

  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %"PRId64" bigger than 16 bit.\n", n);
    return -1;
  }

  uint16_t value = (n & 0xffff);

  if (dspic->reg < 8)
  {
    fprintf(out, "  mov #0x%02x, w%d\n", value, (dspic->reg + 1));
    dspic->reg++;
  }
    else
  {
    fprintf(out, "  mov #0x%02x, w0\n", value);
    fprintf(out, "  push w0\n");
  }
  return 0;
}

int dspic_push_float(void *context, FILE *out, float f)
{
  printf("Error: float is not currently supported.\n");
  return -1;
}

int dspic_push_double(void *context, FILE *out, double f)
{
  printf("Error: double is not currently supported.\n");
  return -1;
}

void dspic_close(void *context, FILE *out)
{
  free(context);
}


