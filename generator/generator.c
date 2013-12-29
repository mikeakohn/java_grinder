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
#include "msp430.h"
#include "generator.h"

int generator_init(struct generator_t *generator, char *filename, int cpu_type)
{
  generator->out = fopen(filename, "wb");

  if (generator->out == NULL)
  {
    printf("Couldn't open file %s for writing.\n", filename);
    return -1;
  }

  switch(cpu_type)
  {
    case CPU_MSP430:
      generator->init = msp430_init;
      generator->close = msp430_close;
      break;
    case CPU_DSPIC:
      generator->init = dspic_init;
      generator->close = dspic_close;
      break;
    default:
      printf("WTF: error\n");
      return -1;
  }

  return generator->init(&generator->context);
}

void generator_close(struct generator_t *generator)
{
  generator->close(generator->context);
  fclose(generator->out);
}

