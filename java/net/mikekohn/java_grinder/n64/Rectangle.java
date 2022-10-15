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

  public void setPosition(int x, int y) { }
  public void setSize(int width, int height) { }
  public void setTextureEnabled(int width, int height) { }
  public void setTextureDisabled() { }

  public void draw() { }

  private short x, y;
  private short width, height;
  private int color;
  private short[] texture;
  private short texture_width, texture_height;
}

