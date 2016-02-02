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

#ifndef _API_TRS80_COCO_H
#define _API_TRS80_COCO_H

class API_TRS80_Coco
{
public:
  virtual int trs80_coco_clearScreenLores() { return -1; }
  virtual int trs80_coco_clearScreenMidres() { return -1; }
  virtual int trs80_coco_setText_II() { return -1; }
  virtual int trs80_coco_setGraphicsMode() { return -1; }
  virtual int trs80_coco_setTextMode() { return -1; }
  virtual int trs80_coco_print_X() { return -1; }
  virtual int trs80_coco_plotLores_III() { return -1; }
  virtual int trs80_coco_plotMidres_III() { return -1; }
  virtual int trs80_coco_initVideoFlags() { return -1; }
  virtual int trs80_coco_waitForHsync() { return -1; }
  virtual int trs80_coco_waitForVsync() { return -1; }
  virtual int trs80_coco_initSound() { return -1; }
  virtual int trs80_coco_setSound_aB() { return -1; }
};

#endif

