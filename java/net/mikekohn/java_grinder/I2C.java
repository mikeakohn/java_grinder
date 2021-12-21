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

abstract public class I2C
{
  static public final int DIV1 = 0;
  static public final int DIV2 = 1;
  static public final int DIV4 = 2;
  static public final int DIV8 = 3;
  static public final int DIV16 = 4;
  static public final int DIV32 = 5;
  static public final int DIV64 = 6;
  static public final int DIV128 = 7;

  protected I2C()
  {
  }

  /**
   clock_divsor = DIV1 to DIV128
   */
  public static void init(int clock_divisor) { }

  /** Send a byte over I2C bus. */
  public static void write(int c) { }

  /** Sends a byte, waits until a byte is received, and returns it. */
  public static int read() { return 0; }

  /** Begin either read or write transmission. */
  public static void start() { }

  /** End either read or write transmission. */
  public static void stop() { }

  /** Turn off I2C so alternate peripherals can use pins. */
  public static void disable() { }

  /** Turn on I2C after disable() was called (not needed after init()). */
  public static void enable() { }
}

