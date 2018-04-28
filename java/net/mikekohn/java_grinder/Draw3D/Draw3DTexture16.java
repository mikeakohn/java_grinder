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

package net.mikekohn.java_grinder.Draw3D;

public class Draw3DTexture16 extends Draw3DTexture
{
   public Draw3DTexture16()
   {
   }

   public Draw3DTexture16(int width, int height)
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

