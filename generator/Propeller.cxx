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

#include "Propeller.h"

#define PUSH_IMMEDIATE(a) \
  fprintf(out, "  mov reg_%d, #%d\n", reg++, a & 0x1ff); \
  if (reg > reg_max) { reg_max = reg; }

#define PUSH_CONST(a) \
  get_constant(a); \
  fprintf(out, "  mov reg_%d, _const_%d\n", reg++, a); \
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
  is_main(0)
{

}

Propeller::~Propeller()
{
  int n;

  fprintf(out, "  ;; Constants\n");
  for (std::map<uint32_t,int>::iterator it = constants_pool.begin();
       it != constants_pool.end();
       it++)
  {
    fprintf(out, "_const_%d:\n", it->first);
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
  fprintf(out, "%s:\n", name);
}

void Propeller::method_end(int local_count)
{
  fprintf(out, "\n");
}

int Propeller::push_local_var_int(int index)
{
  return -1;
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
  return -1;
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
  return -1;
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
  fprintf(out, "  add reg_%d, #%d\n", reg - 1, num);
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

  fprintf(out, "  cmps reg_%d, #0\n", --reg);

  return -1;
}

int Propeller::jump_cond_integer(const char *label, int cond, int distance)
{
  fprintf(out, "  cmps reg_%d, reg_%d\n", reg - 2, reg - 1);
  reg -= 2;

  return -1;
}

int Propeller::ternary(int cond, int value_true, int value_false)
{
  return -1;
}

int Propeller::return_local(int index, int local_count)
{
  return -1;
}

int Propeller::return_integer(int local_count)
{
  return -1;
}

int Propeller::return_void(int local_count)
{
  return -1;
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
  return -1;
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
    fprintf(out, "  or _dir%c, _const_%d\n", p, value);
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
    fprintf(out, "  mov _out%c, _const_%d\n", p, value);
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
  if (value >= 0 && value <= 255)
  {
    fprintf(out, "  or _out%c, #0x%02x\n", p, value);
  }
    else
  {
    get_constant(value);
    fprintf(out, "  or _out%c, _const_%d\n", p, value);
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

