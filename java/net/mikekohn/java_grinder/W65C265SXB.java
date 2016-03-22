/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 * W65C265SXB by Joe Davisson
 *
 */

package net.mikekohn.java_grinder;

abstract public class W65C265SXB
{
  protected W65C265SXB() { }

  // terminal interface routines
  public static char getChar() { return 0; }
  public static void putChar(char c) { }
  public static int getInt() { return 0; }
  public static void putInt(int n) { }
  public static void getString() { }
  public static void putString(String text) { }
  public static void setToneEnable(boolean channel1, boolean channel2) { }
  public static void setToneFreq(int freq1, int freq2) { }
}

