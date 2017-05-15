/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2017 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "JavaClass.h"
#include "JavaCompiler.h"
#include "invoke.h"
#include "invoke_static.h"
#include "adc.h"
#include "appleiigs_.h"
#include "atari_2600.h"
#include "c64_sid.h"
#include "c64_vic.h"
#include "cpc_.h"
#include "cpu.h"
#include "dsp.h"
#include "ioport.h"
#include "memory.h"
#include "msx_.h"
#include "playstation_2.h"
#include "propeller_.h"
#include "sega_genesis.h"
#include "spi.h"
#include "snes_.h"
#include "sxb.h"
#include "ti84_.h"
#include "ti99_.h"
#include "timer.h"
#include "trs80_coco.h"
#include "uart.h"

#define CHECK_WITH_PORT(a,b,c) \
    if (strcmp(cls, #a#c) == 0) \
    { \
      ret = b(java_class, generator, function, c); \
    }

#define CHECK(a,b) \
    if (strcmp(cls, #a) == 0) \
    { \
      ret = b(java_class, generator, function); \
    }

#define CHECK_WITH_PORT_CONST(a,b,c) \
    if (strcmp(cls, #a#c) == 0) \
    { \
      ret = b(java_class, generator, function, c, const_vals[0]); \
    }

#define CHECK_CONST(a,b) \
    if (strcmp(cls, #a) == 0) \
    { \
      ret = b(java_class, generator, function, const_vals[0]); \
    }

#define CHECK_WITH_PORT_CONST_2(a,b,c) \
    if (strcmp(cls, #a#c) == 0) \
    { \
      ret = b(java_class, generator, function, c, const_vals[0], const_vals[1]); \
    }

#define CHECK_CONST_2(a,b) \
    if (strcmp(cls, #a) == 0) \
    { \
      ret = b(java_class, generator, function, const_vals[0], const_vals[1]); \
    }

static void remove_illegal_chars(char *function)
{
  while(*function != 0)
  {
    if (*function == '/' || *function == ';') { *function = '_'; }
    function++;
  }
}

int invoke_static(JavaClass *java_class, int method_id, Generator *generator)
{
  char method_name[128];
  char method_sig[128];
  char method_class[128];
  char function[256];

  //printf("invoke_static()\n");

  if (java_class->get_class_name(method_class, sizeof(method_class), method_id) != 0 ||
      java_class->get_ref_name_type(method_name, method_sig, sizeof(method_name), method_id) != 0)
  {
    printf("Error: Couldn't get name and type for method_id %d\n", method_id);
    return -1;
  }

  //printf("method: '%s as %s' from %s\n", method_name, method_sig, method_class);

  get_static_function(function, method_name, method_sig);

  //printf("function: %s()\n", function);
  int ret = -1;

  size_t len = sizeof("net/mikekohn/java_grinder/") - 1;
  size_t len_c64 = sizeof("net/mikekohn/java_grinder/C64/") - 1;

  if (strncmp("net/mikekohn/java_grinder/C64/", method_class, len_c64) == 0)
  {
    char *cls = method_class + len_c64;

    CHECK(SID, c64_sid);
    CHECK(VIC, c64_vic);
  }
    else
  if (strncmp("net/mikekohn/java_grinder/", method_class, len) == 0)
  {
    char *cls = method_class + len;

    CHECK(CPU, cpu)
    CHECK_WITH_PORT(IOPort, ioport, 0)
    CHECK_WITH_PORT(IOPort, ioport, 1)
    CHECK_WITH_PORT(IOPort, ioport, 2)
    CHECK_WITH_PORT(IOPort, ioport, 3)
    CHECK_WITH_PORT(IOPort, ioport, 4)
    CHECK_WITH_PORT(IOPort, ioport, 5)
    CHECK_WITH_PORT(IOPort, ioport, 6)
    CHECK_WITH_PORT(IOPort, ioport, 7)
    CHECK(Memory, memory)
    CHECK(DSP, dsp)
    CHECK(AppleIIgs, appleiigs)
    CHECK(Atari2600, atari_2600)
    CHECK(ADC, adc)
    CHECK(CPC, cpc)
    CHECK(Timer, timer)
    CHECK_WITH_PORT(SPI, spi, 0)
    CHECK_WITH_PORT(SPI, spi, 1)
    CHECK_WITH_PORT(UART, uart, 0)
    CHECK_WITH_PORT(UART, uart, 1)
    CHECK(MSX, msx)
    CHECK(Playstation2, playstation2)
    CHECK(Propeller, propeller)
    CHECK(SegaGenesis, sega_genesis)
    CHECK(SNES, snes)
    CHECK(TI84, ti84)
    CHECK(TI99, ti99)
    CHECK(TRS80Coco, trs80_coco)
    CHECK(SXB, sxb)
      else
    {}
  }
    else
  {
    // FIXME: This probably is not good, although the assembler will catch it.
    //if (strcmp(method_class, java_class->class_name) == 0)
    {
      int params,is_void;
      get_signature(method_sig, &params, &is_void);
      remove_illegal_chars(function);
      ret = generator->invoke_static_method(function, params, is_void);
    }
  }

  if (ret == 0) { return 0; }

  printf("--> Function not implemented '%s' %s:%d\n", function, __FILE__, __LINE__);

  return -1;
}

int invoke_static(JavaClass *java_class, int method_id, Generator *generator, int *const_vals, int const_count)
{
  char method_name[128];
  char method_sig[128];
  char method_class[128];
  char function[256];

  //printf("const invoke_static() const_count=%d\n", const_count);

  if (java_class->get_class_name(method_class, sizeof(method_class), method_id) != 0 ||
      java_class->get_ref_name_type(method_name, method_sig, sizeof(method_name), method_id) != 0)
  {
    printf("Error: Couldn't get name and type for method_id %d\n", method_id);
    return -1;
  }

  //printf("const method: '%s as %s' from %s\n  const_count=%d\n", method_name, method_sig, method_class, const_count);

  get_static_function(function, method_name, method_sig);

  //printf("const function: %s()\n", function);
  int ret = -1;

  size_t len = sizeof("net/mikekohn/java_grinder/") - 1;

  if (strncmp("net/mikekohn/java_grinder/", method_class, len)!=0)
  {
    return -1;
  }

  char *cls = method_class + len;

  if (const_count == 1)
  {
    CHECK_WITH_PORT_CONST(IOPort, ioport, 0)
    CHECK_WITH_PORT_CONST(IOPort, ioport, 1)
    CHECK_WITH_PORT_CONST(IOPort, ioport, 2)
    CHECK_WITH_PORT_CONST(IOPort, ioport, 3)
    CHECK_WITH_PORT_CONST(IOPort, ioport, 4)
    CHECK_WITH_PORT_CONST(IOPort, ioport, 5)
    CHECK_WITH_PORT_CONST(IOPort, ioport, 6)
    CHECK_WITH_PORT_CONST(IOPort, ioport, 7)
    CHECK_CONST(Memory, memory)
    CHECK_CONST(ADC, adc)
    CHECK_CONST(AppleIIgs, appleiigs)
    CHECK_CONST(Atari2600, atari_2600)
    CHECK_CONST(Timer, timer)
    CHECK_CONST(CPU, cpu)
    CHECK_CONST(CPC, cpc)
    CHECK_CONST(MSX, msx)
    CHECK_CONST(Playstation2, playstation2)
    CHECK_CONST(Propeller, propeller)
    CHECK_CONST(SegaGenesis, sega_genesis)
    CHECK_CONST(SNES, snes)
    CHECK_CONST(TRS80Coco, trs80_coco)
    CHECK_CONST(SXB, sxb)
    CHECK_WITH_PORT_CONST(SPI, spi, 0)
    CHECK_WITH_PORT_CONST(SPI, spi, 1)
    CHECK_WITH_PORT_CONST(TI99, ti99, 0)
    CHECK_WITH_PORT_CONST(UART, uart, 0)
    CHECK_WITH_PORT_CONST(UART, uart, 1)
      else
    {}
  }
    else
  if (const_count == 2)
  {
    CHECK_WITH_PORT_CONST_2(SPI, spi, 0)
    CHECK_WITH_PORT_CONST_2(SPI, spi, 1)
    CHECK_CONST_2(Timer, timer)
    CHECK_CONST_2(Atari2600, atari_2600)
    CHECK_CONST_2(CPC, cpc)
    CHECK_CONST_2(MSX, msx)
    CHECK_CONST_2(Playstation2, playstation2)
    CHECK_CONST_2(TI99, ti99)
    CHECK_CONST_2(SegaGenesis, sega_genesis)
    CHECK_CONST_2(SNES, snes)
      else
    {}
  }

  if (ret == 0) { return 0; }

  //printf("invoke static const (not found)\n");

  return -1;
}


