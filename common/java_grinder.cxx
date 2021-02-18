/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2021 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/Compiler.h"
#include "common/JavaCompiler.h"
#include "common/execute_static.h"
#include "common/version.h"
#include "generator/Amiga.h"
#include "generator/AppleIIgs.h"
#include "generator/ARM.h"
#include "generator/Atari2600.h"
#include "generator/AVR8.h"
#include "generator/C64.h"
#include "generator/CPC.h"
#include "generator/DotNet.h"
#include "generator/DSPIC.h"
#include "generator/Epiphany.h"
#include "generator/Intellivision.h"
#include "generator/M6502.h"
#include "generator/M6502_8.h"
#include "generator/MC68000.h"
#include "generator/MIPS32.h"
#include "generator/MSP430.h"
#include "generator/MSP430X.h"
#include "generator/MCS51.h"
#include "generator/MSX.h"
#include "generator/PIC32.h"
#include "generator/Playstation2.h"
#include "generator/Propeller.h"
#include "generator/SegaDreamcast.h"
#include "generator/SegaGenesis.h"
#include "generator/SNES.h"
#include "generator/STDC.h"
#include "generator/TI84.h"
#include "generator/TI99.h"
#include "generator/TMS9900.h"
#include "generator/TRS80Coco.h"
#include "generator/W65816.h"
#include "generator/W65C134SXB.h"
#include "generator/W65C265SXB.h"
#include "generator/WebAssembly.h"
#include "generator/X86.h"
#include "generator/X86_64.h"
#include "generator/Z80.h"

#define STACK_LEN 65536

static Generator *new_generator(const char *chip_type)
{
  Generator *generator = NULL;

  if (strcasecmp("8051", chip_type) == 0)
  {
    generator = new MCS51();
  }
    else
  if (strcasecmp("amiga", chip_type) == 0)
  {
    generator = new Amiga();
  }
  if (strcasecmp("appleiigs", chip_type) == 0)
  {
    generator = new AppleIIgs();
  }
    else
  if (strcasecmp("arm", chip_type) == 0)
  {
    generator = new ARM();
  }
    else
  if (strcasecmp("attiny13", chip_type) == 0)
  {
    generator = new AVR8(ATTINY13);
  }
    else
  if (strcasecmp("attiny85", chip_type) == 0)
  {
    generator = new AVR8(ATTINY85);
  }
    else
  if (strcasecmp("attiny84", chip_type) == 0)
  {
    generator = new AVR8(ATTINY84);
  }
    else
  if (strcasecmp("attiny2313", chip_type) == 0)
  {
    generator = new AVR8(ATTINY2313);
  }
    else
  if (strcasecmp("atmega328", chip_type) == 0)
  {
    generator = new AVR8(ATMEGA328);
  }
    else
  if (strcasecmp("atmega328p", chip_type) == 0)
  {
    generator = new AVR8(ATMEGA328P);
  }
    else
  if (strcasecmp("atari2600", chip_type) == 0)
  {
    generator = new Atari2600();
  }
    else
  if (strcasecmp("c64", chip_type) == 0)
  {
    generator = new C64();
  }
    else
  if (strcasecmp("cpc", chip_type) == 0)
  {
    generator = new CPC();
  }
    else
  if (strcasecmp("dotnet", chip_type) == 0)
  {
    generator = new DotNet();
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
  if (strcasecmp("epiphany", chip_type) == 0)
  {
    generator = new Epiphany();
  }
    else
  if (strcasecmp("intellivision", chip_type) == 0)
  {
    generator = new Intellivision();
  }
    else
  if (strcasecmp("m6502", chip_type) == 0)
  {
    generator = new M6502();
  }
    else
  if (strcasecmp("m6502_8", chip_type) == 0)
  {
    generator = new M6502_8();
  }
    else
  if (strcasecmp("mc68000", chip_type) == 0)
  {
    generator = new MC68000();
  }
    else
  if (strcasecmp("mips32", chip_type) == 0)
  {
    generator = new MIPS32();
  }
    else
  if (strcasecmp("msp430g2231", chip_type) == 0)
  {
    generator = new MSP430(MSP430G2231);
  }
    else
  if (strcasecmp("msp430g2452", chip_type) == 0)
  {
    generator = new MSP430(MSP430G2452);
  }
    else
  if (strcasecmp("msp430g2553", chip_type) == 0)
  {
    generator = new MSP430(MSP430G2553);
  }
    else
  if (strcasecmp("msp430f5529", chip_type) == 0)
  {
    generator = new MSP430X(MSP430F5529);
  }
    else
  if (strcasecmp("msx", chip_type) == 0)
  {
    generator = new MSX();
  }
    else
  if (strcasecmp("pic32", chip_type) == 0)
  {
    generator = new PIC32();
  }
    else
  if (strcasecmp("playstation2", chip_type) == 0)
  {
    generator = new Playstation2();
  }
    else
  if (strcasecmp("propeller", chip_type) == 0)
  {
    generator = new Propeller();
  }
    else
  if (strcasecmp("sega_dreamcast", chip_type) == 0)
  {
    generator = new SegaDreamcast();
  }
    else
  if (strcasecmp("sega_genesis", chip_type) == 0)
  {
    generator = new SegaGenesis();
  }
    else
  if (strcasecmp("snes", chip_type) == 0)
  {
    generator = new SNES();
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
  if (strcasecmp("ti99", chip_type) == 0)
  {
    generator = new TI99();
  }
    else
  if (strcasecmp("tms9900", chip_type) == 0)
  {
    generator = new TMS9900();
  }
    else
  if (strcasecmp("trs80_coco", chip_type) == 0)
  {
    generator = new TRS80Coco();
  }
    else
  if (strcasecmp("w65816", chip_type) == 0)
  {
    generator = new W65816();
  }
    else
  if (strcasecmp("w65c134sxb", chip_type) == 0)
  {
    generator = new W65C134SXB();
  }
    else
  if (strcasecmp("w65c265sxb", chip_type) == 0)
  {
    generator = new W65C265SXB();
  }
    else
  if (strcasecmp("webasm", chip_type) == 0)
  {
    generator = new WebAssembly();
  }
    else
  if (strcasecmp("x86", chip_type) == 0)
  {
    generator = new X86();
  }
    else
  if (strcasecmp("x86_64", chip_type) == 0)
  {
    generator = new X86_64();
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
  const char *java_file = "";
  const char *asm_file = "";
  const char *chip_type = "";
  int option = 0;
  int n;

  printf("\nJava Grinder\n"
         "Authors: Michael Kohn, Joe Davisson, Carsten Dost\n"
         "    Web: http://www.mikekohn.net/micro/java_grinder.php\n"
         "  Email: mike@mikekohn.net\n\n"
         "Version: " VERSION "\n\n");

  if (argc < 4)
  {
    printf("Usage: %s [ -v -O0 ] <class> <outfile> <platform>\n"
           "   options:\n"
           "     -v verbose output\n"
           "     -O0 turn off optimizer\n"
           "   platforms:\n"
           "     8051\n"
           "     amiga\n"
           "     appleiigs\n"
           "     attiny2313, atmega328, atmega328p, attiny85, attiny84, attiny13,\n"
           "     dspic,\n"
           "     intellivision,\n"
           "     m6502, c64\n"
           "     m6502_8, atari2600\n"
           "     mips32, pic32, playstation2\n"
           "     msp430g2231, msp430g2452, msp430g2553, msp430f5529\n"
           "     propeller\n"
           "     sega_dreamcast, sega_genesis\n"
           "     ti99\n"
           "     w65c134sxb, w65c265sxb\n"
           "     x86, x86_64\n"
           "     z80, cpc, msx, ti84plus\n", argv[0]);
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
    if (strcmp(argv[n], "-v") == 0)
    {
      compiler->set_verbose();
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

  if (chip_type[0] == 0)
  {
    printf("No chip selected.  Bad command line arguments.\n");
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
    // Add constants at end if needed.
    if (compiler->add_constants() == -1) { ret = -1; break; }
  } while(0);

  // Any subclass (CPU / system) in the Generator can add any extra
  // functions and data sections needed to the assembly source before
  // closing the file.
  generator->finish();

  // The main generator super class can add any extra functions or
  // data sections before the file is close.
  generator->close();

  delete generator;
  delete compiler;

  return ret;
}


