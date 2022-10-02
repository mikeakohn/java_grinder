/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2022 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_NINTENDO64_H
#define JAVA_GRINDER_GENERATOR_NINTENDO64_H

#include "generator/R4000.h"

class Nintendo64 : public R4000
{
public:
  Nintendo64();
  virtual ~Nintendo64();

  virtual int open(const char *filename);
  virtual int finish();
  virtual int start_init();

  virtual int nintendo64_waitVsync();
  virtual int nintendo64_clearScreen();
  virtual int nintendo64_setScreen_I();
  virtual int nintendo64_plot_III();

  virtual int nintendo64_n64_triangle_Constructor();
  virtual int nintendo64_n64_triangle_setColor_I();
  virtual int nintendo64_n64_triangle_setPosition_III();
  virtual int nintendo64_n64_triangle_setRotation_III();
  virtual int nintendo64_n64_triangle_setVertex0_III();
  virtual int nintendo64_n64_triangle_setVertex1_III();
  virtual int nintendo64_n64_triangle_setVertex2_III();
  virtual int nintendo64_n64_triangle_setVertexes_bS();
  virtual int nintendo64_n64_triangle_draw();

  virtual int nintendo64_n64_rectangle_Constructor();
  virtual int nintendo64_n64_rectangle_setColor_I();
  virtual int nintendo64_n64_rectangle_setPosition_II();
  virtual int nintendo64_n64_rectangle_setSize_II();
  virtual int nintendo64_n64_rectangle_setTexture_aBII();
  virtual int nintendo64_n64_rectangle_draw();

private:
  void catridge_header();
  void init_system();
  void ntsc_setup();
};

#endif

