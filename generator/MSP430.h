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

#ifndef _MSP430_H
#define _MSP430_H

#include "Generator.h"

enum
{
  MSP430G2231,
  MSP430G2553,
};

class MSP430 : public Generator
{
public:
  MSP430(uint8_t chip_type);
  virtual ~MSP430();

  virtual int open(char *filename);
  virtual int start_init();
  virtual int insert_static_field_define(const char *name, int index);
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
  virtual int set_integer_local(int index, int value);
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
  virtual int add_integers();
  virtual int sub_integers();
  virtual int mul_integers();
  virtual int div_integers();
  virtual int mod_integers();
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
  virtual int jump_cond_integer(const char *label, int cond, int const_val);
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

  // GPIO functions
  virtual int ioport_setPinsAsInput(int port);
  virtual int ioport_setPinsAsInput(int port, int const_val);
  virtual int ioport_setPinsAsOutput(int port);
  virtual int ioport_setPinsAsOutput(int port, int const_val);
  virtual int ioport_setPinsValue(int port);
  virtual int ioport_setPinsValue(int port, int const_val);
  virtual int ioport_setPinsHigh(int port);
  virtual int ioport_setPinsLow(int port);
  virtual int ioport_setPinAsOutput(int port);
  virtual int ioport_setPinAsInput(int port);
  virtual int ioport_setPinHigh(int port);
  virtual int ioport_setPinLow(int port);
  virtual int ioport_isPinInputHigh(int port);
  virtual int ioport_getPortInputValue(int port);
  //virtual int ioport_setPortOutputValue(int port);

  // UART functions
  virtual int uart_init(int port);
  virtual int uart_send(int port);
  virtual int uart_read(int port);
  virtual int uart_isDataAvailable(int port);
  virtual int uart_isSendReady(int port);

  // SPI functions
  virtual int spi_init(int port);
  virtual int spi_init(int port, int clock_divisor, int mode);
  virtual int spi_send(int port);
  virtual int spi_read(int port);
  virtual int spi_isDataAvailable(int port);
  virtual int spi_isBusy(int port);
  virtual int spi_disable(int port);
  virtual int spi_enable(int port);

  // CPU functions
  virtual int cpu_setClock16();
  virtual int cpu_nop();

  // Memory
  virtual int memory_read8();
  virtual int memory_write8();
  virtual int memory_read16();
  virtual int memory_write16();

protected:
  int set_periph(const char *instr, const char *periph);
  int stack_alu(const char *instr);
  void push_reg(const char *reg);
  void pop_reg(char *reg);
  void insert_read_spi();
  void insert_mul_integers();
  void insert_div_integers();
  int array_get_registers(int *value_reg, int *index_reg, int *ref_reg);
  int reg;
  int reg_max;
  int stack;
  int label_count;
  bool need_read_spi:1;
  bool need_mul_integers:1;
  bool need_div_integers:1;
  bool is_main:1;
  uint32_t ram_start;
  uint32_t stack_start;
  uint32_t flash_start;
};

#endif

