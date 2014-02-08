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

package net.mikekohn.java_grinder;

// http://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus

abstract public class SPI
{
  static public final int DIV1 = 0;
  static public final int DIV2 = 1;
  static public final int DIV4 = 2;
  static public final int DIV8 = 3;
  static public final int DIV16 = 4;
  static public final int DIV32 = 5;
  static public final int DIV64 = 6;
  static public final int DIV128 = 7;

  protected SPI()
  {
  }

  /**
   clock_divsor = DIV1 to DIV128
   mode = 0 to 3 where:
      mode 0: CPOL=0, CPHA=0
      mode 1: CPOL=0, CPHA=1
      mode 2: CPOL=1, CPHA=0
      mode 3: CPOL=1, CPHA=1
   */
  public static void init(int clock_divisor, int mode)
  {
  }

  /** send and receive a byte */
  public static int send(int c)
  {
    //System.out.println("Sent char " + c);
    return 0;
  }

  /** receive a byte */
  public static int read()
  {
    return 0;
  }

  /** Returns true if data is available on the SPI bus. */
  public static boolean isDataAvailable()
  {
    return true;
  }

  /** Returns true if data is being transfered on the SPI bus. */
  public static boolean isBusy()
  {
    return false;
  }

  /** Turn off SPI so alternate peripherals can use pins. */
  public static void disable()
  {
  }

  /** Turn on SPI after disable() was called (not needed after init()). */
  public static void enable()
  {
  }
}

