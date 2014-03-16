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

#include "Generator.h"
#include "JavaClass.h"
#include "JavaCompiler.h"
#include "execute_static.h"
#include "ARM.h"
#include "DSPIC.h"
#include "M6502.h"
#include "C64.h"
#include "MC68000.h"
#include "MIPS.h"
#include "MSP430.h"
#include "MSP430X.h"
#include "STDC.h"
#include "version.h"

#define STACK_LEN 65536

static Generator *new_generator(const char *chip_type)
{
Generator *generator = NULL;

  if (strcasecmp("arm", chip_type) == 0)
  {
    generator = new ARM();
  }
    else
  if (strcasecmp("c64", chip_type) == 0)
  {
    generator = new C64();
  }
    else
  if (strcasecmp("dspic30f3012", chip_type) == 0)
  {
    generator = new DSPIC(DSPIC30F3012);
  }
    else
  if (strcasecmp("dspic33fj06gs101a", chip_type) == 0)
  {
    generator = new DSPIC(DSPIC33FJ06GS101A);
  }
    else
  if (strcasecmp("m6502", chip_type) == 0)
  {
    generator = new M6502();
  }
    else
  if (strcasecmp("mc68000", chip_type) == 0)
  {
    generator = new MC68000();
  }
    else
  if (strcasecmp("mips", chip_type) == 0)
  {
    generator = new MIPS();
  }
    else
  if (strcasecmp("msp430g2231", chip_type) == 0)
  {
    generator = new MSP430(MSP430G2231);
  }
    else
  if (strcasecmp("msp430g2553", chip_type) == 0)
  {
    generator = new MSP430(MSP430G2553);
  }
    else
  if (strcasecmp("msp430x", chip_type) == 0)
  {
    generator = new MSP430X(0);
  }
    else
  if (strcasecmp("stdc", chip_type) == 0)
  {
    generator = new STDC();
  }

  return generator;
}

int main(int argc, char *argv[])
{
FILE *in;
Generator *generator;
JavaClass *java_class;
JavaCompiler java_compiler;
char method_name[32];
const char *java_file;
const char *asm_file;
const char *chip_type;
int index;
int option = 0;
int n;

  printf("\nJava Grinder\n");
  printf("Authors: Michael Kohn, Joe Davisson\n");
  printf("    Web: http://www.mikekohn.net/micro/java_grinder.php\n");
  printf("  Email: mike@mikekohn.net\n\n");
  printf("Version: "VERSION"\n\n");

  if (argc < 4)
  {
    printf("Usage: %s <class> <outfile> <dspic/msp430g2231/msp430g2553/m6502/c64/arm/mips/stdc>\n", argv[0]);
    exit(0);
  }

  for (n = 1; n < argc; n++)
  {
    if (strcmp(argv[n], "-O0") == 0)
    {
      java_compiler.disable_optimizer();
      continue;
    }
      else
    if (option == 0)
    {
      java_file = argv[n];
    }
      else
    if (option == 1)
    {
      asm_file = argv[n];
    }
      else
    if (option == 2)
    {
      chip_type = argv[n];
    }

    option++;
  }

  in = fopen(java_file, "rb");
  if (in == NULL)
  {
    printf("Cannot open classfile %s\n", argv[1]);
    exit(1);
  }

  generator = new_generator(chip_type);

  if (generator == NULL)
  {
    printf("Unknown cpu type: %s\n", chip_type);
    exit(1);
  }

  if (generator->open(asm_file) == -1)
  {
    exit(1);
  }

  java_compiler.set_generator(generator);

  java_class = new JavaClass(in);
#ifdef DEBUG
  java_class->print();
#endif

  int method_count = java_class->get_method_count();
  int field_count = java_class->get_field_count();
  int ret = 0;

  for (index = 0; index < field_count; index++)
  {
    char field_name[64];
    char field_type[64];
    java_class->get_field_name(field_name, sizeof(field_name), index);
    java_class->get_field_type(field_type, sizeof(field_type), index);
    generator->insert_static_field_define(field_name, field_type, index);
  }

  generator->start_init();
  generator->init_heap(field_count);

  do
  {
    // Add all the static initializers
    for (index = 0; index < method_count; index++)
    {
      if (java_class->get_method_name(method_name, sizeof(method_name), index) == 0)
      {
        if (strcmp("<clinit>", method_name) == 0)
        {
          if (execute_static(java_class, index, generator, false) != 0)
          {
            printf("** Error setting statics.\n");
            ret = -1;
            break;
          }
          continue;
        }
      }
    }

    if (ret != 0) { break; }
    generator->add_newline();

    // Add the main function directly under init to save a jmp.
    for (index = 0; index < method_count; index++)
    {
      if (java_class->get_method_name(method_name, sizeof(method_name), index) == 0)
      {
        if (strcmp(method_name, "main") == 0)
        {
          if (java_compiler.compile_method(java_class, index) != 0)
          {
            printf("** Error compiling class.\n");
            ret = -1;
            break;
          }
        }
      }
    }

    if (ret != 0) { break; }

    for (index = 0; index < method_count; index++)
    {
      if (java_class->get_method_name(method_name, sizeof(method_name), index) == 0)
      {
        if (strcmp(method_name, "main") == 0) { continue; }
        if (strcmp("<clinit>", method_name) == 0)
        {
          if (execute_static(java_class, index, generator, true) != 0)
          {
            printf("** Error setting statics.\n");
            ret = -1;
            break;
          }
          continue;
        }
      }

      if (java_compiler.compile_method(java_class, index) != 0)
      {
        printf("** Error compiling class.\n");
        ret = -1;
        break;
      }
    }
  } while(0);

  delete generator;
  delete java_class;

  fclose(in);

  return ret;
}


