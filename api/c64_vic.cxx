/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "api/cpu.h"

#define CHECK_FUNC(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return c64_vic_##funct(java_class, generator); \
  }

static int c64_vic_sprite0pos(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite0pos();
}

static int c64_vic_sprite1pos(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite1pos();
}

static int c64_vic_sprite2pos(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite2pos();
}

static int c64_vic_sprite3pos(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite3pos();
}

static int c64_vic_sprite4pos(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite4pos();
}

static int c64_vic_sprite5pos(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite5pos();
}

static int c64_vic_sprite6pos(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite6pos();
}

static int c64_vic_sprite7pos(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite7pos();
}

static int c64_vic_write_control1(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_write_control1();
}

static int c64_vic_read_control1(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_read_control1();
}

static int c64_vic_wait_raster(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_wait_raster();
}

static int c64_vic_sprite_enable(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite_enable();
}

static int c64_vic_write_control2(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_write_control2();
}

static int c64_vic_read_control2(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_read_control2();
}

static int c64_vic_sprite_expandy(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite_expandy();
}

static int c64_vic_write_pointer(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_write_pointer();
}

static int c64_vic_read_pointer(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_read_pointer();
}

static int c64_vic_write_interrupt_status(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_write_interrupt_status();
}

static int c64_vic_read_interrupt_status(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_read_interrupt_status();
}

static int c64_vic_interrupt_control(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_interrupt_control();
}

static int c64_vic_sprite_priority(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite_priority();
}

static int c64_vic_sprite_multicolor_enable(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite_multicolor_enable();
}

static int c64_vic_sprite_expandx(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite_expandx();
}

static int c64_vic_sprite_collision(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite_collision();
}

static int c64_vic_data_collision(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_data_collision();
}

static int c64_vic_border(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_border();
}

static int c64_vic_background(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_background();
}

static int c64_vic_multi1(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_multi1();
}

static int c64_vic_multi2(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_multi2();
}

static int c64_vic_multi3(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_multi3();
}

static int c64_vic_sprite_multicolor0(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite_multicolor0();
}

static int c64_vic_sprite_multicolor1(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite_multicolor1();
}

static int c64_vic_sprite0color(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite0color();
}

static int c64_vic_sprite1color(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite1color();
}

static int c64_vic_sprite2color(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite2color();
}

static int c64_vic_sprite3color(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite3color();
}

static int c64_vic_sprite4color(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite4color();
}

static int c64_vic_sprite5color(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite5color();
}

static int c64_vic_sprite6color(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite6color();
}

static int c64_vic_sprite7color(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite7color();
}

static int c64_vic_hires_enable(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_hires_enable();
}

static int c64_vic_hires_clear(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_hires_clear();
}

static int c64_vic_hires_plot(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_hires_plot();
}

static int c64_vic_make_hires_tables(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_make_hires_tables();
}

static int c64_vic_text_enable(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_text_enable();
}

static int c64_vic_text_clear(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_text_clear();
}

static int c64_vic_text_copy(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_text_copy();
}

static int c64_vic_text_plot(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_text_plot();
}

static int c64_vic_text_read(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_text_read();
}

static int c64_vic_make_text_table(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_make_text_table();
}

static int c64_vic_make_color_table(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_make_color_table();
}

static int c64_vic_color_ram_clear(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_color_ram_clear();
}

static int c64_vic_copy_uppercase(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_copy_uppercase();
}

static int c64_vic_copy_lowercase(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_copy_lowercase();
}

int c64_vic(JavaClass *java_class, Generator *generator, const char *function)
{
  CHECK_FUNC(sprite0pos)
  CHECK_FUNC(sprite1pos)
  CHECK_FUNC(sprite2pos)
  CHECK_FUNC(sprite3pos)
  CHECK_FUNC(sprite4pos)
  CHECK_FUNC(sprite5pos)
  CHECK_FUNC(sprite6pos)
  CHECK_FUNC(sprite7pos)
  CHECK_FUNC(write_control1)
  CHECK_FUNC(read_control1)
  CHECK_FUNC(wait_raster)
  CHECK_FUNC(sprite_enable)
  CHECK_FUNC(write_control2)
  CHECK_FUNC(read_control2)
  CHECK_FUNC(sprite_expandy)
  CHECK_FUNC(write_pointer)
  CHECK_FUNC(read_pointer)
  CHECK_FUNC(write_interrupt_status)
  CHECK_FUNC(read_interrupt_status)
  CHECK_FUNC(interrupt_control)
  CHECK_FUNC(sprite_priority)
  CHECK_FUNC(sprite_multicolor_enable)
  CHECK_FUNC(sprite_expandx)
  CHECK_FUNC(sprite_collision)
  CHECK_FUNC(data_collision)
  CHECK_FUNC(border)
  CHECK_FUNC(background)
  CHECK_FUNC(multi1)
  CHECK_FUNC(multi2)
  CHECK_FUNC(multi3)
  CHECK_FUNC(sprite_multicolor0)
  CHECK_FUNC(sprite_multicolor1)
  CHECK_FUNC(sprite0color)
  CHECK_FUNC(sprite1color)
  CHECK_FUNC(sprite2color)
  CHECK_FUNC(sprite3color)
  CHECK_FUNC(sprite4color)
  CHECK_FUNC(sprite5color)
  CHECK_FUNC(sprite6color)
  CHECK_FUNC(sprite7color)
  CHECK_FUNC(hires_enable)
  CHECK_FUNC(hires_clear)
  CHECK_FUNC(hires_plot)
  CHECK_FUNC(make_hires_tables)
  CHECK_FUNC(text_enable)
  CHECK_FUNC(text_clear)
  CHECK_FUNC(text_copy)
  CHECK_FUNC(text_plot)
  CHECK_FUNC(text_read)
  CHECK_FUNC(make_text_table)
  CHECK_FUNC(make_color_table)
  CHECK_FUNC(color_ram_clear)
  CHECK_FUNC(copy_uppercase)
  CHECK_FUNC(copy_lowercase)

  return -1;
}
