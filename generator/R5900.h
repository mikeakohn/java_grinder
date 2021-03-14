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

#ifndef JAVA_GRINDER_GENERATOR_R5900_H
#define JAVA_GRINDER_GENERATOR_R5900_H

#include "generator/R4000.h"

class R5900 : public R4000
{
public:
  R5900();
  virtual ~R5900();

  virtual int open(const char *filename);
  virtual int start_init();
};

#endif

