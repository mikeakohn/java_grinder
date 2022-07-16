/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2019 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "generator/Generator.h"

Generator::Generator() : label_count(0), preload_array_align(32)
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

void Generator::close()
{
  add_array_files();
}

int Generator::new_object_array(std::string &class_name)
{
  printf("Error: Arrays of 'object' are not supported on this platform.\n");
  return -1;
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

int Generator::div_float()
{
  printf("Error: Floats are not supported on this platform.\n");
  return -1;
}

int Generator::neg_float()
{
  printf("Error: Floats are not supported on this platform.\n");
  return -1;
}

int Generator::float_to_integer()
{
  printf("Error: Floats are not supported on this platform.\n");
  return -1;
}

int Generator::integer_to_float()
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
  printf("Error: Arrays of 'float' are not supported on this platform.\n");
  return -1;
}

int Generator::array_read_long()
{
  printf("Error: Arrays of 'long' are not supported on this platform.\n");
  return -1;
}

int Generator::array_read_object()
{
  printf("Error: Arrays of 'object' are not supported on this platform.\n");
  return -1;
}

int Generator::array_read_float(std::string &name, int field_id)
{
  printf("Error: Arrays of 'float' are not supported on this platform.\n");
  return -1;
}

int Generator::array_read_long(std::string &name, int field_id)
{
  printf("Error: Arrays of 'long' are not supported on this platform.\n");
  return -1;
}

int Generator::array_read_object(std::string &name, int field_id)
{
  printf("Error: Arrays of 'object' are not supported on this platform.\n");
  return -1;
}

int Generator::array_write_float()
{
  printf("Error: Arrays of 'float' are not supported on this platform.\n");
  return -1;
}

int Generator::array_write_long()
{
  printf("Error: Arrays of 'long' are not supported on this platform.\n");
  return -1;
}

int Generator::array_write_object()
{
  printf("Error: Arrays of 'objects' are not supported on this platform.\n");
  return -1;
}

int Generator::array_write_float(std::string &name, int field_id)
{
  printf("Error: Arrays of 'float' are not supported on this platform.\n");
  return -1;
}

int Generator::array_write_long(std::string &name, int field_id)
{
  printf("Error: Arrays of 'long' are not supported on this platform.\n");
  return -1;
}

int Generator::array_write_object(std::string &name, int field_id)
{
  printf("Error: Arrays of 'objects' are not supported on this platform.\n");
  return -1;
}

int Generator::new_object(std::string &object_name, int field_count)
{
  printf("Error: Object instantiation is not supported on this platform.\n");
  return -1;
}

void Generator::label(std::string &name)
{
  fprintf(out, "%s:\n", name.c_str());
}

void Generator::add_newline()
{
  fprintf(out, "\n");
}

int Generator::insert_db(std::string &name, int32_t *data, int len, uint8_t len_type)
{
  if (len_type == TYPE_SHORT)
  {
    fprintf(out, "  dw %d   ; %s.length\n", len, name.c_str());
  }
    else
  if (len_type == TYPE_INT)
  {
    fprintf(out, "  dc32 %d   ; %s.length\n", len, name.c_str());
  }

  fprintf(out, "_%s:\n", name.c_str());

  for (int n = 0; n < len; n++)
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

int Generator::insert_dw(std::string &name, int32_t *data, int len, uint8_t len_type)
{
  if (len_type == TYPE_SHORT)
  {
    fprintf(out, "  dw %d   ; %s.length\n", len, name.c_str());
  }
    else
  if (len_type == TYPE_INT)
  {
    fprintf(out, "  dc32 %d   ; %s.length\n", len, name.c_str());
  }
  fprintf(out, "_%s:\n", name.c_str());

  for (int n = 0; n < len; n++)
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

int Generator::insert_dc32(std::string &name, int32_t *data, int len, uint8_t len_type, const char *dc32)
{
  // FIXME: For dc32, the len_type should be dc32 always.
  if (len_type == TYPE_SHORT)
  {
    fprintf(out, "  dw %d   ; %s.length\n", len, name.c_str());
  }
    else
  if (len_type == TYPE_INT)
  {
    fprintf(out, "  %s %d   ; %s.length\n", dc32, len, name.c_str());
  }

  fprintf(out, "_%s:\n", name.c_str());

  for (int n = 0; n < len; n++)
  {
    if ((n % 8) == 0) { fprintf(out, "  %s", dc32); }
    else { fprintf(out, ","); }

    fprintf(out, " 0x%04x", (uint32_t)data[n]);

    if (((n + 1) % 8) == 0) { fprintf(out, "\n"); }
  }

  fprintf(out, "\n\n");

  return 0;
}

int Generator::insert_float(std::string &name, int32_t *data, int len, uint8_t len_type, const char *dc32)
{
  // FIXME: For dc32, the len_type should be dc32 always.
  if (len_type == TYPE_SHORT)
  {
    fprintf(out, "  dw %d   ; %s.length\n", len, name.c_str());
  }
    else
  if (len_type == TYPE_INT)
  {
    fprintf(out, "  %s %d   ; %s.length\n", dc32, len, name.c_str());
  }

  fprintf(out, "_%s:\n", name.c_str());

  for (int n = 0; n < len; n++)
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

  printf("Error: Constant pool exhausted.\n");

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

int Generator::insert_utf8(std::string &name, uint8_t *bytes, int len)
{
  int n;

  fprintf(out, "_%s:\n", name.c_str());
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

int Generator::use_array_file(const char *filename, const char *array, int type)
{
   ArrayFiles array_file;

  if (preload_arrays.find(filename) != preload_arrays.end()) { return 0; }

  array_file.name = array;
  array_file.type = type;

  preload_arrays[filename] = array_file;

  return 0;
}

int Generator::add_array_files()
{
  struct stat statbuf;
  std::map<std::string, ArrayFiles>::iterator iter;

  const char *constant = "dc32";

  if (get_int_size() == 16)
  {
    constant = "dc16";
  }
  else if (get_int_size() == 8)
  {
    constant = "dc8";
  }

  for (iter = preload_arrays.begin(); iter != preload_arrays.end(); iter++)
  {
    if (stat(iter->first.c_str(), &statbuf) != 0)
    {
      printf("Error opening %s\n", iter->first.c_str());
      return -1;
    }

    if (preload_array_align == 128)
    {
      fprintf(out, ".align 128\n");
      fprintf(out, "  %s 0, 0, 0, %d\n",
        constant,
        (int)(iter->second.type == TYPE_BYTE ?
              statbuf.st_size : statbuf.st_size / get_int_size()));
    }
      else
    {
      fprintf(out, ".align 32\n");
      fprintf(out, "  %s %d\n",
        constant,
        (int)(iter->second.type == TYPE_BYTE ?
              statbuf.st_size : statbuf.st_size / get_int_size()));
    }

    fprintf(out, "_%s:\n", iter->second.name.c_str());
    fprintf(out, ".binfile \"%s\"\n\n", iter->first.c_str());
  }

  return 0;
}

