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
}

