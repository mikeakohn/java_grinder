/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 * C64 written by Joe Davisson
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_C64_H
#define JAVA_GRINDER_GENERATOR_C64_H

#include "generator/M6502.h"

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
  virtual int c64_vic_write_interrupt_status();
  virtual int c64_vic_read_interrupt_status();
  virtual int c64_vic_interrupt_control();
  virtual int c64_vic_sprite_priority();
  virtual int c64_vic_sprite_multicolor_enable();
  virtual int c64_vic_sprite_expandx();
  virtual int c64_vic_sprite_collision();
  virtual int c64_vic_data_collision();
  virtual int c64_vic_border();
  virtual int c64_vic_background();
  virtual int c64_vic_multi1();
  virtual int c64_vic_multi2();
  virtual int c64_vic_multi3();
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
  virtual int c64_vic_make_hires_tables();
  virtual int c64_vic_text_enable();
  virtual int c64_vic_text_clear();
  virtual int c64_vic_text_copy();
  virtual int c64_vic_text_plot();
  virtual int c64_vic_text_read();
  virtual int c64_vic_make_text_table();
  virtual int c64_vic_make_color_table();
  virtual int c64_vic_color_ram_clear();
  virtual int c64_vic_copy_uppercase();
  virtual int c64_vic_copy_lowercase();

protected:
  int label_count;

  bool need_c64_vic_hires_enable:1;
  bool need_c64_vic_hires_clear:1;
  bool need_c64_vic_hires_plot:1;
  bool need_c64_vic_make_hires_tables:1;
  bool need_c64_vic_text_enable:1;
  bool need_c64_vic_text_clear:1;
  bool need_c64_vic_text_copy:1;
  bool need_c64_vic_text_plot:1;
  bool need_c64_vic_text_read:1;
  bool need_c64_vic_text_print:1;
  bool need_c64_vic_make_text_table:1;
  bool need_c64_vic_make_color_table:1;
  bool need_c64_vic_color_ram_clear:1;
  bool need_c64_vic_copy_uppercase:1;
  bool need_c64_vic_copy_lowercase:1;

  void insert_c64_vic_hires_enable();
  void insert_c64_vic_hires_clear();
  void insert_c64_vic_hires_plot();
  void insert_c64_vic_make_hires_tables();
  void insert_c64_vic_text_enable();
  void insert_c64_vic_text_clear();
  void insert_c64_vic_text_copy();
  void insert_c64_vic_text_plot();
  void insert_c64_vic_text_read();
  void insert_c64_vic_make_text_table();
  void insert_c64_vic_make_color_table();
  void insert_c64_vic_color_ram_clear();
  void insert_c64_vic_copy_uppercase();
  void insert_c64_vic_copy_lowercase();
};

#endif

