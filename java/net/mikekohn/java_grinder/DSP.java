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

abstract public class DSP
{
  static long A,B;

  protected DSP()
  {
  }

  /** Return value of A (only bits [31:16]) */
  public static int getA()
  {
    short n = (short)(A >> 16);
    return (int)n;
  }

  /** Return value of B (only bits [31:16]) */
  public static int getB()
  {
    short n = (short)(B >> 16);
    return (int)n;
  }

  /** Return value of A (only bits [38:23]) */
  public static int getUpperA()
  {
    short n = (short)(A >> 23);
    return (int)n;
  }

  /** Return value of B (only bits [38:23]) */
  public static int getUpperB()
  {
    short n = (short)(B >> 23);
    return (int)n;
  }

  /** Return value of A (only bits [23:8]) */
  public static int getLowerA()
  {
    short n = (short)(A >> 8);
    return (int)n;
  }

  /** Return value of B (only bits [23:8]) */
  public static int getLowerB()
  {
    short n = (short)(B >> 8);
    return (int)n;
  }

  /** Return value of A rounded */
  public static int getRoundedA()
  {
    return 0;
  }

  /** Return value of B rounded */
  public static int getRoundedB()
  {
    return 0;
  }

  /** Clear Accum A */
  public static void clearA() { A = 0; }

  /** Clear Accum B */
  public static void clearB() { B = 0; }

  /** Load Accum A */
  public static void loadA(int n) { A = n; }

  /** Load Accum B */
  public static void loadB(int n) { B = n; }

  /** Negate Accum A */
  public static void negA() { A = -A; }

  /** Negate Accum B */
  public static void negB() { B = -B; }

  /** Add Accum A with Accum B and store in Accum A */
  public static void addABAndStoreInA() { A = A + B; }

  /** Add Accum A with Accum B and store in Accum B */
  public static void addABAndStoreInB() { B = A + B; }

  /** Sub Accum A with Accum B (A-B) and store in Accum A */
  public static void subABAndStoreInA() { A = A - B; }

  /** Sub Accum A with Accum B (B-A) and store in Accum B */
  public static void subBAAndStoreInB() { B = B - A; }

  /** Add to Accum A */
  public static void addToA(int n) { A += n; }

  /** Add to Accum B */
  public static void addToB(int n) { B += n; }

  /** Square and store result in Accum A */
  public static void squareToA(int n) { A = n * n; }

  /** Square and store result in Accum B */
  public static void squareToB(int n) { B = n * n; }

  /** Multiply and store result in Accum A */
  public static void mulToA(int n, int m) { A = n * m; }

  /** Multiply and store result in Accum B */
  public static void mulToB(int n, int m) { B = n * m; }

  /** Euclidean distance to A */
  //public static void euclideanDistanceToA(int n, int m) { }

  /** Euclidean distance to B */
  //public static void euclideanDistanceToB(int n, int m) { }

  /** Square and add result in Accum A */
  public static void squareAndAddToA(int n) { A += n * n; }

  /** Square and add result in Accum B */
  public static void squareAndAddToB(int n) { B += n * n; }

  /** Multiply and add result in Accum A */
  public static void mulAndAddToA(int n, int m) { A += n * m; }

  /** Multiply and add result in Accum B */
  public static void mulAndAddToB(int n, int m) { B += n * m; }

  /** Multiply and substract result from Accum A */
  public static void mulAndSubFromA(int n, int m) { A -= n * m; }

  /** Multiply and substract result from Accum B */
  public static void mulAndSubFromB(int n, int m) { B -= n * m; }

  /** Euclidean distance added to A */
  //public static void euclideanDistanceAndAddToA(int n, int m) { }

  /** Euclidean distance added to B */
  //public static void euclideanDistanceAndAddToB(int n, int m) { }

  /** Arithmetic shift accumulator A (-16 to 16 bits only).
      Negative n means shift left, positive is shift right. */
  public static void shiftA(int n)
  {
    if (n > 0 && n <= 16) { A = A >> n; }
    else if (n < 0 && n >= -16) { A = A << (-n); }
  }

  /** Arithmetic shift accumulator B (-16 to 16 bits only) */
  public static void shiftB(int n)
  {
    if (n > 0 && n <= 16) { B = B >> n; }
    else if (n < 0 && n >= -16) { B = B << (-n); }
  }
}

