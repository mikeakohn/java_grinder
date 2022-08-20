/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2022 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder;

abstract public class NES
{
  protected NES() { }

  public static void setBackgroundPalette(int index, int color) { }
  public static void setSpritePalette(int index, int color) { }
  public static void setPattern(int index, byte[] data) { }
  public static void setNameTable(int location, int pattern_index) { }
  public static void setNameTable(int x, int y, int pattern_index) { }
  public static void setScroll(int x, int y) { }
  public static void waitVerticalBlank() { }

  public static final int NAME_TABLE_0 = 0;
  public static final int NAME_TABLE_1 = 1;
  public static final int NAME_TABLE_2 = 2;
  public static final int NAME_TABLE_3 = 3;
  public static final int SPRITE_PATTERN_TABLE_0 = 0;
  public static final int SPRITE_PATTERN_TABLE_1 = 8;
  public static final int BACKGROUND_PATTERN_TABLE_0 = 0;
  public static final int BACKGROUND_PATTERN_TABLE_1 = 16;
  public static final int SPRITE_SIZE_8x8 = 0;
  public static final int SPRITE_SIZE_8x16 = 32;

  /** Sets up the PPU ctrl register. This allows setting which name_table
      to use (using the NAME_TABLE_ constants), the sprite pattern table
      to use (using the SPRITE_PATTERN_TABLE_ constants), the background
      pattern table to use (using BACKGROUND_PATTERN_TABLE_ constants),
      and the sprite size using the (using the SPRITE_SIZE_ constants). */
  public static void setPPUCtrl(int value) { }

  public static final int MASK_SHOW_BACKGROUND_LEFT = 2;
  public static final int MASK_SHOW_SPRITES_LEFT = 4;
  public static final int MASK_SHOW_BACKGROUND = 8;
  public static final int MASK_SHOW_SPRITES = 16;

  /** Setup the PPU mask register. The constants that start with MASK_
      can be used to set this up. Default value is 0x0e which is:
      MASK_SHOW_BACKGROUND | MASK_SHOW_SPRITES_LEFT| MASK_SHOW_BACKGROUND_LEFT.
    */
  public static void setPPUMask(int value) { }
}

