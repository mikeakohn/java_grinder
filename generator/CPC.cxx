/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 * cpc support by SRS  - https://github.com/deringenieur71
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "CPC.h"

CPC::CPC()
{
#if 0
  start_org = 0x0170;
  ram_start = 0x0170;
  ram_end = 0xA100;
#endif
}

CPC::~CPC()
{
  //if (need_plot_lores) { add_plot_lores(); }
}

int CPC::open(const char *filename)
{
  if (Z80::open(filename) != 0) { return -1; }

  fprintf(out, "ram_start equ 0x0170\n");
  fprintf(out, "heap_ptr equ ram_start\n");
  fprintf(out, "save_iy equ heap_ptr\n");

  fprintf(out,
    "TXT_OUTPUT     equ 0xbb5a\n"
    "TXT_WR_CHAR    equ 0xbb5d\n"
    "TXT_SET_COLUMN equ 0xbb6f\n"
    "TXT_SET_ROW    equ 0xbb72\n"
    "TXT_SET_PEN    equ 0xbb90\n");

  fprintf(out,
    "GRA_PLOT_ABSOLUTE equ 0xbbea\n"
    "GRA_SET_PEN       equ 0xbb900\n");

  fprintf(out,
    "SCR_SET_MODE equ 0xbc0e\n"
    "SCR_CLEAR    equ 0xbc14\n");


  fprintf(out, "\n");
  fprintf(out, ".org 0x0170\n");

  return 0;
}

int CPC::cpc_beep()
{
  fprintf(out, "ld a, 7\n");
  fprintf(out, "call TXT_OUTPUT\n");

  return 0;
}

int CPC::cpc_color_III()
{
#if 0
  if ( foreground !=-1 )
  {
    foreground = foreground & 15;
    fprintf(out, "ld a,0x%02x\n",foreground);
    fprintf(out, "call TXT_SET_PEN\n");
  }

  if ( background !=-1 )
  {
    background = background & 15;
    fprintf(out, "ld a,0x%02x\n",background);
    fprintf(out, "call TXT_SET_PAPER\n");
  }

  if ( border != -1 )
  {
    border = border & 15;
    fprintf(out, "ld b,0x%02x\n",border);
    fprintf(out, "ld c,b\n");
    fprintf(out, "call SCR_SET_BORDER\n");
  }
#endif

  return 0;
}

int CPC::cpc_screen_I()
{
#if 0
  mode = mode & 2;
  fprintf(out, "ld a, 0x%02x\n",mode);
  fprintf(out, "call SCR_SET_MODE\n");
#endif

  return 0;
}

int CPC::cpc_cls()
{
  fprintf(out, "  ; cls()\n");
  //fprintf(out, "  xor a\n");
  //fprintf(out, "  ld hl, 0xc000\n");
  //fprintf(out, "  ld de, 0xc001\n");
  //fprintf(out, "  ld bc, 0x3fff\n");
  //fprintf(out, "  ld (hl), a\n");
  //fprintf(out, "  call LDIR\n");
  fprintf(out, "  call SCR_CLEAR\n");
  return 0;
}

int CPC::cpc_setCursor_II()
{
  fprintf(out, "  ; setCursor_II()\n");
#if 0
  if ( column==-1 )
  {
    // adjust only line position, column is untouched!
    line = line & 255;
    fprintf(out, "  ld a, 0x%02x\n",++line);
    fprintf(out, "  call TXT_SET_ROW\n");
  }
  if ( line==-1 )
  {
    // adjust onlu column position, line is untouched!
    column = column & 255;
    fprintf(out, "  ld a, 0x%02x\n",++column);
    fprintf(out, "  call TXT_SET_COLUMN\n");
  }
#endif

  return 0;
}

int CPC::cpc_putChar_C()
{
  fprintf(out, "  ; putChar_C()\n");
#if 0
  byte = byte & 255;
  fprintf(out, "  ld a, 0x%02x\n",byte);
  fprintf(out, "  call TXT_WR_CHAR\n");
#endif

  return 0;
}

int CPC::cpc_plot_III()
{
  fprintf(out, "  ; plot_III()\n");
#if 0
  x = x & 65535;
  y = y & 65535;

  foreground = foreground & 15;
  fprintf(out, "  ld a, 0x%02x\n",foreground);
  fprintf(out, "  call GRA_SET_PEN\n");
  fprintf(out, "  ld de, 0x%02x\n",x);
  fprintf(out, "  ld hl, 0x%02x\n",y);
  fprintf(out, "  call GRA_PLOT_ABSOLUTE\n");
#endif

  return 0;
}

int CPC::cpc_draw_III()
{
  fprintf(out, "  ; draw_III()\n");
#if 0
  x = x & 65535;
  y = y & 65535;

  foreground = foreground & 15;
  fprintf(out, "  ld a, 0x%02x\n",foreground);
  fprintf(out, "  call GRA_SET_PEN\n");
  fprintf(out, "  ld de, 0x%02x\n",x);
  fprintf(out, "  ld hl, 0x%02x\n",y);
  fprintf(out, "  call GRA_LINE_ABSOLUTE\n");
#endif

  return 0;
}

