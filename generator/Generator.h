/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2021 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_GENERATOR_H
#define JAVA_GRINDER_GENERATOR_GENERATOR_H

#include <stdio.h>
#include <stdint.h>

#include <map>
#include <string>

#include "generator/API_Amiga.h"
#include "generator/API_APPLEIIGS.h"
#include "generator/API_Atari2600.h"
#include "generator/API_C64.h"
#include "generator/API_CPC.h"
#include "generator/API_DSP.h"
#include "generator/API_Draw3D.h"
#include "generator/API_Intellivision.h"
#include "generator/API_Joystick.h"
#include "generator/API_Math.h"
#include "generator/API_Microcontroller.h"
#include "generator/API_MSX.h"
#include "generator/API_NES.h"
#include "generator/API_Parallella.h"
#include "generator/API_Playstation2.h"
#include "generator/API_Propeller.h"
#include "generator/API_SegaGenesis.h"
#include "generator/API_SNES.h"
#include "generator/API_System.h"
#include "generator/API_SXB.h"
#include "generator/API_TI84.h"
#include "generator/API_TI99.h"
#include "generator/API_TRS80_Coco.h"

class Generator :
  public API_Amiga,
  public API_AppleIIgs,
  public API_Atari2600,
  public API_C64,
  public API_CPC,
  public API_DSP,
  public API_Draw3D,
  public API_Intellivision,
  public API_Joystick,
  public API_Math,
  public API_Microcontroller,
  public API_MSX,
  public API_NES,
  public API_Parallella,
  public API_Playstation2,
  public API_Propeller,
  public API_SegaGenesis,
  public API_SNES,
  public API_SXB,
  public API_System,
  public API_TI84,
  public API_TI99,
  public API_TRS80_Coco
{
public:
  Generator();
  virtual ~Generator();

  virtual int open(const char *filename);
  void close();
  virtual int finish() { return 0; }
  virtual int get_cpu_byte_alignment() { return 2; }
  void label(std::string &name);
  virtual int start_init() = 0;
  virtual int insert_static_field_define(std::string &name, std::string &type, int index) = 0;
  virtual int init_heap(int field_count) = 0;
  //virtual int field_init_boolean(char *name, int index, int value) = 0;
  //virtual int field_init_byte(char *name, int index, int value) = 0;
  //virtual int field_init_short(char *name, int index, int value) = 0;
  virtual int field_init_int(std::string &name, int index, int value) = 0;
  virtual int field_init_ref(std::string &name, int index) = 0;
  virtual void method_start(int local_count, int max_stack, int param_count, std::string &name) = 0;
  virtual void method_end(int local_count) = 0;
  virtual int push_local_var_int(int index) = 0;
  virtual int push_local_var_ref(int index) = 0;
  virtual int push_local_var_float(int index);
  virtual int push_local_var_long(int index) { return -1; }
  virtual int push_ref_static(std::string &name, int index) = 0;
  virtual int push_fake() { return -1; } // move stack ptr without push
  virtual int set_integer_local(int index, int value) { return -1; }
  virtual int set_float_local(int index, float value);
  virtual int set_ref_local(int index, std::string &name) { return -1; }
  virtual int push_int(int32_t n) = 0;
  virtual int push_long(int64_t n);
  virtual int push_float(float f);
  virtual int push_double(double f);
  virtual int push_ref(std::string &name) = 0;
  virtual int pop_local_var_int(int index) = 0;
  virtual int pop_local_var_ref(int index) = 0;
  virtual int pop_local_var_float(int index) { return -1; }
  virtual int pop_local_var_long(int index) { return -1; }
  virtual int pop() = 0;
  virtual int dup() = 0;
  virtual int dup2() = 0;
  virtual int swap() = 0;
  virtual int add_integer() = 0;
  virtual int add_integer(int num) { return -1; }
  virtual int sub_integer() = 0;
  virtual int sub_integer(int num) { return -1; }
  virtual int mul_integer() = 0;
  virtual int div_integer() = 0;
  virtual int mod_integer() = 0;
  virtual int neg_integer() = 0;
  virtual int shift_left_integer() = 0;
  virtual int shift_left_integer(int count) { return -1; }
  virtual int shift_right_integer() = 0;
  virtual int shift_right_integer(int count) { return -1; }
  virtual int shift_right_uinteger() = 0;
  virtual int shift_right_uinteger(int count) { return -1; }
  virtual int and_integer() = 0;
  virtual int and_integer(int num) { return -1; }
  virtual int or_integer() = 0;
  virtual int or_integer(int num) { return -1; }
  virtual int xor_integer() = 0;
  virtual int xor_integer(int num) { return -1; }
  virtual int inc_integer(int index, int num) = 0;
  virtual int integer_to_byte() = 0;
  virtual int integer_to_short() = 0;
  virtual int integer_to_long() { return -1; }
  virtual int long_to_integer() { return -1; }
  virtual int add_float();
  virtual int sub_float();
  virtual int mul_float();
  virtual int div_float();
  virtual int neg_float();
  virtual int float_to_integer();
  virtual int integer_to_float();
  virtual int add_long() { return -1; }
  virtual int sub_long() { return -1; }
  virtual int mul_long() { return -1; }
  virtual int div_long() { return -1; }
  virtual int mod_long() { return -1; }
  virtual int neg_long() { return -1; }
  virtual int shift_left_long() { return -1; }
  virtual int shift_right_long() { return -1; }
  virtual int shift_right_ulong() { return -1; }
  virtual int and_long() { return -1; }
  virtual int or_long() { return -1; }
  virtual int xor_long() { return -1; }
  virtual int compare_longs() { return -1; }
  virtual int jump_cond(std::string &label, int cond, int distance) = 0;
  virtual int jump_cond_zero(std::string &label, int cond, int distance) { return -1; }
  virtual int jump_cond_integer(std::string &label, int cond, int distance) = 0;
  virtual int jump_cond_integer(std::string &label, int cond, int const_val, int distance) { return -1; }
  virtual int compare_floats(int cond);
  virtual int ternary(int cond, int value_true, int value_false) = 0;
  virtual int ternary(int cond, int compare, int value_true, int value_false) = 0;
  virtual int return_local(int index, int local_count) = 0;
  virtual int return_integer(int local_count) = 0;
  virtual int return_void(int local_count) = 0;
  virtual int return_long(int local_count) { return -1; }
  virtual int jump(std::string &name, int distance) = 0;
  virtual int call(std::string &name) = 0;
  virtual int invoke_static_method(const char *name, int params, int is_void) = 0;
  virtual int put_static(std::string &name, int index) = 0;
  virtual int get_static(std::string &name, int index) = 0;
  virtual int brk() = 0;
  virtual int new_object(std::string &object_name, int field_count);
  virtual int new_array(uint8_t type) = 0;
  virtual int new_object_array(std::string &class_name);
  virtual int insert_array(std::string &name, int32_t *data, int len, uint8_t type) = 0;
  virtual int insert_string(std::string &name, uint8_t *bytes, int len) = 0;
  virtual int push_array_length() = 0;
  virtual int push_array_length(std::string &name, int field_id) = 0;
  virtual int array_read_byte() = 0;
  virtual int array_read_short() = 0;
  virtual int array_read_int() = 0;
  virtual int array_read_float();
  virtual int array_read_long();
  virtual int array_read_object();
  virtual int array_read_byte(std::string &name, int field_id) = 0;
  virtual int array_read_short(std::string &name, int field_id) = 0;
  virtual int array_read_int(std::string &name, int field_id) = 0;
  virtual int array_read_float(std::string &name, int field_id);
  virtual int array_read_long(std::string &name, int field_id);
  virtual int array_read_object(std::string &name, int field_id);
  virtual int array_write_byte() = 0;
  virtual int array_write_short() = 0;
  virtual int array_write_int() = 0;
  virtual int array_write_float();
  virtual int array_write_long();
  virtual int array_write_object();
  virtual int array_write_byte(std::string &name, int field_id) = 0;
  virtual int array_write_short(std::string &name, int field_id) = 0;
  virtual int array_write_int(std::string &name, int field_id) = 0;
  virtual int array_write_float(std::string &name, int field_id);
  virtual int array_write_long(std::string &name, int field_id);
  virtual int array_write_object(std::string &name, int field_id);

  // CPU
  virtual int cpu_asm_X(const char *code, int len);

  void add_newline();
  void instruction_count_clear() { instruction_count = 0; }
  void instruction_count_inc() { instruction_count++; }

  int use_array_file(const char *filename, const char *array, int type);
  int ignore() { return 0; }

protected:
  struct ArrayFiles
  {
    std::string name;
    int type;
  };

  virtual int add_array_files();
  virtual int get_int_size() { return 4; }
  int insert_db(std::string &name, int32_t *data, int len, uint8_t len_type);
  int insert_dw(std::string &name, int32_t *data, int len, uint8_t len_type);
  int insert_dc32(std::string &name, int32_t *data, int len, uint8_t len_type, const char *dc32 = "dc32");
  int insert_float(std::string &name, int32_t *data, int len, uint8_t len_type, const char *dc32 = "dc32");
  int get_constant(uint32_t constant);
  void insert_constants_pool();
  int insert_utf8(std::string &name, uint8_t *bytes, int len);

  FILE *out;
  int label_count;
  int instruction_count;
  int preload_array_align;
  std::map<uint32_t, int> constants_pool;
  std::map<std::string, ArrayFiles> preload_arrays;
};

enum
{
  COND_EQUAL = 0,
  COND_NOT_EQUAL,
  COND_LESS,
  COND_LESS_EQUAL,
  COND_GREATER,
  COND_GREATER_EQUAL,
};

// This is redundant
enum
{
  TYPE_BOOLEAN=4,
  TYPE_CHAR=5,
  TYPE_FLOAT=6,
  TYPE_DOUBLE=7,
  TYPE_BYTE=8,
  TYPE_SHORT=9,
  TYPE_INT=10,
  TYPE_LONG=11,
};

#endif

