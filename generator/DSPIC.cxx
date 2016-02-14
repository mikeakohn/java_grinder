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

#define __STDC_FORMAT_MACROS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#include "DSPIC.h"

#define REG_STACK(a) (stack_regs[a])
#define LOCALS(i) (i * 2)

// ABI is:
// w0 temp, return value from method call
// w1 temp
// w4 start of stack
// w5 ..
// w11 ..
// w2 ..
// w3 ..
// w8 ..
// w9 ..
// w10 end of stack
// w6 temp DSP, array ptr
// w7 temp DSP, array index
// w13 temp
// w14 pointer to locals
//
// Stack on dsPIC moves value to [sp] and then increments sp by 2 (odd).

static const char *cond_str[] = { "z", "nz", "lt", "le", "gt", "ge" };
static int8_t stack_regs[] = { 5, 4, 11, 2, 3, 8, 9, 10 };

DSPIC::DSPIC(uint8_t chip_type) :
  reg(0),
  reg_max(sizeof(stack_regs)),
  stack(0),
  is_main(false),
  need_stack_set(false),
  need_tables(false)
{
  this->chip_type = chip_type;
}

DSPIC::~DSPIC()
{
  fprintf(out, ".org __FICD\n");
  fprintf(out, "  dc32 0xffcf\n\n");
}

int DSPIC::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  // For now we only support a specific chip
  fprintf(out, ".dspic\n");

  switch(chip_type)
  {
    case DSPIC30F3012:
      fprintf(out, ".include \"p30f3012.inc\"\n\n");
      //flash_start = 0x100;
      break;
    case DSPIC33FJ06GS101A:
      fprintf(out, ".include \"p33fj06gs101a.inc\"\n\n");
      //flash_start = 0x100;
      need_stack_set = true;
      break;
    default:
      printf("Unknown chip type.\n");
  }

  // Set where RAM starts / ends
  fprintf(out, "ram_start equ __DATA_BASE\n");
  fprintf(out, "ram_end equ __DATA_BASE+__DATA_LENGTH\n");

  return 0;
}

int DSPIC::start_init()
{
  fprintf(out, ".org 0\n");
  fprintf(out, "  goto start\n\n");

  // Add any set up items (stack, registers, etc).  On some CPU's (dsPICF3012)
  // SP is automatically set but some it's not.  So for now we'll just set it.
  // Also, not sure what to do with SPLIM.
  fprintf(out, ".org __CODE_BASE\n");
  fprintf(out, "start:\n");
  if (need_stack_set) { fprintf(out, "  mov #__DATA_BASE, SP\n\n"); }
  if (need_tables)
  {
    fprintf(out, "  ;; For reading out of flash, need table instructions\n");
    fprintf(out, "  mov #0, w1\n");
    fprintf(out, "  mov w1, TBLPAG\n\n");
  }

  return 0;
}

int DSPIC::insert_static_field_define(const char *name, const char *type, int index)
{
  if (type[0] == '[') { need_tables = true; }
  fprintf(out, "%s equ ram_end-%d\n", name, (index + 2) * 2);

  return 0;
}

int DSPIC::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");
  fprintf(out, "  mov #ram_end-%d, w0\n", (field_count + 2) * 2);
  fprintf(out, "  mov w0, ram_end-2\n");

  return 0;
}

int DSPIC::field_init_int(char *name, int index, int value)
{
  if (value < -32768 || value > 65535) { return -1; }
  if (value == 0)
  {
    fprintf(out, "  clr %s\n", name);
  }
    else
  {
    fprintf(out, "  mov #%d, w0\n", value);
    fprintf(out, "  mov w0, %s\n", name);
  }
  return 0;
}

int DSPIC::field_init_ref(char *name, int index)
{
  fprintf(out, "  mov #_%s, w0\n", name);
  fprintf(out, "  mov w0, %s\n", name);
  return 0;
}

void DSPIC::method_start(int local_count, int max_stack, int param_count, const char *name)
{
  reg = 0;
  stack = 0;

  is_main = (strcmp(name, "main") == 0) ? true : false;

  // main() function goes here
  fprintf(out, "%s:\n", name);
  if (!is_main)
  {
    //fprintf(out, "  push w14\n");
    //fprintf(out, "  mov sp, w14\n");
    //fprintf(out, "  add #0x%x, sp\n", local_count * 2);
    if (local_count != 0) { fprintf(out, "  lnk #0x%x\n", local_count * 2); }
  }
    else
  {
    fprintf(out, "  mov sp, w14\n");
    fprintf(out, "  add #0x%x, sp\n", local_count * 2);
  }
}

void DSPIC::method_end(int local_count)
{
  //fprintf(out, "  add #0x%x, sp\n", local_count * 2);
  //fprintf(out, "  ret\n\n");
  fprintf(out, "\n");
}

int DSPIC::push_local_var_int(int index)
{
  if (reg < reg_max)
  {
    //fprintf(out, "  mov [w14+%d], w0\n", LOCALS(index));
    //fprintf(out, "  mov w0, w%d\n", REG_STACK(reg));
    fprintf(out, "  mov [w14+%d], w%d\n", LOCALS(index), REG_STACK(reg));
    reg++;
  }
    else
  {
    //fprintf(out, "  mov [w14+%d], w0\n", LOCALS(index));
    //fprintf(out, "  push w0\n");
    fprintf(out, "  push [w14+%d]\n", LOCALS(index));
    stack++;
  }

  return 0;
}

int DSPIC::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int DSPIC::push_ref_static(const char *name, int index)
{
  return -1;
}

int DSPIC::push_fake()
{
  if (stack != 0) { return -1; }
  reg++;
  return 0;
}

int DSPIC::push_int(int32_t n)
{
  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);
    return -1;
  }

  uint16_t value = (n & 0xffff);

  if (reg < reg_max)
  {
    fprintf(out, "  mov #0x%02x, w%d\n", value, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  mov #0x%02x, w0\n", value);
    fprintf(out, "  push w0\n");
    stack++;
  }

  return 0;
}

int DSPIC::push_long(int64_t n)
{
  printf("long is not supported right now\n");
  return -1;
}

int DSPIC::push_float(float f)
{
  printf("float is not supported right now\n");
  return -1;
}

int DSPIC::push_double(double f)
{
  printf("double is not supported right now\n");
  return -1;
}

int DSPIC::push_byte(int8_t b)
{
  uint16_t value = ((int32_t)b)&0xffff;

  if (reg < reg_max)
  {
    fprintf(out, "  mov #0x%02x, w%d\n", value, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  mov #0x%02x, w0\n", value);
    fprintf(out, "  push w0\n");
    stack++;
  }
  return 0;
}

int DSPIC::push_short(int16_t s)
{
  uint16_t value = s;

  if (reg < reg_max)
  {
    fprintf(out, "  mov #0x%02x, w%d\n", value, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  mov #0x%02x, w0\n", value);
    fprintf(out, "  push w0\n");
    stack++;
  }
  return 0;
}

int DSPIC::push_ref(char *name)
{
  if (reg < reg_max)
  {
    fprintf(out, "  mov #%s, w0\n", name);
    fprintf(out, "  mov [w0], w%d\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  mov #%s, w0\n", name);
    fprintf(out, "  push [w0]\n");
    stack++;
  }
  return 0;
}

int DSPIC::pop_local_var_int(int index)
{
  if (stack > 0)
  {
    fprintf(out, "  pop w0\n");
    fprintf(out, "  mov.w w0, [w14+%d]\n", LOCALS(index));
    stack--;
  }
    else
  if (reg > 0)
  {
    fprintf(out, "  mov.w w%d, [w14+%d]\n", REG_STACK(reg-1), LOCALS(index));
    reg--;
  }

  return 0;
}

int DSPIC::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int DSPIC::pop()
{
  if (stack > 0)
  {
    fprintf(out, "  pop w0\n");
    stack--;
  }
    else
  if (reg > 0)
  {
    reg--;
  }

  return 0;
}

int DSPIC::dup()
{
  if (stack > 0)
  {
    fprintf(out, "  push [SP]\n");
    stack++;
  }
    else
  if (reg == reg_max)
  {
    fprintf(out, "  push w%d\n", REG_STACK(reg-1));
    stack++;
  }
    else
  {
    fprintf(out, "  mov.w w%d, w%d\n", REG_STACK(reg-1), REG_STACK(reg));
    reg++;
  }

  return 0;
}

int DSPIC::dup2()
{
  printf("Need to implement dup2()\n");
  return -1;
}

int DSPIC::swap()
{
  if (stack == 0)
  {
    fprintf(out, "  mov.w w%d, w0\n", REG_STACK(reg-1));
    fprintf(out, "  mov.w w%d, w%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
    fprintf(out, "  mov.w w0, w%d\n", REG_STACK(reg-2));
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  mov.w w%d, w0\n", REG_STACK(reg-1));
    fprintf(out, "  mov.w [SP-2], w%d\n", REG_STACK(reg-1));
    fprintf(out, "  mov.w w0, [SP-2]\n");
  }
    else
  {
    fprintf(out, "  mov.w [SP-4], w0\n");
    fprintf(out, "  mov.w [SP-2], w13\n");
    fprintf(out, "  mov.w w13, [SP-4]\n");
    fprintf(out, "  mov.w w0, [SP-2]\n");
  }

  return 0;
}

int DSPIC::add_integer()
{
  return stack_alu("add");
}

int DSPIC::sub_integer()
{
  return stack_alu("sub");
}

int DSPIC::mul_integer()
{
  return stack_alu("mul");
}

int DSPIC::div_integer()
{
  stack_alu_div();

  // Result of divide is in w0
  if (stack > 0) { fprintf(out, "  mov.w w0, [SP-2]\n"); }
  else { fprintf(out, "  mov.w w0, w%d\n", REG_STACK(reg-1)); }

  return 0;
}

int DSPIC::mod_integer()
{
  stack_alu_div();

  // Remainder of divide is in w1
  if (stack > 0) { fprintf(out, "  mov.w w1, [SP-2]\n"); }
  else { fprintf(out, "  mov.w w1, w%d\n", REG_STACK(reg-1)); }

  return 0;
}

int DSPIC::neg_integer()
{
  if (stack > 0)
  {
    fprintf(out, "  mov.w [SP-2], w0\n");
    fprintf(out, "  neg.w w0, w0\n");
    fprintf(out, "  mov.w w0, [SP-2]\n");
    stack--;
  }
    else
  {
    fprintf(out, "  neg.w w%d, w%d\n", REG_STACK(reg-1), REG_STACK(reg-1));
  }

  return 0;
}

int DSPIC::shift_left_integer()
{
  return stack_shift("sl");
}

int DSPIC::shift_left_integer(int count)
{
  if (stack == 0) { return -1; }
  fprintf(out, "  sl w%d, #%d, w%d\n", REG_STACK(reg-1), count, REG_STACK(reg-1));

  return 0;
}

int DSPIC::shift_right_integer()
{
  return stack_shift("asr");
}

int DSPIC::shift_right_integer(int count)
{
  if (stack == 0) { return -1; }
  fprintf(out, "  asr w%d, #%d, w%d\n", REG_STACK(reg-1), count, REG_STACK(reg-1));

  return 0;
}

int DSPIC::shift_right_uinteger()
{
  return stack_shift("lsr");
}

int DSPIC::shift_right_uinteger(int count)
{
  if (stack == 0) { return -1; }
  fprintf(out, "  lsr w%d, #%d, w%d\n", REG_STACK(reg-1), count, REG_STACK(reg-1));

  return 0;
}

int DSPIC::and_integer()
{
  return stack_alu("and");
}

int DSPIC::or_integer()
{
  return stack_alu("ior");
}

int DSPIC::xor_integer()
{
  return stack_alu("xor");
}

int DSPIC::inc_integer(int index, int num)
{
//int8_t n = (int8_t)num;

  fprintf(out, "  mov [w14+%d], w0\n", LOCALS(index));
  if (num >= 0)
  {
    fprintf(out, "  add #%d, w0\n", num);
  }
    else
  {
    fprintf(out, "  sub #%d, w0\n", -num);
  }

  fprintf(out, "  mov w0, [w14+%d]\n", LOCALS(index));

  return 0;
}

int DSPIC::integer_to_byte()
{
  if (stack == 0)
  {
    fprintf(out, "  pop w0\n");
    fprintf(out, "  se w0, w0\n");
    fprintf(out, "  push w0\n");
  }
    else
  {
    fprintf(out, "  se w%d, w%d\n", REG_STACK(reg-1), REG_STACK(reg-1));
  }

  return 0;
}

int DSPIC::integer_to_short()
{
  // I think we do nothing here.  On dsPIC, integers are already 16 bit.
  return 0;
}

int DSPIC::jump_cond(const char *label, int cond, int distance)
{
  if (stack > 0)
  {
    //fprintf(out, "  mov [SP-2], w0\n");
    fprintf(out, "  pop w0\n");
    fprintf(out, "  cp0 w0\n");
    stack--;
  }
    else
  {
    fprintf(out, "  cp0 w%d\n", REG_STACK(reg-1));
    reg--;
  }

  fprintf(out, "  bra %s, %s\n", cond_str[cond], label);
  return 0;
}

int DSPIC::jump_cond_integer(const char *label, int cond, int distance)
{
  if (stack > 1)
  {
    fprintf(out, "  mov [SP-2], w0\n");
    fprintf(out, "  mov [SP-4], w13\n");
    //fprintf(out, "  cmp.w 2(SP), 4(SP)\n");
    //fprintf(out, "  cp w0, w13\n");
    fprintf(out, "  cp w13, w0\n");
    stack -= 2;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  mov [SP-2], w0\n");
    fprintf(out, "  cp w%d, w0\n", REG_STACK(reg-1));
    stack--;
    reg--;
  }
    else
  {
    fprintf(out, "  cp w%d, w%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
    reg -= 2;
  }

  fprintf(out, "  bra %s, %s\n", cond_str[cond], label);

  return 0;
}

int DSPIC::return_local(int index, int local_count)
{
#if 0
  fprintf(out, "  mov [w14-#%d], w0\n", LOCALS(index));
  //fprintf(out, "  add #0x%x, sp\n", local_count * 2);
  fprintf(out, "  mov w14, sp\n");
  fprintf(out, "  ret\n");

  return 0;
#endif
  return -1;
}

int DSPIC::return_integer(int local_count)
{
  if (stack > 0)
  {
    fprintf(out, "  mov [sp-2], w0\n");
  }
    else 
  {
    fprintf(out, "  mov w%d, w0\n", REG_STACK(reg - 1));
  }

  if (local_count != 0) { fprintf(out, "  ulnk\n"); }
  //fprintf(out, "  mov w14, sp\n");
  //if (!is_main) { fprintf(out, "  pop w14\n"); }
  fprintf(out, "  return\n");
  return 0;
}

int DSPIC::return_void(int local_count)
{
  //fprintf(out, "  mov w14, sp\n");
  //if (!is_main) { fprintf(out, "  pop w14\n"); }
  if (local_count != 0) { fprintf(out, "  ulnk\n"); }
  fprintf(out, "  return\n");

  return 0;
}

int DSPIC::jump(const char *name, int distance)
{
  fprintf(out, "  bra %s\n", name);
  return 0;
}

int DSPIC::call(const char *name)
{
  fprintf(out, "  call %s\n", name);
  return 0;
}

int DSPIC::invoke_static_method(const char *name, int params, int is_void)
{
int local;
int stack_vars = stack;
int reg_vars = reg;
int saved_registers;
int n;

  printf("invoke_static_method() name=%s params=%d is_void=%d\n", name, params, is_void);

  // Push all used registers on the stack except the ones that are pulled
  // out for parameters.
  saved_registers = reg - (params - stack);
  for (n = 0; n < saved_registers; n++)
  {
    fprintf(out, "  push w%d\n", REG_STACK(n));
  }

  // Copy parameters onto the stack so they are local variables in
  // the called method.  Start with -4 because the return address will
  // be at 0 and w14 will be at 2.
  local = (params * 2);
  while(local > 0)
  {
    if (stack_vars > 0)
    {
      fprintf(out, "  mov [SP-%d], w0\n", (stack-(stack_vars-1)) * 2);
      fprintf(out, "  mov w0, [SP+%d]\n", local+4);
      stack_vars--;
    }
      else
    {
      fprintf(out, "  mov w%d, [SP+%d]\n", REG_STACK(reg_vars-1), local+4);
      reg_vars--;
    }

    local -= 2;
  }

  // Make the call
  fprintf(out, "  call %s\n", name);

  // Pop all used registers off the stack
  for (n = saved_registers-1; n >= 0; n--)
  {
    fprintf(out, "  pop w%d\n", REG_STACK(n));
  }

  // Pop all params off the Java stack
  if ((stack - stack_vars) > 0)
  {
    fprintf(out, "  sub #%d, SP\n", (stack - stack_vars) * 2);
    params -= (stack - stack_vars);
  }

  if (params != 0)
  {
    reg -= params;
  }

  if (!is_void)
  {
    // Put r15 on the top of the stack
    if (reg < reg_max)
    {
      fprintf(out, "  mov.w w0, w%d\n", REG_STACK(reg));
      reg++;
    }
      else
    {
      // REVIEW - This looks wrong
      fprintf(out, "  push w0\n");
      stack++;
    }
  }

  return 0;
}

int DSPIC::put_static(const char *name, int index)
{
  return -1;
}

int DSPIC::get_static(const char *name, int index)
{
  return -1;
}

int DSPIC::brk()
{
  return -1;
}

int DSPIC::new_array(uint8_t type)
{
  return -1;
}

int DSPIC::insert_array(const char *name, int32_t *data, int len, uint8_t type)
{
  fprintf(out, ".align 16\n");

  if (type == TYPE_BYTE)
  {
    return insert_db(name, data, len, TYPE_SHORT);
  }
    else
  if (type == TYPE_SHORT)
  { return insert_dw(name, data, len, TYPE_SHORT); }
    else
  if (type == TYPE_INT)
  { return insert_dw(name, data, len, TYPE_SHORT); }

  return -1;
}

int DSPIC::insert_string(const char *name, uint8_t *bytes, int len)
{
  fprintf(out, ".align 16\n");
  fprintf(out, "  dw %d\n", len);
  return insert_utf8(name, bytes, len);
}

int DSPIC::push_array_length()
{
  if (stack > 0)
  {
    fprintf(out, "  pop w0\n");
    fprintf(out, "  mov [w0-2], w0\n");
    fprintf(out, "  push w0\n");
  }
    else
  {
    fprintf(out, "  mov.w [w%d-2], w0\n", REG_STACK(reg-1));
    fprintf(out, "  mov.w w0, [w%d]\n", REG_STACK(reg-1));
  }

  return 0;
}

int DSPIC::push_array_length(const char *name, int field_id)
{
  fprintf(out, "  mov %s, w13\n", name);

  if (reg < reg_max)
  {
    fprintf(out, "  tblrdl [w13-2], w%d\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  push [w13-2]\n");
    stack++;
  }

  return 0;
}

int DSPIC::array_read_byte()
{
int index_reg;
int ref_reg;

  get_values_from_stack(&index_reg, &ref_reg);
  fprintf(out, "  sl w%d\n", index_reg);
  fprintf(out, "  add w%d, w%d\n", index_reg, ref_reg);
  if (reg < reg_max)
  {
    fprintf(out, "  mov.b [w%d], w%d\n", ref_reg, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  push [w%d]\n", ref_reg);
    stack++;
  }

  return 0;
}

int DSPIC::array_read_short()
{
int index_reg;
int ref_reg;

  get_values_from_stack(&index_reg, &ref_reg);
  fprintf(out, "  sl w%d\n", index_reg);
  fprintf(out, "  add w%d, w%d\n", index_reg, ref_reg);
  if (reg < reg_max)
  {
    fprintf(out, "  mov [w%d], w%d\n", ref_reg, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  push [w%d]\n", ref_reg);
    stack++;
  }

  return 0;
}

int DSPIC::array_read_int()
{
  return array_read_short();
}

int DSPIC::array_read_byte(const char *name, int field_id)
{
  fprintf(out, "  mov %s, w6\n", name);

  if (stack > 0)
  {
    fprintf(out, "  pop w7\n");
    fprintf(out, "  add w7, w6, w6\n");
    fprintf(out, "  mov [w6], w0\n");
    fprintf(out, "  se w0, w0\n");
    fprintf(out, "  push w0\n");
  }
    else
  {
    fprintf(out, "  add w%d, w6, w6\n", REG_STACK(reg-1));
    fprintf(out, "  mov [w6], w%d\n", REG_STACK(reg-1));
    fprintf(out, "  se w%d, w%d\n", REG_STACK(reg-1), REG_STACK(reg-1));
  }

  return 0;
}

int DSPIC::array_read_short(const char *name, int field_id)
{
  fprintf(out, "  mov %s, w6\n", name);

  if (stack > 0)
  {
    fprintf(out, "  pop w7\n");
    fprintf(out, "  sl w7\n");
    fprintf(out, "  add w7, w6, w6\n");
    fprintf(out, "  push [w6]\n");
  }
    else
  {
    fprintf(out, "  sl w%d\n", REG_STACK(reg-1));
    fprintf(out, "  add w%d, w6, w6\n", REG_STACK(reg-1));
    fprintf(out, "  mov [w6], w%d\n", REG_STACK(reg-1));
  }

  return 0;
}

int DSPIC::array_read_int(const char *name, int field_id)
{
  return array_read_short(name, field_id);
}

int DSPIC::array_write_byte()
{
int value_reg;
int index_reg;
int ref_reg;

  get_values_from_stack(&value_reg, &index_reg, &ref_reg);
  fprintf(out, "  sl w%d\n", index_reg);
  fprintf(out, "  add w%d, w%d, w%d\n", index_reg, ref_reg, ref_reg);
  fprintf(out, "  mov.b w%d, [w%d]\n", value_reg, ref_reg);

  return 0;
}

int DSPIC::array_write_short()
{
int value_reg;
int index_reg;
int ref_reg;

  get_values_from_stack(&value_reg, &index_reg, &ref_reg);
  fprintf(out, "  sl w%d\n", index_reg);
  fprintf(out, "  add w%d, w%d, w%d\n", index_reg, ref_reg, ref_reg);
  fprintf(out, "  mov w%d, [w%d]\n", value_reg, ref_reg);

  return 0;
}

int DSPIC::array_write_int()
{
  return array_write_short();
}

int DSPIC::array_write_byte(const char *name, int field_id)
{
int value_reg;
int index_reg;

  get_values_from_stack(&value_reg, &index_reg);

  fprintf(out, "  sl w%d\n", index_reg);
  fprintf(out, "  add %s, w%d\n", name, index_reg);
  fprintf(out, "  mov.b w%d, [w%d]\n", value_reg, index_reg);

  return 0;
}

int DSPIC::array_write_short(const char *name, int field_id)
{
int value_reg;
int index_reg;

  get_values_from_stack(&value_reg, &index_reg);

  fprintf(out, "  sl w%d\n", index_reg);
  fprintf(out, "  add %s, w%d\n", name, index_reg);
  fprintf(out, "  mov w%d, [w%d]\n", value_reg, index_reg);

  return 0;
}

int DSPIC::array_write_int(const char *name, int field_id)
{
  return array_write_short(name, field_id);
}


#if 0
void DSPIC::close()
{
}
#endif

// GPIO functions
int DSPIC::ioport_setPinsAsInput(int port)
{
  char periph[32];
  sprintf(periph, "TRIS%c", port+'A');
  return set_periph("ior", periph);
}

int DSPIC::ioport_setPinsAsInput(int port, int const_val)
{
  char periph[32];
  sprintf(periph, "TRIS%c", port+'A');

  int pin = get_pin_number(const_val);
  if (pin == -1)
  {
    fprintf(out, "  mov #0x%04x, w0\n", const_val);
    fprintf(out, "  ior %s\n", periph);
    return 0;
  }

  fprintf(out, "  bset %s, #%d\n", periph, pin);

  return 0;
}

int DSPIC::ioport_setPinsAsOutput(int port)
{
  char periph[32];
  sprintf(periph, "TRIS%c", port+'A');
  return set_periph("and", periph, true);
}

int DSPIC::ioport_setPinsAsOutput(int port, int const_val)
{
  char periph[32];
  sprintf(periph, "TRIS%c", port+'A');

  int pin = get_pin_number(const_val);
  if (pin == -1)
  {
    fprintf(out, "  mov #0x%04x, w0\n", const_val^0xffff);
    fprintf(out, "  and %s\n", periph);
    return 0;
  }

  fprintf(out, "  bclr %s, #%d\n", periph, pin);

  return 0;
}

int DSPIC::ioport_setPinsValue(int port)
{
  char periph[32];
  sprintf(periph, "LAT%c", port+'A');

  if (stack == 0)
  {
    fprintf(out, "  mov w%d, %s\n", REG_STACK(reg-1), periph);
    reg--;
  }
    else
  {
    fprintf(out, "  pop w0\n");
    fprintf(out, "  mov %s\n", periph);
    stack--;
  }

  return 0;
}

int DSPIC::ioport_setPinsHigh(int port)
{
  char periph[32];
  sprintf(periph, "LAT%c", port+'A');
  return set_periph("ior", periph);
}

int DSPIC::ioport_setPinsHigh(int port, int const_val)
{
  char periph[32];
  sprintf(periph, "LAT%c", port+'A');

  int pin = get_pin_number(const_val);
  if (pin == -1)
  {
    fprintf(out, "  mov #0x%04x, w0\n", const_val);
    fprintf(out, "  ior %s\n", periph);
    return 0;
  }

  fprintf(out, "  bset %s, #%d\n", periph, pin);

  return 0;
}

int DSPIC::ioport_setPinsLow(int port)
{
  char periph[32];
  sprintf(periph, "LAT%c", port+'A');
  return set_periph("and", periph, true);
}

int DSPIC::ioport_setPinsLow(int port, int const_val)
{
  char periph[32];
  sprintf(periph, "LAT%c", port+'A');

  int pin = get_pin_number(const_val);
  if (pin == -1)
  {
    fprintf(out, "  mov #0x%04x, w0\n", const_val^0xffff);
    fprintf(out, "  and %s\n", periph);
    return 0;
  }

  fprintf(out, "  bclr %s, #%d\n", periph, pin);

  return 0;
}

int DSPIC::ioport_setPinAsOutput(int port)
{
  return -1;
}

int DSPIC::ioport_setPinAsOutput(int port, int const_val)
{
  if (const_val < 0 || const_val > 15) { return -1; }
  fprintf(out, "  bclr TRIS%c, #%d\n", port+'A', const_val);
  return 0;
}

int DSPIC::ioport_setPinAsInput(int port)
{
  return -1;
}

int DSPIC::ioport_setPinAsInput(int port, int const_val)
{
  if (const_val < 0 || const_val > 15) { return -1; }
  fprintf(out, "  bset TRIS%c, #%d\n", port+'A', const_val);
  return 0;
}

int DSPIC::ioport_setPinHigh(int port)
{
  return -1;
}

int DSPIC::ioport_setPinHigh(int port, int const_val)
{
  if (const_val < 0 || const_val > 15) { return -1; }
  fprintf(out, "  bset LAT%c, #%d\n", port+'A', const_val);
  return 0;
}

int DSPIC::ioport_setPinLow(int port)
{
  return -1;
}

int DSPIC::ioport_setPinLow(int port, int const_val)
{
  if (const_val < 0 || const_val > 15) { return -1; }
  fprintf(out, "  bclr LAT%c, #%d\n", port+'A', const_val);
  return 0;
}

int DSPIC::ioport_isPinInputHigh(int port)
{
  return -1;
}

int DSPIC::ioport_getPortInputValue(int port)
{
  return -1;
}

// SPI functions
int DSPIC::spi_init(int port)
{
char dst[16];

  if (port != 0) { return -1; }

  fprintf(out, "  ;; Set up SPI\n");
  // This chip needs the RP pins set.
  if (chip_type == DSPIC33FJ06GS101A)
  {
    fprintf(out, "  ; SDI is on RP2\n");
    fprintf(out, "  mov #SDI1R2, w0\n");
    fprintf(out, "  mov w0, RPINR20\n");

    fprintf(out, "  ; SDO is on RP3\n");
    fprintf(out, "  mov #(0x7<<8), w0\n");
    fprintf(out, "  mov w0, RPOR1     ; controls RP2, RP3\n");

    fprintf(out, "  ; SCLK is on RP1\n");
    fprintf(out, "  mov #(0x8<<8), w0\n");
    fprintf(out, "  mov w0, RPOR0     ; controls RP0, RP1\n");
  }

  fprintf(out, "  mov #(1<<MSTEN), w0\n");
  fprintf(out, "  mov w0, SPI1CON1\n");
  pop_reg(dst);
  if (strcmp(dst, "w0") == 0)
  {
    fprintf(out, "  mov w0, w13\n");
    strcpy(dst, "w13");
  }
    else
  {
    fprintf(out, "  mov %s, w0\n", dst);
  }
  fprintf(out, "  and #2, w0\n");
  fprintf(out, "  sl w0, #5, w0\n");
  fprintf(out, "  and #1, %s\n", dst);
  fprintf(out, "  sl %s, #8, %s\n", dst, dst);
  fprintf(out, "  ior %s, w0, w0\n", dst);
  fprintf(out, "  sl %s, #1, %s\n", dst, dst);
  fprintf(out, "  ior %s, w0, w0\n", dst);
  fprintf(out, "  ior SPI1CON1\n");

  pop_reg(dst);
  fprintf(out, "  ; primary_prescale=(div>>1)&0x3\n");
  fprintf(out, "  ; secondary_prescale=((div&1)&0x7)<<2)\n");
  if (strcmp(dst, "w0") == 0)
  {
    fprintf(out, "  mov w0, w13\n");
    strcpy(dst, "w13");
  }
    else
  {
    fprintf(out, "  mov %s, w0\n", dst);
  }
  fprintf(out, "  asr w0, #1, w0\n");
  fprintf(out, "  xor #3, w0\n");
  fprintf(out, "  and #1, %s\n", dst);
  fprintf(out, "  xor #7, %s\n", dst);
  fprintf(out, "  sl %s, #2, %s\n", dst, dst);
  fprintf(out, "  ior %s, w0, w0\n", dst);
  fprintf(out, "  ior SPI1CON1\n");

  fprintf(out, "  mov #(1<<SPIEN), w0\n");
  fprintf(out, "  ior SPI1STAT\n");

  return 0;
}

int DSPIC::spi_init(int port, int clock_divisor, int mode)
{
  int spre = (clock_divisor & 1) ^ 0x7;
  int ppre = (clock_divisor >> 1) ^ 0x3;

  fprintf(out, "  ;; Set up SPI\n");
  // This chip needs the RP pins set.
  if (chip_type == DSPIC33FJ06GS101A)
  {
    fprintf(out, "  ; SDI is on RP2\n");
    fprintf(out, "  mov #SDI1R2, w0\n");
    fprintf(out, "  mov w0, RPINR20\n");

    fprintf(out, "  ; SDO is on RP3\n");
    fprintf(out, "  mov #(0x7<<8), w0\n");
    fprintf(out, "  mov w0, RPOR1     ; controls RP2, RP3\n");

    fprintf(out, "  ; SCLK is on RP1\n");
    fprintf(out, "  mov #(0x8<<8), w0\n");
    fprintf(out, "  mov w0, RPOR0     ; controls RP0, RP1\n");
  }
  fprintf(out, "  mov #(1<<MSTEN)|%s%s(%d<<2)|(%d), w0\n",
    (mode & 2) == 0 ? "":"(1<<CKP)|",
    (mode &1) == 0 ? "" : "(1<<CKE)|",
    spre, ppre);
  fprintf(out, "  mov w0, SPI1CON1\n");
  fprintf(out, "  mov #(1<<SPIEN), w0\n");
  fprintf(out, "  mov w0, SPI1STAT\n\n");

  return 0;
}

int DSPIC::spi_send(int port)
{
char dst[16];

  pop_reg(dst);
  fprintf(out, "  mov %s, SPI1BUF\n", dst);

  return 0;
}

int DSPIC::spi_read(int port)
{
  if (reg < reg_max)
  {
    fprintf(out, "  mov SPI1BUF, w%d\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  push SPI1BUF\n");
    stack++;
  }

  return 0;
}

int DSPIC::spi_isDataAvailable(int port)
{
  if (reg < reg_max)
  {
    fprintf(out, "  mov SPI1STAT, w%d\n", REG_STACK(reg));
    fprintf(out, "  and #(1<<SPIRBF), w%d\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  mov SPI1STAT, w0\n");
    fprintf(out, "  and #(1<<SPIRBF), w0\n");
    fprintf(out, "  push w0\n");
    stack++;
  }

  return 0;
}

int DSPIC::spi_isBusy(int port)
{
  if (reg < reg_max)
  {
    fprintf(out, "  mov SPI1STAT, w%d\n", REG_STACK(reg));
    fprintf(out, "  and #(1<<SPITBF), w%d\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  mov SPI1STAT, w0\n");
    fprintf(out, "  and #(1<<SPITBF), w0\n");
    fprintf(out, "  push w0\n");
    stack++;
  }

  return 0;
}

int DSPIC::spi_disable(int port)
{
  fprintf(out, "  bclr SPI1STAT, #SPIEN\n");
  return 0;
}

int DSPIC::spi_enable(int port)
{
  fprintf(out, "  bset SPI1STAT, #SPIEN\n");
  return 0;
}

// CPU functions
int DSPIC::cpu_setClock16()
{
  return -1;
}

int DSPIC::cpu_nop()
{
  fprintf(out, "  nop\n");
  return 0;
}


// Memory
int DSPIC::memory_read8_I()
{
  if (stack != 0)
  {
    fprintf(out, "  mov.w [SP-2], w0\n");
    fprintf(out, "  mov.b [w0], w0\n");
    fprintf(out, "  mov.b w0, [SP-2]\n");
  }
    else
  {
    fprintf(out, "  mov.b [w%d], w%d\n", REG_STACK(reg-1), REG_STACK(reg-1));
  }

  return 0;
}

int DSPIC::memory_write8_IB()
{
  if (stack >= 2)
  {
    fprintf(out, "  pop w0\n");
    fprintf(out, "  mov.w [SP-2], w13\n");
    fprintf(out, "  mov.b w0, [w13]\n");
    stack--;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop w0\n");
    fprintf(out, "  mov.b w0, [w%d]\n", REG_STACK(reg-1));
    stack--;
  }
    else
  {
    //fprintf(out, "  mov.b w%d, w0\n", REG_STACK(reg-1));
    //fprintf(out, "  mov.b w0, [w%d]\n", REG_STACK(reg-2));
    fprintf(out, "  mov.b w%d, [w%d]\n", REG_STACK(reg-1), REG_STACK(reg-2));
    reg--;
  }

  return 0;
}

int DSPIC::memory_read16_I()
{
  if (stack != 0)
  {
    fprintf(out, "  mov.w [SP-2], w0\n");
    fprintf(out, "  mov.w [w0], w0\n");
    fprintf(out, "  mov.w w0, [SP-2]\n");
  }
    else
  {
    fprintf(out, "  mov.w [w%d], w%d\n", REG_STACK(reg-1), REG_STACK(reg-1));
  }

  return 0;
}

int DSPIC::memory_write16_IS()
{
  if (stack >= 2)
  {
    fprintf(out, "  pop w0\n");
    fprintf(out, "  mov.w [SP-2], w13\n");
    fprintf(out, "  mov.w w0, [w13]\n");
    stack--;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop w0\n");
    fprintf(out, "  mov.w w0, [w%d]\n", REG_STACK(reg-1));
    stack--;
  }
    else
  {
    //fprintf(out, "  mov.w w%d, w0\n", REG_STACK(reg-1));
    //fprintf(out, "  mov.w w0, [w%d]\n", REG_STACK(reg-2));
    fprintf(out, "  mov.w w%d, [w%d]\n", REG_STACK(reg-1), REG_STACK(reg-2));
    reg--;
  }

  return 0;
}

// DSP (dsPIC stuff)
int DSPIC::dsp_getA()
{
  return dsp_store("sac", "A", 0);
}

int DSPIC::dsp_getB()
{
  return dsp_store("sac", "B", 0);
}

int DSPIC::dsp_getUpperA()
{
  return dsp_store("sac", "A", 7);
}

int DSPIC::dsp_getUpperB()
{
  return dsp_store("sac", "B", 7);
}

int DSPIC::dsp_getLowerA()
{
  return dsp_store("sac", "A", -8);
}

int DSPIC::dsp_getLowerB()
{
  return dsp_store("sac", "B", -8);
}

int DSPIC::dsp_getRoundedA()
{
  return dsp_store("sac.r", "A", 0);
}

int DSPIC::dsp_getRoundedB()
{
  return dsp_store("sac.r", "B", 0);
}

int DSPIC::dsp_clearA()
{
  fprintf(out, "  clr A\n");
  return 0;
}

int DSPIC::dsp_clearB()
{
  fprintf(out, "  clr B\n");
  return 0;
}

int DSPIC::dsp_loadA()
{
char dst[16];

  pop_reg(dst);
  fprintf(out, "  lac %s, A\n", dst);
  return 0;
}

int DSPIC::dsp_loadB()
{
char dst[16];

  pop_reg(dst);
  fprintf(out, "  lac %s, B\n", dst);
  return 0;
}

int DSPIC::dsp_negA()
{
  fprintf(out, "  neg A\n");
  return 0;
}

int DSPIC::dsp_negB()
{
  fprintf(out, "  neg B\n");
  return 0;
}

int DSPIC::dsp_addABAndStoreInA()
{
  fprintf(out, "  add A\n");
  return 0;
}

int DSPIC::dsp_addABAndStoreInB()
{
  fprintf(out, "  add B\n");
  return 0;
}

int DSPIC::dsp_subABAndStoreInA()
{
  fprintf(out, "  sub A\n");
  return 0;
}

int DSPIC::dsp_subBAAndStoreInB()
{
  fprintf(out, "  sub B\n");
  return 0;
}

int DSPIC::dsp_addToA()
{
char dst[16];

  pop_reg(dst);
  fprintf(out, "  add %s, A\n", dst);
  return 0;
}

int DSPIC::dsp_addToB()
{
char dst[16];

  pop_reg(dst);
  fprintf(out, "  add %s, B\n", dst);
  return 0;
}

int DSPIC::dsp_squareToA()
{
  return dsp_square("mpy", "A");
}

int DSPIC::dsp_squareToB()
{
  return dsp_square("mpy", "B");
}

int DSPIC::dsp_mulToA()
{
  return dsp_mul("mpy", "A");
}

int DSPIC::dsp_mulToB()
{
  return dsp_mul("mpy", "B");
}

#if 0
int DSPIC::dsp_euclideanDistanceToA()
{
  return dsp_mul("ed", "A");
}

int DSPIC::dsp_euclideanDistanceToB()
{
  return dsp_mul("ed", "B");
}
#endif

int DSPIC::dsp_squareAndAddToA()
{
  return dsp_square("mac", "A");
}

int DSPIC::dsp_squareAndAddToB()
{
  return dsp_square("mac", "B");
}

int DSPIC::dsp_mulAndAddToA()
{
  return dsp_mul("mac", "A");
}

int DSPIC::dsp_mulAndAddToB()
{
  return dsp_mul("mac", "B");
}

int DSPIC::dsp_mulAndSubFromA()
{
  return dsp_mul("msc", "A");
}

int DSPIC::dsp_mulAndSubFromB()
{
  return dsp_mul("msc", "B");
}

#if 0
int DSPIC::dsp_euclideanDistanceAndAddToA()
{
  return dsp_mul("edac", "A");
}

int DSPIC::dsp_euclideanDistanceAndAddToB()
{
  return dsp_mul("edac", "B");
}
#endif

int DSPIC::dsp_shiftA()
{
char dst[16];

  pop_reg(dst);
  fprintf(out, "  sftac A, %s\n", dst);
  return 0;
}

int DSPIC::dsp_shiftB()
{
char dst[16];

  pop_reg(dst);
  fprintf(out, "  sftac B, %s\n", dst);
  return 0;
}

int DSPIC::dsp_mul(const char *instr, const char *accum)
{
char dst[16];
int reg_num1 = reg > 1 ? (REG_STACK(reg-1)) : -1;
int reg_num2 = reg > 1 ? (REG_STACK(reg-2)) : -1;

  if (reg_num1 > reg_num2)
  {
    int x;
    x = reg_num1;
    reg_num1 = reg_num2;
    reg_num2 = x;
  }

  if (stack == 0 && reg_num1 == 4 && reg_num2 == 5)
  {
    reg -= 2;
    fprintf(out, "  %s w%d*w%d, %s\n", instr, reg_num1, reg_num2, accum);
  }
    else
  {
    pop_reg(dst);
    fprintf(out, "  mov %s, w7\n", dst);
    pop_reg(dst);
    fprintf(out, "  mov %s, w6\n", dst);
    fprintf(out, "  %s w6*w7, %s\n", instr, accum);
  }

  return 0;
}

int DSPIC::dsp_square(const char *instr, const char *accum)
{
char dst[16];
int reg_num = reg > 0 ? (REG_STACK(reg-1)) : -1;

  if (stack > 0 || reg == -1)
  {
    pop_reg(dst);
    fprintf(out, "  mov %s, w7\n", dst);
    fprintf(out, "  %s w7*w7, %s\n", instr, accum);
  }
    else
  {
    reg--;
    fprintf(out, "  %s w%d*w%d, %s\n", instr, reg_num, reg_num, accum);
  }

  return 0;
}

int DSPIC::dsp_store(const char *instr, const char *accum, int shift)
{
char shift_str[32];

  if (shift != 0) { sprintf(shift_str, "#%d, ", shift); }
  else { shift_str[0] = 0; }

  if (reg < reg_max)
  {
    fprintf(out, "  %s %s, %sw%d\n", instr, accum, shift_str, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  %s %s, %sw0\n", instr, accum, shift_str);
    fprintf(out, "  push w0\n");
    stack++;
  }

  return 0;
}

void DSPIC::pop_reg(char *dst)
{
  if (stack > 0)
  {
    stack--;
    fprintf(out, "  pop w0\n");
    sprintf(dst, "w0");
  }
    else
  {
    reg--;
    sprintf(dst, "w%d", REG_STACK(reg));
  }
}

#if 0
void DSPIC::push_w0()
{
  if (stack > 0)
  {
    fprintf(out, "  push w0\n");
    stack++;
  }
    else
  {
    sprintf(dst, "  mov w0, w%d", REG_STACK(reg));
    reg++;
  }
}
#endif

int DSPIC::set_periph(const char *instr, const char *periph, bool reverse)
{
  if (stack == 0)
  {
    fprintf(out, "  mov w%d, w0\n", REG_STACK(reg-1));
    reg--;
  }
    else
  {
    fprintf(out, "  pop w0\n");
    stack--;
  }

  if (reverse) { fprintf(out, "  xor #0xff, w0\n"); }
  fprintf(out, "  %s %s\n", instr, periph);

  return 0;
}

int DSPIC::stack_alu(const char *instr)
{
  if (stack == 0)
  {
    fprintf(out, "  %s.w w%d, w%d, w%d\n", instr, REG_STACK(reg-2), REG_STACK(reg-1), REG_STACK(reg-2));
    reg--;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop w0\n");
    fprintf(out, "  %s.w w%d, w0, w%d\n", instr, REG_STACK(reg-1), REG_STACK(reg-1));
    stack--;
  }
    else
  {
    fprintf(out, "  pop w0\n");
    fprintf(out, "  pop w1\n");
    fprintf(out, "  %s.w w1, w0, w0\n", instr);
    fprintf(out, "  push w0\n");
  }

  return 0;
}

int DSPIC::stack_alu_div()
{
  if (stack == 0)
  {
    fprintf(out, "  repeat #17\n");
    fprintf(out, "  div.s w%d, w%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
    reg--;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop w0\n");
    fprintf(out, "  repeat #17\n");
    fprintf(out, "  div.s w%d, w0\n", REG_STACK(reg-1));
    stack--;
  }
    else
  {
    fprintf(out, "  pop w0\n");
    fprintf(out, "  repeat #17\n");
    fprintf(out, "  div.s w0, [SP-2]\n");
  }

  return 0;
}

int DSPIC::stack_shift(const char *instr)
{
  if (stack >= 2)
  {
    fprintf(out, "  pop w0\n");
    fprintf(out, "  pop w13\n");
    fprintf(out, "  %s w13, w0, w13\n", instr);
    fprintf(out, "  push w13\n");
    stack--;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop w0\n");
    fprintf(out, "  %s w%d, w0, w%d\n", instr, REG_STACK(reg-1), REG_STACK(reg-1));
    stack--;
  }
    else
  if (reg > 0)
  {
    fprintf(out, "  %s w%d, w%d, w%d\n", instr, REG_STACK(reg-2), REG_STACK(reg-1), REG_STACK(reg-2));
    reg--;
  }

  return 0;
}

int DSPIC::get_pin_number(int const_val)
{
int n,pin=-1;

  for (n = 0; n < 8; n++)
  {
    if ((const_val & (1 << n)) != 0)
    {
      // if more than one pin is 1, we can't use it.
      if (pin != -1) { return -1; }
      pin = n;
    }
  }

  return pin;
}

int DSPIC::get_values_from_stack(int *value1, int *value2, int *value3)
{
  if (stack > 0)
  {
    fprintf(out, "  pop w0\n");
    *value1 = 0;
    stack--;
  }
    else
  {
    *value1 = REG_STACK(reg-1);
    reg--;
  }

  if (stack > 0)
  {
    fprintf(out, "  pop w7\n");
    *value2 = 7;
    stack--;
  }
    else
  {
    *value2 = REG_STACK(reg-1);
    reg--;
  }

  if (stack > 0)
  {
    fprintf(out, "  pop w6\n");
    *value3 = 6;
    stack--;
  }
    else
  {
    *value3 = REG_STACK(reg-1);
    reg--;
  }

  return 0;
}

int DSPIC::get_values_from_stack(int *value1, int *value2)
{
  if (stack > 0)
  {
    fprintf(out, "  pop w7\n");
    *value1 = 7;
    stack--;
  }
    else
  {
    *value1 = REG_STACK(reg-1);
    reg--;
  }

  if (stack > 0)
  {
    fprintf(out, "  pop w6\n");
    *value2 = 6;
    stack--;
  }
    else
  {
    *value2 = REG_STACK(reg-1);
    reg--;
  }

  return 0;
}


