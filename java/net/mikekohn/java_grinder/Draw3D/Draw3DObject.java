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

abstract public class Draw3DObject
{
   public Draw3DObject()
   {
   }

   public Draw3DObject(String filename)
   {
   }

   public Draw3DObject(int points)
   {
   }

   public void rotate512(int rx, int ry, int rz) { }
   public void setPosition(float x, float y, float z) { }
   public void setPointPosition(int index, float x, float y, float z) { }
   public void setPointColor(int index, int color) { }
   public void draw() { }

   private float rx, ry, rz;
   private int x, y, z;
}

