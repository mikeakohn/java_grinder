/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2022 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder;

abstract public class Nintendo64
{
  protected Nintendo64() { }

  public void waitVsync() { }
  public void clearScreen() { }
  public void setScreen(int value) { }

  /** Set a pixel at location (x, y). Color is a value (in binary)
      RRRR_RGGG_GGBB_BBBA. So full red, no green or blue would be
      0xf800. */
  public void plot(int x, int y, int color) { }
}

