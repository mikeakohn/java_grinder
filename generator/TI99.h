/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2023 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_TI99_H
#define JAVA_GRINDER_GENERATOR_TI99_H

#include "generator/TMS9900.h"

class TI99 : public TMS9900
{
public:
  TI99();
  virtual ~TI99();

  virtual int open(const char *filename);
  virtual int start_init();
  virtual int finish();
  virtual int tms9918a_print_X();
  virtual int tms9918a_printChar_C();
  virtual int tms9918a_printChar_C(int c);
  virtual int tms9918a_setCursor_II();
  virtual int tms9918a_setCursor_II(int x, int y);
  virtual int tms9918a_setGraphicsMode_I();
  virtual int tms9918a_setGraphicsMode_I(int mode);
  virtual int tms9918a_clearScreen();
  virtual int tms9918a_plot_III();
  virtual int tms9918a_initDisplay();
  virtual int tms9918a_setSpriteVisible_IZ();
  virtual int tms9918a_setSpriteImage_IaB();
  virtual int tms9918a_setSpritePos_III();
  virtual int tms9918a_setSpriteColor_II();
  virtual int tms9918a_setSpriteSize_I();

  virtual int sn76489_setSoundFreq_II();
  virtual int sn76489_setSoundVolume_II();

private:
  void insert_print_string();
  void insert_vdp_command();
  void insert_clear_screen();
  void insert_plot();
  void insert_set_colors();
  void insert_set_sound_freq();
  void insert_set_sound_volume();
  void insert_set_sprite_visible();
  void insert_set_sprite_image();
  void insert_set_sprite_pos();
  void insert_set_sprite_color();

  bool need_vdp_command : 1;
  bool need_print_string : 1;
  bool need_clear_screen : 1;
  bool need_plot : 1;
  bool need_set_colors : 1;
  bool need_set_sound_freq : 1;
  bool need_set_sound_volume : 1;
  bool need_set_sprite_visible : 1;
  bool need_set_sprite_image : 1;
  bool need_set_sprite_pos : 1;
  bool need_set_sprite_color : 1;
  char app_name[16];
};

#endif

