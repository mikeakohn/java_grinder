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
#include <stdint.h>

#include "JavaClass.h"
#include "compile.h"
#include "invoke.h"
#include "adc.h"
#include "cpu.h"
#include "dsp.h"
#include "ioport.h"
#include "memory.h"
#include "spi.h"
#include "uart.h"
#include "c64_system.h"
#include "c64_sid.h"
#include "c64_vic.h"
#include "java_lang_system.h"

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

static void get_virtual_function(char *function, char *method_name, char *method_sig, char *field_name, char *field_class)
{
char *s;
int ptr = 0;

  s = field_class;
  while(*s != 0)
  {
    if (*s == '/') { ptr = 0; s++; continue; }
    function[ptr++] = *s | 32;
    s++;
  }

  function[ptr++] = '_';
  s = field_name;
  while (*s != 0) { function[ptr++] = *s; s++; }

  function[ptr++] = '_';
  s = method_name;
  while (*s != 0) { function[ptr++] = *s; s++; }

  function[ptr++] = '_';
  s = method_sig + 1;
  while(*s != 0)
  {
    //if (*s == '(') { method_sig = s + 1; }
    if (*s == ')') { break; }
    function[ptr++] = *s;
    s++;
  }

  function[ptr] = 0;
  //sprintf(function, "%s_%s_%s_%s", field_class, field_name, method_name, method_sig);
}

static void get_signature(char *signature, int *params, int *is_void)
{
  *params = 0;
  *is_void = 0;

  while(*signature != 0)
  {
    if (*signature == '(')
    {
    }
      else
    if (*signature == ')')
    {
      *is_void = (signature[1] == 'V') ? 1 : 0;
      break;
    }
      else
    {
      (*params)++;
    }

    signature++;
  }
}

static void get_static_function(char *function, char *method_name, char *method_sig)
{
char *s;
int ptr = 0;

  s = method_name;
  while (*s != 0) { function[ptr++] = *s; s++; }

  function[ptr++] = '_';
  s = method_sig + 1;
  while(*s != 0)
  {
    if (*s == ')') { break; }
    function[ptr++] = *s;
    s++;
  }

  if (ptr > 1 && function[ptr-1] == '_') { function[ptr-1] = 0; }
  function[ptr] = 0;
}

int invoke_virtual(JavaClass *java_class, int method_id, int field_id, Generator *generator)
{
char field_name[128];
char field_type[128];
char field_class[128];
char method_name[128];
char method_sig[128];
char method_class[128];
char function[256];

  printf("invoke_virtual()\n");

  if (java_class->get_ref_name_type(field_name, field_type, sizeof(field_name), field_id) !=0 ||
      java_class->get_class_name(field_class, sizeof(field_class), field_id) != 0)
  {
    printf("Error: Could not field info for field_id %d\n", field_id);
    return -1;
  }

  if (java_class->get_class_name(method_class, sizeof(method_class), method_id) != 0 ||
      java_class->get_ref_name_type(method_name, method_sig, sizeof(method_name), method_id) != 0)
  {
    printf("Error: Couldn't get name and type for method_id %d\n", method_id);
    return -1;
  }

  printf("field: '%s as %s' from %s\n", field_name, field_type, field_class);
  printf("method: '%s as %s' from %s\n", method_name, method_sig, method_class);

  get_virtual_function(function, method_name, method_sig, field_name, field_class);

  printf("function: %s()\n", function);

  int ret = -1;
  if (strcmp(field_class, "java/lang/System") == 0)
  {
    ret = java_lang_system(java_class, generator, function);
  }

  if (ret == 0) { return 0; }

  printf("--> Function not implemented '%s'\n", function);

  return -1;
}

int invoke_static(JavaClass *java_class, int method_id, Generator *generator)
{
char method_name[128];
char method_sig[128];
char method_class[128];
char function[256];

  printf("invoke_static()\n");

  if (java_class->get_class_name(method_class, sizeof(method_class), method_id) != 0 ||
      java_class->get_ref_name_type(method_name, method_sig, sizeof(method_name), method_id) != 0)
  {
    printf("Error: Couldn't get name and type for method_id %d\n", method_id);
    return -1;
  }

  printf("method: '%s as %s' from %s\n", method_name, method_sig, method_class);

  get_static_function(function, method_name, method_sig);

  printf("function: %s()\n", function);
  int ret = -1;

  size_t len = sizeof("net/mikekohn/java_grinder/") - 1;
  size_t len_c64 = sizeof("net/mikekohn/java_grinder/C64/") - 1;

  if (strncmp("net/mikekohn/java_grinder/C64/", method_class, len_c64) == 0)
  {
    char *cls = method_class + len_c64;

    CHECK(System, c64_system);
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
    CHECK(Memory, memory)
    CHECK(DSP, dsp)
    CHECK(ADC, adc)
    CHECK_WITH_PORT(SPI, spi, 0)
    CHECK_WITH_PORT(SPI, spi, 1)
    CHECK_WITH_PORT(UART, uart, 0)
    CHECK_WITH_PORT(UART, uart, 1)
      else
    {}
  }
    else
  {
    if (strcmp(method_class, java_class->class_name) == 0)
    {
      int params,is_void;
      get_signature(method_sig, &params, &is_void);
      ret = generator->invoke_static_method(function, params, is_void);
    }
  }

  if (ret == 0) { return 0; }

  printf("--> Function not implemented '%s'\n", function);

  return -1;
}

int invoke_static(JavaClass *java_class, int method_id, Generator *generator, int *const_vals, int const_count)
{
char method_name[128];
char method_sig[128];
char method_class[128];
char function[256];

  printf("const invoke_static()\n");

  if (java_class->get_class_name(method_class, sizeof(method_class), method_id) != 0 ||
      java_class->get_ref_name_type(method_name, method_sig, sizeof(method_name), method_id) != 0)
  {
    printf("Error: Couldn't get name and type for method_id %d\n", method_id);
    return -1;
  }

  printf("const method: '%s as %s' from %s\n", method_name, method_sig, method_class);

  get_static_function(function, method_name, method_sig);

  printf("const function: %s()\n", function);
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
    CHECK_CONST(Memory, memory)
    CHECK_CONST(ADC, adc)
    CHECK_WITH_PORT_CONST(SPI, spi, 0)
    CHECK_WITH_PORT_CONST(SPI, spi, 1)
      else
    {}
  }
    else
  if (const_count == 2)
  {
    CHECK_WITH_PORT_CONST_2(SPI, spi, 0)
    CHECK_WITH_PORT_CONST_2(SPI, spi, 1)
      else
    {}
  }

  if (ret == 0) { return 0; }

  printf("invoke static const NOPE\n");

  return -1;
}


