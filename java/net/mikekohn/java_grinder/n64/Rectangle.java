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

  /** Position of the rectangle on the screen. */
  public void setPosition(int x, int y) { }

  /** Size of the Rectangle. */
  public void setSize(int width, int height) { }

  /** Enable the drawing of texture with the specified width, height. */
  public void setTextureEnabled(int width, int height) { }

  /** Do not draw texture on rectangle. */
  public void setTextureDisabled() { }

  /** Draw the rectangle on the screen. */
  public void draw() { }

  private short x, y;
  private short width, height;
  private int color;
  private short[] texture;
  private short texture_width, texture_height;
}

