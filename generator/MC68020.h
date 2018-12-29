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

#ifndef _MC68020_H
#define _MC68020_H

#include "generator/MC68000.h"

class MC68020 : public MC68000
{
public:
  MC68020();
  virtual ~MC68020();

  virtual int integer_to_byte();
  virtual int array_read_byte();
  virtual int array_read_byte(std::string &name, int field_id);
  virtual int mul_integer();
  virtual int mul_integer(int num);
  virtual int div_integer();
  virtual int div_integer(int num);
  virtual int mod_integer();
  virtual int mod_integer(int num);
};

#endif

