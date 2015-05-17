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

  /** Load fonts. */
  public static void loadFonts() { }

  /** Move the text cursor to where the next character will be printed. */
  public static void setCursor(int x, int y) { }

  /** Print a character to the screen.  Must call setCursor() first.
      The cursor will increment for every character printed. */
  public static void printChar(char c) { }
}

