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

public class Rectangle
{
  public Rectangle()
  {
  }

  /** Value is 32 bit RRGGBBAA. So 0x00ff00ff would be green. */
  public void setColor(int value) { }

  public void setPosition(short dx, short dy) { }
  public void setSize(short width, short height) { }
  public void setTexture(short[] texture, short width, short height) { }


  public void draw() { }

  private short x0, y0;
  private short x1, y1;
  private int color;
  private short[] texture;
  private short texture_width, texture_height;
}

