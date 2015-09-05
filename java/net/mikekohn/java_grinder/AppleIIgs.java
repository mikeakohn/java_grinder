/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 * AppleIIgs by Joe Davisson
 *
 */

package net.mikekohn.java_grinder;

abstract public class AppleIIgs
{
  protected AppleIIgs() { }
  public static void plotChar(int address, char c) { }
  public static void printChar(char c) { }
  public static void hiresEnable() { }
  public static void hiresPlot(int address, int c) { }
}

