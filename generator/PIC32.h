/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 */

#ifndef _PIC32_H
#define _PIC32_H

#include "MIPS32.h"

class PIC32 : public MIPS32
{
public:
  PIC32();
  virtual ~PIC32();

  virtual int open(const char *filename);

  virtual int start_init();

  // GPIO functions
  virtual int ioport_setPinsAsInput_I(int port);
  virtual int ioport_setPinsAsOutput_I(int port);
  virtual int ioport_setPinsValue_I(int port);
  virtual int ioport_setPinsHigh_I(int port);
  virtual int ioport_setPinsLow_I(int port);
  virtual int ioport_setPinAsOutput_I(int port);
  virtual int ioport_setPinAsInput_I(int port);
  virtual int ioport_setPinHigh_I(int port);
  virtual int ioport_setPinLow_I(int port);
  virtual int ioport_isPinInputHigh_I(int port);
  virtual int ioport_getPortInputValue(int port);
  //virtual int ioport_setPortOutputValue_I(int port);

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

private:
  void add_spi_read();

  uint32_t need_spi_read : 1;
};

#endif

