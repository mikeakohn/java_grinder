/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder;

public interface VsyncListener
{
  /** Vsync interrupt routine. */
  public void onVsync();
}

