/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2014 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder.c64;

public class VIC
{
  private VIC()
  {
  }

  // 0xd000
  // 0xd001
  public static void sprite0pos(int x, int y) { }

  // 0xd002
  // 0xd003
  public static void sprite1pos(int x, int y) { }

  // 0xd004
  // 0xd005
  public static void sprite2pos(int x, int y) { }

  // 0xd006
  // 0xd007
  public static void sprite3pos(int x, int y) { }

  // 0xd008
  // 0xd009
  public static void sprite4pos(int x, int y) { }

  // 0xd00a
  // 0xd00b
  public static void sprite5pos(int x, int y) { }

  // 0xd00c
  // 0xd00d
  public static void sprite6pos(int x, int y) { }

  // 0xd00e
  // 0xd00f
  public static void sprite7pos(int x, int y) { }

  // 0xd010

  // 0xd011
  public static void write_control1(int value) { }
  public static int read_control1() { return 0; }

  // 0xd012
  public static void wait_raster(int value) { }

  // 0xd013
  // 0xd014

  // 0xd015
  public static void sprite_enable(int value) { }

  // 0xd016
  public static void write_control2(int value) { }
  public static int read_control2() { return 0; }

  // 0xd017
  public static void sprite_expandy(int value) { }

  // 0xd018
  public static void write_pointer(int value) { }
  public static int read_pointer() { return 0; }

  // 0xd019
  public static void write_interrupt_status(int value) { }
  public static int read_interrupt_status() { return 0; }

  // 0xd01a
  public static void interrupt_control(int value) { }

  // 0xd01b
  public static void sprite_priority(int value) { }

  // 0xd01c
  public static void sprite_multicolor_enable(int value) { }

  // 0xd01d
  public static void sprite_expandx(int value) { }

  // 0xd01e
  public static int sprite_collision() { return 0; }

  // 0xd01f
  public static int data_collision() { return 0; }

  // 0xd020
  public static void border(int value) { }

  // 0xd021
  public static void background(int value) { }

  // 0xd022
  public static void multi1(int value) { }

  // 0xd023
  public static void multi2(int value) { }

  // 0xd024
  public static void multi3(int value) { }

  // 0xd025
  public static void sprite_multicolor0(int value) { }

  // 0xd026
  public static void sprite_multicolor1(int value) { }

  // 0xd027
  public static void sprite0color(int value) { }

  // 0xd028
  public static void sprite1color(int value) { }

  // 0xd029
  public static void sprite2color(int value) { }

  // 0xd02a
  public static void sprite3color(int value) { }

  // 0xd02b
  public static void sprite4color(int value) { }

  // 0xd02c
  public static void sprite5color(int value) { }

  // 0xd02d
  public static void sprite6color(int value) { }

  // 0xd02e
  public static void sprite7color(int value) { }

  public static void hires_enable() { }
  public static void hires_clear(int value) { }
  public static void hires_plot(int x, int y, int value) { }
  public static void make_hires_tables() { }
  public static void text_enable() { }
  public static void text_clear(int value) { }
  public static void text_copy() { }
  public static void text_plot(int x, int y, int value, int color) { }
  public static int text_read(int x, int y) { return 0; }
  public static void make_text_table() { }
  public static void make_color_table() { }
  public static void color_ram_clear(int value) { }
  public static void copy_uppercase() { }
  public static void copy_lowercase() { }
}

