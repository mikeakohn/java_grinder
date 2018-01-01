/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2014-2017 by Michael Kohn
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

   public void rotate(int rx, int ry, int rz) { }
   public void setPosition(int x, int y, int z) { }
   public void setPointPosition(int index, int x, int y, int z) { }
   public void setPointColor(int index, int color) { }
   public void draw() { }

   float rx, ry, rz;
   int x, y, z;
}

