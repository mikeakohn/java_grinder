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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "generator/SleepyBee.h"

SleepyBee::SleepyBee()
{
}

SleepyBee::~SleepyBee()
{
}

int SleepyBee::open(const char *filename)
{
  if (MCS51::open(filename) != 0) { return -1; }

  return 0;
}

int SleepyBee::start_init()
{
  MCS51::start_init();

  fprintf(out,
    "  ;; Disable watchdog.\n"
    "  anl 0xd9, #0xbf\n"
    "  mov 0xd9, A\n\n");

  return 0;
}

int SleepyBee::ioport_setPinsAsInput_I(int port)
{
  fprintf(out,
    "  ;; ioport_setPinsAsInput_I(%d)\n"
    "  mov A, %d\n"
    "  cpl A\n"
    "  anl 0x%02x, A\n",
    port,
    REG_ADDRESS_STACK_LO(--reg),
    0xa4 + port);

  return 0;
}

int SleepyBee::ioport_setPinsAsInput_I(int port, int const_val)
{
  fprintf(out,
    "  ;; ioport_setPinsAsInput_I(%d)\n"
    "  mov A, #%02x\n"
    "  cpl A\n"
    "  anl 0x%02x, A\n",
    port,
    const_val,
    0xa4 + port);

  return 0;
}

int SleepyBee::ioport_setPinsAsOutput_I(int port)
{
  fprintf(out,
    "  ;; ioport_setPinsAsOutput_I(%d)\n"
    "  mov A, %d\n"
    "  orl 0x%02x, A\n",
    port,
    REG_ADDRESS_STACK_LO(--reg),
    0xa4 + port);

  return 0;
}

int SleepyBee::ioport_setPinsAsOutput_I(int port, int const_val)
{
  // FIXME: How to take care of XBR1.
  fprintf(out, "  mov 0xe3, #0x40\n");

  fprintf(out,
    "  ;; ioport_setPinsAsOutput_I(%d)\n"
    "  mov A, #0x%02x\n"
    "  orl 0x%02x, A\n",
    port,
    const_val,
    0xa4 + port);

  return 0;
}

int SleepyBee::ioport_setPinAsOutput_I(int port)
{
  fprintf(out,
    "  ;; ioport_setPinAsOutput_I(%d)\n"
    "  mov A, 0\n"
    "  setb C\n"
    "label_%d:\n"
    "  rlc\n"
    "  djnz %d, label_%d\n"
    "  orl 0x%02x, A\n",
    port,
    label_count,
    REG_ADDRESS_STACK_LO(--reg), label_count,
    0xa4 + port);

  label_count++;

  return 0;
}

int SleepyBee::ioport_setPinAsOutput_I(int port, int const_val)
{
  fprintf(out,
    "  ;; ioport_setPinAsOutput_I(%d)\n"
    "  setb 0x%02x.%d\n",
    port,
    0xa4 + port, const_val);

  return 0;
}

int SleepyBee::ioport_setPinAsInput_I(int port)
{
  fprintf(out,
    "  ;; ioport_setPinAsInput_I(%d)\n"
    "  mov A, 0\n"
    "  setb C\n"
    "label_%d:\n"
    "  rlc\n"
    "  djnz %d, label_%d\n"
    "  cpl A\n"
    "  anl 0x%02x, A\n",
    port,
    label_count,
    REG_ADDRESS_STACK_LO(--reg), label_count,
    0xa4 + port);

  label_count++;

  return 0;
}

int SleepyBee::ioport_setPinAsInput_I(int port, int const_val)
{
  fprintf(out,
    "  ;; ioport_setPinAsInput_I(%d)\n"
    "  clr 0x%02x.%d\n",
    port,
    0xa4 + port, const_val);

  return 0;
}

