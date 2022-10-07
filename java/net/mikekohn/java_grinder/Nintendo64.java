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

  /** Wait for the video beam to go into blanking mode. */
  public static void waitVsync() { }

  /** Clear the screen that's currently not being displayed. This will
      also clear the Z Buffer. */
  public static void clearScreen() { }

  /** Reset the Z buffer. This can be called without clearing the screen. */
  public static void resetZBuffer() { }

  /** Sets the screen the currently displayed. The value can be either 0 or
      or 1. The screen that is not being displayed will be affected by
      clearScreen(), plot(), or any of the drawing methods. */
  public static void setScreen(int value) { }

  /** Set a pixel at location (x, y). Color is a value (in binary)
      RRRR_RGGG_GGBB_BBBA. So full red, no green or blue would be
      0xf800. */
  public static void plot(int x, int y, int color) { }

  /** Wait for the last rectangle / triangle to finish drawing. */
  public static void waitForPolygon() { }
}

