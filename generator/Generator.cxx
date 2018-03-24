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

#include "DSPIC.h"
#include "MSP430.h"
#include "Generator.h"

Generator::Generator() : label_count(0)
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

int Generator::push_local_var_float(int index)
{
  printf("Error: Floats are not supported on this platform.\n");
  return -1;
}

int Generator::set_float_local(int index, float value)
{
  printf("Error: Floats are not supported on this platform.\n");
  return -1;
}

int Generator::push_long(int64_t n)
{
  printf("Error: Longs are not currently supported on this platform.\n");
  return -1;
}

int Generator::push_float(float f)
{
  printf("Error: Floats are not currently supported on this platform.\n");
  return -1;
}

int Generator::push_double(double f)
{
  printf("Error: Doubles are not currently supported on this platform.\n");
  return -1;
}

int Generator::add_float()
{
  printf("Error: Floats are not supported on this platform.\n");
  return -1;
}

int Generator::sub_float()
{
  printf("Error: Floats are not supported on this platform.\n");
  return -1;
}

int Generator::mul_float()
{
  printf("Error: Floats are not supported on this platform.\n");
  return -1;
}

int Generator::compare_floats(int cond)
{
  printf("Error: Floats are not supported on this platform.\n");
  return -1;
}

int Generator::array_read_float()
{
  printf("Error: Floats are not supported on this platform.\n");
  return -1;
}

int Generator::array_read_float(const char *name, int field_id)
{
  printf("Error: Floats are not supported on this platform.\n");
  return -1;
}

int Generator::array_write_float()
{
  printf("Error: Floats are not supported on this platform.\n");
  return -1;
}

int Generator::array_write_float(const char *name, int field_id)
{
  printf("Error: Floats are not supported on this platform.\n");
  return -1;
}

int Generator::new_object(const char *object_name, int field_count)
{
  printf("Error: Object instantiation is not supported on this platform.\n");
  return -1;
}

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

int Generator::insert_dc32(const char *name, int32_t *data, int len, uint8_t len_type, const char *dc32)
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
    fprintf(out, "  %s %d   ; %s.length\n", dc32, len, name);
  }

  fprintf(out, "_%s:\n", name);

  for (n = 0; n < len; n++)
  {
    if ((n % 8) == 0) { fprintf(out, "  %s", dc32); }
    else { fprintf(out, ","); }

    fprintf(out, " 0x%04x", (uint32_t)data[n]);

    if (((n + 1) % 8) == 0) { fprintf(out, "\n"); }
  }

  fprintf(out, "\n\n");

  return 0;
}

int Generator::insert_float(const char *name, int32_t *data, int len, uint8_t len_type, const char *dc32)
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
    fprintf(out, "  %s %d   ; %s.length\n", dc32, len, name);
  }

  fprintf(out, "_%s:\n", name);

  for (n = 0; n < len; n++)
  {
    if ((n % 8) == 0) { fprintf(out, "  %s", dc32); }
    else { fprintf(out, ","); }

    float *a = (float *)&data[n];
    fprintf(out, " %f", *a);

    if (((n + 1) % 8) == 0) { fprintf(out, "\n"); }
  }

  fprintf(out, "\n\n");

  return 0;
}

int Generator::get_constant(uint32_t value)
{
  std::map<uint32_t,int>::iterator it;

  int index;

  it = constants_pool.find(value);

  if (it != constants_pool.end())
  {
    return it->second;
  }

  if (constants_pool.size() < 8192)
  {
    index = constants_pool.size();

    constants_pool[value] = index;

    return index;
  }

  //printf("Error: Constant pool exhausted.\n");

  return -1;
}

void Generator::insert_constants_pool()
{
  uint32_t constants_ordered[8192];
  std::map<uint32_t,int>::iterator it;
  int length = constants_pool.size();
  int n;

  fprintf(out, "_constant_pool:\n");

  for (it = constants_pool.begin(); it != constants_pool.end(); it++)
  {
    constants_ordered[it->second] = it->first;
  }

  for (n = 0; n < length; n++)
  {
    if ((n % 8) == 0) { fprintf(out, "  dc32"); }
    else { fprintf(out, ","); }

    fprintf(out, " 0x%04x", constants_ordered[n]);

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
    if (bytes[n] == '\n')
    {
      fprintf(out, "\\n");
    }
      else
    if (bytes[n] == '\"')
    {
      fprintf(out, "\\\"");
    }
      else
    {
      fprintf(out, "%c", bytes[n]);
    }
  }
  fprintf(out, "\"\n");

  fprintf(out, ".align %d\n\n", get_cpu_byte_alignment() * 8);

  return 0;
}

int Generator::cpu_asm_X(const char *code, int len)
{
  int n;

  for (n = 0; n < len; n++)
  {
    putc(code[n], out);
  }
  return 0;
}


