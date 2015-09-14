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

#ifndef _APPLE_II_GS_H
#define _APPLE_II_GS_H

#include "W65816.h"

class AppleIIgs : public W65816
{
public:
  AppleIIgs();
  virtual ~AppleIIgs();

  virtual int open(const char *filename);
  virtual int appleiigs_plotChar_IC();
  virtual int appleiigs_printChar_C();
  virtual int appleiigs_hiresEnable();
  virtual int appleiigs_hiresPlot_III();
  virtual int appleiigs_hiresLine_IIIII();
  virtual int appleiigs_hiresSpan_IIII();
  virtual int appleiigs_hiresRead_II();
  virtual int appleiigs_hiresSprite_aBIIII();
  virtual int appleiigs_hiresPalette_IaI();
  virtual int appleiigs_hiresSetRow_II();

  virtual int appleiigs_loadWaveTable_BA();
  virtual int appleiigs_enableOscillators_I();
  virtual int appleiigs_setMasterVolume_I();
  virtual int appleiigs_setSoundVolume_II();
  virtual int appleiigs_setSoundFrequency_II();

private:
  void insert_hires_enable();
  void insert_hires_plot();
  void insert_hires_line();
  void insert_hires_span();
  void insert_hires_read();
  void insert_hires_sprite();
  void insert_hires_palette();
  void insert_hires_set_row();
  void insert_hires_calc_address();

  bool need_hires_enable:1;
  bool need_hires_plot:1;
  bool need_hires_line:1;
  bool need_hires_span:1;
  bool need_hires_read:1;
  bool need_hires_sprite:1;
  bool need_hires_palette:1;
  bool need_hires_set_row:1;
};

#endif

