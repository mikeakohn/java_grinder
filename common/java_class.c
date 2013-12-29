/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2013 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#include "fileio.h"
#include "java_class.h"

#ifdef DEBUG
static void print_constant_pool(struct java_class_t *java_class);
static void print_attributes(struct java_class_t *java_class);
static void print_fields(struct java_class_t *java_class);
static void print_methods(struct java_class_t *java_class);
#endif

static void read_attributes(FILE *in, struct java_class_t *java_class)
{
long marker;
int count;
int len;
int l;

  marker = ftell(in);

  for (count = 0; count < java_class->attributes_count; count++)
  {
    java_class->attributes[count] = ftell(in) - marker;
    fseek(in, 2, SEEK_CUR);
    l=read_int32(in);
    fseek(in, l, SEEK_CUR);
  }

  len = ftell(in) - marker;
  java_class->attributes_heap = (unsigned char *)malloc(len);
  fseek(in, marker, SEEK_SET);

#ifdef BIG_INDIAN
  (void)fread(java_class->attributes_heap, 1, len, in);
#else
  struct attributes_t *attribute;
  for (count = 0; count < java_class->attributes_count; count++)
  {
    attribute = (struct attributes_t *)(java_class->attributes_heap + java_class->attributes[count]);
    attribute->attribute_name_index = read_int16(in);
    attribute->attribute_length = read_int32(in);
    if (fread(attribute->info, 1, attribute->attribute_length, in));
  }
#endif
}

static void read_fields(FILE *in, struct java_class_t *java_class)
{
long marker;
int count;
int len;
int n,l,r;

  marker = ftell(in);

  for (count = 0; count < java_class->fields_count; count++)
  {
    java_class->fields[count] = ftell(in) - marker;
    fseek(in, 6, SEEK_CUR);
    n = read_int16(in);
    for (r = 0; r < n; r++)
    {
      fseek(in, 2, SEEK_CUR);
      l = read_int32(in);
      fseek(in, l, SEEK_CUR);
    }
  }

  len = ftell(in)-marker;
  java_class->fields_heap = (unsigned char *)malloc(len);
  fseek(in, marker, SEEK_SET);

#ifdef BIG_INDIAN
  if (fread(java_class->fields_heap, 1, len, in));
#else
  struct attributes_t *attribute;
  struct fields_t *field;
  for (count = 0; count < java_class->fields_count; count++)
  {
    field = (struct fields_t *)(java_class->fields_heap + java_class->fields[count]);
    field->access_flags = read_int16(in);
    field->name_index = read_int16(in);
    field->descriptor_index = read_int16(in);
    field->attribute_count = read_int16(in);
    n = 8;
    for (r = 0; r<field->attribute_count; r++)
    {
      attribute = (struct attributes_t *)(java_class->fields_heap+java_class->fields[count]+n);
      attribute->attribute_name_index = read_int16(in);
      attribute->attribute_length = read_int32(in);
      if (fread(attribute->info, 1, attribute->attribute_length, in));
      n = n + 6 + attribute->attribute_length;
    }
  }
#endif
}

static void read_methods(FILE *in, struct java_class_t *java_class)
{
long marker;
int count;
int len;
int n,l,r;

  marker = ftell(in);

  for (count = 0; count < java_class->methods_count; count++)
  {
    java_class->methods[count] = ftell(in) - marker;
    fseek(in, 6, SEEK_CUR);
    n = read_int16(in);
    for (r = 0; r < n; r++)
    {
      fseek(in, 2, SEEK_CUR);
      l = read_int32(in);
      fseek(in, l, SEEK_CUR);
    }
  }

  len = ftell(in)-marker;
  java_class->methods_heap = (unsigned char *)malloc(len);
  fseek(in, marker, SEEK_SET);
#ifdef BIG_INDIAN
  if (fread(java_class->methods_heap, 1, len, in));
#else
  struct attributes_t *attribute;
  struct methods_t *method;
  for (count = 0; count < java_class->methods_count; count++)
  {
    method = (struct methods_t *)(java_class->methods_heap + java_class->methods[count]);
    method->access_flags = read_int16(in);
    method->name_index = read_int16(in);
    method->descriptor_index = read_int16(in);
    method->attribute_count = read_int16(in);
    n = 8;
    for (r = 0; r < method->attribute_count; r++)
    {
      attribute = (struct attributes_t *)(java_class->methods_heap + java_class->methods[count] + n);
      attribute->attribute_name_index = read_int16(in);
      attribute->attribute_length = read_int32(in);
      if (fread(attribute->info, 1, attribute->attribute_length, in));
      n = n + 6 + attribute->attribute_length;
    }
  }
#endif
}

static void read_constant_pool(FILE *in, struct java_class_t *java_class)
{
long marker;
int len,count;
int ch;

  marker = ftell(in);
  len = 0;

  for (count = 0; count < java_class->constant_pool_count - 1; count++)
  {
    // java_class->constant_pool[count+1] = ftell(in) - marker;
    java_class->constant_pool[count+1] = len;

    ch = getc(in);

    switch(ch)
    {
      case CONSTANT_FIELDREF:
      case CONSTANT_METHODREF:
      case CONSTANT_INTERFACEMETHODREF:
      case CONSTANT_NAMEANDTYPE:
        fseek(in, 4, SEEK_CUR);
        len = len + sizeof(struct generic_twoint16_t);
        break;

      case CONSTANT_INTEGER:
      case CONSTANT_FLOAT:
        fseek(in, 4, SEEK_CUR);
        len = len + sizeof(struct generic_32bit_t);
        break;

      case CONSTANT_CLASS:
      case CONSTANT_STRING:
        fseek(in, 2, SEEK_CUR);
        len = len + sizeof(struct constant_class_t);
        break;

      case CONSTANT_LONG:
      case CONSTANT_DOUBLE:
        fseek(in, 8, SEEK_CUR);
        len = len + sizeof(struct constant_double_t);
        break;

      case CONSTANT_UTF8:
        ch = read_int16(in);
        fseek(in, ch, SEEK_CUR);
        len = len + 2 + 2 + ch;
        break;

      default:
        printf("Error: Uknown constant type\n");
        break;
    }
  }

  // len=ftell(in)-marker;
  java_class->constants_heap = (unsigned char *)malloc(len);
  fseek(in, marker, SEEK_SET);

  // fread(java_class->constants_heap, 1, len, in);
  // Alla dina Indianer tillbeho:r oss

  void *constant_pool;
  struct generic_twoint16_t *gen2int;
  struct generic_32bit_t *gen32bit;
  struct generic_64bit_t *gen64bit;
  struct constant_utf8_t *utf8;
  struct constant_class_t *cls;

  for (count = 0; count < java_class->constant_pool_count - 1; count++)
  {
    constant_pool = ((void *)java_class->constants_heap) + java_class->constant_pool[count+1];

    ch = getc(in);

    switch(ch)
    {
      case CONSTANT_FIELDREF:
      case CONSTANT_METHODREF:
      case CONSTANT_INTERFACEMETHODREF:
      case CONSTANT_NAMEANDTYPE:
        gen2int = (struct generic_twoint16_t *)constant_pool;
        gen2int->tag = ch;
        gen2int->int1 = read_int16(in);
        gen2int->int2 = read_int16(in);
        break;

      case CONSTANT_INTEGER:
      case CONSTANT_FLOAT:
        gen32bit = (struct generic_32bit_t *)constant_pool;
        gen32bit->tag = ch;
        gen32bit->value = read_int32(in);
        break;

      case CONSTANT_CLASS:
      case CONSTANT_STRING:
        cls = (struct constant_class_t *)constant_pool;
        cls->tag = ch;
        cls->name_index = read_int16(in);
        break;

      case CONSTANT_LONG:
      case CONSTANT_DOUBLE:
        gen64bit = (struct generic_64bit_t *)constant_pool;
        gen64bit->tag = ch;
        gen64bit->value = read_int64(in);
        break;

      case CONSTANT_UTF8:
        utf8 = (struct constant_utf8_t *)constant_pool;
        utf8->tag = ch;
        utf8->length = read_int16(in);
        if (fread(utf8->bytes, 1, utf8->length, in));
        break;

      default:
        printf("Error: Uknown constant type\n");
        break;
    }
  }
}

/* In the movie the 6th Sense, the character Bruce Willis plays is
 * already dead and no one can see him except the little boy who
 * can see dead people.  That's what you get for reading my source
 * code!  :)  */

struct java_class_t *java_class_read(FILE *in)
{
struct java_class_t *java_class;
int t;

  java_class = (struct java_class_t *)malloc(sizeof(struct java_class_t));
  memset(java_class, 0, sizeof(struct java_class_t));

  java_class->magic = read_int32(in);
  java_class->minor_version = read_int16(in);
  java_class->major_version = read_int16(in);

  java_class->constant_pool_count = read_int16(in);
  if (java_class->constant_pool_count!=0)
  {
    java_class->constant_pool = (int *)malloc(java_class->constant_pool_count*sizeof(int));
    memset(java_class->constant_pool, 0, java_class->constant_pool_count * sizeof(int));
    read_constant_pool(in, java_class);
  }

  java_class->access_flags = read_int16(in);
  java_class->this_class = read_int16(in);
  java_class->super_class = read_int16(in);
  java_class->interfaces_count = read_int16(in);

  if (java_class->interfaces_count != 0)
  {
    java_class->interfaces = (int *)malloc(java_class->interfaces_count * sizeof(int));
    for (t = 0; t < java_class->interfaces_count; t++)
    {
      java_class->interfaces[t] = read_int16(in);
    }
  }

  java_class->fields_count = read_int16(in);
  if (java_class->fields_count != 0)
  {
    java_class->fields = (int *)malloc(java_class->fields_count * sizeof(int));
    memset(java_class->fields, 0, java_class->fields_count * sizeof(int));
    read_fields(in, java_class);
  }

  java_class->methods_count = read_int16(in);
  if (java_class->methods_count != 0)
  {
    java_class->methods = (int *)malloc(java_class->methods_count * sizeof(int));
    memset(java_class->methods, 0, java_class->methods_count * sizeof(int));
    read_methods(in, java_class);
  }

  java_class->attributes_count = read_int16(in);
  if (java_class->attributes_count != 0)
  {
    java_class->attributes = (int *)malloc(java_class->attributes_count * sizeof(int));
    memset(java_class->attributes, 0, java_class->attributes_count * sizeof(int));
    read_attributes(in, java_class);
  }

  return java_class;
}

void free_class(struct java_class_t *java_class)
{
  if (java_class->constant_pool != 0) free(java_class->constant_pool);
  if (java_class->interfaces != 0) free(java_class->interfaces);
  if (java_class->fields != 0) free(java_class->fields);
  if (java_class->methods != 0) free(java_class->methods);
  if (java_class->attributes != 0) free(java_class->attributes);

  if (java_class->constants_heap != 0) free(java_class->constants_heap);
  if (java_class->fields_heap != 0) free(java_class->fields_heap);
  if (java_class->methods_heap != 0) free(java_class->methods_heap);
  if (java_class->attributes_heap != 0) free(java_class->attributes_heap);
  free(java_class);
}

#ifdef DEBUG
static void print_access(int a)
{
  if (a & ACC_PUBLIC) printf(" public");
  if (a & ACC_PRIVATE) printf(" private");
  if (a & ACC_PROTECTED) printf(" protected");
  if (a & ACC_STATIC) printf(" static");
  if (a & ACC_FINAL) printf(" final");
  if (a & ACC_SUPER) printf(" super");
  if (a & ACC_VOLATILE) printf(" volatile");
  if (a & ACC_TRANSIENT) printf(" transient");
  if (a & ACC_INTERFACE) printf(" interface");
  if (a & ACC_ABSTRACT) printf(" abstract");
}

void java_class_print(struct java_class_t *java_class)
{
int r;

  printf("   MagicNumber: 0x%02x%02x%02x%02x\n",((java_class->magic>>24)&0xff),
                                 ((java_class->magic>>16)&0xff),
                                 ((java_class->magic>>8)&0xff),
                                 ((java_class->magic)&0xff));
  printf("  MinorVersion: %d\n",java_class->minor_version);
  printf("  MajorVersion: %d (",java_class->major_version);
  if (java_class->major_version == 45) printf("1.1");
  else if (java_class->major_version == 46) printf("1.2");
  else if (java_class->major_version == 47) printf("1.3");
  else if (java_class->major_version == 48) printf("1.4");
  else if (java_class->major_version == 49) printf("5.0");
  else if (java_class->major_version == 50) printf("6.0");
  else { printf("???"); }

  printf(")\n");

  printf(" ConstantCount: %d\n", java_class->constant_pool_count);
  print_constant_pool(java_class);

  printf("   AccessFlags: %d", java_class->access_flags);
  print_access(java_class->access_flags);
  printf("\n");

  printf("     ThisClass: %d\n", java_class->this_class);
  printf("    SuperClass: %d\n", java_class->super_class);
  printf("InterfaceCount: %d\n", java_class->interfaces_count);
  for (r = 0; r < java_class->interfaces_count; r++)
  {
    printf("      %d) %d\n", r, java_class->interfaces[r]);
  }

  printf("    FieldCount: %d\n", java_class->fields_count);
  print_fields(java_class);
  printf("   MethodCount: %d\n", java_class->methods_count);
  print_methods(java_class);
  printf("    Attributes: %d\n", java_class->attributes_count);
  print_attributes(java_class);
}

static void print_constant_pool(struct java_class_t *java_class)
{
int count;
int tag,offset;
struct constant_class_t *constant_class;
struct constant_fieldref_t *constant_fieldref;
struct constant_methodref_t *constant_methodref;
struct constant_interfacemethodref_t *constant_interfacemethodref;
struct constant_string_t *constant_string;
struct constant_integer_t *constant_integer;
struct constant_float_t *constant_float;
struct constant_long_t *constant_long;
struct constant_double_t *constant_double;
struct constant_nameandtype_t *constant_nameandtype;
struct constant_utf8_t *constant_utf8;
void *heap;
int i;

  for (count = 1; count <= java_class->constant_pool_count; count++)
  {
    printf("   %d) ", count);

    offset = java_class->constant_pool[count];
    heap = ((void*)java_class->constants_heap)+offset;
    tag = java_class->constants_heap[offset];

    switch(tag)
    {
      case CONSTANT_FIELDREF:
        constant_fieldref = heap;
        printf("FieldRef: class_index=%d name_and_type_index=%d\n",
                constant_fieldref->class_index,
                constant_fieldref->name_and_type_index);
        break;

      case CONSTANT_METHODREF:
        constant_methodref = heap;
        printf("MethodRef: class_index=%d name_and_type_index=%d\n",
                constant_methodref->class_index,
                constant_methodref->name_and_type_index);
        break;

      case CONSTANT_INTERFACEMETHODREF:
        constant_interfacemethodref = heap;
        printf("InterfaceMethodRef: class_index=%d name_and_type_index=%d\n",
                constant_interfacemethodref->class_index,
                constant_interfacemethodref->name_and_type_index);
        break;

      case CONSTANT_INTEGER:
        constant_integer = heap;
        printf("Integer: %d\n", constant_integer->value);
        break;

      case CONSTANT_FLOAT:
        constant_float = heap;
        printf("Float: %f\n",constant_float->value);
        break;

      case CONSTANT_NAMEANDTYPE:
        constant_nameandtype = heap;
        printf("NameAndType: name_index=%d descriptor_index=%d\n",
                constant_nameandtype->name_index,
                constant_nameandtype->descriptor_index);
        break;

      case CONSTANT_CLASS:
        constant_class = heap;
        printf("Class: name_index=%d\n",
                constant_class->name_index);
        break;

      case CONSTANT_STRING:
        constant_string = heap;
        printf("String: string_index=%d\n",
                constant_string->string_index);
        break;

      case CONSTANT_LONG:
        constant_long = heap;
        printf("Long: %"PRId64"\n",constant_long->value);
        break;

      case CONSTANT_DOUBLE:
        constant_double = heap;
        printf("Double: %f\n",constant_double->value);
        break;

      case CONSTANT_UTF8:
        constant_utf8 = heap;
        printf("UTF8: ");
        for (i = 0; i < constant_utf8->length; i++)
        { printf("%c", constant_utf8->bytes[i]); }
        printf("\n");
        break;

      default:
        printf("Error: Uknown constant type\n");
        break;
    }
  }
}

static void print_attributes(struct java_class_t *java_class)
{
struct attributes_t *attribute;
int count,r;

  for (count = 0; count < java_class->attributes_count; count++)
  {
    attribute=(struct attributes_t *)(java_class->attributes_heap + java_class->attributes[count]);
    printf("                ----- %d -----\n", count);
    printf("         name_index: %d\n", attribute->attribute_name_index);
    printf("             length: %d\n", attribute->attribute_length);
    printf("               info: { ");
    for (r = 0; r < attribute->attribute_length; r++)
    {
      printf("%02x ", attribute->info[r]);
    }
    printf("}\n");
  }
}

static void print_fields(struct java_class_t *java_class)
{
struct attributes_t *attribute;
struct fields_t *field;
int count,r,n;

  for (count = 0; count < java_class->fields_count; count++)
  {
    field = (struct fields_t *)(java_class->fields_heap + java_class->fields[count]);
    printf("                ----- %d -----\n", count);
    printf("         access_flags: %d", field->access_flags);
    print_access(field->access_flags);
    printf("\n");
    printf("           name_index: %d\n", field->name_index);
    printf("     descriptor_index: %d\n", field->descriptor_index);
    printf("      attribute_count: %d\n", field->attribute_count);

    n = 8;
    for (r = 0; r < field->attribute_count; r++)
    {
      attribute=(struct attributes_t *)(java_class->fields_heap + java_class->fields[count]+n);
      printf("                ----- %d -----\n", r);
      printf("         name_index: %d\n", attribute->attribute_name_index);
      printf("             length: %d\n", attribute->attribute_length);
      printf("               info: { ");
      for (r=0; r<attribute->attribute_length; r++)
      {
        printf("%02x ",attribute->info[r]);
      }
      printf("}\n");

      n=n+6+attribute->attribute_length;
    }
  }
}

static void print_methods(struct java_class_t *java_class)
{
struct attributes_t *attribute;
struct methods_t *method;
int count,r,n;

  for (count = 0; count < java_class->methods_count; count++)
  {
    method=(struct methods_t *)(java_class->methods_heap + java_class->methods[count]);
    printf("                ----- %d -----\n", count);
    printf("         access_flags: %d", method->access_flags);
    print_access(method->access_flags);
    printf("\n");
    printf("           name_index: %d\n", method->name_index);
    printf("     descriptor_index: %d\n", method->descriptor_index);
    printf("      attribute_count: %d\n", method->attribute_count);

    n = 8;
    for (r = 0; r < method->attribute_count; r++)
    {
      attribute = (struct attributes_t *)(java_class->methods_heap + java_class->methods[count]+n);
    printf("                ----- %d -----\n", r);
      printf("         name_index: %d\n", attribute->attribute_name_index);
      printf("             length: %d\n", attribute->attribute_length);
      printf("               info: { ");
      for (r = 0; r < attribute->attribute_length; r++)
      {
        printf("%02x ", attribute->info[r]);
      }
      printf("}\n");

      n = n + 6 + attribute->attribute_length;
    }
  }
}

#endif

