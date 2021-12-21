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

#include "generator/NES.h"

NES::NES()
{
  start_org = 0x8000;
}

NES::~NES()
{
  write_interrupts();
  write_cartridge_info();
}

int NES::open(const char *filename)
{
  if (M6502::open(filename) != 0) { return -1; }

  fprintf(out, ".include \"nes.inc\"\n\n");

  write_init();

  return 0;
}

int NES::nes_setBackgroundPalette_II()
{
  fprintf(out,
    "  ;; nes_setBackgroundPalette(int index, int color)\n"
    "  lda NES_PPU_STATUS\n"
    "  lda #0x3f\n"
    "  sta NES_PPU_ADDRESS\n"
    "  lda stack_lo + 2, x\n"
    "  sta NES_PPU_ADDRESS\n"
    "  lda stack_lo + 1, x\n"
    "  sta NES_PPU_DATA\n"
    "  inx\n"
    "  inx\n");

  return 0;
}

int NES::nes_setSpritePalette_II()
{
  fprintf(out,
    "  ;; nes_setSpritePalette(int index, int color)\n"
    "  lda NES_PPU_STATUS\n"
    "  lda #0x3f\n"
    "  sta NES_PPU_ADDRESS\n"
    "  lda stack_lo + 2, x\n"
    "  sta NES_PPU_ADDRESS\n"
    "  lda stack_lo + 1, x\n"
    "  sta NES_PPU_DATA\n"
    "  inx\n"
    "  inx\n");

  return 0;
}

int NES::nes_setPattern_IaB()
{
  return -1;
}

int NES::nes_setNameTable_II()
{
  return -1;
}

int NES::nes_setNameTable_III()
{
  return -1;
}

void NES::write_init()
{
  fprintf(out,
    "  ;; Disable NMI, rendering, DMC IRQs\n"
    "  lda #0xff\n"
    "  sta NES_PPU_CONTROL\n"
    "  sta NES_PPU_MASK\n"
    "  sta NES_APU_MOD_CONTROL\n\n"

    ";; Wait for vertical blank.\n"
    "_wait_vblank_1:\n"
    "  bit NES_PPU_STATUS\n"
    "  bpl _wait_vblank_1\n\n"

    "  ;; Set up PPU.\n"
    "  lda #0x00\n"
    "  sta NES_PPU_CONTROL\n"
    "  lda #0x0e\n"
    "  sta NES_PPU_MASK\n\n");
}

void NES::write_interrupts()
{
  fprintf(out,
    "_vblank_interrupt:\n"
    "  rti\n\n");

  fprintf(out,
    ".org 0xfffa\n"
    "  dc16 _vblank_interrupt\n"
    ".org 0xfffc\n"
    "  dc16 reset\n\n");
}

void NES::write_cartridge_info()
{
  fprintf(out,
    ".org 0x7ff0\n"
    ".ascii \"NES\"\n"
    ".db 0x1a\n"
    "; PRG-ROM size LSB.\n"
    ".db 2\n"
    "; CHR-ROM size LSB.\n"
    ".db 0\n"
    "; Flags 6-7.\n"
    ".db 1\n"
    ".db 0\n"
    "; Mapper MSB/submapper.\n"
    ".db 0\n"
    "; PRG-ROM/CHR-ROM size MSB (really separate nybbles).\n"
    ".db 0x00\n"
    "; PRG-RAM/EEPROM size.\n"
    ".db 0\n"
    "; PRG-RAM size.\n"
    ".ascii \"NI2.1\"\n\n");
}

