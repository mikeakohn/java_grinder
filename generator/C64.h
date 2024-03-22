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
  virtual int c64_sid_frequency1();
  virtual int c64_sid_pulseWidth1();
  virtual int c64_sid_waveform1();
  virtual int c64_sid_adsr1();
  virtual int c64_sid_frequency2();
  virtual int c64_sid_pulseWidth2();
  virtual int c64_sid_waveform2();
  virtual int c64_sid_adsr2();
  virtual int c64_sid_frequency3();
  virtual int c64_sid_pulseWidth3();
  virtual int c64_sid_waveform3();
  virtual int c64_sid_adsr3();
  virtual int c64_sid_filterCutoff();
  virtual int c64_sid_filterResonance();
  virtual int c64_sid_volume();
  virtual int c64_sid_oscillator3();
  virtual int c64_sid_envelope3();
  virtual int c64_sid_clear();

  virtual int c64_vic_sprite0pos();
  virtual int c64_vic_sprite1pos();
  virtual int c64_vic_sprite2pos();
  virtual int c64_vic_sprite3pos();
  virtual int c64_vic_sprite4pos();
  virtual int c64_vic_sprite5pos();
  virtual int c64_vic_sprite6pos();
  virtual int c64_vic_sprite7pos();
  virtual int c64_vic_writeControl1();
  virtual int c64_vic_readControl1();
  virtual int c64_vic_spriteEnable();
  virtual int c64_vic_writeControl2();
  virtual int c64_vic_readControl2();
  virtual int c64_vic_spriteExpandY();
  virtual int c64_vic_writePointer();
  virtual int c64_vic_readPointer();
  virtual int c64_vic_writeInterruptStatus();
  virtual int c64_vic_readInterruptStatus();
  virtual int c64_vic_interruptControl();
  virtual int c64_vic_spritePriority();
  virtual int c64_vic_spriteMulticolorEnable();
  virtual int c64_vic_spriteExpandX();
  virtual int c64_vic_spriteCollision();
  virtual int c64_vic_dataCollision();
  virtual int c64_vic_border();
  virtual int c64_vic_background();
  virtual int c64_vic_multicolor1();
  virtual int c64_vic_multicolor2();
  virtual int c64_vic_multicolor3();
  virtual int c64_vic_spriteMulticolor0();
  virtual int c64_vic_spriteMulticolor1();
  virtual int c64_vic_sprite0color();
  virtual int c64_vic_sprite1color();
  virtual int c64_vic_sprite2color();
  virtual int c64_vic_sprite3color();
  virtual int c64_vic_sprite4color();
  virtual int c64_vic_sprite5color();
  virtual int c64_vic_sprite6color();
  virtual int c64_vic_sprite7color();

  virtual int c64_vic_hiresEnable();
  virtual int c64_vic_hiresClear();
  virtual int c64_vic_hiresPlot();
  virtual int c64_vic_makeHiresTables();
  virtual int c64_vic_textEnable();
  virtual int c64_vic_textClear();
  virtual int c64_vic_textCopy();
  virtual int c64_vic_textPlot();
  virtual int c64_vic_textAsciiPlot();
  virtual int c64_vic_textRead();
  virtual int c64_vic_textString();
  virtual int c64_vic_textFill();
  virtual int c64_vic_textPaint();
  virtual int c64_vic_textScrollLeft();
  virtual int c64_vic_textScrollRight();
  virtual int c64_vic_textScrollUp();
  virtual int c64_vic_textScrollDown();
  virtual int c64_vic_makeTextTable();
  virtual int c64_vic_makeColorTable();
  virtual int c64_vic_colorRamClear();
  virtual int c64_vic_copyUppercase();
  virtual int c64_vic_copyLowercase();
  virtual int c64_vic_copyDataFromArray();

  virtual int timer_setInterval_II();
  virtual int timer_setInterval_II(int cycles, int divider);
  virtual int timer_setListener_Z();
  virtual int timer_setListener_Z(int const_value);
  virtual int timer_getValue();
  virtual int timer_setValue_I();
  virtual int timer_setValue_I(int const_value);

  virtual int joystick_isRight();
  virtual int joystick_isRight_I(int index);
  virtual int joystick_isLeft();
  virtual int joystick_isLeft_I(int index);
  virtual int joystick_isDown();
  virtual int joystick_isDown_I(int index);
  virtual int joystick_isUp();
  virtual int joystick_isUp_I(int index);
  virtual int joystick_isButtonDown_0();
  virtual int joystick_isButtonDown_0_I(int index);

  virtual int keyboard_isKeyPressed_I();
  virtual int keyboard_isKeyPressed_I(int index);
  virtual int keyboard_currentKeyPressed();

  virtual int grinder_largeJavaStack();

  virtual int return_void(int local_count);

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
  bool need_c64_vic_text_ascii_plot:1;
  bool need_c64_vic_text_read:1;
  bool need_c64_vic_text_string:1;
  bool need_c64_vic_text_fill:1;
  bool need_c64_vic_text_paint:1;
  bool need_c64_vic_text_scroll_left:1;
  bool need_c64_vic_text_scroll_right:1;
  bool need_c64_vic_text_scroll_up:1;
  bool need_c64_vic_text_scroll_down:1;
  bool need_c64_vic_make_text_table:1;
  bool need_c64_vic_make_color_table:1;
  bool need_c64_vic_color_ram_clear:1;
  bool need_c64_vic_copy_uppercase:1;
  bool need_c64_vic_copy_lowercase:1;
  bool need_c64_vic_copy_data_from_array:1;
  bool need_c64_timer_interrupt:1;
  bool need_c64_keyboard:1;

  void insert_c64_vic_hires_enable();
  void insert_c64_vic_hires_clear();
  void insert_c64_vic_hires_plot();
  void insert_c64_vic_make_hires_tables();
  void insert_c64_vic_text_enable();
  void insert_c64_vic_text_clear();
  void insert_c64_vic_text_copy();
  void insert_c64_vic_text_plot();
  void insert_c64_vic_text_ascii_plot();
  void insert_c64_vic_text_read();
  void insert_c64_vic_text_string();
  void insert_c64_vic_text_fill();
  void insert_c64_vic_text_paint();
  void insert_c64_vic_text_scroll_left();
  void insert_c64_vic_text_scroll_right();
  void insert_c64_vic_text_scroll_up();
  void insert_c64_vic_text_scroll_down();
  void insert_c64_vic_make_text_table();
  void insert_c64_vic_make_color_table();
  void insert_c64_vic_color_ram_clear();
  void insert_c64_vic_copy_uppercase();
  void insert_c64_vic_copy_lowercase();
  void insert_c64_vic_copy_data_from_array();
  void insert_c64_keyboard();
  void insert_c64_sprite_interrupt();
};

#endif

