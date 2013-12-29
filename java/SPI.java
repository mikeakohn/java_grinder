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
  public SPI(int clock_divisor, int clock_phase, int clock_polarity)
  {
  }

  public void send(byte c)
  {
     System.out.println("Sent char " + c);
  }

  public byte read(byte c)
  {
    return 0;
  }

  public boolean isDataAvailable()
  {
    return false;
  }

  public boolean isSendReady()
  {
    return false;
  }
}

