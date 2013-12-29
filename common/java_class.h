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

#ifndef _JAVA_CLASS_H
#define _JAVA_CLASS_H

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

struct generic_twoint16_t
{
  unsigned char tag;
  int16_t int1;
  int16_t int2;
};

struct generic_32bit_t
{
  unsigned char tag;
  int32_t value;
};

struct generic_64bit_t
{
  unsigned char tag;
  int64_t value;
};

struct constant_class_t
{
  unsigned char tag;
  int16_t name_index;
};

struct constant_fieldref_t
{
  unsigned char tag;
  int16_t class_index;
  int16_t name_and_type_index;
};

struct constant_methodref_t
{
  unsigned char tag;
  int16_t class_index;
  int16_t name_and_type_index;
};

struct constant_interfacemethodref_t
{
  unsigned char tag;
  int16_t class_index;
  int16_t name_and_type_index;
};

struct constant_string_t
{
  unsigned char tag;
  int16_t string_index;
};

struct constant_integer_t
{
  unsigned char tag;
  int32_t value;
};

struct constant_float_t
{
  unsigned char tag;
  float value;
};

struct constant_long_t
{
  unsigned char tag;
  int64_t value;
};

struct constant_double_t
{
  unsigned char tag;
  double value;
};

struct constant_nameandtype_t
{
  unsigned char tag;
  int16_t name_index;
  int16_t descriptor_index;
};

struct constant_utf8_t
{
  unsigned char tag;
  int16_t length;
  unsigned char bytes[];
};

struct attributes_t
{
  int16_t attribute_name_index;
  int32_t attribute_length;
  unsigned char info[];
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

struct java_class_t
{
  int32_t magic;
  int16_t minor_version;
  int16_t major_version;
  int16_t constant_pool_count;
  int *constant_pool;  // len = contant_pool_count - 1 (points in the heap)
  int16_t access_flags;
  int16_t this_class;
  int16_t super_class;
  int16_t interfaces_count;
  int *interfaces;
  int16_t fields_count;
  int *fields;
  int16_t methods_count;
  int *methods;
  int16_t attributes_count;
  int *attributes;

  // Extra Crap (these should be voids *'s really.. it was easier to make
  //             them unsigned char * at first)
  unsigned char *constants_heap;
  unsigned char *fields_heap;
  unsigned char *methods_heap;
  unsigned char *attributes_heap;
};

struct java_class_t *java_class_read(FILE *in);
void java_class_print(struct java_class_t *java_class);
void java_class_free(struct java_class_t *java_class);

#endif

