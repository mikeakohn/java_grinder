/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2023 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "generator/TI99.h"

// http://www.nouspikel.com/ti99/titechpages.htm

TI99::TI99() :
  need_vdp_command(false),
  need_print_string(false),
  need_clear_screen(false),
  need_plot(false),
  need_init_display(false),
  need_set_pattern(false),
  need_set_color(false),
  need_set_sound_freq(false),
  need_set_sound_volume(false),
  need_set_sprite_visible(false),
  need_set_sprite_image(false),
  need_set_sprite_pos(false),
  need_set_sprite_color(false)
{
}

TI99::~TI99()
{
}

int TI99::open(const char *filename)
{
  if (TMS9900::open(filename) != 0) { return -1; }

  fprintf(out, ".include \"ti99.inc\"\n\n");
  fprintf(out, "ram_start equ RAM\n");
  fprintf(out, "heap_ptr equ ram_start\n");

  strncpy(app_name, filename, 16);
  app_name[15] = 0;

  int n;
  for (n = 0; n < 16; n++)
  {
    if (app_name[n] == '_') { app_name[n] = ' '; }
    else if (app_name[n] >= 'a' && app_name[n] <= 'z') { app_name[n] &= 0xdf; }
    else if (app_name[n] == 0) { break; }
  }

  return 0;
}

int TI99::finish()
{
  if (need_vdp_command)        { insert_vdp_command(); }
  if (need_print_string)       { insert_print_string(); }
  if (need_clear_screen)       { insert_clear_screen(); }
  if (need_plot)               { insert_plot(); }
  if (need_init_display)       { insert_init_display(); }
  if (need_set_pattern)        { insert_set_pattern(); }
  if (need_set_color)          { insert_set_color(); }
  if (need_set_sound_freq)     { insert_set_sound_freq(); }
  if (need_set_sound_volume)   { insert_set_sound_volume(); }
  if (need_set_sprite_visible) { insert_set_sprite_visible(); }
  if (need_set_sprite_image)   { insert_set_sprite_image(); }
  if (need_set_sprite_pos)     { insert_set_sprite_pos(); }
  if (need_set_sprite_color)   { insert_set_sprite_color(); }

  return 0;
}

int TI99::start_init()
{
  fprintf(out,
    "\n"
  ".org 0x6000\n"
  "  .db 0xaa, 0x01, 0x01, 0x00\n"
  "  .dw 0x0000\n"
  "  .dw _prog\n"
  "  .dw 0x0000\n"
  "  .dw 0x0000\n"
  "  .dw 0x0000\n"
  "  .dw 0x0000\n"
  "_prog:\n"
  "  .dw 0x0000\n"
  "  .dw start\n"
  "  .db %d, \"%s\"\n", (int)strlen(app_name), app_name);

  fprintf(out, ".align 16\n\n");

  // Add any set up items (stack, registers, etc).
  fprintf(out, "start:\n");

  // Sprite pattern table should start at 0x1600
  fprintf(out,
    "  li r0, 0x0386\n"
    "  movb r0, @VDP_COMMAND\n"
    "  swpb r0\n"
    "  movb r0, @VDP_COMMAND\n");

  return 0;
}

int TI99::tms9918a_initDisplay()
{
  need_init_display = true;

  fprintf(out,
    "  ;; tms9918a_initDisplay()\n"
    "  mov r11, *r10+\n"
    "  bl @_init_display\n"
    "  ai r10, -2\n"
    "  mov *r10, r11\n");

  return 0;
}

int TI99::tms9918a_setGraphicsMode_I()
{
  return -1;
}

int TI99::tms9918a_setGraphicsMode_I(int mode)
{
  need_vdp_command = true;

  // firstbyte: reg content, second byte 0x8000 | reg num

  fprintf(out, "  ;; tms9918a_setGraphicsMode_I(%d)\n", mode);

  switch (mode)
  {
    case 0:
      fprintf(out,
        "  li r0, 0x8000\n"
        "  bl @_vdp_command\n"
        "  li r0, 0x8100|0x040\n"
        "  bl @_vdp_command\n");
      break;
    case 1:
      fprintf(out,
        "  li r0, 0x8000\n"
        "  bl @_vdp_command\n"
        "  li r0, 0x8110|0x40\n"
        "  bl @_vdp_command\n");
      break;
    case 2:
      fprintf(out,
        "  li r0, 0x8000\n"
        "  bl @_vdp_command\n"
        "  li r0, 0x8108|0x040\n"
        "  bl @_vdp_command\n");
      break;
    case 3:
      fprintf(out,
        "  li r0, 0x8002\n"
        "  bl @_vdp_command\n"
        "  li r0, 0x8100|0x040\n"
        "  bl @_vdp_command\n");
      break;
    default:
      printf("Illegal graphics mode %d\n", mode);
      return -1;
  }

  return 0;
}

int TI99::tms9918a_setPattern_IaB()
{
  need_set_pattern = true;

  fprintf(out,
    "  ;; tms9918a_setPattern_IaB()\n"
    "  mov r%d, r0\n"
    "  mov r%d, r1\n"
    "  mov r11, *r10+\n"
    "  bl @_set_pattern\n"
    "  ai r10, -2\n"
    "  mov *r10, r11\n",
    REG_STACK(reg - 2),
    REG_STACK(reg - 1));

  reg -= 2;

  return -1;
}

int TI99::tms9918a_setColor_II()
{
  need_set_color = true;

  fprintf(out,
    "  ;; tms9918a_setColor_II()\n"
    "  mov r%d, r0\n"
    "  mov r%d, r1\n"
    "  mov r11, *r10+\n"
    "  bl @_set_color\n"
    "  ai r10, -2\n"
    "  mov *r10, r11\n",
    REG_STACK(reg - 2),
    REG_STACK(reg - 1));

  reg -= 2;

  return 0;
}

int TI99::tms9918a_setTextBackdropColor_I()
{
  fprintf(out,
    "  ;; tms9918a_setTextBackdropColor_I()\n"
    "  ai r%d, 0x8700\n"
    "  swpb r%d\n"
    "  movb r%d, @VDP_COMMAND\n"
    "  swpb r%d\n"
    "  movb r%d, @VDP_COMMAND\n",
    REG_STACK(reg - 1),
    REG_STACK(reg - 1),
    REG_STACK(reg - 1),
    REG_STACK(reg - 1),
    REG_STACK(reg - 1));

  reg -= 1;

  return 0;
}

int TI99::tms9918a_clearScreen()
{
  need_clear_screen = true;

  fprintf(out,
    "  ;; tms9918a_clearScreen()\n"
    "  mov r11, *r10+\n"
    "  bl @_clear_screen\n"
    "  ai r10, -2\n"
    "  mov *r10, r11\n");

  return 0;
}

int TI99::tms9918a_print_X()
{
  need_print_string = true;

  fprintf(out,
    "  ;; tms9918a_printChar_X()\n"
    "  mov r%d, r1\n"
    "  mov r11, *r10+\n"
    "  bl @_print_string\n"
    "  ai r10, -2\n"
    "  mov *r10, r11\n",
    REG_STACK(reg - 1));

  reg--;

  return 0;
}

int TI99::tms9918a_printChar_C()
{
  fprintf(out,
    "  ;; tms9918a_printChar_C()\n"
    "  swpb r%d\n"
    "  movb r%d, @VDP_WRITE\n",
    REG_STACK(reg - 1),
    REG_STACK(reg - 1));

  reg--;

  return 0;
}

int TI99::tms9918a_printChar_C(int c)
{
  fprintf(out,
    "  ;; tms9918a_printChar_C(%d)\n"
    "  li r0, 0x%02x\n"
    "  movb r0, @VDP_WRITE\n",
    c,
    c << 8);

  return 0;
}

int TI99::tms9918a_setCursor_II()
{
  //mov @-24(r10), r3  ; push local_11
  //mov @-24(r10), r4  ; push local_11
  //mpy r4, r3
  //mov r4, r3

  // (REG-1 * 32) + REG-2

  // FIXME - Is this better as a function?
  fprintf(out,
    "  ;; tms9918a_setCursor_II()\n"
    "  li r0, 32\n"
    "  mpy r%d, r0\n"
    "  a r%d, r1\n"
    "  mov r1, r0\n"
    "  ai r0, 0x4000\n"
    "  bl @_vdp_command\n",
    REG_STACK(reg - 1),
    REG_STACK(reg - 2));

  reg -= 2;

  return 0;
}

int TI99::tms9918a_setCursor_II(int x, int y)
{
  need_vdp_command = true;
  int offset = (y * 32) + x;
  int address = offset + 0x4000;

  fprintf(out, "  ;; tms9918a_setCursor_II(%d, %d)\n", x, y);
  fprintf(out,
    "  li r0, 0x%02x%02x ; set write byte to %d\n",
    address & 0xff, address >> 8, offset);
  fprintf(out, "  movb r0, @VDP_COMMAND\n");
  fprintf(out, "  swpb r0\n");
  fprintf(out, "  movb r0, @VDP_COMMAND\n");
  //fprintf(out, "  bl @_vdp_command\n");

  return 0;
}

int TI99::tms9918a_plot_III()
{
  need_plot = true;

  fprintf(out,
    "  ;; tms9918a_plot_III()\n"
    "  mov r%d, r0\n"
    "  mov r%d, r1\n"
    "  mov r%d, r9\n"
    "  mov r11, *r10+\n"
    "  bl @_plot\n"
    "  ai r10, -2\n"
    "  mov *r10, r11\n",
    REG_STACK(reg - 3),
    REG_STACK(reg - 2),
    REG_STACK(reg - 1));

  reg -= 3;

  return 0;
}

int TI99::tms9918a_setSpriteVisible_IZ()
{
  need_set_sprite_visible = true;

  fprintf(out,
    "  ;; tms9918a_setSpriteVisible_IZ()\n"
    "  mov r%d, r0\n"
    "  mov r%d, r1\n"
    "  mov r11, *r10+\n"
    "  bl @_set_sprite_visible\n"
    "  ai r10, -2\n"
    "  mov *r10, r11\n",
    REG_STACK(reg - 2),
    REG_STACK(reg - 1));

  reg -= 2;

  return 0;
}

int TI99::tms9918a_setSpriteImage_IaB()
{
  need_set_sprite_image = true;

  if (reg < 2)
  {
    printf("Internal Error: Empty stack?\n");
    return -1;
  }

  fprintf(out,
    "  ;; tms9918a_setSpriteImage_IaB()\n"
    "  mov r%d, r0\n"
    "  mov r%d, r1\n"
    "  mov r11, *r10+\n"
    "  bl @_set_sprite_image\n"
    "  ai r10, -2\n"
    "  mov *r10, r11\n",
    REG_STACK(reg - 2),
    REG_STACK(reg - 1));

  reg -= 2;

  return 0;
}

int TI99::tms9918a_setSpritePos_III()
{
  need_set_sprite_pos = true;

  fprintf(out,
    "  ;; tms9918a_setSpritePos_III()\n"
    "  mov r%d, r0\n"
    "  mov r%d, r1\n"
    "  mov r%d, r9\n"
    "  mov r11, *r10+\n"
    "  bl @_set_sprite_pos\n"
    "  ai r10, -2\n"
    "  mov *r10, r11\n",
    REG_STACK(reg - 3),
    REG_STACK(reg - 2),
    REG_STACK(reg - 1));

  reg -= 3;

  return 0;
}

int TI99::tms9918a_setSpriteColor_II()
{
  need_set_sprite_color = true;

  fprintf(out,
    "  ;; tms9918a_setSpriteColor_II()\n"
    "  mov r%d, r0\n"
    "  mov r%d, r1\n"
    "  mov r11, *r10+\n"
    "  bl @_set_sprite_color\n"
    "  ai r10, -2\n"
    "  mov *r10, r11\n",
    REG_STACK(reg - 2),
    REG_STACK(reg - 1));

  reg -= 2;

  return 0;
}

int TI99::tms9918a_setSpriteSize_I()
{
  fprintf(out,
    "  ;; tms9918a_setSpriteSize_I()\n"
    "  sla r%d, 8\n"
    "  ori r%d, 0xe081\n"
    "  movb r%d, @VDP_COMMAND\n"
    "  swpb r%d\n"
    "  movb r%d, @VDP_COMMAND\n",
    REG_STACK(reg - 1),
    REG_STACK(reg - 1),
    REG_STACK(reg - 1),
    REG_STACK(reg - 1),
    REG_STACK(reg - 1));

  reg -= 1;

  return 0;
}

int TI99::sn76489_setSoundFreq_II()
{
  need_set_sound_freq = true;

  fprintf(out,
    "  ;; sn76489_setSoundFreq_II()\n"
    "  mov r%d, r0\n"
    "  mov r%d, r1\n"
    "  mov r11, *r10+\n"
    "  bl @_set_sound_freq\n"
    "  ai r10, -2\n"
    "  mov *r10, r11\n",
    REG_STACK(reg - 2),
    REG_STACK(reg - 1));

  reg -= 2;

  return 0;
}

int TI99::sn76489_setSoundVolume_II()
{
  need_set_sound_volume = true;

  fprintf(out,
    " ;; n76489_setSoundVolume_II()\n"
    "  mov r%d, r0\n"
    "  mov r%d, r1\n"
    "  mov r11, *r10+\n"
    "  bl @_set_sound_volume\n"
    "  ai r10, -2\n"
    "  mov *r10, r11\n",
    REG_STACK(reg - 2),
    REG_STACK(reg - 1));

  reg -= 2;

  return 0;
}

void TI99::insert_print_string()
{
  fprintf(out,
    "_print_string:\n"
    "  mov @-2(r1), r0\n"
    "_print_string_loop:\n"
    "  movb *r1+, @VDP_WRITE\n"
    "  dec r0\n"
    "  jne _print_string_loop\n"
    "  b *r11\n\n");
}

void TI99::insert_vdp_command()
{
  fprintf(out,
    "_vdp_command:\n"
    "  swpb r0\n"
    "  movb r0, @VDP_COMMAND\n"
    "  swpb r0\n"
    "  movb r0, @VDP_COMMAND\n"
    "  b *r11\n\n");
}

void TI99::insert_clear_screen()
{
  fprintf(out,
    "_clear_screen:\n"
    "  li r0, 0x0040\n"
    "  movb r0, @VDP_COMMAND\n"
    "  swpb r0\n"
    "  movb r0, @VDP_COMMAND\n"
    "  s r0, r0\n"
    "  li r1, 0x300\n"
    "_clear_screen_loop:\n"
    "  movb r0, @VDP_WRITE\n"
    "  dec r1\n"
    "  jne _clear_screen_loop\n"
    "  b *r11\n\n");
}

void TI99::insert_plot()
{
  fprintf(out,
    "  ;; plot(r0, r1, r9)\n"
    "_plot:\n"
    "  sla r1, 5\n"
    "  a r1, r0\n"
    "  ai r0, 0x4000\n"
    "  swpb r0\n"
    "  movb r0, @VDP_COMMAND\n"
    "  swpb r0\n"
    "  movb r0, @VDP_COMMAND\n"
    "  swpb r9\n"
    "  movb r9, @VDP_WRITE\n"
    "  b *r11\n\n");
}

void TI99::insert_init_display()
{
  // Screen image is 300 bytes long (24x32) (defaults to 0x000)
  // Color table is 32 bytes long. (defaults to 0x380)
  // Character pattern table is 2048k (256 entries * 8 bytes) defaults to 0x800)
  fprintf(out,
    "_init_display:\n"
    "  ;; Set color table\n"
    "  li r0, 0x8043\n"
    "  movb r0, @VDP_COMMAND\n"
    "  swpb r0\n"
    "  movb r0, @VDP_COMMAND\n"
    "  s r0, r0\n"
    "  li r1, 32\n"
    "_set_colors_loop:\n"
    "  movb r0, @VDP_WRITE\n"
    "  ai r0, 0x1000\n"
    "  dec r1\n"
    "  jne _set_colors_loop\n"
    "  ;; Set pattern table\n"
    "  li r0, 0x0048\n"
    "  movb r0, @VDP_COMMAND\n"
    "  swpb r0\n"
    "  movb r0, @VDP_COMMAND\n"
    "  li r0, 0xffff\n"
    "  li r1, 2048\n"
    "_set_patterns_loop:\n"
    "  movb r0, @VDP_WRITE\n"
    "  dec r1\n"
    "  jne _set_patterns_loop\n"
    "  b *r11\n\n");
}

void TI99::insert_set_pattern()
{
  fprintf(out,
    "_set_pattern:\n"
    "  ai r0, 0x4000|0x800\n"
    "  movb r0, @VDP_COMMAND\n"
    "  swpb r0\n"
    "  movb r0, @VDP_COMMAND\n"
    "  mov @-2(r1), r0\n"
    "_set_patterns_loop:\n"
    "  movb *r1+, @VDP_WRITE\n"
    "  dec r0\n"
    "  jne _set_patterns_loop\n"
    "  b *r11\n\n");
}

void TI99::insert_set_color()
{
  fprintf(out,
    "_set_color:\n"
    "  ai r0, 0x4000|0x380\n"
    "  movb r0, @VDP_COMMAND\n"
    "  swpb r0\n"
    "  movb r0, @VDP_COMMAND\n"
    "  movb r1, @VDP_WRITE\n"
    "  b *r11\n\n");
}

void TI99::insert_set_sound_freq()
{
  fprintf(out,
    "  ;; set_sound_freq(voice=r0, freq=r1);\n"
    "_set_sound_freq:\n"
    "  sla r0, 13\n"
    "  ori r0, 0x8000\n"
    "  soc r1, r0\n"
    "  movb r0, @0x8400\n"
    "  swpb r0\n"
    "  movb r0, @0x8400\n"
    "  b *r11\n\n");
}

void TI99::insert_set_sound_volume()
{
  fprintf(out,
    "  ;; set_sound_volume(voice=r0, volume=r1);\n"
    "_set_sound_volume:\n"
    "  sla r0, 13\n"
    "  ori r0, 0x9000\n"
    "  soc r1, r0\n"
    "  movb r0, @0x8400\n"
    "  swpb r0\n"
    "  movb r0, @0x8400\n"
    "  b *r11\n\n");
}

void TI99::insert_set_sprite_visible()
{
  // Sprite attributes table is at 0x300-0x380
  fprintf(out,
    "  ;; set_sprite_visible(index=r0, visible=r1)\n"
    "_set_sprite_visible:\n"
    "  sla r0, 2\n"
    "  sla r1, 8\n"
    "  ai r0, 0x4300\n"
    "  swpb r0\n"
    "  movb r0, @VDP_COMMAND\n"
    "  swpb r0\n"
    "  movb r0, @VDP_COMMAND\n"
    "  swpb r1\n"
    "  movb r1, @VDP_WRITE\n"
    "  b *r11\n\n");
}

void TI99::insert_set_sprite_image()
{
  // Sprite patterns table is at 0x3a0-0x780
  fprintf(out,
    "  ;; set_sprite_image(index=r0, image=r1)\n"
    "_set_sprite_image:\n"
    "  mov r0, r9\n"
    "  sla r0, 5\n"
    "  ai r0, 0x4000|0x1800\n"
    "  swpb r0\n"
    "  movb r0, @VDP_COMMAND\n"
    "  swpb r0\n"
    "  movb r0, @VDP_COMMAND\n"
    "  mov @-2(r1), r0\n"
    "_set_sprites_image_loop:\n"
    "  movb *r1+, @VDP_WRITE\n"
    "  dec r0\n"
    "  jne _set_sprites_image_loop\n");

  // Sprite attributes table is at 0x300
  // Need to set the pattern number for this sprite
  fprintf(out,
    "  mov r9, r0\n"
    "  sla r0, 2\n"
    "  ai r0, 0x4302\n"
    "  swpb r0\n"
    "  movb r0, @VDP_COMMAND\n"
    "  swpb r0\n"
    "  movb r0, @VDP_COMMAND\n"
    "  sla r9, 2\n"
    "  swpb r9\n"
    "  movb r9, @VDP_WRITE\n"
    "  b *r11\n\n");
}

void TI99::insert_set_sprite_pos()
{
  // Sprite attributes table is at 0x300
  fprintf(out,
    "  ;; set_sprite_pos(index=r0, x=r1, y=r9)\n"
    "_set_sprite_pos:\n"
    "  sla r0, 2\n"
    "  ai r0, 0x4300\n"
    "  swpb r0\n"
    "  movb r0, @VDP_COMMAND\n"
    "  swpb r0\n"
    "  movb r0, @VDP_COMMAND\n"
    "  swpb r9\n"
    "  swpb r1\n"
    "  movb r9, @VDP_WRITE\n"
    "  movb r1, @VDP_WRITE\n"
    "  b *r11\n\n");
}

void TI99::insert_set_sprite_color()
{
  // Sprite attributes table is at 0x300
  fprintf(out,
    "  ;; set_sprite_color(index=r0, color=r1)\n"
    "_set_sprite_color:\n"
    "  sla r0, 2\n"
    "  ai r0, 0x4303\n"
    "  swpb r0\n"
    "  movb r0, @VDP_COMMAND\n"
    "  swpb r0\n"
    "  movb r0, @VDP_COMMAND\n"
    "  swpb r1\n"
    "  movb r1, @VDP_WRITE\n"
    "  b *r11\n\n");
}

