/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
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

/*
  public static boolean isJoystick0Right() { return false; }
  public static boolean isJoystick0Left() { return false; }
  public static boolean isJoystick0Down() { return false; }
  public static boolean isJoystick0Up() { return false; }
  public static boolean isJoystick0ButtonDown() { return false; }

  public static boolean isJoystick1Right() { return false; }
  public static boolean isJoystick1Left() { return false; }
  public static boolean isJoystick1Down() { return false; }
  public static boolean isJoystick1Up() { return false; }
  public static boolean isJoystick1ButtonDown() { return false; }
*/
}

