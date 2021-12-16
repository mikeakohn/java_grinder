/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2021 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "generator/SNES.h"

SNES::SNES()
{
  start_org = 0x8000;
}

SNES::~SNES()
{
  write_interrupts();
  write_cartridge_info();
}

int SNES::open(const char *filename)
{
  if (W65816::open(filename) != 0) { return -1; }

  return 0;
}

void SNES::write_interrupts()
{
  fprintf(out,
    "cop_interrupt:\n"
    "brk_interrupt:\n"
    "abort_interrupt:\n"
    "nmi_interrupt:\n"
    "irq_interrupt:\n"
    "  rti\n\n");
}

void SNES::write_cartridge_info()
{
  fprintf(out,
    ".org 0xffc0\n"
    "  db \"SNES naken demo      \"  ; program title\n"
    "  ;  \"123456789012345678901\"\n"
    "  db 0x20                     ; 0x20=LoROM, 0x21=HiROM, Add 16 for FastROM\n"
    "  db 0x00                     ; cartridge type $00 (ROM only)\n"
    "  db 0x08                     ; rom size (08 = 2 Mbits)\n"
    "  db 0x00                     ; sram size (no sram in this cartridge)\n"
    "  db 0x01                     ; country (1 = USA)\n"
    "  db 0x00                     ; license code\n"
    "  db 0x00                     ; version (0 = 1.0)\n"
    "  dw 0x0000                   ; checksum complement (\?\?\?)\n"
    "  dw 0x0000                   ; checksum  (\?\?\?)\n"
    "  dw 0xdddd                   ; unknown\n"
    "  dw 0xeeee                   ; unknown\n"
    "  dw cop_interrupt            ; interrupts for native mode\n"
    "  dw brk_interrupt\n"
    "  dw abort_interrupt\n"
    "  dw nmi_interrupt\n"
    "  dw 0x0000                   ; unused?\n"
    "  dw irq_interrupt\n"
    "  dw 0x0000                   ; unknown\n"
    "  dw 0x0000                   ; unknown\n"
    "  dw cop_interrupt            ; interrupts for emulation mode\n"
    "  dw 0x0000                   ; unused?\n"
    "  dw abort_interrupt\n"
    "  dw nmi_interrupt\n"
    "  dw start                    ; reset vector\n"
    "  dw irq_interrupt\n\n");
}

int SNES::snes_setBackgroundColor_I()
{
  fprintf(out, "  ; snes_setBackgroundColor_I()\n");
  fprintf(out, "  sep #0x30\n");
  fprintf(out, "  lda.b #010000000b\n");
  fprintf(out, "  sta 0x2100\n");             // Disable screen
  fprintf(out, "  pla\n");
  fprintf(out, "  sta 0x2122\n");
  fprintf(out, "  xba\n");
  fprintf(out, "  sta 0x2122\n");

  // Dafuq is this?
  fprintf(out, "  lda.b #000001111b  ; End VBlank, setting brightness to 15 (100%%).\n");
  fprintf(out, "  sta 0x2100\n");             // Enable screen and set brightness
  fprintf(out, "  rep #0x30\n");

  return 0;
}

