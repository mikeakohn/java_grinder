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

/*
 *   MSX support by Giovanni Nunes - https://github.com/plainspooky
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "MSX.h"

MSX::MSX() :
  need_ldirvv(false)
{
  // MSX Memory layout:
  //   0x0000..0x3fff : MSX-BIOS (16KiB)
  //   0x4000..0xbfff : For 8, 16 or 32K cartridges (32Kib)
  //   0xc000..0xffff : RAM (16KiB) -- Don't touch in the last 4KiB!

  //start_org = 0x4000;
  //ram_start = 0xc000;
  //ram_end = 0xdfff;
}

MSX::~MSX()
{
  if (need_ldirvv) { insert_ldirvv(); }
  //if (need_plot_lores) { add_plot_lores(); }
}

int MSX::open(const char *filename)
{
  if (Z80::open(filename) != 0) { return -1; }
  fprintf(out, ".include \"msx.inc\"\n\n");
  //fprintf(out, "  _sound_waveform_start equ 0\n");
  //fprintf(out, "  _sound_waveform_end equ 2\n");
  //fprintf(out, "  _sound_waveform_ptr equ 4\n");

  return 0;
}

int MSX::start_init(void)
{
  fprintf(out, "\n");
  fprintf(out, ".org 0x4000\n");

  return 0;
}

/*
 *  just like MSX-BASIC syntax
 */
int MSX::msx_beep_I(void)
{
  fprintf(out,"call BEEP\n");
  return 0;
}

int MSX::msx_color_I(int foreground=-1,int background=-1,int border=-1)
{
  if ( foreground !=-1 )
  {
    foreground = foreground & 15;
    fprintf(out,"ld a,0x%20x\n",foreground);
    fprintf(out,"ld (FORCLR),a\n");
  }
  if ( background !=-1 )
  {
    background = background & 15;
    fprintf(out,"ld a,0x%20x\n",background);
    fprintf(out,"ld (BAKCLR),a\n");
  }
  if ( border != -1 )
  {
    border = border & 15;
    fprintf(out,"ld a,0x%20x\n",border);
    fprintf(out,"ld (BDRCLR),a\n");
  }
  fprintf(out,"call CHGCLR\n");
  return 0;
}

int MSX::msx_screen_I(int mode)
{
  mode = mode & 3;
  fprintf(out,"ld a,0x%02x\n",mode);
  fprintf(out,"call CHGMOD\n");
  // adjust text modes to maximun width
  if ( mode<2 )
  {
    if ( mode==0 ) { fprintf(out,"ld a,40\n"); }
    else { fprintf(out,"ld a,32\n"); }
    fprintf(out,"ld (LINLEN),a\n");
  }
  return 0;
}

int MSX::msx_cls_I(void)
{
  fprintf(out,"call CLS\n");
  return 0;
}

int MSX::msx_keyOn_I(void)
{
  fprintf(out,"call DSPFNK\n");
  return 0;
}

int MSX::msx_keyOff_I(void)
{
  fprintf(out,"call ERAFNK\n");
  return 0;
}

/*
 *  VRAM manipulation
 */
int MSX::msx_copyVRAM_I(int len,int source,int dest)
{
  need_ldirvv = true;
  len = len & 65535;
  source = source & 65535;
  dest = dest & 65535;
  fprintf(out,"ld bc,0x%02x\n",len);
  fprintf(out,"ld de,0x%02x\n",dest);
  fprintf(out,"ld hl,0x%02x\n",source);
  fprintf(out,"call FILVRM\n");
  fprintf(out,"call LDIRVV");
  return 0;
}

int MSX::msx_fillVRAM_I(int c,int len,int addr)
{
  c = c & 255;
  len = len & 65535;
  addr = addr & 65535;
  fprintf(out,"ld a,0x%02x\n",c);
  fprintf(out,"ld bc,0x%02x\n",len);
  fprintf(out,"ld hl,0x%02x\n",addr);
  fprintf(out,"call FILVRM\n");
  return 0;
}

/*
 *  Text console
 */
int MSX::msx_setCursor_I(int column=-1,int line=-1)
{
  if ( column==-1 )
  {
    // adjust only line position, column is untouched!
    line = line & 255;
    fprintf(out,"ld a,0x%20x\n",++line);
    fprintf(out,"ld (CSRY),a\n");
  }
  if ( line==-1 )
  {
    // adjust onlu column position, line is untouched!
    column = column & 255;
    fprintf(out,"ld a,0x%20x\n",++column);
    fprintf(out,"ld (CSRX),a\n");
  }
  return 0;
}

int MSX::msx_putChar_C(char byte)
{
  byte = byte & 255;
  fprintf(out,"ld a,0x%02x\n",byte);
  fprintf(out,"call CHPUT\n");
  return 0;
}

/*
 *  Support routines
 */
void MSX::insert_ldirvv(void)
{
  fprintf(out,"LDIRVV:\n");
  fprintf(out,"call RDVRM\n");
  fprintf(out,"ex de,hl\n");
  fprintf(out,"call WRTVRM\n");
  fprintf(out,"ex de,hl\n");
  fprintf(out,"inc de\n");
  fprintf(out,"cpi\n");
  fprintf(out,"jp PE,LDIRVV\n");
  fprintf(out,"ret\n");
}
