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
#include <stdint.h>
#include <algorithm>

#include "api/invoke.h"
#include "api/invoke_static.h"
#include "api/adc.h"
#include "api/amiga.h"
#include "api/appleiigs.h"
#include "api/atari_2600.h"
#include "api/c64_sid.h"
#include "api/c64_vic.h"
#include "api/cpc.h"
#include "api/cpu.h"
#include "api/dsp.h"
#include "api/ioport.h"
#include "api/joystick.h"
#include "api/math.h"
#include "api/memory.h"
#include "api/msx.h"
#include "api/parallella.h"
#include "api/playstation_2.h"
#include "api/propeller.h"
#include "api/sega_genesis.h"
#include "api/spi.h"
#include "api/snes.h"
#include "api/sxb.h"
#include "api/ti84.h"
#include "api/ti99.h"
#include "api/timer.h"
#include "api/trs80_coco.h"
#include "api/uart.h"
#include "api/watchdog.h"
#include "common/JavaClass.h"
#include "common/JavaCompiler.h"

#define CHECK_WITH_PORT(a,b,c) \
    if (strcmp(cls, #a#c) == 0) \
    { \
      ret = b(java_class, generator, function.c_str(), c); \
    }

#define CHECK(a,b) \
    if (strcmp(cls, #a) == 0) \
    { \
      ret = b(java_class, generator, function.c_str()); \
    }

#define CHECK_WITH_PORT_CONST(a,b,c) \
    if (strcmp(cls, #a#c) == 0) \
    { \
      ret = b(java_class, generator, function.c_str(), c, const_vals[0]); \
    }

#define CHECK_CONST(a,b) \
    if (strcmp(cls, #a) == 0) \
    { \
      ret = b(java_class, generator, function.c_str(), const_vals[0]); \
    }

#define CHECK_WITH_PORT_CONST_2(a,b,c) \
    if (strcmp(cls, #a#c) == 0) \
    { \
      ret = b(java_class, generator, function.c_str(), c, const_vals[0], const_vals[1]); \
    }

#define CHECK_CONST_2(a,b) \
    if (strcmp(cls, #a) == 0) \
    { \
      ret = b(java_class, generator, function.c_str(), const_vals[0], const_vals[1]); \
    }

static void remove_illegal_chars(std::string &function)
{
  std::replace(function.begin(), function.end(), '/', '_');
  std::replace(function.begin(), function.end(), ';', '_');
}

int invoke_static(JavaClass *java_class, int method_id, Generator *generator)
{
  std::string method_name;
  std::string method_sig;
  std::string method_class;
  std::string function;

  //printf("invoke_static()\n");

  if (java_class->get_class_name(method_class, method_id) != 0 ||
      java_class->get_ref_name_type(method_name, method_sig, method_id) != 0)
  {
    printf("Error: Couldn't get name and type for method_id %d\n", method_id);
    return -1;
  }

  //printf("method: [%d] '%s' as %s' from %s\n",
  //  method_id, method_name.c_str(), method_sig.c_str(), method_class.c_str());

  function = get_static_function(method_name, method_sig);

  //printf("function: %s()\n", function.c_str());
  int ret = -1;

  #define PACKAGE_C64 "net/mikekohn/java_grinder/c64/"
  #define PACKAGE_AMIGA "net/mikekohn/java_grinder/amiga/"

  const size_t len = sizeof("net/mikekohn/java_grinder/") - 1;
  const size_t len_c64 = sizeof(PACKAGE_C64) - 1;
  const size_t len_amiga = sizeof(PACKAGE_AMIGA) - 1;

  if (strncmp(PACKAGE_C64, method_class.c_str(), len_c64) == 0)
  {
    const char *cls = method_class.c_str() + len_c64;

    CHECK(SID, c64_sid);
    CHECK(VIC, c64_vic);
  }
    else
  if (strncmp(PACKAGE_AMIGA, method_class.c_str(), len_amiga) == 0)
  {
    const char *cls = method_class.c_str() + len_amiga;

    CHECK(Amiga, amiga);
    //CHECK(Copper, copper);
    CHECK(Blitter, blitter);
  }
    else
  if (strncmp("net/mikekohn/java_grinder/", method_class.c_str(), len) == 0)
  {
    const char *cls = method_class.c_str() + len;

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
    CHECK(Joystick, joystick)
    CHECK(Math, math)
    CHECK(MSX, msx)
    CHECK(Parallella, parallella)
    CHECK(Playstation2, playstation2)
    CHECK(Propeller, propeller)
    CHECK(SegaGenesis, sega_genesis)
    CHECK(SNES, snes)
    CHECK(TI84, ti84)
    CHECK(TI99, ti99)
    CHECK(TRS80Coco, trs80_coco)
    CHECK(SXB, sxb)
    CHECK(Watchdog, watchdog)
      else
    {}
  }
    else
  {
    // FIXME: This probably is not good, although the assembler will catch it.
    //if (strcmp(method_class, java_class->class_name) == 0)
    {
      int params, is_void;

      get_signature(method_sig, &params, &is_void);

      remove_illegal_chars(function);

      ret = generator->invoke_static_method(function.c_str(), params, is_void);
    }
  }

  if (ret == 0) { return 0; }

  printf("--> Function not implemented '%s' %s:%d\n",
    function.c_str(), __FILE__, __LINE__);

  return -1;
}

int invoke_static(
  JavaClass *java_class,
  int method_id,
  Generator *generator,
  int *const_vals,
  int const_count)
{
  std::string method_name;
  std::string method_sig;
  std::string method_class;
  std::string function;

  //printf("const invoke_static() const_count=%d\n", const_count);

  if (java_class->get_class_name(method_class, method_id) != 0 ||
      java_class->get_ref_name_type(method_name, method_sig, method_id) != 0)
  {
    printf("Error: Couldn't get name and type for method_id %d\n", method_id);
    return -1;
  }

  //printf("const method: '%s as %s' from %s\n  const_count=%d\n",
  //  method_name.c_str(), method_sig.c_str(), method_class.c_str(),
  //  const_count);

  function = get_static_function(method_name, method_sig);

  //printf("const function: %s()\n", function.c_str());
  int ret = -1;

  const size_t len = sizeof("net/mikekohn/java_grinder/") - 1;

  if (strncmp("net/mikekohn/java_grinder/", method_class.c_str(), len)!=0)
  {
    return -1;
  }

  const char *cls = method_class.c_str() + len;

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
    CHECK_CONST(Joystick, joystick)
    CHECK_CONST(MSX, msx)
    CHECK_CONST(Parallella, parallella)
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
    CHECK_CONST(Watchdog, watchdog)
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

int invoke_static(
  JavaClass *java_class,
  int method_id,
  Generator *generator,
  const char *const_val)
{
  std::string method_name;
  std::string method_sig;
  std::string method_class;
  std::string function;

  if (java_class->get_class_name(method_class, method_id) != 0 ||
      java_class->get_ref_name_type(method_name, method_sig, method_id) != 0)
  {
    printf("Error: Couldn't get name and type for method_id %d\n", method_id);
    return -1;
  }

  function = get_static_function(method_name, method_sig);

  //printf("const function: %s()\n", function.c_str());
  int ret = -1;

  const size_t len = sizeof("net/mikekohn/java_grinder/") - 1;

  if (strncmp("net/mikekohn/java_grinder/", method_class.c_str(), len)!=0)
  {
    return -1;
  }

  if (strncmp("net/mikekohn/java_grinder/", method_class.c_str(), len) == 0)
  {
    const char *cls = method_class.c_str() + len;

    if (strcmp(cls, "Memory") == 0)
    {
      if (function == "preloadByteArray_X" ||
          function == "preloadIntArray_X")
      {
        char array_name[strlen(const_val) + 1];
        int ptr = 0;
        int type = TYPE_BYTE;

        if (function == "preloadIntArray_X") { type = TYPE_INT; }

        while(const_val[ptr] != 0)
        {
          if (const_val[ptr] == '.' ||
              const_val[ptr] == '/' ||
              const_val[ptr] == ' ' ||
              const_val[ptr] == '\\')
          {
            array_name[ptr] = '_';
          }
          else
          {
            array_name[ptr] = const_val[ptr];
          }

          ptr++;
        }

        array_name[ptr] = 0;

        generator->use_array_file(const_val, array_name, type);

        return memory(java_class, generator, function.c_str(), array_name);
      }
    }
  }

  if (ret == 0) { return 0; }

  return -1;
}

