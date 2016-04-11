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
#include <string.h>
#include <stdint.h>

#include "MC6809.h"

#define LOCALS(i) (i * 2)

#define CHECK_INT16(a) \
  if (a > 65535 || a < -32768) \
  { \
    printf("Error: literal value %d bigger than 16 bit.\n", a); \
    return -1; \
  }

// ABI is:

//                        ==     !=     yum    <=      >     >=
const char *branch[] = { "beq", "bne", "blt", "ble", "bgt", "bge" };

MC6809::MC6809() :
  start_org(0),
  ram_start(0x0600),
  ram_end(0x06ff),
  reg(0),
  reg_max(9),
  stack(0),
  is_main(0),
  need_multiply(0)
{

}

MC6809::~MC6809()
{
  if (need_multiply) { add_multiply(); }
}

int MC6809::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".6809\n");

  // Set where RAM starts / ends
  fprintf(out, "ram_start equ 0x%04x\n", ram_start);
  fprintf(out, "ram_end equ 0x0x%04x\n", ram_end);

  return 0;
}

int MC6809::start_init()
{
  // Add any set up items (stack, registers, etc).
  fprintf(out, ".org 0x%04x\n", start_org);
  fprintf(out, "start:\n");
  fprintf(out, "  lds #0x%04x ; Set SP stack pointer\n", ram_end + 1);

  return 0;
}

int MC6809::insert_static_field_define(const char *name, const char *type, int index)
{
  fprintf(out, "%s equ ram_start+%d\n", name, index * 2);

  return 0;
}

int MC6809::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");
  //fprintf(out, "  mov #ram_start+%d, &ram_start\n", (field_count + 1) * 2);

  return -1;
}

int MC6809::field_init_int(char *name, int index, int value)
{
  CHECK_INT16(value);

  fprintf(out, "  ldd #0x%04x\n", (uint16_t)value);
  fprintf(out, "  std %s\n", name);
  return 0;
}

int MC6809::field_init_ref(char *name, int index)
{
  fprintf(out, "  ldd #_%s\n", name);
  fprintf(out, "  std %s\n", name);
  return 0;
}

void MC6809::method_start(int local_count, int max_stack, int param_count, const char *name)
{
  int n;

  is_main = (strcmp(name, "main") == 0) ? 1 : 0;

  fprintf(out, "%s:\n", name);

  fprintf(out, "  pshs u\n");    // really only needed when not main()

  if (local_count != 0)
  {
    fprintf(out, "  leas -%d,s\n", local_count * 2);
  }

  fprintf(out, "  tfr s,u\n");

  if (!is_main)
  {
    int index = (local_count + param_count + 1) * 2;

    for (n = 0; n < param_count; n++)
    {
      fprintf(out, "  ldd %d,s\n", index);
      fprintf(out, "  std %d,s\n", n * 2);
      index -= 2;
    }
  }
}

void MC6809::method_end(int local_count)
{
  fprintf(out, "\n");
}

int MC6809::push_local_var_int(int index)
{
  fprintf(out, "  ; push_local_var_int() index=%d\n", index);
  fprintf(out, "  ldd %d,u\n", LOCALS(index));
  fprintf(out, "  pshs a,b\n");
  return 0;
}

int MC6809::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int MC6809::push_ref_static(const char *name, int index)
{
  fprintf(out, "  ; put_static()\n");
  fprintf(out, "  puls a,b\n");
  fprintf(out, "  std %s\n", name);
  return 0;
}

int MC6809::push_fake()
{
  fprintf(out, "  ; push_fake\n");
  fprintf(out, "  leas -2,s\n");
  return 0;
}

int MC6809::push_int(int32_t n)
{
  CHECK_INT16(n);

  fprintf(out, "  ; push_int\n");
  fprintf(out, "  ldd #0x%04x\n", n & 0xffff);
  fprintf(out, "  pshs a,b\n");

  return 0;
}

int MC6809::push_long(int64_t n)
{
  return -1;
}

int MC6809::push_float(float f)
{
  return -1;
}

int MC6809::push_double(double f)
{
  return -1;
}

int MC6809::push_ref(char *name)
{
  fprintf(out, "  ; push_ref()\n");
  fprintf(out, "  ldd %s\n", name);
  fprintf(out, "  pshs a,b\n");
  return 0;
}

int MC6809::pop_local_var_int(int index)
{
  fprintf(out, "  ; pop_local_var_int() index=%d\n", index);
  fprintf(out, "  puls a,b\n");
  fprintf(out, "  std %d,u\n", LOCALS(index));
  return 0;
}

int MC6809::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int MC6809::pop()
{
  fprintf(out, "  ; pop()\n");
  fprintf(out, "  puls x\n");
  return 0;
}

int MC6809::dup()
{
  fprintf(out, "  ; dup()\n");
  fprintf(out, "  ldx ,s\n");
  fprintf(out, "  pshs x\n");
  return 0;
}

int MC6809::dup2()
{
  fprintf(out, "  ; dup2()\n");
  fprintf(out, "  ldx ,s\n");
  fprintf(out, "  ldy 2,s\n");
  fprintf(out, "  pshs x,y\n");
  return 0;
}

int MC6809::swap()
{
  fprintf(out, "  ; swap()\n");
  fprintf(out, "  puls x,y\n");
  fprintf(out, "  exg x,y\n");
  fprintf(out, "  pshs x,y\n");
  return 0;
}

int MC6809::add_integer()
{
  fprintf(out, "  ; add_integer()\n");
  fprintf(out, "  puls a,b\n");
  fprintf(out, "  addd ,s\n");
  fprintf(out, "  std ,s\n");
  return 0;
}

int MC6809::add_integer(int num)
{
  CHECK_INT16(num);
  fprintf(out, "  ; add_integer()\n");
  fprintf(out, "  ldd #0x%04x\n", num & 0xffff);
  fprintf(out, "  addd ,s\n");
  fprintf(out, "  std ,s\n");
  return 0;
}

int MC6809::sub_integer()
{
  fprintf(out, "  ; sub_integer()\n");
  fprintf(out, "  ldd 2,s\n");
  fprintf(out, "  subd ,s\n");
  fprintf(out, "  std 2,s\n");
  fprintf(out, "  leas 2,s\n");
  return 0;
}

int MC6809::sub_integer(int num)
{
  CHECK_INT16(num);
  fprintf(out, "  ; sub_integer()\n");
  //fprintf(out, "  ldd #0x%04x\n", num & 0xffff);
  //fprintf(out, "  subd ,s\n");
  fprintf(out, "  ldd ,s\n");
  fprintf(out, "  subb #0x%02x\n", num & 0xff);
  fprintf(out, "  sbca #0x%02x\n", (num & 0xff00) >> 8);
  fprintf(out, "  std ,s\n");
  return 0;
}

int MC6809::mul_integer()
{
  need_multiply = 1;

  fprintf(out, "  ; mul_integer()\n");
  fprintf(out, "  jsr _multiply\n");
  fprintf(out, "  leas 2,s\n");

  return 0;
}

int MC6809::div_integer()
{
  return -1;
}

int MC6809::mod_integer()
{
  return -1;
}

int MC6809::neg_integer()
{
  fprintf(out, "  ; neg_integer()\n");
  fprintf(out, "  ldd ,s\n");
  fprintf(out, "  coma\n");
  fprintf(out, "  comb\n");
  fprintf(out, "  subd #-1\n");
  fprintf(out, "  std ,s\n");
  return 0;
}

int MC6809::shift_left_integer()
{
  fprintf(out, "  ; shift_left_integer()\n");
  return -1;
}

int MC6809::shift_left_integer(int num)
{
  int n;

  fprintf(out, "  ; shift_left_integer()\n");

  fprintf(out, "  ldd ,s\n");

  if (num < 8)
  {
    for (n = 0; n < num; n++)
    {
      fprintf(out, "  lslb\n");
      fprintf(out, "  rola\n");
    }
  }
    else
  {
    fprintf(out, "  tfr b,a\n");
    fprintf(out, "  clr b\n");
    num -= 8;

    for (n = 0; n < num; n++)
    {
      fprintf(out, "  asla \n");
    }
  }

  fprintf(out, "  std ,s\n");

  return 0;
}

int MC6809::shift_right_integer()
{
  fprintf(out, "  ; shift_right_integer()\n");
  return -1;
}

int MC6809::shift_right_integer(int num)
{
  int n;

  fprintf(out, "  ; shift_right_integer()\n");
  fprintf(out, "  ldd ,s\n");

  if (num < 8)
  {
    for (n = 0; n < num; n++)
    {
      fprintf(out, "  asra \n");
      fprintf(out, "  rorb \n");
    }
  }
    else
  {
    fprintf(out, "  tfr a,b\n");
    fprintf(out, "  clr a\n");
    num -= 8;

    for (n = 0; n < num; n++)
    {
      fprintf(out, "  lsrb \n");
    }
  }

  fprintf(out, "  std ,s\n");

  return 0;
}

int MC6809::shift_right_uinteger()
{
  fprintf(out, "  ; shift_right_uinteger()\n");
  return -1;
}

int MC6809::shift_right_uinteger(int num)
{
  int n;

  fprintf(out, "  ; shift_right_uinteger()\n");
  fprintf(out, "  ldd ,s\n");

  if (num < 8)
  {
    for (n = 0; n < num; n++)
    {
      fprintf(out, "  lsra \n");
      fprintf(out, "  rorb \n");
    }
  }
    else
  {
    fprintf(out, "  tfr a,b\n");
    fprintf(out, "  clr a\n");
    num -= 8;

    for (n = 0; n < num; n++)
    {
      fprintf(out, "  lsrb \n");
    }
  }

  fprintf(out, "  std ,s\n");

  return 0;
}

int MC6809::and_integer()
{
  return -1;
}

int MC6809::and_integer(int num)
{
  uint16_t value = (uint16_t)num;

  fprintf(out, "  ; and_integer()\n");
  fprintf(out, "  ldd ,s\n");
  fprintf(out, "  anda #0x%02x\n", value >> 8);
  fprintf(out, "  andb #0x%02x\n", value & 0xff);
  fprintf(out, "  std ,s\n");

  return 0;
}

int MC6809::or_integer()
{
  return -1;
}

int MC6809::or_integer(int num)
{
  uint16_t value = (uint16_t)num;

  fprintf(out, "  ; or_integer()\n");
  fprintf(out, "  ldd ,s\n");
  fprintf(out, "  ora #0x%02x\n", value >> 8);
  fprintf(out, "  orb #0x%02x\n", value & 0xff);
  fprintf(out, "  std ,s\n");

  return 0;
}

int MC6809::xor_integer()
{
  fprintf(out, "  ; xor_integer()\n");
  fprintf(out, "  puls a,b\n");
  fprintf(out, "  eora ,s\n");
  fprintf(out, "  eorb 1,s\n");
  fprintf(out, "  std ,s\n");
  return 0;
}

int MC6809::xor_integer(int num)
{
  uint16_t value = (uint16_t)num;

  fprintf(out, "  ; xor_integer()\n");
  fprintf(out, "  ldd ,s\n");
  fprintf(out, "  eora #0x%02x\n", value >> 8);
  fprintf(out, "  eorb #0x%02x\n", value & 0xff);
  fprintf(out, "  std ,s\n");

  return 0;
}

int MC6809::inc_integer(int index, int num)
{
  fprintf(out, "  ; inc_integer() index=%d\n", index);
  fprintf(out, "  ldd %d,u\n", LOCALS(index));
  fprintf(out, "  addd #%d\n", (uint16_t)num);
  fprintf(out, "  std %d,u\n", LOCALS(index));

  return 0;
}

int MC6809::integer_to_byte()
{
  fprintf(out, "  ; integer_to_byte()\n");
  fprintf(out, "  puls a,b\n");
  fprintf(out, "  sex ; (oh yeah, harder!)\n");
  fprintf(out, "  pshs a,b\n");
  return 0;
}

int MC6809::integer_to_short()
{
  fprintf(out, "  ; integer_to_short() - do nothing\n");
  return 0;
}

int MC6809::jump_cond(const char *label, int cond, int distance)
{
  bool use_long = distance > 30;

  fprintf(out, "  ; jump_cond()\n");
  fprintf(out, "  puls a,b\n");
  fprintf(out, "  cmpd #0\n");
  fprintf(out, "  %s%s %s\n", use_long ? "l":"", branch[cond], label);

  return 0;
}

int MC6809::jump_cond_integer(const char *label, int cond, int distance)
{
  bool use_long = distance > 30;

  fprintf(out, "  ; jump_cond()\n");
  fprintf(out, "  puls y\n");
  fprintf(out, "  puls a,b\n");
  fprintf(out, "  cmpd -4,s\n");
  fprintf(out, "  %s%s %s\n", use_long ? "l":"", branch[cond], label);

  return 0;
}

int MC6809::ternary(int cond, int value_true, int value_false)
{
  return -1;
}

int MC6809::ternary(int cond, int compare, int value_true, int value_false)
{
  return -1;
}

int MC6809::return_local(int index, int local_count)
{
  fprintf(out, "  std %d,u\n", LOCALS(index));

  if (local_count != 0)
  {
    fprintf(out, "  leas %d,s\n", local_count * 2);
  }

  fprintf(out, "  puls u\n");
  fprintf(out, "  rts\n");
  return 0;
}

int MC6809::return_integer(int local_count)
{
  fprintf(out, "  puls a,b\n");

  if (local_count != 0)
  {
    fprintf(out, "  leas %d,s\n", local_count * 2);
  }

  fprintf(out, "  puls u\n");
  fprintf(out, "  rts\n");
  return 0;
}

int MC6809::return_void(int local_count)
{
  if (local_count != 0)
  {
    fprintf(out, "  leas %d,s\n", local_count * 2);
  }

  fprintf(out, "  puls u\n");
  fprintf(out, "  rts\n");
  return 0;
}

int MC6809::jump(const char *name, int distance)
{
  if (distance < 20)
  {
    fprintf(out, "  bra %s\n", name);
  }
    else
  {
    fprintf(out, "  jmp %s\n", name);
  }

  return 0;
}

int MC6809::call(const char *name)
{
  return -1;
}

int MC6809::invoke_static_method(const char *name, int params, int is_void)
{
  printf("invoke_static_method() name=%s params=%d is_void=%d\n", name, params, is_void);

  fprintf(out, "  ; invoke_static_method() params=%d\n", params);
  fprintf(out, "  jsr %s\n", name);

  if (params != 0)
  {
    fprintf(out, "  leas %d,s\n", params * 2);
  }

  if (is_void == 0)
  {
    fprintf(out, "  pshs a,b\n");
  }

  return 0;
}

int MC6809::put_static(const char *name, int index)
{
  fprintf(out, "  ; put_static()\n");
  fprintf(out, "  puls a,b\n");
  fprintf(out, "  std %s\n", name);
  return 0;
}

int MC6809::get_static(const char *name, int index)
{
  fprintf(out, "  ; get_static()\n");
  fprintf(out, "  ldd %s\n", name);
  fprintf(out, "  pshs a,b\n");
  return 0;
}

int MC6809::brk()
{
  return -1;
}

int MC6809::new_array(uint8_t type)
{
  return -1;
}

int MC6809::insert_array(const char *name, int32_t *data, int len, uint8_t type)
{
  if (type == TYPE_BYTE)
  { return insert_db(name, data, len, TYPE_SHORT); }
    else
  if (type == TYPE_SHORT)
  { return insert_dw(name, data, len, TYPE_SHORT); }
    else
  if (type == TYPE_INT)
  { return insert_dw(name, data, len, TYPE_SHORT); }

  return -1;
}

int MC6809::insert_string(const char *name, uint8_t *bytes, int len)
{
  return -1;
}

int MC6809::push_array_length()
{
  fprintf(out, "  ; push_array_length()\n");
  fprintf(out, "  ldy ,s\n");
  fprintf(out, "  ldd -2,y\n");
  fprintf(out, "  std ,s\n");

  return 0;
}

int MC6809::push_array_length(const char *name, int field_id)
{
  fprintf(out, "  ; push_array_length()\n");
  fprintf(out, "  ldy [name]\n");
  fprintf(out, "  ldd -2,y\n");
  fprintf(out, "  pshs a,b\n");

  return 0;
}

int MC6809::array_read_byte()
{
  fprintf(out, "  ; array_read_byte()\n");
  fprintf(out, "  puls a,b\n");
  fprintf(out, "  addd ,s\n");
  fprintf(out, "  tfr d,x\n");
  fprintf(out, "  ldb ,x\n");
  fprintf(out, "  sex\n");
  fprintf(out, "  std ,s\n");
  return 0;
}

int MC6809::array_read_short()
{
  fprintf(out, "  ; array_read_short()\n");
  fprintf(out, "  puls a,b\n");
  fprintf(out, "  lslb\n");
  fprintf(out, "  rola\n");
  fprintf(out, "  addd ,s\n");
  fprintf(out, "  tfr d,x\n");
  fprintf(out, "  ldd ,x\n");
  fprintf(out, "  std ,s\n");
  return 0;
}

int MC6809::array_read_int()
{
  return array_read_short();
}

int MC6809::array_read_byte(const char *name, int field_id)
{
  fprintf(out, "  ; array_read_byte()\n");
  fprintf(out, "  ldy [name]\n");
  fprintf(out, "  addd ,s\n");
  fprintf(out, "  tfr d,x\n");
  fprintf(out, "  ldb ,x\n");
  fprintf(out, "  sex\n");
  fprintf(out, "  std ,s\n");
  return 0;
}

int MC6809::array_read_short(const char *name, int field_id)
{
  fprintf(out, "  ; array_read_short()\n");
  fprintf(out, "  puls a,b\n");
  fprintf(out, "  lslb\n");
  fprintf(out, "  rola\n");
  fprintf(out, "  addd ,s\n");
  fprintf(out, "  tfr d,x\n");
  fprintf(out, "  ldd ,x\n");
  fprintf(out, "  std ,s\n");
  return 0;
}

int MC6809::array_read_int(const char *name, int field_id)
{
  return array_read_short(name, field_id);
}

int MC6809::array_write_byte()
{
  return -1;
}

int MC6809::array_write_short()
{
  return -1;
}

int MC6809::array_write_int()
{
  return array_write_short();
}

int MC6809::array_write_byte(const char *name, int field_id)
{
  return -1;
}

int MC6809::array_write_short(const char *name, int field_id)
{
  return -1;
}

int MC6809::array_write_int(const char *name, int field_id)
{
  return array_write_short();
}

void MC6809::add_multiply()
{
  // HH2 LL3
  // HH4 LL5
  //
  //   A           B
  // x C           D
  // ------------------
  //  AD(L)+BD(H) BD(L)
  //  CB          0
  // +

  fprintf(out, "_multiply:\n");
  fprintf(out, "  lda 3,s\n");
  fprintf(out, "  ldb 5,s\n");
  fprintf(out, "  mul\n");
  fprintf(out, "  tfr d,x\n");
  fprintf(out, "  lda 2,s\n");
  fprintf(out, "  ldb 5,s\n");
  fprintf(out, "  mul\n");
  fprintf(out, "  tfr d,y\n");
  fprintf(out, "  lda 3,s\n");
  fprintf(out, "  ldb 4,s\n");
  fprintf(out, "  mul\n");
  fprintf(out, "  stb 4,s\n");
  fprintf(out, "  tfr x,d\n");
  fprintf(out, "  stb 5,s\n");
  fprintf(out, "  adda 4,s\n");
  fprintf(out, "  sta 4,s\n");
  fprintf(out, "  tfr y,d\n");
  fprintf(out, "  addb 4,s\n");
  fprintf(out, "  stb 4,s\n");
  fprintf(out, "  rts\n");
}

