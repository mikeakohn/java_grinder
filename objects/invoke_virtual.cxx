/**
 *  Java Grinder
 *  Author: The Naken Crew
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2013 by The Naken Crew
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "JavaClass.h"
#include "compile.h"
#include "java_lang_system.h"

static void get_function(char *function, char *method_name, char *method_type, char *field_name, char *field_class)
{
char *s;

  s = field_class;
  while(*s != 0)
  {
    if (*s == '/') { field_class = s + 1; }
    if (*s >='A' && *s <= 'Z') { *s ^= 32; } // convert to lower
    s++;
  }

  s = method_type;
  while(*s != 0)
  {
    if (*s == '(') { method_type = s + 1; }
    if (*s == ')') { *s = 0; break; }
    s++;
  }

  sprintf(function, "%s_%s_%s_%s", field_class, field_name, method_name, method_type);
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

  get_function(function, method_name, method_type, field_name, field_class);

  printf("function: %s()\n", function);

  if (strncmp("system_out_println", function, sizeof("system_out_println"-1)) == 0)
  {
    return system_out_println(java_class, generator);
  }


  printf("Function not implemented '%s'\n", function);

  return 0;
}



