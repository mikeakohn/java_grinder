/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "PIC32.h"

PIC32::PIC32()
{
  org = 0x9d000000;
  ram_start = 0x80000000;
  ram_end = 0x80003fff;
}

PIC32::~PIC32()
{
}

int PIC32::open(const char *filename)
{
  if (MIPS32::open(filename) != 0) { return -1; }

  return 0;
}

// GPIO functions
int PIC32::ioport_setPinsAsInput(int port) { return -1; }
int PIC32::ioport_setPinsAsOutput(int port) { return -1; }
int PIC32::ioport_setPinsValue(int port) { return -1; }
int PIC32::ioport_setPinsHigh(int port) { return -1; }
int PIC32::ioport_setPinsLow(int port) { return -1; }
int PIC32::ioport_setPinAsOutput(int port) { return -1; }
int PIC32::ioport_setPinAsInput(int port) { return -1; }
int PIC32::ioport_setPinHigh(int port) { return -1; }
int PIC32::ioport_setPinLow(int port) { return -1; }
int PIC32::ioport_isPinInputHigh(int port) { return -1; }
int PIC32::ioport_getPortInputValue(int port) { return -1; }
//int PIC32::ioport_setPortOutputValue(int port) { return -1; }

