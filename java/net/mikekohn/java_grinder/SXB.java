/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPLv3
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 * Western Design Center SXB board by Joe Davisson
 *
 */

package net.mikekohn.java_grinder;

abstract public class SXB
{
  protected SXB() { }

  // terminal interface routines
  public static char getChar() { return 0; }
  public static void putChar(char c) { }
  public static int getInt() { return 0; }
  public static void putInt(int n) { }
  public static void getString() { }
  public static void putString(String text) { }
  public static void controlTones(int freq1, int freq2,
                                  boolean enable1, boolean enable2) { }
}

