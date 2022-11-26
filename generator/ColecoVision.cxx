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
#include <string.h>
#include <stdint.h>

#include "generator/ColecoVision.h"

// https://github-wiki-see.page/m/alekmaul/pvcollib/wiki/Complete-Colecovision-Specs

// Memory Map:
// 0x0000: BIOS ROM
// 0x2000: Expansion Port
// 0x4000: Expansion Port
// 0x6000: RAM (1K)
// 0x8000: Cartridge (32k)

// IO Map:
// 0xbe - VDP RAM
// 0xbf - VDP Control (write), VDP Status (read)
// 0xff - Sound (write), Controls(read)

// VDP format:
// Command is two bytes: data, control (bit 7: 1, bits 6-3: 0, bits 2-0: reg)
// VRAM Address Pointer:
// Byte 0:
//   bits 7-0: Lower 8 bits of VRAM pointer
// Byte 1:
//   bit 7: 0
//   bit 6: 0=read, 1=write
//   bits 5-0: Upper 6 bits of VRAM pointer

// Audio Format:
// Byte 0: 1VVRDDDD
//       VV: Voice number (0 to 3) where voice 3 is noise.
//        R: 0=Frequency, 1=Volume.
//     DDDD: lower 4 bits of data for volume, lower 4 bits for frequency.
// Byte 1: 00DDDDDD
//   DDDDDD: Upper 6 bits of frequency (not needed for volume).

ColecoVision::ColecoVision() :
  need_vdp_command(false),
  need_print_string(false),
  need_clear_screen(false),
  need_plot(false),
  need_set_colors(false),
  need_set_sound_freq(false),
  need_set_sound_volume(false),
  need_set_sprite_visible(false),
  need_set_sprite_image(false),
  need_set_sprite_pos(false),
  need_set_sprite_color(false)
{
}

ColecoVision::~ColecoVision()
{
}

int ColecoVision::open(const char *filename)
{
  if (Z80::open(filename) != 0) { return -1; }

  fprintf(out,
    "ram_start equ 0x6000\n"
    "heap_ptr equ ram_start\n"
    "save_iy equ heap_ptr\n"
    "SN76489 equ 0xff\n"
    "VDP_DATA equ 0xbe\n"
    "VDP_COMMAND equ 0xbf\n\n");

  return 0;
}

int ColecoVision::finish()
{
  Z80::finish();

  if (need_vdp_command) { insert_vdp_command(); }
  if (need_print_string) { insert_print_string(); }
  if (need_clear_screen) { insert_clear_screen(); }
  if (need_plot) { insert_plot(); }
  if (need_set_colors) { insert_set_colors(); }
  if (need_set_sound_freq) { insert_set_sound_freq(); }
  if (need_set_sound_volume) { insert_set_sound_volume(); }
  if (need_set_sprite_visible) { insert_set_sprite_visible(); }
  if (need_set_sprite_image) { insert_set_sprite_image(); }
  if (need_set_sprite_pos) { insert_set_sprite_pos(); }
  if (need_set_sprite_color) { insert_set_sprite_color(); }

  return 0;
}

int ColecoVision::start_init()
{
  // Catridge start info.
  fprintf(out,
    ".org 0x8000\n"
    "  .db 0xaa, 0x55\n"
    //"  .db 0x55, 0xaa\n"
    "  .dw 0\n"
    "  .dw 0\n"
    "  .dw 0\n"
    "  .dw 0\n"
    "  .dw start\n"
    "  jp 0\n"
    "  jp 0\n"
    "  jp 0\n"
    "  jp 0\n"
    "  jp 0\n"
    "  jp 0\n"
    "  jp 0\n"
    "  jp 0\n"
    "  .ascii \"JAVA/GRINDER/2022\"\n\n");

  // Add any set up items (stack, registers, etc).
  fprintf(out, "start:\n");

  // Sprite pattern table should start at 0x1800 (3 * 0x800).
  fprintf(out,
    "  ld a, 0x03\n"
    "  out (VDP_COMMAND), a\n"
    "  ld a, 0x86\n"
    "  out (VDP_COMMAND), a\n");

  return 0;
}

int ColecoVision::tms9918a_print_X()
{
  need_print_string = true;

  fprintf(out,
    "  ;; tms9918a_printChar_X()\n"
    "  pop ix\n"
    "  call _print_string\n");

  return 0;
}

int ColecoVision::tms9918a_printChar_C()
{
  fprintf(out,
    "  ;; tms9918a_printChar_C()\n"
    "  pop bc\n"
    "  out (VDP_DATA), c\n");

  return 0;
}

int ColecoVision::tms9918a_printChar_C(int c)
{
  fprintf(out,
    "  ;; tms9918a_printChar_C(%d)\n"
    "  ld a, 0x%02x\n"
    "  out (VDP_DATA), a\n",
    c,
    c);

  return 0;
}

int ColecoVision::tms9918a_setCursor_II()
{
  // FIXME - Is this better as a function?
  fprintf(out,
    "  ;; tms9918a_setCursor_II()\n"
    "  pop bc\n"
    "  sla c\n"
    "  rlc b\n"
    "  sla c\n"
    "  rlc b\n"
    "  sla c\n"
    "  rlc b\n"
    "  sla c\n"
    "  rlc b\n"
    "  sla c\n"
    "  rlc b\n"
    "  pop de\n"
    "  ld ix, 0x4000\n"
    "  add ix, bc\n"
    "  add ix, de\n"
    "  ld bc, ix\n"
    "  call _vdp_command\n");

  return 0;
}

int ColecoVision::tms9918a_setCursor_II(int x, int y)
{
  need_vdp_command = true;
  int offset = (y * 32) + x;
  int address = offset + 0x4000;

  fprintf(out,
    "  ;; tms9918a_setCursor_II(%d, %d) offset=%d\n"
    "  ld a, 0x%02x\n"
    "  out (VDP_COMMAND), a\n"
    "  ld a, 0x%02x\n"
    "  out (VDP_COMMAND), a\n",
    x, y, offset,
    address >> 8,
    address & 0xff);

  return 0;
}

int ColecoVision::tms9918a_setGraphicsMode_I()
{
  return -1;
}

int ColecoVision::tms9918a_setGraphicsMode_I(int mode)
{
  need_vdp_command = true;

  // firstbyte: reg content, second byte 0x8000 | reg num

  fprintf(out, "  ;; tms9918a_setGraphicsMode_I(%d)\n", mode);

  switch (mode)
  {
    case 0:
      fprintf(out,
        "  ld bc, 0x8000\n"
        "  call _vdp_command\n"
        "  ld bc, 0x8100|0x040\n"
        "  call _vdp_command\n");
      break;
    case 1:
      fprintf(out,
        "  ld bc, 0x8000\n"
        "  call _vdp_command\n"
        "  ld bc, 0x8110|0x40\n"
        "  call _vdp_command\n");
      break;
    case 2:
      fprintf(out,
        "  ld bc, 0x8000\n"
        "  call _vdp_command\n"
        "  ld bc, 0x8108|0x040\n"
        "  call _vdp_command\n");
      break;
    case 3:
      fprintf(out,
        "  ld bc, 0x8002\n"
        "  call _vdp_command\n"
        "  ld bc, 0x8100|0x040\n"
        "  call _vdp_command\n");
      break;
    default:
      printf("Illegal graphics mode %d\n", mode);
      return -1;
  }

  return 0;
}

int ColecoVision::tms9918a_clearScreen()
{
  need_clear_screen = true;

  fprintf(out,
    "  ;; tms9918a_clearScreen()\n"
    "  call _clear_screen\n");

  return 0;
}

int ColecoVision::tms9918a_plot_III()
{
  need_plot = true;

  fprintf(out,
    "  ;; tms9918a_plot_III()\n"
    "  pop de\n"
    "  pop bc\n"
    "  pop ix\n"
    "  call _plot\n");

  return 0;
}

int ColecoVision::tms9918a_setColors()
{
  need_set_colors = true;

  fprintf(out,
    "  ;; tms9918a_setColors()\n"
    "  call _set_colors\n");

  return 0;
}

int ColecoVision::tms9918a_setSpriteVisible_IZ()
{
  need_set_sprite_visible = true;

  fprintf(out,
    "  ;; tms9918a_setSpriteVisible_IZ()\n"
    "  pop de\n"
    "  pop bc\n"
    "  call _set_sprite_visible\n");

  return 0;
}

int ColecoVision::tms9918a_setSpriteImage_IaB()
{
  need_set_sprite_image = true;

  fprintf(out,
    "  ;; tms9918a_setSpriteImage_IaB()\n"
    "  pop de\n"
    "  pop bc\n"
    "  call _set_sprite_image\n");

  return 0;
}

int ColecoVision::tms9918a_setSpritePos_III()
{
  need_set_sprite_pos = true;

  fprintf(out,
    "  ;; tms9918a_setSpritePos_III()\n"
    "  pop hl\n"
    "  pop de\n"
    "  pop bc\n"
    "  call _set_sprite_pos\n");

  return 0;
}

int ColecoVision::tms9918a_setSpriteColor_II()
{
  need_set_sprite_color = true;

  fprintf(out,
    "  ;; tms9918a_setSpriteColor_II()\n"
    "  pop de\n"
    "  pop bc\n"
    "  call _set_sprite_color\n");

  return 0;
}

int ColecoVision::tms9918a_setSpriteSize_I()
{
  fprintf(out,
    "  ;; tms9918a_setSpriteSize_I()\n"
    "  pop af\n"
    "  or 0xe0\n"
    "  out (VDP_COMMAND), a\n"
    "  ld a, 0x81\n"
    "  out (VDP_COMMAND), a\n");

  return 0;
}

int ColecoVision::sn76489_setSoundFreq_II()
{
  need_set_sound_freq = true;

  fprintf(out,
    "  ;; sn76489_setSoundFreq_II()\n"
    "  pop de\n"
    "  pop bc\n"
    "  call _set_sound_freq\n");

  return 0;
}

int ColecoVision::sn76489_setSoundVolume_II()
{
  need_set_sound_volume = true;

  fprintf(out,
    " ;; n76489_setSoundVolume_II()\n"
    "  pop de\n"
    "  pop bc\n"
    "  call _set_sound_volume\n");

  return 0;
}

int ColecoVision::joystick_isRight_I()
{
  return -1;
}

int ColecoVision::joystick_isRight_I(int index)
{
  return -1;
}

int ColecoVision::joystick_isLeft_I()
{
  return -1;
}

int ColecoVision::joystick_isLeft_I(int index)
{
  return -1;
}

int ColecoVision::joystick_isDown_I()
{
  return -1;
}

int ColecoVision::joystick_isDown_I(int index)
{
  return -1;
}

int ColecoVision::joystick_isUp_I()
{
  return -1;
}

int ColecoVision::joystick_isUp_I(int index)
{
  return -1;
}

int ColecoVision::joystick_isButtonDown_0_I()
{
  return -1;
}

int ColecoVision::joystick_isButtonDown_0_I(int index)
{
  return -1;
}

void ColecoVision::insert_print_string()
{
  fprintf(out,
    "  ;; insert_print_string(string=ix)\n"
    "_print_string:\n"
    "  ld c, (ix-2)\n"
    "  ld b, (ix-1)\n"
    "_print_string_loop:\n"
    "  ld a, (ix)\n"
    "  out (VDP_DATA), a\n"
    "  dec c\n"
    "  jr nz, _print_string_loop\n"
    "  ret\n\n");
}

void ColecoVision::insert_vdp_command()
{
  fprintf(out,
    "_vdp_command:\n"
    "  ld a, c\n"
    "  out (VDP_COMMAND), a\n"
    "  ld a, b\n"
    "  out (VDP_COMMAND), a\n"
    "  ret\n\n");
}

void ColecoVision::insert_clear_screen()
{
  fprintf(out,
    "_clear_screen:\n"
    "  ld a, 0x00\n"
    "  out (VDP_COMMAND), a\n"
    "  ld a, 0x40\n"
    "  out (VDP_COMMAND), a\n"
    "  ld a, 0\n"
    "  ld b, 0xc0\n"
    "_clear_screen_loop:\n"
    "  out (VDP_COMMAND), a\n"
    "  out (VDP_COMMAND), a\n"
    "  out (VDP_COMMAND), a\n"
    "  out (VDP_COMMAND), a\n"
    "  dec b\n"
    "  jr nz, _clear_screen_loop\n"
    "  ret\n\n");
}

void ColecoVision::insert_plot()
{
  fprintf(out,
    "  ;; plot(x=ix, y=bc, color=de)\n"
    "_plot:\n"
    "  sla c\n"
    "  rlc b\n"
    "  sla c\n"
    "  rlc b\n"
    "  sla c\n"
    "  rlc b\n"
    "  sla c\n"
    "  rlc b\n"
    "  sla c\n"
    "  rlc b\n"
    "  add ix, bc\n"
    "  ld bc, 0x4000\n"
    "  add ix, bc\n"
    "  push ix\n"
    "  pop bc\n"
    "  ld a, c\n"
    "  out (VDP_COMMAND), a\n"
    "  ld a, b\n"
    "  out (VDP_COMMAND), a\n"
    "  ld a, e\n"
    "  out (VDP_DATA), a\n"
    "  ret\n\n");
}

void ColecoVision::insert_set_colors()
{
  // Screen image is 300 bytes long (24x32) (defaults to 0x000)
  // Color table is 32 bytes long. (defaults to 0x380)
  // Character pattern table is 2048k (256 entries * 8 bytes) defaults to 0x800)
  fprintf(out,
    "_set_colors:\n"
    "  ;; Set color table\n"
    "  ld a, 0x43\n"
    "  out (VDP_COMMAND), a\n"
    "  ld a, 0x80\n"
    "  out (VDP_COMMAND), a\n"
    "  ld a, 0\n"
    "  ld b, 32\n"
    "_set_colors_loop:\n"
    "  out (VDP_DATA), a\n"
    "  add a, 0x10\n"
    "  dec c\n"
    "  jr nz, _set_colors_loop\n"
    "  ;; Set pattern table\n"
    "  ld a, 0x48\n"
    "  out (VDP_COMMAND), a\n"
    "  ld a, 0x00\n"
    "  out (VDP_COMMAND), a\n"
    "  ld a, 0xff\n"
    "  ld ix, 2048\n"
    "_set_patterns_loop:\n"
    "  out (VDP_DATA), a\n"
    "  dec ixl\n"
    "  jr nz, _set_patterns_loop\n"
    "  ret\n\n");
}

void ColecoVision::insert_set_sound_freq()
{
  fprintf(out,
    "  ;; set_sound_freq(voice=bc, freq=de);\n"
    "_set_sound_freq:\n"
    "  ld a, c\n"
    "  sll a\n"
    "  sll a\n"
    "  sll a\n"
    "  sll a\n"
    "  sll a\n"
    "  or 0x80\n"
    "  ld c, e\n"
    "  or c\n"
    "  and 0x0f\n"
    "  out (SN76489), a\n"
    "  sra d\n"
    "  rrc e\n"
    "  sra d\n"
    "  rrc e\n"
    "  sra d\n"
    "  rrc e\n"
    "  sra d\n"
    "  rrc e\n"
    "  ld a, e\n"
    "  out (SN76489), a\n"
    "  ret\n\n");
}

void ColecoVision::insert_set_sound_volume()
{
  fprintf(out,
    "  ;; set_sound_volume(voice=bc, volume=de);\n"
    "_set_sound_volume:\n"
    "  ld a, c\n"
    "  sll a\n"
    "  sll a\n"
    "  sll a\n"
    "  sll a\n"
    "  sll a\n"
    "  or 0x90\n"
    "  or e\n"
    "  and 0x0f\n"
    "  out (SN76489), a\n"
    "  ret\n\n");
}

void ColecoVision::insert_set_sprite_visible()
{
  // Sprite attributes table is at 0x300-0x380
  fprintf(out,
    "  ;; set_sprite_visible(index=bc, visible=de)\n"
    "_set_sprite_visible:\n"
    "  sla c\n"
    "  sla c\n"
    "  ld a, c\n"
    "  out (VDP_COMMAND), a\n"
    "  ld a, 0x43\n"
    "  out (VDP_COMMAND), a\n"
    "  out (VDP_DATA), e\n"
    "  ret\n\n");
}

void ColecoVision::insert_set_sprite_image()
{
  // Sprite patterns table is at 0x3a0-0x780
  fprintf(out,
    "  ;; set_sprite_image(index=bc, image=ix)\n"
    "_set_sprite_image:\n"
    "  push bc\n"
    "  sla c\n"
    "  rlc b\n"
    "  sla c\n"
    "  rlc b\n"
    "  sla c\n"
    "  rlc b\n"
    "  sla c\n"
    "  rlc b\n"
    "  sla c\n"
    "  rlc b\n"
    "  ld iy, 0x4000|0x1800\n"
    "  add iy, bc\n"
    "  push iy\n"
    "  pop bc\n"
    "  ld a, c\n"
    "  out (VDP_COMMAND), a\n"
    "  ld a, b\n"
    "  out (VDP_COMMAND), a\n"
    "  ld e, (ix-2)\n"
    "_set_sprites_image_loop:\n"
    "  ld a, (ix)\n"
    "  out (VDP_DATA), a\n"
    "  dec e\n"
    "  jr nz, _set_sprites_image_loop\n");

  // Sprite attributes table is at 0x300
  // Need to set the pattern number for this sprite
  fprintf(out,
    "  pop de\n"
    "  sla e\n"
    "  sla e\n"
    "  ld iy, 0x4302\n"
    "  add iy, de\n"
    "  ld a, iyl\n"
    "  out (VDP_COMMAND), a\n"
    "  ld a, iyh\n"
    "  out (VDP_COMMAND), a\n"
    "  sla c\n"
    "  rlc b\n"
    "  sla c\n"
    "  rlc b\n"
    "  ld a, c\n"
    "  out (VDP_DATA), a\n"
    "  ret\n\n");
}

void ColecoVision::insert_set_sprite_pos()
{
  // Sprite attributes table is at 0x300
  fprintf(out,
    "  ;; set_sprite_pos(index=bc, x=de, y=hl)\n"
    "_set_sprite_pos:\n"
    "  ld c, a\n"
    "  sla a\n"
    "  sla a\n"
    "  out (VDP_COMMAND), a\n"
    "  ld a, 0x43\n"
    "  out (VDP_COMMAND), a\n"
    "  ld a, l\n"
    "  out (VDP_DATA), a\n"
    "  ld a, e\n"
    "  out (VDP_DATA), a\n"
    "  ret\n\n");
}

void ColecoVision::insert_set_sprite_color()
{
  fprintf(out,
    "  ;; set_sprite_color(index=bc, color=de)\n"
    "_set_sprite_color:\n"
    "  ld c, a\n"
    "  sla a\n"
    "  sla a\n"
    "  add a, 0x03\n"
    "  out (VDP_COMMAND), a\n"
    "  ld a, 0x43\n"
    "  out (VDP_COMMAND), a\n"
    "  ld a, e\n"
    "  out (VDP_DATA), a\n"
    "  ret\n\n");
}

