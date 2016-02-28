/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "PIC32.h"

#define CHECK_PORT_SPI() \
  if (port != 0) { printf("Illegal SPI port\n"); return -1; }

PIC32::PIC32()
{
  org = 0x1d001000;
  ram_start = 0x80000000;
  ram_end = 0x80003fff;
  virtual_address = 0x9d000000;
  physical_address = 0x1d000000;
}

PIC32::~PIC32()
{
  //if (need_spi_send) { add_spi_send(); }
}

int PIC32::open(const char *filename)
{
  if (MIPS32::open(filename) != 0) { return -1; }

  fprintf(out, "  SPI1CON equ 0x5800\n");
  fprintf(out, "  SPI1STAT equ 0x5810\n");
  fprintf(out, "  SPI1BUF equ 0x5820\n");
  fprintf(out, "  SPI1BRG equ 0x5830\n");

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
  fprintf(out, "  xori $t%d, $t%d, 0xff\n", reg - 1, reg - 1);
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

  fprintf(out, "  ;; spit_init(%d, %d %d)\n", port, clock_divisor, mode);
  fprintf(out, "  li $t8, 0x%x\n", (1 << 15) | (1 << 5) | (mode << 8));
  fprintf(out, "  sw $t8, SPI1CON($k1)\n");

  fprintf(out, "  li $at, 0xbfc02ff8 ; DEVCFG1\n");
  fprintf(out, "  lw $t9, ~(3 << 12)\n");
  fprintf(out, "  lw $t8, 0($at)\n");
  fprintf(out, "  and $t8, $t8, $t9\n");
  fprintf(out, "  lw $t9, %d << 12\n", clock_divisor);
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

  // bit 15 = ENABLE
  // bit 10-11: 0=8 bit, 1=16 bit, 2=32 bit
  // bit 8 = CKE (clock edge select)
  // bit 6 = CKP (clock polarity select)
  // bit 5 = MSTEN

  fprintf(out, "  ;; spit_init(%d, %d %d)\n", port, clock_divisor, mode);
  fprintf(out, "  li $t8, 0x%x\n", (1 << 15) | (1 << 10) | (1 << 5) | (mode << 8));
  fprintf(out, "  sw $t8, SPI1CON($k1)\n");

  fprintf(out, "  li $at, 0xbfc02ff8 ; DEVCFG1\n");
  fprintf(out, "  lw $t9, ~(3 << 12)\n");
  fprintf(out, "  lw $t8, 0($at)\n");
  fprintf(out, "  and $t8, $t8, $t9\n");
  fprintf(out, "  lw $t9, %d << 12\n", clock_divisor);
  fprintf(out, "  or $t8, $t8, $t9\n");
  fprintf(out, "  sw $t8, 0($at)\n");

  return 0;
}

int PIC32::spi_send_I(int port)
{
  CHECK_PORT_SPI();

#if 0
  add_spi_send = 1;

  fprintf(out, "  addi $sp, $sp, -4\n");
  fprintf(out, "  sw $ra, 0($sp)\n", t);
  fprintf(out, "  jal _spi_send\n");
  fprintf(out, "  nop\n");
  fprintf(out, "  lw $ra, 0($sp)\n", t);
  fprintf(out, "  addi $sp, $sp, 4\n");
#endif

  fprintf(out, "  lw $t8, SPI1STAT($k1)\n");
  fprintf(out, "  andi $t9, $t8, (1 << 11) | (1 << 1)\n");
  fprintf(out, "  bne _spi_send\n");

  return -1;
}

int PIC32::spi_send16_I(int port)
{
  CHECK_PORT_SPI();

#if 0
  add_spi_send = 1;

  fprintf(out, "  addi $sp, $sp, -4\n");
  fprintf(out, "  sw $ra, 0($sp)\n", t);
  fprintf(out, "  jal _spi_send\n");
  fprintf(out, "  nop\n");
  fprintf(out, "  lw $ra, 0($sp)\n", t);
  fprintf(out, "  addi $sp, $sp, 4\n");
#endif

  fprintf(out, "  lw $t8, SPI1STAT($k1)\n");
  fprintf(out, "  andi $t9, $t8, (1 << 11) | (1 << 1)\n");
  fprintf(out, "  bne _spi_send\n");

  return -1;
}

int PIC32::spi_read(int port)
{
  CHECK_PORT_SPI();
  return -1;
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

#if 0
void PIC32::add_spi_send()
{
  fprintf(out, "_spi_send:\n");
  //fprintf(out, "  li $t9, (1 << 11) | (1 << 1)\n");
  //fprintf(out, "_spi_send_wait:\n");
  fprintf(out, "  lw $t8, SPI1STAT($k1)\n");
  fprintf(out, "  andi $t9, $t8, (1 << 11) | (1 << 1)\n");
  fprintf(out, "  bne _spi_send\n");
  fprintf(out, "  jr $ra\n");
}
#endif


