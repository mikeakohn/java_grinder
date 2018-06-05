/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder;

public class Memory
{
  private Memory() { }

  /** Read a single byte from an address */
  public static byte read8(int address) { return 0; }

  /** Write a single byte to an address */
  public static void write8(int address, byte value) { }

  /** Read a 2 byte value from an address */
  public static short read16(int address) { return 0; }

  /** Write a 2 byte value to an address */
  public static void write16(int address, short value) { }

  /** Allocate a byte array on the stack.  Automatically free()'d when
      the current method ends. */
  public static byte[] allocStackBytes(int length) { return null; }

  /** Allocate a short array on the stack.  Automatically free()'d when
      the current method ends. */
  public static short[] allocStackShorts(int length) { return null; }

  /** Allocate a int array on the stack.  Automatically free()'d when
      the current method ends. */
  public static int[] allocStackInts(int length) { return null; }

  /** Load a file from disk (at assembler time) and return a reference
      to it as an array of ints. Not endian safe. */
  public static int[] preloadIntArray(String filename) { return null; }

  /** Load a file from disk (at assembler time) and return a reference
      to it as an array of ints. */
  public static byte[] preloadByteArray(String filename) { return null; }
}

