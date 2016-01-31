/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 */

#ifndef _TRS80_COCO_H
#define _TRS80_COCO_H

#include "MC6809.h"

class TRS80Coco : public MC6809
{
public:
  TRS80Coco();
  virtual ~TRS80Coco();

  virtual int open(const char *filename);
  virtual int trs80_coco_clearScreenLores();
  virtual int trs80_coco_clearScreenMidres();
  //virtual int trs80_coco_setBackgroundColor_I();
  //virtual int trs80_coco_setBackgroundColor_I(int value);
  virtual int trs80_coco_setText_II();
  virtual int trs80_coco_setGraphicsMode();
  virtual int trs80_coco_setTextMode();
  virtual int trs80_coco_print_X();
  virtual int trs80_coco_plotLores_III();
  virtual int trs80_coco_plotMidres_III();
  virtual int trs80_coco_enableVsyncListener();
  virtual int trs80_coco_disableVsyncListener();
  virtual int trs80_coco_enableHsyncListener();
  virtual int trs80_coco_disableHsyncListener();
  virtual int trs80_coco_setSound_L();

private:
  void add_plot_lores();
  void add_plot_midres();
  void add_clear_screen_lores();
  void add_clear_screen_midres();
  void add_print();

  uint32_t need_plot_lores : 1;
  uint32_t need_plot_midres : 1;
  uint32_t need_clear_screen_lores : 1;
  uint32_t need_clear_screen_midres : 1;
  uint32_t need_print : 1;
};

#endif

