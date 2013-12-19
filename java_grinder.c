#include <stdio.h>
#include <stdlib.h>

#include "java_thread.h"
#include "java_class.h"
#include "java_execute.h"

#define STACK_LEN 65536

int main(int argc, char *argv[])
{
FILE *in;
struct java_thread_t java_thread;
struct java_class_t *java_class;

  if (argc != 2)
  {
    printf("Usage: %s <class>\n", argv[0]);
    exit(0);
  }

  in = fopen(argv[1],"rb");
  if (in == NULL)
  {
    printf("Cannot open classfile %s\n", argv[1]);
    exit(1);
  }

  java_class = java_class_read(in);
#ifdef DEBUG
  java_class_print(java_class);
#endif

  java_init_stack(&java_thread, STACK_LEN);
  java_execute_method(java_class, 1, &java_thread, 0);
  java_free_stack(&java_thread);

  fclose(in);

  return 0;
}


