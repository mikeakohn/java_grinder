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

#define __STDC_FORMAT_MACROS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#include "fileio.h"
#include "JavaClass.h"

JavaClass::JavaClass(FILE *in, bool is_main_class) :
  constant_pool(NULL),
  interfaces(NULL),
  fields(NULL),
  methods(NULL),
  attributes(NULL),
  constants_heap(NULL),
  fields_heap(NULL),
  methods_heap(NULL),
  attributes_heap(NULL),
  is_main_class(is_main_class)
{
  int t;

  //memset(java_class, 0, sizeof(struct java_class_t));

  magic = read_int32(in);
  minor_version = read_int16(in);
  major_version = read_int16(in);

  constant_pool_count = read_int16(in);
  if (constant_pool_count != 0)
  {
    constant_pool = (int *)malloc((constant_pool_count + 1) * sizeof(int));
    memset(constant_pool, 0, (constant_pool_count + 1) * sizeof(int));
    read_constant_pool(in);
  }

  access_flags = read_int16(in);
  this_class = read_int16(in);
  super_class = read_int16(in);
  interfaces_count = read_int16(in);

  if (interfaces_count != 0)
  {
    interfaces = (uint16_t *)malloc(interfaces_count * sizeof(uint16_t));
    for (t = 0; t < interfaces_count; t++)
    {
      interfaces[t] = read_int16(in);
    }
  }

  fields_count = read_int16(in);
  if (fields_count != 0)
  {
    fields = (int *)malloc(fields_count * sizeof(int));
    memset(fields, 0, fields_count * sizeof(int));
    read_fields(in);
  }

  methods_count = read_int16(in);
  if (methods_count != 0)
  {
    methods = (int *)malloc(methods_count * sizeof(int));
    memset(methods, 0, methods_count * sizeof(int));
    read_methods(in);
  }

  attributes_count = read_int16(in);
  if (attributes_count != 0)
  {
    attributes = (int *)malloc(attributes_count * sizeof(int));
    memset(attributes, 0, attributes_count * sizeof(int));
    read_attributes(in);
  }

  get_class_name(class_name, sizeof(class_name), this_class);
}

JavaClass::~JavaClass()
{
  if (constant_pool != NULL) { free(constant_pool); }
  if (interfaces != NULL) { free(interfaces); }
  if (fields != NULL) { free(fields); }
  if (methods != NULL) { free(methods); }
  if (attributes != NULL) { free(attributes); }

  if (constants_heap != NULL) { free(constants_heap); }
  if (fields_heap != NULL) { free(fields_heap); }
  if (methods_heap != NULL) { free(methods_heap); }
  if (attributes_heap != NULL) { free(attributes_heap); }
}

void JavaClass::read_attributes(FILE *in)
{
  long marker;
  int count;
  int len = 0;
  int l;

  marker = ftell(in);

  for (count = 0; count < attributes_count; count++)
  {
    //attributes[count] = ftell(in) - marker;
    attributes[count] = len;
    fseek(in, 2, SEEK_CUR);
    l=read_int32(in);
    fseek(in, l, SEEK_CUR);
    len += sizeof(struct attributes_t) + l;
  }

  //len = ftell(in) - marker;
  attributes_heap = (uint8_t *)malloc(len);
  fseek(in, marker, SEEK_SET);

  struct attributes_t *attribute;
  for (count = 0; count < attributes_count; count++)
  {
    attribute = (struct attributes_t *)(attributes_heap + attributes[count]);
    attribute->name_index = read_int16(in);
    attribute->length = read_int32(in);
    if (fread(attribute->info, 1, attribute->length, in)) {}
  }
}

void JavaClass::read_fields(FILE *in)
{
  long marker;
  int count;
  int len = 0;
  int n,l,r;

  marker = ftell(in);

  for (count = 0; count < fields_count; count++)
  {
    //fields[count] = ftell(in) - marker;
    fields[count] = len;
    fseek(in, 6, SEEK_CUR);
    n = read_int16(in);
    len += sizeof(struct fields_t);
    for (r = 0; r < n; r++)
    {
      fseek(in, 2, SEEK_CUR);
      l = read_int32(in);
      len += sizeof(struct attributes_t) + l;
      fseek(in, l, SEEK_CUR);
    }
  }

  //len = ftell(in) - marker;
  fields_heap = (uint8_t *)malloc(len);
  fseek(in, marker, SEEK_SET);

  struct attributes_t *attribute;
  struct fields_t *field;
  for (count = 0; count < fields_count; count++)
  {
    field = (struct fields_t *)(fields_heap + fields[count]);
    field->access_flags = read_int16(in);
    field->name_index = read_int16(in);
    field->descriptor_index = read_int16(in);
    field->attribute_count = read_int16(in);
    n = sizeof(struct fields_t);
    for (r = 0; r < field->attribute_count; r++)
    {
      attribute = (struct attributes_t *)(fields_heap + fields[count]+n);
      attribute->name_index = read_int16(in);
      attribute->length = read_int32(in);
      if (fread(attribute->info, 1, attribute->length, in)) {}
      n = n + 6 + attribute->length;
    }
  }
}

void JavaClass::read_methods(FILE *in)
{
  long marker;
  int count;
  int len = 0;
  int n,l,r;

  marker = ftell(in);
  //int attribute_count = 0;

  // Compute how much memory to malloc()
  for (count = 0; count < methods_count; count++)
  {
    //methods[count] = ftell(in) - marker;
    methods[count] = len;
    fseek(in, 6, SEEK_CUR);   // sizeof struct methods_t
    n = read_int16(in);       // attribute count
    len += sizeof(struct methods_t);
    for (r = 0; r < n; r++)
    {
      fseek(in, 2, SEEK_CUR); // attribute name
      l = read_int32(in);     // attribute len
      len += sizeof(struct attributes_t) + l;
      fseek(in, l, SEEK_CUR); // attribute data
      //attribute_count++;
    }
  }

  //len = ftell(in) - marker;
  //len += attribute_count * 2;
  methods_heap = (uint8_t *)malloc(len);
  fseek(in, marker, SEEK_SET);

  struct attributes_t *attribute;
  struct methods_t *method;
  for (count = 0; count < methods_count; count++)
  {
    method = (struct methods_t *)(methods_heap + methods[count]);
    method->access_flags = read_int16(in);
    method->name_index = read_int16(in);
    method->descriptor_index = read_int16(in);
    method->attribute_count = read_int16(in);
    n = sizeof(struct methods_t);
    for (r = 0; r < method->attribute_count; r++)
    {
      attribute = (struct attributes_t *)(methods_heap + methods[count] + n);
      attribute->name_index = read_int16(in);
      attribute->length = read_int32(in);
      if (fread(attribute->info, 1, attribute->length, in)) {}
      n = n + 6 + attribute->length;
    }
  }
}

void JavaClass::read_constant_pool(FILE *in)
{
  long marker;
  int len,count;
  int ch;

  marker = ftell(in);
  len = 0;

  for (count = 0; count < constant_pool_count - 1; count++)
  {
    constant_pool[count+1] = len;

    ch = getc(in);

    switch(ch)
    {
      case CONSTANT_FIELDREF:
      case CONSTANT_METHODREF:
      case CONSTANT_INTERFACEMETHODREF:
      case CONSTANT_NAMEANDTYPE:
        fseek(in, 4, SEEK_CUR);
        len += sizeof(struct generic_twoint16_t);
        break;

      case CONSTANT_INTEGER:
      case CONSTANT_FLOAT:
        fseek(in, 4, SEEK_CUR);
        len += sizeof(struct generic_32bit_t);
        break;

      case CONSTANT_CLASS:
      case CONSTANT_STRING:
        fseek(in, 2, SEEK_CUR);
        len += sizeof(struct constant_class_t);
        break;

      case CONSTANT_LONG:
      case CONSTANT_DOUBLE:
        fseek(in, 8, SEEK_CUR);
        len += sizeof(struct constant_double_t);
        break;

      case CONSTANT_UTF8:
        ch = read_int16(in);
        fseek(in, ch, SEEK_CUR);
        len += sizeof(struct constant_utf8_t) + ch;
        break;

      default:
        printf("Error: Unknown constant type %d (please email author)\n", ch);
        exit(1);
        break;
    }
  }

  constants_heap = (uint8_t *)malloc(len);
  fseek(in, marker, SEEK_SET);

  void *constant;
  struct generic_twoint16_t *gen2int;
  struct generic_32bit_t *gen32bit;
  struct generic_64bit_t *gen64bit;
  struct constant_utf8_t *utf8;
  struct constant_class_t *cls;

  for (count = 0; count < constant_pool_count - 1; count++)
  {
    constant = constants_heap + constant_pool[count+1];

    ch = getc(in);

    switch(ch)
    {
      case CONSTANT_FIELDREF:
      case CONSTANT_METHODREF:
      case CONSTANT_INTERFACEMETHODREF:
      case CONSTANT_NAMEANDTYPE:
        gen2int = (struct generic_twoint16_t *)constant;
        gen2int->tag = ch;
        gen2int->int1 = read_int16(in);
        gen2int->int2 = read_int16(in);
        break;

      case CONSTANT_INTEGER:
      case CONSTANT_FLOAT:
        gen32bit = (struct generic_32bit_t *)constant;
        gen32bit->tag = ch;
        gen32bit->value = read_int32(in);
        break;

      case CONSTANT_CLASS:
      case CONSTANT_STRING:
        cls = (struct constant_class_t *)constant;
        cls->tag = ch;
        cls->name_index = read_int16(in);
        break;

      case CONSTANT_LONG:
      case CONSTANT_DOUBLE:
        gen64bit = (struct generic_64bit_t *)constant;
        gen64bit->tag = ch;
        gen64bit->value = read_int64(in);
        break;

      case CONSTANT_UTF8:
        utf8 = (struct constant_utf8_t *)constant;
        utf8->tag = ch;
        utf8->length = read_int16(in);
        if (fread(utf8->bytes, 1, utf8->length, in)) {}
        break;

      default:
        printf("Error: Unknown constant type\n");
        break;
    }
  }
}

/* In the movie the 6th Sense, the character Bruce Willis plays is
 * already dead and no one can see him except the little boy who
 * can see dead people.  That's what you get for reading my source
 * code!  :)  */

int JavaClass::get_name_constant(char *name, int len, int index)
{
  struct constant_utf8_t *constant_utf8;
  int tag,offset;
  void *heap;

  name[0] = 0;
  if (index > constant_pool_count) { return -1; }

  offset = constant_pool[index];
  heap = (void *)(((uint8_t *)constants_heap) + offset);
  tag = constants_heap[offset];

  if (tag != CONSTANT_UTF8) { return -1; }

  constant_utf8 = (constant_utf8_t *)heap;
  if (len < constant_utf8->length - 1) { return -1; }
  memcpy(name, constant_utf8->bytes, constant_utf8->length);
  name[constant_utf8->length] = 0;

  return 0;
}

int JavaClass::get_method_name(char *name, int len, int index)
{
  struct methods_t *method;

  name[0] = 0;
  if (index >= methods_count) { return -1; }

  method = (struct methods_t *)(methods_heap + methods[index]);
  get_name_constant(name, len, method->name_index);

  return 0;
}

int JavaClass::get_field_name(char *name, int len, int index)
{
  struct fields_t *field;

  name[0] = 0;
  if (index >= fields_count) { return -1; }

  field = (struct fields_t *)(fields_heap + fields[index]);
  get_name_constant(name, len, field->name_index);

  return 0;
}

int JavaClass::get_field_type(char *type, int len, int index)
{
  struct fields_t *field;

  type[0] = 0;
  if (index >= fields_count) { return -1; }

  field = (struct fields_t *)(fields_heap + fields[index]);
  get_name_constant(type, len, field->descriptor_index);

  return 0;
}

const fields_t *JavaClass::get_field(int index)
{
  struct fields_t *field;

  if (index >= fields_count) { return NULL; }

  field = (struct fields_t *)(fields_heap + fields[index]);

  return field;
}

int JavaClass::get_ref_name_type(char *name, char *type, int len, int index)
{
  struct constant_fieldref_t *constant_fieldref;
  struct constant_methodref_t *constant_methodref;
  struct constant_nameandtype_t *constant_nameandtype;
  int tag,offset;
  void *heap;

  name[0] = 0;
  type[0] = 0;

  while(1)
  {
    if (index > constant_pool_count) { return -1; }
 
    offset = constant_pool[index];
    heap = (void *)(((uint8_t *)constants_heap) + offset);
    tag = constants_heap[offset];

    if (tag == CONSTANT_FIELDREF)
    {
      constant_fieldref = (constant_fieldref_t *)heap;
      index = constant_fieldref->name_and_type_index;

      // If the constant doesn't exist in this class, then add the class
      // name to the field.
      if (constant_fieldref->class_index != this_class ||
          use_full_method_name())
      {
        get_class_name(name, len, constant_fieldref->class_index);
        //printf("  class_name='%s' %d\n", name, constant_fieldref->class_index);
        strcat(name, "_");
      }
    }
      else
    if (tag == CONSTANT_METHODREF)
    {
      constant_methodref = (constant_methodref_t *)heap;
      index = constant_methodref->name_and_type_index;

      // If the method doesn't exist in this class, then add the class
      // name to the field.
      if (constant_methodref->class_index != this_class ||
          use_full_method_name())
      {
        get_class_name(name, len, constant_methodref->class_index);
        //printf("  class_name='%s' %d\n", name, constant_methodref->class_index);
        strcat(name, "_");

        // Is this needed?
        if (strncmp(name, "java/", 5) == 0 ||
            strncmp(name, "net/mikekohn/java_grinder/",
                   sizeof("net/mikekohn/java_grinder/") -1) == 0)
        {
          name[0] = 0;
        }
      }
    }
      else
    if (tag == CONSTANT_NAMEANDTYPE)
    {
      constant_nameandtype = (constant_nameandtype_t *)heap;
      int class_name_len = strlen(name);
      get_name_constant(name + class_name_len, len - class_name_len, constant_nameandtype->name_index);
      get_name_constant(type, len, constant_nameandtype->descriptor_index);
      return 0;
    }
      else
    {
      break;
    }
  }

  return -1;
}

bool JavaClass::is_ref_in_api(int index)
{
  char name[128];

  if (get_class_name(name, sizeof(name), index) == -1) { return true; }

  if (strncmp(name, "java/", 5) == 0) { return true; }
  if (strncmp(name, "net/mikekohn/java_grinder/",
             sizeof("net/mikekohn/java_grinder/") -1) == 0)
  {
    return true;
  }

  return false;
}

int JavaClass::get_class_name(char *name, int len, int index)
{
  struct constant_fieldref_t *constant_fieldref;
  struct constant_methodref_t *constant_methodref;
  struct constant_class_t *constant_class;
  int tag,offset;
  void *heap;

  name[0] = 0;

  while(1)
  {
    if (index > constant_pool_count) { return -1; }
 
    offset = constant_pool[index];
    heap = (void *)(((uint8_t *)constants_heap) + offset);
    tag = constants_heap[offset];

    if (tag == CONSTANT_FIELDREF)
    {
      constant_fieldref = (constant_fieldref_t *)heap;
      index = constant_fieldref->class_index;
    }
      else
    if (tag == CONSTANT_METHODREF)
    {
      constant_methodref = (constant_methodref_t *)heap;
      index = constant_methodref->class_index;
    }
      else
    if (tag == CONSTANT_CLASS)
    {
      constant_class = (constant_class_t *)heap;
      get_name_constant(name, len, constant_class->name_index);
      return 0;
    }
      else
    {
      break;
    }
  }

  return -1;
}

int JavaClass::get_field_index(const char *field_name)
{
  struct attributes_t *attribute;
  struct fields_t *field;
  int count,r,n;
  char name[256];

  //printf("----- FieldCount: %d\n", fields_count);

  for (count = 0; count < fields_count; count++)
  {
    field = (struct fields_t *)(fields_heap + fields[count]);
    get_name_constant(name, sizeof(name), field->name_index);
    if (strcmp(field_name, name) == 0) { return count; }
    //get_name_constant(desc, sizeof(desc), field->descriptor_index);
    //printf("                ----- %d -----\n", count);
    //printf("         access_flags: %d", field->access_flags);
    //print_access(field->access_flags);
    //printf("\n");
    //printf("           name_index: %d (%s)\n", field->name_index, name);
    //printf("     descriptor_index: %d (%s)\n", field->descriptor_index, desc);
    //printf("      attribute_count: %d\n", field->attribute_count);

    n = 8;
    for (r = 0; r < field->attribute_count; r++)
    {
      attribute=(struct attributes_t *)(fields_heap + fields[count]+n);
      //printf("                ----- attr %d -----\n", r);
      get_name_constant(name, sizeof(name), attribute->name_index);
      //printf("         name_index: %d (%s)\n", attribute->name_index, name);
      //printf("             length: %d\n", attribute->length);
      //printf("               info: { ");
      //for (r=0; r<attribute->length; r++)
      //{
      //  printf("%02x ",attribute->info[r]);
      //}
      //printf("}\n");

      n=n+6+attribute->length;
    }
  }

  return -1;
}

int JavaClass::get_clinit_method()
{
  int method_count = get_method_count();
  char method_name[128];
  int index;

  for (index = 0; index < method_count; index++)
  {
    if (get_method_name(method_name, sizeof(method_name), index) == 0)
    {
      if (strcmp("<clinit>", method_name) == 0)
      {
        return index;
      }
    }
  }

  return -1;
}

const char *JavaClass::tag_as_string(int tag)
{
  const char *tags[] =
  {
    "???",
    "UTF8",               // 1
    "???",
    "INTEGER",            // 3
    "FLOAT",              // 4
    "LONG",               // 5
    "DOUBLE",             // 6
    "CLASS",              // 7
    "STRING",             // 8
    "FIELDREF",           // 9
    "METHODREF",          // 10
    "INTERFACEMETHODREF", // 11
    "NAMEANDTYPE",        // 12
  };

  if (tag < 0 || tag > 12) { return "???"; }
  return tags[tag];
}

void *JavaClass::get_constant(int index)
{
  return constants_heap + constant_pool[index];
}

struct methods_t *JavaClass::get_method(int index)
{
  return (struct methods_t *)(methods_heap + methods[index]);
}

#ifdef DEBUG
void JavaClass::print_access(int a)
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

void JavaClass::print()
{
  char name[128];
  int r;

  printf("   MagicNumber: 0x%02x%02x%02x%02x\n",((magic>>24)&0xff),
                                 ((magic>>16)&0xff),
                                 ((magic>>8)&0xff),
                                 ((magic)&0xff));
  printf("  MinorVersion: %d\n",minor_version);
  printf("  MajorVersion: %d (",major_version);
  if (major_version == 45) printf("1.1");
  else if (major_version == 46) printf("1.2");
  else if (major_version == 47) printf("1.3");
  else if (major_version == 48) printf("1.4");
  else if (major_version == 49) printf("5.0");
  else if (major_version == 50) printf("6.0");
  else if (major_version == 51) printf("7");
  else { printf("???"); }

  printf(")\n");

  print_constant_pool();

  printf("   AccessFlags: %d", access_flags);
  print_access(access_flags);
  printf("\n");

  //get_class_name(name, sizeof(name), this_class);
  printf("     ThisClass: %s (%d)\n", class_name, this_class);
  get_class_name(name, sizeof(name), super_class);
  printf("    SuperClass: %s (%d)\n", name, super_class);
  printf("InterfaceCount: %d\n", interfaces_count);
  for (r = 0; r < interfaces_count; r++)
  {
    printf("      %d) %d\n", r, interfaces[r]);
  }

  print_fields();
  print_methods();
  print_attributes();
}

void JavaClass::print_constant_pool()
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

  printf("----- ConstantCount: %d\n", constant_pool_count);

  for (count = 1; count <= constant_pool_count; count++)
  {
    printf("   %d) ", count);

    offset = constant_pool[count];
    heap = (void *)(((uint8_t *)constants_heap) + offset);
    tag = constants_heap[offset];

    switch(tag)
    {
      case CONSTANT_FIELDREF:
        constant_fieldref = (constant_fieldref_t *)heap;
        printf("FieldRef: class_index=%d name_and_type_index=%d\n",
                constant_fieldref->class_index,
                constant_fieldref->name_and_type_index);
        break;

      case CONSTANT_METHODREF:
        constant_methodref = (constant_methodref_t *)heap;
        printf("MethodRef: class_index=%d name_and_type_index=%d\n",
                constant_methodref->class_index,
                constant_methodref->name_and_type_index);
        break;

      case CONSTANT_INTERFACEMETHODREF:
        constant_interfacemethodref = (constant_interfacemethodref_t *)heap;
        printf("InterfaceMethodRef: class_index=%d name_and_type_index=%d\n",
                constant_interfacemethodref->class_index,
                constant_interfacemethodref->name_and_type_index);
        break;

      case CONSTANT_INTEGER:
        constant_integer = (constant_integer_t *)heap;
        printf("Integer: %d\n", constant_integer->value);
        break;

      case CONSTANT_FLOAT:
        constant_float = (constant_float_t *)heap;
        printf("Float: %f\n",constant_float->value);
        break;

      case CONSTANT_NAMEANDTYPE:
        constant_nameandtype = (constant_nameandtype_t *)heap;
        printf("NameAndType: name_index=%d descriptor_index=%d\n",
                constant_nameandtype->name_index,
                constant_nameandtype->descriptor_index);
        break;

      case CONSTANT_CLASS:
        constant_class = (constant_class_t *)heap;
        printf("Class: name_index=%d\n",
                constant_class->name_index);
        break;

      case CONSTANT_STRING:
        constant_string = (constant_string_t *)heap;
        printf("String: string_index=%d\n",
                constant_string->string_index);
        break;

      case CONSTANT_LONG:
        constant_long = (constant_long_t *)heap;
        printf("Long: %" PRId64 "\n",constant_long->value);
        //printf("Long: %lld\n",constant_long->value);
        break;

      case CONSTANT_DOUBLE:
        constant_double = (constant_double_t *)heap;
        printf("Double: %f\n",constant_double->value);
        break;

      case CONSTANT_UTF8:
        constant_utf8 = (constant_utf8_t *)heap;
        printf("UTF8: ");
        for (i = 0; i < constant_utf8->length; i++)
        { printf("%c", constant_utf8->bytes[i]); }
        printf("\n");
        break;

      default:
        printf("Error: Unknown constant type\n");
        break;
    }
  }
}

void JavaClass::print_attributes()
{
  struct attributes_t *attribute;
  int count,r;
  char name[256];
  
  printf("----- Attributes: %d\n", attributes_count);

  for (count = 0; count < attributes_count; count++)
  {
    attribute=(struct attributes_t *)(attributes_heap + attributes[count]);
    get_name_constant(name, sizeof(name), attribute->name_index);
    printf("                ----- %d -----\n", count);
    printf("         name_index: %d (%s)\n", attribute->name_index, name);
    printf("             length: %d\n", attribute->length);
    printf("               info: { ");
    for (r = 0; r < attribute->length; r++)
    {
      printf("%02x ", attribute->info[r]);
    }
    printf("}\n");
  }
}

void JavaClass::print_fields()
{
  struct attributes_t *attribute;
  struct fields_t *field;
  int count,r,n;
  char name[256];
  char desc[256];

  printf("----- FieldCount: %d\n", fields_count);

  for (count = 0; count < fields_count; count++)
  {
    field = (struct fields_t *)(fields_heap + fields[count]);
    get_name_constant(name, sizeof(name), field->name_index);
    get_name_constant(desc, sizeof(desc), field->descriptor_index);
    printf("                ----- %d -----\n", count);
    printf("         access_flags: %d", field->access_flags);
    print_access(field->access_flags);
    printf("\n");
    printf("           name_index: %d (%s)\n", field->name_index, name);
    printf("     descriptor_index: %d (%s)\n", field->descriptor_index, desc);
    printf("      attribute_count: %d\n", field->attribute_count);

    n = 8;
    for (r = 0; r < field->attribute_count; r++)
    {
      attribute=(struct attributes_t *)(fields_heap + fields[count]+n);
      printf("                ----- attr %d -----\n", r);
      get_name_constant(name, sizeof(name), attribute->name_index);
      printf("         name_index: %d (%s)\n", attribute->name_index, name);
      printf("             length: %d\n", attribute->length);
      printf("               info: { ");
      for (r=0; r<attribute->length; r++)
      {
        printf("%02x ",attribute->info[r]);
      }
      printf("}\n");

      n=n+6+attribute->length;
    }
  }
}

void JavaClass::print_methods()
{
  struct attributes_t *attribute;
  struct methods_t *method;
  int count,r,n;
  char name[256];
  char desc[256];

  printf("----- MethodCount: %d\n", methods_count);

  for (count = 0; count < methods_count; count++)
  {
    method = (struct methods_t *)(methods_heap + methods[count]);
    get_name_constant(name, sizeof(name), method->name_index);
    get_name_constant(desc, sizeof(desc), method->descriptor_index);
    printf("                ----- %d -----\n", count);
    printf("         access_flags: %d", method->access_flags);
    print_access(method->access_flags);
    printf("\n");
    printf("           name_index: %d (%s)\n", method->name_index, name);
    printf("     descriptor_index: %d (%s)\n", method->descriptor_index, desc);
    printf("      attribute_count: %d\n", method->attribute_count);

    n = 8;
    for (r = 0; r < method->attribute_count; r++)
    {
      attribute = (struct attributes_t *)(methods_heap + methods[count] + n);
      printf("                ----- attr %d -----\n", r);
      get_name_constant(name, sizeof(name), attribute->name_index);
      printf("         name_index: %d (%s)\n", attribute->name_index, name);
      printf("             length: %d\n", attribute->length);
      printf("               info: { ");
      for (r = 0; r < attribute->length; r++)
      {
        printf("%02x ", attribute->info[r]);
      }
      printf("}\n");

      n = n + 6 + attribute->length;
    }
  }
}

#endif

