/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder.amiga;

abstract public class Amiga
{
  /** Sets one of the color palettes. */
  public static void setPalette(int index, int color) { }

  /** Disable task switching in the OS. exec.library: Forbid(). */
  public static void disableMultitasking() { }

  /** Enable task switching in the OS. exec.library: Permit(). */
  public static void enableMultitasking() { }

  /** Disable interrupts in the OS. exec.library: Disable(). */
  public static void disableInterrupts() { }

  /** Disable interrupts in the OS. exec.library: Enable(). */
  public static void enableInterrupts() { }
}

