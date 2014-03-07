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

package net.mikekohn.java_grinder.C64;

public class SID
{
  private SID()
  {
  }

  // 0xd400
  // 0xd401
  public static void voice1_frequency(int value) { }

  // 0xd402
  // 0xd403
  public static void voice1_pulse_width(int value) { }

  // 0xd404
  public static void voice1_waveform(int value) { }

  // 0xd405
  // 0xd406
  public static void voice1_adsr(int value) { }

  // 0xd407
  // 0xd408
  public static void voice2_frequency(int value) { }

  // 0xd409
  // 0xd40a
  public static void voice2_pulse_width(int value) { }

  // 0xd40b
  public static void voice2_waveform(int value) { }

  // 0xd40c
  // 0xd40d
  public static void voice2_adsr(int value) { }

  // 0xd40e
  // 0xd40f
  public static void voice3_frequency(int value) { }

  // 0xd410
  // 0xd411
  public static void voice3_pulse_width(int value) { }

  // 0xd412
  public static void voice3_waveform(int value) { }

  // 0xd413
  // 0xd414
  public static void voice3_adsr(int value) { }

  // 0xd415
  // 0xd416
  public static void filter_cutoff(int value) { }

  // 0xd417
  public static void filter_resonance(int value) { }

  // 0xd418
  public static void volume(int value) { }

  // 0xd419
  // 0xd41a

  // 0xd41b
  public static void voice3_oscillator(int value) { }

  // 0xd41c
  public static void voice3_envelope(int value) { }

  public static void clear() { }
}

