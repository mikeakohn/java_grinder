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

public class UART
{
  private UART()
  {
  }

  public static void init(int port, int baud_rate)
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

