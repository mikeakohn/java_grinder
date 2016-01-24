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

TRS80Coco::TRS80Coco() :
  need_plot_lores(0),
  need_plot_midres(0)
{
  // Cartridge ROM starts at 0xc0000.  RAM would start at 0x0600.
  start_org = 0xc000;
  ram_start = 0x0600;
  ram_end = 0x1fff;
}

TRS80Coco::~TRS80Coco()
{
  if (need_plot_lores) { add_plot_lores(); }
  if (need_plot_midres) { add_plot_midres(); }
}

int TRS80Coco::open(const char *filename)
{
  if (MC6809::open(filename) != 0) { return -1; }

  return 0;
}

int TRS80Coco::trs80_coco_clearScreen()
{
  fprintf(out, "  ; clearScreen()\n");
  fprintf(out, "  ldx #1024\n");
  fprintf(out, "  ldd #0x8080\n");
  fprintf(out, "_clear_screen_%d:\n", label_count);
  fprintf(out, "  std ,x++\n");
  fprintf(out, "  cmpx #1024+(32*16)\n");
  fprintf(out, "  bne _clear_screen_%d\n", label_count);
  label_count++;

  return 0;
}

#if 0
int TRS80Coco::trs80_coco_setBackgroundColor_I()
{

  return -1;
}

int TRS80Coco::trs80_coco_setBackgroundColor_I(int value)
{
  fprintf(out, "  ; setBackgroundColor_I()\n");
  fprintf(out, "  puls a,b\n");
  fprintf(out, "  stb 0xffb0\n");

  return 0;
}
#endif

int TRS80Coco::trs80_coco_setText_II()
{
  fprintf(out, "  ;; setText_II();\n");
  fprintf(out, "  puls a,b\n");
  fprintf(out, "  puls y\n");
  fprintf(out, "  stb ,y\n");
  return 0;
}

int TRS80Coco::trs80_coco_setGraphicsMode()
{
  fprintf(out, "  ; setGraphicsMode()\n");
  fprintf(out, "  lda 0xff98\n");
  fprintf(out, "  ora #0x80\n");
  fprintf(out, "  sta 0xff22\n");

  return 0;
}

int TRS80Coco::trs80_coco_setTextMode()
{
  fprintf(out, "  ; setTextMode()\n");
  fprintf(out, "  lda 0xff22\n");
  fprintf(out, "  ora #0xef\n");
  fprintf(out, "  sta 0xff22\n");

  return 0;
}


int TRS80Coco::trs80_coco_plotLores_III()
{
  need_plot_lores = 1;

  fprintf(out, "  ;; plotLores_III();\n");
  fprintf(out, "  jsr _plot_lores\n");
  fprintf(out, "  leas 6,s\n");
  return 0;
}

int TRS80Coco::trs80_coco_plotMidres_III()
{
  need_plot_midres = 1;

  fprintf(out, "  ;; plotMidres_III();\n");
  fprintf(out, "  jsr _plot_midres\n");
  fprintf(out, "  leas 6,s\n");
  return 0;
}

int TRS80Coco::trs80_coco_enableVsyncListener()
{
  fprintf(out, "  ; enableVsyncListener()\n");
  fprintf(out, "  lda 0xff92\n");
  fprintf(out, "  ora #0x08\n");
  fprintf(out, "  sta 0xff92\n");

  return 0;
}

int TRS80Coco::trs80_coco_disableVsyncListener()
{
  fprintf(out, "  ; disableVsyncListener()\n");
  fprintf(out, "  lda 0xff92\n");
  fprintf(out, "  anda #0xf7\n");
  fprintf(out, "  sta 0xff92\n");

  return 0;
}

int TRS80Coco::trs80_coco_enableHsyncListener()
{
  fprintf(out, "  ; enableVsyncListener()\n");
  fprintf(out, "  lda 0xff92\n");
  fprintf(out, "  ora #0x10\n");
  fprintf(out, "  sta 0xff92\n");

  return 0;
}

int TRS80Coco::trs80_coco_disableHsyncListener()
{
  fprintf(out, "  ; disableVsyncListener()\n");
  fprintf(out, "  lda 0xff92\n");
  fprintf(out, "  anda #0xef\n");
  fprintf(out, "  sta 0xff92\n");

  return 0;
}

void TRS80Coco::add_plot_lores()
{
  // RET    0 1
  // 00 CC  2 3
  // 00 YY  4 5
  // 00 XX  6 7
  // address = ((y / 2) * 32) +  (x / 2)
  fprintf(out, "_plot_lores:\n");
  fprintf(out, "  lda 5,s\n");
  fprintf(out, "  ldb #32\n");
  fprintf(out, "  mul\n");
  fprintf(out, "  addd 6,s\n");
  fprintf(out, "  addd #1024\n");
  fprintf(out, "  tfr d,y\n");
  fprintf(out, "  ldb 3,s\n");
  fprintf(out, "  stb ,y\n");
  fprintf(out, "  rts\n\n");
}

void TRS80Coco::add_plot_midres()
{
  // RET    0 1
  // 00 CC  2 3
  // 00 YY  4 5
  // 00 XX  6 7
  // address = ((y / 2) * 32) +  (x / 2)
  // data = (color & 0xf0)
  // if (x & 1) == 0: data |= 0x01
  // if (x & 1) == 1: data |= 0x02
  // Above is equivalent to: data = (x & 1) + 1
  // if (y & 1) == 0: data <<= 4
  fprintf(out, "_plot_midres:\n");
  fprintf(out, "  lda 5,s\n");
  fprintf(out, "  ldb #32\n");
  fprintf(out, "  lsra\n");
  fprintf(out, "  lsrb\n");
  fprintf(out, "  mul\n");
  fprintf(out, "  addd 6,s\n");
  fprintf(out, "  addd #1024\n");
  fprintf(out, "  tfr d,y      ; y now holds address where to write pixel\n");
  fprintf(out, "  lda 3,s\n");
  fprintf(out, "  anda #0x01\n");
  fprintf(out, "  adda #0x01   ; a now holds 1 or 2 / if x is odd/even\n");
  fprintf(out, "  ldb 5,s\n");
  fprintf(out, "  bitb #0x01\n");
  fprintf(out, "  bne _plot_midres_odd_y\n"); 
  fprintf(out, "  lsla\n");
  fprintf(out, "  lsla\n");
  fprintf(out, "  lsla\n");
  fprintf(out, "  lsla\n");
  fprintf(out, "_plot_midres_odd_y:\n");
  fprintf(out, "  ldb 3,s     ; b holds the color to plot\n");
  fprintf(out, "  cmpb #0\n");
  fprintf(out, "  beq _plot_midres_del_pixel\n");
  fprintf(out, "  andb #0xf0  ; b holds the color to plot\n");
  fprintf(out, "  sta 3,s\n");
  fprintf(out, "  orb 3,s     ; b now holds the color and the pixel\n");
  fprintf(out, "  lda ,y\n");
  fprintf(out, "  anda #0x0f  ; a now holds the current 2x2 pixel value\n");
  fprintf(out, "  sta ,y      ; This might cause a little flicker :(\n");
  fprintf(out, "  orb ,y\n");
  fprintf(out, "  rts\n");
  fprintf(out, "_plot_midres_del_pixel:\n");
  fprintf(out, "  eora #0xff  ; if color was 0, then erase the pixel\n");
  fprintf(out, "  anda ,y\n");
  fprintf(out, "  sta ,y\n");
  fprintf(out, "  rts\n\n");
}


