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

#ifndef TRS80_COCO_H
#define TRS80_COCO_H

#include "MC6809.h"

class TRS80Coco : public MC6809
{
public:
  TRS80Coco();
  virtual ~TRS80Coco();

  virtual int open(const char *filename);
  virtual int trs80_coco_clearScreenLores();
  virtual int trs80_coco_clearScreenMidres();
  virtual int trs80_coco_setText_II();
  virtual int trs80_coco_setGraphicsMode();
  virtual int trs80_coco_setTextMode();
  virtual int trs80_coco_print_X();
  virtual int trs80_coco_plotLores_III();
  virtual int trs80_coco_plotMidres_III();
  virtual int trs80_coco_initVideoFlags();
  virtual int trs80_coco_waitForHsync();
  virtual int trs80_coco_waitForVsync();
  virtual int trs80_coco_initSound();
  virtual int trs80_coco_setSound_aB();

private:
  void add_plot_lores();
  void add_plot_midres();
  void add_clear_screen_lores();
  void add_clear_screen_midres();
  void add_print();
  void add_set_sound();

  uint32_t need_plot_lores : 1;
  uint32_t need_plot_midres : 1;
  uint32_t need_clear_screen_lores : 1;
  uint32_t need_clear_screen_midres : 1;
  uint32_t need_print : 1;
  uint32_t need_set_sound : 1;
};

#endif

