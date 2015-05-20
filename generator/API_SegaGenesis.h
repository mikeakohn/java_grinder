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

#ifndef _API_SEGA_GENESIS_H
#define _API_SEGA_GENESIS_H

class API_SegaGenesis
{
public:
  // Sega Genesis
  virtual int sega_genesis_setPalettePointer() { return -1; }
  virtual int sega_genesis_setPalettePointer(int index) { return -1; }
  virtual int sega_genesis_setPaletteColor() { return -1; }
  virtual int sega_genesis_setPaletteColor(int color) { return -1; }
  virtual int sega_genesis_loadFonts() { return -1; }
  virtual int sega_genesis_setCursor() { return -1; }
  virtual int sega_genesis_setCursor(int x, int y) { return -1; }
  virtual int sega_genesis_printChar() { return -1; }
  virtual int sega_genesis_printChar(int c) { return -1; }
  virtual int sega_genesis_print() { return -1; }
  virtual int sega_genesis_setHorizontalScroll() { return -1; }
  virtual int sega_genesis_setVerticalScroll() { return -1; }
};

#endif

