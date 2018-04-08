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

public class Draw3DTexture
{
   public Draw3DTexture()
   {
   }

   public Draw3DTexture(int width, int height)
   {
   }

   /** Sets the color of a pixel in a texture buffer at the index
       (x + (y * width)) provided. */
   public void setPixel(int index, int color) { }

   /** Sets the color of a multiple pixels in a texture buffer at the index
       (x + (y * width)) provided. */
   public void setPixels(int index, int[] pixels) { }

   /** If the color is black (RGB=0,0,0) the pixels will be transparent. */
   public void enableTransparency() { }

   /** If the color is black (RGB=0,0,0) the pixels will be black. */
   public void disableTransparency() { }

   /** Uploads the texture from main memory into the Graphics Synthesizer
       (the video chip) of the Playstation 2. */
   public void upload() { }
}

