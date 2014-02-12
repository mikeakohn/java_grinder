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

#ifndef _DSPIC_H
#define _DSPIC_H

#include "Generator.h"

enum
{
  DSPIC30F3012,
  DSPIC33FJ06GS101A,
};

class DSPIC : public Generator
{
public:
  DSPIC(uint8_t chip_type);
  virtual ~DSPIC();

  virtual int open(char *filename);

  //virtual void serial_init();
  virtual void method_start(int local_count, const char *name);
  virtual void method_end(int local_count);
  virtual int push_integer(int32_t n);
  virtual int push_integer_local(int index);
  virtual int push_long(int64_t n);
  virtual int push_float(float f);
  virtual int push_double(double f);
  virtual int push_byte(int8_t b);
  virtual int push_short(int16_t s);
  virtual int pop_integer_local(int index);
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
  virtual int return_local(int index, int local_count);
  virtual int return_integer(int local_count);
  virtual int return_void(int local_count);
  virtual int jump(const char *name);
  virtual int call(const char *name);
  virtual int invoke_static_method(const char *name, int params, int is_void);
  virtual int brk();
  virtual int insert_array(const char *name, int32_t *data, int len, uint8_t type);
  //virtual void close();

  // GPIO functions
  virtual int ioport_setPinsAsInput(int port);
  virtual int ioport_setPinsAsInput(int port, int const_val);
  virtual int ioport_setPinsAsOutput(int port);
  virtual int ioport_setPinsAsOutput(int port, int const_val);
  virtual int ioport_setPinsValue(int port);
  virtual int ioport_setPinsHigh(int port);
  virtual int ioport_setPinsHigh(int port, int const_val);
  virtual int ioport_setPinsLow(int port);
  virtual int ioport_setPinsLow(int port, int const_val);
  virtual int ioport_setPinAsOutput(int port);
  virtual int ioport_setPinAsOutput(int port, int const_val);
  virtual int ioport_setPinAsInput(int port);
  virtual int ioport_setPinAsInput(int port, int const_val);
  virtual int ioport_setPinHigh(int port);
  virtual int ioport_setPinHigh(int port, int const_val);
  virtual int ioport_setPinLow(int port);
  virtual int ioport_setPinLow(int port, int const_val);
  virtual int ioport_isPinInputHigh(int port);
  virtual int ioport_getPortInputValue(int port);
  //virtual int ioport_setPortOutputValue(int port);

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

  // DSP (dsPIC stuff)
  virtual int dsp_getA();
  virtual int dsp_getB();
  virtual int dsp_getUpperA();
  virtual int dsp_getUpperB();
  virtual int dsp_getLowerA();
  virtual int dsp_getLowerB();
  virtual int dsp_getRoundedA();
  virtual int dsp_getRoundedB();
  virtual int dsp_clearA();
  virtual int dsp_clearB();
  virtual int dsp_loadA();
  virtual int dsp_loadB();
  virtual int dsp_negA();
  virtual int dsp_negB();
  virtual int dsp_addABAndStoreInA();
  virtual int dsp_addABAndStoreInB();
  virtual int dsp_subABAndStoreInA();
  virtual int dsp_subBAAndStoreInB();
  virtual int dsp_addToA();
  virtual int dsp_addToB();
  virtual int dsp_squareToA();
  virtual int dsp_squareToB();
  virtual int dsp_mulToA();
  virtual int dsp_mulToB();
  //virtual int dsp_euclideanDistanceToA();
  //virtual int dsp_euclideanDistanceToB();
  virtual int dsp_squareAndAddToA();
  virtual int dsp_squareAndAddToB();
  virtual int dsp_mulAndAddToA();
  virtual int dsp_mulAndAddToB();
  virtual int dsp_mulAndSubFromA();
  virtual int dsp_mulAndSubFromB();
  //virtual int dsp_euclideanDistanceAndAddToA();
  //virtual int dsp_euclideanDistanceAndAddToB();
  virtual int dsp_shiftA();
  virtual int dsp_shiftB();

private:
  int dsp_mul(const char *instr, const char *accum);
  int dsp_square(const char *instr, const char *accum);
  int dsp_store(const char *instr, const char *accum, int shift);
  void pop_reg(char *dst);
  //void push_w0();
  int set_periph(const char *instr, const char *periph, bool reverse=false);
  int stack_alu(const char *instr);
  int stack_alu_div();
  int stack_shift(const char *instr);
  int get_pin_number(int const_val);

  int reg;            // count number of registers are are using as stack
  int reg_max;        // size of register stack 
  int stack;          // count how many things we put on the stack
  uint8_t chip_type;
  bool is_main;
  bool need_stack_set;
  int flash_start;
};

#endif

