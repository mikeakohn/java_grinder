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

abstract public class Amiga
{
  public static final int VIDEO_MODE_HIRES = 0x8000;
  public static final int VIDEO_MODE_BITPLANE_COUNT_0 = 0x0000;
  public static final int VIDEO_MODE_BITPLANE_COUNT_1 = 0x1000;
  public static final int VIDEO_MODE_BITPLANE_COUNT_2 = 0x2000;
  public static final int VIDEO_MODE_BITPLANE_COUNT_3 = 0x3000;
  public static final int VIDEO_MODE_BITPLANE_COUNT_4 = 0x4000;
  public static final int VIDEO_MODE_BITPLANE_COUNT_5 = 0x5000;
  public static final int VIDEO_MODE_BITPLANE_COUNT_6 = 0x6000;
  public static final int VIDEO_MODE_HAM = 0x0800;
  public static final int VIDEO_MODE_DOUBLE_PLAYFIELD = 0x0400;
  public static final int VIDEO_MODE_COLOR = 0x0200;
  public static final int VIDEO_MODE_INTERLACE = 0x0004;

  public static final int DMA_AUDIO_0 = 0x01;
  public static final int DMA_AUDIO_1 = 0x02;
  public static final int DMA_AUDIO_2 = 0x04;
  public static final int DMA_AUDIO_3 = 0x08;
  public static final int DMA_DISK = 0x10;
  public static final int DMA_SPRITE = 0x20;
  public static final int DMA_BLITTER = 0x40;
  public static final int DMA_COPPER = 0x80;
  public static final int DMA_BITPLANE = 0x100;
  public static final int DMA_ENABLE = 0x200;

  public static final int AUDIO_MOD_VOLUME_CHANNEL_0_TO_1 = 0x01;
  public static final int AUDIO_MOD_VOLUME_CHANNEL_1_TO_2 = 0x02;
  public static final int AUDIO_MOD_VOLUME_CHANNEL_2_TO_3 = 0x04;
  public static final int AUDIO_MOD_VOLUME_CHANNEL_3_TO_NONE = 0x08;
  public static final int AUDIO_MOD_PERIOD_CHANNEL_0_TO_1 = 0x10;
  public static final int AUDIO_MOD_PERIOD_CHANNEL_1_TO_2 = 0x20;
  public static final int AUDIO_MOD_PERIOD_CHANNEL_2_TO_3 = 0x40;
  public static final int AUDIO_MOD_PERIOD_CHANNEL_3_TO_NONE = 0x80;

  /** Disable task switching in the OS. exec.library: Forbid(). */
  public static void disableMultitasking() { }

  /** Enable task switching in the OS. exec.library: Permit(). */
  public static void enableMultitasking() { }

  /** Disable interrupts in the OS. exec.library: Disable(). */
  public static void disableInterrupts() { }

  /** Disable interrupts in the OS. exec.library: Enable(). */
  public static void enableInterrupts() { }

  /** Set one of the color palettes. */
  public static void setPalette(int index, int color) { }

  /** Set one of the 8 sprite's data to specified array.  Probably better
      to do this in the Copper. */
  public static void setSpriteImage(int index, short[] data) { }

  /** Set one of the 8 sprite's position on the screen.
      hstart is 0 to 511.
      vstart is 0 to 511.
      vstop is 0 to 511.  */
  public static void setSpritePosition(int index, int hstart, int vstart, int vstop) { }

  /** Set video mode.  Bitplanes can be 0 to 6.  For a width of 640, set
      hi_res to true.  For ham_mode (hold and modify) bitplanes must be 6.
      If bitplanes == 6 and ham_mode is false, then system is in extra half
      bright mode (in other words, bitplanes 0 to 4 select a color palette
      for this pixel and bitplane 5 says if the pixel's brightness should
      be 1/2).  If double_playfield is true then playfield 1 are the odd
      bitplanes and playfield 2 are the even. */
  public static void setVideoMode(int value) { }

  /** Set the scroll value for each playfield. The horizontal scroll value
      for each playfield is a number from 0 to 15. */
  public static void setPlayfieldScroll(int playfield_1, int playfield_2) { } 

  /** Set playfield priority.  The priorities are from 0 to 15 and are
      in respect to sprites.  The pf2_over_p1 is for double playfield
      mode saying which playfield has priority over the other. */
  public static void setPlayfieldPriority(int playfield_1, int playfield_2, boolean pf2_over_pf1) { } 

  /** Modulo for even bitplanes.  This number is automatically added to the
      address at the end of each line. */
  public static void setBitplaneModuloEven(int value) { }

  /** Modulo for odd bitplanes.  This number is automatically added to the
      address at the end of each line. */
  public static void setBitplaneModuloOdd(int value) { }

  /** Upper left vertical-horizontal position of CRT. */
  public static void setDisplayWindowStart(int horizontal, int vertical) { }

  /** Lower right vertical-horizontal position of CRT. */
  public static void setDisplayWindowStop(int horizontal, int vertical) { }

  /** Display data fetch start.  Controls the horizontal timing of the
      beginning of the bitplane display DMA fetch. */
  public static void setDisplayBitplaneStart(int horizontal) { }

  /** Display data fetch end.  Controls the horizontal timing of the
      end of the bitplane display DMA fetch. */
  public static void setDisplayBitplaneStop(int horizontal) { }

  /** Set the sound data for audio channel 0 to 3. */
  public static void setAudioData(int channel, byte[] data) { }

  /** Set the rate of the length in unsigned words (samples) for audio
      channel 0 to 3. */
  public static void setAudioLength(int channel, int value) { }

  /** Set the rate of the audio output for channel 0 to 3.
      Minimum value is 124 color clocks. This is the number
      of color ticks between samples.  For example on an NTSC
      Amiga to play 8000 samples a second this is calculated
      as 3,579,545 / 8000 = 447. */
  public static void setAudioPeriod(int channel, int value) { }

  /** Set the volume of the audio output for channel 0 to 3. 
      Value can be 0 to 63. */
  public static void setAudioVolume(int channel, int value) { }

  /** Set audio modulation.  This is register ADKCON. */
  public static void setAudioModulation(int mask) { }

  /** Turn off audio modulation.  This is register ADKCON. */
  public static void clearAudioModulation(int mask) { }

  /** Turn on DMA based on mask while ignoring DMA not set by mask. */
  public static void setDMA(int mask) { }

  /** Turn off DMA based on mask while ignoring DMA not set by mask. */
  public static void clearDMA(int mask) { }

  /** Returns true if the video is in a vertical blank state. */
  public static boolean inVerticalBlank() { return false; }

  /** Plot a pixel on a 320x200 bitplane at the address passed in. */
  public static void plot(int address, int x, int y, int color, int depth) { }
}

