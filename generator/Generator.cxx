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

#include "DSPIC.h"
#include "MSP430.h"
#include "Generator.h"

Generator::Generator()
{
}

Generator::~Generator()
{
}

int Generator::open(char *filename)
{
  out = fopen(filename, "wb");

  if (out == NULL)
  {
    printf("Couldn't open file %s for writing.\n", filename);
    return -1;
  }

  return 0;
}

void Generator::close()
{
  fclose(out);
}

void Generator::label(char *name)
{
  fprintf(out, "%s:", name);
}


