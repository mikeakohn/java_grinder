/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 * Sega Genesis initialization code is based on Bruce Tomlin's hello.asm:
 * http://atariage.com/forums/topic/98540-sega-genesis-programming/
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "SegaGenesis.h"

SegaGenesis::SegaGenesis()
{
  // FIXME - What's this access prohibited crap?
  //ram_start = 0xe00000;
  ram_start = 0xff0000;
  stack_start = 0x1000000;
}

SegaGenesis::~SegaGenesis()
{
}

int SegaGenesis::open(const char *filename)
{
  if (MC68000::open(filename) != 0) { return -1; }

  fprintf(out, ".include \"genesis.h\"\n\n");

  return 0;
}

int SegaGenesis::start_init()
{
  // Add any set up items (stack, registers, etc).
  MC68000::start_init();

  // Need to initialize the Genesis hardware


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

SegaGenesis::vdp_reg_init()
{
  printf(out,
    "vdp_reg_init:\n"
    "  dc.l  0x00008000  ; d5 = VDP register 0 write command\n"
    "  dc.l  0    ; D6 = unused\n"
    "  dc.l  0x00000100  ; d7 = video register offset\n"
    "  dc.l  0    ; A0 = unused\n"
    "  dc.l  HW_version  ; a1 = hardware version register\n"
    "  dc.l  TMSS_reg    ; a2 = TMSS register\n"
    "  dc.l  VDP_data    ; a3 = VDP data\n"
    "  dc.l  VDP_ctrl    ; a4 = VDP control / status\n\n"

    "  ; VDP register initialization (24 bytes)\n"
    "vdp_reg_init_table:\n"
    "  dc.b  0x04  ; reg  0 = mode reg 1: no H interrupt\n"
    "  dc.b  0x14  ; reg  1 = mode reg 2: blanked, no V interrupt, DMA enable\n"
    "  dc.b  0x30  ; reg  2 = name table base for scroll A: 0xC000\n"
    "  dc.b  0x3c  ; reg  3 = name table base for window:   0xF000\n"
    "  dc.b  0x07  ; reg  4 = name table base for scroll B: 0xE000\n"
    "  dc.b  0x6c  ; reg  5 = sprite attribute table base: 0xD800\n"
    "  dc.b  0x00  ; reg  6 = unused register: 0x00\n"
    "  dc.b  0x00  ; reg  7 = background color: 0x00\n"
    "  dc.b  0x00  ; reg  8 = unused register: 0x00\n"
    "  dc.b  0x00  ; reg  9 = unused register: 0x00\n"
    "  dc.b  0xff  ; reg 10 = H interrupt register: 0xFF (esentially off)\n"
    "  dc.b  0x00  ; reg 11 = mode reg 3: disable ext int, full H/V scroll\n"
    "  dc.b  0x81  ; reg 12 = mode reg 4: 40 cell horiz mode, no interlace\n"
    "  dc.b  0x37  ; reg 13 = H scroll table base: 0xFC00\n"
    "  dc.b  0x00  ; reg 14 = unused register: 0x00\n"
    "  dc.b  0x01  ; reg 15 = auto increment: 0x01\n"
    "  dc.b  0x01  ; reg 16 = scroll size: V=32 cell, H=64 cell\n"
    "  dc.b  0x00  ; reg 17 = window H position: 0x00\n"
    "  dc.b  0x00  ; reg 18 = window V position: 0x00\n"
    "  dc.b  0xff  ; reg 19 = DMA length count low:   0x00FF\n"
    "  dc.b  0xff  ; reg 20 = DMA length count high:  0xFFxx\n"
    "  dc.b  0x00  ; reg 21 = DMA source address low: 0xxxxx00\n"
    "  dc.b  0x00  ; reg 22 = DMA source address mid: 0xxx00xx\n"
    "  dc.b  0x80  ; reg 23 = DMA source address high: VRAM fill, addr = 0x00xxxx\n\n"

    "  ; PSG initialization: set all channels to minimum volume\n"
    "  dc.b  0x9f,0xbf,0xdf,0xff\n\n");
}




