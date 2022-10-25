/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2022 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder;

abstract public class SN76489
{
  protected SN76489() { }

  /** Set sound frequency for a sound channel (voice).  The voice
      param can be between 0 to 2 for normal notes and 3 for a noise
      voice.  Anything out of that range will cause unknown results.
      The freq param is based on the table from this page:
      http://www.nouspikel.com/ti99/titechpages.htm */
  public static void setSoundFreq(int voice, int freq) { }

  /** Set sound volume for a sound channel (voice).  The voice
      param can be between 0 to 2 for normal notes and 3 for a noise
      voice.  Anything out of that range will cause unknown results.
      The volume can be between 0 and 15.  0 = loudest, 15 = sound is off. */
  public static void setSoundVolume(int voice, int volume) { }

}

