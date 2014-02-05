/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2014 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder;

abstract public class DSP
{
  protected DSP()
  {
  }

  public static void clearA() { }
  public static void clearB() { }
  public static void addABAndStoreInA() { }
  public static void addABAndStoreInB() { }
  public static void addToA(int n) { }
  public static void addToB(int n) { }
  public static void squareAndAddToA(int n) { }
  public static void squareAndAddToB(int n) { }
  public static void mulAndAddToA(int a, int b) { }
  public static void mulAndAddToB(int a, int b) { }
  public static void mulAndSubFromA(int a, int b) { }
  public static void mulAndSubFromB(int a, int b) { }
}

