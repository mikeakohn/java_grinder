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

  virtual int sega_genesis_setPalettePointer();
  virtual int sega_genesis_setPalettePointer(int index);
  virtual int sega_genesis_setPaletteColor();
  virtual int sega_genesis_setPaletteColor(int color);
  virtual int sega_genesis_loadFonts();
  virtual int sega_genesis_setCursor();
  virtual int sega_genesis_setCursor(int x, int y);
  virtual int sega_genesis_printChar();
  virtual int sega_genesis_printChar(int c);
  virtual int sega_genesis_print();
  virtual int sega_genesis_setHorizontalScroll();
  virtual int sega_genesis_setVerticalScroll();

protected:

private:
  void add_exception_vectors();
  void add_cartridge_info_header();
  void add_exception_handler();
  void add_load_fonts();
  void add_vdp_reg_init();
  void add_print_string();

  bool need_print_string:1;
  bool need_load_fonts:1;
};

#endif

