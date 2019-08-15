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

abstract public class Copper
{
  /** Wait till video beam hits x,y coordinates. */
  public static void setWait(int[] code, int index, int x, int y) { }

  /** Set one of the color palettes. */
  public static void setColor(int[] code, int index, int palette, int value) { }

  /** Set the bitplane to point to an array of memory. */
  public static void setBitplane(int[] code, int index, int bitplane, int value) { }

  /** Move immediate value to hardware register. */
  public static void setMove(int[] code, int index, int register, int value) { }

  /** Skip next instruction if video beam already passed x,y coordinates. */
  public static void setSkip(int[] code, int index, int x, int y) { }

  /** Waits till the video beam reaches the end of the screen. */
  public static void setWaitEnd(int[] code, int index) { }
}

