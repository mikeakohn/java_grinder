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

#ifndef _API_MICROCONTROLLER_H
#define _API_MICROCONTROLLER_H

class API_Microcontroller
{
public:
  // GPIO functions
  virtual int ioport_setPinsAsInput_I(int port) { return -1; }
  virtual int ioport_setPinsAsInput_I(int port, int const_val) { return -1; }
  virtual int ioport_setPinsAsOutput_I(int port) { return -1; }
  virtual int ioport_setPinsAsOutput_I(int port, int const_val) { return -1; }
  virtual int ioport_setPinsValue_I(int port) { return -1; }
  virtual int ioport_setPinsValue_I(int port, int const_val) { return -1; }
  virtual int ioport_setPinsHigh_I(int port) { return -1; }
  virtual int ioport_setPinsHigh_I(int port, int const_val) { return -1; }
  virtual int ioport_setPinsLow_I(int port) { return -1; }
  virtual int ioport_setPinsLow_I(int port, int const_val) { return -1; }
  virtual int ioport_setPinAsOutput_I(int port) { return -1; }
  virtual int ioport_setPinAsOutput_I(int port, int const_val) { return -1; }
  virtual int ioport_setPinAsInput_I(int port) { return -1; }
  virtual int ioport_setPinAsInput_I(int port, int const_val) { return -1; }
  virtual int ioport_setPinHigh_I(int port) { return -1; }
  virtual int ioport_setPinHigh_I(int port, int const_val) { return -1; }
  virtual int ioport_setPinLow_I(int port) { return -1; }
  virtual int ioport_setPinLow_I(int port, int const_val) { return -1; }
  virtual int ioport_isPinInputHigh_I(int port) { return -1; }
  virtual int ioport_getPortInputValue(int port) { return -1; }
  //virtual int ioport_setPortOutputValue(int port) { return -1; }
  virtual int ioport_setPinsResistorEnable_I(int port) { return -1; }
  virtual int ioport_setPinsResistorEnable_I(int port, int const_val) { return -1; }

  // UART functions
  virtual int uart_init_I(int port) { return -1; }
  virtual int uart_init_I(int port, int baud_rate) { return -1; }
  virtual int uart_send_I(int port) { return -1; }
  virtual int uart_read(int port) { return -1; }
  virtual int uart_isDataAvailable(int port) { return -1; }
  virtual int uart_isSendReady(int port) { return -1; }

  // SPI functions
  virtual int spi_init_II(int port) { return -1; }
  virtual int spi_init_II(int port, int clock_divisor, int mode) { return -1; }
  virtual int spi_init16_II(int port) { return -1; }
  virtual int spi_init16_II(int port, int clock_divisor, int mode) { return -1; }
  virtual int spi_send_I(int port) { return -1; }
  virtual int spi_send16_I(int port) { return -1; }
  virtual int spi_read_I(int port) { return -1; }
  virtual int spi_read16_I(int port) { return -1; }
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

  // Timer functions
  virtual int timer_setInterval_II() { return -1; }
  virtual int timer_setInterval_II(int cycles, int divider) { return -1; }
  virtual int timer_setListener_Z() { return -1; }
  virtual int timer_setListener_Z(int const_value) { return -1; }
  virtual int timer_getValue() { return -1; }
  virtual int timer_setValue_I() { return -1; }
  virtual int timer_setValue_I(int const_value) { return -1; }

  // Watchdog
  virtual int watchdog_enable() { return -1; }
  virtual int watchdog_disable() { return -1; }
  virtual int watchdog_kick() { return -1; }
};

#endif

