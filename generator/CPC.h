/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 * CPC support by Carsten Dost  - https://github.com/deringenieur71
 *
 */

#ifndef _CPC_H
#define _CPC_H

#include "Z80.h"

class CPC : public Z80
{
public:
  CPC();
  virtual ~CPC();
  virtual int open(const char *filename);
  virtual int cpc_getFunction_IC();

private:
  //void insert_ldirvv(void);

  //uint32_t need_plot_lores : 1;
};

#endif

