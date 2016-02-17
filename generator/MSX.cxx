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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "MSX.h"

MSX::MSX()
  //need_plot_lores(0),
{
  // Cartridge ROM starts at 0xc0000.  RAM would start at 0x0600.
  //start_org = 0xc000;
  //ram_start = 0x0600;
  //ram_end = 0x1fff;
}

MSX::~MSX()
{
  //if (need_plot_lores) { add_plot_lores(); }
}

int MSX::open(const char *filename)
{
  if (Z80::open(filename) != 0) { return -1; }

  //fprintf(out, "  _sound_waveform_start equ 0\n");
  //fprintf(out, "  _sound_waveform_end equ 2\n");
  //fprintf(out, "  _sound_waveform_ptr equ 4\n");

  return 0;
}

int MSX::msx_someFunction_I()
{
  //need_clear_screen_lores = 1;

  //fprintf(out, "  ; clearScreenLores()\n");
  //fprintf(out, "  jsr clear_screen_lores\n");

  return 0;
}

