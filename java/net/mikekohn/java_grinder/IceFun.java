/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2023 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder;

public class IceFun
{
  private IceFun()
  {
  }

  /** Set musical tone. Valid values are 60 to 96 (C4 to C7). Value
      of 0 will turn the speaker off. */
  public static void setTone(int value) { }

  /** Change value of servo peripheral. Valid values are 12000 to 24000
      where 12000 is full rotation in one direction and 24000 is full
      rotation in the reverse direction. */
  public static void setServo(int index, int value) { }

}

