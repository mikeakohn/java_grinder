/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2017 by Michael Kohn
 *
 */

#ifndef _PROPELLER_H
#define _PROPELLER_H

#include <vector>
#include <string>

#include "Generator.h"

class Propeller : public Generator
{
public:
  Propeller();
  virtual ~Propeller();

  virtual int open(const char *filename);
  virtual int start_init();
  virtual int insert_static_field_define(const char *name, const char *type, int index);
  virtual int init_heap(int field_count);
  virtual int field_init_int(char *name, int index, int value);
  virtual int field_init_ref(char *name, int index);
  virtual void method_start(int local_count, int max_stack, int param_count, const char *name);
  virtual void method_end(int local_count);
  virtual int push_local_var_int(int index);
  virtual int push_local_var_ref(int index);
  virtual int push_ref_static(const char *name, int index);
  virtual int push_fake();
  virtual int set_integer_local(int index, int value);
  virtual int push_int(int32_t n);
  virtual int push_long(int64_t n);
  virtual int push_float(float f);
  virtual int push_double(double f);
  virtual int push_ref(char *name);
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
  virtual int jump_cond(const char *label, int cond, int distance);
  virtual int jump_cond_integer(const char *label, int cond, int distance);
  virtual int ternary(int cond, int value_true, int value_false);
  virtual int ternary(int cond, int compare, int value_true, int value_false);
  virtual int return_local(int index, int local_count);
  virtual int return_integer(int local_count);
  virtual int return_void(int local_count);
  virtual int jump(const char *name, int distance);
  virtual int call(const char *name);
  virtual int invoke_static_method(const char *name, int params, int is_void);
  virtual int put_static(const char *name, int index);
  virtual int get_static(const char *name, int index);
  virtual int brk();
  virtual int new_array(uint8_t type);
  virtual int insert_array(const char *name, int32_t *data, int len, uint8_t type);
  virtual int insert_string(const char *name, uint8_t *bytes, int len);
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

  // Math
  virtual int math_abs_I();
  virtual int math_xor_I();
  virtual int math_min_II();
  virtual int math_max_II();

  // Propeller specific
  virtual int propeller_setClock_I();
  virtual int propeller_getCogId();
  virtual int propeller_stopCog_I();
  virtual int propeller_waitPinsEqual_II();
  virtual int propeller_waitPinsEqual_II(int mask);
  virtual int propeller_waitPinsNotEqual_II();
  virtual int propeller_waitPinsNotEqual_II(int mask);
  virtual int propeller_waitCount_II();
  virtual int propeller_waitCount_II(int delay);

  // CPU
  virtual int cpu_getCycleCount();

  // I/O
  virtual int ioport_setPinsAsInput_I(int port);
  virtual int ioport_setPinsAsOutput_I(int port);
  virtual int ioport_setPinsAsOutput_I(int port, int value);
  virtual int ioport_setPinsValue_I(int port);
  virtual int ioport_setPinsValue_I(int port, int value);
  virtual int ioport_setPinsHigh_I(int port);
  virtual int ioport_setPinsHigh_I(int port, int value);
  virtual int ioport_setPinsLow_I(int port);
  //virtual int ioport_setPinAsOutput_I(int port);
  //virtual int ioport_setPinAsInput_I(int port);
  //virtual int ioport_setPinHigh_I(int port);
  //virtual int ioport_setPinLow_I(int port);
  virtual int ioport_isPinInputHigh_I(int port);
  virtual int ioport_getPortInputValue(int port);

protected:
  int reg;            // count number of registers are are using as stack
  int reg_max;        // size of register stack 
  std::string method_name;
  std::vector<std::string> statics;
  bool is_main : 1;
  bool need_muls : 1;

private:
  int add_muls();
};

#endif

