/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "api/invoke.h"
#include "api/adc.h"
#include "api/c64_sid.h"
#include "api/c64_vic.h"
#include "api/cpu.h"
#include "api/dsp.h"
#include "api/ioport.h"
#include "api/memory.h"
#include "api/spi.h"
#include "api/ti84.h"
#include "api/ti99.h"
#include "api/timer.h"
#include "api/uart.h"
#include "api/java_lang_string.h"
#include "api/java_lang_system.h"
#include "common/JavaClass.h"
#include "common/JavaCompiler.h"

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

void get_signature(std::string &signature, int *params, int *is_void)
{
  const char *s = signature.c_str();

  *params = 0;
  *is_void = 0;

  while(*s != 0)
  {
    if (*s == '(')
    {
    }
      else
    if (*s == ')')
    {
      *is_void = (s[1] == 'V') ? 1 : 0;
      break;
    }
      else
    if (*s == '[')
    {
      // The next char (or set of chars) should be the array type...
      // don't think anything else really needs to be done here.
    }
      else
    if (*s == 'L')
    {
      while(*s != ';' && *s != 0)
      {
        s++; 
      }
      if (*s == 0) { s--; }

      (*params)++;
    }
      else
    {
      (*params)++;
    }

    s++;
  }
}

std::string get_static_function(
  std::string &method_name,
  std::string &method_sig)
{
  const char *s;
  int ptr = 0;

  // function name length should be length of method_name plus 1 char
  // for the _ in between plus method_sig length plus a NULL terminator
  // at maximum.
  char function[method_name.length() + method_sig.length() + 2];

  s = method_name.c_str();

  while (*s != 0)
  {
    function[ptr++] = *s;
    s++;
  }

  function[ptr++] = '_';

  s = method_sig.c_str() + 1;

  while(*s != 0)
  {
    if (*s == ')') { break; }

    if (*s == '[')
    {
      function[ptr++] = 'a';
    }
      else
    if (*s == 'L' &&
        strncmp(s, "Ljava/lang/String;", sizeof("Ljava/lang/String;") - 1) == 0)
    {
      function[ptr++] = 'X';
      s += sizeof("Ljava/lang/String;") - 2;
    }
      else
    {
      function[ptr++] = *s;
    }

    s++;
  }

  if (ptr > 1 && function[ptr-1] == '_') { function[ptr-1] = 0; }

  function[ptr] = 0;

  return std::string(function);
}

