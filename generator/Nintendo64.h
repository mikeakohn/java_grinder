/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2021 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_NINTENDO64_H
#define JAVA_GRINDER_GENERATOR_NINTENDO64_H

#include "generator/R4000.h"

class Nintendo64 : public R4000
{
public:
  Nintendo64();
  virtual ~Nintendo64();

  virtual int open(const char *filename);
  virtual int finish();
  virtual int start_init();

private:
  void catridge_header();
  void init_system();
  void ntsc_setup();
};

#endif

