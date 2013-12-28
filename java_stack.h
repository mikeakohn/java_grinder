#ifndef _JAVA_STACK_H
#define _JAVA_STACK_H

struct java_stack_t
{
  void *values;
  void *types;
  int len;
};

int java_stack_init(struct java_stack_t *java_stack, int len);
void java_stack_free(struct java_stack_t *java_stack);

#endif

