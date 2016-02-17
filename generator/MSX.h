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

#ifndef _MSX_H
#define _MSX_H

#include "Z80.h"

class MSX : public Z80
{
public:
  MSX();
  virtual ~MSX();

  virtual int open(const char *filename);
  virtual int msx_someFunction_I();

private:
  //void add_plot_lores();

  //uint32_t need_plot_lores : 1;
};

#endif

