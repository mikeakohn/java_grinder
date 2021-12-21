/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2021 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder;

abstract public class Intellivision
{
  protected Intellivision()
  {
  }

  public static final int COLOR_BLACK = 0;
  public static final int COLOR_BLUE = 1;
  public static final int COLOR_RED = 2;
  public static final int COLOR_TAN = 3;
  public static final int COLOR_DARK_GREEN = 4;
  public static final int COLOR_GREEN = 5;
  public static final int COLOR_YELLOW = 6;
  public static final int COLOR_WHITE = 7;
  public static final int COLOR_GRAY = 8;
  public static final int COLOR_CYAN = 9;
  public static final int COLOR_ORANGE = 10;
  public static final int COLOR_BROWN = 11;
  public static final int COLOR_PINK = 12;
  public static final int COLOR_LIGHT_BLUE = 13;
  public static final int COLOR_YELLOW_GREEN = 14;
  public static final int COLOR_PURPLE = 15;

  public static final int VIDEO_MODE_COLOR_STACK = 0;
  public static final int VIDEO_MODE_FOREGROUND_BACKGROUND = 1;
  public static final int COLOR_STACK_ADVANCE = 0x2000;

  // Video functions.

  // Data is:
  // Bits [ 12, 13, 10, 9] : Background color
  // Bits [ 11 ]           : GROM vs GRAM 
  // Bits [ 8 - 3 ]        : Card #
  // Bits [ 2 - 0 ]        : Foreground Color
  public static void plot(int data, int pos) { }
  public static void plot(int data, int x, int y) { }
  public static int readDisplay(int x, int y) { return 0; }
  public static void waitForVerticalBlank() { }

  /** Scroll the entire video display horizontally by (value 0 to 7) pixels. */
  public static void setHorizontalDelay(int value) { }

  /** Scroll the entire video display vertically by (value 0 to 7) pixels. */
  public static void setVerticalDelay(int value) { }

  /** Select either foreground / background mode or color stack mode. */
  public static void setVideoMode(int value) { }

  /** Sets the color of the color stack (index from 0 to 3). */
  public static void setColorStack(int index, int color) { }

  // Controller input.
  // http://wiki.intellivision.us/index.php?title=Hand_Controller
  public static int getControllerLeft() { return 0; }
  public static int getControllerRight() { return 0; }

  public static final int SOUND_CHANNEL_A = 0;
  public static final int SOUND_CHANNEL_B = 1;
  public static final int SOUND_CHANNEL_C = 2;

  // Sound functions.

  /** Sound period: Valid values between 0 to 4095. */
  public static void setSoundChannelPeriod(int channel, int value) { }

  /** Sound volume: Valid values between 0 to 63. */
  public static void setSoundChannelVolume(int channel, int value) { }

  /** Sound envelope period: Valid values between 0 to 65535. */
  public static void setSoundEnvelopePeriod(int value) { }

  /** Sound envelope type: Valid values between 0 to 16. */
  public static void setSoundEnvelopeType(int value) { }

  /** Sound noise period: Valid values between 0 to 31. */
  public static void setSoundNoisePeriod(int value) { }

  /** Sound type: Valid values between 0 to 31 where
      bits 3-5 noise and 0-2 tone. */
  public static void setSoundType(int value) { }

  // Sprite Functions.
}

