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

  /** Set clock speed to XT2 (external crystal) */
  public static void setClockExternal2() { }

  /** Insert a CPU level NOP */
  public static void nop() { }

  /** Insert inline assembly */
  public static void asm(String code) { }

  /** Get cycle counter from CPU (if available) { } */
  public static int getCycleCount() { return 0; }
}

