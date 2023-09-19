/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2023 by Michael Kohn
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
  virtual int new_object(std::string &object_name, int field_count);

  virtual int nintendo64_waitVsync();
  virtual int nintendo64_clearScreen();
  virtual int nintendo64_resetZBuffer();
  virtual int nintendo64_setScreen_I();
  virtual int nintendo64_plot_II();
  virtual int nintendo64_plot_III();
  virtual int nintendo64_loadTexture_aSII();
  virtual int nintendo64_waitForPolygon();

  virtual int nintendo64_getAudioStatus();
  virtual int nintendo64_setAudioDACRate_I();
  virtual int nintendo64_setAudioBitRate_I();
  virtual int nintendo64_playAudio_aSII();

  virtual int nintendo64_n64_triangle_Constructor();
  virtual int nintendo64_n64_triangle_setColor_I();
  virtual int nintendo64_n64_triangle_setPosition_III();
  virtual int nintendo64_n64_triangle_setRotation_III();
  virtual int nintendo64_n64_triangle_setVertex0_III();
  virtual int nintendo64_n64_triangle_setVertex1_III();
  virtual int nintendo64_n64_triangle_setVertex2_III();
  virtual int nintendo64_n64_triangle_setVertexes_aS();
  virtual int nintendo64_n64_triangle_setVertexes_aSI();
  virtual int nintendo64_n64_triangle_setZBuffer_Z();
  virtual int nintendo64_n64_triangle_draw();

  virtual int nintendo64_n64_rectangle_Constructor();
  virtual int nintendo64_n64_rectangle_setColor_I();
  virtual int nintendo64_n64_rectangle_setPosition_II();
  virtual int nintendo64_n64_rectangle_setSize_II();
  virtual int nintendo64_n64_rectangle_setTextureEnabled_II();
  virtual int nintendo64_n64_rectangle_setTextureDisabled();
  virtual int nintendo64_n64_rectangle_draw();

  virtual int math_sin512_I();
  virtual int math_cos512_I();

private:
  void catridge_header();
  void init_system();
  void copy_1mb_from_rom();
  void rsp_halt();
  void rsp_start();
  void rsp_copy_code();
  void rsp_copy_cos_table();
  void rdp_copy_instructions();
  void insert_ntsc_setup();
  void insert_rsp_data();
  void insert_run_rdp_screen_setup();
  void insert_clear_screen();
  void insert_reset_z_buffer();
  void insert_set_screen();
  void insert_triangle_constructor();
  void insert_rectangle_constructor();
  void insert_triangle_draw();
  void insert_rectangle_draw();
  void insert_triangle_vertex_copy();
  void insert_load_texture();
  void insert_cos_table();
  void insert_rsp_code();
};

#endif

