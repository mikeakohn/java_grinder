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

#ifndef _GENERATOR_H
#define _GENERATOR_H

#include <stdio.h>

class Generator
{
public:
  Generator();
  virtual ~Generator();

  virtual int open(char *filename);
  void label(char *name);

  //virtual int init() = 0;
  //virtual void serial_init() = 0;
  virtual void method_start(int local_count, const char *name) = 0;
  virtual void method_end(int local_count) = 0;
  virtual int push_integer(int32_t n) = 0;
  virtual int push_integer_local(int index) = 0;
  virtual int set_integer_local(int index, int value) { return -1; }
  virtual int push_long(int64_t n) = 0;
  virtual int push_float(float f) = 0;
  virtual int push_double(double f) = 0;
  virtual int push_byte(int8_t b) = 0;
  virtual int push_short(int16_t s) = 0;
  virtual int pop_integer_local(int index) = 0;
  virtual int pop() = 0;
  virtual int dup() = 0;
  virtual int dup2() = 0;
  virtual int swap() = 0;
  virtual int add_integers() = 0;
  virtual int sub_integers() = 0;
  virtual int mul_integers() = 0;
  virtual int div_integers() = 0;
  virtual int mod_integers() = 0;
  virtual int neg_integer() = 0;
  virtual int shift_left_integer() = 0;
  virtual int shift_right_integer() = 0;
  virtual int shift_right_uinteger() = 0;
  virtual int and_integer() = 0;
  virtual int or_integer() = 0;
  virtual int xor_integer() = 0;
  virtual int inc_integer(int index, int num) = 0;
  virtual int jump_cond(const char *label, int cond) = 0;
  virtual int jump_cond_integer(const char *label, int cond) = 0;
  virtual int jump_cond_integer(const char *label, int cond, int const_val) { return -1; } 
  virtual int return_local(int index, int local_count) = 0;
  virtual int return_integer(int local_count) = 0;
  virtual int return_void(int local_count) = 0;
  virtual int jump(const char *name) = 0;
  virtual int call(const char *name) = 0;
  virtual int invoke_static_method(const char *name, int params, int is_void) = 0;
  virtual int brk() = 0;
  //virtual void close() = 0;

  // GPIO functions
  virtual int ioport_setPinsAsInput(int port) { return -1; }
  virtual int ioport_setPinsAsInput(int port, int const_val) { return -1; }
  virtual int ioport_setPinsAsOutput(int port) { return -1; }
  virtual int ioport_setPinsAsOutput(int port, int const_val) { return -1; }
  virtual int ioport_setPinsValue(int port) { return -1; }
  virtual int ioport_setPinsValue(int port, int const_val) { return -1; }
  virtual int ioport_setPinsHigh(int port) { return -1; }
  virtual int ioport_setPinsHigh(int port, int const_val) { return -1; }
  virtual int ioport_setPinsLow(int port) { return -1; }
  virtual int ioport_setPinsLow(int port, int const_val) { return -1; }
  virtual int ioport_setPinAsOutput(int port) { return -1; }
  virtual int ioport_setPinAsOutput(int port, int const_val) { return -1; }
  virtual int ioport_setPinAsInput(int port) { return -1; }
  virtual int ioport_setPinAsInput(int port, int const_val) { return -1; }
  virtual int ioport_setPinHigh(int port) { return -1; }
  virtual int ioport_setPinHigh(int port, int const_val) { return -1; }
  virtual int ioport_setPinLow(int port) { return -1; }
  virtual int ioport_setPinLow(int port, int const_val) { return -1; }
  virtual int ioport_isPinInputHigh(int port) { return -1; }
  virtual int ioport_getPortInputValue(int port) { return -1; }
  //virtual int ioport_setPortOutputValue(int port) { return -1; }

  // UART functions
  virtual int uart_init(int port) { return -1; }
  virtual int uart_send(int port) { return -1; }
  virtual int uart_read(int port) { return -1; }
  virtual int uart_isDataAvailable(int port) { return -1; }
  virtual int uart_isSendReady(int port) { return -1; }

  // SPI functions
  virtual int spi_init(int port) { return -1; }
  virtual int spi_send(int port) { return -1; }
  virtual int spi_read(int port) { return -1; }
  virtual int spi_isDataAvailable(int port) { return -1; }
  virtual int spi_isBusy(int port) { return -1; }
  virtual int spi_disable(int port) { return -1; }
  virtual int spi_enable(int port) { return -1; }

  // CPU functions
  virtual int cpu_setClock16() { return -1; }
  virtual int cpu_nop() { return -1; }

  // Memory
  virtual int memory_read8() { return -1; }
  virtual int memory_write8() { return -1; }
  virtual int memory_read16() { return -1; }
  virtual int memory_write16() { return -1; }

  // DSP (dsPIC stuff)
  virtual int dsp_getA() { return -1; }
  virtual int dsp_getB() { return -1; }
  virtual int dsp_getUpperA() { return -1; }
  virtual int dsp_getUpperB() { return -1; }
  virtual int dsp_getLowerA() { return -1; }
  virtual int dsp_getLowerB() { return -1; }
  virtual int dsp_getRoundedA() { return -1; }
  virtual int dsp_getRoundedB() { return -1; }
  virtual int dsp_clearA() { return -1; }
  virtual int dsp_clearB() { return -1; }
  virtual int dsp_loadA() { return -1; }
  virtual int dsp_loadB() { return -1; }
  virtual int dsp_negA() { return -1; }
  virtual int dsp_negB() { return -1; }
  virtual int dsp_addABAndStoreInA() { return -1; }
  virtual int dsp_addABAndStoreInB() { return -1; }
  virtual int dsp_subABAndStoreInA() { return -1; }
  virtual int dsp_subBAAndStoreInB() { return -1; }
  virtual int dsp_addToA() { return -1; }
  virtual int dsp_addToB() { return -1; }
  virtual int dsp_squareToA() { return -1; }
  virtual int dsp_squareToB() { return -1; }
  virtual int dsp_mulToA() { return -1; }
  virtual int dsp_mulToB() { return -1; }
  //virtual int dsp_euclideanDistanceToA() { return -1; }
  //virtual int dsp_euclideanDistanceToB() { return -1; }
  virtual int dsp_squareAndAddToA() { return -1; }
  virtual int dsp_squareAndAddToB() { return -1; }
  virtual int dsp_mulAndAddToA() { return -1; }
  virtual int dsp_mulAndAddToB() { return -1; }
  virtual int dsp_mulAndSubFromA() { return -1; }
  virtual int dsp_mulAndSubFromB() { return -1; }
  //virtual int dsp_euclideanDistanceAndAddToA() { return -1; }
  //virtual int dsp_euclideanDistanceAndAddToB() { return -1; }
  virtual int dsp_shiftA() { return -1; }
  virtual int dsp_shiftB() { return -1; }

protected:
  FILE *out;
  int label_count;
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

#if 0
enum
{
  CPU_INVALID,
  CPU_MSP430,
  CPU_DSPIC,
};
#endif

#endif

