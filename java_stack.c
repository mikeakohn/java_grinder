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
#include <string.h>

#include "java_stack.h"

int java_stack_init(struct java_stack_t *java_stack, int len)
{
  java_stack->values = malloc(len * sizeof(int));
  java_stack->types = malloc(len);
  java_stack->len = len;

  return 0;
}

void java_stack_free(struct java_stack_t *java_stack)
{
  free(java_stack->values);
  free(java_stack->types);
  java_stack->values = 0;
  java_stack->types = 0;
}

