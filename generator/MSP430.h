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

#ifndef _MSP430_H
#define _MSP430_H

#include "Generator.h"

enum
{
  MSP430G2231,
  MSP430G2452,
  MSP430G2553,
  MSP430F5529,
};

class MSP430 : public Generator
{
public:
  MSP430(uint8_t chip_type);
  virtual ~MSP430();

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
  virtual int set_integer_local(int index, int value);
  virtual int set_ref_local(int index, std::string &name);
  virtual int push_int(int32_t n);
  //virtual int push_long(int64_t n);
  //virtual int push_float(float f);
  //virtual int push_double(double f);
  virtual int push_ref(std::string &name);
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
  virtual int shift_left_integer(int count);
  virtual int shift_right_integer();
  virtual int shift_right_integer(int count);
  virtual int shift_right_uinteger();
  virtual int shift_right_uinteger(int count);
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
  virtual int jump_cond_integer(std::string &label, int cond, int const_val, int distance);
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
  //virtual void close();

  // GPIO functions
  virtual int ioport_setPinsAsInput_I(int port);
  virtual int ioport_setPinsAsInput_I(int port, int const_val);
  virtual int ioport_setPinsAsOutput_I(int port);
  virtual int ioport_setPinsAsOutput_I(int port, int const_val);
  virtual int ioport_setPinsValue_I(int port);
  virtual int ioport_setPinsValue_I(int port, int const_val);
  virtual int ioport_setPinsHigh_I(int port);
  virtual int ioport_setPinsLow_I(int port);
  virtual int ioport_setPinAsOutput_I(int port);
  virtual int ioport_setPinAsOutput_I(int port, int const_val);
  virtual int ioport_setPinAsInput_I(int port);
  virtual int ioport_setPinAsInput_I(int port, int const_val);
  virtual int ioport_setPinHigh_I(int port);
  virtual int ioport_setPinHigh_I(int port, int const_val);
  virtual int ioport_setPinLow_I(int port);
  virtual int ioport_setPinLow_I(int port, int const_val);
  virtual int ioport_isPinInputHigh_I(int port);
  virtual int ioport_getPortInputValue(int port);
  //virtual int ioport_setPortOutputValue_I(int port);
  virtual int ioport_setPinsResistorEnable_I(int port);
  virtual int ioport_setPinsResistorEnable_I(int port, int const_val);

  // UART functions
  virtual int uart_init_I(int port);
  virtual int uart_init_I(int port, int baud_rate);
  virtual int uart_send_I(int port);
  virtual int uart_read(int port);
  virtual int uart_isDataAvailable(int port);
  virtual int uart_isSendReady(int port);

  // SPI functions
  virtual int spi_init_II(int port);
  virtual int spi_init_II(int port, int clock_divisor, int mode);
  virtual int spi_init16_II(int port);
  virtual int spi_init16_II(int port, int clock_divisor, int mode);
  virtual int spi_send_I(int port);
  virtual int spi_send16_I(int port);
  virtual int spi_read_I(int port);
  virtual int spi_read16_I(int port);
  virtual int spi_isDataAvailable(int port);
  virtual int spi_isBusy(int port);
  virtual int spi_disable(int port);
  virtual int spi_enable(int port);

  // ADC Functions
  virtual int adc_enable();
  virtual int adc_disable();
  virtual int adc_setChannel_I();
  virtual int adc_setChannel_I(int channel);
  virtual int adc_read();

  // Timer functions
  virtual int timer_setInterval_II();
  virtual int timer_setInterval_II(int cycles, int divider);
  virtual int timer_setListener_Z();
  virtual int timer_setListener_Z(int const_value);
  virtual int timer_getValue();
  virtual int timer_setValue_I();
  virtual int timer_setValue_I(int const_value);

  // Watchdog
  virtual int watchdog_enable();
  virtual int watchdog_disable();
  virtual int watchdog_kick();

  // CPU functions
  virtual int cpu_setClock8();
  virtual int cpu_setClock16();
  virtual int cpu_nop();

  // Memory
  virtual int memory_read8_I();
  virtual int memory_write8_IB();
  virtual int memory_read16_I();
  virtual int memory_write16_IS();

protected:
  virtual int get_int_size() { return 2; }
  int set_periph(const char *instr, const char *periph);
  char *pop_reg();
  char *top_reg();
  int stack_alu(const char *instr);
  void push_reg(const char *reg);
  void pop_reg(char *reg);
  void insert_read_spi();
  void insert_mul_integers();
  void insert_div_integers();
  int get_values_from_stack(int *value1, int *value2, int *value3);
  int get_values_from_stack(int *value1, int *value2);
  int get_values_from_stack(int *value1);

  int reg;
  int reg_max;
  int stack;
  int label_count;
  char reg_string[8];
  bool need_read_spi:1;
  bool need_mul_integers:1;
  bool need_div_integers:1;
  bool need_timer_interrupt:1;
  bool is_main:1;
  bool is_interrupt:1;
  uint32_t ram_start;
  uint32_t stack_start;
  uint32_t flash_start;
  int max_stack;
  const char *include_file;
  uint16_t vector_timer;
};

#endif

