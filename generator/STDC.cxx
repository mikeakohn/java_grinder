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

#include "STDC.h"

#define REG_STACK(a) (reg)
#define LOCALS(i) (i * 4)

// ABI is:
// Nothing.. It's C!

static const char *cond_str[] = { "==", "!=", "<", "<=", ">", ">=" };

STDC::STDC() :
  stack(0),
  is_main(0),
  temp_var(0)
{

}

STDC::~STDC()
{

}

int STDC::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, "#include <stdio.h>\n");
  fprintf(out, "#include <stdlib.h>\n");
  fprintf(out, "#include <stdint.h>\n\n");

  return 0;
}

int STDC::start_init()
{
  // Add any set up items (stack, registers, etc).
  fprintf(out, "\nvoid init()\n");
  fprintf(out, "{\n");

  return 0;
}

int STDC::insert_static_field_define(const char *name, const char *type, int index)
{
  fprintf(out, "static int32_t %s;\n", name);

  return 0;
}


int STDC::init_heap(int field_count)
{
#if 0
int n;

  for (n = 0; n < field_count; n++)
  {
    fprintf(out, "static int32_t global_%d;\n", n);
  }
#endif

  return 0;
}

int STDC::insert_field_init_boolean(char *name, int index, int value)
{
  fprintf(out, "  %s = %d;\n", name, value);

  return 0;
}

int STDC::insert_field_init_byte(char *name, int index, int value)
{
  fprintf(out, "  %s = %d;\n", name, value);

  return 0;
}

int STDC::insert_field_init_short(char *name, int index, int value)
{
  fprintf(out, "  %s = %d;\n", name, value);

  return 0;
}

int STDC::insert_field_init_int(char *name, int index, int value)
{
  fprintf(out, "  %s = %d;\n", name, value);

  return 0;
}

int STDC::insert_field_init(char *name, int index)
{
  fprintf(out, "  %s = (int)_%s;\n", name, name);

  return 0;
}

void STDC::method_start(int local_count, int max_stack, int param_count, const char *name)
{
int n;

  if (strcmp(name, "main") == 0)
  {
    fprintf(out, "}\n\n");
    is_main = true;
  }

  fprintf(out, "int32_t %s(", name);
  for (n = 0; n < param_count; n++)
  {
    if (n != 0) { fprintf(out, ", "); }
    fprintf(out, "int local_%d", n);
  }
  fprintf(out, ")\n");
  fprintf(out, "{\n");

  for (n = 0; n < local_count - param_count; n++)
  {
    fprintf(out, "  xint32_t local_%d;\n", param_count + n);
  }

  for (n = 0; n < max_stack; n++)
  {
    fprintf(out, "  xint32_t stack_%d;\n", n);
  }

  fprintf(out, "\n");

  if (is_main) { fprintf(out, "  init();\n"); }

  stack = 0;
  temp_var = false;
}

void STDC::method_end(int local_count)
{
  fprintf(out, "}\n\n");
}

int STDC::push_integer(int32_t n)
{
  fprintf(out, "  stack_%d = %d;\n", stack++, n);

  return 0;
}

int STDC::push_integer_local(int index)
{
  fprintf(out, "  stack_%d = local_%d;\n", stack++, index);

  return 0;
}

int STDC::push_ref_local(int index)
{
  fprintf(out, "  stack_%d = local_%d;\n", stack++, index);

  return 0;
}

int STDC::push_fake()
{
  stack++;
  return 0;
}

int STDC::set_integer_local(int index, int value)
{
  fprintf(out, "  local_%d = %d;\n", index, value);

  return 0;
}

int STDC::push_long(int64_t n)
{
  return -1;
}

int STDC::push_float(float f)
{
  return -1;
}

int STDC::push_double(double f)
{
  return -1;
}

int STDC::push_byte(int8_t b)
{
  fprintf(out, "  stack_%d = %d;\n", stack++, b);

  return 0;
}

int STDC::push_short(int16_t s)
{
  fprintf(out, "  stack_%d = %d;\n", stack++, s);

  return 0;
}

int STDC::push_ref(char *name)
{
  fprintf(out, "  stack_%d = (uint32_t)%s;\n", stack++, name);

  return 0;
}

int STDC::pop_integer_local(int index)
{
  fprintf(out, "  local_%d = stack_%d;\n", index, --stack);

  return 0;
}

int STDC::pop_ref_local(int index)
{
  fprintf(out, "  local_%d = stack_%d;\n", index, --stack);

  return 0;
}

int STDC::pop()
{
  stack--;

  return 0;
}

int STDC::dup()
{
  fprintf(out, "  stack_%d = stack=%d;\n", stack, stack - 1);
  stack++;

  return 0;
}

int STDC::dup2()
{
  return -1;
}

int STDC::swap()
{
  if (!temp_var)
  {
    fprintf(out, "  int32_t temp;\n");
    temp_var = true;
  }

  fprintf(out, "  temp = stack=%d;\n", stack - 1);
  fprintf(out, "  stack_%d = stack=%d;\n", stack - 1, stack - 2);
  fprintf(out, "  stack_%d = temp;\n", stack - 2);

  return 0;
}

int STDC::add_integer()
{
  fprintf(out, "  stack_%d += stack_%d;\n", stack - 2, stack - 1);
  stack--;

  return 0;
}

int STDC::add_integer(int num)
{
  fprintf(out, "  stack_%d += %d;\n", stack - 1, num);

  return 0;
}

int STDC::sub_integer()
{
  fprintf(out, "  stack_%d -= stack_%d;\n", stack - 2, stack - 1);
  stack--;

  return 0;
}

int STDC::sub_integer(int num)
{
  fprintf(out, "  stack_%d -= %d;\n", stack - 1, num);

  return 0;
}

int STDC::mul_integer()
{
  fprintf(out, "  stack_%d *= stack_%d;\n", stack - 2, stack - 1);
  stack--;

  return 0;
}

int STDC::div_integer()
{
  fprintf(out, "  stack_%d /= stack_%d;\n", stack - 2, stack - 1);
  stack--;

  return 0;
}

int STDC::mod_integer()
{
  fprintf(out, "  stack_%d %%= stack_%d;\n", stack - 2, stack - 1);
  stack--;

  return 0;
}

int STDC::neg_integer()
{
  fprintf(out, "  stack_%d = ~stack_%d;\n", stack - 1, stack - 1);

  return 0;
}

int STDC::shift_left_integer()
{
  fprintf(out, "  stack_%d <<= stack_%d;\n", stack - 2, stack - 1);
  stack--;

  return 0;
}

int STDC::shift_left_integer(int num)
{
  fprintf(out, "  stack_%d <<= %d;\n", stack - 1, num);

  return 0;
}

int STDC::shift_right_integer()
{
  fprintf(out, "  stack_%d >>= stack_%d;\n", stack - 2, stack - 1);
  stack--;

  return 0;
}

int STDC::shift_right_integer(int num)
{
  fprintf(out, "  stack_%d >>= %d;\n", stack - 1, num);

  return 0;
}

int STDC::shift_right_uinteger()
{
  fprintf(out, "  stack_%d = (int32_t)((int32_t)stack_%d >> (uint32_t)stack_%d);\n", stack - 2, stack - 2, stack - 1);
  stack--;

  return 0;
}

int STDC::shift_right_uinteger(int num)
{
  fprintf(out, "  stack_%d = (int32_t)((uint32_t)stack_%d >> (uint32_t)%d);\n", stack - 1, stack - 1, num);

  return 0;
}

int STDC::and_integer()
{
  fprintf(out, "  stack_%d &= stack_%d;\n", stack - 2, stack - 1);
  stack--;

  return 0;
}

int STDC::and_integer(int num)
{
  fprintf(out, "  stack_%d &= %d;\n", stack - 1, num);

  return 0;
}

int STDC::or_integer()
{
  fprintf(out, "  stack_%d |= stack_%d;\n", stack - 2, stack - 1);
  stack--;

  return 0;
}

int STDC::or_integer(int num)
{
  fprintf(out, "  stack_%d |= %d;\n", stack - 1, num);

  return 0;
}

int STDC::xor_integer()
{
  fprintf(out, "  stack_%d ^= stack_%d;\n", stack - 2, stack - 1);
  stack--;

  return 0;
}

int STDC::xor_integer(int num)
{
  fprintf(out, "  stack_%d ^= %d;\n", stack - 1, num);

  return 0;
}

int STDC::inc_integer(int index, int num)
{
  fprintf(out, "  local_%d += %d;\n", index, num);

  return 0;
}

int STDC::integer_to_byte()
{
  fprintf(out, "  stack_%d = (int32_t)((int8_t)stack_%d);\n", stack - 1, stack - 1);

  return 0;
}

int STDC::jump_cond(const char *label, int cond)
{
  fprintf(out, "  if (stack_%d %s 0) { goto %s; }\n", stack--, cond_str[cond], label);

  return 0;
}

int STDC::jump_cond_integer(const char *label, int cond)
{
  fprintf(out, "  if (stack_%d %s stack_%d) { goto %s; }\n", stack - 2, cond_str[cond], stack - 1, label);
  stack -= 2;

  return 0;
}

int STDC::return_local(int index, int local_count)
{
  fprintf(out, "  return local_%d;\n", index);

  return 0;
}

int STDC::return_integer(int local_count)
{
  fprintf(out, "\n  return stack_%d;\n", stack - 1);

  return 0;
}

int STDC::return_void(int local_count)
{
  fprintf(out, "\n  return 0;\n");

  return 0;
}

int STDC::jump(const char *name)
{
  fprintf(out, "  goto %s;\n", name);

  return 0;
}

int STDC::call(const char *name)
{
  //fprintf(out, "  stack_%d = %s();\n", stack++, name);

  return -1;
}

int STDC::invoke_static_method(const char *name, int params, int is_void)
{
int n;

  fprintf(out, "  stack_%d = %s(", stack - params, name);
  for (n = 0; n < params; n++)
  {
    if (n != 0) { fprintf(out, ", "); }
    fprintf(out, "stack_%d", stack - params + n);
  }
  fprintf(out, ");\n");

  stack = stack - params + 1;

  return 0;
}

int STDC::put_static(const char *name, int index)
{
  fprintf(out, "  %s = stack_%d;\n", name, stack++);
  return 0;
}

int STDC::get_static(const char *name, int index)
{
  fprintf(out, "  stack_%d = %s;\n", --stack, name);
  return 0;
}

int STDC::brk()
{
  return -1;
}

int STDC::new_array(uint8_t type)
{
  if (!temp_var)
  {
    fprintf(out, "  int32_t temp;\n");
    temp_var = true;
  }

  fprintf(out, "  temp = stack_%d;\n", stack - 1);

  if (type == TYPE_SHORT || type == TYPE_CHAR)
  {
    fprintf(out, "  stack_%d = (int32_t)malloc(temp * 2 + 4);\n", stack - 1);
  }
    else
  if (type == TYPE_INT)
  {
    fprintf(out, "  stack_%d = (int32_t)malloc(temp * 4 + 4);\n", stack - 1);
  }
    else
  {
    fprintf(out, "  stack_%d = (int32_t)malloc(temp + 4);\n", stack - 1);
  }

  fprintf(out, "  *((int32_t *)stack_%d) = temp;\n", stack - 1);
  fprintf(out, "  stack_%d += 4;\n", stack - 1);

  return 0;
}

int STDC::insert_array(const char *name, int32_t *data, int len, uint8_t type)
{
  return -1;
}

int STDC::insert_string(const char *name, uint8_t *bytes, int len)
{
  return -1;
}

int STDC::push_array_length()
{
  return -1;
}

int STDC::push_array_length(const char *name, int field_id)
{
  fprintf(out, "  stack_%d = ((int32_t *)%s)[-1];\n", stack - 1, name);
  return 0;
}

int STDC::array_read_byte()
{
  return -1;
}

int STDC::array_read_short()
{
  return -1;
}

int STDC::array_read_int()
{
  return -1;
}

int STDC::array_read_byte(const char *name, int field_id)
{
  fprintf(out, "  stack_%d = ((int8_t *)%s)[stack_%d];\n", stack - 1, name, stack - 1);
  return 0;
}

int STDC::array_read_short(const char *name, int field_id)
{
  fprintf(out, "  stack_%d = ((int16_t *)%s)[stack_%d];\n", stack - 1, name, stack - 1);
  return 0;
}

int STDC::array_read_int(const char *name, int field_id)
{
  fprintf(out, "  stack_%d = ((int32_t *)%s)[stack_%d];\n", stack - 1, name, stack - 1);
  return 0;
}

int STDC::array_write_byte()
{
  return -1;
}

int STDC::array_write_short()
{
  return -1;
}

int STDC::array_write_int()
{
  return -1;
}

int STDC::array_write_byte(const char *name, int field_id)
{
  fprintf(out, "  ((int8_t *)%s)[stack_%d] = stack_%d;\n", name, stack - 2, stack - 1);
  stack -= 2;
  return 0;
}

int STDC::array_write_short(const char *name, int field_id)
{
  fprintf(out, "  ((int16_t *)%s)[stack_%d] = stack_%d;\n", name, stack - 2, stack - 1);
  stack -= 2;
  return 0;
}

int STDC::array_write_int(const char *name, int field_id)
{
  fprintf(out, "  ((int32_t *)%s)[stack_%d] = stack_%d;\n", name, stack - 2, stack - 1);
  stack -= 2;
  return 0;
}

