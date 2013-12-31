/**
 *  Java Grinder
 *  Author: The Naken Crew
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2013 by The Naken Crew
 *
 */

package cc.naken.java_grinder;

public class SPI
{
  private SPI()
  {
  }

  public static void init(int clock_divisor, int clock_phase, int clock_polarity)
  {
  }

  public static void send(byte c)
  {
     System.out.println("Sent char " + c);
  }

  public static byte read(byte c)
  {
    return 0;
  }

  public static boolean isDataAvailable()
  {
    return false;
  }

  public static boolean isSendReady()
  {
    return false;
  }
}

