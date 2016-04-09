/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 * CPC support by SRS  - https://github.com/deringenieur71
 *
 */

package net.mikekohn.java_grinder;

abstract public class CPC
{

  protected CPC() { }

  public static void beep() { }
  public static void setTxtPen(int color) { }
  public static void setTxtPaper(int color) { }
  public static void setBorderColor(int color) { }
  public static void setGraPen(int color) { }
  public static void setGraPaper(int color) { }
  public static void cls() { }
  public static void screen(int mode) { }
  public static void putChar(char c) { }
  public static char readChar() { return 0; }
  public static void setCursor(int column,int line) { }
  public static void plot(int x, int y, int foreground) { }
  public static void draw(int x, int y, int foreground) { }
  
}


