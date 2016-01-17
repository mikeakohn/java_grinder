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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "TRS80Coco.h"

TRS80Coco::TRS80Coco()
{
  start_org = 0x8000;
}

TRS80Coco::~TRS80Coco()
{
}

int TRS80Coco::open(const char *filename)
{
  if (MC6809::open(filename) != 0) { return -1; }

  return 0;
}

int TRS80Coco::trs80_coco_setBackgroundColor_I()
{

  return -1;
}

int TRS80Coco::trs80_coco_setBackgroundColor_I(int value)
{

  return -1;
}

