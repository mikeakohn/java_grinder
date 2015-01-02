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

abstract public class UART
{
  // These constants assume specific baud rates.  This makes the
  // generated code cleaner and simpler, but kind of makes it limiting.
  // This can be fixed later.
  public static final int BAUD_1200 = 0;
  public static final int BAUD_2400 = 1;
  public static final int BAUD_9600 = 2;
  public static final int BAUD_19200 = 3;
  public static final int BAUD_38400 = 4;
  public static final int BAUD_57600 = 5;

  protected UART()
  {
  }

  public static void init(int baud_rate)
  {
  }

  /** send a byte to UART */ 
  public static void send(int c)
  {
     System.out.println("Sent char " + c);
  }

  /** read a byte from UART */ 
  public static int read()
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

