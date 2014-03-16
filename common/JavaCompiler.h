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

#ifndef _COMPILE_H
#define _COMPILE_H

#include "Generator.h"
#include "JavaClass.h"

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

class JavaCompiler
{
public:
  JavaCompiler();
  ~JavaCompiler();

  void disable_optimizer() { optimize = false; }
  void set_generator(Generator *generator) { this->generator = generator; }
  int load_class(FILE *in);
  void insert_static_field_defines();
  void init_heap();
  int add_static_initializers();
  int compile_methods(bool do_main);
  JavaClass *get_java_class() { return java_class; }

private:
  void fill_label_map(uint8_t *label_map, int label_map_len, uint8_t *bytes, int code_len, int pc_start);
  int optimize_const(JavaClass *java_class, char *method_name, uint8_t *bytes, int pc, int pc_end, int address, int const_val);
  int array_load(JavaClass *java_class, int constant_id, uint8_t array_type);
  int array_store(JavaClass *java_class, int constant_id, uint8_t array_type);
  int compile_method(JavaClass *java_class, int method_id);

  Generator *generator;
  JavaClass *java_class;
  static uint8_t cond_table[];
  bool optimize;
};

#endif

