/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2023 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder;

abstract public class TMS9918A
{
  public static final int COLOR_TRANSPARENT  = 0;
  public static final int COLOR_BLACK        = 1;
  public static final int COLOR_MEDIUM_GREEN = 2;
  public static final int COLOR_LIGHT_GREEN  = 3;
  public static final int COLOR_DARK_BLUE    = 4;
  public static final int COLOR_LIGHT_BLUE   = 5;
  public static final int COLOR_DARK_RED     = 6;
  public static final int COLOR_CYAN         = 7;
  public static final int COLOR_MEDIUM_RED   = 8;
  public static final int COLOR_LIGHT_RED    = 9;
  public static final int COLOR_DARK_YELLOW  = 10;
  public static final int COLOR_LIGHT_YELLOW = 11;
  public static final int COLOR_DARK_GREEN   = 12;
  public static final int COLOR_MAGENTA      = 13;
  public static final int COLOR_GRAY         = 14;
  public static final int COLOR_WHITE        = 15;

  public static final int SPRITE_SIZE_8X8_SMALL = 0;
  public static final int SPRITE_SIZE_8X8_BIG = 1;
  public static final int SPRITE_SIZE_16X16_SMALL = 2;
  public static final int SPRITE_SIZE_16X16_BIG = 3;

  /** Standard Mode (Text) (32x24) */
  public static final int MODE_0 = 0;
  /** Text Mode (40x24) */
  public static final int MODE_1 = 1;
  /** Multicolor mode (64x48) */
  public static final int MODE_2 = 2;
  /** Bitmap mode (?) */
  public static final int MODE_3 = 3;

  protected TMS9918A() { }

  //public static void clearScreen() { }
  public static void print(String text) { }
  public static void printChar(char c) { }
  public static void setCursor(int x, int y) { }
  public static void setGraphicsMode(int mode) { }
  public static void clearScreen() { }
  public static void plot(int x, int y, int color) { }

  /** For mode 0, remove characters and set colors for drawing */
  public static void initDisplay() { }

  /** Change visibility of a sprite.  Sprite index can be between 0 and 32.
      there are restrictions on the number of sprites that can be displayed
      on a line. */
  public static void setSpriteVisible(int index, boolean visible) { }

  /** Sets what a sprite looks like.  Takes an array of 8 or 32 bytes. */
  public static void setSpriteImage(int index, byte[] image) { }

  /** Sets the position on the screen where a sprite is seen. */
  public static void setSpritePos(int index, int x, int y) { }

  /** Sets the color (between 0 and 15) of the sprite. */
  public static void setSpriteColor(int index, int color) { }

  /** Sets if the sprite is 8x8 or 16x16 pixels. */
  public static void setSpriteSize(int size) { }
}

