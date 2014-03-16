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
#include <stdint.h>

class Generator
{
public:
  Generator();
  virtual ~Generator();

  virtual int open(const char *filename);
  void label(char *name);
  virtual int start_init() = 0;
  virtual int insert_static_field_define(const char *name, const char *type, int index) = 0;
  virtual int init_heap(int field_count) = 0;
  virtual int insert_field_init_boolean(char *name, int index, int value) = 0;
  virtual int insert_field_init_byte(char *name, int index, int value) = 0;
  virtual int insert_field_init_short(char *name, int index, int value) = 0;
  virtual int insert_field_init_int(char *name, int index, int value) = 0;
  virtual int insert_field_init(char *name, int index) = 0;
  virtual void method_start(int local_count, int max_stack, int param_count, const char *name) = 0;
  virtual void method_end(int local_count) = 0;
  virtual int push_integer(int32_t n) = 0;
  virtual int push_integer_local(int index) = 0;
  virtual int push_ref_local(int index) = 0;
  virtual int push_fake() { return -1; } // move stack ptr without push
  virtual int set_integer_local(int index, int value) { return -1; }
  virtual int push_long(int64_t n) = 0;
  virtual int push_float(float f) = 0;
  virtual int push_double(double f) = 0;
  virtual int push_byte(int8_t b) = 0;
  virtual int push_short(int16_t s) = 0;
  virtual int pop_integer_local(int index) = 0;
  virtual int pop_ref_local(int index) = 0;
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
  virtual int jump_cond(const char *label, int cond) = 0;
  virtual int jump_cond_integer(const char *label, int cond) = 0;
  virtual int jump_cond_integer(const char *label, int cond, int const_val) { return -1; } 
  virtual int return_local(int index, int local_count) = 0;
  virtual int return_integer(int local_count) = 0;
  virtual int return_void(int local_count) = 0;
  virtual int jump(const char *name) = 0;
  virtual int call(const char *name) = 0;
  virtual int invoke_static_method(const char *name, int params, int is_void) = 0;
  virtual int put_static(const char *name, int index) = 0;
  virtual int get_static(const char *name, int index) = 0;
  virtual int brk() = 0;
  virtual int new_array(uint8_t type) = 0;
  virtual int insert_array(const char *name, int32_t *data, int len, uint8_t type) = 0;
  virtual int insert_array(const char *name, char *data) = 0;
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
  virtual int spi_init(int port, int clock_divisor, int mode) { return -1; }
  virtual int spi_send(int port) { return -1; }
  virtual int spi_read(int port) { return -1; }
  virtual int spi_isDataAvailable(int port) { return -1; }
  virtual int spi_isBusy(int port) { return -1; }
  virtual int spi_disable(int port) { return -1; }
  virtual int spi_enable(int port) { return -1; }

  // ADC functions
  virtual int adc_enable() { return -1; }
  virtual int adc_disable() { return -1; }
  virtual int adc_setChannel_I() { return -1; }
  virtual int adc_setChannel_I(int channel) { return -1; }
  virtual int adc_read() { return -1; }

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

  // Commodore 64 stuff
  virtual int c64_sid_voice1_frequency() { return -1; }
  virtual int c64_sid_voice1_pulse_width() { return -1; }
  virtual int c64_sid_voice1_waveform() { return -1; }
  virtual int c64_sid_voice1_adsr() { return -1; }
  virtual int c64_sid_voice2_frequency() { return -1; }
  virtual int c64_sid_voice2_pulse_width() { return -1; }
  virtual int c64_sid_voice2_waveform() { return -1; }
  virtual int c64_sid_voice2_adsr() { return -1; }
  virtual int c64_sid_voice3_frequency() { return -1; }
  virtual int c64_sid_voice3_pulse_width() { return -1; }
  virtual int c64_sid_voice3_waveform() { return -1; }
  virtual int c64_sid_voice3_adsr() { return -1; }
  virtual int c64_sid_filter_cutoff() { return -1; }
  virtual int c64_sid_filter_resonance() { return -1; }
  virtual int c64_sid_volume() { return -1; }
  virtual int c64_sid_voice3_oscillator() { return -1; }
  virtual int c64_sid_voice3_envelope() { return -1; }
  virtual int c64_sid_clear() { return -1; }

  virtual int c64_vic_sprite0pos() { return -1; }
  virtual int c64_vic_sprite1pos() { return -1; }
  virtual int c64_vic_sprite2pos() { return -1; }
  virtual int c64_vic_sprite3pos() { return -1; }
  virtual int c64_vic_sprite4pos() { return -1; }
  virtual int c64_vic_sprite5pos() { return -1; }
  virtual int c64_vic_sprite6pos() { return -1; }
  virtual int c64_vic_sprite7pos() { return -1; }
  virtual int c64_vic_write_control1() { return -1; }
  virtual int c64_vic_read_control1() { return -1; }
  virtual int c64_vic_wait_raster() { return -1; }
  virtual int c64_vic_sprite_enable() { return -1; }
  virtual int c64_vic_write_control2() { return -1; }
  virtual int c64_vic_read_control2() { return -1; }
  virtual int c64_vic_sprite_expandy() { return -1; }
  virtual int c64_vic_write_pointer() { return -1; }
  virtual int c64_vic_read_pointer() { return -1; }
  virtual int c64_vic_sprite_priority() { return -1; }
  virtual int c64_vic_sprite_multicolor_enable() { return -1; }
  virtual int c64_vic_sprite_expandx() { return -1; }
  virtual int c64_vic_sprite_collision() { return -1; }
  virtual int c64_vic_data_collision() { return -1; }
  virtual int c64_vic_border() { return -1; }
  virtual int c64_vic_background() { return -1; }
  virtual int c64_vic_background1() { return -1; }
  virtual int c64_vic_background2() { return -1; }
  virtual int c64_vic_background3() { return -1; }
  virtual int c64_vic_sprite_multicolor0() { return -1; }
  virtual int c64_vic_sprite_multicolor1() { return -1; }
  virtual int c64_vic_sprite0color() { return -1; }
  virtual int c64_vic_sprite1color() { return -1; }
  virtual int c64_vic_sprite2color() { return -1; }
  virtual int c64_vic_sprite3color() { return -1; }
  virtual int c64_vic_sprite4color() { return -1; }
  virtual int c64_vic_sprite5color() { return -1; }
  virtual int c64_vic_sprite6color() { return -1; }
  virtual int c64_vic_sprite7color() { return -1; }

  void add_newline();

protected:
  int insert_db(const char *name, int32_t *data, int len, uint8_t len_type);
  int insert_dw(const char *name, int32_t *data, int len, uint8_t len_type);
  int insert_dc32(const char *name, int32_t *data, int len, uint8_t len_type);
  int get_constant(uint32_t constant);
  void write_constants();

  FILE *out;
  int label_count;
  int constants_len;
  uint32_t constants[16384];
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

