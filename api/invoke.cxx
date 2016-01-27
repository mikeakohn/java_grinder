/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "JavaClass.h"
#include "JavaCompiler.h"
#include "invoke.h"
#include "adc.h"
#include "c64_sid.h"
#include "c64_vic.h"
#include "cpu.h"
#include "dsp.h"
#include "ioport.h"
#include "memory.h"
#include "spi.h"
#include "ti84_.h"
#include "ti99_.h"
#include "timer.h"
#include "uart.h"
#include "java_lang_string.h"
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

void get_signature(char *signature, int *params, int *is_void)
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
    if (*signature == '[')
    {
      // The next char (or set of chars) should be the array type...
      // don't think anything else really needs to be done here.
    }
      else
    if (*signature == 'L')
    {
      while(*signature != ';' && *signature != 0)
      {
        signature++; 
      }
      if (*signature == 0) { signature--; }

      (*params)++;
    }
      else
    {
      (*params)++;
    }

    signature++;
  }
}

void get_static_function(char *function, char *method_name, char *method_sig)
{
char *s;
int ptr = 0;

  s = method_name;
  while (*s != 0)
  {
    function[ptr++] = *s;
    s++;
  }

  function[ptr++] = '_';
  s = method_sig + 1;
  while(*s != 0)
  {
    if (*s == ')') { break; }
    if (*s == '[') { function[ptr++] = 'a'; }
    if (*s == 'L' &&
        strncmp(s, "Ljava/lang/String;", sizeof("Ljava/lang/String;") - 1) == 0)
    {
      function[ptr++] = 'X';
      s += sizeof("Ljava/lang/String;") - 2;
    }
    else { function[ptr++] = *s; }
    //function[ptr++] = *s;
    s++;
  }

  if (ptr > 1 && function[ptr-1] == '_') { function[ptr-1] = 0; }
  function[ptr] = 0;
}

