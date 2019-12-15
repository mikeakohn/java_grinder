/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPLv3
 *
 * Copyright 2014-2019 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder.amiga;

public class Copper
{
  public static final int MOVE_BLTDDAT = 0x0000000;
  public static final int MOVE_DMACONR = 0x0020000;
  public static final int MOVE_VPOSR = 0x0040000;
  public static final int MOVE_VHPOSR = 0x0060000;
  public static final int MOVE_DSKDATR = 0x0080000;
  public static final int MOVE_JOY0DAT = 0x00a0000;
  public static final int MOVE_JOY1DAT = 0x00c0000;
  public static final int MOVE_CLXDAT = 0x00e0000;
  public static final int MOVE_ADKCONR = 0x0100000;
  public static final int MOVE_POT0DAT = 0x0120000;
  public static final int MOVE_POT1DAT = 0x0140000;
  public static final int MOVE_POTINP = 0x0160000;
  public static final int MOVE_SERDATR = 0x0180000;
  public static final int MOVE_DSKBYTR = 0x01a0000;
  public static final int MOVE_INTENAR = 0x01c0000;
  public static final int MOVE_INTREQR = 0x01e0000;
  public static final int MOVE_DSKPTH = 0x0200000;
  public static final int MOVE_DSKPTL = 0x0220000;
  public static final int MOVE_DSKLEN = 0x0240000;
  public static final int MOVE_DSKDAT = 0x0260000;
  public static final int MOVE_REFPTR = 0x0280000;
  public static final int MOVE_VPOSW = 0x02a0000;
  public static final int MOVE_VHPOSW = 0x02c0000;
  public static final int MOVE_COPCON = 0x02e0000;
  public static final int MOVE_SERDAT = 0x0300000;
  public static final int MOVE_SERPER = 0x0320000;
  public static final int MOVE_POTGO = 0x0340000;
  public static final int MOVE_JOYTEST = 0x0360000;
  public static final int MOVE_STREQU = 0x0380000;
  public static final int MOVE_STRVBL = 0x03a0000;
  public static final int MOVE_STRHOR = 0x03c0000;
  public static final int MOVE_STRLONG = 0x03e0000;
  public static final int MOVE_BLTCON0 = 0x0400000;
  public static final int MOVE_BLTCON1 = 0x0420000;
  public static final int MOVE_BLTAFWM = 0x0440000;
  public static final int MOVE_BLTALWM = 0x0460000;
  public static final int MOVE_BLTCPTH = 0x0480000;
  public static final int MOVE_BLTCPTL = 0x04a0000;
  public static final int MOVE_BLTBPTH = 0x04c0000;
  public static final int MOVE_BLTBPTL = 0x04e0000;
  public static final int MOVE_BLTAPTH = 0x0500000;
  public static final int MOVE_BLTAPTL = 0x0520000;
  public static final int MOVE_BLTDPTH = 0x0540000;
  public static final int MOVE_BLTDPTL = 0x0560000;
  public static final int MOVE_BLTSIZE = 0x0580000;
  public static final int MOVE_BLTCON0L = 0x05a0000;
  public static final int MOVE_BLTSIZV = 0x05c0000;
  public static final int MOVE_BLTSIZH = 0x05e0000;
  public static final int MOVE_BLTCMOD = 0x0600000;
  public static final int MOVE_BLTBMOD = 0x0620000;
  public static final int MOVE_BLTAMOD = 0x0640000;
  public static final int MOVE_BLTDMOD = 0x0660000;
  public static final int MOVE_BLTCDAT = 0x0700000;
  public static final int MOVE_BLTBDAT = 0x0720000;
  public static final int MOVE_BLTADAT = 0x0740000;
  public static final int MOVE_SPRHDAT = 0x0780000;
  public static final int MOVE_BPLHDAT = 0x07a0000;
  public static final int MOVE_DENISEID = 0x07c0000;
  public static final int MOVE_DSKSYNC = 0x07e0000;
  public static final int MOVE_COP1LCH = 0x0800000;
  public static final int MOVE_COP1LCL = 0x0820000;
  public static final int MOVE_COP2LCH = 0x0840000;
  public static final int MOVE_COP2LCL = 0x0860000;
  public static final int MOVE_COPJMP1 = 0x0880000;
  public static final int MOVE_COPJMP2 = 0x08a0000;
  public static final int MOVE_COPINS = 0x08c0000;
  public static final int MOVE_DIWSTRT = 0x08e0000;
  public static final int MOVE_DIWSTOP = 0x0900000;
  public static final int MOVE_DDFSTRT = 0x0920000;
  public static final int MOVE_DDFSTOP = 0x0940000;
  public static final int MOVE_DMACON = 0x0960000;
  public static final int MOVE_CLXCON = 0x0980000;
  public static final int MOVE_INTENA = 0x09a0000;
  public static final int MOVE_INTREQ = 0x09c0000;
  public static final int MOVE_ADKCON = 0x09e0000;
  public static final int MOVE_AUD0LCH = 0x0a00000;
  public static final int MOVE_AUD0LCL = 0x0a20000;
  public static final int MOVE_AUD0LEN = 0x0a40000;
  public static final int MOVE_AUD0PER = 0x0a60000;
  public static final int MOVE_AUD0VOL = 0x0a80000;
  public static final int MOVE_AUD0DAT = 0x0aa0000;
  public static final int MOVE_AUD1LCH = 0x0b00000;
  public static final int MOVE_AUD1LCL = 0x0b20000;
  public static final int MOVE_AUD1LEN = 0x0b40000;
  public static final int MOVE_AUD1PER = 0x0b60000;
  public static final int MOVE_AUD1VOL = 0x0b80000;
  public static final int MOVE_AUD1DAT = 0x0ba0000;
  public static final int MOVE_AUD2LCH = 0x0c00000;
  public static final int MOVE_AUD2LCL = 0x0c20000;
  public static final int MOVE_AUD2LEN = 0x0c40000;
  public static final int MOVE_AUD2PER = 0x0c60000;
  public static final int MOVE_AUD2VOL = 0x0c80000;
  public static final int MOVE_AUD2DAT = 0x0ca0000;
  public static final int MOVE_AUD3LCH = 0x0d00000;
  public static final int MOVE_AUD3LCL = 0x0d20000;
  public static final int MOVE_AUD3LEN = 0x0d40000;
  public static final int MOVE_AUD3PER = 0x0d60000;
  public static final int MOVE_AUD3VOL = 0x0d80000;
  public static final int MOVE_AUD3DAT = 0x0da0000;
  public static final int MOVE_BPL1PTH = 0x0e00000;
  public static final int MOVE_BPL1PTL = 0x0e20000;
  public static final int MOVE_BPL2PTH = 0x0e40000;
  public static final int MOVE_BPL2PTL = 0x0e60000;
  public static final int MOVE_BPL3PTH = 0x0e80000;
  public static final int MOVE_BPL3PTL = 0x0ea0000;
  public static final int MOVE_BPL4PTH = 0x0ec0000;
  public static final int MOVE_BPL4PTL = 0x0ee0000;
  public static final int MOVE_BPL5PTH = 0x0f00000;
  public static final int MOVE_BPL5PTL = 0x0f20000;
  public static final int MOVE_BPL6PTH = 0x0f40000;
  public static final int MOVE_BPL6PTL = 0x0f60000;
  public static final int MOVE_BPL7PTH = 0x0f80000;
  public static final int MOVE_BPL7PTL = 0x0fa0000;
  public static final int MOVE_BPL8PTH = 0x0fc0000;
  public static final int MOVE_BPL8PTL = 0x0fe0000;
  public static final int MOVE_BPLCON0 = 0x1000000;
  public static final int MOVE_BPLCON1 = 0x1020000;
  public static final int MOVE_BPLCON2 = 0x1040000;
  public static final int MOVE_BPLCON3 = 0x1060000;
  public static final int MOVE_BPL1MOD = 0x1080000;
  public static final int MOVE_BPL2MOD = 0x10a0000;
  public static final int MOVE_BPLCON4 = 0x10c0000;
  public static final int MOVE_CLXCON2 = 0x10e0000;
  public static final int MOVE_BPL1DAT = 0x1100000;
  public static final int MOVE_BPL2DAT = 0x1120000;
  public static final int MOVE_BPL3DAT = 0x1140000;
  public static final int MOVE_BPL4DAT = 0x1160000;
  public static final int MOVE_BPL5DAT = 0x1180000;
  public static final int MOVE_BPL6DAT = 0x11a0000;
  public static final int MOVE_BPL7DAT = 0x11c0000;
  public static final int MOVE_BPL8DAT = 0x11e0000;
  public static final int MOVE_SPR0PTH = 0x1200000;
  public static final int MOVE_SPR0PTL = 0x1220000;
  public static final int MOVE_SPR1PTH = 0x1240000;
  public static final int MOVE_SPR1PTL = 0x1260000;
  public static final int MOVE_SPR2PTH = 0x1280000;
  public static final int MOVE_SPR2PTL = 0x12a0000;
  public static final int MOVE_SPR3PTH = 0x12c0000;
  public static final int MOVE_SPR3PTL = 0x12e0000;
  public static final int MOVE_SPR4PTH = 0x1300000;
  public static final int MOVE_SPR4PTL = 0x1320000;
  public static final int MOVE_SPR5PTH = 0x1340000;
  public static final int MOVE_SPR5PTL = 0x1360000;
  public static final int MOVE_SPR6PTH = 0x1380000;
  public static final int MOVE_SPR6PTL = 0x13a0000;
  public static final int MOVE_SPR7PTH = 0x13c0000;
  public static final int MOVE_SPR7PTL = 0x13e0000;
  public static final int MOVE_SPR0POS = 0x1400000;
  public static final int MOVE_SPR0CTL = 0x1420000;
  public static final int MOVE_SPR0DATA = 0x1440000;
  public static final int MOVE_SPR0DATB = 0x1460000;
  public static final int MOVE_SPR1POS = 0x1480000;
  public static final int MOVE_SPR1CTL = 0x14a0000;
  public static final int MOVE_SPR1DATA = 0x14c0000;
  public static final int MOVE_SPR1DATB = 0x14e0000;
  public static final int MOVE_SPR2POS = 0x1500000;
  public static final int MOVE_SPR2CTL = 0x1520000;
  public static final int MOVE_SPR2DATA = 0x1540000;
  public static final int MOVE_SPR2DATB = 0x1560000;
  public static final int MOVE_SPR3POS = 0x1580000;
  public static final int MOVE_SPR3CTL = 0x15a0000;
  public static final int MOVE_SPR3DATA = 0x15c0000;
  public static final int MOVE_SPR3DATB = 0x15e0000;
  public static final int MOVE_SPR4POS = 0x1600000;
  public static final int MOVE_SPR4CTL = 0x1620000;
  public static final int MOVE_SPR4DATA = 0x1640000;
  public static final int MOVE_SPR4DATB = 0x1660000;
  public static final int MOVE_SPR5POS = 0x1680000;
  public static final int MOVE_SPR5CTL = 0x16a0000;
  public static final int MOVE_SPR5DATA = 0x16c0000;
  public static final int MOVE_SPR5DATB = 0x16e0000;
  public static final int MOVE_SPR6POS = 0x1700000;
  public static final int MOVE_SPR6CTL = 0x1720000;
  public static final int MOVE_SPR6DATA = 0x1740000;
  public static final int MOVE_SPR6DATB = 0x1760000;
  public static final int MOVE_SPR7POS = 0x1780000;
  public static final int MOVE_SPR7CTL = 0x17a0000;
  public static final int MOVE_SPR7DATA = 0x17c0000;
  public static final int MOVE_SPR7DATB = 0x17e0000;
  public static final int MOVE_COLOR00 = 0x1800000;
  public static final int MOVE_COLOR01 = 0x1820000;
  public static final int MOVE_COLOR02 = 0x1840000;
  public static final int MOVE_COLOR03 = 0x1860000;
  public static final int MOVE_COLOR04 = 0x1880000;
  public static final int MOVE_COLOR05 = 0x18a0000;
  public static final int MOVE_COLOR06 = 0x18c0000;
  public static final int MOVE_COLOR07 = 0x18e0000;
  public static final int MOVE_COLOR08 = 0x1900000;
  public static final int MOVE_COLOR09 = 0x1920000;
  public static final int MOVE_COLOR10 = 0x1940000;
  public static final int MOVE_COLOR11 = 0x1960000;
  public static final int MOVE_COLOR12 = 0x1980000;
  public static final int MOVE_COLOR13 = 0x19a0000;
  public static final int MOVE_COLOR14 = 0x19c0000;
  public static final int MOVE_COLOR15 = 0x19e0000;
  public static final int MOVE_COLOR16 = 0x1a00000;
  public static final int MOVE_COLOR17 = 0x1a20000;
  public static final int MOVE_COLOR18 = 0x1a40000;
  public static final int MOVE_COLOR19 = 0x1a60000;
  public static final int MOVE_COLOR20 = 0x1a80000;
  public static final int MOVE_COLOR21 = 0x1aa0000;
  public static final int MOVE_COLOR22 = 0x1ac0000;
  public static final int MOVE_COLOR23 = 0x1ae0000;
  public static final int MOVE_COLOR24 = 0x1b00000;
  public static final int MOVE_COLOR25 = 0x1b20000;
  public static final int MOVE_COLOR26 = 0x1b40000;
  public static final int MOVE_COLOR27 = 0x1b60000;
  public static final int MOVE_COLOR28 = 0x1b80000;
  public static final int MOVE_COLOR29 = 0x1ba0000;
  public static final int MOVE_COLOR30 = 0x1bc0000;
  public static final int MOVE_COLOR31 = 0x1be0000;
  public static final int MOVE_HTOTAL = 0x1c00000;
  public static final int MOVE_HSSTOP = 0x1c20000;
  public static final int MOVE_HBSTRT = 0x1c40000;
  public static final int MOVE_HBSTOP = 0x1c60000;
  public static final int MOVE_VTOTAL = 0x1c80000;
  public static final int MOVE_VSSTOP = 0x1ca0000;
  public static final int MOVE_VBSTRT = 0x1cc0000;
  public static final int MOVE_VBSTOP = 0x1ce0000;
  public static final int MOVE_SPRHSTRT = 0x1d00000;
  public static final int MOVE_SPRHSTOP = 0x1d20000;
  public static final int MOVE_BPLHSTRT = 0x1d40000;
  public static final int MOVE_BPLHSTOP = 0x1d60000;
  public static final int MOVE_HHPOSW = 0x1d80000;
  public static final int MOVE_HHPOSR = 0x1da0000;
  public static final int MOVE_BEAMCON0 = 0x1dc0000;
  public static final int MOVE_HSSTRT = 0x1de0000;
  public static final int MOVE_VSSTRT = 0x1e00000;
  public static final int MOVE_HCENTER = 0x1e20000;
  public static final int MOVE_DIWHIGH = 0x1e40000;
  public static final int MOVE_BPLHMOD = 0x1e60000;
  public static final int MOVE_SPRHPTH = 0x1e80000;
  public static final int MOVE_SPRHPTL = 0x1ea0000;
  public static final int MOVE_BPLHPTH = 0x1ec0000;
  public static final int MOVE_BPLHPTL = 0x1ee0000;
  public static final int MOVE_FMODE = 0x1fc0000;

  public Copper(int max_instructions)
  {
  }

  public void appendInstruction(int instruction) { }

  /** Wait till video beam hits x,y coordinates. */
  public void appendWait(int x, int y) { }

  /** Skip next instruction if video beam already hit x,y coordinates. */
  public void appendSkip(int x, int y) { }

  /** Add instruction to set the color of a palette . */
  public void appendSetColor(int palette, int color) { }

  /** Add instruction to set the bitplane to a byte array. */
  public void appendSetBitplane(int bitplane, byte[] data) { }

  /** Add instruction to set the bitplane to an address. */
  public void appendSetBitplane(int bitplane, int address) { }

  /** Add instruction to set 1 of 8 sprites to a char array
      (unsigned 16 bit words). */
  public void appendSetSprite(int index, char[] data) { }

  /** Add instruction stop the copper until the next vertical blank. */
  public void appendEnd() { }

  /** Reset internal instruction index so the copper instruction list
      can be reloaded. */
  public void resetIndex() { }

  /** Reset internal instruction list index. */
  public void setIndex(int index) { }

  /** Run this Copper program. */
  public void run() { }

  /** Stop the Copper. */
  public void stop() { }

  /** Get the Copper list as an array of int. */
  public int[] getArrayAsInt() { return null; }

  /** Get the Copper list as an array of char (16 bit unsigned). */
  public char[] getArrayAsChar() { return null; }

  private int index;
}

