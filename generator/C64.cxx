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

#include "C64.h"

// ABI is:
// A
// X
// Y

#define BEGIN_SUB \
  fprintf(out, "  pla\n"); \
  fprintf(out, "  sta return + 1\n"); \
  fprintf(out, "  pla\n"); \
  fprintf(out, "  sta return + 0\n")

#define END_SUB \
  fprintf(out, "  lda return + 0\n"); \
  fprintf(out, "  pha\n"); \
  fprintf(out, "  lda return + 1\n"); \
  fprintf(out, "  pha\n"); \
  fprintf(out, "  rts\n\n")

#define PUSH_LO \
  fprintf(out, "; PUSH_LO\n"); \
  fprintf(out, "  ldy SP\n"); \
  fprintf(out, "  sta stack_lo,y\n"); \

#define PUSH_HI \
  fprintf(out, "; PUSH_HI\n"); \
  fprintf(out, "  ldy SP\n"); \
  fprintf(out, "  sta stack_hi,y\n"); \
  fprintf(out, "  dec SP\n")

#define POP_HI \
  fprintf(out, "; POP_HI\n"); \
  fprintf(out, "  inc SP\n"); \
  fprintf(out, "  ldy SP\n"); \
  fprintf(out, "  lda stack_hi,y\n")

#define POP_LO \
  fprintf(out, "; POP_LO\n"); \
  fprintf(out, "  ldy SP\n"); \
  fprintf(out, "  lda stack_lo,y\n")

#define POKE(dst) \
  POP_HI; \
  POP_LO; \
  fprintf(out, "  sta 0x%04x\n", dst)

C64::C64()
{

}

C64::~C64()
{
}

int C64::open(char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".65xx\n");
  // heap
  fprintf(out, "ram_start equ 0x8000\n");
  fprintf(out, "heap_ptr equ ram_start\n");
  // for indirection (2 bytes)
  fprintf(out, "address equ 0xfb\n");
  // java stack
  fprintf(out, "stack_lo equ 0xc000\n");
  fprintf(out, "stack_hi equ 0xc100\n");
  fprintf(out, "SP equ 0xfd\n");
  // points to locals
  fprintf(out, "locals equ 0xfe\n");
  // temp variables
  fprintf(out, "result equ 0x20\n");
  fprintf(out, "return equ 0x22\n");
  fprintf(out, "remainder equ 0x24\n");
  fprintf(out, "length equ 0x26\n");
  fprintf(out, "temp equ 0x28\n");
  fprintf(out, "value1 equ 0x2a\n");
  fprintf(out, "value2 equ 0x2c\n");
  fprintf(out, "value3 equ 0x2e\n");
  fprintf(out, "value4 equ 0x30\n");
  // sprites
  fprintf(out, "sprite_msb_set equ 0x10\n");
  fprintf(out, "sprite_msb_clear equ 0x11\n");
  fprintf(out, "sprite_x equ 0x12\n");
  fprintf(out, "sprite_y equ 0x13\n");

  // basic loader
  fprintf(out, ".org 0x07ff\n\n");

  fprintf(out, "dw 0x0801\n");
  fprintf(out, "dw start\n");
  fprintf(out, "dw 2013\n");
  fprintf(out, "db 0x9e\n");
  fprintf(out, "db (((start / 1000) %% 10) + 0x30)\n");
  fprintf(out, "db (((start / 100) %% 10) + 0x30)\n");
  fprintf(out, "db (((start / 10) %% 10) + 0x30)\n");
  fprintf(out, "db (((start / 1) %% 10) + 0x30)\n");
  fprintf(out, "db ':'\n");
  fprintf(out, "db 0x8f\n");
  fprintf(out, "db \" MORTIS\"\n");
  fprintf(out, "db 0\n");
  fprintf(out, "dw 0\n\n");

  fprintf(out, "start:\n");
  fprintf(out, "  sei\n");
  fprintf(out, "  cld\n");
  fprintf(out, "  lda #0xff\n");
  fprintf(out, "  sta SP\n\n");

  return 0;
}

// C64 API
int C64::c64_vic_border(/* color */) { POKE(0xd020); return 0; }

int C64::c64_vic_background(/* color */) { POKE(0xd021); return 0; }

int C64::c64_vic_sprite_enable(/* num */ ) { POKE(0xd015); return 0; }

// slow but convenient :)
int C64::c64_vic_sprite_pos(/* sprite, x, y */ )
{
  fprintf(out, "; c64_vic_sprite_pos\n");
  // y
  POP_HI;
  POP_LO;
  fprintf(out, "  sta sprite_y\n");

  // x
  POP_HI;
  fprintf(out, "  sta sprite_x + 1\n");
  POP_LO;
  fprintf(out, "  sta sprite_x + 0\n");

  // sprite
  POP_HI;
  POP_LO;
  fprintf(out, "  tay\n");
  fprintf(out, "  sta sprite_msb_set\n");
  fprintf(out, "  tax\n");
  fprintf(out, "  beq #6\n");
  fprintf(out, "  asl sprite_msb_set\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  bne #-8\n");

  // vic sprite position offset (sprite * 2)
  fprintf(out, "  tya\n");
  fprintf(out, "  asl\n");
  fprintf(out, "  tay\n");

  // set xy pos
  fprintf(out, "  lda sprite_msb_set\n");
  fprintf(out, "  eor #0xff\n");
  fprintf(out, "  sta sprite_msb_clear\n");

  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and sprite_msb_clear\n");
  fprintf(out, "  sta 0xd010\n");
  fprintf(out, "  lda sprite_x + 1\n");
  fprintf(out, "  and sprite_msb_set\n");
  fprintf(out, "  ora 0xd010\n");
  fprintf(out, "  sta 0xd010\n");

  fprintf(out, "  lda sprite_x + 0\n");
  fprintf(out, "  sta 0xd000,y\n");

  fprintf(out, "  lda sprite_y\n");
  fprintf(out, "  sta 0xd001,y\n");
  
  return 0;
}

int C64::c64_vic_poke(/* dest, value */ )
{
  // value
  fprintf(out, "; c64_vic_poke\n");
  POP_HI;
  POP_LO;
  fprintf(out, "  tax\n");

  // reg
  POP_HI;
  fprintf(out, "  sta address + 1\n");
  POP_LO;
  fprintf(out, "  sta address + 0\n");

  fprintf(out, "  txa\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  sta (address),y\n");

  return 0;
}

#if 0
void C64::close()
{
}
#endif

