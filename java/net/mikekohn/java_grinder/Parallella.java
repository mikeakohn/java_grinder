/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPLv3
 *
 * Copyright 2014-2017 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder;

// This class goes along with the Epiphany chip and assists with communication
// between to the Epiphany and Parallella host.

public class Parallella
{
  private Parallella()
  {
  }

  /** Write data to shared memory.  Addresses start at 0 and extend 32k. */
  public static void writeSharedRamByte(int address, byte data) { }
  public static void writeSharedRamShort(int address, short data) { }
  public static void writeSharedRamInt(int address, int data) { }
  public static void writeSharedRamFloat(int address, float data) { }

  /** Read data from shared memory.  Addresses start at 0 and extend 32k. */
  public static byte readSharedRamByte(int address) { return 0; }
  public static short readSharedRamShort(int address) { return 0; }
  public static int readSharedRamInt(int address) { return 0; }
  public static float readSharedRamFloat(int address) { return 0; }

  /** Core information */
  public static int getCoreId() { return 0; }

  /** Enable user interrupts */
  public static void setUserInterruptListener(boolean enabled) { }
}

