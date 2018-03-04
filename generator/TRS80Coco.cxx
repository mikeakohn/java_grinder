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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "TRS80Coco.h"

TRS80Coco::TRS80Coco() :
  need_plot_lores(0),
  need_plot_midres(0),
  need_clear_screen_lores(0),
  need_clear_screen_midres(0),
  need_print(0),
  need_set_sound(0)
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
  if (need_clear_screen_lores) { add_clear_screen_lores(); }
  if (need_clear_screen_midres) { add_clear_screen_midres(); }
  if (need_print) { add_print(); }
  if (need_set_sound) { add_set_sound(); }
}

int TRS80Coco::open(const char *filename)
{
  if (MC6809::open(filename) != 0) { return -1; }

  fprintf(out, "  _sound_waveform_start equ 0\n");
  fprintf(out, "  _sound_waveform_end equ 2\n");
  fprintf(out, "  _sound_waveform_ptr equ 4\n");

  //fprintf(out, "  ldd 0xff02\n");
  //fprintf(out, "  orb #0x06\n");
  //fprintf(out, "  std 0xff02\n");

  return 0;
}

int TRS80Coco::trs80_coco_clearScreenLores()
{
  need_clear_screen_lores = 1;

  fprintf(out, "  ; clearScreenLores()\n");
  fprintf(out, "  jsr clear_screen_lores\n");

  return 0;
}

int TRS80Coco::trs80_coco_clearScreenMidres()
{
  need_clear_screen_midres = 1;

  fprintf(out, "  ; clearScreenMidres()\n");
  fprintf(out, "  jsr clear_screen_midres\n");

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

int TRS80Coco::trs80_coco_print_X()
{
  need_print = 1;

  fprintf(out, "  ; print_X()\n");
  fprintf(out, "  puls x\n");
  fprintf(out, "  jsr _print\n");
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

int TRS80Coco::trs80_coco_initVideoFlags()
{
  // FIXME - Can this be 8 bit?  This changed the IRQ to be on the rising
  // edge.  Is this right?
  fprintf(out, "  ; initVideoFlags()\n");
  fprintf(out, "  orcc #0x10\n");
  fprintf(out, "  ldd 0xff02\n");
  fprintf(out, "  orb #0x01\n");
  fprintf(out, "  std 0xff02\n");

  return 0;
}

int TRS80Coco::trs80_coco_waitForHsync()
{
  fprintf(out, "  ; waitForHsync()\n");
  fprintf(out, "_wait_hsync_%d:\n", label_count);
  fprintf(out, "  ldd 0xff02\n");
  fprintf(out, "  bita #0x80\n");
  fprintf(out, "  beq _wait_hsync_%d\n", label_count);
  label_count++;

  // Force this to fail for now.

  return -1;
}

int TRS80Coco::trs80_coco_waitForVsync()
{
  fprintf(out, "  ; waitForVsync()\n");
  fprintf(out, "  sync\n");
  fprintf(out, "  ldd 0xff02\n");
  fprintf(out, "  andb #0x7f\n");
  fprintf(out, "  std 0xff02\n");
  label_count++;

  return 0;
}

int TRS80Coco::trs80_coco_initSound()
{
  fprintf(out, "  ;; initSound()\n");
  fprintf(out, "  ldd 0xff22\n");
  fprintf(out, "  orb #0x08\n");
  fprintf(out, "  std 0xff22\n");

  return 0;
}

int TRS80Coco::trs80_coco_setSound_aB()
{
  need_set_sound = 1;

  fprintf(out, "  ;; setSound_aB()\n");
  fprintf(out, "  puls a,b\n");
  fprintf(out, "  jsr _set_sound\n");

  return 0;
}

void TRS80Coco::add_plot_lores()
{
  // RET    0 1
  // 00 CC  2 3
  // 00 YY  4 5
  // 00 XX  6 7
  // address = ((y / 2) * 32) +  (x / 2)
  fprintf(out,
    "_plot_lores:\n"
    "  lda 5,s\n"
    "  ldb #32\n"
    "  mul\n"
    "  addd 6,s\n"
    "  addd #1024\n"
    "  tfr d,y\n"
    "  ldb 3,s\n"
    "  stb ,y\n"
    "  rts\n\n");
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
  fprintf(out,
    "_plot_midres:\n"
    "  lda 5,s\n"
    "  ldb #32\n"
    "  mul\n"
    "  addd 6,s\n"
    "  lsra\n"
    "  rorb\n"
    "  addd #1024\n"
    "  tfr d,y      ; y now holds address where to write pixel\n"
    "  lda 7,s\n"
    "  eora #0x01\n"
    "  anda #0x01\n"
    "  adda #0x01   ; a now holds 1 or 2 / if x is odd/even\n"
    "  ldb 5,s\n"
    "  bitb #0x01\n"
    "  bne _plot_midres_odd_y\n"
    "  lsla\n"
    "  lsla\n"
    "_plot_midres_odd_y:\n"
    "  ldb 3,s     ; b holds the color to plot\n"
    "  cmpb #0\n"
    "  beq _plot_midres_del_pixel\n"
    "  andb #0xf0  ; b holds the color to plot\n"
    "  sta 2,s\n"
    "  orb 2,s     ; b now holds the color and the pixel\n"
    "  lda ,y\n"
    "  anda #0x0f  ; a now holds the current 2x2 pixel value\n"
    "  sta ,y      ; This might cause a little flicker :(\n"
    "  orb ,y\n"
    "  stb ,y\n"
    "  rts\n"
    "_plot_midres_del_pixel:\n"
    "  eora #0xff  ; if color was 0, then erase the pixel\n"
    "  anda ,y\n"
    "  sta ,y\n"
    "  rts\n\n");
}

void TRS80Coco::add_clear_screen_lores()
{
  fprintf(out,
    "clear_screen_lores:\n"
    "  ldx #1024\n"
    "  ldd #0x8f8f\n"
    "_clear_screen_lores:\n"
    "  std ,x++\n"
    "  cmpx #1024+(32*16)\n"
    "  bne _clear_screen_lores\n"
    "  rts\n\n");
}

void TRS80Coco::add_clear_screen_midres()
{
  fprintf(out,
    "clear_screen_midres:\n"
    "  ldx #1024\n"
    "  ldd #0x8080\n"
    "_clear_screen_midres:\n"
    "  std ,x++\n"
    "  cmpx #1024+(32*16)\n"
    "  bne _clear_screen_midres\n"
    "  rts\n\n");
}

void TRS80Coco::add_print()
{
  fprintf(out,
    "_print:\n"
    "  lda ,x\n"
    "  tsta\n"
    "  beq _print_exit\n"
    "  jsr $a002\n"
    "  bra _print\n"
    "_print_exit:\n"
    "  rts\n");
}

void TRS80Coco::add_set_sound()
{
  fprintf(out,
    "_set_sound:\n"
    "  orcc #0x10\n"
    "  tsta\n"
    "  bne _set_sound_on\n"
    "  tstb\n"
    "  bne _set_sound_on\n"
    "  rts\n"
    "_set_sound_on:\n"
    "  std >_sound_waveform_start\n"
    "  std >_sound_waveform_ptr\n"
    "  tfr d, x\n"
    "  addd -2,x\n"
    "  std >_sound_waveform_end\n"
    "  ldd #_timer_interrupt\n"
    "  sta 0x010d\n"
    "  stb 0x010e\n"
    "  andcc #0xef\n"
    "  rts\n\n");

  fprintf(out,
    "_timer_interrupt:\n"
    "  ldx >_sound_waveform_ptr\n"
    "  lda ,x++\n"
    "  sta 0xff20\n"
    "  cmpx >_sound_waveform_end\n"
    "  bne _timer_interrupt_exit\n"
    "  ldd >_sound_waveform_start\n"
    "  std >_sound_waveform_ptr\n"
    "_timer_interrupt_exit:\n"
    "  rti\n\n");
}

