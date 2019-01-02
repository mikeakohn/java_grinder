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

#ifndef JAVA_GRINDER_GENERATOR_API_SEGA_GENESIS_H
#define JAVA_GRINDER_GENERATOR_API_SEGA_GENESIS_H

class API_SegaGenesis
{
public:
  // Sega Genesis
  virtual int sega_genesis_setPalettePointer_I() { return -1; }
  virtual int sega_genesis_setPalettePointer_I(int index) { return -1; }
  virtual int sega_genesis_setPaletteColor_I() { return -1; }
  virtual int sega_genesis_setPaletteColor_I(int color) { return -1; }
  virtual int sega_genesis_initBitmap() { return -1; }
  virtual int sega_genesis_clearBitmap() { return -1; }
  virtual int sega_genesis_clearPatterns_I() { return -1; }
  virtual int sega_genesis_plot_III() { return -1; }
  virtual int sega_genesis_setPlotAddress_I() { return -1; }
  virtual int sega_genesis_fastPlot_IIII() { return -1; }
  virtual int sega_genesis_inVerticalBlank() { return -1; }
  virtual int sega_genesis_waitVerticalBlank() { return -1; }
  virtual int sega_genesis_waitHorizontalBlank() { return -1; }
  virtual int sega_genesis_loadFonts() { return -1; }
  virtual int sega_genesis_clearText() { return -1; }
  virtual int sega_genesis_setCursor_II() { return -1; }
  virtual int sega_genesis_setCursor_II(int x, int y) { return -1; }
  virtual int sega_genesis_printChar_C() { return -1; }
  virtual int sega_genesis_printChar_C(int c) { return -1; }
  virtual int sega_genesis_print_X() { return -1; }
  virtual int sega_genesis_setHorizontalScrollA_II() { return -1; }
  virtual int sega_genesis_setHorizontalScrollB_II() { return -1; }
  virtual int sega_genesis_setVerticalScrollA_I() { return -1; }
  virtual int sega_genesis_setVerticalScrollB_I() { return -1; }
  virtual int sega_genesis_setHorizontalScrollModeLine() { return -1; }
  virtual int sega_genesis_setHorizontalScrollModeFull() { return -1; }
  virtual int sega_genesis_setPatternTable_aI() { return -1; }
  virtual int sega_genesis_setPatternTableAtIndex_IaI() { return -1; }
  virtual int sega_genesis_setPatternLocation_II() { return -1; }
  virtual int sega_genesis_putPattern_I() { return -1; }
  virtual int sega_genesis_putPattern_I(int c) { return -1; }
  virtual int sega_genesis_setImageData_aS() { return -1; }
  virtual int sega_genesis_setPaletteColors_aS() { return -1; }
  virtual int sega_genesis_setPaletteColorsAtIndex_IaS() { return -1; }
  virtual int sega_genesis_setSpritePosition_III() { return -1; }
  virtual int sega_genesis_setSpriteConfig1_II() { return -1; }
  virtual int sega_genesis_setSpriteConfig2_II() { return -1; }
  virtual int sega_genesis_getJoypadValuePort1() { return -1; }
  virtual int sega_genesis_getJoypadValuePort2() { return -1; }
  virtual int sega_genesis_loadZ80_aB() { return -1; }
  virtual int sega_genesis_resetZ80() { return -1; }
  virtual int sega_genesis_pauseZ80() { return -1; }
  virtual int sega_genesis_startZ80() { return -1; }

};

#endif

