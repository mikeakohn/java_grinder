/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2014 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder.c64;

public class VIC
{
  private VIC()
  {
  }

  // 0xd000
  // 0xd001
  public static void sprite0pos(int x, int y) { }

  // 0xd002
  // 0xd003
  public static void sprite1pos(int x, int y) { }

  // 0xd004
  // 0xd005
  public static void sprite2pos(int x, int y) { }

  // 0xd006
  // 0xd007
  public static void sprite3pos(int x, int y) { }

  // 0xd008
  // 0xd009
  public static void sprite4pos(int x, int y) { }

  // 0xd00a
  // 0xd00b
  public static void sprite5pos(int x, int y) { }

  // 0xd00c
  // 0xd00d
  public static void sprite6pos(int x, int y) { }

  // 0xd00e
  // 0xd00f
  public static void sprite7pos(int x, int y) { }

  // 0xd010

  // 0xd011
  public static void writeControl1(int value) { }
  public static int readControl1() { return 0; }

  // 0xd012
  // 0xd013
  // 0xd014

  // 0xd015
  public static void spriteEnable(int value) { }

  // 0xd016
  public static void writeControl2(int value) { }
  public static int readControl2() { return 0; }

  // 0xd017
  public static void spriteExpandY(int value) { }

  // 0xd018
  public static void writePointer(int value) { }
  public static int readPointer() { return 0; }

  // 0xd019
  public static void writeInterruptStatus(int value) { }
  public static int readInterruptStatus() { return 0; }

  // 0xd01a
  public static void interruptControl(int value) { }

  // 0xd01b
  public static void spritePriority(int value) { }

  // 0xd01c
  public static void spriteMulticolorEnable(int value) { }

  // 0xd01d
  public static void spriteExpandX(int value) { }

  // 0xd01e
  public static int spriteCollision() { return 0; }

  // 0xd01f
  public static int dataCollision() { return 0; }

  // 0xd020
  public static void border(int value) { }

  // 0xd021
  public static void background(int value) { }

  // 0xd022
  public static void multicolor1(int value) { }

  // 0xd023
  public static void multicolor2(int value) { }

  // 0xd024
  public static void multicolor3(int value) { }

  // 0xd025
  public static void spriteMulticolor0(int value) { }

  // 0xd026
  public static void spriteMulticolor1(int value) { }

  // 0xd027
  public static void sprite0color(int value) { }

  // 0xd028
  public static void sprite1color(int value) { }

  // 0xd029
  public static void sprite2color(int value) { }

  // 0xd02a
  public static void sprite3color(int value) { }

  // 0xd02b
  public static void sprite4color(int value) { }

  // 0xd02c
  public static void sprite5color(int value) { }

  // 0xd02d
  public static void sprite6color(int value) { }

  // 0xd02e
  public static void sprite7color(int value) { }

  public static void hiresEnable(int mode) { }
  public static void hiresClear(int value) { }
  public static void hiresPlot(int x, int y, int value) { }
  public static void makeHiresTables() { }
  public static void textEnable(int screen) { }
  public static void textClear(int value) { }
  public static void textCopy() { }
  public static void textPlot(int x, int y, int value, int color) { }
  public static void textAsciiPlot(int x, int y, int value, int color) { }
  public static int textRead(int x, int y) { return 0; }
  public static void textString(int x, int y, String str, int color) { }
  public static void textFill(int x, int y, int w, int h, int value) { }
  public static void textPaint(int x, int y, int w, int h, int value) { }
  public static void textScrollLeft() { }
  public static void textScrollRight() { }
  public static void textScrollUp() { }
  public static void textScrollDown() { }
  public static void makeTextTable() { }
  public static void makeColorTable() { }
  public static void colorRamClear(int value) { }
  public static void copyUppercase() { }
  public static void copyLowercase() { }
  public static void copyDataFromArray(byte[] array, int address, int bytes) { }
}

