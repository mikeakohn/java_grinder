/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2021 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "generator/Intellivision.h"

// ABI is:
// r0 temp
// r1 temp
// r2
// r3
// r4 points to locals
// r5 points to heap
// r6 SP (stack write/inc with push, dec/read with pop)
// r7 PC
//
// 0x0102 Scratchpad RAM start (EXEC uses this?)
// 0x015d Scratchpad RAM general area start
// 0x01ef Scratchpad RAM end

Intellivision::Intellivision() : is_main(false)
{

}

Intellivision::~Intellivision()
{
}

int Intellivision::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".cp1610\n");

  // Set where RAM starts / ends
  fprintf(out, "ram_start equ 0x15d\n");
  fprintf(out, "ram_end equ 0x1ef\n");

  return 0;
}

int Intellivision::start_init()
{
  // Add any set up items (stack, registers, etc).
  fprintf(out, ".define BIDECLE(a) .dw a & 0xff, a >> 8\n");

  fprintf(out,
    ".org 0x57ff\n"
    "  .dw 0x0000\n\n"
    ".org 0x5000\n\n");

  fprintf(out,
    "rom_header:\n"
    "  BIDECLE(zero)       ; MOB picture base   (points to NULL list)\n"
    "  BIDECLE(zero)       ; Process table      (points to NULL list)\n"
    "  BIDECLE(start)      ; Program start address\n"
    "  BIDECLE(zero)       ; Bkgnd picture base (points to NULL list)\n"
    "  BIDECLE(ones)       ; GRAM pictures      (points to NULL list)\n"
    "  BIDECLE(title)      ; Cartridge title/date\n"
    "  .dw 0x03c0          ; Flags:  No ECS title, run code after title,\n"
    "zero:\n"
    "  .dw 0x0000          ; Screen border control\n"
    "  .dw 0x0000          ; 0 = color stack, 1 = f/b mode\n"
    "  BIDECLE(ones)       ; GRAM pictures      (points to NULL list)\n"
    "ones:\n"
    "  .dw 1, 1, 1, 1, 1   ; Color stack initialization\n\n");

  fprintf(out,
    "title:"
    "  .dw 107, 0\n");

  fprintf(out,
    "start:\n"
    "  mvii #ram_start, r6\n");

  // FIXME: Remove.
#if 0
  fprintf(out,
    "main:\n"
    "  eis\n"
    "while_1:\n"
    "  b while_1\n\n");
#endif

  return 0;
}

int Intellivision::insert_static_field_define(
  std::string &name,
  std::string &type,
  int index)
{
  fprintf(out, "  %s equ ram_end-%d\n", name.c_str(), index);

  return 0;
}

int Intellivision::init_heap(int field_count)
{
  fprintf(out,
    "  ; Set up heap and static initializers\n"
    "  mvii #ram_end-%d, r5\n",
    field_count);

  return -1;
}

int Intellivision::field_init_int(std::string &name, int index, int value)
{
  fprintf(out,
    "  mvii #ram_end-%d, r0\n"
    "  mvii #%d, r1\n"
    "  mvo@ r1, r0\n",
    index,
    value & 0xffff);

  return 0;
}

int Intellivision::field_init_ref(std::string &name, int index)
{
  return -1;
}

void Intellivision::method_start(
  int local_count,
  int max_stack,
  int param_count,
  std::string &name)
{
  is_main = name == "main";

  fprintf(out, "%s:\n", name.c_str());
  fprintf(out, "  ; %s(local_count=%d, param_count=%d)\n",
    name.c_str(),
    local_count,
    param_count);

}

void Intellivision::method_end(int local_count)
{
  fprintf(out, "\n");
}

int Intellivision::push_local_var_int(int index)
{
  fprintf(out,
    "  ; push_local_var_int(%d)\n"
    "  movr r4, r3\n"
    "  subi #%d, r3\n"
    "  mvi@ r3, r0\n"
    "  mvo@ r0, r6\n",
    index,
    index);

  return 0;
}

int Intellivision::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int Intellivision::push_ref_static(std::string &name, int index)
{
  return -1;
}

int Intellivision::push_fake()
{
  fprintf(out,
    "  ; push_fake()\n"
    "  incr r6\n");

  return 0;
}

int Intellivision::push_int(int32_t n)
{
  fprintf(out,
    "  ; push_int()\n"
    "  mvii #0x%04x, r0\n"
    "  mvo@ r0, r6\n",
    n & 0xffff);

  return 0;
}

#if 0
int Intellivision::push_long(int64_t n)
{
  return -1;
}

int Intellivision::push_float(float f)
{
  return -1;
}

int Intellivision::push_double(double f)
{
  return -1;
}
#endif

int Intellivision::push_ref(std::string &name)
{
  // Need to move the address of name to the top of stack
  return -1;
}

int Intellivision::pop_local_var_int(int index)
{
  fprintf(out,
    "  ; pop_local_var_int(%d)\n"
    "  movr r4, r3\n"
    "  subi #%d, r3\n"
    "  mvi@ r6, r0\n"
    "  mvo@ r0, r3\n",
    index,
    index);

  return 0;
}

int Intellivision::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int Intellivision::pop()
{
  fprintf(out,
    "  ; pop()\n"
    "  decr r6\n");

  return 0;
}

int Intellivision::dup()
{
  fprintf(out,
    "  ; dup()\n"
    "  mvi@ r6, r0\n"
    "  mvo@ r0, r6\n"
    "  mvo@ r0, r6\n");

  return 0;
}

int Intellivision::dup2()
{
  fprintf(out,
    "  ; dup2()\n"
    "  mvi@ r6, r0\n"
    "  mvi@ r6, r1\n"
    "  mvo@ r1, r6\n"
    "  mvo@ r0, r6\n"
    "  mvo@ r1, r6\n"
    "  mvo@ r0, r6\n");

  return 0;
}

int Intellivision::swap()
{
  fprintf(out,
    "  ; swap()\n"
    "  mvi@ r6, r0\n"
    "  mvi@ r6, r1\n"
    "  mvo@ r0, r6\n"
    "  mvo@ r1, r6\n");

  return 0;
}

int Intellivision::add_integer()
{
  fprintf(out,
    "  ; add_integer()\n"
    "  mvi@ r6, r0\n"
    "  add@ r6, r0\n"
    "  mvo@ r0, r6\n");

  return 0;
}

int Intellivision::add_integer(int num)
{
  fprintf(out,
    "  ; add_integer(%d)\n"
    "  mvii #%d, r0\n"
    "  add@ r6, r0\n"
    "  mvo@ r0, r6\n",
    num,
    num & 0xffff);

  return 0;
}

int Intellivision::sub_integer()
{
  fprintf(out,
    "  ; sub_integer()\n"
    "  mvi@ r6, r0\n"
    "  mvi@ r6, r1\n"
    "  subr r0, r1\n"
    "  mvo@ r1, r6\n");

  return 0;
}

int Intellivision::sub_integer(int num)
{
  fprintf(out,
    "  ; sub_integer(%d)\n"
    "  mvi@ r6, r0\n"
    "  mvii #%d, r1\n"
    "  subr r0, r1\n"
    "  mvo@ r1, r6\n",
    num,
    num & 0xffff);

  return 0;
}

int Intellivision::mul_integer()
{
  return -1;
}

int Intellivision::div_integer()
{
  return -1;
}

int Intellivision::mod_integer()
{
  return -1;
}

int Intellivision::neg_integer()
{
  fprintf(out,
    "  ; neg_integer()\n"
    "  mvi@ r6, r0\n"
    "  neg r0\n"
    "  mvo@ r0, r6\n");

  return 0;
}

int Intellivision::shift_left_integer()
{
  fprintf(out,
    "  ; shift_left_integer()\n"
    "  mvi@ r6, r1\n"
    "  mvi@ r6, r0\n"
    "label_%d:\n"
    "  sll r0\n"
    "  decr r1\n"
    "  bneq label_%d\n"
    "  mvo@ r0, r6\n",
    label_count,
    label_count);

  label_count++;

  return 0;
}

int Intellivision::shift_left_integer(int num)
{
  fprintf(out,
    "  ; shift_left_integer(%d)\n"
    "  mvi@ r6, r0\n",
    num);

  if (num <= 8)
  {
    for (int n = 0; n < num; n = n + 2)
    {
      if (n >= 2)
      {
        fprintf(out, "  sll r0, 2\n");
      }
        else
      {
        fprintf(out, "  sll r0, 1\n");
      }
    }
  }
    else
  {
    fprintf(out,
      "  mvii #%d, r0\n"
      "label_%d:\n"
      "  sll r0\n"
      "  decr r1\n"
      "  bneq label_%d\n",
      num,
      label_count,
      label_count);

    label_count++;
  }

  fprintf(out, "  mvi@ r6, r0\n");

  return 0;
}

int Intellivision::shift_right_integer()
{
  fprintf(out,
    "  ; shift_right_integer()\n"
    "  mvi@ r6, r1\n"
    "  mvi@ r6, r0\n"
    "label_%d:\n"
    "  sar r0\n"
    "  decr r1\n"
    "  bneq label_%d\n"
    "  mvo@ r0, r6\n",
    label_count,
    label_count);

  label_count++;

  return 0;
}

int Intellivision::shift_right_integer(int num)
{
  fprintf(out,
    "  ; shift_right_integer(%d)\n"
    "  mvi@ r6, r0\n",
    num);

  if (num <= 8)
  {
    for (int n = 0; n < num; n = n + 2)
    {
      if (n >= 2)
      {
        fprintf(out, "  sar r0, 2\n");
      }
        else
      {
        fprintf(out, "  sar r0, 1\n");
      }
    }
  }
    else
  {
    fprintf(out,
      "  mvii #%d, r0\n"
      "label_%d:\n"
      "  sar r0\n"
      "  decr r1\n"
      "  bneq label_%d\n",
      num,
      label_count,
      label_count);

    label_count++;
  }

  fprintf(out, "  mvi@ r6, r0\n");

  return 0;
}

int Intellivision::shift_right_uinteger()
{
  fprintf(out,
    "  ; shift_right_uinteger()\n"
    "  mvi@ r6, r1\n"
    "  mvi@ r6, r0\n"
    "label_%d:\n"
    "  slr r0\n"
    "  decr r1\n"
    "  bneq label_%d\n"
    "  mvo@ r0, r6\n",
    label_count,
    label_count);

  label_count++;

  return 0;
}

int Intellivision::shift_right_uinteger(int num)
{
  fprintf(out,
    "  ; shift_right_uinteger(%d)\n"
    "  mvi@ r6, r0\n",
    num);

  if (num <= 8)
  {
    for (int n = 0; n < num; n = n + 2)
    {
      if (n >= 2)
      {
        fprintf(out, "  sar r0, 2\n");
      }
        else
      {
        fprintf(out, "  sar r0, 1\n");
      }
    }
  }
    else
  {
    fprintf(out,
      "  mvii #%d, r0\n"
      "label_%d:\n"
      "  sar r0\n"
      "  decr r1\n"
      "  bneq label_%d\n",
      num,
      label_count,
      label_count);

    label_count++;
  }

  fprintf(out, "  mvi@ r6, r0\n");

  return 0;
}

int Intellivision::and_integer()
{
  fprintf(out,
    "  ; add_integer()\n"
    "  mvi@ r6, r0\n"
    "  and@ r6, r0\n"
    "  mvo@ r0, r6\n");

  return 0;
}

int Intellivision::and_integer(int num)
{
  fprintf(out,
    "  ; and_integer(0x%04x)\n"
    "  mvi@ r6, r0\n"
    "  mvii #0x%04x, r1\n"
    "  andr r0, r1\n"
    "  mvo@ r1, r6\n",
    num & 0xffff,
    num & 0xffff);

  return 0;
}

int Intellivision::or_integer()
{
  fprintf(out,
    "  ; or_integer()\n"
    "  mvi@ r6, r0\n"
    "  mvi@ r6, r1\n"
    "  comr r0\n"
    "  comr r1\n"
    "  andr r1, r0\n"
    "  comr r0\n"
    "  mvo@ r0, r6\n");

  return 0;
}

int Intellivision::or_integer(int num)
{
  fprintf(out,
    "  ; or_integer(0x%04x)\n"
    "  mvi@ r6, r0\n"
    "  mvii #0x%04x, r1\n"
    "  comr r0\n"
    "  comr r1\n"
    "  andr r1, r0\n"
    "  comr r0\n"
    "  mvo@ r0, r6\n",
    num & 0xffff,
    num & 0xffff);

  return 0;
}

int Intellivision::xor_integer()
{
  fprintf(out,
    "  ; add_integer()\n"
    "  mvi@ r6, r0\n"
    "  xor@ r6, r0\n"
    "  mvo@ r0, r6\n");

  return 0;
}

int Intellivision::xor_integer(int num)
{
  fprintf(out,
    "  ; xor_integer(0x%04x)\n"
    "  mvi@ r6, r0\n"
    "  mvii #0x%04x, r1\n"
    "  xorr r0, r1\n"
    "  mvo@ r1, r6\n",
    num & 0xffff,
    num & 0xffff);

  return 0;
}

int Intellivision::inc_integer(int index, int num)
{
  fprintf(out,
    "  ; inc_integer(%d)\n"
    "  movr r4, r3\n"
    "  subi #%d, r3\n"
    "  addi #d, r0\n"
    "  mvo@ r0, r3\n",
    num,
    num & 0xffff);

  return 0;
}

int Intellivision::integer_to_byte()
{
  fprintf(out,
    "  ; integer_to_byte()\n"
    "  mvi@ r6, r0\n"
    "  movr r0, r1\n"
    "  andi #0x80, r1\n"
    "  beq label_%d\n"
    "  andi #0x00ff, r0\n"
    "  xori #0xff00, r0\n"
    "label_%d\n"
    "  mvo@ r0, r6\n",
    label_count,
    label_count);

  return 0;
}

int Intellivision::integer_to_short()
{
  // Nothing to sign extend since this chip is 16 bit.
  return 0;
}

int Intellivision::jump_cond(std::string &label, int cond, int distance)
{
  return -1;
}

int Intellivision::jump_cond_integer(std::string &label, int cond, int distance)
{
  return -1;
}

int Intellivision::ternary(int cond, int value_true, int value_false)
{
  return -1;
}

int Intellivision::ternary(int cond, int compare, int value_true, int value_false)
{
  return -1;
}

int Intellivision::return_local(int index, int local_count)
{
  return -1;
}

int Intellivision::return_integer(int local_count)
{
  return -1;
}

int Intellivision::return_void(int local_count)
{
  return -1;
}

int Intellivision::jump(std::string &name, int distance)
{
  return -1;
}

int Intellivision::call(std::string &name)
{
  return -1;
}

int Intellivision::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int Intellivision::put_static(std::string &name, int index)
{
  return -1;
}

int Intellivision::get_static(std::string &name, int index)
{
  return -1;
}

int Intellivision::brk()
{
  return -1;
}

int Intellivision::new_array(uint8_t type)
{
  return -1;
}

int Intellivision::insert_array(std::string &name, int32_t *data, int len, uint8_t type)
{
  if (type == TYPE_BYTE)
  {
    return insert_db(name, data, len, TYPE_INT);
  }
    else
  if (type == TYPE_SHORT)
  {
    return insert_dw(name, data, len, TYPE_INT);
  }
    else
  if (type == TYPE_INT)
  {
    return insert_dc32(name, data, len, TYPE_INT);
  }

  return -1;
}

int Intellivision::insert_string(std::string &name, uint8_t *bytes, int len)
{
  return -1;
}

int Intellivision::push_array_length()
{
  return -1;
}

int Intellivision::push_array_length(std::string &name, int field_id)
{
  return -1;
}

int Intellivision::array_read_byte()
{
  return -1;
}

int Intellivision::array_read_short()
{
  return -1;
}

int Intellivision::array_read_int()
{
  return -1;
}

int Intellivision::array_read_byte(std::string &name, int field_id)
{
  return -1;
}

int Intellivision::array_read_short(std::string &name, int field_id)
{
  return -1;
}

int Intellivision::array_read_int(std::string &name, int field_id)
{
  return -1;
}

int Intellivision::array_write_byte()
{
  return -1;
}

int Intellivision::array_write_short()
{
  return -1;
}

int Intellivision::array_write_int()
{
  return -1;
}

int Intellivision::array_write_byte(std::string &name, int field_id)
{
  return -1;
}

int Intellivision::array_write_short(std::string &name, int field_id)
{
  return -1;
}

int Intellivision::array_write_int(std::string &name, int field_id)
{
  return -1;
}

