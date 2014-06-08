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
#include <stdint.h>

#include "DSPIC.h"
#include "MSP430.h"
#include "Generator.h"

Generator::Generator() : label_count(0), constants_len(0)
{
}

Generator::~Generator()
{
  fprintf(out, "\n");
  fclose(out);
}

int Generator::open(const char *filename)
{
  out = fopen(filename, "wb");

  if (out == NULL)
  {
    printf("Couldn't open file %s for writing.\n", filename);
    return -1;
  }

  return 0;
}

#if 0
void Generator::close()
{
}
#endif

void Generator::label(char *name)
{
  fprintf(out, "%s:\n", name);
}

void Generator::add_newline()
{
  fprintf(out, "\n");
}

int Generator::insert_db(const char *name, int32_t *data, int len, uint8_t len_type)
{
int n;

  if (len_type == TYPE_SHORT)
  {
    fprintf(out, "  dw %d   ; %s.length\n", len, name);
  }
    else
  if (len_type == TYPE_INT)
  {
    fprintf(out, "  dc32 %d   ; %s.length\n", len, name);
  }
  fprintf(out, "_%s:\n", name);

  for (n = 0; n < len; n++)
  {
    if ((n % 8) == 0) { fprintf(out, "  db"); }
    else { fprintf(out, ","); }

    if (data[n] < -128 || data[n] > 255)
    {
      printf("Array element out of 8 bit range: %d\n", data[n]);
      return -1;
    }

    fprintf(out, " 0x%02x", (uint8_t)data[n]);

    if (((n + 1) % 8) == 0) { fprintf(out, "\n"); }
  }

  fprintf(out, "\n\n");
  return 0;
}

int Generator::insert_dw(const char *name, int32_t *data, int len, uint8_t len_type)
{
int n;

  if (len_type == TYPE_SHORT)
  {
    fprintf(out, "  dw %d   ; %s.length\n", len, name);
  }
    else
  if (len_type == TYPE_INT)
  {
    fprintf(out, "  dc32 %d   ; %s.length\n", len, name);
  }
  fprintf(out, "_%s:\n", name);

  for (n = 0; n < len; n++)
  {
    if ((n % 8) == 0) { fprintf(out, "  dw"); }
    else { fprintf(out, ","); }

    if (data[n] < -32768 || data[n] > 65535)
    {
      printf("Array element out of 16 bit range: %d\n", data[n]);
      return -1;
    }

    fprintf(out, " 0x%04x", (uint16_t)data[n]);

    if (((n + 1) % 8) == 0) { fprintf(out, "\n"); }
  }

  fprintf(out, "\n\n");

  return 0;
}

int Generator::insert_dc32(const char *name, int32_t *data, int len, uint8_t len_type)
{
int n;

  // FIXME: For dc32, the len_type should be dc32 always.
  if (len_type == TYPE_SHORT)
  {
    fprintf(out, "  dw %d   ; %s.length\n", len, name);
  }
    else
  if (len_type == TYPE_INT)
  {
    fprintf(out, "  dc32 %d   ; %s.length\n", len, name);
  }

  fprintf(out, "_%s:\n", name);

  for (n = 0; n < len; n++)
  {
    if ((n % 8) == 0) { fprintf(out, "  dc32"); }
    else { fprintf(out, ","); }

    fprintf(out, " 0x%04x", (uint32_t)data[n]);

    if (((n + 1) % 8) == 0) { fprintf(out, "\n"); }
  }

  fprintf(out, "\n\n");

  return 0;
}

int Generator::get_constant(uint32_t constant)
{
int n;

  for (n = 0; n < constants_len; n++)
  {
    if (constants[n] == constant) { return n; }
  }

  if (constants_len < (int)(sizeof(constants) / sizeof(uint32_t)))
  {
    constants[constants_len++] = constant;
    return n;
  }

  printf("Error: Constant pool exhausted.\n");

  return 0;
}

void Generator::write_constants()
{
int n;

  if (constants_len == 0) { return; }

  fprintf(out, "_constants:\n");

  for (n = 0; n < constants_len; n++)
  {
    if ((n % 8) == 0) { fprintf(out, "  dc32"); }
    else { fprintf(out, ","); }

    fprintf(out, " 0x%04x", constants[n]);

    if (((n + 1) % 8) == 0) { fprintf(out, "\n"); }
  }

  fprintf(out, "\n\n");
}

int Generator::insert_utf8(const char *name, uint8_t *bytes, int len)
{
int n;

  fprintf(out, "_%s:\n", name);
  fprintf(out, "  db \"");
  for (n = 0; n < len; n++)
  {
    fprintf(out, "%c", bytes[n]);
  }
  fprintf(out, "\"\n");
  fprintf(out, "  db 0\n");

  return 0;
}

int Generator::cpu_asm(const char *code, int len)
{
int n;

  for (n = 0; n < len; n++)
  {
    putc(code[n], out);
  }
  return 0;
}


