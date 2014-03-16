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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "JavaClass.h"
#include "cpu.h"

#define CHECK_FUNC(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return c64_sid_##funct(java_class, generator); \
  }

static int c64_sid_voice1_frequency(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_voice1_frequency();
}

static int c64_sid_voice1_pulse_width(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_voice1_pulse_width();
}

static int c64_sid_voice1_waveform(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_voice1_waveform();
}

static int c64_sid_voice1_adsr(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_voice1_adsr();
}

static int c64_sid_voice2_frequency(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_voice2_frequency();
}

static int c64_sid_voice2_pulse_width(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_voice2_pulse_width();
}

static int c64_sid_voice2_waveform(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_voice2_waveform();
}

static int c64_sid_voice2_adsr(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_voice2_adsr();
}

static int c64_sid_voice3_frequency(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_voice3_frequency();
}

static int c64_sid_voice3_pulse_width(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_voice3_pulse_width();
}

static int c64_sid_voice3_waveform(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_voice3_waveform();
}

static int c64_sid_voice3_adsr(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_voice3_adsr();
}

static int c64_sid_filter_cutoff(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_filter_cutoff();
}

static int c64_sid_filter_resonance(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_filter_resonance();
}

static int c64_sid_volume(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_volume();
}

static int c64_sid_voice3_oscillator(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_voice3_oscillator();
}

static int c64_sid_voice3_envelope(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_voice3_envelope();
}

static int c64_sid_clear(JavaClass *java_class, Generator *generator)
{
  return generator->c64_sid_clear();
}

int c64_sid(JavaClass *java_class, Generator *generator, char *function)
{
  CHECK_FUNC(voice1_frequency)
  CHECK_FUNC(voice1_pulse_width)
  CHECK_FUNC(voice1_waveform)
  CHECK_FUNC(voice1_adsr)
  CHECK_FUNC(voice2_frequency)
  CHECK_FUNC(voice2_pulse_width)
  CHECK_FUNC(voice2_waveform)
  CHECK_FUNC(voice2_adsr)
  CHECK_FUNC(voice3_frequency)
  CHECK_FUNC(voice3_pulse_width)
  CHECK_FUNC(voice3_waveform)
  CHECK_FUNC(voice3_adsr)
  CHECK_FUNC(filter_cutoff)
  CHECK_FUNC(filter_resonance)
  CHECK_FUNC(volume)
  CHECK_FUNC(voice3_oscillator)
  CHECK_FUNC(voice3_envelope)
  CHECK_FUNC(clear)

  return -1;
}



