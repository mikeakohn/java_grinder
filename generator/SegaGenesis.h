/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2019 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_SEGA_GENESIS_H
#define JAVA_GRINDER_GENERATOR_SEGA_GENESIS_H

#include "generator/MC68000.h"

class SegaGenesis : public MC68000
{
public:
  SegaGenesis();
  virtual ~SegaGenesis();

  virtual int open(const char *filename);
  virtual int finish();
  virtual int start_init();

  virtual int sega_genesis_setPalettePointer_I();
  virtual int sega_genesis_setPalettePointer_I(int index);
  virtual int sega_genesis_setPaletteColor_I();
  virtual int sega_genesis_setPaletteColor_I(int color);
  virtual int sega_genesis_initBitmap();
  virtual int sega_genesis_clearBitmap();
  virtual int sega_genesis_clearPatterns_I();
  virtual int sega_genesis_plot_III();
  virtual int sega_genesis_setPlotAddress_I();
  virtual int sega_genesis_fastPlot_IIII();
  virtual int sega_genesis_inVerticalBlank();
  virtual int sega_genesis_waitVerticalBlank();
  virtual int sega_genesis_waitHorizontalBlank();
  virtual int sega_genesis_loadFonts();
  virtual int sega_genesis_clearText();
  virtual int sega_genesis_setCursor_II();
  virtual int sega_genesis_setCursor_II(int x, int y);
  virtual int sega_genesis_printChar_C();
  virtual int sega_genesis_printChar_C(int c);
  virtual int sega_genesis_print_X();
  virtual int sega_genesis_setHorizontalScrollA_II();
  virtual int sega_genesis_setHorizontalScrollB_II();
  virtual int sega_genesis_setVerticalScrollA_I();
  virtual int sega_genesis_setVerticalScrollB_I();
  virtual int sega_genesis_setHorizontalScrollModeLine();
  virtual int sega_genesis_setHorizontalScrollModeFull();
  virtual int sega_genesis_setPatternTable_aI();
  virtual int sega_genesis_setPatternTableAtIndex_IaI();
  virtual int sega_genesis_setPatternLocation_II();
  virtual int sega_genesis_putPattern_I();
  virtual int sega_genesis_putPattern_I(int c);
  virtual int sega_genesis_setImageData_aS();
  virtual int sega_genesis_setPaletteColors_aS();
  virtual int sega_genesis_setPaletteColorsAtIndex_IaS();
  virtual int sega_genesis_setSpritePosition_III();
  virtual int sega_genesis_setSpriteConfig1_II();
  virtual int sega_genesis_setSpriteConfig2_II();
  virtual int sega_genesis_getJoypadValuePort1();
  virtual int sega_genesis_getJoypadValuePort2();
  virtual int sega_genesis_loadZ80_aB();
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
  void add_set_palette_colors_at_index();
  void add_init_bitmap();
  void add_clear_bitmap();
  void add_clear_pattern();
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
  bool need_set_palette_colors_at_index:1;
  bool need_init_bitmap:1;
  bool need_clear_bitmap:1;
  bool need_clear_pattern:1;
  bool need_plot:1;
  bool need_set_plot_address:1;
};

#endif

