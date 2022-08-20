/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2022 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "generator/NES.h"

NES::NES() :
  need_set_background_palette_II(false),
  need_set_name_table_II(false),
  need_set_name_table_III(false),
  need_set_scroll_II(false)
{
  start_org = 0x8000;
  ram_start = 0x400;
}

NES::~NES()
{
  write_set_pattern();
  if (need_set_background_palette_II) { write_set_background_palette_II(); }
  if (need_set_name_table_II) { write_set_name_table_II(); }
  if (need_set_name_table_III) { write_set_name_table_III(); }
  if (need_set_scroll_II) { write_set_scroll_II(); }
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
    "  ;; nes_setBackgroundPalette_II(int index, int color)\n"
    "  jsr _set_background_palette_II\n");

  need_set_background_palette_II = true;

  return 0;
}

int NES::nes_setSpritePalette_II()
{
  fprintf(out,
    "  ;; nes_setSpritePalette_II(int index, int color)\n"
    "  inx\n"
    "  lda NES_PPU_STATUS\n"
    "  lda #0x3f\n"
    "  sta NES_PPU_ADDRESS\n"
    "  lda stack_lo + 1, x\n"
    "  sta NES_PPU_ADDRESS\n"
    "  lda stack_lo + 0, x\n"
    "  sta NES_PPU_DATA\n"
    "  inx\n");

  return 0;
}

int NES::nes_setPattern_IaB()
{
  fprintf(out,
    "  ;; nes_setPattern_IaB(int index, byte[] data)\n"
    "  jsr _set_pattern\n");

  return 0;
}

int NES::nes_setNameTable_II()
{
  fprintf(out,
    "  ;; nes_setNameTable_II(int location, int pattern_index)\n"
    "  jsr _set_name_table_II\n");

  need_set_name_table_II = true;

  return 0;
}

int NES::nes_setNameTable_III()
{
  fprintf(out,
    "  ;; nes_setNameTable_III(int x, int y, int pattern_index)\n"
    "  jsr _set_name_table_III\n");

  need_set_name_table_III = true;

  return 0;
}

int NES::nes_setScroll_II()
{
  fprintf(out,
    "  ;; nes_setScroll_II(int x, int y)\n"
    "  jsr _set_scroll_II\n");

  need_set_scroll_II = true;

  return 0;
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

void NES::write_set_pattern()
{
  // Point the PPU_ADDRESS to index * 16.
  fprintf(out,
    "  ;; nes_setPattern_IaB(int index, byte[] data)\n"
    "_set_pattern:\n"
    "  inx\n"
    "  lda NES_PPU_STATUS\n"
    "  asl stack_lo + 1, x\n"
    "  rol stack_hi + 1, x\n"
    "  asl stack_lo + 1, x\n"
    "  rol stack_hi + 1, x\n"
    "  asl stack_lo + 1, x\n"
    "  rol stack_hi + 1, x\n"
    "  asl stack_lo + 1, x\n"
    "  rol stack_hi + 1, x\n"
    "  lda stack_hi + 1, x\n"
    "  sta NES_PPU_ADDRESS\n"
    "  lda stack_lo + 1, x\n"
    "  sta NES_PPU_ADDRESS\n");

  // Point address to address of tile data.
  fprintf(out,
    "  lda stack_lo + 0, x\n"
    "  sta address + 0\n"
    "  lda stack_hi + 0, x\n"
    "  sta address + 1\n");

  // Copy pattern from RAM to the PPU RAM.
  fprintf(out,
    "  ldy #0\n"
    "_set_pattern_loop:\n"
    "  lda (address), y\n"
    "  sta NES_PPU_DATA\n"
    "  iny\n"
    "  cpy #16\n"
    "  bne _set_pattern_loop\n"
    "  inx\n"
    "  rts\n\n");
}

void NES::write_set_background_palette_II()
{
  // Point the PPU_ADDRESS to 0x3f00 + index.
  fprintf(out,
    "  ;; nes_setBackgroundPalette_II(int index, int color)\n"
    "_set_background_palette_II:\n"
    "  inx\n"
    "  lda NES_PPU_STATUS\n"
    "  lda #0x3f\n"
    "  sta NES_PPU_ADDRESS\n"
    "  lda stack_lo + 1, x\n"
    "  sta NES_PPU_ADDRESS\n"
    "  lda stack_lo + 0, x\n"
    "  sta NES_PPU_DATA\n"
    "  inx\n"
    "  rts\n\n");
}

void NES::write_set_name_table_II()
{
  // Point the PPU_ADDRESS to 0x2000 + index.
  fprintf(out,
    "  ;; nes_setNameTable_II(int index, int pattern_index)\n"
    "_set_name_table_II:\n"
    "  inx\n"
    "  lda NES_PPU_STATUS\n"
    "  lda stack_hi + 1, x\n"
    "  clc\n"
    "  adc #0x20\n"
    //"  sta stack_hi + 1, x\n"
    "  sta NES_PPU_ADDRESS\n"
    "  lda stack_lo + 1, x\n"
    "  sta NES_PPU_ADDRESS\n");

  fprintf(out,
    "  lda stack_lo + 0, x\n"
    "  sta NES_PPU_DATA\n"
    "  inx\n"
    "  rts\n\n");
}

void NES::write_set_name_table_III()
{
  // Point the PPU_ADDRESS to x + (y * 32).
  fprintf(out,
    "  ;; nes_setNameTable_III(int x, int y, int pattern_index)\n"
    "_set_name_table_III:\n"
    "  inx\n"
    "  lda NES_PPU_STATUS\n"
    "  asl stack_lo + 1, x\n"
    "  rol stack_hi + 1, x\n"
    "  asl stack_lo + 1, x\n"
    "  rol stack_hi + 1, x\n"
    "  asl stack_lo + 1, x\n"
    "  rol stack_hi + 1, x\n"
    "  asl stack_lo + 1, x\n"
    "  rol stack_hi + 1, x\n"
    "  asl stack_lo + 1, x\n"
    "  rol stack_hi + 1, x\n"
    "  clc\n"
    "  lda stack_lo + 2, x\n"
    "  adc stack_lo + 1, x\n"
    "  sta stack_lo + 1, x\n"
    "  lda stack_hi + 2, x\n"
    "  adc stack_hi + 1, x\n"
    "  adc #0x20\n"
    "  sta stack_hi + 1, x\n"
    "  sta NES_PPU_ADDRESS\n"
    "  lda stack_lo + 1, x\n"
    "  sta NES_PPU_ADDRESS\n");

  fprintf(out,
    "  lda stack_lo + 0, x\n"
    "  sta NES_PPU_DATA\n"
    "  inx\n"
    "  inx\n"
    "  rts\n\n");
}

void NES::write_set_scroll_II()
{
  // REVIEW: Is this better as inlined? Or can be optimzed as constant?
  fprintf(out,
    "  ;; nes_setScroll(int x, int y)\n"
    "_set_scroll_II:\n"
    "  inx\n"
    "  lda NES_PPU_STATUS\n"
    "  lda stack_lo + 1, x\n"
    "  sta NES_PPU_SCROLL\n"
    "  lda stack_lo + 0, x\n"
    "  sta NES_PPU_SCROLL\n"
    "  inx\n"
    "  rts\n\n");
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

