/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2023 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder;

abstract public class Nintendo64
{
  protected Nintendo64() { }

  public static final int AUDIO_STATUS_FULL = 1 << 31;
  public static final int AUDIO_STATUS_BUSY = 1 << 30;

  /** Wait for the video beam to go into blanking mode. */
  public static void waitVsync() { }

  /** Clear the screen that's currently not being displayed. This will
      also clear the Z Buffer. */
  public static void clearScreen() { }

  /** Reset the Z buffer. This can be called without clearing the screen. */
  public static void resetZBuffer() { }

  /** Sets the screen the currently displayed. The value can be either 0 or
      or 1. The screen that is not being displayed will be affected by
      clearScreen(), plot(), or any of the drawing methods. */
  public static void setScreen(int value) { }

  /** Set a pixel in the screen's buffer. Color is a value (in binary)
      RRRR_RGGG_GGBB_BBBA. So full red, no green or blue would be
      0xf800. */
  public static void plot(int index, int color) { }

  /** Set a pixel at location (x, y). Color is a value (in binary)
      RRRR_RGGG_GGBB_BBBA. So full red, no green or blue would be
      0xf800. */
  public static void plot(int x, int y, int color) { }

  /** Load texture into TMEM. */
  public static void loadTexture(short[] texture, int width, int height) { }

  /** Waits for the last poly to finish rendering before continuing. */
  public static void waitForPolygon() { }

  /** Get Audio Interface status register value. */
  public static int getAudioStatus() { return 0; }

  /** Set Audio Interface DAC rate. Value is vid_clock / (dperiod + 1). */
  public static void setAudioDACRate(int value) { }

  /** Set Audio Interface bitrate. Valid values are 0 to 15. */
  public static void setAudioBitRate(int value) { }

  /** Start Audio Interface playing sound. */
  public static void playAudio(short[] data, int offset, int length) { }
}

