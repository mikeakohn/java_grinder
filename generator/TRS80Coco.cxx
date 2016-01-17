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
  // Cartridge ROM starts at 0xc0000.  RAM would start at 0x0600.
  start_org = 0xc000;
  ram_start = 0x0600;
  ram_end = 0x1fff;
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

int TRS80Coco::trs80_coco_setText_II()
{
  fprintf(out, "  ;; setText_II();\n");
  fprintf(out, "  puls y\n");
  fprintf(out, "  puls a,b\n");
  fprintf(out, "  std ,y\n");
  return 0;
}



