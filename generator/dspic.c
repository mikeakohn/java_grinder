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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "dspic.h"

int dspic_init(void **context, FILE *out)
{
  fprintf(out, ".dspic\n\n");

  // Add any set up items here
  fprintf(out, "start:\n\n");

  // main() function goes here
  fprintf(out, "main:\n\n");

  return 0;
}

void dspic_close(void *context, FILE *out)
{
}


