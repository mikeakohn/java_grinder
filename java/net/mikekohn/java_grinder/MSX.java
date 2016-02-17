/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 */

/*
 *   MSX support by Giovanni Nunes - https://github.com/plainspooky
 */

package net.mikekohn.java_grinder;

abstract public class MSX
{

  protected MSX() { }

  public static void beep() { }
  public static void color(int foreground,int background,int border) { }
  public static void cls() { }
  public static void screen(int mode) { }
  public static void keyOn() { }
  public static void keyOff() { }
  public static void fillVRAM(int c,int len,int addr) { }
  public static void copyVRAM(int len,int source,int dest) { }
  public static void putChar(char c) { }
}
