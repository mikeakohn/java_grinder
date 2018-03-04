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

#ifndef _PLAYSTATION_2_H
#define _PLAYSTATION_2_H

#include "MIPS64.h"

class Playstation2 : public MIPS64
{
public:
  Playstation2();
  virtual ~Playstation2();

  virtual int open(const char *filename);
  virtual int start_init();
  virtual int new_object(const char *object_name, int field_count);
  virtual int draw3d_Constructor_X(int type);
  virtual int draw3d_Constructor_I(int type);
  virtual int draw3d_rotateX512_I();
  virtual int draw3d_rotateY512_I();
  virtual int draw3d_rotateZ512_I();
  virtual int draw3d_setPosition_FFF();
  virtual int draw3d_setPointPosition_IFFF();
  virtual int draw3d_setPointColor_II();
  virtual int draw3d_draw();

  virtual int playstation2_clearScreen();
  virtual int playstation2_waitVsync();

private:
  void add_dma_reset();
  void add_dma_wait();
  void add_copy_vu1_code();
  void add_screen_init_clear();
  void add_primitive_gif_tag();
  void add_vu1_code();
};

#endif

