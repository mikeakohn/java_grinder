/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

#ifndef _TI99_H
#define _TI99_H

#include "TMS9900.h"

class TI99 : public TMS9900
{
public:
  TI99();
  virtual ~TI99();

  virtual int open(const char *filename);
  virtual int start_init();
  virtual int ti99_print();
  virtual int ti99_printChar();
  virtual int ti99_printChar(int c);
  virtual int ti99_setCursor();
  virtual int ti99_setCursor(int x, int y);
  //virtual int ti99_setTextColor();
  virtual int ti99_setGraphicsMode();
  virtual int ti99_setGraphicsMode(int mode);
  virtual int ti99_clearScreen();
  virtual int ti99_plot();
  virtual int ti99_setColors();
  virtual int ti99_setSoundFreq();
  virtual int ti99_setSoundVolume();

private:
  void insert_print_string();
  void insert_vdp_command();
  void insert_clear_screen();
  void insert_plot();
  void insert_set_colors();
  void insert_set_sound_freq();
  void insert_set_sound_volume();

  bool need_vdp_command:1;
  bool need_print_string:1;
  bool need_clear_screen:1;
  bool need_plot:1;
  bool need_set_colors:1;
  bool need_set_sound_freq:1;
  bool need_set_sound_volume:1;
  char app_name[16];
};

#endif

