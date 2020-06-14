/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2020 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "generator/SegaGenesis.h"

// Notes:
// Screen size: 32x28 cells (or 256 x 224)
// Scroll A is set to 64x32 = 64 * 32 * 2 bytes = 4096 bytes (0x1000)
// Pattern Table: 32 * 28 * (8 * 8 / 2) = 28672 bytes (0x7000)
// Font table: 32 * 27 = 864 bytes (0x360).  Load at 0x7000 (pattern #896)

// Screen size: 40 cells (or 320 x 224)
// Scroll A is set to 64x32 = 64 * 32 * 2 bytes = 4096 bytes (0x1000)
// Pattern Table: 40 * 28 * (8 * 8 / 2) = 35840 bytes (0x8c00)
// Font table: 32 * 27 = 864 bytes (0x360).  Load at 0x8c00 (pattern #1120)

// FIXME - This is redundant
#define REG_STACK(a) (a)

#define CD_VRAM_WRITE 1
#define CD_CRAM_WRITE 3
#define CD_VSRAM_WRITE 5
#define CD_VRAM_READ 0
#define CD_CRRAM_READ 8
#define CD_VSRAM_READ 4

#define CTRL_REG(cd, a) \
  (((cd & 0x3) << 30) | \
  ((a & 0x3fff) << 16) | \
  ((cd & 0x3c) << 2) | \
  (a >> 14))

// Functions that return void should never use the stack for registers.. ?
#define CHECK_STACK \
  if (stack != 0) \
  { \
    printf("Internal Error: %s:%d\n", __FILE__, __LINE__); \
    return -1; \
  }

// a0 = VDP_DATA
// a1 = VDP_control
// a2 = temp
// a3 = temp

SegaGenesis::SegaGenesis() :
  sprite_attribute_table(0xd800),
  need_print_string(false),
  need_load_fonts(false),
  need_clear_text(false),
  need_load_z80(false),
  need_set_pattern_table(false),
  need_set_image_data(false),
  need_set_palette_colors(false),
  need_set_palette_colors_at_index(false),
  need_init_bitmap(false),
  need_clear_bitmap(false),
  need_clear_pattern(false),
  need_plot(false),
  need_set_plot_address(false)
{
  // FIXME - What's this access prohibited crap?
  //ram_start = 0xe00000;
  ram_start = 0xff0000;
  stack_start = 0x00000000;
  //stack_start = 0xfffffe00;  // <-- dafuq?
}

SegaGenesis::~SegaGenesis()
{
}

int SegaGenesis::open(const char *filename)
{
  if (MC68000::open(filename) != 0) { return -1; }

  //fprintf(out, ".include \"genesis.h\"\n\n");
  fprintf(out,
    "  TMSS_REG equ 0xa14000\n"
    "  HW_VERSION equ 0xa10001\n"
    "  VDP_DATA equ 0xc00000\n"
    "  VDP_CTRL equ 0xc00004\n"
    "  Z80_RAM equ 0xa00000\n"
    "  Z80_BUSREQ equ 0xa11100\n"
    "  Z80_RESET equ 0xa11200\n\n");

  add_exception_vectors();
  add_cartridge_info_header();
  add_exception_handler();

  return 0;
}

int SegaGenesis::finish()
{
  add_vdp_reg_init();

  if (need_print_string) { add_print_string(); }
  if (need_load_fonts) { add_load_fonts(); }
  if (need_clear_text) { add_clear_text(); }
  if (need_load_z80) { add_load_z80(); }
  if (need_set_pattern_table) { add_set_pattern_table(); }
  if (need_set_image_data) { add_set_image_data(); }
  if (need_set_palette_colors) { add_set_palette_colors(); }
  if (need_set_palette_colors_at_index) { add_set_palette_colors_at_index(); }
  if (need_init_bitmap) { add_init_bitmap(); }
  if (need_clear_bitmap) { add_clear_bitmap(); }
  if (need_clear_pattern) { add_clear_pattern(); }
  if (need_plot) { add_plot(); }
  if (need_set_plot_address) { add_set_plot_address(); }

  return 0;
}

int SegaGenesis::start_init()
{
  // Add any set up items (stack, registers, etc).
  MC68000::start_init();

  // Setup registers used to talk to VDP
  fprintf(out,
    "  ; Setup registers used to talk to VDP\n"
    "  movea.l #VDP_DATA, a0\n"
    "  movea.l #VDP_CTRL, a1\n\n");

  fprintf(out,
    "  ; During initialization:\n"
    "  ; d0 = 0\n"
    "  ; d1 = data movement\n"
    "  ; d2 = counter\n"
    "  eor.l d0, d0\n\n");

  // Initialize TMSS
  fprintf(out,
    "  ; Initialize TMSS\n"
    "  movea.l #HW_VERSION, a2\n"
    "  movea.l #TMSS_REG, a3\n"
    "  move.b (a2), d1           ; A10001 test the hardware version\n"
    "  andi.b #0x0f, d1\n"
    "  beq.b start_init_tmss     ; branch if no TMSS\n"
    "  move.l #0x53454741, (a3)  ; A14000 disable TMSS\n"
    "start_init_tmss:\n"
    "  move.w (a1), d1    ; C00004 read VDP status (interrupt acknowledge?)\n\n");

  // Video initialization
  fprintf(out,
    "  ; Initialize video\n"
    "  movea.l #vdp_reg_init_table, a2\n"
    "  move.w #0x8000, d1\n"
    "  moveq #24-1, d2   ; length of video initialization block\n"
    "start_video_init:\n"
    "  move.b (a2)+, d1  ; get next video control byte\n"
    "  move.w d1, (a1)   ; C00004 send write register command to VDP\n"
    "  add.w #0x100, d1  ; point to next VDP register\n"
    "  dbra d2, start_video_init  ; loop for rest of block\n\n");

  // Use DMA to clear VRAM and wait on busy VDP
  fprintf(out,
    "  ; DMA is now set up for 65535-byte fill of VRAM\n"
    "  move.l #0x40000080, (a1)  ; C00004 = VRAM write to 0x0000\n"
    "  move.w d0, (a0)      ; C00000 = write zero to VRAM (starts DMA fill)\n"
    "  ; Wait on busy VDP\n"
    "start_wait_dma:\n"
    "  move.w (a1), d1      ; C00004 read VDP status\n"
    "  btst #1, d1        ; test DMA busy flag\n"
    "  bne.s start_wait_dma ; loop while DMA busy\n\n");

  // Initalize CRAM
  fprintf(out,
    "  ; initialize CRAM\n"
    "  move.l #0x81048f02, (a1) ; C00004 reg 1 = 0x04, reg 15 = 0x02: blank, auto-increment=2\n"
    "  move.l #0xc0000000, (a1) ; C00004 write CRAM address 0x0000\n"
    "  moveq #32-1, d2          ; loop for 32 CRAM registers\n"
    "start_init_cram:\n"
    "  move.l d0, (a0)          ; C00000 clear CRAM register\n"
    "  dbra d2, start_init_cram\n\n");

  // Initalize VSRAM
  fprintf(out,
    "  ; Initialize VSRAM\n"
    "  move.l #0x40000010, (a1) ; C00004 VSRAM write address 0x0000\n"
    "  moveq #20-1, d2          ; loop for 20 VSRAM registers\n"
    "start_init_vsram:\n"
    "  move.l d0, (a0)          ; C00000 clear VSRAM register\n"
    "  dbra d2, start_init_vsram\n\n");

  // Initialize PSG
  fprintf(out,
    "  ; Initialize PSG\n"
    "  moveq #4-1, d2             ; loop for 4 PSG registers\n"
    "  movea.l #psg_reg_init_table, a2\n"
    "start_init_psg:\n"
    "  move.b (a2)+, (0x0011, a0) ; C00011 copy PSG initialization commands\n"
    "  dbra d2, start_init_psg\n\n");

  // Unblank display
  fprintf(out,
    "  ; Unblank display\n"
    "  move.w #0x8144, (a1)   ; C00004 reg 1 = 0x44 unblank display\n\n");

  return 0;
}

int SegaGenesis::sega_genesis_setPalettePointer_I()
{
  int d;

  if (stack > 0)
  {
    d = 5;
    fprintf(out, "  move.l (SP)+, d5\n");
    stack--;
  }
    else
  {
    d = REG_STACK(reg-1);
    reg--;
  }

  //fprintf(out, "  lsl.l #8, d%d\n", d);
  //fprintf(out, "  lsl.l #8, d%d\n", d);
  fprintf(out, "  // setPalettePointer()\n");
  fprintf(out, "  lsl.l #1, d%d\n", d);
  fprintf(out, "  swap d%d\n", d);
  fprintf(out, "  or.l #0xc0000000, d%d\n", d);
  fprintf(out, "  move.l d%d, (a1) ; Set CRAM write address\n", d);

  return 0;
}

int SegaGenesis::sega_genesis_setPalettePointer_I(int index)
{
  if (index < 0 || index > 63)
  {
    printf("Palette index %d out of range 0-63\n", index);
    return -1;
  }

  fprintf(out, "  // setPalettePointer(%d)\n", index);
  fprintf(out, "  move.l #0xc0%02x0000, (a1) ; Set CRAM write address\n", index * 2);

  return 0;
}

int SegaGenesis::sega_genesis_setPaletteColor_I()
{
  fprintf(out, "  move.w %s, (a0)\n", pop_reg());

  return 0;
}

int SegaGenesis::sega_genesis_setPaletteColor_I(int color)
{
  if (color < 0 || color > 4095)
  {
    printf("Color %d is out of 12 bit range\n", color);
    return -1;
  }

  fprintf(out, "  move.w #0x%03x, (a0)      ; setPaletteColor()\n", color);

  return 0;
}

int SegaGenesis::sega_genesis_initBitmap()
{
  need_init_bitmap = true;

  fprintf(out, "  jsr (_init_bitmap).l\n");
  return 0;
}

int SegaGenesis::sega_genesis_clearBitmap()
{
  need_clear_bitmap = true;

  fprintf(out, "  jsr (_clear_bitmap).l\n");
  return 0;
}

int SegaGenesis::sega_genesis_clearPatterns_I()
{
  need_clear_pattern = true;

  if (reg != 1)
  {
     fprintf(out, "  push.l d0\n");
     fprintf(out, "  move.l d%d, d0\n", REG_STACK(reg - 1));
  }

  fprintf(out, "  jsr (_clear_pattern).l\n");

  if (reg != 1)
  {
     fprintf(out, "  pop.l d0\n");
  }

  reg--;

  return 0;
}

int SegaGenesis::sega_genesis_plot_III()
{
  need_plot = true;

  if (reg != 3)
  {
    fprintf(out, "  push.l d0\n");
    fprintf(out, "  push.l d1\n");
    fprintf(out, "  push.l d2\n");

    fprintf(out, "  move.l d%d, d0\n", REG_STACK(reg - 3));
    fprintf(out, "  move.l d%d, d1\n", REG_STACK(reg - 2));
    fprintf(out, "  move.l d%d, d2\n", REG_STACK(reg - 1));
  }

  fprintf(out, "  jsr (_plot).l\n");

  if (reg != 3)
  {
    fprintf(out, "  pop.l d2\n");
    fprintf(out, "  pop.l d1\n");
    fprintf(out, "  pop.l d0\n");
  }

  reg -= 3;

  return 0;
}

int SegaGenesis::sega_genesis_setPlotAddress_I()
{
  need_set_plot_address = true;

  fprintf(out, "  move.l %s, d5\n", pop_reg());
  fprintf(out, "  jsr (_set_plot_address).l\n");

  return 0;
}

int SegaGenesis::sega_genesis_fastPlot_IIII()
{
  // FIXME
  CHECK_STACK

  fprintf(out, "  lsl.w #4, d%d\n", REG_STACK(reg-2));
  fprintf(out, "  lsl.w #8, d%d\n", REG_STACK(reg-3));
  fprintf(out, "  lsl.w #8, d%d\n", REG_STACK(reg-4));
  fprintf(out, "  lsl.w #4, d%d\n", REG_STACK(reg-4));
  fprintf(out, "  or.w d%d, d%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
  fprintf(out, "  or.w d%d, d%d\n", REG_STACK(reg-3), REG_STACK(reg-1));
  fprintf(out, "  or.w d%d, d%d\n", REG_STACK(reg-4), REG_STACK(reg-1));
  fprintf(out, "  move.w d%d, (a0)\n", REG_STACK(reg-1));

  reg -= 4;

  return 0;
}

int SegaGenesis::sega_genesis_inVerticalBlank()
{
  const char *r = push_reg();

  // NOTE: Documentation seems to have the flags wrong.
  fprintf(out,
    "  move.w (a1), %s      ; C00004 read VDP status\n"
    "  and.l #8, %s         ; test vertical blank flag\n", r, r);

  return 0;
}

int SegaGenesis::sega_genesis_waitVerticalBlank()
{
  // NOTE: Documentation seems to have the flags wrong.
  fprintf(out,
    "  ; Wait for vertical blank\n"
    "_wait_vertical_blank_%d:\n"
    "  move.w (a1), d7\n"
    "  btst #3, d7      ; test vertical blank flag\n"
    "  beq.s _wait_vertical_blank_%d\n",
    label_count, label_count);

  label_count++;

  return 0;
}

int SegaGenesis::sega_genesis_waitHorizontalBlank()
{
  // NOTE: Documentation seems to have the flags wrong.
  fprintf(out,
    "  ; Wait for horizontal blank\n"
    "_wait_horizontal_blank_%d:\n"
    "  move.w (a1), d7\n"
    "  btst #2, d7      ; test horizontal blank flag\n"
    "  beq.s _wait_horizontal_blank_%d\n",
    label_count, label_count);

  label_count++;

  return 0;
}

int SegaGenesis::sega_genesis_loadFonts()
{
  need_load_fonts = true;

  fprintf(out, "  jsr (_load_fonts).l\n");
  return 0;
}

int SegaGenesis::sega_genesis_clearText()
{
  need_clear_text = true;

  fprintf(out, "  jsr (_clear_text).l\n");
  return 0;
}

int SegaGenesis::sega_genesis_setCursor_II()
{
  // FIXME - Holy crap.  This is fugly.  Should this just be a function?
  fprintf(out, "  ; setCursor() - Set cursor position in VDP\n");
  //fprintf(out, "  mulu.w #128, d%d\n", REG_STACK(reg-1));
  fprintf(out, "  lsl.w #7, d%d\n", REG_STACK(reg-1));
  fprintf(out, "  lsl.w #1, d%d\n", REG_STACK(reg-2));
  fprintf(out, "  add.w d%d, d%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
  fprintf(out, "  swap d%d\n", REG_STACK(reg-1));
  fprintf(out, "  or.l #0x%08x, d%d\n", CTRL_REG(CD_VRAM_WRITE, 0xe000), REG_STACK(reg-1));
  fprintf(out, "  move.l d%d, (a1)\n", REG_STACK(reg-1));

  reg -= 2;

  return 0;
}

int SegaGenesis::sega_genesis_setCursor_II(int x, int y)
{
  // FIXME - This function isn't getting called when it should.
  // 0100 0101 1001 0100 0000 0000 0000 0011
  // CD = 000001 (VRAM WRITE)
  //  A = 1100 0101 1001 0100 = 0xc594
  int address = (0xe000 + (y * 128 + x * 2));

  fprintf(out,
    "  move.l #0x%8x, (a1) ; setCursor(const,const) Set cursor position in VDP\n",
    CTRL_REG(CD_VRAM_WRITE, address));

  return 0;
}

int SegaGenesis::sega_genesis_printChar_C()
{
  fprintf(out, "  add.w #(1120 - 'A'), %s; printChar()\n", pop_reg());
  fprintf(out, "  or.w #0x6000, %s\n", pop_reg());
  fprintf(out, "  move.w %s, (a0)\n", pop_reg());

  return 0;
}

int SegaGenesis::sega_genesis_printChar_C(int c)
{
  int pattern = 0x6000 | ((c - 'A') + 1120);
  fprintf(out, "  move.w #0x%02x, (a0) ; printChar(0x%02x)\n", pattern, c);

  return 0;
}

int SegaGenesis::sega_genesis_print_X()
{
  need_print_string = true;

  fprintf(out, "  movea.l %s, a2\n", pop_reg());
  fprintf(out, "  jsr (_print_string).l\n");

  return 0;
}

int SegaGenesis::sega_genesis_setHorizontalScrollA_II()
{
  fprintf(out,
    "  ;; Update horizontal scroll A (text)\n"
    "  lsl.l #2, d%d\n"
    "  swap d%d\n"
    "  move.l #0x%8x, d7\n"
    "  add.l d%d, d7\n"
    "  move.l d7, (a1)\n"
    "  move.w d%d, (a0)\n",
    REG_STACK(reg-2),
    REG_STACK(reg-2),
    CTRL_REG(CD_VRAM_WRITE, 0xfc00),
    REG_STACK(reg-2),
    REG_STACK(reg-1));

  reg -= 2;

  return 0;
}

int SegaGenesis::sega_genesis_setHorizontalScrollB_II()
{
  fprintf(out,
    "  ;; Update horizontal scroll B (bitmap)\n"
    "  lsl.l #2, d%d\n"
    "  swap d%d\n"
    "  move.l #0x%8x, d7\n"
    "  add.l d%d, d7\n"
    "  move.l d7, (a1)\n"
    "  move.w d%d, (a0)\n",
    REG_STACK(reg-2),
    REG_STACK(reg-2),
    CTRL_REG(CD_VRAM_WRITE, 0xfc02),
    REG_STACK(reg-2),
    REG_STACK(reg-1));

  reg -= 2;

  return 0;
}

int SegaGenesis::sega_genesis_setVerticalScrollA_I()
{
  fprintf(out,
    "  ;; Update vertical scroll A\n"
    "  move.l #0x%8x, (a1)\n"
    "  move.w d%d, (a0)\n",
    CTRL_REG(CD_VSRAM_WRITE, 0x0000),
    REG_STACK(reg-1));

  reg--;

  return 0;
}

int SegaGenesis::sega_genesis_setVerticalScrollB_I()
{
  fprintf(out,
    "  ;; Update vertical scroll B\n"
    "  move.l #0x%8x, (a1)\n"
    "  move.w d%d, (a0)\n",
    CTRL_REG(CD_VSRAM_WRITE, 0x0002),
    REG_STACK(reg-1));

  reg--;

  return 0;
}

int SegaGenesis::sega_genesis_setHorizontalScrollModeLine()
{
  fprintf(out,
    "  ;; Swith to line by line horizontal scroll mode\n"
    "  move.w #0x8b03, (a1)\n");

  return 0;
}

int SegaGenesis::sega_genesis_setHorizontalScrollModeFull()
{
  fprintf(out,
    "  ;; Swith to full screen horizontal scroll mode\n"
    "  move.w #0x8b00, (a1)\n");

  return 0;
}


int SegaGenesis::sega_genesis_setPatternTable_aI()
{
  need_set_pattern_table = true;

  fprintf(out, "  movea.l %s, a3\n", pop_reg());
  fprintf(out, "  moveq.l #0, d7\n");
  fprintf(out, "  jsr (_set_pattern_table).l\n");

  return 0;
}

int SegaGenesis::sega_genesis_setPatternTableAtIndex_IaI()
{
  need_set_pattern_table = true;

  fprintf(out, "  movea.l %s, a3\n", pop_reg());
  fprintf(out, "  move.l %s, d7\n", pop_reg());
  fprintf(out, "  jsr (_set_pattern_table).l\n");

  return 0;
}

int SegaGenesis::sega_genesis_setPatternLocation_II()
{
  fprintf(out, "  ; setPatternLocation()\n");
  fprintf(out, "  lsl.w #7, d%d\n", REG_STACK(reg-1));
  fprintf(out, "  lsl.w #1, d%d\n", REG_STACK(reg-2));
  fprintf(out, "  add.w d%d, d%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
  fprintf(out, "  swap d%d\n", REG_STACK(reg-1));
  fprintf(out, "  or.l #0x%08x, d%d\n", CTRL_REG(CD_VRAM_WRITE, 0xc000), REG_STACK(reg-1));
  fprintf(out, "  move.l d%d, (a1)\n", REG_STACK(reg-1));

  reg -= 2;

  return 0;
}

int SegaGenesis::sega_genesis_putPattern_I()
{
  fprintf(out, "  move.w %s, (a0) ; putPattern()\n", pop_reg());

  return 0;
}

int SegaGenesis::sega_genesis_putPattern_I(int c)
{
  fprintf(out, "  move.w #0x%04x, (a0) ; putPattern(0x%04x)\n", c, c);

  return 0;
}

int SegaGenesis::sega_genesis_setImageData_aS()
{
  need_set_image_data = true;

  fprintf(out, "  movea.l %s, a3\n", pop_reg());
  fprintf(out, "  jsr (_set_image_data).l\n");

  return 0;
}

int SegaGenesis::sega_genesis_setPaletteColors_aS()
{
  need_set_palette_colors = true;

  fprintf(out, "  movea.l d%d, a3\n", REG_STACK(reg-1));
  fprintf(out, "  jsr (_set_palette_colors).l\n");
  reg--;

  return 0;
}

int SegaGenesis::sega_genesis_setPaletteColorsAtIndex_IaS()
{
  need_set_palette_colors_at_index = true;

  fprintf(out, "  movea.l d%d, a3\n", REG_STACK(reg-1));
  reg--;
  if (reg != 1)
  {
    fprintf(out, "  push.l d0\n");
    fprintf(out, "  move.l d%d, d0\n", REG_STACK(reg-1));
  }

  fprintf(out, "  jsr (_set_palette_colors_at_index).l\n");

  if (reg != 1)
  {
    fprintf(out, "  pop.l d0\n");
    reg--;
  }

  return 0;
}

int SegaGenesis::sega_genesis_setSpritePosition_III()
{
  CHECK_STACK

  fprintf(out, "  ;; setSpritePosition()\n");
  fprintf(out, "  asl.w #3, d%d\n", REG_STACK(reg-3));
  fprintf(out, "  swap d%d\n", REG_STACK(reg-3));

  fprintf(out, "  move.l #0x%08x, d5\n", CTRL_REG(CD_VRAM_WRITE, sprite_attribute_table));
  fprintf(out, "  add.l d%d, d5\n", REG_STACK(reg-3));
  fprintf(out, "  move.l d5, (a1)\n");
  fprintf(out, "  move.w d%d, (a0)\n", REG_STACK(reg-1));

  //fprintf(out, "  move.l #0x%08x, d5\n", CTRL_REG(CD_VRAM_WRITE, (sprite_attribute_table + 6)));
  //fprintf(out, "  add.l d%d, d5\n", REG_STACK(reg-3));
  fprintf(out, "  add.l #0x00060000, d5\n");
  fprintf(out, "  move.l d5, (a1)\n");
  fprintf(out, "  move.w d%d, (a0)\n", REG_STACK(reg-2));

  reg -= 3;

  return 0;
}

int SegaGenesis::sega_genesis_setSpriteConfig1_II()
{
  CHECK_STACK

  fprintf(out, "  ;; setSpriteConfig1()\n");
  fprintf(out, "  asl.w #3, d%d\n", REG_STACK(reg-2));
  fprintf(out, "  swap d%d\n", REG_STACK(reg-2));
  fprintf(out, "  move.l #0x%08x, d5\n", CTRL_REG(CD_VRAM_WRITE, (sprite_attribute_table + 2)));
  fprintf(out, "  add.l d%d, d5\n", REG_STACK(reg-2));
  fprintf(out, "  move.l d5, (a1)\n");
  fprintf(out, "  move.w d%d, (a0)\n", REG_STACK(reg-1));

  reg -= 2;

  return 0;
}

int SegaGenesis::sega_genesis_setSpriteConfig2_II()
{
  CHECK_STACK

  fprintf(out, "  ;; setSpriteConfig2()\n");
  fprintf(out, "  asl.w #3, d%d\n", REG_STACK(reg-2));
  fprintf(out, "  swap d%d\n", REG_STACK(reg-2));
  fprintf(out, "  move.l #0x%08x, d5\n", CTRL_REG(CD_VRAM_WRITE, (sprite_attribute_table + 4)));
  fprintf(out, "  add.l d%d, d5\n", REG_STACK(reg-2));
  fprintf(out, "  move.l d5, (a1)\n");
  //fprintf(out, "  or.w #0x8000, d%d\n", REG_STACK(reg-1));
  fprintf(out, "  move.w d%d, (a0)\n", REG_STACK(reg-1));

  reg -= 2;

  return 0;
}

int SegaGenesis::sega_genesis_getJoypadValuePort1()
{
  fprintf(out, "  ;; Read from joypad 1\n");
  fprintf(out, "  move.b #0x40, (0xa10009) ; Set direction\n");
  fprintf(out, "  move.b #0x40, (0xa10003) ; TH=1\n");
  fprintf(out, "  move.b (0xa10003), d%d\n", REG_STACK(reg));
  fprintf(out, "  and.l #0xff, d%d\n", REG_STACK(reg));

  fprintf(out, "  move.b #0x00, (0xa10003) ; TH=0\n");
  fprintf(out, "  move.b (0xa10003), d5\n");
  fprintf(out, "  and.l #0xff, d5\n");
  fprintf(out, "  lsl.l #8, d5\n");
  fprintf(out, "  or.l d5, d%d\n", REG_STACK(reg));
  fprintf(out, "  eori.w #0xffff, d%d\n", REG_STACK(reg));

  reg++;

  return 0;
}

int SegaGenesis::sega_genesis_getJoypadValuePort2()
{
  fprintf(out, "  ;; Read from joypad 2\n");
  fprintf(out, "  move.b #0x40, (0xa1000b) ; Set direction\n");
  fprintf(out, "  move.b #0x40, (0xa10005) ; TH=1\n");
  fprintf(out, "  move.b (0xa10005), d%d\n", REG_STACK(reg));
  fprintf(out, "  and.l #0xff, d%d\n", REG_STACK(reg));

  fprintf(out, "  move.b #0x00, (0xa10005) ; TH=0\n");
  fprintf(out, "  move.b (0xa10005), d5\n");
  fprintf(out, "  and.l #0xff, d5\n");
  fprintf(out, "  lsl.l #8, d5\n");
  fprintf(out, "  or.l d5, d%d\n", REG_STACK(reg));
  fprintf(out, "  eori.w #0xffff, d%d\n", REG_STACK(reg));

  reg++;

  return 0;

  return 0;
}

int SegaGenesis::sega_genesis_loadZ80_aB()
{
  need_load_z80 = true;

  fprintf(out, "  movea.l d%d, a3\n", REG_STACK(reg-1));
  fprintf(out, "  jsr (_load_z80).l\n");
  reg--;

  return 0;
}

int SegaGenesis::sega_genesis_resetZ80()
{
  // REVIEW: Does there need to be code that waits for the reset like
  //         done in the loading section?
  fprintf(out, "  move.w #0x100, (Z80_RESET)  ; Reset Z80\n");
  return 0;
}

int SegaGenesis::sega_genesis_pauseZ80()
{
  fprintf(out, "  move.w #0x100, (Z80_BUSREQ) ; Pause Z80\n");

  return 0;
}

int SegaGenesis::sega_genesis_startZ80()
{
  fprintf(out, "  move.w #0x100, (Z80_BUSREQ) ; Start Z80\n");

  return 0;
}

void SegaGenesis::add_exception_vectors()
{
  fprintf(out,
    "  ;-------------------------------:\n"
    "  ; exception vectors\n"
    "  ;-------------------------------:\n\n"

    "  dc32 0x00000000   ; startup SP\n"
    "  dc32 start        ; startup PC\n"
    "  dc32 interrupt    ; bus\n"
    "  dc32 interrupt    ; addr\n"
    "  dc32 interrupt    ; illegal\n"
    "  dc32 interrupt    ; divzero\n"
    "  dc32 interrupt    ; CHK\n"
    "  dc32 interrupt    ; TRAPV\n"
    "  dc32 interrupt    ; priv\n"
    "  dc32 interrupt    ; trace\n"
    "  dc32 interrupt    ; line 1010 emulator\n"
    "  dc32 interrupt    ; line 1111 emulator\n"
    "  dc32 interrupt\n"
    "  dc32 interrupt\n"
    "  dc32 interrupt\n"
    "  dc32 interrupt\n"
    "  dc32 interrupt\n"
    "  dc32 interrupt\n"
    "  dc32 interrupt\n"
    "  dc32 interrupt\n"
    "  dc32 interrupt\n"
    "  dc32 interrupt\n"
    "  dc32 interrupt\n"
    "  dc32 interrupt\n"
    "  dc32 interrupt    ; spurious interrupt\n"
    "  dc32 interrupt    ; interrupt level 1 (lowest priority)\n"
    "  dc32 extint       ; interrupt level 2 = external interrupt\n"
    "  dc32 interrupt    ; interrupt level 3\n"
    "  dc32 hsync        ; interrupt level 4 = H-sync interrupt\n"
    "  dc32 interrupt    ; interrupt level 5\n"
    "  dc32 vsync        ; interrupt level 6 = V-sync interrupt\n"
    "  dc32 interrupt    ; interrupt level 7 (highest priority)\n"
    "  dc32 interrupt    ; TRAP #00 exception\n"
    "  dc32 interrupt    ; TRAP #01 exception\n"
    "  dc32 interrupt    ; TRAP #02 exception\n"
    "  dc32 interrupt    ; TRAP #03 exception\n"
    "  dc32 interrupt    ; TRAP #04 exception\n"
    "  dc32 interrupt    ; TRAP #05 exception\n"
    "  dc32 interrupt    ; TRAP #06 exception\n"
    "  dc32 interrupt    ; TRAP #07 exception\n"
    "  dc32 interrupt    ; TRAP #08 exception\n"
    "  dc32 interrupt    ; TRAP #09 exception\n"
    "  dc32 interrupt    ; TRAP #10 exception\n"
    "  dc32 interrupt    ; TRAP #11 exception\n"
    "  dc32 interrupt    ; TRAP #12 exception\n"
    "  dc32 interrupt    ; TRAP #13 exception\n"
    "  dc32 interrupt    ; TRAP #14 exception\n"
    "  dc32 interrupt    ; TRAP #15 exception\n"
    "  dc32 interrupt    ; Unused (reserved)\n"
    "  dc32 interrupt    ; Unused (reserved)\n"
    "  dc32 interrupt    ; Unused (reserved)\n"
    "  dc32 interrupt    ; Unused (reserved)\n"
    "  dc32 interrupt    ; Unused (reserved)\n"
    "  dc32 interrupt    ; Unused (reserved)\n"
    "  dc32 interrupt    ; Unused (reserved)\n"
    "  dc32 interrupt    ; Unused (reserved)\n"
    "  dc32 interrupt    ; Unused (reserved)\n"
    "  dc32 interrupt    ; Unused (reserved)\n"
    "  dc32 interrupt    ; Unused (reserved)\n"
    "  dc32 interrupt    ; Unused (reserved)\n"
    "  dc32 interrupt    ; Unused (reserved)\n"
    "  dc32 interrupt    ; Unused (reserved)\n"
    "  dc32 interrupt    ; Unused (reserved)\n"
    "  dc32 interrupt    ; Unused (reserved)\n\n");
}

void SegaGenesis::add_cartridge_info_header()
{
  fprintf(out,
    "  ;-------------------------------:\n"
    "  ; cartridge info header\n"
    "  ;-------------------------------:\n\n"

    "  dc.b \"SEGA GENESIS    \"  ; must start with \"SEGA\"\n"
    "  dc.b \"(C)---- \"          ; copyright\n"
    "  dc.b \"2015.MAY\"          ; date\n"
    "  dc.b \"JAVA GRINDER                                    \" ; cart name\n"
    "  dc.b \"JAVA GRINDER                                    \" ; cart name (alt. language)\n"
    "  dc.b \"GM MK-0000 -00\"    ; program type / catalog number\n"
    "  dc.w 0x0000                ; ROM checksum\n"
    "  dc.b \"J               \"  ; hardware used\n"
    "  dc32 0x00000000            ; start of ROM\n"
    "  dc32 0x003fffff            ; end of ROM\n"
    "  dc32 0x00ff0000,0x00ffffff ; RAM start/end\n"
    "  dc.b \"            \"      ; backup RAM info\n"
    "  dc.b \"            \"      ; modem info\n"
    "  dc.b \"                                        \" ; comment\n"
    "  dc.b \"JUE             \"  ; regions allowed\n\n");
}

void SegaGenesis::add_exception_handler()
{
  fprintf(out,
    "  ;-------------------------------:\n"
    "  ; exception handlers\n"
    "  ;-------------------------------:\n\n"

    "extint:\n"
    "hsync:\n"
    "vsync:\n"
    "interrupt:\n"
    "  rte\n\n");
}

void SegaGenesis::add_load_fonts()
{
  fprintf(out,
    "_load_fonts:\n"
    "  move.w #((fontend - font) / 4) - 1, d6\n"
    //"  move.l #0x4c200000, (a1)  ; C00004 VRAM write to 0x0c20\n"
    "  move.l #0x%08x, (a1)  ; C00004 VRAM write to 0x8c00\n"
    "  movea.l #font, a2         ; Point to font set\n"
    "_load_fonts_loop:\n"
    "  move.l (a2)+, (a0)        ; C00000 write next longword of charset to VDP\n"
    "  dbra d6, _load_fonts_loop ; loop until done\n"
    "  rts\n\n", CTRL_REG(CD_VRAM_WRITE, 0x8c00));

  fprintf(out,
    ".align 32\n"
    "font:\n"
    "  dc32 0x01111100, 0x11000110, 0x11000110, 0x11000110 ; A\n"
    "  dc32 0x11111110, 0x11000110, 0x11000110, 0x00000000\n"
    "  dc32 0x11111100, 0x11000110, 0x11000110, 0x11111100 ; B\n"
    "  dc32 0x11000110, 0x11000110, 0x11111100, 0x00000000\n"
    "  dc32 0x11111110, 0x11000110, 0x11000110, 0x11000000 ; C\n"
    "  dc32 0x11000110, 0x11000110, 0x11111110, 0x00000000\n"
    "  dc32 0x11111100, 0x11000110, 0x11000110, 0x11000110 ; D\n"
    "  dc32 0x11000110, 0x11000110, 0x11111100, 0x00000000\n"
    "  dc32 0x11111110, 0x11000000, 0x11000000, 0x11111100 ; E\n"
    "  dc32 0x11000000, 0x11000000, 0x11111110, 0x00000000\n"
    "  dc32 0x11111110, 0x11000000, 0x11000000, 0x11111100 ; F\n"
    "  dc32 0x11000000, 0x11000000, 0x11000000, 0x00000000\n"
    "  dc32 0x11111110, 0x11000110, 0x11000000, 0x11001110 ; G\n"
    "  dc32 0x11000110, 0x11000110, 0x11111110, 0x00000000\n"
    "  dc32 0x11000110, 0x11000110, 0x11000110, 0x11111110 ; H\n"
    "  dc32 0x11000110, 0x11000110, 0x11000110, 0x00000000\n"
    "  dc32 0x00111000, 0x00111000, 0x00111000, 0x00111000 ; I\n"
    "  dc32 0x00111000, 0x00111000, 0x00111000, 0x00000000\n"
    "  dc32 0x00000110, 0x00000110, 0x00000110, 0x00000110 ; J\n"
    "  dc32 0x00000110, 0x01100110, 0x01111110, 0x00000000\n"
    "  dc32 0x11000110, 0x11001100, 0x11111000, 0x11111000 ; K\n"
    "  dc32 0x11001100, 0x11000110, 0x11000110, 0x00000000\n"
    "  dc32 0x01100000, 0x01100000, 0x01100000, 0x01100000 ; L\n"
    "  dc32 0x01100000, 0x01100000, 0x01111110, 0x00000000\n"
    "  dc32 0x11000110, 0x11101110, 0x11111110, 0x11010110 ; M\n"
    "  dc32 0x11000110, 0x11000110, 0x11000110, 0x00000000\n"
    "  dc32 0x11000110, 0x11100110, 0x11110110, 0x11011110 ; N\n"
    "  dc32 0x11001110, 0x11000110, 0x11000110, 0x00000000\n"
    "  dc32 0x11111110, 0x11000110, 0x11000110, 0x11000110 ; O\n"
    "  dc32 0x11000110, 0x11000110, 0x11111110, 0x00000000\n"
    "  dc32 0x11111110, 0x11000110, 0x11000110, 0x11111110 ; P\n"
    "  dc32 0x11000000, 0x11000000, 0x11000000, 0x00000000\n"
    "  dc32 0x11111110, 0x11000110, 0x11000110, 0x11000110 ; Q\n"
    "  dc32 0x11001110, 0x11001110, 0x11111110, 0x00000000\n"
    "  dc32 0x11111110, 0x11000110, 0x11000110, 0x11111100 ; R\n"
    "  dc32 0x11000110, 0x11000110, 0x11000110, 0x00000000\n"
    "  dc32 0x11111110, 0x11000110, 0x11000000, 0x11111110 ; S\n"
    "  dc32 0x00000110, 0x11000110, 0x11111110, 0x00000000\n"
    "  dc32 0x11111110, 0x00111000, 0x00111000, 0x00111000 ; T\n"
    "  dc32 0x00111000, 0x00111000, 0x00111000, 0x00000000\n"
    "  dc32 0x11000110, 0x11000110, 0x11000110, 0x11000110 ; U\n"
    "  dc32 0x11000110, 0x11000110, 0x11111110, 0x00000000\n"
    "  dc32 0x11000110, 0x11000110, 0x11000110, 0x11000110 ; V\n"
    "  dc32 0x01101100, 0x00111000, 0x00010000, 0x00000000\n"
    "  dc32 0x11000110, 0x11000110, 0x11000110, 0x11010110 ; W\n"
    "  dc32 0x11111110, 0x11101110, 0x11000110, 0x00000000\n"
    "  dc32 0x11000110, 0x11000110, 0x11101110, 0x01111100 ; X\n"
    "  dc32 0x11101110, 0x11000110, 0x11000110, 0x00000000\n"
    "  dc32 0x11000110, 0x11000110, 0x11000110, 0x01101100 ; Y\n"
    "  dc32 0x00111000, 0x00111000, 0x00111000, 0x00000000\n"
    "  dc32 0x11111110, 0x00001110, 0x00011100, 0x00111000 ; Z\n"
    "  dc32 0x01110000, 0x11100000, 0x11111110, 0x00000000\n"
    "  dc32 0x00000000, 0x00000000, 0x00000000, 0x00000000 ; ' '\n"
    "  dc32 0x00000000, 0x00000000, 0x00000000, 0x00000000\n"
    "fontend:\n\n");
}

void SegaGenesis::add_clear_text()
{
  fprintf(out,
    "_clear_text:\n"
    "  move.w #(64 * 32 / 2) - 1, d6\n"
    "  move.l #0x%08x, (a1)  ; C00004 VRAM write to 0xe000\n"
    "  move.l #((1120 + (']' - 'A')) << 16) | (1120 + (']' - 'A')), d7\n"
    "_clear_text_loop:\n"
    "  move.l d7, (a0)           ; C00000 write next longword of ' ' to VDP\n"
    "  dbra d6, _clear_text_loop ; loop until done\n"
    "  rts\n\n", CTRL_REG(CD_VRAM_WRITE, 0xe000));
}

void SegaGenesis::add_vdp_reg_init()
{
  fprintf(out,
    "  ; VDP register initialization (24 bytes)\n"
    ".align 32\n" 
    "vdp_reg_init_table:\n"
    "  dc.b  0x04  ; reg  0 = mode reg 1: no H interrupt\n"
    "  dc.b  0x14  ; reg  1 = mode reg 2: blanked, no V interrupt, DMA enable\n"
    "  dc.b  0x38  ; reg  2 = name table base for scroll A: 0xe000\n"
    "  dc.b  0x3c  ; reg  3 = name table base for window:   0xf000\n"
    "  dc.b  0x06  ; reg  4 = name table base for scroll B: 0xc000\n"
    "  dc.b  0x6c  ; reg  5 = sprite attribute table base: 0xd800\n"
    "  dc.b  0x00  ; reg  6 = unused register: 0x00\n"
    "  dc.b  0x00  ; reg  7 = background color: 0x00\n"
    "  dc.b  0x00  ; reg  8 = unused register: 0x00\n"
    "  dc.b  0x00  ; reg  9 = unused register: 0x00\n"
    "  dc.b  0xff  ; reg 10 = H interrupt register: 0xFF (esentially off)\n"
    "  dc.b  0x03  ; reg 11 = mode reg 3: disable ext int, full H/V scroll\n"
    "  dc.b  0x81  ; reg 12 = mode reg 4: 40 cell horiz mode, no interlace\n"
    "  dc.b  0x3f  ; reg 13 = H scroll table base: 0xfc00\n"
    "  dc.b  0x00  ; reg 14 = unused register: 0x00\n"
    "  dc.b  0x02  ; reg 15 = auto increment: 0x02\n"
    "  dc.b  0x01  ; reg 16 = scroll size: V=32 cell, H=64 cell\n"
    "  dc.b  0x00  ; reg 17 = window H position: 0x00\n"
    "  dc.b  0x00  ; reg 18 = window V position: 0x00\n"
    "  dc.b  0xff  ; reg 19 = DMA length count low:   0x00ff\n"
    "  dc.b  0xff  ; reg 20 = DMA length count high:  0xffxx\n"
    "  dc.b  0x00  ; reg 21 = DMA source address low: 0xxxxx00\n"
    "  dc.b  0x00  ; reg 22 = DMA source address mid: 0xxx00xx\n"
    "  dc.b  0x80  ; reg 23 = DMA source address high: VRAM fill, addr = 0x00xxxx\n\n"

    "  ; PSG initialization: set all channels to minimum volume\n"
    "psg_reg_init_table:\n"
    "  dc.b  0x9f,0xbf,0xdf,0xff\n\n");
}

void SegaGenesis::add_print_string()
{
  fprintf(out,
    "_print_string:\n"
    "  move.l (-4,a2), d5\n"
    "  subq.l #1, d5\n"
    "_print_string_loop:\n"
    "  eor.l d6, d6\n"
    "  move.b (a2)+, d6\n"
    "  add.w #1120 - 'A', d6\n"
    "  or.w #0x6000, d6\n"
    "  move.w d6, (a0)\n"
    "  dbra d5, _print_string_loop\n"
    "  rts\n\n");
}

void SegaGenesis::add_load_z80()
{
  fprintf(out,
    "  ; Load Z80 with software up to 8k.  a3 = pointer to code\n"
    ".align 32\n" 
    "_load_z80:\n"
    "  move.w #0x100, (Z80_BUSREQ) ; Pause Z80\n"
    "  move.w #0x100, (Z80_RESET)  ; Reset Z80\n"
    "_load_z80_wait_reset:\n"
    "  btst #0, (Z80_BUSREQ)\n"
    "  bne.s _load_z80_wait_reset\n"
    "  lea (Z80_RAM), a2          ; 8k RAM area\n"
    "  move.l (-4,a3), d5         ; Code len\n"
    "_load_z80_next_byte:\n"
    "  move.b (a3)+, (a2)+\n"
    "  dbf d5, _load_z80_next_byte\n"
    "  move.w #0, (Z80_RESET)     ; Disable the Z80 reset.\n"
    "  move.w #0, (Z80_BUSREQ)    ; Give the Z80 the bus back.\n"
    "  move.w #0x100, (Z80_RESET) ; Reset Z80\n"
    "  rts\n\n");
}

void SegaGenesis::add_set_pattern_table()
{
  // a3 points to int[] array
  fprintf(out,
    "_set_pattern_table:\n"
    //"  move.l #0x%08x, d6\n"
    "  lsl.l #5, d7               ; pattern_index * 32\n"
    "  move.l d7, d5\n"
    "  rol.w #2, d5\n"
    "  and.w #3, d5               ; d5 = upper 2 bits moved to lower 2 bits\n"
    "  and.w #0x3ffe, d7          ; d7 = lower 13 bits\n"
    "  or.w #0x4000, d7\n"
    "  swap d7\n"
    "  or.w d5, d7\n"
    "  move.l d7, (a1)\n"
    "  move.l (-4,a3), d5         ; Code len\n"
    "  subq.l #1, d5\n"
    "_set_pattern_table_loop:\n"
    "  move.l (a3)+, (a0)\n"
    "  dbf d5, _set_pattern_table_loop\n"
    "  rts\n\n");
}

void SegaGenesis::add_set_image_data()
{
  // a3 points to byte[] array
  fprintf(out,
    "_set_image_data:\n"
    "  move.l #0x40000003, d7     ; Set cursor position in VDP\n"
    "  move.l d7, (a1)\n"
    "  move.l (-4,a3), d5         ; Code len\n"
    "  eor.w d6, d6\n"
    "_set_image_data_loop:\n"
    "  move.w (a3)+, (a0)\n"
    "  add.w #1, d6\n"
    "  cmp.w #40, d6\n"
    "  bne.s _set_image_data_not_40\n"
    "  eor.w d6, d6\n"
    "  add.l #0x00800000, d7\n"
    "  move.l d7, (a1)\n"
    "_set_image_data_not_40:\n"
    "  dbf d5, _set_image_data_loop\n"
    "  rts\n\n");
}

void SegaGenesis::add_set_palette_colors()
{
  // a3 points to short[] array
  fprintf(out,
    "_set_palette_colors:\n"
    "  move.l #0xc0000000, (a1)   ; C00004 write CRAM address 0x0000\n"
    "  move.l (-4,a3), d5         ; Code len\n"
    "_set_palette_colors_loop:\n"
    "  move.w (a3)+, (a0)\n"
    "  dbf d5, _set_palette_colors_loop\n"
    "  rts\n\n");
}

void SegaGenesis::add_set_palette_colors_at_index()
{
  // a3 points to short[] array
  fprintf(out,
    "_set_palette_colors_at_index:\n"
    "  lsl.w #1, d0\n"
    "  swap d0\n"
    "  add.l #0xc0000000, d0\n"
    "  move.l d0, (a1)   ; C00004 write CRAM address d0 * 2\n"
    "  move.l (-4,a3), d5         ; Code len\n"
    "_set_palette_colors_at_index_loop:\n"
    "  move.w (a3)+, (a0)\n"
    "  dbf d5, _set_palette_colors_at_index_loop\n"
    "  rts\n\n");
}

void SegaGenesis::add_init_bitmap()
{
  need_clear_bitmap = true;

  fprintf(out,
    "  ;; 1) Clear out first 1120 pattersn\n"
    "  ;; 2) Set the 40x28 pattern area in Scroll B to the first 1120 patterns\n"
    "  ;; 3) Set the other patterns in Sroll B to a blank pattern\n"
    "_init_bitmap:\n"
    "  move.l d4, (-4,a7)         ; put d4 on stack (or really below)\n"
    "  jsr (_clear_bitmap).l\n"

    "  ; Set pattern table to blank pattern\n"
    "  move.l #0x40000003, (a1)   ; C00004 VRAM write to 0xC000\n"
    "  move.l #(0x1000 / 4)-1, d5 ; pattern table len in longs minus 1\n"
    "  move.l #((1120 + (']' - 'A')) << 16) | (1120 + (']' - 'A')), d7\n"
    "_init_bitmap_other_loop:\n"
    "  move.l d7, (a0)\n"
    "  dbf d5, _init_bitmap_other_loop\n"

    "  ; Set up pattern table\n"
    "  move.l #0x40000003, d4     ; Set cursor position in VDP\n"
    "  move.l d4, (a1)            ; C00004 VRAM write to 0xC000\n"
    "  move.w #(40*28)-1, d5      ; data len\n"
    "  eor.l d6, d6\n"
    "  move.l d6, d7\n"
    "_init_bitmap_loop:\n"
    "  move.w d7, (a0)\n"
    "  addq.l #1, d6\n"
    "  cmp.w #40, d6\n"
    "  bne.s _init_bitmap_loop_not_40\n"
    "  eor.w d6, d6\n"
    "  add.l #0x00800000, d4\n"
    "  move.l d4, (a1)\n"
    "_init_bitmap_loop_not_40:\n"
    "  addq.l #1, d7\n"
    "  dbf d5, _init_bitmap_loop\n"

    "  move.l (-4,a7), d4         ; recover d4\n"
    "  rts\n\n");
}

void SegaGenesis::add_clear_bitmap()
{
  fprintf(out,
    "_clear_bitmap:\n"
    "  move.l #0x40000000, (a1)     ; C00004 VRAM write to 0x0000\n"
    "  move.l #((40*28*32)/4)-1, d5 ; Pattern len (w*h*32 bytes)\n"
    "  eor.l d6, d6\n"
    "_clear_bitmap_loop:\n"
    "  move.l d6, (a0)\n"
    "  dbf d5, _clear_bitmap_loop\n"
    "  rts\n\n");
}

void SegaGenesis::add_clear_pattern()
{
  fprintf(out,
    "_clear_pattern:\n"
    //"  swap d0\n"
    "  lsl.l #3, d0\n"
    "  subq.l #1, d0\n"
    //"  add.l #0x40000000, d0\n"
    //"  move.l d0, (a1)              ; C00004 VRAM write to 0x0000\n"
    //"  move.w #7, d5                ; Pattern len 32 bytes (8 longs)\n"
    "  eor.l d6, d6\n"
    "_clear_pattern_loop:\n"
    "  move.l d6, (a0)\n"
    "  dbf d0, _clear_pattern_loop\n"
    "  rts\n\n");
}

void SegaGenesis::add_plot()
{
  // block_x = x / 8
  // block_y = y / 8
  // address = (block_x + (block_y * (32 + 8))) * 32
  // pixel_x = x % 8
  // pixel_y = y % 8
  // address += ((pixel_x) + (pixel_y * 8)) / 2  or
  // address += (pixel_x / 2) + (pixel_y * 4)

  // d0 = X
  // d1 = Y
  // d2 = color
  fprintf(out,
    "  ;; plot(x=d0, y=d1, color=d2);\n"
    "_plot:\n"
    "  move.w d0, d7\n"
    "  move.w d1, d6\n"
    "  lsr.w #3, d0   ; block_x = x / 8\n"
    "  and.w #0xfff8, d1 ; block_y = (block_y / 8) * 8\n"
    "  add.l d1, d0   ; address = block_x + block_y\n"
    "  lsl.l #2, d1   ; block_y *= 4\n"
    "  add.l d1, d0   ; address = block_x + block_y\n"
    "  lsl.l #5, d0   ; address *= 32\n"
    "  move.w d7, d1\n"
    "  and.l #0x7, d1 ; x = x %% 8\n"
    "  lsr.w #1, d1   ; x = x / 2\n"
    "  add.l d1, d0   ; address += x\n"
    "  move.w d6, d1\n"
    "  and.l #0x7, d1 ; y = y %% 8\n"
    "  lsl.w #2, d1   ; y = y * 4\n"
    "  add.l d1, d0   ; address += y\n"
    "  move.l d0, d5  ; need the upper 2 bits\n"
    "  rol.w #2, d5\n"
    "  and.w #3, d5\n"
    "  and.w #0x3ffe, d0; address &= 0xffff3ffe (write to even address)\n"
    "  swap d0        ; address <<= 16\n"
    "  or.b d5, d0    ; move upper two bits of address into lower\n"
    //"_plot_wait_fifo_empty:\n"
    //"  btst.l #9, (a1)\n"
    //"  beq.s _plot_wait_fifo_empty\n"
    "  move.l d0, (a1); read word from VRAM and save in temp space\n"
    "  move.w (a0), d5\n"
    "  or.l #0x40000000, d0\n"
    "  move.w d7, d1\n"
    "  and.w #0x3, d1 ; x = x & 0x3\n"
    "  neg.w d1\n"
    "  addq.w #3, d1\n"
    "  lsl.w #2, d1\n"
    "  lsl.w d1, d2\n"
    "  move.w #0xfff0, d6\n"
    "  rol.w d1, d6\n"
    "  and.w d5, d6\n"
    "  or.w d6, d2\n"
    "  move.l d0, (a1)\n"
    "  move.w d2, (a0)\n"
    "  rts\n\n");
}

void SegaGenesis::add_set_plot_address()
{
  fprintf(out,
    "  ;; set_plot_address(d5=address);\n"
    "_set_plot_address:\n"
    "  move.l d5, d6  ; need the upper 2 bits\n"
    "  rol.w #2, d6\n"
    "  and.w #3, d6\n"
    "  and.w #0x3ffe, d5; address &= 0xffff3ffe (write to even address)\n"
    "  swap d5        ; address <<= 16\n"
    "  or.b d6, d5    ; move upper two bits of address into lower\n"
    "  or.l #0x40000000, d5\n"
    "  move.l d5, (a1); set write word to VRAM\n"
    "  rts\n\n");
}

