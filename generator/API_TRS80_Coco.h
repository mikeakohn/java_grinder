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
  virtual int trs80_coco_setBackgroundColor_I() { return -1; }
  virtual int trs80_coco_setBackgroundColor_I(int value) { return -1; }
};

#endif

