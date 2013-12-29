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

void dspic_main(void *context, FILE *out)
{
  // main() function goes here
  fprintf(out, "main:\n\n");
}

void dspic_close(void *context, FILE *out)
{
}


