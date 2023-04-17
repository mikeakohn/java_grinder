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

package net.mikekohn.java_grinder.c64;

public class SID
{
  private SID()
  {
  }

  // 0xd400
  // 0xd401
  public static void frequency1(int value) { }

  // 0xd402
  // 0xd403
  public static void pulseWidth1(int value) { }

  // 0xd404
  public static void waveform1(int value) { }

  // 0xd405
  // 0xd406
  public static void adsr1(int value) { }

  // 0xd407
  // 0xd408
  public static void frequency2(int value) { }

  // 0xd409
  // 0xd40a
  public static void pulseWidth2(int value) { }

  // 0xd40b
  public static void waveform2(int value) { }

  // 0xd40c
  // 0xd40d
  public static void adsr2(int value) { }

  // 0xd40e
  // 0xd40f
  public static void frequency3(int value) { }

  // 0xd410
  // 0xd411
  public static void pulseWidth3(int value) { }

  // 0xd412
  public static void waveform3(int value) { }

  // 0xd413
  // 0xd414
  public static void adsr3(int value) { }

  // 0xd415
  // 0xd416
  public static void filterCutoff(int value) { }

  // 0xd417
  public static void filterResonance(int value) { }

  // 0xd418
  public static void volume(int value) { }

  // 0xd419
  // 0xd41a

  // 0xd41b
  public static void oscillator3(int value) { }

  // 0xd41c
  public static void envelope3(int value) { }

  public static void clear() { }
}

