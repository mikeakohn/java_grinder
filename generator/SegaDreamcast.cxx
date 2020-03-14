/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2020 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "generator/Math.h"
#include "generator/SegaDreamcast.h"

SegaDreamcast::SegaDreamcast()
{
  //org = 0x0c010000;
  org = 0x8c010000;
  ram_start = 0x00000000;
  ram_end = 32 * 1024 * 1024;
  //physical_address = 0x0;
  preload_array_align = 128;
}

SegaDreamcast::~SegaDreamcast()
{
}

int SegaDreamcast::open(const char *filename)
{
  if (SH4::open(filename) != 0) { return -1; }

  fprintf(out,
    ".include \"powervr.inc\"\n"
    ".entry_point start\n"
    ".export start\n\n");

  return 0;
}

int SegaDreamcast::finish()
{
  SH4::finish();

  //Math::add_sin_table(out);
  //Math::add_cos_table(out);

  return 0;
}

int SegaDreamcast::start_init()
{
  SH4::start_init();

  return 0;
}

int SegaDreamcast::new_object(std::string &object_name, int field_count)
{
  fprintf(out, "  ;; new_object(%s, field_count=%d)\n",
    object_name.c_str(), field_count);


  reg++;

  return 0;
}

