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
      generator->serial_init = msp430_serial_init;
      generator->method_start = msp430_method_start;
      generator->method_end = msp430_method_end;
      generator->push_integer = msp430_push_integer;
      generator->push_integer_local = msp430_push_integer_local;
      generator->push_long = msp430_push_long;
      generator->push_float = msp430_push_float;
      generator->push_double = msp430_push_double;
      generator->push_byte = msp430_push_byte;
      generator->push_short = msp430_push_short;
      generator->close = msp430_close;
      break;
    case CPU_DSPIC:
      generator->init = dspic_init;
      generator->close = dspic_close;
      generator->serial_init = dspic_serial_init;
      generator->method_start = dspic_method_start;
      generator->method_end = dspic_method_end;
      generator->push_integer = dspic_push_integer;
      generator->push_integer_local = dspic_push_integer_local;
      generator->push_long = dspic_push_long;
      generator->push_float = dspic_push_float;
      generator->push_double = dspic_push_double;
      generator->push_byte = dspic_push_byte;
      generator->push_short = dspic_push_short;
      generator->close = dspic_close;
      break;
    default:
      printf("WTF: error\n");
      return -1;
  }

  return generator->init(&generator->context, generator->out);
}

void generator_close(struct generator_t *generator)
{
  generator->close(generator->context, generator->out);
  fclose(generator->out);
}

