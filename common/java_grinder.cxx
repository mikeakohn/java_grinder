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

#include "JavaClass.h"
#include "compile.h"
#include "Generator.h"
#include "ARM.h"
#include "DSPIC.h"
#include "M6502.h"
#include "MSP430.h"
#include "MSP430X.h"

#define STACK_LEN 65536

int main(int argc, char *argv[])
{
FILE *in;
Generator *generator;
JavaClass *java_class;
int index;

  if (argc != 4)
  {
    printf("Usage: %s <class> <outfile> <dspic/msp430g2231/msp430g2553/m6502/arm>\n", argv[0]);
    exit(0);
  }

  in = fopen(argv[1],"rb");
  if (in == NULL)
  {
    printf("Cannot open classfile %s\n", argv[1]);
    exit(1);
  }

  if (strcasecmp("msp430g2231",argv[3]) == 0)
  {
    generator = new MSP430(MSP430G2231);
  }
    else
  if (strcasecmp("msp430g2553",argv[3]) == 0)
  {
    generator = new MSP430(MSP430G2553);
  }
    else
  if (strcasecmp("msp430x",argv[3]) == 0)
  {
    generator = new MSP430X(0);
  }
    else
  if (strcasecmp("dspic30f3012",argv[3]) == 0)
  {
    generator = new DSPIC(DSPIC30F3012);
  }
    else
  if (strcasecmp("m6502",argv[3]) == 0)
  {
    generator = new M6502();
  }
    else
  if (strcasecmp("arm",argv[3]) == 0)
  {
    generator = new ARM();
  }
    else
  {
    printf("Unknown cpu type: %s\n", argv[3]);
    exit(1);
  }

  if (generator->open(argv[2]) == -1)
  {
    exit(1);
  }

  java_class = new JavaClass(in);
#ifdef DEBUG
  java_class->print();
#endif

  int method_count = java_class->get_method_count();
  int ret = 0;
  for (index = 0; index < method_count; index++)
  {
    if (compile_method(java_class, index, generator) != 0)
    {
      printf("** Error compiling class.\n");
      ret = -1;
      break;
    }
  }

  delete generator;
  delete java_class;

  fclose(in);

  return ret;
}


