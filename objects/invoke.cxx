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
#include "ioport.h"
#include "memory.h"
#include "java_lang_system.h"

static void get_virtual_function(char *function, char *method_name, char *method_type, char *field_name, char *field_class)
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
  s = method_type + 1;
  while(*s != 0)
  {
    //if (*s == '(') { method_type = s + 1; }
    if (*s == ')') { break; }
    function[ptr++] = *s;
    s++;
  }

  function[ptr] = 0;
  //sprintf(function, "%s_%s_%s_%s", field_class, field_name, method_name, method_type);
}

static void get_static_function(char *function, char *method_name, char *method_type)
{
char *s;
int ptr = 0;

  s = method_name;
  while (*s != 0) { function[ptr++] = *s; s++; }

  function[ptr++] = '_';
  s = method_type + 1;
  while(*s != 0)
  {
    if (*s == ')') { break; }
    function[ptr++] = *s;
    s++;
  }

  function[ptr] = 0;
}

int invoke_virtual(JavaClass *java_class, int method_id, int field_id, Generator *generator)
{
char field_name[128];
char field_type[128];
char field_class[128];
char method_name[128];
char method_type[128];
char method_class[128];
char function[256];

  if (java_class->get_ref_name_type(field_name, field_type, sizeof(field_name), field_id) !=0 ||
      java_class->get_class_name(field_class, sizeof(field_class), field_id) != 0)
  {
    printf("Error: Could not field info for field_id %d\n", field_id);
    return -1;
  }

  if (java_class->get_class_name(method_class, sizeof(method_class), method_id) != 0 ||
      java_class->get_ref_name_type(method_name, method_type, sizeof(method_name), method_id) != 0)
  {
    printf("Error: Couldn't get name and type for method_id %d\n", method_id);
    return -1;
  }

  printf("field: '%s as %s' from %s\n", field_name, field_type, field_class);
  printf("method: '%s as %s' from %s\n", method_name, method_type, method_class);

  get_virtual_function(function, method_name, method_type, field_name, field_class);

  printf("function: %s()\n", function);

  int ret = -1;
  if (strcmp(field_class, "java/lang/System") == 0)
  {
    ret = java_lang_system(java_class, generator, function);
  }

  if (ret == 0) { return 0; }

  printf("Function not implemented '%s'\n", function);

  return -1;
}

int invoke_static(JavaClass *java_class, int method_id, Generator *generator)
{
char method_name[128];
char method_type[128];
char method_class[128];
char function[256];

  if (java_class->get_class_name(method_class, sizeof(method_class), method_id) != 0 ||
      java_class->get_ref_name_type(method_name, method_type, sizeof(method_name), method_id) != 0)
  {
    printf("Error: Couldn't get name and type for method_id %d\n", method_id);
    return -1;
  }

  printf("method: '%s as %s' from %s\n", method_name, method_type, method_class);

  get_static_function(function, method_name, method_type);

  printf("function: %s()\n", function);
  int ret = -1;

  size_t len = sizeof("net/mikekohn/java_grinder/") - 1;
  if (strncmp("net/mikekohn/java_grinder/", method_class, len)==0)
  {
    char *cls = method_class + len;

    if (strcmp(cls, "IOPort0") == 0)
    {
      ret = ioport(java_class, generator, function, 0);
    }
      else
    if (strcmp(cls, "IOPort1") == 0)
    {
      ret = ioport(java_class, generator, function, 1);
    }
      else
    if (strcmp(cls, "Memory") == 0)
    {
      ret = memory(java_class, generator, function);
    } 
  }

  if (ret == 0) { return 0; }

  printf("Function not implemented '%s'\n", function);

  return -1;
}



