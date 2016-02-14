/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 */

#ifndef _GENERATOR_H
#define _GENERATOR_H

#include <stdio.h>
#include <stdint.h>

#include <map>

#include "API_APPLEIIGS.h"
#include "API_Atari2600.h"
#include "API_C64.h"
#include "API_DSP.h"
#include "API_Math.h"
#include "API_Microcontroller.h"
#include "API_Propeller.h"
#include "API_SegaGenesis.h"
#include "API_SNES.h"
#include "API_System.h"
#include "API_TI84.h"
#include "API_TI99.h"
#include "API_TRS80_Coco.h"
#include "API_W65C265SXB.h"

class Generator :
  public API_AppleIIgs,
  public API_Atari2600,
  public API_C64,
  public API_DSP,
  public API_Math,
  public API_Microcontroller,
  public API_Propeller,
  public API_SegaGenesis,
  public API_SNES,
  public API_System,
  public API_TI84,
  public API_TI99,
  public API_TRS80_Coco,
  public API_W65C265SXB
{
public:
  Generator();
  virtual ~Generator();

  virtual int open(const char *filename);
  virtual int add_functions() { return 0; }
  virtual int get_cpu_byte_alignment() { return 2; }
  void label(char *name);
  virtual int start_init() = 0;
  virtual int insert_static_field_define(const char *name, const char *type, int index) = 0;
  virtual int init_heap(int field_count) = 0;
  //virtual int field_init_boolean(char *name, int index, int value) = 0;
  //virtual int field_init_byte(char *name, int index, int value) = 0;
  //virtual int field_init_short(char *name, int index, int value) = 0;
  virtual int field_init_int(char *name, int index, int value) = 0;
  virtual int field_init_ref(char *name, int index) = 0;
  virtual void method_start(int local_count, int max_stack, int param_count, const char *name) = 0;
  virtual void method_end(int local_count) = 0;
  virtual int push_local_var_int(int index) = 0;
  virtual int push_local_var_ref(int index) = 0;
  virtual int push_ref_static(const char *name, int index) = 0;
  virtual int push_fake() { return -1; } // move stack ptr without push
  virtual int set_integer_local(int index, int value) { return -1; }
  virtual int set_ref_local(int index, char *name) { return -1; }
  virtual int push_int(int32_t n) = 0;
  virtual int push_long(int64_t n) = 0;
  virtual int push_float(float f) = 0;
  virtual int push_double(double f) = 0;
  virtual int push_byte(int8_t b) = 0;
  virtual int push_short(int16_t s) = 0;
  virtual int push_ref(char *name) = 0;
  virtual int pop_local_var_int(int index) = 0;
  virtual int pop_local_var_ref(int index) = 0;
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
  virtual int jump_cond(const char *label, int cond, int distance) = 0;
  virtual int jump_cond_zero(const char *label, int cond, int distance) { return -1; }
  virtual int jump_cond_integer(const char *label, int cond, int distance) = 0;
  virtual int jump_cond_integer(const char *label, int cond, int const_val, int distance) { return -1; } 
  virtual int return_local(int index, int local_count) = 0;
  virtual int return_integer(int local_count) = 0;
  virtual int return_void(int local_count) = 0;
  virtual int jump(const char *name, int distance) = 0;
  virtual int call(const char *name) = 0;
  virtual int invoke_static_method(const char *name, int params, int is_void) = 0;
  virtual int put_static(const char *name, int index) = 0;
  virtual int get_static(const char *name, int index) = 0;
  virtual int brk() = 0;
  virtual int new_array(uint8_t type) = 0;
  virtual int insert_array(const char *name, int32_t *data, int len, uint8_t type) = 0;
  virtual int insert_string(const char *name, uint8_t *bytes, int len) = 0;
  virtual int push_array_length() = 0;
  virtual int push_array_length(const char *name, int field_id) = 0;
  virtual int array_read_byte() = 0;
  virtual int array_read_short() = 0;
  virtual int array_read_int() = 0;
  virtual int array_read_byte(const char *name, int field_id) = 0;
  virtual int array_read_short(const char *name, int field_id) = 0;
  virtual int array_read_int(const char *name, int field_id) = 0;
  virtual int array_write_byte() = 0;
  virtual int array_write_short() = 0;
  virtual int array_write_int() = 0;
  virtual int array_write_byte(const char *name, int field_id) = 0;
  virtual int array_write_short(const char *name, int field_id) = 0;
  virtual int array_write_int(const char *name, int field_id) = 0;
  //virtual void close() = 0;

  // CPU
  virtual int cpu_asm_X(const char *code, int len);

  void add_newline();
  void instruction_count_clear() { instruction_count = 0; }
  void instruction_count_inc() { instruction_count++; }

protected:
  int insert_db(const char *name, int32_t *data, int len, uint8_t len_type);
  int insert_dw(const char *name, int32_t *data, int len, uint8_t len_type);
  int insert_dc32(const char *name, int32_t *data, int len, uint8_t len_type, const char *dc32 = "dc32");
  int get_constant(uint32_t constant);
  void insert_constants_pool();
  int insert_utf8(const char *name, uint8_t *bytes, int len);

  FILE *out;
  int label_count;
  int instruction_count;
  std::map<uint32_t,int> constants_pool;
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

#if 0
enum
{
  CPU_INVALID,
  CPU_MSP430,
  CPU_DSPIC,
};
#endif

#endif

