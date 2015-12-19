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

abstract public class Atari2600
{
  protected Atari2600()
  {
  }

  public static void waitHsync() { }
  public static void startVblank() { }
  public static void waitVblank() { }
  public static void startOverscan() { }
  public static void waitOverscan() { }
  public static void setColorPlayer0(int color) { }
  public static void setColorPlayer1(int color) { }
  public static void setColorPlayfield(int color) { }
  public static void setColorBackground(int color) { }

  public static void resetPlayer0() { }
  public static void resetPlayer1() { }
  public static void resetMissile0() { }
  public static void resetMissile1() { }
  public static void resetBall() { }
  public static void applyHorizontalMotion() { }
  public static void clearMotionRegisters() { }
  public static void clearCollisionLatches() { }
}

