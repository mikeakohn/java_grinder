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

#ifndef JAVA_GRINDER_GENERATOR_SEGA_DREAMCAST_H
#define JAVA_GRINDER_GENERATOR_SEGA_DREAMCAST_H

#include "generator/SH4.h"

class SegaDreamcast : public SH4
{
public:
  SegaDreamcast();
  virtual ~SegaDreamcast();

  virtual int open(const char *filename);
  virtual int finish();
  virtual int start_init();
  virtual int new_object(std::string &object_name, int field_count);

private:
  void init_system();
  void init_video();
};

#endif

