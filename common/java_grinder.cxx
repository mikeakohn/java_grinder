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
#include "Compiler.h"
#include "JavaCompiler.h"
#include "execute_static.h"
#include "ARM.h"
#include "DSPIC.h"
#include "M6502.h"
#include "C64.h"
#include "AVR8.h"
#include "MC68000.h"
#include "MIPS.h"
#include "MSP430.h"
#include "MSP430X.h"
#include "STDC.h"
#include "TI84.h"
#include "Z80.h"
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
  if (strcasecmp("avr8", chip_type) == 0)
  {
    generator = new AVR8();
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
    else
  if (strcasecmp("ti84plus", chip_type) == 0)
  {
    generator = new TI84(TI84_PLUS);
  }
    else
  if (strcasecmp("ti84plusc", chip_type) == 0)
  {
    generator = new TI84(TI84_PLUS_C);
  }
    else
  if (strcasecmp("z80", chip_type) == 0)
  {
    generator = new Z80();
  }

  return generator;
}

int main(int argc, char *argv[])
{
Generator *generator;
Compiler *compiler;
const char *java_file;
const char *asm_file;
const char *chip_type;
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

  // Can we do .NET too? :)
  compiler = new JavaCompiler();

  for (n = 1; n < argc; n++)
  {
    if (strcmp(argv[n], "-O0") == 0)
    {
      compiler->disable_optimizer();
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

  generator = new_generator(chip_type);

  if (generator == NULL)
  {
    printf("Unknown cpu type: %s\n", chip_type);
    exit(1);
  }

  if (generator->open(asm_file) == -1)
  {
    delete generator;
    exit(1);
  }

  compiler->set_generator(generator);

  if (compiler->load_class(java_file) == -1)
  {
    printf("Couldn't open class file '%s'\n", java_file);
    delete generator;
    exit(1);
  }

  int ret = 0;

  compiler->insert_static_field_defines();
  compiler->init_heap();

  do
  {
    if (compiler->add_static_initializers() == -1) { ret = -1; break; }
    // Add the main function directly under init to save a jmp.
    if (compiler->compile_methods(true) == -1) { ret = -1; break; }
    // Compile all other methods.
    if (compiler->compile_methods(false) == -1) { ret = -1; break; }
  } while(0);

  delete generator;
  delete compiler;

  return ret;
}


