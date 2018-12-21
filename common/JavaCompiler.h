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

#ifndef _JAVA_COMPILER_H
#define _JAVA_COMPILER_H

#include <map>
#include <string>

#include "Compiler.h"
#include "Generator.h"
#include "JavaClass.h"
#include "stack.h"

#define GET_PC_INT16(a) ((int16_t)(((uint16_t)bytes[pc+a+0])<<8|bytes[pc+a+1]))
#define GET_PC_UINT16(a) (((uint16_t)bytes[pc+a+0])<<8|bytes[pc+a+1])
#define GET_PC_INT32(a) ((int32_t)((uint32_t)bytes[pc+a+0])<<24| \
                        ((uint32_t)bytes[pc+a+1])<<16|\
                        ((uint32_t)bytes[pc+a+2])<<8|\
                         bytes[pc+a+3])
#define GET_PC_UINT32(a) (((uint32_t)bytes[pc+a+0])<<24| \
                         ((uint32_t)bytes[pc+a+1])<<16|\
                         ((uint32_t)bytes[pc+a+2])<<8|\
                          bytes[pc+a+3])

class JavaCompiler : public Compiler
{
public:
  JavaCompiler();
  virtual ~JavaCompiler();

  virtual int load_class(const char *filename);
  virtual void insert_static_field_defines();
  virtual void init_heap();
  virtual int add_static_initializers();
  virtual int compile_methods(bool do_main);
  virtual int add_constants(JavaClass *java_class);
  virtual int add_constants();

private:
  int find_external_fields(JavaClass *java_class, bool is_parent);
  void fill_label_map(uint8_t *label_map, int label_map_len, uint8_t *bytes, int code_len, int pc_start);
  int optimize_const(JavaClass *java_class, char *method_name, uint8_t *bytes, int pc, int pc_end, int address, int const_val);
  int optimize_const(JavaClass *java_class, char *method_name, uint8_t *bytes, int pc, int pc_end, int address, const char *const_val);
  int optimize_compare(JavaClass *java_class, char *method_name, uint8_t *bytes, int pc, int pc_end, int address, int index);
  int array_load(JavaClass *java_class, int constant_id, uint8_t array_type);
  int array_store(JavaClass *java_class, int constant_id, uint8_t array_type);
  int push_ref(int index, _stack *stack);
  int compile_method(JavaClass *java_class, int method_id, const char *alt_name = NULL);
  int field_type_to_int(const char *field_type);
  const char *field_type_from_int(int type);
  int execute_statics(int index);
  int get_const(uint8_t *bytes, int len, int pc, int *value);
  int get_cond(uint8_t *bytes, int len, int pc, int *cond, int *label);
  int try_ternary(uint8_t *bytes, int len, int pc, bool compare_with_value, int compare);

  JavaClass *java_class;  // FIXME - Why is this here?
  char classpath[128];
  std::map<std::string, int> external_fields;
  std::map<std::string, JavaClass *> external_classes;
  FILE *in;
  static uint8_t cond_table[];
  static const char *type_table[];
};

#endif

