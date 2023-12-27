/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2023 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_F100_L_H
#define JAVA_GRINDER_GENERATOR_F100_L_H

#include "generator/Generator.h"

class F100_L : public Generator
{
public:
  F100_L();
  virtual ~F100_L();

  virtual int open(const char *filename);
  virtual int start_init();
  virtual int insert_static_field_define(std::string &name, std::string &type, int index);
  virtual int init_heap(int field_count);
  virtual int field_init_int(std::string &name, int index, int value);
  virtual int field_init_ref(std::string &name, int index);
  virtual void method_start(int local_count, int max_stack, int param_count, std::string &name);
  virtual void method_end(int local_count);
  virtual int push_local_var_int(int index);
  virtual int push_local_var_ref(int index);
  virtual int push_ref_static(std::string &name, int index);
  virtual int push_fake();
  virtual int push_int(int32_t n);
  //virtual int push_long(int64_t n);
  //virtual int push_float(float f);
  //virtual int push_double(double f);
  virtual int push_ref(std::string &name, int index);
  virtual int pop_local_var_int(int index);
  virtual int pop_local_var_ref(int index);
  virtual int pop();
  virtual int dup();
  virtual int dup2();
  virtual int swap();
  virtual int add_integer();
  virtual int add_integer(int num);
  virtual int sub_integer();
  virtual int sub_integer(int num);
  virtual int mul_integer();
  virtual int div_integer();
  virtual int mod_integer();
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
  virtual int integer_to_short();
  virtual int jump_cond(std::string &label, int cond, int distance);
  virtual int jump_cond_zero(std::string &label, int cond, int distance);
  virtual int jump_cond_integer(std::string &label, int cond, int distance);
  virtual int ternary(int cond, int value_true, int value_false);
  virtual int ternary(int cond, int compare, int value_true, int value_false);
  virtual int return_local(int index, int local_count);
  virtual int return_integer(int local_count);
  virtual int return_void(int local_count);
  virtual int jump(std::string &name, int distance);
  virtual int call(std::string &name);
  virtual int invoke_static_method(const char *name, int params, int is_void);
  virtual int put_static(std::string &name, int index);
  virtual int get_static(std::string &name, int index);
  virtual int brk();
  virtual int new_array(uint8_t type);
  virtual int insert_array(std::string &name, int32_t *data, int len, uint8_t type);
  virtual int insert_string(std::string &name, uint8_t *bytes, int len);
  virtual int push_array_length();
  virtual int push_array_length(std::string &name, int field_id);
  virtual int array_read_byte();
  virtual int array_read_short();
  virtual int array_read_int();
  virtual int array_read_byte(std::string &name, int field_id);
  virtual int array_read_short(std::string &name, int field_id);
  virtual int array_read_int(std::string &name, int field_id);
  virtual int array_write_byte();
  virtual int array_write_short();
  virtual int array_write_int();
  virtual int array_write_byte(std::string &name, int field_id);
  virtual int array_write_short(std::string &name, int field_id);
  virtual int array_write_int(std::string &name, int field_id);

  // If this CPU is used for other things, move this into a more
  // specific file.
  virtual int ioport_setPinsValue_I(int port);
  virtual int ioport_setPinsValue_I(int port, int const_val);
  virtual int ice_fun_setTone_I();
  virtual int ice_fun_setServo_II();

protected:
  int stack;          // count how many things currently on the stack
  int heap_address;
  int frame_ptr;
  int java_stack_ptr;
  int heap_ptr;
  int global_vars;
  int temp_1;
  int temp_2;
  int temp_ptr;
  bool is_main : 1;
};

#endif

