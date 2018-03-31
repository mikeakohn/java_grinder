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

  // Used for setVideoMode()
  public static final int NON_INTERLACED = 0;
  public static final int INTERLACED = 1;
  public static final int VIDEO_TYPE_NTSC = 2;
  public static final int VIDEO_TYPE_PAL = 3;
  public static final int FRAME_FIELD = 0;
  public static final int FRAME_FRAME = 1;

  public static final int PSMCT32 = 0x00;
  public static final int PSMCT24 = 0x01;
  public static final int PSMCT16 = 0x02;
  public static final int PSMCT16S = 0x0a;
  public static final int PSGPU24 = 0x12;

  public static final int MAGNIFY_H_1 = 0;
  public static final int MAGNIFY_H_2 = 1;
  public static final int MAGNIFY_H_3 = 2;
  public static final int MAGNIFY_H_4 = 3;
  public static final int MAGNIFY_H_5 = 4;
  public static final int MAGNIFY_H_6 = 5;
  public static final int MAGNIFY_H_7 = 6;
  public static final int MAGNIFY_H_8 = 7;
  public static final int MAGNIFY_H_9 = 8;
  public static final int MAGNIFY_H_10 = 9;
  public static final int MAGNIFY_H_11 = 10;
  public static final int MAGNIFY_H_12 = 11;
  public static final int MAGNIFY_H_13 = 12;
  public static final int MAGNIFY_H_14 = 13;
  public static final int MAGNIFY_H_15 = 14;
  public static final int MAGNIFY_H_16 = 15;

  public static final int MAGNIFY_V_1 = 0;
  public static final int MAGNIFY_V_2 = 1;
  public static final int MAGNIFY_V_3 = 2;
  public static final int MAGNIFY_V_4 = 3;

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

  /** Set video display format (what video mode the CRT will use). */
  //public static void setVideoMode(int interlaced, int video_type, int frame) { }

  /** Set frame buffer 1's (DISPFB1) location in memory and shape. */
  //public static void setFrameBuffer1(int base_pointer, int frame_buffer_width, int pixel_storage_format, int position_x, int position_y) { }

  /** Set frame buffer 2's (DISPFB2) location in memory and shape. */
  //public static void setFrameBuffer2(int base_pointer, int frame_buffer_width, int pixel_storage_format, int position_x, int position_y) { }

  /** Set frame buffer 1's (DISPLAY1) offset and shape. */
  //public static void setDisplay1(int dx, int dy, int magnify_x, int magnify_y, int dw, int dh) { }

  /** Set frame buffer 2's (DISPLAY2) offset and shape. */
  //public static void setDisplay2(int dx, int dy, int magnify_x, int magnify_y, int dw, int dh) { }

  public static void clearScreen() { }
  public static void waitVsync() { }

  /** Upload program in code[] to VU0. */
  public static void vu0UploadCode(byte[] code) { }

  /** Upload data from data[] to VU0's data RAM starting at index.
      VU0 has 4k of RAM, so index is a number between 0 and 255. */
  public static void vu0UploadData(int index, byte[] data) { }

  /** Download data into data[] from VU0's data RAM starting at index.
      VU0 has 4k of RAM, so index is a number between 0 and 255. */
  public static void vu0DownloadData(int index, byte[] data) { }

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
}

