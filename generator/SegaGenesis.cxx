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
    "  dc.w 0x0000              ; ROM checksum\n"
    "  dc.b \"J               \"  ; hardware used\n"
    "  dc.l 0x00000000          ; start of ROM\n"
    "  dc.l 0x003FFFFF          ; end of ROM\n"
    "  dc.l 0x00FF0000,0x00FFFFFF  ; RAM start/end\n"
    "  dc.b \"            \"      ; backup RAM info\n"
    "  dc.b \"            \"      ; modem info\n"
    "  dc.b \"                                        \" ; comment\n"
    "  dc.b \"JUE             \"  ; regions allowed\n\n");
}


