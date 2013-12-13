#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "java_thread.h"

int java_init_stack(struct java_thread_t *java_thread, int len)
{
  java_thread->stack_values = malloc(len * sizeof(int));
  java_thread->stack_types = malloc(len);
  java_thread->stack_len = len;

  return 0;
}

void java_free_stack(struct java_thread_t *java_thread)
{
  free(java_thread->stack_values);
  free(java_thread->stack_types);
  java_thread->stack_values = 0;
  java_thread->stack_types = 0;
}

