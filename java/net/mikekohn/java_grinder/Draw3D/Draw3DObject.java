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

   public Draw3DObject(int point_count)
   {
   }

   public void setContext(int context) { }
   public void rotateX512(int rx) { }
   public void rotateY512(int ry) { }
   public void rotateZ512(int rz) { }
   public void setPosition(float x, float y, float z) { }
   public void setPoint(int index, float x, float y, float z) { }
   public void setPointColor(int index, int color) { }
   public void setPoints(float[] points) { }
   public void setPointColors(int[] colors) { }
   public void disableGouraudShading() { }
   public void enableGouraudShading() { }
   public void disableFogging() { }
   public void enableFogging() { }
   public void disableTexture() { }
   public void enableTexture() { }
   public void disableAlphaBlending() { }
   public void enableAlphaBlending() { }
   public void disableAntialiasing() { }
   public void enableAntialiasing() { }
   public void draw() { }

   protected float rx, ry, rz;
   protected int x, y, z;
}

