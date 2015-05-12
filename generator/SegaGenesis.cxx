/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "SegaGenesis.h"

SegaGenesis::SegaGenesis()
{

}

SegaGenesis::~SegaGenesis()
{
}

int SegaGenesis::open(const char *filename)
{
  if (MC68000::open(filename) != 0) { return -1; }

  return 0;
}

int SegaGenesis::start_init()
{
  // Add any set up items (stack, registers, etc).
  //fprintf(out, ".org ???\n");
  fprintf(out, "start:\n");

  return 0;
}

void SegaGenesis::add_exception_vectors()
{
  fprintf(out,
    ";-------------------------------:\n"
    "; exception vectors\n"
    ";-------------------------------:\n\n"

    "  dc.l 0xfffffe00   ; startup SP\n"
    "  dc.l start        ; startup PC\n"
    "  dc.l interrupt    ; bus\n"
    "  dc.l interrupt    ; addr\n"
    "  dc.l interrupt    ; illegal\n"
    "  dc.l interrupt    ; divzero\n"
    "  dc.l interrupt    ; CHK\n"
    "  dc.l interrupt    ; TRAPV\n"
    "  dc.l interrupt    ; priv\n"
    "  dc.l interrupt    ; trace\n"
    "  dc.l interrupt    ; line 1010 emulator\n"
    "  dc.l interrupt    ; line 1111 emulator\n"
    "  dc.l interrupt\n"
    "  dc.l interrupt\n"
    "  dc.l interrupt\n"
    "  dc.l interrupt\n"
    "  dc.l interrupt\n"
    "  dc.l interrupt\n"
    "  dc.l interrupt\n"
    "  dc.l interrupt\n"
    "  dc.l interrupt\n"
    "  dc.l interrupt\n"
    "  dc.l interrupt\n"
    "  dc.l interrupt\n"
    "  dc.l interrupt    ; spurious interrupt\n"
    "  dc.l interrupt    ; interrupt level 1 (lowest priority)\n"
    "  dc.l extint       ; interrupt level 2 = external interrupt\n"
    "  dc.l interrupt    ; interrupt level 3\n"
    "  dc.l hsync        ; interrupt level 4 = H-sync interrupt\n"
    "  dc.l interrupt    ; interrupt level 5\n"
    "  dc.l vsync        ; interrupt level 6 = V-sync interrupt\n"
    "  dc.l interrupt    ; interrupt level 7 (highest priority)\n"
    "  dc.l interrupt    ; TRAP #00 exception\n"
    "  dc.l interrupt    ; TRAP #01 exception\n"
    "  dc.l interrupt    ; TRAP #02 exception\n"
    "  dc.l interrupt    ; TRAP #03 exception\n"
    "  dc.l interrupt    ; TRAP #04 exception\n"
    "  dc.l interrupt    ; TRAP #05 exception\n"
    "  dc.l interrupt    ; TRAP #06 exception\n"
    "  dc.l interrupt    ; TRAP #07 exception\n"
    "  dc.l interrupt    ; TRAP #08 exception\n"
    "  dc.l interrupt    ; TRAP #09 exception\n"
    "  dc.l interrupt    ; TRAP #10 exception\n"
    "  dc.l interrupt    ; TRAP #11 exception\n"
    "  dc.l interrupt    ; TRAP #12 exception\n"
    "  dc.l interrupt    ; TRAP #13 exception\n"
    "  dc.l interrupt    ; TRAP #14 exception\n"
    "  dc.l interrupt    ; TRAP #15 exception\n"
    "  dc.l interrupt    ; Unused (reserved)\n"
    "  dc.l interrupt    ; Unused (reserved)\n"
    "  dc.l interrupt    ; Unused (reserved)\n"
    "  dc.l interrupt    ; Unused (reserved)\n"
    "  dc.l interrupt    ; Unused (reserved)\n"
    "  dc.l interrupt    ; Unused (reserved)\n"
    "  dc.l interrupt    ; Unused (reserved)\n"
    "  dc.l interrupt    ; Unused (reserved)\n"
    "  dc.l interrupt    ; Unused (reserved)\n"
    "  dc.l interrupt    ; Unused (reserved)\n"
    "  dc.l interrupt    ; Unused (reserved)\n"
    "  dc.l interrupt    ; Unused (reserved)\n"
    "  dc.l interrupt    ; Unused (reserved)\n"
    "  dc.l interrupt    ; Unused (reserved)\n"
    "  dc.l interrupt    ; Unused (reserved)\n"
    "  dc.l interrupt    ; Unused (reserved)\n");
}

void SegaGenesis::add_cartridge_info_header()
{
  fprintf(out,
    ";-------------------------------:\n"
    "; cartridge info header\n"
    ";-------------------------------:\n\n"

    "  dc.b \"SEGA GENESIS    \"  ; must start with \"SEGA\"\n"
    "  dc.b \"(C)---- \"          ; copyright\n"
    "  dc.b \"2015.MAY\"          ; date\n"
    "  dc.b \"HELLO WORLD                                     \" ; cart name\n"
    "  dc.b \"HELLO WORLD                                     \" ; cart name (alt. language)\n"
    "  dc.b \"GM MK-0000 -00\"    ; program type / catalog number\n"
    "  dc.w 0x0000                ; ROM checksum\n"
    "  dc.b \"J               \"  ; hardware used\n"
    "  dc.l 0x00000000            ; start of ROM\n"
    "  dc.l 0x003fffff            ; end of ROM\n"
    "  dc.l 0x00ff0000,0x00ffffff ; RAM start/end\n"
    "  dc.b \"            \"      ; backup RAM info\n"
    "  dc.b \"            \"      ; modem info\n"
    "  dc.b \"                                        \" ; comment\n"
    "  dc.b \"JUE             \"  ; regions allowed\n\n");
}

void SegaGenesis::add_exception_handler()
{
  fprintf(out,
    ";-------------------------------:\n"
    "; exception handlers\n"
    ";-------------------------------:\n\n"

    "extint:\n"
    "hsync:\n"
    "vsync:\n"
    "interrupt:\n"
    "  rte\n");
}

void SegaGenesis::add_set_fonts()
{
  fprintf(out,
    "font:\n"
    "  dc.l 0x01111100, 0x11000110, 0x11000110, 0x11000110 ; A\n"
    "  dc.l 0x11111110, 0x11000110, 0x11000110, 0x00000000\n"
    "  dc.l 0x11111100, 0x11000110, 0x11000110, 0x11111100 ; B\n"
    "  dc.l 0x11000110, 0x11000110, 0x11111100, 0x00000000\n"
    "  dc.l 0x11111110, 0x11000110, 0x11000110, 0x11000000 ; C\n"
    "  dc.l 0x11000110, 0x11000110, 0x11111110, 0x00000000\n"
    "  dc.l 0x11111100, 0x11000110, 0x11000110, 0x11000110 ; D\n"
    "  dc.l 0x11000110, 0x11000110, 0x11111100, 0x00000000\n"
    "  dc.l 0x11111110, 0x11000000, 0x11000000, 0x11111100 ; E\n"
    "  dc.l 0x11000000, 0x11000000, 0x11111110, 0x00000000\n"
    "  dc.l 0x11111110, 0x11000000, 0x11000000, 0x11111100 ; F\n"
    "  dc.l 0x11000000, 0x11000000, 0x11000000, 0x00000000\n"
    "  dc.l 0x11111110, 0x11000110, 0x11000000, 0x11001110 ; G\n"
    "  dc.l 0x11000110, 0x11000110, 0x11111110, 0x00000000\n"
    "  dc.l 0x11000110, 0x11000110, 0x11000110, 0x11111110 ; H\n"
    "  dc.l 0x11000110, 0x11000110, 0x11000110, 0x00000000\n"
    "  dc.l 0x00111000, 0x00111000, 0x00111000, 0x00111000 ; I\n"
    "  dc.l 0x00111000, 0x00111000, 0x00111000, 0x00000000\n"
    "  dc.l 0x00000110, 0x00000110, 0x00000110, 0x00000110 ; J\n"
    "  dc.l 0x00000110, 0x01100110, 0x01111110, 0x00000000\n"
    "  dc.l 0x11000110, 0x11001100, 0x11111000, 0x11111000 ; K\n"
    "  dc.l 0x11001100, 0x11000110, 0x11000110, 0x00000000\n"
    "  dc.l 0x01100000, 0x01100000, 0x01100000, 0x01100000 ; L\n"
    "  dc.l 0x01100000, 0x01100000, 0x01111110, 0x00000000\n"
    "  dc.l 0x11000110, 0x11101110, 0x11111110, 0x11010110 ; M\n"
    "  dc.l 0x11000110, 0x11000110, 0x11000110, 0x00000000\n"
    "  dc.l 0x11000110, 0x11100110, 0x11110110, 0x11011110 ; N\n"
    "  dc.l 0x11001110, 0x11000110, 0x11000110, 0x00000000\n"
    "  dc.l 0x11111110, 0x11000110, 0x11000110, 0x11000110 ; O\n"
    "  dc.l 0x11000110, 0x11000110, 0x11111110, 0x00000000\n"
    "  dc.l 0x11111110, 0x11000110, 0x11000110, 0x11111110 ; P\n"
    "  dc.l 0x11000000, 0x11000000, 0x11000000, 0x00000000\n"
    "  dc.l 0x11111110, 0x11000110, 0x11000110, 0x11000110 ; Q\n"
    "  dc.l 0x11001110, 0x11001110, 0x11111110, 0x00000000\n"
    "  dc.l 0x11111110, 0x11000110, 0x11000110, 0x11111100 ; R\n"
    "  dc.l 0x11000110, 0x11000110, 0x11000110, 0x00000000\n"
    "  dc.l 0x11111110, 0x11000110, 0x11000000, 0x11111110 ; S\n"
    "  dc.l 0x00000110, 0x11000110, 0x11111110, 0x00000000\n"
    "  dc.l 0x11111110, 0x00111000, 0x00111000, 0x00111000 ; T\n"
    "  dc.l 0x00111000, 0x00111000, 0x00111000, 0x00000000\n"
    "  dc.l 0x11000110, 0x11000110, 0x11000110, 0x11000110 ; U\n"
    "  dc.l 0x11000110, 0x11000110, 0x11111110, 0x00000000\n"
    "  dc.l 0x11000110, 0x11000110, 0x11000110, 0x11000110 ; V\n"
    "  dc.l 0x01101100, 0x00111000, 0x00010000, 0x00000000\n"
    "  dc.l 0x11000110, 0x11000110, 0x11000110, 0x11010110 ; W\n"
    "  dc.l 0x11111110, 0x11101110, 0x11000110, 0x00000000\n"
    "  dc.l 0x11000110, 0x11000110, 0x11101110, 0x01111100 ; X\n"
    "  dc.l 0x11101110, 0x11000110, 0x11000110, 0x00000000\n"
    "  dc.l 0x11000110, 0x11000110, 0x11000110, 0x01101100 ; Y\n"
    "  dc.l 0x00111000, 0x00111000, 0x00111000, 0x00000000\n"
    "  dc.l 0x11111110, 0x00001110, 0x00011100, 0x00111000 ; Z\n"
    "  dc.l 0x01110000, 0x11100000, 0x11111110, 0x00000000\n"
    "  dc.l 0x00000000, 0x00000000, 0x00000000, 0x00000000 ; 7B = .\n"
    "  dc.l 0x00000000, 0x01100000, 0x01100000, 0x00000000\n"
    "  dc.l 0x02222200, 0x22000220, 0x22000000, 0x02222200 ; 7C = S\n"
    "  dc.l 0x00000220, 0x22000220, 0x02222200, 0x00000000\n"
    "  dc.l 0x02222220, 0x22000000, 0x22000000, 0x22222200 ; 7D = E\n"
    "  dc.l 0x22000000, 0x22000000, 0x02222220, 0x00000000\n"
    "  dc.l 0x02222200, 0x22000220, 0x22000000, 0x22002220 ; 7E = G\n"
    "  dc.l 0x22000220, 0x22000220, 0x02222220, 0x00000000\n"
    "  dc.l 0x00022000, 0x00222200, 0x00222200, 0x02200220 ; 7F = A\n"
    "  dc.l 0x02200220, 0x22000022, 0x22022222, 0x00000000\n"
    "fontend:\n\n");
}


