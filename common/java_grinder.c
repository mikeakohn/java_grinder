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

#include "java_stack.h"
#include "java_class.h"
#include "java_compile.h"
#include "generator.h"

#define STACK_LEN 65536

int main(int argc, char *argv[])
{
FILE *in;
struct java_stack_t java_stack;
struct generator_t generator;
struct java_class_t java_class;
int cpu_type = CPU_INVALID;

  if (argc != 4)
  {
    printf("Usage: %s <class> <outfile> <dspic/msp430>\n", argv[0]);
    exit(0);
  }

  in = fopen(argv[1],"rb");
  if (in == NULL)
  {
    printf("Cannot open classfile %s\n", argv[1]);
    exit(1);
  }

  if (strcmp("msp430",argv[3]) == 0) { cpu_type = CPU_MSP430; }
  else if (strcmp("dspic",argv[3]) == 0) { cpu_type = CPU_DSPIC; }

  if (cpu_type == CPU_INVALID)
  {
    printf("Invalid cpu type %s\n", argv[3]);
    exit(1);
  }

  if (generator_init(&generator, argv[2], cpu_type) == -1)
  {
    exit(1);
  }

  java_class_read(&java_class, in);
#ifdef DEBUG
  java_class_print(&java_class);
#endif

  java_stack_init(&java_stack, STACK_LEN);
  java_compile_method(&java_class, 1, &generator, &java_stack, 0);
  java_stack_free(&java_stack);
  generator_close(&generator);

  fclose(in);

  return 0;
}


