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

  /** Return value of A */
  public static int getA() { }
  /** Return value of B */
  public static int getB() { }
  /** Return value of A rounded */
  public static int getRoundedA() { }
  /** Return value of B rounded */
  public static int getRoundedB() { }
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
  /** Sub Accum A with Accum B (A-B) and store in Accum A */
  public static void subBAAndStoreInA() { }
  /** Sub Accum A with Accum B (B-A) and store in Accum B */
  public static void subABAndStoreInB() { }
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
  /** Euclidean distance to A */
  public static void euclideanDistanceToA(int n, int m) { }
  /** Euclidean distance to B */
  public static void euclideanDistanceToB(int n, int m) { }
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
  /** Euclidean distance added to A */
  public static void euclideanDistanceAndAddToA(int n, int m) { }
  /** Euclidean distance added to B */
  public static void euclideanDistanceAndAddToB(int n, int m) { }
  /** Arithmetic shift accumulator A (-16 to 16 bits only) */
  public static void shiftA(int n) { }
  /** Arithmetic shift accumulator B (-16 to 16 bits only) */
  public static void shiftB(int n) { }
}

