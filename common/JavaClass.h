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

#ifndef _JAVA_CLASS_H
#define _JAVA_CLASS_H

#include <stdint.h>
#include <map>

// http://java.sun.com/docs/books/jvms/second_edition/html/ClassFile.doc.html
// http://www.brics.dk/~mis/dOvs/jvmspec/ref-Java.html
// http://java.sun.com/docs/books/jvms/second_edition/html/Mnemonics.doc.html
// http://java.sun.com/docs/books/jvms/second_edition/html/ClassFile.doc.html#1546

#define ACC_PUBLIC 0x0001
#define ACC_PRIVATE 0x0002
#define ACC_PROTECTED 0x0004
#define ACC_STATIC 0x0008
#define ACC_FINAL 0x0010
#define ACC_SUPER 0x0020
#define ACC_VOLATILE 0x0040
#define ACC_TRANSIENT 0x0080
#define ACC_INTERFACE 0x0200
#define ACC_ABSTRACT 0x0400

#define CONSTANT_CLASS 7
#define CONSTANT_FIELDREF 9
#define CONSTANT_METHODREF 10
#define CONSTANT_INTERFACEMETHODREF 11
#define CONSTANT_STRING 8
#define CONSTANT_INTEGER 3
#define CONSTANT_FLOAT 4
#define CONSTANT_LONG 5
#define CONSTANT_DOUBLE 6
#define CONSTANT_NAMEANDTYPE 12
#define CONSTANT_UTF8 1

#define JAVA_TYPE_INTEGER 0
#define JAVA_TYPE_LONG 1
#define JAVA_TYPE_FLOAT 2
#define JAVA_TYPE_DOUBLE 3
#define JAVA_TYPE_REF 4

#define ARRAY_TYPE_BOOLEAN 4
#define ARRAY_TYPE_CHAR 5
#define ARRAY_TYPE_FLOAT 6
#define ARRAY_TYPE_DOUBLE 7
#define ARRAY_TYPE_BYTE 8
#define ARRAY_TYPE_SHORT 9
#define ARRAY_TYPE_INT 10
#define ARRAY_TYPE_LONG 11
#define ARRAY_TYPE_OBJECT 100

struct generic_twoint16_t
{
  uint8_t tag;
  int16_t int1;
  int16_t int2;
};

struct generic_32bit_t
{
  uint8_t tag;
  int32_t value;
};

struct generic_64bit_t
{
  uint8_t tag;
  int64_t value;
};

struct constant_class_t
{
  uint8_t tag;
  int16_t name_index;
};

struct constant_fieldref_t
{
  uint8_t tag;
  int16_t class_index;
  int16_t name_and_type_index;
};

struct constant_methodref_t
{
  uint8_t tag;
  int16_t class_index;
  int16_t name_and_type_index;
};

struct constant_interfacemethodref_t
{
  uint8_t tag;
  int16_t class_index;
  int16_t name_and_type_index;
};

struct constant_string_t
{
  uint8_t tag;
  int16_t string_index;
};

struct constant_integer_t
{
  uint8_t tag;
  int32_t value;
};

struct constant_float_t
{
  uint8_t tag;
  float value;
};

struct constant_long_t
{
  uint8_t tag;
  int64_t value;
};

struct constant_double_t
{
  uint8_t tag;
  double value;
};

struct constant_nameandtype_t
{
  uint8_t tag;
  int16_t name_index;
  int16_t descriptor_index;
};

struct constant_utf8_t
{
  uint8_t tag;
  int16_t length;
  uint8_t bytes[];
};

struct attributes_t
{
  int16_t name_index;
  int32_t length;
  uint8_t info[];
};

struct fields_t
{
  int16_t access_flags;
  int16_t name_index;
  int16_t descriptor_index;
  int16_t attribute_count;
  struct attributes_t attributes[];
};

struct methods_t
{
  int16_t access_flags;
  int16_t name_index;
  int16_t descriptor_index;
  int16_t attribute_count;
  struct attributes_t attributes[];
};

class JavaClass
{
public:
  JavaClass(FILE *in, bool is_main_class=true);
  ~JavaClass();
  void print();
  int get_name_constant(char *name, int len, int index);
  int get_method_name(char *name, int len, int index);
  int get_field_name(char *name, int len, int index);
  int get_field_type(char *name, int len, int index);
  const fields_t *get_field(int index);
  int get_ref_name_type(char *name, char *type, int len, int index);
  bool is_ref_in_api(int index);
  int get_class_name(char *name, int len, int index);
  void *get_constant(int index);
  struct methods_t *get_method(int index);
  int get_method_count() { return methods_count; }
  int get_field_count() { return fields_count; }
  int get_constant_count() { return constant_pool_count; }
  int get_field_index(const char *field_name);
  int get_clinit_method();
  static const char *tag_as_string(int tag);
  bool use_full_method_name() { return is_main_class == false; }

  int32_t magic;
  int16_t minor_version;
  int16_t major_version;
  int16_t access_flags;
  int16_t this_class;
  int16_t super_class;

  char class_name[128];

  // Keep track of constants that need to be defined.
  std::map<int,int> needed_constants;

private:
  void read_attributes(FILE *in);
  void read_fields(FILE *in);
  void read_methods(FILE *in);
  void read_constant_pool(FILE *in);
#ifdef DEBUG
  void print_access(int a);
  void print_constant_pool();
  void print_attributes();
  void print_fields();
  void print_methods();
#endif

  // Counts of fields, methods, etc.
  uint16_t constant_pool_count;
  uint16_t interfaces_count;
  uint16_t fields_count;
  uint16_t methods_count;
  uint16_t attributes_count;

  // Indexed pointers into the heaps.
  int *constant_pool;  // len = contant_pool_count - 1 (points in the heap)
  uint16_t *interfaces;
  int *fields;
  int *methods;
  int *attributes;

  // Where all the data is stored.
  uint8_t *constants_heap;
  uint8_t *fields_heap;
  uint8_t *methods_heap;
  uint8_t *attributes_heap;

  bool is_main_class : 1;
};

#endif

