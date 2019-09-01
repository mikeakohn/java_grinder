/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2014-2019 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder.amiga;

abstract public class Blitter
{
  //public static final int FILL_OPTIONS_LINE = 0x01;
  public static final int FILL_OPTIONS_DESC = 0x02;
  public static final int FILL_OPTIONS_FILL_CARRY_INPUT = 0x04;
  public static final int FILL_OPTIONS_INCLUSIVE_FILL = 0x08;
  public static final int FILL_OPTIONS_EXCLUSIVE_FILL = 0x10;

  //public static final int LINE_OPTIONS_LINE = 0x01;
  public static final int LINE_OPTIONS_SINGLE_BIT = 0x02;
  public static final int LINE_OPTIONS_ALWAYS_UP_LEFT = 0x04;
  public static final int LINE_OPTIONS_SOMETIMES_UP_LEFT = 0x08;
  public static final int LINE_OPTIONS_SOMETIMES_UP_DOWN = 0x10;
  public static final int LINE_OPTIONS_SIGN_FLAG = 0x40;

  public static final int LINE_FUNCTION_NANBNC = 0x01;
  public static final int LINE_FUNCTION_NANBC = 0x02;
  public static final int LINE_FUNCTION_NABNC = 0x04;
  public static final int LINE_FUNCTION_NABC = 0x08;
  public static final int LINE_FUNCTION_ANBNC = 0x10;
  public static final int LINE_FUNCTION_ANBC = 0x20;
  public static final int LINE_FUNCTION_ABNC = 0x40;
  public static final int LINE_FUNCTION_ABC = 0x80;

  public static final int ENABLE_CHANNEL_A = 0x08;
  public static final int ENABLE_CHANNEL_B = 0x04;
  public static final int ENABLE_CHANNEL_C = 0x02;
  public static final int ENABLE_CHANNEL_D = 0x01;

  public static final int LINE_TYPE_TEXTURE = 0x8000;
  public static final int LINE_TYPE_SOLID = 0xff;

  /** Set source A. */
  public void setSourceA(byte[] data) { }

  /** Set source B. */
  public void setSourceB(byte[] data) { }

  /** Set source C. */
  public void setSourceC(byte[] data) { }

  /** Set source D. */
  public void setSourceD(byte[] data) { }

  /** Set modulo A. */
  public void setModuloA(short value) { }

  /** Set modulo B. */
  public void setModuloB(short value) { }

  /** Set modulo C. */
  public void setModuloC(short value) { }

  /** Set modulo D. */
  public void setModuloD(short value) { }

  /** Set shift value A. */
  public void setShiftA(short value) { }

  /** Set shift value B. */
  public void setShiftB(short value) { }

  /** Blitter channel A masks. */
  public void setChannelAMasks(int first_word, int last_word) { }

  /** Enable channels (bit mask A=8, B=4, C=2, D=1). */
  public void enableChannels(int mask) { }

  /** Put the blitter in fill mode. */
  public void setAsFillMode(int options) { }

  /** Put the blitter in line mode. */
  public void setAsLineMode(int options) { }

  /** Set the logical function mode of the blittler. */
  public void setLogicalFunction(int mask) { }

  /** Line mode. */
  public void setDescMode(boolean enabled) { }

  /** No idea what this is (BLTSIZH, BLTSIZV). */
  public void setSize(int horizontal, int vertical) { }

  /** Set line type A (BLTDATA). */
  public void setLineTypeA(int line_type) { }

  /** Set line type B (BLTDATB). */
  public void setLineTypeB(int line_type) { }

  /** Set line type C (BLTDATC). */
  public void setLineTypeC(int line_type) { }

  /** Set line texture (BLTCON1 15-12). */
  public void setLineTexture(int value) { }

  /** Set line start (BLTCON0 15-12). */
  public void setLineStart(int value) { }

  /** Execute current blitter settings with passed in width in
      words and height in pixels. */
  public void runCopy(int width, int height) { }

  /** Execute the current blitter settings as a line draw. */
  public void drawLine(int length) { }

  private int modulo_a;
  private int modulo_b;
  private int modulo_c;
  private int modulo_d;
  private byte[] source_a;
  private byte[] source_b;
  private byte[] source_c;
  private byte[] source_d;
}

