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

package net.mikekohn.java_grinder.n64;

public class Triangle
{
  public Triangle()
  {
    do_zbuffer = true;
  }

  /** Value is 32 bit RRGGBBAA. So 0x00ff00ff would be green. */
  public void setColor(int value) { }

  public void setPosition(int dx, int dy, int dz) { }

  /** Values are between 0 and 511. */
  public void setRotation(int rx, int ry, int rz) { }


  public void setVertex0(int x, int y, int z) { }
  public void setVertex1(int x, int y, int z) { }
  public void setVertex2(int x, int y, int z) { }

  /** Array of [ x0, y0, z0, x1, y1, z1, x2, y2, z2 ]. */
  public void setVertexes(short[] points) { }
  public void setVertexes(short[] points, int offset) { }

  public void setZBuffer(boolean is_enabled) { }

  public void draw() { }

  private short x0, y0, z0;
  private short x1, y1, z1;
  private short x2, y2, z2;
  private short dx, dy, dz;
  private short rx, ry, rz;
  private int color;
  private boolean do_zbuffer;
}

