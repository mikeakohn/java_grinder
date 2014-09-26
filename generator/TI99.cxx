/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "TI99.h"

TI99::TI99() : need_vdp_command(false)
{
}

TI99::~TI99()
{
  if (need_vdp_command)
  {
    insert_vdp_command();
  }
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

int TI99::start_init()
{
  fprintf(out, "\n");
  fprintf(out, ".org 0x6000\n");

  fprintf(out, "  .db 0xaa, 0x01, 0x01, 0x00\n");
  fprintf(out, "  .dw 0x0000\n");
  fprintf(out, "  .dw _prog\n");
  fprintf(out, "  .dw 0x0000\n");
  fprintf(out, "  .dw 0x0000\n");
  fprintf(out, "  .dw 0x0000\n");
  fprintf(out, "  .dw 0x0000\n");
  fprintf(out, "_prog:\n");
  fprintf(out, "  .dw 0x0000\n");
  fprintf(out, "  .dw start\n");
  fprintf(out, "  .db %d, \"%s\"\n", strlen(app_name), app_name);

  fprintf(out, ".align 16\n\n");

  // Add any set up items (stack, registers, etc).
  fprintf(out, "start:\n");

  return 0;
}

int TI99::ti99_print()
{
  return -1;
}

int TI99::ti99_printChar()
{
  fprintf(out, "  sla r%d, 8\n", REG_STACK(reg-1));
  fprintf(out, "  mov r%d, @VDP_WRITE\n", REG_STACK(reg-1));
  reg--;

  return 0;
}

int TI99::ti99_printChar(int c)
{
  fprintf(out, "  li r0, 0x%02x\n", c << 8);
  fprintf(out, "  mov r0, @VDP_WRITE\n");

  return 0;
}

int TI99::ti99_setCursor()
{
  //mov @-24(r10), r3  ; push local_11
  //mov @-24(r10), r4  ; push local_11
  //mpy r4, r3
  //mov r4, r3

  // (REG-1 * 32) + REG-2

  // FIXME - Is this better as a function?
  fprintf(out, "  li r0, 32\n");
  fprintf(out, "  mpy r%d, r0\n", REG_STACK(reg-1));
  fprintf(out, "  a r%d, r1\n", REG_STACK(reg-2));
  fprintf(out, "  mov r1, r0\n");
  fprintf(out, "  ai r0, 0x4000\n");
  fprintf(out, "  bl @_vdp_command\n");

  reg -= 2;

  return 0;
}

int TI99::ti99_setCursor(int x, int y)
{
  need_vdp_command = true;
  int offset = (y * 32) + x;
  int address = offset + 0x4000;

  fprintf(out, "  li r0, 0x%04x   ; set write byte to %d\n", address, offset);
  fprintf(out, "  bl @_vdp_command\n");

  return 0;
}

void TI99::insert_write_string()
{
  fprintf(out, "_write_string:\n");
  fprintf(out, "  movb *r2+, r0\n");
  fprintf(out, "  jeq _write_string_exit\n");
  fprintf(out, "  mov r0, @VDP_WRITE\n");
  fprintf(out, "  jmp write_string\n");
  fprintf(out, "_write_string_exit:\n");
  fprintf(out, "  b *r11\n\n");
}

void TI99::insert_vdp_command()
{
  fprintf(out, "_vdp_command:\n");
  fprintf(out, "  swpb r0\n");
  fprintf(out, "  movb r0, @VDP_COMMAND\n");
  fprintf(out, "  swpb r0\n");
  fprintf(out, "  movb r0, @VDP_COMMAND\n");
  fprintf(out, "  b *r11\n\n");
}


