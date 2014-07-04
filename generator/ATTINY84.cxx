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

#include "ATTINY84.h"

ATTINY84::ATTINY84()
{

}

ATTINY84::~ATTINY84()
{
}

int ATTINY84::start_init()
{
  need_farjump = 1;

  fprintf(out, ".avr8\n");
  fprintf(out, ".include \"tn84def.inc\"\n");
  AVR8::start_init();

  return 0;
}

#if 0
void ATTINY84::close()
{
}
#endif

