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

abstract public class Draw3DObjectWithTexture extends Draw3DObject
{
   public Draw3DObjectWithTexture()
   {
   }

   public Draw3DObjectWithTexture(String filename)
   {
   }

   public Draw3DObjectWithTexture(int point_count)
   {
   }

   public void setTextureCoord(int index, float s, float t) { }
   public void setTextureCoords(float[] coords) { }
}

