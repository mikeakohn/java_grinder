/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 * W65C265SXB by Joe Davisson
 *
 */

#ifndef _W65C265SXB_H
#define _W65C265SXB_H

#include "W65816.h"

class W65C265SXB : public W65816
{
public:
  W65C265SXB();
  virtual ~W65C265SXB();

  virtual int open(const char *filename);

  // terminal interface API
  virtual int w65c265sxb_getChar();
  virtual int w65c265sxb_putChar_C();
  virtual int w65c265sxb_getInt();
  virtual int w65c265sxb_putInt_I();
  virtual int w65c265sxb_getString();
  virtual int w65c265sxb_putString();

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
  virtual int ioport_setPinHigh(int port, int const_val);
  virtual int ioport_setPinLow(int port);
  virtual int ioport_setPinLow(int port, int const_val);
  virtual int ioport_isPinInputHigh(int port);
  virtual int ioport_getPortInputValue(int port);
  //virtual int ioport_setPortOutputValue(int port);


private:
  bool need_put_string:1;

  void insert_put_string();
};

#endif

