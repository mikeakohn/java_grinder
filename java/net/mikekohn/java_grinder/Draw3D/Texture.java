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

abstract public class Texture
{
   public Texture()
   {
   }

   public Texture(int width, int height)
   {
   }

   /** Sets the color of a pixel in a texture buffer at the index
       (x + (y * width)) provided. */
   public void setPixel(int index, int color) { }

   /** The A value in the image will be used for 16/24 bit textures.
       With 16 bit textures this is only 1 bit. */
   public void enableTransparency() { }

   /** If the color is black (RGB=0,0,0) the pixels will be transparent. */
   public void enableTransparencyOnBlack() { }

   /** No pixels in image will be transparent. */
   public void disableTransparency() { }

   /** Uploads the texture from main memory into the Graphics Synthesizer
       (the video chip) of the Playstation 2. */
   public void upload() { }
}

