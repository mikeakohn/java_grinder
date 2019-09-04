/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPLv3
 *
 * Copyright 2014-2019 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder.amiga;

public class Copper
{
  public Copper(int max_instructions)
  {
  }

  public void appendInstruction(int instruction) { }

  /** Wait till video beam hits x,y coordinates. */
  public void appendWait(int x, int y) { }

  /** Skip next instruction if video beam already hit x,y coordinates. */
  public void appendSkip(int x, int y) { }

  /** Add instruction to set the color of a palette . */
  public void appendSetColor(int palette, int color) { }

  /** Add instruction to set the bitplane to a byte array. */
  public void appendSetBitplane(int bitplane, byte[] data) { }

  /** Add instruction to set the bitplane to a byte array. */
  public void appendSetSprite(int index, byte[] data) { }

  /** Add instruction stop the copper until the next vertical blank. */
  public void appendEnd() { }

  /** Reset internal instruction index so the copper instruction list
      can be reloaded. */
  public void resetIndex() { }

  /** Reset internal instruction list index. */
  public void setIndex(int index) { }

  /** Run this Copper program. */
  public void run() { }

  /** Stop the Copper. */
  public void stop() { }

  /** Get the Copper list as an array of int. */
  public int[] getArrayAsInt() { return null; }

  /** Get the Copper list as an array of char. */
  public char[] getArrayAsChar() { return null; }

  private int index;
}

