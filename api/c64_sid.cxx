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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "api/cpu.h"

#define CHECK_FUNC(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return c64_sid_##funct(java_class, generator); \
  }

static int c64_sid_frequency1(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_frequency1();
}

static int c64_sid_pulseWidth1(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_pulseWidth1();
}

static int c64_sid_waveform1(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_waveform1();
}

static int c64_sid_adsr1(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_adsr1();
}

static int c64_sid_frequency2(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_frequency2();
}

static int c64_sid_pulseWidth2(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_pulseWidth2();
}

static int c64_sid_waveform2(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_waveform2();
}

static int c64_sid_adsr2(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_adsr2();
}

static int c64_sid_frequency3(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_frequency3();
}

static int c64_sid_pulseWidth3(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_pulseWidth3();
}

static int c64_sid_waveform3(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_waveform3();
}

static int c64_sid_adsr3(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_adsr3();
}

static int c64_sid_filterCutoff(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_filterCutoff();
}

static int c64_sid_filterResonance(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_filterResonance();
}

static int c64_sid_volume(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_volume();
}

static int c64_sid_oscillator3(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_oscillator3();
}

static int c64_sid_envelope3(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_envelope3();
}

static int c64_sid_clear(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_clear();
}

int c64_sid(JavaClass *java_class, Generator *generator, const char *function)
{
  CHECK_FUNC(frequency1)
  CHECK_FUNC(pulseWidth1)
  CHECK_FUNC(waveform1)
  CHECK_FUNC(adsr1)
  CHECK_FUNC(frequency2)
  CHECK_FUNC(pulseWidth2)
  CHECK_FUNC(waveform2)
  CHECK_FUNC(adsr2)
  CHECK_FUNC(frequency3)
  CHECK_FUNC(pulseWidth3)
  CHECK_FUNC(waveform3)
  CHECK_FUNC(adsr3)
  CHECK_FUNC(filterCutoff)
  CHECK_FUNC(filterResonance)
  CHECK_FUNC(volume)
  CHECK_FUNC(oscillator3)
  CHECK_FUNC(envelope3)
  CHECK_FUNC(clear)

  return -1;
}

