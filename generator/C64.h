/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014 by Michael Kohn
 *
 */

#ifndef _C64_H
#define _C64_H

#include "M6502.h"

class C64 : public M6502
{
public:
  C64();
  virtual ~C64();

  virtual int open(const char *filename);

  // API
  virtual int c64_sid_voice1_frequency();
  virtual int c64_sid_voice1_pulse_width();
  virtual int c64_sid_voice1_waveform();
  virtual int c64_sid_voice1_adsr();
  virtual int c64_sid_voice2_frequency();
  virtual int c64_sid_voice2_pulse_width();
  virtual int c64_sid_voice2_waveform();
  virtual int c64_sid_voice2_adsr();
  virtual int c64_sid_voice3_frequency();
  virtual int c64_sid_voice3_pulse_width();
  virtual int c64_sid_voice3_waveform();
  virtual int c64_sid_voice3_adsr();
  virtual int c64_sid_filter_cutoff();
  virtual int c64_sid_filter_resonance();
  virtual int c64_sid_volume();
  virtual int c64_sid_voice3_oscillator();
  virtual int c64_sid_voice3_envelope();
  virtual int c64_sid_clear();

  virtual int c64_vic_sprite0pos();
  virtual int c64_vic_sprite1pos();
  virtual int c64_vic_sprite2pos();
  virtual int c64_vic_sprite3pos();
  virtual int c64_vic_sprite4pos();
  virtual int c64_vic_sprite5pos();
  virtual int c64_vic_sprite6pos();
  virtual int c64_vic_sprite7pos();
  virtual int c64_vic_write_control1();
  virtual int c64_vic_read_control1();
  virtual int c64_vic_wait_raster();
  virtual int c64_vic_sprite_enable();
  virtual int c64_vic_write_control2();
  virtual int c64_vic_read_control2();
  virtual int c64_vic_sprite_expandy();
  virtual int c64_vic_write_pointer();
  virtual int c64_vic_read_pointer();
  virtual int c64_vic_sprite_priority();
  virtual int c64_vic_sprite_multicolor_enable();
  virtual int c64_vic_sprite_expandx();
  virtual int c64_vic_sprite_collision();
  virtual int c64_vic_data_collision();
  virtual int c64_vic_border();
  virtual int c64_vic_background();
  virtual int c64_vic_background1();
  virtual int c64_vic_background2();
  virtual int c64_vic_background3();
  virtual int c64_vic_sprite_multicolor0();
  virtual int c64_vic_sprite_multicolor1();
  virtual int c64_vic_sprite0color();
  virtual int c64_vic_sprite1color();
  virtual int c64_vic_sprite2color();
  virtual int c64_vic_sprite3color();
  virtual int c64_vic_sprite4color();
  virtual int c64_vic_sprite5color();
  virtual int c64_vic_sprite6color();
  virtual int c64_vic_sprite7color();

  virtual int c64_vic_hires_enable();
  virtual int c64_vic_hires_clear();
  virtual int c64_vic_hires_plot();
  virtual int c64_vic_text_enable();
  virtual int c64_vic_text_clear();
  virtual int c64_vic_text_plot();
  virtual int c64_vic_color_ram_clear();

protected:
  bool need_c64_vic_hires_enable:1;
  bool need_c64_vic_hires_clear:1;
  bool need_c64_vic_hires_plot:1;
  bool need_c64_vic_make_hires_tables:1;
  bool need_c64_vic_text_enable:1;
  bool need_c64_vic_text_clear:1;
  bool need_c64_vic_text_plot:1;
  bool need_c64_vic_color_ram_clear:1;

  void insert_c64_vic_hires_enable();
  void insert_c64_vic_hires_clear();
  void insert_c64_vic_hires_plot();
  void insert_c64_vic_make_hires_tables();
  void insert_c64_vic_text_enable();
  void insert_c64_vic_text_clear();
  void insert_c64_vic_text_plot();
  void insert_c64_vic_color_ram_clear();
};

#endif

