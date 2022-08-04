/**
 *  Java Grinder
 *  Author: Joe Davisson
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_API_C64_H
#define JAVA_GRINDER_GENERATOR_API_C64_H

class API_C64
{
public:
  // Commodore 64 stuff
  virtual int c64_sid_voice1_frequency() { return -1; }
  virtual int c64_sid_voice1_pulse_width() { return -1; }
  virtual int c64_sid_voice1_waveform() { return -1; }
  virtual int c64_sid_voice1_adsr() { return -1; }
  virtual int c64_sid_voice2_frequency() { return -1; }
  virtual int c64_sid_voice2_pulse_width() { return -1; }
  virtual int c64_sid_voice2_waveform() { return -1; }
  virtual int c64_sid_voice2_adsr() { return -1; }
  virtual int c64_sid_voice3_frequency() { return -1; }
  virtual int c64_sid_voice3_pulse_width() { return -1; }
  virtual int c64_sid_voice3_waveform() { return -1; }
  virtual int c64_sid_voice3_adsr() { return -1; }
  virtual int c64_sid_filter_cutoff() { return -1; }
  virtual int c64_sid_filter_resonance() { return -1; }
  virtual int c64_sid_volume() { return -1; }
  virtual int c64_sid_voice3_oscillator() { return -1; }
  virtual int c64_sid_voice3_envelope() { return -1; }
  virtual int c64_sid_clear() { return -1; }

  virtual int c64_vic_sprite0pos() { return -1; }
  virtual int c64_vic_sprite1pos() { return -1; }
  virtual int c64_vic_sprite2pos() { return -1; }
  virtual int c64_vic_sprite3pos() { return -1; }
  virtual int c64_vic_sprite4pos() { return -1; }
  virtual int c64_vic_sprite5pos() { return -1; }
  virtual int c64_vic_sprite6pos() { return -1; }
  virtual int c64_vic_sprite7pos() { return -1; }
  virtual int c64_vic_write_control1() { return -1; }
  virtual int c64_vic_read_control1() { return -1; }
  virtual int c64_vic_wait_raster() { return -1; }
  virtual int c64_vic_sprite_enable() { return -1; }
  virtual int c64_vic_write_control2() { return -1; }
  virtual int c64_vic_read_control2() { return -1; }
  virtual int c64_vic_sprite_expandy() { return -1; }
  virtual int c64_vic_write_pointer() { return -1; }
  virtual int c64_vic_read_pointer() { return -1; }
  virtual int c64_vic_write_interrupt_status() { return -1; }
  virtual int c64_vic_read_interrupt_status() { return -1; }
  virtual int c64_vic_interrupt_control() { return -1; }
  virtual int c64_vic_sprite_priority() { return -1; }
  virtual int c64_vic_sprite_multicolor_enable() { return -1; }
  virtual int c64_vic_sprite_expandx() { return -1; }
  virtual int c64_vic_sprite_collision() { return -1; }
  virtual int c64_vic_data_collision() { return -1; }
  virtual int c64_vic_border() { return -1; }
  virtual int c64_vic_background() { return -1; }
  virtual int c64_vic_multi1() { return -1; }
  virtual int c64_vic_multi2() { return -1; }
  virtual int c64_vic_multi3() { return -1; }
  virtual int c64_vic_sprite_multicolor0() { return -1; }
  virtual int c64_vic_sprite_multicolor1() { return -1; }
  virtual int c64_vic_sprite0color() { return -1; }
  virtual int c64_vic_sprite1color() { return -1; }
  virtual int c64_vic_sprite2color() { return -1; }
  virtual int c64_vic_sprite3color() { return -1; }
  virtual int c64_vic_sprite4color() { return -1; }
  virtual int c64_vic_sprite5color() { return -1; }
  virtual int c64_vic_sprite6color() { return -1; }
  virtual int c64_vic_sprite7color() { return -1; }
  virtual int c64_vic_hires_enable() { return -1; }
  virtual int c64_vic_hires_clear() { return -1; }
  virtual int c64_vic_hires_plot() { return -1; }
  virtual int c64_vic_make_hires_tables() { return -1; }
  virtual int c64_vic_text_enable() { return -1; }
  virtual int c64_vic_text_clear() { return -1; }
  virtual int c64_vic_text_copy() { return -1; }
  virtual int c64_vic_text_plot() { return -1; }
  virtual int c64_vic_text_read() { return -1; }
  virtual int c64_vic_make_text_table() { return -1; }
  virtual int c64_vic_make_color_table() { return -1; }
  virtual int c64_vic_color_ram_clear() { return -1; }
  virtual int c64_vic_copy_uppercase() { return -1; }
  virtual int c64_vic_copy_lowercase() { return -1; }
};

#endif

