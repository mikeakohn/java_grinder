/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2017 by Michael Kohn
 *
 */

#ifndef _API_PARALLELLA_H
#define _API_PARALLELLA_H

class API_Parallella
{
public:
  // Parallella
  virtual int parallella_writeSharedRamByte_IB() { return -1; }
  virtual int parallella_writeSharedRamShort_IS() { return -1; }
  virtual int parallella_writeSharedRamInt_II() { return -1; }
  virtual int parallella_writeSharedRamFloat_IF() { return -1; }

  virtual int parallella_readSharedRamByte_I() { return -1; }
  virtual int parallella_readSharedRamShort_I() { return -1; }
  virtual int parallella_readSharedRamInt_I() { return -1; }
  //virtual int parallella_readSharedRamByte_I(int address) { return -1; }
  //virtual int parallella_readSharedRamShort_I(int address) { return -1; }
  //virtual int parallella_readSharedRamInt_I(int address) { return -1; }
};

#endif

