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

abstract public class Keyboard
{
  protected Keyboard()
  {
  }

  public static boolean isKeyPressed(int index) { return false; }
  public static int currentKeyPressed() { return 0; }
}

