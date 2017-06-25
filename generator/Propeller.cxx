/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2017 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "Propeller.h"

#define PUSH_IMMEDIATE(a) \
  fprintf(out, "  mov reg_%d, #%d\n", reg++, a & 0x1ff); \
  if (reg > reg_max) { reg_max = reg; }

#define PUSH_CONST(a) \
  get_constant(a); \
  fprintf(out, "  mov reg_%d, _const_%x\n", reg++, a); \
  if (reg > reg_max) { reg_max = reg; }

#define PUSH_SIGNED(a) \
  if (a < -256 || a > 255) { PUSH_CONST(a); } \
  else { PUSH_IMMEDIATE(a); } \

#define PUSH_UNSIGNED(a) \
  if (a < 0 || a > 511) { PUSH_CONST(a); } \
  else { PUSH_IMMEDIATE(a); } \

#define PUSH_TEMP() \
  reg++; \
  if (reg > reg_max) { reg_max = reg; }

#define CHECK_PORT() \
  if (port > 1) { printf("Port out of range\n"); return -1; } \
  char p = 'a' + port;

// ABI is:

Propeller::Propeller() :
  reg(0),
  reg_max(0),
  is_main(0),
  need_muls(0)
{

}

Propeller::~Propeller()
{
  int n;

  if (need_muls)
  {
    add_muls();
  }

  fprintf(out, "  ;; Constants\n");
  for (std::map<uint32_t,int>::iterator it = constants_pool.begin();
       it != constants_pool.end();
       it++)
  {
    fprintf(out, "_const_%x:\n", it->first);
    fprintf(out, "  dc32 0x%x\n", it->first);
  }

  fprintf(out, "  ;; Static variables\n");
  for (std::vector<std::string>::iterator it = statics.begin();
       it != statics.end();
       it++)
  {
    fprintf(out, "_static_%s:\n", it->c_str());
    fprintf(out, "  dc32 0\n");
  }

  fprintf(out, "  ;; Register stack\n");
  for (n = 0; n < reg_max; n++)
  {
    fprintf(out, "reg_%d:\n", n);
    fprintf(out, "  dc32 0\n");
  }

  fprintf(out, "_temp0:\n");
  fprintf(out, "  dc32 0\n");
  fprintf(out, "_stack_ptr:\n");
  fprintf(out, "  dc32 496\n");
}

int Propeller::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".propeller\n\n");

  fprintf(out,
    "  _par equ 496\n"
    "  _cnt equ 497\n"
    "  _ina equ 498\n"
    "  _inb equ 499\n"
    "  _outa equ 500\n"
    "  _outb equ 501\n"
    "  _dira equ 502\n"
    "  _dirb equ 503\n"
    "  _ctra equ 504\n"
    "  _ctrb equ 505\n"
    "  _frqa equ 506\n"
    "  _frqb equ 507\n"
    "  _phsa equ 508\n"
    "  _phsb equ 509\n"
    "  _vcfg equ 510\n"
    "  _vscl equ 511\n\n"
    );

  // Set where RAM starts / ends
  //fprintf(out, "ram_start equ 0\n");
  //fprintf(out, "ram_end equ 0x8000\n");

  return 0;
}

int Propeller::start_init()
{
  // Add any set up items (stack, registers, etc).
  fprintf(out, "start:\n");

  return 0;
}

int Propeller::insert_static_field_define(const char *name, const char *type, int index)
{
  statics.push_back(name);

  return 0;
}

int Propeller::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");
  //fprintf(out, "  mov #ram_start+%d, &ram_start\n", (field_count + 1) * 2);

  return 0;
}

int Propeller::field_init_int(char *name, int index, int value)
{
  return -1;
}

int Propeller::field_init_ref(char *name, int index)
{
  return -1;
}

void Propeller::method_start(int local_count, int max_stack, int param_count, const char *name)
{
  method_name = name;

  is_main = (strcmp(name, "main") == 0);

  if (is_main)
  {
    extra_stack = local_count;
  }
    else
  {
    extra_stack = local_count - param_count;
  }

  if (extra_stack < 0)
  {
    printf("Internal Error: %s:%d\n", __FILE__, __LINE__);
    exit(1);
  }

  fprintf(out, "  ;; method_start() local_count=%d is_main=%d\n", local_count, is_main);
  fprintf(out, "%s:\n", name);

  if (extra_stack != 0)
  {
    fprintf(out, "  sub _stack_ptr, #%d\n", extra_stack);
  }

  this->local_count = local_count;
}

void Propeller::method_end(int local_count)
{
  fprintf(out, "\n");
}

int Propeller::push_local_var_int(int index)
{
  index = local_count - index - 1;

  if (index == 0)
  {
    fprintf(out, "  movs label_%d, _stack_ptr\n", label_count);
  }
    else
  {
    fprintf(out, "  mov _temp0, _stack_ptr\n");
    fprintf(out, "  add _temp0, #%d\n", index);
    fprintf(out, "  movs label_%d, _temp0\n", label_count);
  }

  fprintf(out, "label_%d:\n", label_count);
  fprintf(out, "  mov reg_%d, 0\n", reg++);
  if (reg > reg_max) { reg_max = reg; }

  label_count++;

  return 0;
}

int Propeller::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int Propeller::push_ref_static(const char *name, int index)
{
  //fprintf(out, "  mov reg_%d, _static_%s\n", reg++, name);
  //if (reg > reg_max) { reg_max = reg; }
  return -1;
}

int Propeller::push_fake()
{
  return -1;
}

int Propeller::set_integer_local(int index, int value)
{
  index = local_count - index - 1;

  if (index == 0)
  {
    fprintf(out, "  movd label_%d, _stack_ptr\n", label_count);
  }
    else
  {
    fprintf(out, "  mov _temp0, _stack_ptr\n");
    fprintf(out, "  add _temp0, #%d\n", index);
    fprintf(out, "  movd label_%d, _temp0\n", label_count);
  }

  fprintf(out, "label_%d:\n", label_count);

  if (value >= 0 && value < 512)
  {
    fprintf(out, "  mov 0, #%d\n", value);
  }
    else
  {
    get_constant(value);
    fprintf(out, "  mov 0, _const_%x\n", value);
  }

  label_count++;

  return 0;
}

int Propeller::push_int(int32_t n)
{
  PUSH_SIGNED(n);
  return 0;
}

int Propeller::push_long(int64_t n)
{
  return -1;
}

int Propeller::push_float(float f)
{
  return -1;
}

int Propeller::push_double(double f)
{
  return -1;
}

int Propeller::push_ref(char *name)
{
  // Need to move the address of name to the top of stack
  //fprintf(out, "  mov reg_%d, #_static_%s\n", reg++, name);
  fprintf(out, "  mov reg_%d, _static_%s\n", reg++, name);
  if (reg > reg_max) { reg_max = reg; }
  return 0;
}

int Propeller::pop_local_var_int(int index)
{
  index = local_count - index - 1;

  if (index == 0)
  {
    fprintf(out, "  movd label_%d, _stack_ptr\n", label_count);
  }
    else
  {
    fprintf(out, "  mov _temp0, _stack_ptr\n");
    fprintf(out, "  add _temp0, #%d\n", index);
    fprintf(out, "  movd label_%d, _temp0\n", label_count);
  }

  fprintf(out, "label_%d:\n", label_count);
  fprintf(out, "  mov 0, reg_%d\n", --reg);

  label_count++;

  return 0;
}

int Propeller::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int Propeller::pop()
{
  return -1;
}

int Propeller::dup()
{
  return -1;
}

int Propeller::dup2()
{
  return -1;
}

int Propeller::swap()
{
  return -1;
}

int Propeller::add_integer()
{
  fprintf(out, "  adds reg_%d, reg_%d\n", reg - 2, reg - 1);
  reg--;
  return 0;
}

int Propeller::add_integer(int num)
{
  if (num < -256 || num > 255) { return -1; }

  fprintf(out, "  adds reg_%d, #%d\n", reg - 1, num);
  return 0;
}

int Propeller::sub_integer()
{
  fprintf(out, "  subs reg_%d, reg_%d\n", reg - 2, reg - 1);
  reg--;
  return 0;
}

int Propeller::sub_integer(int num)
{
  if (num < -256 || num > 255) { return -1; }

  fprintf(out, "  subs reg_%d, #%d\n", reg - 1, num);
  return 0;
}

int Propeller::mul_integer()
{
  need_muls = 1;

  fprintf(out, "  call _muls_ret, #_muls\n");

  return 0;
}

int Propeller::div_integer()
{
  return -1;
}

int Propeller::mod_integer()
{
  return -1;
}

int Propeller::neg_integer()
{
  fprintf(out, "  neg reg_%d, reg_%d\n", reg - 1, reg - 1);
  return 0;
}

int Propeller::shift_left_integer()
{
  fprintf(out, "  shl reg_%d, reg_%d\n", reg - 2, reg - 1);
  reg--;
  return 0;
}

int Propeller::shift_left_integer(int num)
{
  if (num < -256 || num > 255) { return -1; }

  fprintf(out, "  shl reg_%d, #%d\n", reg - 1, num);
  return 0;
}

int Propeller::shift_right_integer()
{
  fprintf(out, "  sar reg_%d, reg_%d\n", reg - 2, reg - 1);
  reg--;
  return 0;
}

int Propeller::shift_right_integer(int num)
{
  if (num < -256 || num > 255) { return -1; }

  fprintf(out, "  sar reg_%d, #%d\n", reg - 1, num);
  return 0;
}

int Propeller::shift_right_uinteger()
{
  fprintf(out, "  shr reg_%d, reg_%d\n", reg - 2, reg - 1);
  reg--;
  return 0;
}

int Propeller::shift_right_uinteger(int num)
{
  if (num < -256 || num > 255) { return -1; }

  fprintf(out, "  shr reg_%d, #%d\n", reg - 1, num);
  return 0;
}

int Propeller::and_integer()
{
  fprintf(out, "  and reg_%d, reg_%d\n", reg - 2, reg - 1);
  reg--;
  return 0;
}

int Propeller::and_integer(int num)
{
  if (num < -256 || num > 255) { return -1; }

  fprintf(out, "  and reg_%d, #%d\n", reg - 1, num);
  return 0;
}

int Propeller::or_integer()
{
  fprintf(out, "  or reg_%d, reg_%d\n", reg - 2, reg - 1);
  reg--;
  return 0;
}

int Propeller::or_integer(int num)
{
  if (num < -256 || num > 255) { return -1; }

  fprintf(out, "  or reg_%d, #%d\n", reg - 1, num);
  return 0;
}

int Propeller::xor_integer()
{
  fprintf(out, "  xor reg_%d, reg_%d\n", reg - 2, reg - 1);
  reg--;
  return 0;
}

int Propeller::xor_integer(int num)
{
  if (num < -256 || num > 255) { return -1; }

  fprintf(out, "  xor reg_%d, #%d\n", reg - 1, num);
  return 0;
}

int Propeller::inc_integer(int index, int num)
{
  index = local_count - index - 1;

  fprintf(out, "  ;; inc_integer(%d, %d)\n", index, num);

  if (index == 0)
  {
    fprintf(out, "  movd label_%d, _stack_ptr\n", label_count);
  }
    else
  {
    fprintf(out, "  mov _temp0, _stack_ptr\n");
    fprintf(out, "  add _temp0, #%d\n", index);
    fprintf(out, "  movd label_%d, _temp0\n", label_count);
  }

  fprintf(out, "label_%d:\n", label_count);

  if (num >= 0 && num < 512)
  {
    fprintf(out, "  adds 0, #%d\n", num);
  }
    else
  {
    get_constant(num);
    fprintf(out, "  adds 0, _const_%x\n", num);
  }

  label_count++;

  return 0;
}

int Propeller::integer_to_byte()
{
  return -1;
}

int Propeller::integer_to_short()
{
  return -1;
}

int Propeller::jump_cond(const char *label, int cond, int distance)
{
  if (cond == COND_EQUAL)
  {
    fprintf(out, "  tjz reg_%d, #%s\n", --reg, label);
    return 0;
  }

  if (cond == COND_NOT_EQUAL)
  {
    fprintf(out, "  tjnz reg_%d, #%s\n", --reg, label);
    return 0;
  }

  fprintf(out, "  cmps reg_%d, #0, wc wz\n", --reg);

  switch(cond)
  {
    case COND_LESS:
      fprintf(out, "  if_b jmp #%s\n", label);
      break;
    case COND_LESS_EQUAL:
      fprintf(out, "  if_be jmp #%s\n", label);
      break;
    case COND_GREATER:
      fprintf(out, "  if_a jmp #%s\n", label);
      break;
    case COND_GREATER_EQUAL:
      fprintf(out, "  if_ae jmp #%s\n", label);
      break;
    default:
      return -1;
  }

  return 0;
}

int Propeller::jump_cond_integer(const char *label, int cond, int distance)
{
  fprintf(out, "  cmps reg_%d, reg_%d, wc wz\n", reg - 2, reg - 1);
  reg -= 2;

  switch(cond)
  {
    case COND_EQUAL:
      fprintf(out, "  if_e jmp #%s\n", label);
      break;
    case COND_NOT_EQUAL:
      fprintf(out, "  if_ne jmp #%s\n", label);
      break;
    case COND_LESS:
      fprintf(out, "  if_b jmp #%s\n", label);
      break;
    case COND_LESS_EQUAL:
      fprintf(out, "  if_be jmp #%s\n", label);
      break;
    case COND_GREATER:
      fprintf(out, "  if_a jmp #%s\n", label);
      break;
    case COND_GREATER_EQUAL:
      fprintf(out, "  if_ae jmp #%s\n", label);
      break;
    default:
      return -1;
  }

  return 0;

}

int Propeller::ternary(int cond, int value_true, int value_false)
{
  return -1;
}

int Propeller::ternary(int cond, int compare, int value_true, int value_false)
{
  return -1;
}

int Propeller::return_local(int index, int local_count)
{
  index = local_count - index - 1;

  if (index == 0)
  {
    fprintf(out, "  movs label_%d, _stack_ptr\n", label_count);
  }
    else
  {
    fprintf(out, "  mov _temp0, _stack_ptr\n");
    fprintf(out, "  add _temp0, #%d\n", index);
    fprintf(out, "  movs label_%d, _temp0\n", label_count);
  }

  fprintf(out, "label_%d:\n", label_count);
  fprintf(out, "  mov _temp0, 0\n");

  label_count++;

  if (extra_stack != 0)
  {
    fprintf(out, "  add _stack_ptr, #%d\n", extra_stack);
  }

  if (!is_main)
  {
    fprintf(out, "%s_ret:\n", method_name.c_str());
  }

  fprintf(out, "  ret\n");

  return 0;
}

int Propeller::return_integer(int local_count)
{
  fprintf(out, "  mov _temp0, reg_%d\n", --reg);

  if (extra_stack != 0)
  {
    fprintf(out, "  add _stack_ptr, #%d\n", extra_stack);
  }

  if (!is_main)
  {
    fprintf(out, "%s_ret:\n", method_name.c_str());
  }

  fprintf(out, "  ret\n");

  return 0;
}

int Propeller::return_void(int local_count)
{
  if (extra_stack != 0)
  {
    fprintf(out, "  add _stack_ptr, #%d\n", extra_stack);
  }

  if (!is_main)
  {
    fprintf(out, "%s_ret:\n", method_name.c_str());
  }

  fprintf(out, "  ret\n");

  return 0;
}

int Propeller::jump(const char *name, int distance)
{
  fprintf(out, "  jmp #%s\n", name);
  return 0;
}

int Propeller::call(const char *name)
{
  return -1;
}

int Propeller::invoke_static_method(const char *name, int params, int is_void)
{
  int n;

  fprintf(out, "  ;; invoke_static_method(%s, %d, %d)\n", name, params, is_void);

  // Save any unused registers
  for (n = 0; n < reg; n++)
  {
    fprintf(out, "  sub _stack_ptr, #1\n");
    fprintf(out, "  mov _stack_ptr, reg_%d\n", n);
  }

  fprintf(out, "  call %s_ret, #%s\n", name, name);

  fprintf(out, "  add _stack_ptr, #%d\n", params);

  // Restore registers
  for (n = 0; n < reg - params; n++)
  {
    fprintf(out, "  mov _stack_ptr, reg_%d\n", n);
    fprintf(out, "  add _stack_ptr, #1\n");
  }

  reg -= params;

  if (is_void == 0)
  {
    fprintf(out, "  mov reg_%d, _temp0\n", reg++);
  }

  return 0;
}

int Propeller::put_static(const char *name, int index)
{
  fprintf(out, "  mov _static_%s, reg_%d\n", name, --reg);
  return 0;
}

int Propeller::get_static(const char *name, int index)
{
  fprintf(out, "  mov reg_%d, _static_%s\n", reg++, name);
  if (reg > reg_max) { reg_max = reg; }
  return 0;
}

int Propeller::brk()
{
  return -1;
}

int Propeller::new_array(uint8_t type)
{
  return -1;
}

int Propeller::insert_array(const char *name, int32_t *data, int len, uint8_t type)
{
  if (type == TYPE_BYTE)
  { return insert_db(name, data, len, TYPE_INT); }
    else
  if (type == TYPE_SHORT)
  { return insert_dw(name, data, len, TYPE_INT); }
    else
  if (type == TYPE_INT)
  { return insert_dc32(name, data, len, TYPE_INT); }

  return -1;
}

int Propeller::insert_string(const char *name, uint8_t *bytes, int len)
{
  return -1;
}

int Propeller::push_array_length()
{
  return -1;
}

int Propeller::push_array_length(const char *name, int field_id)
{
  return -1;
}

int Propeller::array_read_byte()
{
  return -1;
}

int Propeller::array_read_short()
{
  return -1;
}

int Propeller::array_read_int()
{
  return -1;
}

int Propeller::array_read_byte(const char *name, int field_id)
{
  return -1;
}

int Propeller::array_read_short(const char *name, int field_id)
{
  return -1;
}

int Propeller::array_read_int(const char *name, int field_id)
{
  return -1;
}

int Propeller::array_write_byte()
{
  return -1;
}

int Propeller::array_write_short()
{
  return -1;
}

int Propeller::array_write_int()
{
  return -1;
}

int Propeller::array_write_byte(const char *name, int field_id)
{
  return -1;
}

int Propeller::array_write_short(const char *name, int field_id)
{
  return -1;
}

int Propeller::array_write_int(const char *name, int field_id)
{
  return -1;
}

int Propeller::math_abs_I()
{
  fprintf(out, "  abs reg_%d, reg_%d\n", reg - 1, reg - 1);
  return 0;
}

int Propeller::math_xor_I()
{
  fprintf(out, "  xor reg_%d, reg_%d\n", reg - 1, reg - 1);
  return 0;
}

int Propeller::math_min_II()
{
  fprintf(out, "  min reg_%d, reg_%d\n", reg - 2, reg - 1);
  reg--;
  return 0;
}

int Propeller::math_max_II()
{
  fprintf(out, "  max reg_%d, reg_%d\n", reg - 2, reg - 1);
  reg--;
  return 0;
}

int Propeller::propeller_setClock_I()
{
  return -1;
}

int Propeller::propeller_getCogId()
{
  fprintf(out, "  cogid reg_%d\n", reg++);
  if (reg > reg_max) { reg_max = reg; }
  return 0;
}

int Propeller::propeller_stopCog_I()
{
  fprintf(out, "  cogstop reg_%d\n", --reg);
  return 0;
}

int Propeller::propeller_waitPinsEqual_II()
{
  fprintf(out, "  waitpeq reg_%d, reg_%d\n", reg - 2, reg - 1);
  reg -= 2;
  return 0;
}

int Propeller::propeller_waitPinsEqual_II(int mask)
{
  fprintf(out, "  waitpeq reg_%d, #0x%08x\n", reg - 1, (uint32_t)mask);
  reg--;
  return 0;
}

int Propeller::propeller_waitPinsNotEqual_II()
{
  fprintf(out, "  waitpne reg_%d, reg_%d\n", reg - 2, reg - 1);
  reg -= 2;
  return 0;
}

int Propeller::propeller_waitPinsNotEqual_II(int mask)
{
  fprintf(out, "  waitpne reg_%d, #0x%08x\n", reg - 1, (uint32_t)mask);
  reg--;
  return 0;
}

int Propeller::propeller_waitCount_II()
{
  //fprintf(out, "  add reg_%d, reg_%d\n", reg - 2, reg - 1);
  fprintf(out, "  waitcnt reg_%d, reg_%d\n", reg - 2, reg - 1);
  reg--;
  return 0;
}

int Propeller::propeller_waitCount_II(int delay)
{
  if (delay < 0 || delay > 255) { return -1; }
  //fprintf(out, "  add reg_%d, #0x%02x\n", reg - 1, (uint32_t)delay);
  fprintf(out, "  waitcnt reg_%d, #0x%02x\n", reg - 1, (uint32_t)delay);
  return 0;
}

int Propeller::cpu_getCycleCount()
{
  fprintf(out, "  mov reg_%d, _cnt\n", reg++);
  if (reg > reg_max) { reg_max = reg; }
  return 0;
}

int Propeller::ioport_setPinsAsInput_I(int port)
{
  CHECK_PORT();
  fprintf(out, "  andn _dir%c, reg_%d\n", p, reg - 1);
  reg--;

  return 0;
}

int Propeller::ioport_setPinsAsOutput_I(int port)
{
  CHECK_PORT();
  fprintf(out, "  or _dir%c, reg_%d\n", p, reg - 1);
  reg--;

  return 0;
}

int Propeller::ioport_setPinsAsOutput_I(int port, int value)
{
  CHECK_PORT();
  if (value >= 0 && value <= 255)
  {
    fprintf(out, "  or _dir%c, #0x%02x\n", p, value);
  }
    else
  {
    get_constant(value);
    fprintf(out, "  or _dir%c, _const_%x\n", p, value);
  }

  return 0;
}

int Propeller::ioport_setPinsValue_I(int port)
{
  CHECK_PORT();
  fprintf(out, "  mov _out%c, reg_%d\n", p, reg - 1);
  reg--;

  return 0;
}

int Propeller::ioport_setPinsValue_I(int port, int value)
{
  CHECK_PORT();
  if (value >= 0 && value <= 255)
  {
    fprintf(out, "  mov _out%c, #0x%02x\n", p, value);
  }
    else
  {
    get_constant(value);
    fprintf(out, "  mov _out%c, _const_%x\n", p, value);
  }

  return 0;
}

int Propeller::ioport_setPinsHigh_I(int port)
{
  CHECK_PORT();
  fprintf(out, "  or _out%c, reg_%d\n", p, reg - 1);
  reg--;

  return 0;
}

int Propeller::ioport_setPinsHigh_I(int port, int value)
{
  CHECK_PORT();
  if (value >= 0 && value <= 511)
  {
    fprintf(out, "  or _out%c, #0x%02x\n", p, value);
  }
    else
  {
    get_constant(value);
    fprintf(out, "  or _out%c, _const_%x\n", p, value);
  }

  return 0;
}

int Propeller::ioport_setPinsLow_I(int port)
{
  CHECK_PORT();
  fprintf(out, "  andn _out%c, reg_%d\n", p, reg - 1);
  reg--;

  return 0;
}

int Propeller::ioport_setPinsLow_I(int port, int value)
{
  CHECK_PORT();
  if (value >= 0 && value <= 511)
  {
    fprintf(out, "  andn _out%c, #0x%02x\n", p, value);
  }
    else
  {
    get_constant(value);
    fprintf(out, "  andn _out%c, _const_%x\n", p, value);
  }

  return 0;
}

//int Propeller::ioport_setPinAsOutput(int port)
//int Propeller::ioport_setPinAsInput(int port)
//int Propeller::ioport_setPinHigh(int port)
//int Propeller::ioport_setPinLow(int port)

int Propeller::ioport_isPinInputHigh_I(int port)
{
  return -1;
}

int Propeller::ioport_getPortInputValue(int port)
{
  CHECK_PORT();
  fprintf(out, "  mov reg_%d, _out%c\n", reg++, p);

  return 0;
}

int Propeller::add_muls()
{
  reg -= 1;

  fprintf(out,
    "_muls:\n"
    "  mov _temp0, #0\n"
    "  cmps reg_%d, #0, wc wz\n"
    "  if_b xor _temp0, #1\n"
    "  cmps reg_%d, #0, wc wz\n"
    "  if_b xor _temp0, #1\n",
    reg, reg - 1);

  fprintf(out,
    "  abs reg_%d, reg_%d\n"
    "  abs reg_%d, reg_%d\n"
    "  and reg_%d, _mask16\n"
    "  and reg_%d, _mask16\n",
    reg - 3, reg - 2,
    reg - 2, reg - 1,
    reg - 3,
    reg - 2);

  fprintf(out, "  mov reg_%d, #0\n", reg - 1);

  fprintf(out,
    "_repeat_muls:\n"
    "  shr reg_%d, #1, wc wz\n"
    "  if_c add reg_%d, reg_%d\n"
    "  shl reg_%d, #1\n"
    "  if_nz jmp #_repeat_muls\n",
    reg - 2,
    reg - 1, reg - 3,
    reg - 3);

  fprintf(out,
    "  cmp _temp0, #1, wz\n"
    "  if_e neg reg_%d, reg_%d\n\n"
    "_muls_ret:\n",
    reg - 1, reg - 1);

  fprintf(out,
    "_mask16:\n"
    "  dc32 0xffff\n");

  return 0;
}


