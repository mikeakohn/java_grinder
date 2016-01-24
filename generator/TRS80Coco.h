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
  virtual int trs80_coco_clearScreen();
  //virtual int trs80_coco_setBackgroundColor_I();
  //virtual int trs80_coco_setBackgroundColor_I(int value);
  virtual int trs80_coco_setText_II();
  virtual int trs80_coco_setGraphicsMode();
  virtual int trs80_coco_setTextMode();
  virtual int trs80_coco_plot_III();
  virtual int trs80_coco_enableVsyncListener();
  virtual int trs80_coco_disableVsyncListener();
  virtual int trs80_coco_enableHsyncListener();
  virtual int trs80_coco_disableHsyncListener();

private:
  uint32_t need_plot : 1;
  void add_plot();
};

#endif

