/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2021 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder;

abstract public class Joystick
{
  protected Joystick()
  {
  }

  public static boolean isRight(int index) { return false; }
  public static boolean isLeft(int index) { return false; }
  public static boolean isDown(int index) { return false; }
  public static boolean isUp(int index) { return false; }
  public static boolean isButtonDown_0(int index) { return false; }
}

