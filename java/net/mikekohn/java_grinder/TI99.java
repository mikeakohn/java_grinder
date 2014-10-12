/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2014 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder;

abstract public class TI99
{
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
}

