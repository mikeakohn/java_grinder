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

#ifndef _M6502_H
#define _M6502_H

#include "Generator.h"

class M6502 : public Generator
{
public:
  M6502();
  virtual ~M6502();

  virtual int open(char *filename);
  virtual int start_init();
  virtual int insert_static_field_define(const char *name, const char *type, int index);
  virtual int init_heap(int field_count);
  virtual int insert_field_init_boolean(char *name, int index, int value);
  virtual int insert_field_init_byte(char *name, int index, int value);
  virtual int insert_field_init_short(char *name, int index, int value);
  virtual int insert_field_init_int(char *name, int index, int value);
  virtual int insert_field_init(char *name, int index);
  virtual void method_start(int local_count, const char *name);
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
  virtual int sub_integer();
  virtual int mul_integer();
  virtual int div_integer();
  virtual int mod_integer();
  virtual int neg_integer();
  virtual int shift_left_integer();
  virtual int shift_right_integer();
  virtual int shift_right_uinteger();
  virtual int and_integer();
  virtual int or_integer();
  virtual int xor_integer();
  virtual int inc_integer(int index, int num);
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
  virtual int get_values_from_stack(int num);

  // Memory API
  virtual int memory_read8();
  virtual int memory_write8();

  // VIC API
  virtual int c64_vic_border();
  virtual int c64_vic_background();
  virtual int c64_vic_sprite_enable();
  virtual int c64_vic_sprite_pos();
  virtual int c64_vic_poke_reg();

protected:
  int stack;
  bool is_main:1;
};

#endif

