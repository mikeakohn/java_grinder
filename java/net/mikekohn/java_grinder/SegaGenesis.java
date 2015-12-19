/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder;

abstract public class SegaGenesis
{
  public static final int SPRITE_CONFIG1_HORIZONTAL_SIZE_1_CELL = 0x0000;
  public static final int SPRITE_CONFIG1_HORIZONTAL_SIZE_2_CELL = 0x0400;
  public static final int SPRITE_CONFIG1_HORIZONTAL_SIZE_3_CELL = 0x0800;
  public static final int SPRITE_CONFIG1_HORIZONTAL_SIZE_4_CELL = 0x0c00;

  public static final int SPRITE_CONFIG1_VERTICAL_SIZE_1_CELL = 0x0000;
  public static final int SPRITE_CONFIG1_VERTICAL_SIZE_2_CELL = 0x0100;
  public static final int SPRITE_CONFIG1_VERTICAL_SIZE_3_CELL = 0x0200;
  public static final int SPRITE_CONFIG1_VERTICAL_SIZE_4_CELL = 0x0300;

  public static final int SPRITE_CONFIG2_PRIORITY = 0x8000;

  public static final int SPRITE_CONFIG2_PALETTE_0 = 0x0000;
  public static final int SPRITE_CONFIG2_PALETTE_1 = 0x2000;
  public static final int SPRITE_CONFIG2_PALETTE_2 = 0x4000;
  public static final int SPRITE_CONFIG2_PALETTE_3 = 0x6000;

  public static final int SPRITE_CONFIG2_HORIZONTAL_FLIP = 0x0800;
  public static final int SPRITE_CONFIG2_VERTICAL_FLIP = 0x1000;

  public static final int JOYPAD_START = 0x2000;
  public static final int JOYPAD_A = 0x1000;
  public static final int JOYPAD_C = 0x0020;
  public static final int JOYPAD_B = 0x0010;
  public static final int JOYPAD_RIGHT = 0x0008;
  public static final int JOYPAD_LEFT = 0x0004;
  public static final int JOYPAD_DOWN = 0x0002;
  public static final int JOYPAD_UP = 0x0001;

  protected SegaGenesis() { }

  /** Set a the palette pointer.  Sets the palette pointer to the next
      palette color to change.  Must be called directly before calling
      setPaletteColor.  The index must be between 0 and 63 otherwise
      the behavior is unknown. */
  public static void setPalettePointer(int index) { }

  /** Set a palette color.  Can only be called after setPalettePointer is
      called.  After setting a color, the palette pointer is incremented
      by 1 so if the palette colors 0 to 3 need to be updated, first call
      setPalettePointer(0), and then call setPaletteColor() 4 times.  The
      color takes a binary format of 0000 BBB0 GGG0 RRR0. */
  public static void setPaletteColor(int color) { }

  /** Setup pattern table and display area for plotting pixels.  This method
      sets up the first 40*28 tiles to be patterns 0-1120. */
  public static void initBitmap() { }

  /** Clear pattern table (called after initBitmap to set entire display
      to palette color 0). */
  public static void clearBitmap() { }

  /** Clear "count" patterns in the pattern table.  Call setPlotAddress()
      first to point the VDP to the pattern to clear. */
  public static void clearPatterns(int count) { }

  /** Set a pixel in the 320x224 bitmap area of Scroll B.  In order for this
      to work, initBitmap() has to be called first. */
  public static void plot(int x, int y, int color) { }

  /** Set the VDP bitmap pointer to point to the next pattern address. */
  public static void setPlotAddress(int address) { }

  /** Set the next 4 pixels at the next word in the pattern table. */
  public static void fastPlot(int color0, int color1, int color2, int color3) { }

  /** Returns true if the VDP (and display) is in vertical blanking mode.
      The CPU will get faster access to the VDP at this point and changes
      to the display won't suffer from tearing and such. */
  public static boolean inVerticalBlank() { return false; }

  /** Waits for VDP to go into vertical blanking mode. */
  public static void waitVerticalBlank() { }

  /** Waits for VDP to go into horizontal blanking mode. */
  public static void waitHorizontalBlank() { }

  /** Load fonts. */
  public static void loadFonts() { }

  /** Clear the text window. */
  public static void clearText() { }

  /** Move the text cursor to where the next character will be printed. */
  public static void setCursor(int x, int y) { }

  /** Print a character to the screen.  Must call setCursor() first.
      The cursor will increment for every character printed. */
  public static void printChar(char c) { }

  /** Print a string of characters to the screen.  Must call
      setCursor() first.  The cursor will increment for every
      character printed. */
  public static void print(String text) { }

  /** Set horizontal scroll value for a specific row on the Scroll A screen
      (text). */
  public static void setHorizontalScrollA(int row, int value) { }

  /** Set horizontal scroll value for a specific row on the Scroll B screen
      (graphics). */
  public static void setHorizontalScrollB(int row, int value) { }

  /** Set vertical scroll value for entire Scroll A display (text) */
  public static void setVerticalScrollA(int value) { }

  /** Set vertical scroll value for entire Scroll B display (graphics). */
  public static void setVerticalScrollB(int value) { }

  /** Set scroll mode for Scroll A and B so each line can be shifted
      individually using setHorizontalScrollA/B().  This is the default
      mode. */
  public static void setHorizontalScrollModeLine() { }

  /** Set scroll mode for Scroll A and B so entire image is shifted
      left/right using setHorizontalScrollA/B() setting row to 0. */
  public static void setHorizontalScrollModeFull() { }

  /** Set pattern table. */
  public static void setPatternTable(int[] patterns) { }

  /** Set pattern table starting at an index. */
  public static void setPatternTableAtIndex(int index, int[] patterns) { }

  /** Move the cursor to where the next pattern will be printed.
      Note: x can be between 0 and 63 and y can be between 0 and 31.
      Any x above 39 will be off screen and can be scrolled in and any
      y above 27 will be off screen and can be scrolled in. */
  public static void setPatternLocation(int x, int y) { }

  /** After using setPatternLocation(), putPattern() will place an 8x8
      pattern (tile) in that location and increment X.  The index is
      actually a set of bit fields: 
      Bit     15: Priority flag (set to 1, this tile will be displayed on top)
      Bits 14-13: Palette number to use for this tile (0 to 3)
      Bit     12: Vertical flip this tile.
      Bit     11: Horizontal flip this tile.
      Bits  10-0: Pattern index to use for this tile. */
  public static void putPattern(int index) { }

  /** Set image data. */
  public static void setImageData(short[] image) { }

  /** Set palette colors. */
  public static void setPaletteColors(short[] palette) { }

  /** Set palette colors starting at an index (0 to 63).*/
  public static void setPaletteColorsAtIndex(int index, short[] palette) { }

  /** Set sprite's position on the screen.  Note: (0,0) on the display
      is sprite position (128,128).  If sprites aren't showing up make
      sure that x and y are bigger than 128. */
  public static void setSpritePosition(int index, int x, int y) { }

  /** Set sprite's x,y stretching and link field value.
      Bits 11-10: horizontal size value (0=1 cell, 1=2cell, 2=3cell, 3=4cells)
      Bits   9-8: vertical size value (0=1 cell, 1=2cell, 2=3cell, 3=4cells)
      Bits   7-0: link field value
      The link value points to the next sprite index to be displayed on the
      screen.  If the link value is 0, this is the last sprite to be drawn. */
  public static void setSpriteConfig1(int index, int value) { }

  /** Set sprite's priority, palette, vertical flip, horizontal flip,
      pattern start index.
      Bit     15: priority
      Bits 14-13: palette number
      Bit     12: vertical flip
      Bit     11: horizontal flip
      Bits  10-0: pattern start index
      */
  public static void setSpriteConfig2(int index, int value) { }

  /** Read which buttons are being pushed on the joypad in port 1.
      Use JOYPAD_ bitmask to check which puttons are down */
  public static int getJoypadValuePort1() { return 0; }

  /** Read which buttons are being pushed on the joypad in port 2.
      Use JOYPAD_ bitmask to check which puttons are down */
  public static int getJoypadValuePort2() { return 0; }

  /** Load code up to 8k in size into the Z80 process space.  Z80 will
      be reset by the API and code will start running. */
  public static void loadZ80(byte[] code) { }

  /** Reset the Z80. */
  public static void resetZ80() { }

  /** Pause Z80.  Needed so 68k can access something in Z80 space along
      with just pausing the thing. */
  public static void pauseZ80() { }

  /** Start Z80.  Let the Z80 run again. */
  public static void startZ80() { }
}

