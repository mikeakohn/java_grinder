/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPLv3
 *
 * Copyright 2014-2017 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder;

abstract public class Watchdog
{
  protected Watchdog() { }

  public static void enable() { }
  public static void disable() { }
  public static void kick() { }
}

