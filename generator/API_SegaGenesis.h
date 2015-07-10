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
  virtual int sega_genesis_setPatternTable() { return -1; }
  virtual int sega_genesis_setImageData() { return -1; }
  virtual int sega_genesis_setPaletteColors() { return -1; }
  virtual int sega_genesis_loadZ80() { return -1; }
  virtual int sega_genesis_resetZ80() { return -1; }
  virtual int sega_genesis_pauseZ80() { return -1; }
  virtual int sega_genesis_startZ80() { return -1; }

};

#endif

