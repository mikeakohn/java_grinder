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

  /** Clear Accum A */
  public static void clearA() { }
  /** Clear Accum B */
  public static void clearB() { }
  /** Load Accum A */
  public static void loadA(int n) { }
  /** Load Accum B */
  public static void loadB(int n) { }
  /** Negate Accum A */
  public static void negA() { }
  /** Negate Accum B */
  public static void negB() { }
  /** Add Accum A with Accum B and store in Accum A */
  public static void addABAndStoreInA() { }
  /** Add Accum A with Accum B and store in Accum B */
  public static void addABAndStoreInB() { }
  /** Add to Accum A */
  public static void addToA(int n) { }
  /** Add to Accum B */
  public static void addToB(int n) { }
  /** Square and store result in Accum A */
  public static void squareToA(int n) { }
  /** Square and store result in Accum B */
  public static void squareToB(int n) { }
  /** Multiply and store result in Accum A */
  public static void mulToA(int n, int m) { }
  /** Multiply and store result in Accum B */
  public static void mulToB(int n, int m) { }
  /** Square and add result in Accum A */
  public static void squareAndAddToA(int n) { }
  /** Square and add result in Accum B */
  public static void squareAndAddToB(int n) { }
  /** Multiply and add result in Accum A */
  public static void mulAndAddToA(int n, int m) { }
  /** Multiply and add result in Accum B */
  public static void mulAndAddToB(int n, int m) { }
  /** Multiply and substract result from Accum A */
  public static void mulAndSubFromA(int n, int m) { }
  /** Multiply and substract result from Accum B */
  public static void mulAndSubFromB(int n, int m) { }
}

