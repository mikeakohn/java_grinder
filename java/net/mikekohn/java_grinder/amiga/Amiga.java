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

  /** Set video mode.  Bitplanes can be 0 to 6.  For a width of 640, set
      hi_res to true.  For ham_mode (hold and modify) bitplanes must be 6.
      If bitplanes == 6 and ham_mode is false, then system is in extra half
      bright mode (in other words, bitplanes 0 to 4 select a color palette
      for this pixel and bitplane 5 says if the pixel's brightness should
      be 1/2).  If double_playfield is true then playfield 1 are the odd
      bitplanes and playfield 2 are the even. */
  public static void setVideoMode(int bitplanes, boolean hi_res, boolean ham_mode, boolean double_playfield) { }

  /** Set the scroll value for each playfield. The horizontal scroll value
      for each playfield is a number from 0 to 15. */
  public static void setPlayfieldScroll(int playfield_1, int playfield_2) { } 

  /** Set playfield priority.  The priorities are from 0 to 15 and are
      in respect to sprites.  The pf2_over_p1 is for double playfield
      mode saying which playfield has priority over the other. */
  public static void setPlayfieldPriority(int playfield_1, int playfield_2, boolean pf2_over_pf1) { } 
}

