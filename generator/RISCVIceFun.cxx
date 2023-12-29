/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2023 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "generator/RISCVIceFun.h"

RISCVIceFun::RISCVIceFun()
{
  org = 0x4000;
  ram_start = 0;
  ram_end = 4096;
}

RISCVIceFun::~RISCVIceFun()
{
}

int RISCVIceFun::open(const char *filename)
{
  RISCV::open(filename);

  fprintf(out,
    ";; Registers.\n"
    "PERF_BASE  equ 0x8000\n"
    "BUTTON     equ 0x00\n"
    "SPI_TX     equ 0x04\n"
    "SPI_RX     equ 0x08\n"
    "SPI_CTL    equ 0x0c\n"
    "PORT0      equ 0x20\n"
    "SOUND      equ 0x24\n"
    "PORT1      equ 0x28\n\n");

  fprintf(out,
    ";; Bits in SPI_CTL.\n"
    "SPI_BUSY   equ 0x01\n"
    "SPI_START  equ 0x02\n"
    "SPI_16     equ 0x04\n\n");

  return 0;
}

int RISCVIceFun::ioport_setPinsValue_I(int port)
{
  int peripheral = port == 0 ? 0x20 : 0x28;

  fprintf(out,
    "  ;; setPinsValue(port=%d)\n"
    "  li a1, PERF_BASE\n"
    "  sw t%d, %d(a1)\n",
    port,
    reg - 1, peripheral);

  reg -= 1;

  return 0;
}

int RISCVIceFun::ioport_setPinsValue_I(int port, int const_val)
{
  int peripheral = port == 0 ? 0x20 : 0x28;

  fprintf(out,
    "  ;; setPinsValue(port=%d, const_val=%d)\n"
    "  li a1, PERF_BASE\n"
    "  li a0, %d\n"
    "  sw a0, %d(a1)\n",
    port, const_val,
    const_val,
    peripheral);

  return 0;
}

int RISCVIceFun::spi_send_I(int port)
{
  fprintf(out,
    "  ;; spi_send_I(port=%d)\n"
    "  li a1, PERF_BASE\n"
    "  sw t%d, SPI_TX(a1)\n"
    "  li a0, SPI_START\n"
    "  sw a0, SPI_CTL(a1)\n",
    port,
    reg - 1);

  reg -= 1;

  return 0;
}

int RISCVIceFun::spi_send16_I(int port)
{
  fprintf(out,
    "  ;; spi_send16_I(port=%d)\n"
    "  li a1, PERF_BASE\n"
    "  sw t%d, SPI_TX(a1)\n"
    "  li a0, SPI_16|SPI_START\n"
    "  sw a0, SPI_CTL(a1)\n",
    port,
    reg - 1);

  reg -= 1;

  return 0;
}

int RISCVIceFun::spi_read_I(int port)
{
  fprintf(out,
    "  ;; spi_read_I(port=%d)\n"
    "  li a1, PERF_BASE\n"
    "  li a0, 0\n"
    "  sw a0, SPI_RX(a1)\n"
    "  li a0, SPI_START\n"
    "  sw a0, SPI_CTL(a1)\n"
    "label_%d:\n"
    "  lw a0, SPI_CTL(a1)\n"
    "  andi a0, a0, 1\n"
    "  bnez a0, label_%d\n" 
    "  lw t%d, SPI_RX(a1)\n",
    port,
    label_count,
    label_count,
    reg);

  label_count++;
  reg += 1;

  return 0;
}

int RISCVIceFun::spi_read16_I(int port)
{
  fprintf(out,
    "  ;; spi_read16_I(port=%d)\n"
    "  li a1, PERF_BASE\n"
    "  li a0, 0\n"
    "  sw a0, SPI_RX(a1)\n"
    "  li a0, SPI_START\n"
    "  sw a0, SPI_CTL(a1)\n"
    "label_%d:\n"
    "  lw a0, SPI_CTL(a1)\n"
    "  andi a0, a0, 1\n"
    "  bnez a0, label_%d\n" 
    "  lw t%d, SPI_RX(a1)\n",
    port,
    label_count,
    label_count,
    reg);

  label_count++;
  reg += 1;

  return 0;
}

int RISCVIceFun::spi_isBusy(int port)
{
  fprintf(out,
    "  ;; spi_read16_I(port=%d)\n"
    "  lw t%d, SPI_CTL(a1)\n",
    port,
    reg);

  reg += 1;

  return 0;
}

int RISCVIceFun::ice_fun_setTone_I()
{
  fprintf(out,
    "  ;; ice_fun_setTone_I()\n"
    "  li a1, PERF_BASE\n"
    "  sw t%d, SOUND(a1)\n",
    reg - 1);

  reg -= 1;

  return 0;
}

