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
}

