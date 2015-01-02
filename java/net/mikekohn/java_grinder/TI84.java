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

// http://wikiti.brandonw.net/index.php?title=84PCSE:OS:Applications

abstract public class TI84
{
  protected TI84()
  {
  }

  public static void clearScreen() { }
  public static void clearRect(int x0, int x1, int y0, int y1) { }
  public static void drawLine(int x0, int y0, int x1, int y1, int operation) { }
  public static void drawPoint(int x, int y, int operation) { }
  public static void printHL() { }
  public static void print(String text) { }
  public static void printCenter(String text) { }
  public static void putc(char c) { }
  public static void fillRect(int x0, int x1, int y0, int y1) { }
  public static void setCursorX(int x) { }
  public static void setCursorY(int y) { }
  public static void setDrawBGColor(int color) { }
  public static void setDrawBGWhite() { }
  public static void setDrawColor(int color) { }
  public static void setFillColor(int color) { }
  public static void setTextBGColor(int color) { }
  public static void setTextColor(int color) { }

  public static final int OPERATION_ERASE_PIXEL = 0;
  public static final int OPERATION_DRAW_PIXEL = 1;
  public static final int OPERATION_INVERT_PIXEL = 2;
  public static final int OPERATION_TEST_PIXEL = 3;
  public static final int OPERATION_RESTORE_PIXEL = 4;
}

