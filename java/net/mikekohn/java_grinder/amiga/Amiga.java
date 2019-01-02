/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2014-2019 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder.amiga;

abstract public class Amiga
{
  /** Disable task switching in the OS. exec.library: Forbid(). */
  public static void disableMultitasking() { }

  /** Enable task switching in the OS. exec.library: Permit(). */
  public static void enableMultitasking() { }

  /** Disable interrupts in the OS. exec.library: Disable(). */
  public static void disableInterrupts() { }

  /** Disable interrupts in the OS. exec.library: Enable(). */
  public static void enableInterrupts() { }

  /** Set one of the color palettes. */
  public static void setPalette(int index, int color) { }

  /** Set one of the 8 sprite's data to specified array. */
  public static void setSpriteImage(int index, int[] data) { }

  /** Set one of the 8 sprite's position on the screen.
      hstart is 0 to 511.
      vstart is 0 to 511.
      vstop is 0 to 511.  */
  public static void setSpritePosition(int index, int hstart, int vstart, int vstop) { }
}

