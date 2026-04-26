/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2026 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder;

public class ADC
{
  protected ADC()
  {
  }

  /** Enable ADC converter. */
  public static void enable()
  {
  }

  /** Disable ADC converter. */
  public static void disable()
  {
  }

  /** Set channel (selects which pin to read from). */
  public static void setChannel(int channel)
  {
  }

  /** Read from ADC. */
  public static int read()
  {
    return 0;
  }

  /** Read from ADC for dsPIC33. The channel number for dsPIC is needed
      in this method there fore with this method setChannel() is not
      needed. Probably need to rethink this. */
  public static int read(int channel)
  {
    return 0;
  }
}

