#ifndef _JAVA_THREAD_H
#define _JAVA_THREAD_H

struct java_thread_t
{
  void *stack_values;
  void *stack_types;
  int stack_len;
};

int java_init_stack(struct java_thread_t *java_thread, int len);
void java_free_stack(struct java_thread_t *java_thread);

#endif

