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

package net.mikekohn.java_grinder.draw3d;

public class Texture16 extends Texture
{
  public Texture16()
  {
  }

  public Texture16(int width, int height)
  {
  }

  /** Sets the color of a multiple pixels in a texture buffer at the index
      (x + (y * width)) provided. */
  public void setPixels(int index, short[] pixels) { }

  /** Sets the color of a multiple pixels in a texture buffer at the index
      (x + (y * width)) provided where the array is RLE compressed as
      [ count, color_lower, color_upper ] bytes. */
  public void setPixelsRLE16(int index, byte[] image) { }
}

