/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

#ifndef _SEGA_GENESIS_H
#define _SEGA_GENESIS_H

#include "MC68000.h"

class SegaGenesis : public MC68000
{
public:
  SegaGenesis();
  virtual ~SegaGenesis();

  virtual int open(const char *filename);
  virtual int start_init();

protected:

private:
  void add_exception_vectors();
  void add_cartridge_info_header();
  void add_exception_handler();
  void add_set_fonts();
};

#endif

