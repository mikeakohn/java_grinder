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

#include "R5900.h"

class Playstation2 : public R5900
{
public:
  Playstation2();
  virtual ~Playstation2();

  virtual int open(const char *filename);
  virtual int start_init();
  virtual int new_object(const char *object_name, int field_count);
  virtual int draw3d_object_Constructor_X(int type);
  virtual int draw3d_object_Constructor_I(int type);
  virtual int draw3d_object_rotateX512_I();
  virtual int draw3d_object_rotateY512_I();
  virtual int draw3d_object_rotateZ512_I();
  virtual int draw3d_object_setPosition_FFF();
  virtual int draw3d_object_setPoint_IFFF();
  virtual int draw3d_object_setPointColor_II();
  virtual int draw3d_object_setPoints_aF();
  virtual int draw3d_object_setPointColors_aI();
  virtual int draw3d_object_disableGouraudShading();
  virtual int draw3d_object_enableGouraudShading();
  virtual int draw3d_object_disableFogging();
  virtual int draw3d_object_enableFogging();
  virtual int draw3d_object_disableTexture();
  virtual int draw3d_object_enableTexture();
  virtual int draw3d_object_disableAlphaBlending();
  virtual int draw3d_object_enableAlphaBlending();
  virtual int draw3d_object_disableAntialiasing();
  virtual int draw3d_object_enableAntialiasing();
  virtual int draw3d_object_draw();
  virtual int draw3d_texture_Constructor_II();
  virtual int draw3d_texture_setPixel_II();
  virtual int draw3d_texture_setPixels_aI();
  virtual int draw3d_texture_upload();

  virtual int playstation2_clearScreen();
  virtual int playstation2_waitVsync();
  virtual int playstation2_vu0UploadCode_aB();
  virtual int playstation2_vu0UploadData_IaB();
  virtual int playstation2_vu0DownloadData_IaB();
  virtual int playstation2_vu0Start();
  virtual int playstation2_vu0Stop();
  virtual int playstation2_vu0IsRunning();
  virtual int playstation2_performanceCountEnable_I();
  virtual int playstation2_performanceCountDisable();
  virtual int playstation2_performanceCountGet();
  virtual int playstation2_performanceCountReset();
  virtual int playstation2_randomInit_I();
  virtual int playstation2_randomGet();
  virtual int playstation2_randomNext();

private:
  void add_dma_reset();
  void add_dma_wait();
  void add_copy_vu1_code();
  void add_screen_init_clear();
  void add_primitive_gif_tag();
  void add_texture_gif_tag();
  void add_vu0_code();
  void add_vu1_code();
  void add_draw3d_object_constructor();
  void add_draw3d_texture_constructor();
};

#endif

