/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder;

abstract public class Playstation2
{
  protected Playstation2() { }

  public static final int EVENT_PROCESSOR_CYCLE = 1;
  public static final int EVENT_SINGLE_INSTRUCTION = 2;
  public static final int EVENT_BRANCH = 3;
  public static final int EVENT_BTAC_MISS = 4;
  public static final int EVENT_ITLB_MISS = 5;
  public static final int EVENT_INSTR_CACHE_MISS = 6;
  public static final int EVENT_ACCESS_TO_DTLB = 7;
  public static final int EVENT_NON_BLOCKING_LOAD = 8;
  public static final int EVENT_WBB_SINGLE_REQUEST = 9;
  public static final int EVENT_WBB_BURST_REQUEST = 10;
  public static final int EVENT_CPU_ADDR_BUS_BUSY = 11;
  public static final int EVENT_INSTR_COMPLETED = 12;
  public static final int EVENT_NONBDS_INSTR_COMPLETED = 13;
  public static final int EVENT_COP2_INSTR_COMPLETED = 14;
  public static final int EVENT_LOAD_COMPLETED = 15;
  public static final int EVENT_NO_EVENT = 16;

  /** Wait until the video beam is done drawing this frame (really
      half frame since the video is interlaced) and is ready to go
      back to the top of the screen to draw the next frame. */
  public static void waitVsync() { }

  /** Clear the screen on the current draw context. */
  public static void clearContext(int context) { }

  /** Sets which context will be displayed when the video is drawn. */
  public static void showContext(int context) { }

  /** Upload program in code[] to VU0.  The length of code[] must be a
      multiple of 16 bytes. */
  public static void vu0UploadCode(byte[] code) { }

  /** Upload data from data[] to VU0's data RAM starting at index.
      VU0 has 4k of RAM, so index is a number between 0 and 255.
      The length of the data must be a multiple of 16 bytes. */
  public static void vu0UploadData(int index, byte[] data) { }
  public static void vu0UploadData(int index, int[] data) { }
  public static void vu0UploadData(int index, float[] data) { }

  /** Download data into data[] from VU0's data RAM starting at index.
      VU0 has 4k of RAM, so index is a number between 0 and 255.
      The length of the data must be a multiple of 16 bytes. */
  public static void vu0DownloadData(int index, byte[] data) { }
  public static void vu0DownloadData(int index, int[] data) { }
  public static void vu0DownloadData(int index, float[] data) { }

  /** Start executing code at location 0 of VU0's code RAM. */
  public static void vu0Start() { }

  /** Stop VU0 from executing code. */
  public static void vu0Stop() { }

  /** Check to see if VU0's program is still running. */
  public static boolean vu0IsRunning() { return false; }

  /** Enable performance counter 0. */
  public static void performanceCountEnable(int event) { }

  /** Disable performance counter 0. */
  public static void performanceCountDisable() { }

  /** Get performance counter 0. */
  public static int performanceCountGet() { return 0; }

  /** Clear counter 0 (only valid when performance counter is disabled). */
  public static void performanceCountReset() { }

  /** Initializes random number generator.  Input is a 23 bit integer. */
  public static void randomInit(int r) { }

  /** Get a random number (a 23 bit integer). */
  public static int randomGet() { return 0; }

  /** Get next random number (a 23 bit integer). */
  public static int randomNext() { return 0; }

  /** Initialize system to be able to do sound. */
  public static void spuInit() { }

  /** Upload ADPCM sound data to SPU2 Core 0. */
  public static void spuUploadSoundData(byte[] data) { }

  /** Set voice volume on Core 0. Values between -16384 and 16383 */
  public static void spuSetVolume(int voice, int value) { }

  /** Set master volume on Core 0. Values between -16384 and 16383 */
  public static void spuSetMasterVolume(int value) { }

  /** Set pitch on Core 0. freq = (value / 4096) * f0 */
  public static void spuSetPitch(int voice, int value) { }

  /** KeyOn on Core 0. */
  public static void spuKeyOn(int voice) { }

  /** KeyOff on Core 0. */
  public static void spuKeyOff(int voice) { }
}

