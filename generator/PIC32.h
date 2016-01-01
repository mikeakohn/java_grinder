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
  virtual int ioport_setPinsAsInput(int port);
  virtual int ioport_setPinsAsOutput(int port);
  virtual int ioport_setPinsValue(int port);
  virtual int ioport_setPinsHigh(int port);
  virtual int ioport_setPinsLow(int port);
  virtual int ioport_setPinAsOutput(int port);
  virtual int ioport_setPinAsInput(int port);
  virtual int ioport_setPinHigh(int port);
  virtual int ioport_setPinLow(int port);
  virtual int ioport_isPinInputHigh(int port);
  virtual int ioport_getPortInputValue(int port);
  //virtual int ioport_setPortOutputValue(int port);

private:
};

#endif

