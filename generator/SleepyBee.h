/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2022 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_SLEEPY_BEE_H
#define JAVA_GRINDER_GENERATOR_SLEEPY_BEE_H

#include "generator/MCS51.h"

class SleepyBee : public MCS51
{
public:
  SleepyBee();
  virtual ~SleepyBee();

  virtual int open(const char *filename);
  virtual int start_init();
  virtual int finish();

  virtual int new_array(uint8_t type);
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

  // GPIO methods.
  virtual int ioport_setPinsAsInput_I(int port);
  virtual int ioport_setPinsAsInput_I(int port, int const_val);
  virtual int ioport_setPinsAsOutput_I(int port);
  virtual int ioport_setPinsAsOutput_I(int port, int const_val);
  virtual int ioport_setPinAsOutput_I(int port);
  virtual int ioport_setPinAsOutput_I(int port, int const_val);
  virtual int ioport_setPinAsInput_I(int port);
  virtual int ioport_setPinAsInput_I(int port, int const_val);

  // SPI functions.
  virtual int spi_init_II(int port);
  virtual int spi_init_II(int port, int clock_divisor, int mode);
  virtual int spi_send_I(int port);
  virtual int spi_read_I(int port);
  virtual int spi_isDataAvailable(int port);
  virtual int spi_isBusy(int port);
  virtual int spi_disable(int port);
  virtual int spi_enable(int port);

  // ADC functions.
  virtual int adc_enable();
  virtual int adc_disable();
  virtual int adc_setChannel_I();
  virtual int adc_setChannel_I(int channel);
  virtual int adc_read();

private:
  int add_new_array_byte();
  int add_new_array_int();

  bool need_new_array_byte : 1;
  bool need_new_array_int : 1;
};

#endif

