/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "Compiler.h"

int Compiler::get_class_type(const char *filename)
{
FILE *in;
uint8_t magic[4];

  in = fopen(filename, "rb");
  if (in == NULL) { return INVALID; }
  fread(magic, 1, 4, in);
  fclose(in);

  if (magic[0] == 0xca && magic[1] == 0xfe &&
      magic[2] == 0xba && magic[3] == 0xbe)
  {
    return JAVA;
  }

  if (magic[0] == 'M' && magic[1] == 'Z')
  {
    return DOT_NET;
  }

  return INVALID;
}


