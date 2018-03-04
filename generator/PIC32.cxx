/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "PIC32.h"

#define CHECK_PORT_SPI() \
  if (port > 1) { printf("Illegal SPI port\n"); return -1; } \
  port++;

PIC32::PIC32() :
  need_spi_read(0)
{
  org = 0x1d001000;
  ram_start = 0x80000000;
  ram_end = 0x80003fff;
  virtual_address = 0x9d000000;
  physical_address = 0x1d000000;
}

PIC32::~PIC32()
{
  // it's 6 instructions to inline and 5 instructions to call
  //if (need_spi_read) { add_spi_read(); }
}

int PIC32::open(const char *filename)
{
  if (MIPS32::open(filename) != 0) { return -1; }

  fprintf(out, ".mips32\n");

  fprintf(out, "  SPI1CON equ 0x5800\n");
  fprintf(out, "  SPI1STAT equ 0x5810\n");
  fprintf(out, "  SPI1BUF equ 0x5820\n");
  fprintf(out, "  SPI1BRG equ 0x5830\n");
  fprintf(out, "  SPI2CON equ 0x5a00\n");
  fprintf(out, "  SPI2STAT equ 0x5a10\n");
  fprintf(out, "  SPI2BUF equ 0x5a20\n");
  fprintf(out, "  SPI2BRG equ 0x5a30\n");

  return 0;
}

int PIC32::start_init()
{
  MIPS32::start_init();

  fprintf(out, "  li $k0, 0xbf880000\n");
  fprintf(out, "  li $k1, 0xbf800000\n");

  return 0;
}

// GPIO functions
int PIC32::ioport_setPinsAsInput_I(int port)
{
  return -1;
}

int PIC32::ioport_setPinsAsOutput_I(int port)
{
  fprintf(out, "  ; setPinsAsOutput() port=%d TRIS%c\n", port, 'A' + port);
  fprintf(out, "  xori $t%d, $t%d, 0xffff\n", reg - 1, reg - 1);
  fprintf(out, "  sw $t%d, 0x6%02x0($k0)\n", reg - 1, port * 4);

  reg--;

  return 0;
}

int PIC32::ioport_setPinsValue_I(int port)
{
  fprintf(out, "  ; setPinsValue() port=%d LAT%c\n", port, 'A' + port);
  fprintf(out, "  sw $t%d, 0x6%02x0($k0)\n", reg - 1, (port * 4) + 2);

  reg--;

  return 0;
}

int PIC32::ioport_setPinsHigh_I(int port)
{
  return -1;
}

int PIC32::ioport_setPinsLow_I(int port)
{
  return -1;
}

int PIC32::ioport_setPinAsOutput_I(int port)
{
  return -1;
}

int PIC32::ioport_setPinAsInput_I(int port)
{
  return -1;
}

int PIC32::ioport_setPinHigh_I(int port)
{
  return -1;
}

int PIC32::ioport_setPinLow_I(int port)
{
  return -1;
}

int PIC32::ioport_isPinInputHigh_I(int port)
{
  return -1;
}

int PIC32::ioport_getPortInputValue(int port)
{
  return -1;
}

// SPI functions
int PIC32::spi_init_II(int port)
{
  CHECK_PORT_SPI();
  return -1;
}

int PIC32::spi_init_II(int port, int clock_divisor, int mode)
{
  CHECK_PORT_SPI();

  if (clock_divisor > 3) { printf("Clock divisor out of range\n"); }

  int mode_table[] = { 0, 4, 1, 5 };
  if (mode > 3) { mode = 0; }
  mode = mode_table[mode];

  // bit 15 = ENABLE
  // bit 8 = CKE (clock edge select)
  // bit 6 = CKP (clock polarity select)
  // bit 5 = MSTEN

  fprintf(out, "  ;; spi_init(%d, %d, %d)\n", port, clock_divisor, mode);
  fprintf(out, "  li $t8, 0x%x\n", (1 << 15) | (1 << 5) | (mode << 8));
  fprintf(out, "  sw $t8, SPI%dCON($k1)\n", port);

  fprintf(out, "  li $at, 0xbfc02ff8 ; DEVCFG1\n");
  fprintf(out, "  li $t9, 0x%08x\n", ~(3 << 12));
  fprintf(out, "  lw $t8, 0($at)\n");
  fprintf(out, "  and $t8, $t8, $t9\n");
  fprintf(out, "  li $t9, %d << 12\n", clock_divisor);
  fprintf(out, "  or $t8, $t8, $t9\n");
  fprintf(out, "  sw $t8, 0($at)\n");

  return 0;
}

int PIC32::spi_init16_II(int port)
{
  CHECK_PORT_SPI();
  return -1;
}

int PIC32::spi_init16_II(int port, int clock_divisor, int mode)
{
  CHECK_PORT_SPI();

  if (clock_divisor > 3) { printf("Clock divisor out of range\n"); }

  int mode_table[] = { 0, 4, 1, 5 };
  if (mode > 3) { mode = 0; }
  mode = mode_table[mode];

  // bit 23 = MCLKSEL (1=MCLK, 0=PBCLK)
  // bit 15 = ENABLE
  // bit 10-11: 0=8 bit, 1=16 bit, 2=32 bit
  // bit 8 = CKE (clock edge select)
  // bit 6 = CKP (clock polarity select)
  // bit 5 = MSTEN

  fprintf(out, "  ;; spi_init(%d, %d, %d)\n", port, clock_divisor, mode);
  fprintf(out, "  sw $0, SPI%dCON($k1)\n", port);
  fprintf(out, "  li $t8, %d\n", 1 << clock_divisor);
  fprintf(out, "  sw $t8, SPI%dBRG($k1)\n", port);
  //fprintf(out, "  li $t8, 0x%x\n", (1 << 23) | (1 << 15) | (1 << 10) | (1 << 5) | (mode << 8));
  fprintf(out, "  li $t8, 0x%x\n", (1 << 15) | (1 << 10) | (1 << 5) | (mode << 8));
  fprintf(out, "  sw $t8, SPI%dCON($k1)\n", port);

#if 0
  fprintf(out, "  li $at, 0xbfc02ff8 ; DEVCFG1\n");
  fprintf(out, "  li $t9, 0x%08x\n", ~(3 << 12));
  fprintf(out, "  lw $t8, 0($at)\n");
  fprintf(out, "  and $t8, $t8, $t9\n");
  fprintf(out, "  li $t9, %d << 12\n", clock_divisor);
  fprintf(out, "  or $t8, $t8, $t9\n");
  fprintf(out, "  sw $t8, 0($at)\n");
#endif

  return 0;
}

int PIC32::spi_send_I(int port)
{
  CHECK_PORT_SPI();

  fprintf(out, "  ;; spi_send_I()\n");

  if (stack == 0)
  {
    fprintf(out, "  sw $t%d, SPI%dBUF($k1)\n", reg - 1, port);
    reg--;
  }
    else
  {
    STACK_POP(8);
    fprintf(out, "  sw $t8, SPI%dBUF($k1)\n", port);
  }

  return 0;
}

int PIC32::spi_send16_I(int port)
{
  CHECK_PORT_SPI();

  fprintf(out, "  ;; spi_send16_I()\n");

  if (stack == 0)
  {
    fprintf(out, "  sw $t%d, SPI%dBUF($k1)\n", reg - 1, port);
    reg--;
  }
    else
  {
    STACK_POP(8);
    fprintf(out, "  sw $t8, SPI%dBUF($k1)\n", port);
  }

  return 0;
}

int PIC32::spi_read_I(int port)
{
  CHECK_PORT_SPI();

  need_spi_read = 1;

  fprintf(out, "  ;; spi_read_I()\n");

  if (stack == 0)
  {
    fprintf(out, "  sw $t%d, SPI%dBUF($k1)\n", reg - 1, port);
    reg--;
  }
    else
  {
    STACK_POP(8);
    fprintf(out, "  sw $t8, SPI%dBUF($k1)\n", port);
  }

  fprintf(out, "_spi_read_%d:\n", label_count);
  fprintf(out, "  lw $t8, SPI%dSTAT($k1)\n", port);
  fprintf(out, "  andi $t8, $t8, ((1 << 11) | (1 << 1))\n");
  fprintf(out, "  bnez $t8, _spi_read_%d\n", label_count);
  fprintf(out, "  nop\n");

  label_count++;

  if (reg < reg_max)
  {
    fprintf(out, "  lw $t%d, SPI%dBUF($k1)\n", reg, port);
    reg++;
  }
    else
  {
    fprintf(out, "  lw $t8, SPI%dBUF($k1)\n", port);
    STACK_PUSH(8);
  }

  return 0;
}

int PIC32::spi_read16_I(int port)
{
  CHECK_PORT_SPI();

  need_spi_read = 1;

  fprintf(out, "  ;; spi_read16_I()\n");

  if (stack == 0)
  {
    fprintf(out, "  sw $t%d, SPI%dBUF($k1)\n", reg - 1, port);
    reg--;
  }
    else
  {
    STACK_POP(8);
    fprintf(out, "  sw $t8, SPI%dBUF($k1)\n", port);
  }

  fprintf(out, "_spi_read_%d:\n", label_count);
  fprintf(out, "  lw $t8, SPI%dSTAT($k1)\n", port);
  fprintf(out, "  andi $t8, $t8, (1 << 11)\n");
  fprintf(out, "  bnez $t8, _spi_read_%d\n", label_count);
  fprintf(out, "  nop\n");

  label_count++;

  if (reg < reg_max)
  {
    fprintf(out, "  lw $t%d, SPI%dBUF($k1)\n", reg, port);
    reg++;
  }
    else
  {
    fprintf(out, "  lw $t8, SPI%dBUF($k1)\n", port);
    STACK_PUSH(8);
  }

  return 0;
}

int PIC32::spi_isDataAvailable(int port)
{
  CHECK_PORT_SPI();
  return -1;
}

int PIC32::spi_isBusy(int port)
{
  CHECK_PORT_SPI();
  return -1;
}

int PIC32::spi_disable(int port)
{
  CHECK_PORT_SPI();
  return -1;
}

int PIC32::spi_enable(int port)
{
  CHECK_PORT_SPI();
  return -1;
}

void PIC32::add_spi_read()
{
  fprintf(out, "_spi_read:\n");
  fprintf(out, "  lw $t8, SPI2BUF($k1)\n");
  fprintf(out, "_spi_send_wait:\n");
  fprintf(out, "  lw $t8, SPI2STAT($k1)\n");
  fprintf(out, "  andi $t8, $t8, ((1 << 11) | (1 << 1))\n");
  fprintf(out, "  bne $t8, _spi_read_wait\n");
  fprintf(out, "  lw $t0, SPI2BUF($k1)\n");
  fprintf(out, "  jr $ra\n");
}


