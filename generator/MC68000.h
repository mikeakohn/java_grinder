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

#ifndef _MC68000_H
#define _MC68000_H

#include "Generator.h"

class MC68000 : public Generator
{
public:
  MC68000();
  virtual ~MC68000();

  virtual int open(char *filename);
  virtual int start_init();
  virtual int insert_static_field_define(const char *name, const char *type, int index);
  virtual int init_heap(int field_count);
  virtual int insert_field_init_boolean(char *name, int index, int value);
  virtual int insert_field_init_byte(char *name, int index, int value);
  virtual int insert_field_init_short(char *name, int index, int value);
  virtual int insert_field_init_int(char *name, int index, int value);
  virtual int insert_field_init(char *name, int index);
  virtual void method_start(int local_count, int max_stack, int param_count, const char *name);
  virtual void method_end(int local_count);
  virtual int push_integer(int32_t n);
  virtual int push_integer_local(int index);
  virtual int push_ref_local(int index);
  virtual int push_long(int64_t n);
  virtual int push_float(float f);
  virtual int push_double(double f);
  virtual int push_byte(int8_t b);
  virtual int push_short(int16_t s);
  virtual int pop_integer_local(int index);
  virtual int pop_ref_local(int index);
  virtual int pop();
  virtual int dup();
  virtual int dup2();
  virtual int swap();
  virtual int add_integer();
  virtual int add_integer(int num);
  virtual int sub_integer();
  virtual int sub_integer(int num);
  virtual int mul_integer();
  virtual int mul_integer(int num);
  virtual int div_integer();
  virtual int div_integer(int num);
  virtual int mod_integer();
  virtual int mod_integer(int num);
  virtual int neg_integer();
  virtual int shift_left_integer();
  virtual int shift_left_integer(int num);
  virtual int shift_right_integer();
  virtual int shift_right_integer(int num);
  virtual int shift_right_uinteger();
  virtual int shift_right_uinteger(int num);
  virtual int and_integer();
  virtual int and_integer(int num);
  virtual int or_integer();
  virtual int or_integer(int num);
  virtual int xor_integer();
  virtual int xor_integer(int num);
  virtual int inc_integer(int index, int num);
  virtual int integer_to_byte();
  virtual int jump_cond(const char *label, int cond);
  virtual int jump_cond_integer(const char *label, int cond);
  virtual int return_local(int index, int local_count);
  virtual int return_integer(int local_count);
  virtual int return_void(int local_count);
  virtual int jump(const char *name);
  virtual int call(const char *name);
  virtual int invoke_static_method(const char *name, int params, int is_void);
  virtual int put_static(const char *name, int index);
  virtual int get_static(const char *name, int index);
  virtual int brk();
  virtual int new_array(uint8_t type);
  virtual int insert_array(const char *name, int32_t *data, int len, uint8_t type);
  virtual int push_array_length();
  virtual int push_array_length(const char *name, int field_id);
  virtual int array_read_byte();
  virtual int array_read_short();
  virtual int array_read_int();
  virtual int array_read_byte(const char *name, int field_id);
  virtual int array_read_short(const char *name, int field_id);
  virtual int array_read_int(const char *name, int field_id);
  virtual int array_write_byte();
  virtual int array_write_short();
  virtual int array_write_int();
  virtual int array_write_byte(const char *name, int field_id);
  virtual int array_write_short(const char *name, int field_id);
  virtual int array_write_int(const char *name, int field_id);
  //virtual void close();

protected:
  int reg;            // count number of registers are are using as stack
  int reg_max;        // size of register stack 
  int stack;          // count how many things we put on the stack
  bool is_main : 1;

  int stack_alu(const char *instr);
  int get_values_from_stack(int *value1, int *value2, int *value3);
  int get_values_from_stack(int *value1, int *value2);
  int get_values_from_stack(int *value1);
  int get_ref_from_stack();
};

#endif

