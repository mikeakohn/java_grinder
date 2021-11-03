/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2021 by Michael Kohn
 *
 * V.Smile by Simon Eriksson
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_VSMILE_H
#define JAVA_GRINDER_GENERATOR_VSMILE_H

#include "generator/UNSP.h"

class VSmile : public UNSP
{
public:
  VSmile();
  virtual ~VSmile();

  virtual int open(const char *filename);

private:
};

#endif

