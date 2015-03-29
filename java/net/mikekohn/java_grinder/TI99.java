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

abstract public class TI99
{
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

  protected TI99() { }

  //public static void clearScreen() { }
  public static void print(String text) { }
  public static void printChar(char c) { }
  public static void setCursor(int x, int y) { }
  public static void setGraphicsMode(int mode) { }
  public static void clearScreen() { }
  public static void plot(int x, int y, int color) { }

  /** For mode 0, remove characters and set colors for drawing */
  public static void setColors() { }

  /** Set sound frequency for a sound channel (voice).  The voice
      param can be between 0 to 2 for normal notes and 3 for a noise
      voice.  Anything out of that range will cause unknown results.
      The freq param is based on the table from this page:
      http://www.nouspikel.com/ti99/titechpages.htm */
  public static void setSoundFreq(int voice, int freq) { }

  /** Set sound volume for a sound channel (voice).  The voice
      param can be between 0 to 2 for normal notes and 3 for a noise
      voice.  Anything out of that range will cause unknown results.
      The volume can be between 0 and 15.  0 = loudest, 15 = sound is off. */
  public static void setSoundVolume(int voice, int volume) { }

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

