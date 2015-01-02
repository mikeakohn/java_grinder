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

abstract public class IOPort
{
  protected IOPort()
  {
  }

  public static void setPinsAsInput(int mask) { }
  public static void setPinsAsOutput(int mask) { }
  public static void setPinsValue(int value) { }
  public static void setPinsHigh(int mask) { }
  public static void setPinsLow(int mask) { }
  public static void setPinAsOutput(int pin) { }
  public static void setPinAsInput(int pin) { }
  public static void setPinHigh(int pin) { }
  public static void setPinLow(int pin) { }
  public static boolean isPinInputHigh(int pin) { return false; }
  public static int getPortInputValue() { return 0; }
  //public static void setPortOutputValue(int value) { }
  public static void setPinsResistorEnable(int pin) { }
}

