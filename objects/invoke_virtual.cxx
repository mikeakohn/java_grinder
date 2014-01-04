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
#include "java_compile.h"

int invoke_virtual(JavaClass *java_class, int method_id, Generator *generator)
{
char method_name[128];

  if (java_class->get_method_name(method_name, sizeof(method_name), method_id) != 0)
  {
    printf("Error: Couldn't get method name for method_id %d\n", method_id);
    return -1;
  }

  printf("invoke_virtual: '%s'\n", method_name);

#if 0

  // FIXME - Hack (this should probably be in a function somewhere)
  {
    struct constant_methodref_t *constant_method;
    struct constant_class_t *constant_class;
    struct constant_utf8_t *constant_utf8;
    struct constant_nameandtype_t *constant_nameandtype;
#ifdef DEBUG
    int n;
#endif
    constant_method = ((constant_methodref_t *)java_class->constants_heap) + java_class->constant_pool[ref];
    if (constant_method->tag != CONSTANT_METHODREF)
    {
      printf("Exception: reference is not a method\n");
      goto leave;
    }

    constant_class = ((constant_class_t *)java_class->constants_heap) + java_class->constant_pool[constant_method->class_index];
    constant_utf8 = ((constant_utf8_t *)java_class->constants_heap) + java_class->constant_pool[constant_class->name_index];
    constant_nameandtype = ((constant_nameandtype_t *)java_class->constants_heap) + java_class->constant_pool[constant_method->name_and_type_index];
#ifdef DEBUG
    printf("Call to class: ");
    for (n = 0; n < constant_utf8->length; n++)
    {
      printf("%c", constant_utf8->bytes[n]);
    }
    printf("\n");
#endif
    constant_utf8 = ((constant_utf8_t *)java_class->constants_heap) + java_class->constant_pool[constant_nameandtype->name_index];
#ifdef DEBUG
    printf("Call to method: ");
    for (n = 0; n < constant_utf8->length; n++)
    {
      printf("%c", constant_utf8->bytes[n]);
    }
    printf("\n");
#endif
    if (strncmp((char*)constant_utf8->bytes, "println", constant_utf8->length)!=0)
    {
      printf("We only support println right now (and not even very well :)\n");
      goto leave;
    }
    constant_utf8 = ((constant_utf8_t *)java_class->constants_heap) + java_class->constant_pool[constant_nameandtype->descriptor_index];

#ifdef DEBUG
    printf("Call to signature: ");
    for (n = 0; n < constant_utf8->length; n++)
    {
      printf("%c", constant_utf8->bytes[n]);
    }
    printf("\n");
#endif

    if (constant_utf8->bytes[1] == 'I')
    {
      //value1 = POP_INTEGER(); // FIXME.. wrong
      value1 = 0;
      printf("%d\n",value1);
    }
      else
    if (constant_utf8->bytes[1] == 'F')
    {
      float f1;
      //f1 = POP_FLOAT(); // FIXME.. wrong
      f1 = 0;
      printf("%f\n", f1);
    }
      else
    {
      printf("Cannot println this type %c\n",constant_utf8->bytes[1]);
      goto leave;
    }
  }

  //ref = POP_REF(); // FIXME.. wrong
  // printf("invokevirtual %d\n",GET_PC_UINT16(1));
  ref = 0;  // FIXME.. wrong

#endif

  return 0;
}



