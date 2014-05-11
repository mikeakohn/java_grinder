/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "ATTINY85.h"

ATTINY85::ATTINY85()
{

}

ATTINY85::~ATTINY85()
{
}

int ATTINY85::start_init()
{
  need_farjump = 0;

  fprintf(out, ".avr8\n");
  fprintf(out, ".include \"tn85def.inc\"\n");
  AVR8::start_init();

  return 0;
}

#if 0
void ATTINY85::close()
{
}
#endif

