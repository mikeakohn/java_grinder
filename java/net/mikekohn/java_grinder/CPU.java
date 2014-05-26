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

// This class is a bit awkward. Not sure how to make it better right now. 

public class CPU
{
  private CPU()
  {
  }

  /** Set clock speed to 16MHz (internal crystal? DCO?) */
  public static void setClock16() { }

  /** Set clock speed to 25MHz (internal crystal? DCO?) */
  public static void setClock25() { }

  /** Insert a CPU level NOP */
  public static void nop() { }
}

