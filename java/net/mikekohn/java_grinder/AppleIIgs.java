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
  public static void hiresSpan(int address, int c, int length) { }
  public static int hiresRead(int address) { return 0; }
  public static void hiresBlit(byte[] data, int address, int width, int length) { }
  public static void hiresPalette(int palette, int[] data) { }
  public static void hiresSetLine(int line, int palette) { }

  /** Sound wave table. */
  public static void loadWaveTable(byte[] data) { }

  /** Turn on oscillators (up to 32). */
  public static void enableOscillators(int count) { }

  /** Set master volume (0 to 15). */
  public static void setMasterVolume(int level) { }

  /** Set volume of individual voice (0 to 255). */
  public static void setSoundVolume(int voice, int level) { }

  /** Set frequency of individual voice (0 to 65535). */
  public static void setSoundFrequency(int voice, int frequency) { }
}

