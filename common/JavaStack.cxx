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
#include <string.h>

#include "JavaStack.h"

int JavaStack::JavaStack(int len)
{
  values = malloc(len * sizeof(int));
  types = malloc(len);
  this->len = len;

  memset(values, 0, len * sizeof(int));
  memset(types, 0, len);

  return 0;
}

void ~JavaStack::JavaStack()
{
  free(values);
  free(types);
  values = 0;
  types = 0;
}

