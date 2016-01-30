/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder;

abstract public class Propeller
{
  protected Propeller() { }

  public static void setClock(int value) { }
  public static int getCogId() { return 0; }
  public static void stopCog(int value) { }
  public static int waitCount(int time, int delay) { return 0; }
  public static void waitPinsEqual(int value, int mask) { }
  public static void waitPinsNotEqual(int value, int mask) { }
}

