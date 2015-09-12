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
  virtual int sega_genesis_initBitmap();
  virtual int sega_genesis_clearBitmap();
  virtual int sega_genesis_plot();
  virtual int sega_genesis_setPlotAddress();
  virtual int sega_genesis_fastPlot();
  virtual int sega_genesis_inVerticalBlank();
  virtual int sega_genesis_waitVerticalBlank();
  virtual int sega_genesis_waitHorizontalBlank();
  virtual int sega_genesis_loadFonts();
  virtual int sega_genesis_clearText();
  virtual int sega_genesis_setCursor();
  virtual int sega_genesis_setCursor(int x, int y);
  virtual int sega_genesis_printChar();
  virtual int sega_genesis_printChar(int c);
  virtual int sega_genesis_print();
  virtual int sega_genesis_setHorizontalScrollA();
  virtual int sega_genesis_setHorizontalScrollB();
  virtual int sega_genesis_setVerticalScrollA();
  virtual int sega_genesis_setVerticalScrollB();
  virtual int sega_genesis_setPatternTable();
  virtual int sega_genesis_setImageData();
  virtual int sega_genesis_setPaletteColors();
  virtual int sega_genesis_setSpritePosition();
  virtual int sega_genesis_setSpriteConfig1();
  virtual int sega_genesis_setSpriteConfig2();
  virtual int sega_genesis_loadZ80();
  virtual int sega_genesis_resetZ80();
  virtual int sega_genesis_pauseZ80();
  virtual int sega_genesis_startZ80();

protected:

private:
  void add_exception_vectors();
  void add_cartridge_info_header();
  void add_exception_handler();
  void add_load_fonts();
  void add_clear_text();
  void add_vdp_reg_init();
  void add_print_string();
  void add_load_z80();
  void add_set_pattern_table();
  void add_set_image_data();
  void add_set_palette_colors();
  void add_init_bitmap();
  void add_clear_bitmap();
  void add_plot();
  void add_set_plot_address();

  uint16_t sprite_attribute_table;     // address of 640 byte table
  bool need_print_string:1;
  bool need_load_fonts:1;
  bool need_clear_text:1;
  bool need_load_z80:1;
  bool need_set_pattern_table:1;
  bool need_set_image_data:1;
  bool need_set_palette_colors:1;
  bool need_init_bitmap:1;
  bool need_clear_bitmap:1;
  bool need_plot:1;
  bool need_set_plot_address:1;
};

#endif

