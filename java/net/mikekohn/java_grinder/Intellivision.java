/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2014-2021 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder;

abstract public class Intellivision
{
  protected Intellivision()
  {
  }

  // Data is:
  // Bits [ 12, 13, 10, 9] : Background color
  // Bits [ 11 ]           : GROM vs GRAM 
  // Bits [ 8 - 3 ]        : Card #
  // Bits [ 2 - 0 ]        : Foreground Color
  public static void plot(int data, int pos) { }
  public static void plot(int data, int x, int y) { }
}

