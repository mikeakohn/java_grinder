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

#ifndef JAVA_GRINDER_GENERATOR_R5900_H
#define JAVA_GRINDER_GENERATOR_R5900_H

#include "generator/Generator.h"

// Stack points to the last used slot.
// push = subtract 4, then place value
#define STACK_PUSH(t) \
  fprintf(out, "  addi $sp, $sp, -4\n"); \
  fprintf(out, "  sw $t%d, 0($sp)\n", t); \
  stack++;

// pop = read value, then subtract 4
#define STACK_POP(t) \
  fprintf(out, "  lw $%d, 0($sp)\n", t); \
  fprintf(out, "  addi $sp, $sp, 4\n"); \
  stack--;

class R5900 : public Generator
{
public:
  R5900();
  virtual ~R5900();

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
  virtual int push_local_var_float(int index);
  virtual int push_ref_static(std::string &name, int index);
  virtual int push_fake();
  //virtual int set_integer_local(int index, int value);
  virtual int push_int(int32_t n);
  //virtual int push_long(int64_t n);
  virtual int push_float(float f);
  //virtual int push_double(double f);
  virtual int push_ref(std::string &name);
  virtual int pop_local_var_int(int index);
  virtual int pop_local_var_ref(int index);
  virtual int pop_local_var_float(int index);
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
  virtual int add_float();
  virtual int sub_float();
  virtual int mul_float();
  virtual int div_float();
  virtual int neg_float();
  virtual int float_to_integer();
  virtual int integer_to_float();
  virtual int jump_cond(std::string &label, int cond, int distance);
  virtual int jump_cond_integer(std::string &label, int cond, int distance);
  virtual int compare_floats(int cond);
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
  virtual int new_object_array(std::string &class_name);
  virtual int insert_array(std::string &name, int32_t *data, int len, uint8_t type);
  virtual int insert_string(std::string &name, uint8_t *bytes, int len);
  virtual int push_array_length();
  virtual int push_array_length(std::string &name, int field_id);
  virtual int array_read_byte();
  virtual int array_read_short();
  virtual int array_read_int();
  virtual int array_read_float();
  virtual int array_read_object();
  virtual int array_read_byte(std::string &name, int field_id);
  virtual int array_read_short(std::string &name, int field_id);
  virtual int array_read_int(std::string &name, int field_id);
  virtual int array_read_float(std::string &name, int field_id);
  virtual int array_read_object(std::string &name, int field_id);
  virtual int array_write_byte();
  virtual int array_write_short();
  virtual int array_write_int();
  virtual int array_write_float();
  virtual int array_write_object();
  virtual int array_write_byte(std::string &name, int field_id);
  virtual int array_write_short(std::string &name, int field_id);
  virtual int array_write_int(std::string &name, int field_id);
  virtual int array_write_float(std::string &name, int field_id);
  virtual int array_write_object(std::string &name, int field_id);
  virtual int cpu_nop();
  virtual int memory_preloadByteArray_X(const char *array_name);
  virtual int memory_preloadIntArray_X(const char *array_name);

protected:
  int reg;            // count number of registers are are using as stack
  int reg_max;        // size of register stack 
  int stack;          // count how many things we put on the stack
  uint32_t org;       // .org to use for this chip
  uint32_t ram_start; // start of ram
  uint32_t ram_end;   // end of ram
  //uint32_t virtual_address;  // start of code space virtual address
  uint32_t physical_address; // start of code space physical address

private:
  int stack_alu(const char *instr);
  int divide();
  int get_values_from_stack(int *value1);
  int get_values_from_stack(int *value1, int *value2);
  int get_ref_from_stack(int *value1);
  int set_constant(int reg, int value);

  bool is_main : 1;
};

#endif

