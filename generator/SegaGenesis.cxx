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
 * http://emu-docs.org/Genesis/sega2f.htm
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "SegaGenesis.h"

// FIXME - This is redundant and kind of dangerous.
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

// Functions that return voice should never use the stack for registers.. ?
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
  need_load_z80(false),
  need_set_pattern_table(false),
  need_set_image_data(false),
  need_set_palette_colors(false),
  need_init_bitmap(false),
  need_clear_bitmap(false),
  need_plot(false),
  need_set_plot_address(false)
{
  // FIXME - What's this access prohibited crap?
  //ram_start = 0xe00000;
  ram_start = 0xff0000;
  //stack_start = 0x1000000;
  stack_start = 0xfffffe00;  // <-- dafuq?
}

SegaGenesis::~SegaGenesis()
{
  add_vdp_reg_init();

  if (need_print_string) { add_print_string(); }
  if (need_load_fonts) { add_load_fonts(); }
  if (need_load_z80) { add_load_z80(); }
  if (need_set_pattern_table) { add_set_pattern_table(); }
  if (need_set_image_data) { add_set_image_data(); }
  if (need_set_palette_colors) { add_set_palette_colors(); }
  if (need_init_bitmap) { add_init_bitmap(); }
  if (need_clear_bitmap) { add_clear_bitmap(); }
  if (need_plot) { add_plot(); }
  if (need_set_plot_address) { add_set_plot_address(); }
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
    "  btst.l #1, d1        ; test DMA busy flag\n"
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

int SegaGenesis::sega_genesis_setPalettePointer()
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

  fprintf(out, "  lsl.l #8, d%d\n", d);
  fprintf(out, "  lsl.l #8, d%d\n", d);
  fprintf(out, "  or.l #0xc0000000, d%d\n", d);
  fprintf(out, "  move.l d%d, (a1) ; Set CRAM write address\n", d);

  return 0;
}

int SegaGenesis::sega_genesis_setPalettePointer(int index)
{
  if (index < 0 || index > 63)
  {
    printf("Palette index %d out of range 0-63\n", index);
    return -1;
  }

  fprintf(out, "  move.l #0xc0%02x0000, (a1) ; Set CRAM write address\n", index);

  return 0;
}

int SegaGenesis::sega_genesis_setPaletteColor()
{
  fprintf(out, "  move.l %s, (a0)\n", pop_reg());
#if 0
  if (stack > 0)
  {
    fprintf(out, "  move.l (SP)+, (a0)\n");
    stack--;
  }
    else
  {
    fprintf(out, "  move.l d%d, (a0)\n", REG_STACK(reg-1));
    reg--;
  }
#endif

  return 0;
}

int SegaGenesis::sega_genesis_setPaletteColor(int color)
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

  fprintf(out, "  jsr _init_bitmap\n");
  return 0;
}

int SegaGenesis::sega_genesis_clearBitmap()
{
  need_clear_bitmap = true;

  fprintf(out, "  jsr _clear_bitmap\n");
  return 0;
}

int SegaGenesis::sega_genesis_plot()
{
  need_plot = true;

  // REVIEW: Can possibly save some CPU instructions by moving to stack
  fprintf(out, "  move.l %s, d7\n", pop_reg());
  fprintf(out, "  move.l %s, d6\n", pop_reg());
  fprintf(out, "  move.l %s, d5\n", pop_reg());
  fprintf(out, "  jsr _plot\n");
  return 0;
}

int SegaGenesis::sega_genesis_setPlotAddress()
{
  need_set_plot_address = true;

  fprintf(out, "  move.l %s, d5\n", pop_reg());
  fprintf(out, "  jsr _set_plot_address\n");

  reg--;

  return 0;

}

int SegaGenesis::sega_genesis_fastPlot()
{
  // FIXME
  CHECK_STACK

  fprintf(out, "  move.w d%d, (a1)\n", REG_STACK(reg-2));
  fprintf(out, "  move.w d%d, (a1)\n", REG_STACK(reg-1));

  reg -= 2;

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
    "  move.w (a1), d5      ; C00004 read VDP status\n"
    "  btst.l #3, d5        ; test vertical blank flag\n"
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
    "  move.w (a1), d5      ; C00004 read VDP status\n"
    "  btst.l #2, d5        ; test horizontal blank flag\n"
    "  beq.s _wait_horizontal_blank_%d\n",
    label_count, label_count);

  label_count++;

  return 0;
}

int SegaGenesis::sega_genesis_loadFonts()
{
  need_load_fonts = true;

  fprintf(out, "  jsr _load_fonts\n");
  return 0;
}

int SegaGenesis::sega_genesis_setCursor()
{
  // FIXME - Holy crap.  This is fugly.  Should this just be a function?
  fprintf(out, "  ; Set cursor position in VDP\n");
  fprintf(out, "  mulu.w #128, d%d\n", REG_STACK(reg-1));
  fprintf(out, "  add.w d%d, d%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
  fprintf(out, "  lsl.l #8, d%d\n", REG_STACK(reg-1));
  fprintf(out, "  lsl.l #8, d%d\n", REG_STACK(reg-1));
  fprintf(out, "  or.l #0x%08x, d%d\n", CTRL_REG(CD_VRAM_WRITE, 0xc000), REG_STACK(reg-1));
  fprintf(out, "  move.l d%d, (a1)\n", REG_STACK(reg-1));

  reg -= 2;

  return 0;
}

int SegaGenesis::sega_genesis_setCursor(int x, int y)
{
  // FIXME - This function isn't getting called when it should.
  // 0100 0101 1001 0100 0000 0000 0000 0011
  // CD = 000001 (VRAM WRITE)
  //  A = 1100 0101 1001 0100 = 0xc594
  int address = (0xc000 + (y * 128 + x));

  fprintf(out,
    "  move.l #0x%8x, (a1) ; Set cursor position in VDP\n",
    CTRL_REG(CD_VRAM_WRITE, address));

  return 0;
}

int SegaGenesis::sega_genesis_printChar()
{
  return -1;
}

int SegaGenesis::sega_genesis_printChar(int c)
{
  fprintf(out, "  move.w #0x%02x, (a0) ; printChar(0x%02x)\n", c, c);

  return 0;
}

int SegaGenesis::sega_genesis_print()
{
  need_print_string = true;

  fprintf(out, "  movea.l %s, a2\n", pop_reg());
  fprintf(out, "  jsr _print_string\n");

  return 0;
}

int SegaGenesis::sega_genesis_setHorizontalScroll()
{
  fprintf(out,
    "  move.l #0x%8x, (a1) ; Update horizontal scroll\n",
    CTRL_REG(CD_VRAM_WRITE, 0xfc00));

  reg--;

  return -1;
}

int SegaGenesis::sega_genesis_setVerticalScroll()
{
  reg--;

  return -1;
}

int SegaGenesis::sega_genesis_setPatternTable()
{
  need_set_pattern_table = true;

  fprintf(out, "  movea.l %s, a3\n", pop_reg());
  fprintf(out, "  jsr _set_pattern_table\n");

  return 0;
}

int SegaGenesis::sega_genesis_setImageData()
{
  need_set_image_data = true;

  fprintf(out, "  movea.l %s, a3\n", pop_reg());
  fprintf(out, "  jsr _set_image_data\n");

  return 0;
}

int SegaGenesis::sega_genesis_setPaletteColors()
{
  need_set_palette_colors = true;

  fprintf(out, "  movea.l d%d, a3\n", REG_STACK(reg-1));
  fprintf(out, "  jsr _set_palette_colors\n");
  reg--;

  return 0;
}

int SegaGenesis::sega_genesis_setSpritePosition()
{
  CHECK_STACK

  fprintf(out, "  asl.w #3, d%d\n", REG_STACK(reg-3));
  fprintf(out, "  swap d%d\n", REG_STACK(reg-3));

  fprintf(out, "  ;; setSpritePosition()\n");
  fprintf(out, "  move.l #0x%08x, d5\n", CTRL_REG(CD_VRAM_WRITE, (sprite_attribute_table + 6)));
  fprintf(out, "  add.l d%d, d5\n", REG_STACK(reg-3));
  fprintf(out, "  move.l d5, (a1)\n");
  fprintf(out, "  move.w d%d, (a0)\n", REG_STACK(reg-2));

  fprintf(out, "  move.l #0x%08x, d5\n", CTRL_REG(CD_VRAM_WRITE, sprite_attribute_table));
  fprintf(out, "  add.l d%d, d5\n", REG_STACK(reg-3));
  fprintf(out, "  move.l d5, (a1)\n");
  fprintf(out, "  move.w d%d, (a1)\n", REG_STACK(reg-1));

  reg -= 3;

  return 0;
}

int SegaGenesis::sega_genesis_setSpriteConfig1()
{
  CHECK_STACK

  fprintf(out, "  ;; setSpriteConfig1()\n");
  fprintf(out, "  move.l #0x%08x, d5\n", CTRL_REG(CD_VRAM_WRITE, (sprite_attribute_table + 2)));
  fprintf(out, "  add.l d%d, d5\n", REG_STACK(reg-2));
  fprintf(out, "  move.l d5, (a1)\n");
  fprintf(out, "  move.w d%d, (a0)\n", REG_STACK(reg-1));

  reg -= 2;

  return 0;
}

int SegaGenesis::sega_genesis_setSpriteConfig2()
{
  CHECK_STACK

  fprintf(out, "  ;; setSpriteConfig2()\n");
  fprintf(out, "  move.l #0x%08x, d5\n", CTRL_REG(CD_VRAM_WRITE, (sprite_attribute_table + 4)));
  fprintf(out, "  add.l d%d, d5\n", REG_STACK(reg-2));
  fprintf(out, "  move.l d5, (a1)\n");
  fprintf(out, "  move.w d%d, (a0)\n", REG_STACK(reg-1));

  reg -= 2;

  return 0;
}

int SegaGenesis::sega_genesis_loadZ80()
{
  need_load_z80 = true;

  fprintf(out, "  movea.l d%d, a3\n", REG_STACK(reg-1));
  fprintf(out, "  jsr _load_z80\n");
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
    "  dc.l interrupt    ; Unused (reserved)\n\n");
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
    "  move.l #0x4c200000, (a1)  ; C00004 VRAM write to 0x0c20\n"
    "  movea.l #font, a2         ; Point to font set\n"
    "_load_fonts_loop:\n"
    "  move.l (a2)+, (a0)        ; C00000 write next longword of charset to VDP\n"
    "  dbra d6, _load_fonts_loop ; loop until done\n"
    "  rts\n\n");

  fprintf(out,
    ".align 32\n"
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

void SegaGenesis::add_vdp_reg_init()
{
  fprintf(out,
    //"vdp_reg_init:\n"
    //"  dc.l  0x00008000  ; d5 = VDP register 0 write command\n"
    //"  dc.l  0    ; D6 = unused\n"
    //"  dc.l  0x00000100  ; d7 = video register offset\n"
    //"  dc.l  0    ; A0 = unused\n"
    //"  dc.l  HW_VERSION  ; a1 = hardware version register\n"
    //"  dc.l  TMSS_REG    ; a2 = TMSS register\n"
    //"  dc.l  VDP_DATA    ; a3 = VDP data\n"
    //"  dc.l  VDP_CTRL    ; a4 = VDP control / status\n\n"

    "  ; VDP register initialization (24 bytes)\n"
    ".align 32\n" 
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
    "psg_reg_init_table:\n"
    "  dc.b  0x9f,0xbf,0xdf,0xff\n\n");
}

void SegaGenesis::add_print_string()
{
  fprintf(out,
    "_print_string:\n"
    "  move.l (-4,a2), d5\n"
    "  eor.l d6, d6\n"
    "_print_string_loop:\n"
    "  move.b (a2)+, d6\n"
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
    "  btst.b #0, (Z80_BUSREQ)\n"
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
    "  move.l #0x40000000, (a1)   ; C00004 VRAM write to 0x0000\n"
    "  move.l (-4,a3), d5         ; Code len\n"
    "_set_pattern_table_loop:\n"
    "  move.l (a3)+, (a0)\n"
    "  dbf d5, _set_pattern_table_loop\n"
    "  rts\n\n");
}

void SegaGenesis::add_set_image_data()
{
  //int address = (0xc000 + (0 * 64 + 0));

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

void SegaGenesis::add_init_bitmap()
{
  // REVIEW: Make faster by using a long write to (a0)
  fprintf(out,
    "_init_bitmap:\n"
    "  move.l d4, (-4,a7)         ; put d4 on stack (or really below)\n"
    "  move.l #0x40000000, (a1)   ; C00004 VRAM write to 0x0000\n"
    "  move.l #(40*28*16)+15, d5  ; Pattern len (plus 1)\n"
    "  eor.l d7, d7\n"
    "_clear_pattern_table_loop:\n"
    "  move.w d7, (a0)\n"
    "  dbf d5, _clear_pattern_table_loop\n"
    "  move.l #0x40000003, d4     ; Set cursor position in VDP\n"
    "  move.l d4, (a1)            ; C00004 VRAM write to 0xC000\n"
    "  move.l #(40*28)-1, d5      ; data len\n"
    "  eor.w d6, d6\n"
    "_init_bitmap_loop:\n"
    "  move.w d7, (a0)\n"
    "  add.w #1, d6\n"
    "  cmp.w #40, d6\n"
    "  bne.s _init_bitmap_loop_not_40\n"
    "  eor.w d6, d6\n"
    "  add.l #0x00800000, d4\n"
    "  move.l d4, (a1)\n"
    "_init_bitmap_loop_not_40:\n"
    "  addq.l #1, d7\n"
    "  dbf d5, _init_bitmap_loop\n"
    "  move.l (-4,a7), d4         ; recover d4\n"

    "  move.l #0x60000003, d7     ; Set cursor position in VDP\n"
    "  move.l d7, (a1)            ; C00004 VRAM write to 0xC000\n"
    "  move.l #0x1000-1, d5       ; data len\n"
    "  move.w #0x460, d7          ; blank pattern\n"
    "_init_bitmap_other_loop:\n"
    "  move.w d7, (a0)\n"
    "  dbf d5, _init_bitmap_other_loop\n"
    "  move.l (-4,a7), d4         ; recover d4\n"
    "  rts\n\n");
}

void SegaGenesis::add_clear_bitmap()
{
  // REVIEW: Make faster by using a long write to (a0)
  fprintf(out,
    "_clear_bitmap:\n"
    "  move.l #0x40000000, (a1)   ; C00004 VRAM write to 0x0000\n"
    "  move.l #(40*28*16)+15, d5  ; Pattern len (w*h*16 words) (plus 1)\n"
    "  eor.l d6, d6\n"
    "_clear_bitmap_loop:\n"
    "  move.w d6, (a0)\n"
    "  dbf d5, _clear_bitmap_loop\n"
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

  // d5 = X
  // d6 = Y
  // d7 = color
  fprintf(out,
    "  ;; plot(x=d5, y=d6, d7=color);\n"
    "_plot:\n"
    "  move.w d4, (-8,a7) ; save register so it can be used for other stuff\n"
    "  move.w d5, (-4,a7)\n"
    "  move.w d6, (-6,a7)\n"
    "  lsr.w #3, d5   ; block_x = x / 8\n"
    "  and.w #0xfff8, d6 ; block_y = (block_y / 8) * 8\n"
    "  add.l d6, d5   ; address = block_x + block_y\n"
    "  lsl.l #2, d6   ; block_y *= 4\n"
    "  add.l d6, d5   ; address = block_x + block_y\n"
    "  lsl.l #5, d5   ; address *= 32\n"
    "  move.w (-4,a7), d6\n"
    "  and.l #0x7, d6 ; x = x %% 8\n"
    "  lsr.w #1, d6   ; x = x / 2\n"
    "  add.l d6, d5   ; address += x\n"
    "  move.w (-6,a7), d6\n"
    "  and.l #0x7, d6 ; y = y %% 8\n"
    "  lsl.w #2, d6   ; y = y * 4\n"
    "  add.l d6, d5   ; address += y\n"
    "  move.l d5, d4  ; need the upper 2 bits\n"
    "  rol.w #2, d4\n"
    "  and.w #3, d4\n"
    "  and.w #0x3ffe, d5; address &= 0xffff3ffe (write to even address)\n"
    "  swap d5        ; address <<= 16\n"
    "  or.b d4, d5    ; move upper two bits of address into lower\n"
    "  move.l d5, (a1); read word from VRAM and save in temp space\n"
    "  move.w (a0), d4\n"
    "  or.l #0x40000000, d5\n"
    "  move.l d5, (a1)\n"
    "  move.w (-4,a7), d6\n"
    "  and.w #0x3, d6 ; x = x & 0x3\n"
    "  neg.w d6\n"
    "  addq.w #3, d6\n"
    "  lsl.w #2, d6\n"
    "  lsl.w d6, d7\n"
    "  move.w #0xfff0, d5\n"
    "  rol.w d6, d5\n"
    "  and.w d4, d5\n"
    "  or.w d5, d7\n"
    "  move.w d7, (a0)\n"
    "  move.w (-8,a7), d4 ; restore register\n"
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

