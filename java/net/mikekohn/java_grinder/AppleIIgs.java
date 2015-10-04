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
  public static void hiresEnable(int bank) { }
  public static void hiresClear(int c) { }
  public static void hiresUpdate(int start, int end) { }
  public static void hiresPlot(int x, int y, int c) { }
  public static void hiresLine(int x1, int y1, int x2, int y2, int c) { }
  public static void hiresSpan(int x, int y, int length, int c) { }
  public static int hiresRead(int x, int y) { return 0; }
  public static void hiresSprite(byte[] data, int x, int y, int width, int length) { }
  public static void hiresPalette(int palette, int[] data) { }
  public static void hiresSetRow(int row, int palette) { }
  public static int rnd() { return 0; }

  /** Sound wave table. */
  public static void loadWaveTable(byte[] data, int address) { }

  /** Turn on oscillators (up to 32). */
  public static void enableOscillators(int count) { }

  /** Set master volume (0 to 15). */
  public static void setMasterVolume(int level) { }

  /** Set volume of individual voice (0 to 255). */
  public static void setSoundVolume(int voice, int level) { }

  /** Set frequency of individual voice (0 to 65535). */
  public static void setSoundFrequency(int voice, int frequency) { }

  /** Set volume of individual voice (0 to 255). */
  public static void setWaveTable(int voice, int address, int size) { }

}

