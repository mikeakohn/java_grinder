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

#ifndef _TI99_H
#define _TI99_H

#include "generator/TMS9900.h"

class TI99 : public TMS9900
{
public:
  TI99();
  virtual ~TI99();

  virtual int open(const char *filename);
  virtual int start_init();
  virtual int ti99_print_X();
  virtual int ti99_printChar_C();
  virtual int ti99_printChar_C(int c);
  virtual int ti99_setCursor_II();
  virtual int ti99_setCursor_II(int x, int y);
  virtual int ti99_setGraphicsMode_I();
  virtual int ti99_setGraphicsMode_I(int mode);
  virtual int ti99_clearScreen();
  virtual int ti99_plot_III();
  virtual int ti99_setColors();
  virtual int ti99_setSoundFreq_II();
  virtual int ti99_setSoundVolume_II();
  virtual int ti99_setSpriteVisible_IZ();
  virtual int ti99_setSpriteImage_IaB();
  virtual int ti99_setSpritePos_III();
  virtual int ti99_setSpriteColor_II();
  virtual int ti99_setSpriteSize_I();

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

  bool need_vdp_command:1;
  bool need_print_string:1;
  bool need_clear_screen:1;
  bool need_plot:1;
  bool need_set_colors:1;
  bool need_set_sound_freq:1;
  bool need_set_sound_volume:1;
  bool need_set_sprite_visible:1;
  bool need_set_sprite_image:1;
  bool need_set_sprite_pos:1;
  bool need_set_sprite_color:1;
  char app_name[16];
};

#endif

