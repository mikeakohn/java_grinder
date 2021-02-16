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
// r3 points to locals
// r4
// r5 points to heap
// r6 SP (stack write/inc with push, dec/read with pop)
// r7 PC
//
// 0x0100 Interrupt pointer.
// 0x0102 Scratchpad RAM start (EXEC uses this?)
// 0x0102 VerticalBlank interrupt flag
// 0x0103 Horizontal Delay
// 0x0104 Vertical Delay
// 0x0105 Video mode
// 0x0106-0x0109 Color Stack
// 0x015d Scratchpad RAM general area start
// 0x01ef Scratchpad RAM end
//
// 0x0200-0x02ef BACKTAB (20x12 words)

Intellivision::Intellivision() :
  is_main(false),
  need_multiply(false),
  current_local_count(0)
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

int Intellivision::finish()
{
  if (need_multiply) { add_mul(); }

  fprintf(out,
    "  _vblank_interrupt:\n"
    "  ; Enable display with STIC.\n"
    "  mvo r0, 0x0020\n"
    "  ; Set did_vblank to 1.\n"
    "  mvii #1, r0\n"
    "  mvo r0, 0x0102\n"
    "  ; border color\n"
    "  mvii #0, r0\n"
    "  mvo r0, 0x002c\n"
    "  ; Horizontal / Vertical delay.\n"
    "  mvi 0x0103, r0\n"
    "  mvo r0, 0x0030\n"
    "  mvi 0x0104, r0\n"
    "  mvo r0, 0x0031\n"
    "  ; Color stack\n"
    "  mvi 0x0106, r0\n"
    "  mvo r0, 0x0028\n"
    "  mvi 0x0107, r0\n"
    "  mvo r0, 0x0029\n"
    "  mvi 0x0108, r0\n"
    "  mvo r0, 0x002a\n"
    "  mvi 0x0109, r0\n"
    "  mvo r0, 0x002b\n"
    "  ; Video mode.\n"
    "  mvi 0x0105, r0\n"
    "  cmpi #1, r0\n"
    "  bneq _skip_mode_write\n"
    "  mvo r0, 0x0021\n"
    "  j _finish_mode_strobe\n"
    "_skip_mode_write:\n"
    "  mvi 0x0021, r0\n"
    "_finish_mode_strobe:\n"
    "  ;; Return from interrupt.\n"
    "  jr r5\n");

  return 0;
}

int Intellivision::start_init()
{
  // Add any set up items (stack, registers, etc).
  fprintf(out, ".define BIDECLE(param_) .dc16 param_ & 0xff, param_ >> 8\n");

  fprintf(out,
    ".org 0x57ff\n"
    "  .dc16 0x0000\n\n"
    ".org 0x5000\n\n");

  fprintf(out,
    "rom_header:\n"
    "  BIDECLE(_zero)      ; MOB picture base   (points to NULL list)\n"
    "  BIDECLE(_zero)      ; Process table      (points to NULL list)\n"
    "  BIDECLE(_start)     ; Program start address\n"
    "  BIDECLE(_zero)      ; Bkgnd picture base (points to NULL list)\n"
    "  BIDECLE(_colors)    ; GRAM pictures      (points to NULL list)\n"
    "  BIDECLE(_title)     ; Cartridge title/date\n"
    "  .dc16 0x03c0        ; Flags:  No ECS title, run code after title,\n"
    "_zero:\n"
    "  .dc16 0x0000        ; Screen border control\n"
    "  .dc16 0x0000        ; 0 = color stack, 1 = f/b mode\n"
    //"  BIDECLE(_ones)      ; GRAM pictures      (points to NULL list)\n"
    "_colors:\n"
    "  .dw 1, 1, 1, 1, 1   ; Color stack initialization\n\n");

  // FIXME: 121 = 2021.. Should this be configurable?
  fprintf(out,
    "_title:\n"
    "  .dc16 121, 'J', 'A', 'V', 'A', 0\n");

  fprintf(out,
    "_start:\n"
    "  ; Setup interrupt.\n"
    "  mvii #_vblank_interrupt, r0\n"
    "  mvo r0, 0x100\n"
    "  swap r0\n"
    "  mvo r0, 0x101\n"
    "  mvii #0, r0\n"
    "  mvo r0, 0x0102\n"
    "  mvo r0, 0x0103\n"
    "  mvo r0, 0x0104\n"
    "  mvii #0x0, r0\n"
    "  mvo r0, 0x0105\n"
    "  mvii #0x1, r1\n"
    "  mvii #0x0106, r4\n"
    "  mvo@ r1, r4\n"
    "  mvo@ r1, r4\n"
    "  mvo@ r1, r4\n"
    "  mvo@ r1, r4\n"
    "  ; Point r3 to local variables\n"
    "  mvii #ram_start, r3\n"
    "  eis\n");

  return 0;
}

int Intellivision::insert_static_field_define(
  std::string &name,
  std::string &type,
  int index)
{
  fprintf(out, "static_%s equ ram_end-%d\n", name.c_str(), (index + 1) * 2);

  return 0;
}

int Intellivision::init_heap(int field_count)
{
  fprintf(out,
    "  ; Set up heap and static initializers\n"
    "  mvii #ram_end-%d, r5\n",
    field_count * 2);

  return -1;
}

int Intellivision::field_init_int(std::string &name, int index, int value)
{
  if (index == 0)
  {
    fprintf(out,
      "  ; field_init_int(%s, index=%d, value=%d)\n"
      "  sdbd\n"
      "  mvii #(ram_end-%d) & 0xff, r4\n"
      "  .dc16 (ram_end-%d) >> 8\n"
      "  xorr r1, r1\n"
      "  mvo@ r1, r4\n"
      "  mvo@ r1, r4\n",
      name.c_str(),
      index,
      value,
      (index + 1) * 2,
      (index + 1) * 2);
  }
    else
  {
    fprintf(out,
      "  mvii #ram_end-%d, r4\n"
      "  mvii #%d, r0\n"
      "  mvii #%d, r1\n"
      "  mvo@ r0, r4\n"
      "  mvo@ r1, r4\n",
      (index + 1) * 2,
      value & 0xff,
      (value >> 8) & 0xff);
  }

  return 0;
}

int Intellivision::field_init_ref(std::string &name, int index)
{
  fprintf(out,
    "  ; field_init_ref(%s, %d)\n"
    "  sdbd\n"
    "  mvii #static_%s & 0xff, r4\n"
    "  .dc16 static_%s >> 8\n"
    "  sdbd\n"
    "  mvii #_%s & 0xff, r0\n"
    "  .dc16 _%s >> 8\n"
    "  mvo@ r0, r4\n"
    "  swap r0\n"
    "  mvo@ r0, r4\n",
    name.c_str(),
    index,
    name.c_str(),
    name.c_str(),
    name.c_str(),
    name.c_str());

  return 0;
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

  if (!is_main)
  {
    fprintf(out, "  pshr r4\n");
  }

  current_local_count = local_count;
}

void Intellivision::method_end(int local_count)
{
  fprintf(out, "\n");
}

int Intellivision::push_local_var_int(int index)
{
  if (index == 0)
  {
    fprintf(out,
      "  ; push_local_var_int(%d)\n"
      "  movr r3, r4\n"
      "  sdbd\n"
      "  mvi@ r4, r0\n"
      "  pshr r0\n",
      index);
  }
    else
  {
    fprintf(out,
      "  ; push_local_var_int(%d)\n"
      "  movr r3, r4\n"
      "  addi #%d, r4\n"
      "  sdbd\n"
      "  mvi@ r4, r0\n"
      "  pshr r0\n",
      index,
      index * 2);
  }

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
  if (n >= 0 && n <= 0x3ff)
  {
    fprintf(out,
      "  ; push_int(%d)\n"
      "  mvii #0x%04x, r0\n"
      "  pshr r0\n",
      n,
      n);
  }
    else
  {
    fprintf(out,
      "  ; push_int(%d)\n"
      "  sdbd\n"
      "  mvii #0x%04x, r0\n"
      "  .dc16 0x%04x\n"
      "  pshr r0\n",
      n,
      n & 0xff,
      (n >> 8) & 0xff);
  }

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
  fprintf(out,
    "  ; push_ref(%s)\n"
    "  sdbd\n"
    "  mvii #static_%s & 0xff, r4\n"
    "  .dc16 static_%s >> 8\n"
    "  sdbd\n"
    "  mvi@ r4, r0\n"
    "  pshr r0\n",
    name.c_str(),
    name.c_str(),
    name.c_str());

  return 0;
}

int Intellivision::pop_local_var_int(int index)
{
  if (index == 0)
  {
    fprintf(out,
      "  ; pop_local_var_int(%d)\n"
      "  pulr r1\n"
      "  movr r3, r4\n"
      "  mvo@ r1, r4\n"
      "  swap r1\n"
      "  mvo@ r1, r4\n",
      index);
  }
    else
  {
    fprintf(out,
      "  ; pop_local_var_int(%d)\n"
      "  movr r3, r4\n"
      "  addi #%d, r4\n"
      "  pulr r1\n"
      "  mvo@ r1, r4\n"
      "  swap r1\n"
      "  mvo@ r1, r4\n",
      index,
      index * 2);
  }

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
    "  pulr r0\n"
    "  pshr r0\n"
    "  pshr r0\n");

  return 0;
}

int Intellivision::dup2()
{
  fprintf(out,
    "  ; dup2()\n"
    "  pulr r0\n"
    "  pulr r1\n"
    "  pshr r1\n"
    "  pshr r0\n"
    "  pshr r1\n"
    "  pshr r0\n");

  return 0;
}

int Intellivision::swap()
{
  fprintf(out,
    "  ; swap()\n"
    "  pulr r0\n"
    "  pulr r1\n"
    "  pshr r0\n"
    "  pshr r1\n");

  return 0;
}

int Intellivision::add_integer()
{
  fprintf(out,
    "  ; add_integer()\n"
    "  pulr r0\n"
    "  add@ r6, r0\n"
    "  pshr r0\n");

  return 0;
}

int Intellivision::add_integer(int num)
{
  if (num < 0 || num > 255) { return -1; }

  fprintf(out,
    "  ; add_integer(%d)\n"
    "  mvii #%d, r0\n"
    "  add@ r6, r0\n"
    "  pshr r0\n",
    num,
    num & 0xffff);

  return 0;
}

int Intellivision::sub_integer()
{
  fprintf(out,
    "  ; sub_integer()\n"
    "  pulr r0\n"
    "  pulr r1\n"
    "  subr r0, r1\n"
    "  pshr r1\n");

  return 0;
}

int Intellivision::sub_integer(int num)
{
  if (num < 0 || num > 255) { return -1; }

  fprintf(out,
    "  ; sub_integer(%d)\n"
    "  pulr r0\n"
    //"  mvii #%d, r1\n"
    //"  subr r1, r0\n"
    "  subi #%d, r0\n"
    "  pshr r0\n",
    num,
    num & 0xffff);

  return 0;
}

int Intellivision::mul_integer()
{
  need_multiply = true;

  fprintf(out,
    "  ; mul_integer()\n"
    "  jsr r4, _multiply\n");

  return 0;
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
    "  pulr r0\n"
    "  negr r0\n"
    "  pshr r0\n");

  return 0;
}

int Intellivision::shift_left_integer()
{
  fprintf(out,
    "  ; shift_left_integer()\n"
    "  pulr r1\n"
    "  pulr r0\n"
    "label_%d:\n"
    "  sll r0\n"
    "  decr r1\n"
    "  bneq label_%d\n"
    "  pshr r0\n",
    label_count,
    label_count);

  label_count++;

  return 0;
}

int Intellivision::shift_left_integer(int num)
{
  // FIXME: Optimize further (along with the other shifts).
  fprintf(out,
    "  ; shift_left_integer(%d)\n"
    "  pulr r0\n",
    num);

  if (num < 8)
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
  if (num == 8)
  {
    fprintf(out,
      "  and #0xff, r0\n"
      "  swap r0\n");
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

  fprintf(out, "  pshr r0\n");

  return 0;
}

int Intellivision::shift_right_integer()
{
  fprintf(out,
    "  ; shift_right_integer()\n"
    "  pulr r1\n"
    "  pulr r0\n"
    "label_%d:\n"
    "  sar r0\n"
    "  decr r1\n"
    "  bneq label_%d\n"
    "  pshr r0\n",
    label_count,
    label_count);

  label_count++;

  return 0;
}

int Intellivision::shift_right_integer(int num)
{
  fprintf(out,
    "  ; shift_right_integer(%d)\n"
    "  pulr r0\n",
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

  fprintf(out, "  pshr r0\n");

  return 0;
}

int Intellivision::shift_right_uinteger()
{
  fprintf(out,
    "  ; shift_right_uinteger()\n"
    "  pulr r1\n"
    "  pulr r0\n"
    "label_%d:\n"
    "  slr r0\n"
    "  decr r1\n"
    "  bneq label_%d\n"
    "  pshr r0\n",
    label_count,
    label_count);

  label_count++;

  return 0;
}

int Intellivision::shift_right_uinteger(int num)
{
  fprintf(out,
    "  ; shift_right_uinteger(%d)\n"
    "  pulr r0\n",
    num);

  if (num < 8)
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
  if (num == 8)
  {
    fprintf(out,
      "  swap r0\n"
      "  andi #0xff, r0");
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

  fprintf(out, "  pshr r0\n");

  return 0;
}

int Intellivision::and_integer()
{
  fprintf(out,
    "  ; add_integer()\n"
    "  pulr r0\n"
    "  and@ r6, r0\n"
    "  pshr r0\n");

  return 0;
}

int Intellivision::and_integer(int num)
{
  if (num < 0 || num > 255) { return -1; }

  fprintf(out,
    "  ; and_integer(0x%04x)\n"
    "  pulr r0\n"
    "  mvii #0x%04x, r1\n"
    "  andr r0, r1\n"
    "  pshr r1\n",
    num & 0xffff,
    num & 0xffff);

  return 0;
}

int Intellivision::or_integer()
{
  fprintf(out,
    "  ; or_integer()\n"
    "  pulr r0\n"
    "  pulr r1\n"
    "  comr r0\n"
    "  comr r1\n"
    "  andr r1, r0\n"
    "  comr r0\n"
    "  pshr r0\n");

  return 0;
}

int Intellivision::or_integer(int num)
{
  if (num < 0 || num > 255) { return -1; }

  fprintf(out,
    "  ; or_integer(0x%04x)\n"
    "  pulr r0\n"
    "  mvii #0x%04x, r1\n"
    "  comr r0\n"
    "  comr r1\n"
    "  andr r1, r0\n"
    "  comr r0\n"
    "  pshr r0\n",
    num & 0xffff,
    num & 0xffff);

  return 0;
}

int Intellivision::xor_integer()
{
  fprintf(out,
    "  ; add_integer()\n"
    "  pulr r0\n"
    "  xor@ r6, r0\n"
    "  pshr r0\n");

  return 0;
}

int Intellivision::xor_integer(int num)
{
  if (num < 0 || num > 255) { return -1; }

  fprintf(out,
    "  ; xor_integer(0x%04x)\n"
    "  pulr r0\n"
    "  mvii #0x%04x, r1\n"
    "  xorr r0, r1\n"
    "  pshr r1\n",
    num & 0xffff,
    num & 0xffff);

  return 0;
}

int Intellivision::inc_integer(int index, int num)
{
  const char *operation = "add";

  if (num < 0)
  {
    num = -num;
    operation = "sub";
  }

  if (index == 0)
  {
    fprintf(out,
      "  ; inc_integer(%d)\n"
      "  movr r3, r4\n"
      "  sdbd\n"
      "  mvi@ r4, r1\n"
      "  %si #%d, r1\n"
      "  movr r3, r4\n"
      "  mvo@ r1, r4\n"
      "  swap r1\n"
      "  mvo@ r1, r4\n",
      num,
      operation,
      num & 0xfff);
  }
    else
  {
    fprintf(out,
      "  ; inc_integer(%d)\n"
      "  movr r3, r4\n"
      "  addi #%d, r4\n"
      "  sdbd\n"
      "  mvi@ r4, r1\n"
      "  %si #%d, r1\n"
      "  movr r3, r4\n"
      "  addi #%d, r4\n"
      "  mvo@ r1, r4\n"
      "  swap r1\n"
      "  mvo@ r1, r4\n",
      num,
      index * 2,
      operation,
      num & 0xffff,
      index * 2);
  }

  return 0;
}

int Intellivision::integer_to_byte()
{
  fprintf(out,
    "  ; integer_to_byte()\n"
    "  pulr r0\n"
    "  movr r0, r1\n"
    "  andi #0x80, r1\n"
    "  beq label_%d\n"
    "  andi #0x00ff, r0\n"
    "  xori #0xff00, r0\n"
    "label_%d\n"
    "  pshr r0\n",
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
  fprintf(out, "  ;; jump_cond(%s, cond=%d, distance=%d)\n",
    label.c_str(), cond, distance);

  fprintf(out,
    "  pulr r1\n"
    "  cmpi #0, r1\n");

#if 0
  fprintf(out,
    "  xorr r0, r0\n"
    "  pulr r1\n"
    "  cmpr r0, r1\n");
#endif

  switch (cond)
  {
    case COND_EQUAL:
      fprintf(out, "  beq %s\n", label.c_str());
      return 0;
    case COND_NOT_EQUAL:
      fprintf(out, "  bneq %s\n", label.c_str());
      return 0;
    case COND_LESS:
      fprintf(out, "  blt %s\n", label.c_str());
      return 0;
    case COND_LESS_EQUAL:
      fprintf(out, "  ble %s\n", label.c_str());
      return 0;
    case COND_GREATER:
      fprintf(out, "  bgt %s\n", label.c_str());
      return 0;
    case COND_GREATER_EQUAL:
      fprintf(out, "  bge %s\n", label.c_str());
      return 0;
    default:
      return -1;
  }

  return -1;
}

int Intellivision::jump_cond_integer(std::string &label, int cond, int distance)
{
  fprintf(out, "  ;; jump_cond_integer(%s, cond=%d, distance=%d)\n",
    label.c_str(), cond, distance);

  fprintf(out,
    "  pulr r0\n"
    "  pulr r1\n"
    "  cmpr r0, r1\n");

  switch (cond)
  {
    case COND_EQUAL:
      fprintf(out, "  beq %s\n", label.c_str());
      return 0;
    case COND_NOT_EQUAL:
      fprintf(out, "  bneq %s\n", label.c_str());
      return 0;
    case COND_LESS:
      fprintf(out, "  blt %s\n", label.c_str());
      return 0;
    case COND_LESS_EQUAL:
      fprintf(out, "  ble %s\n", label.c_str());
      return 0;
    case COND_GREATER:
      fprintf(out, "  bgt %s\n", label.c_str());
      return 0;
    case COND_GREATER_EQUAL:
      fprintf(out, "  bge %s\n", label.c_str());
      return 0;
    default:
      return -1;
  }

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
  fprintf(out,
    "  ; return_integer(index=%d, local_count=%d)\n",
    index,
    local_count);

  if (index == 0)
  {
    fprintf(out,
      "  movr r3, r4\n"
      "  sdbd\n"
      "  mvi@ r4, r0\n");
  }
    else
  {
    fprintf(out,
      "  movr r3, r4\n"
      "  addi #%d, r4\n"
      "  sdbd\n"
      "  mvi@ r4, r0\n",
      index * 2);
  }

  fprintf(out, "  pulr r7\n");

  return 0;
}

int Intellivision::return_integer(int local_count)
{
  fprintf(out,
    "  ; return_integer(local_count=%d)\n"
    "  pulr r0\n"
    "  pulr r7\n",
    local_count);

  return 0;
}

int Intellivision::return_void(int local_count)
{
  fprintf(out,
    "  ; return_void(local_count=%d)\n"
    "  pulr r7\n",
    local_count);

  return 0;
}

int Intellivision::jump(std::string &name, int distance)
{
  fprintf(out,
    "  ; jump(%s, %d)\n"
    "  j %s\n",
    name.c_str(),
    distance,
    name.c_str());

  return 0;
}

int Intellivision::call(std::string &name)
{
  return -1;
}

int Intellivision::invoke_static_method(
  const char *name,
  int params,
  int is_void)
{
  int n;

  fprintf(out,
    "  ; invoke_static_method(%s, %d, is_void=%d)\n",
    name,
    params,
    is_void);

  // Copy locals pointer to r4.
  // Copy stack pointer to r5.
  fprintf(out,
    "  ; Copy locals pointer to r4 and add current local length\n"
    "  ; Copy stack pointer to r5 - number of parameters to copy\n"
    "  movr r3, r4\n"
    "  addi #%d, r4\n"
    "  movr r6, r5\n"
    "  subi #%d, r5\n",
    current_local_count * 2,
    params);

  // Copy params from stack to locals.
  for (n = 0; n < params; n++)
  {
    fprintf(out,
      "  ; Setup param %d (local %d)\n"
      "  mvi@ r5, r1\n"
      "  mvo@ r1, r4\n"
      "  swap r1\n"
      "  mvo@ r1, r4\n",
      n, n);
  }

  // Fix stack, push current locals pointer and adjust for new locals.
  fprintf(out,
    "  ; Pop paramters off the stack, push local pointer, and move\n"
    "  ; pointer past current locals.\n"
    "  subi #%d, r6\n"
    "  pshr r3\n"
    "  addi #%d, r3\n",
    params,
    current_local_count * 2);

  // Call the function and restore locals.
  fprintf(out,
    "  ; Call function then restore locals pointer.\n"
    "  jsr r4, %s\n"
    "  pulr r3\n",
    name);

  // Push return value to stack.
  if (!is_void)
  {
    fprintf(out, "  pshr r0\n");
  }

  return 0;
}

int Intellivision::put_static(std::string &name, int index)
{
  fprintf(out,
    "  ; put_static(%s, %d)\n"
    "  sdbd\n"
    "  mvii #(ram_end-%d) & 0xff, r4\n"
    "  .dc16 (ram_end-%d) >> 8\n"
    "  pulr r1\n"
    "  mvo@ r1, r4\n"
    "  swap r1\n"
    "  mvo@ r1, r4\n",
    name.c_str(),
    index,
    (index + 1) * 2,
    (index + 1) * 2);

  return 0;
}

int Intellivision::get_static(std::string &name, int index)
{
  fprintf(out,
    "  ; get_static(%s, %d)\n"
    "  mvii #ram_end-%d, r4\n"
    "  sdbd\n"
    "  mvi@ r4, r0\n"
    "  pshr r0\n",
    name.c_str(),
    index,
    (index + 1) * 2);

  return 0;
}

int Intellivision::brk()
{
  fprintf(out,
    "  ; brk()\n"
    "  hlt\n");

  return 0;
}

int Intellivision::new_array(uint8_t type)
{
  return -1;
}

int Intellivision::insert_array(
  std::string &name,
  int32_t *data,
  int len,
  uint8_t type)
{
  if (type == TYPE_BYTE)
  {
    return insert_dw(name, data, len, TYPE_SHORT);
  }
    else
  if (type == TYPE_SHORT)
  {
    fprintf(out,
      "  dc16 %d   ; %s.length\n"
      "_%s:\n",
      len, name.c_str(),
      name.c_str());

    for (int n = 0; n < len; n++)
    {
      if ((n % 4) == 0) { fprintf(out, "  .dc16"); }
      else { fprintf(out, ","); }

      uint16_t value = data[n];

      fprintf(out, " 0x%04x, 0x%04x", value & 0xff, value >> 8);

      if (((n + 1) % 4) == 0) { fprintf(out, "\n"); }
    }

    fprintf(out, "\n");

    return 0;
  }

  return -1;
}

int Intellivision::insert_string(std::string &name, uint8_t *bytes, int len)
{
  fprintf(out,
    "  .dc16 %d   ; %s.length\n"
    "_%s:\n",
    len, name.c_str(),
    name.c_str());

  for (int n = 0; n < len; n++)
  {
    if ((n % 8) == 0) { fprintf(out, "  .dc16"); }
    else { fprintf(out, ","); }

    fprintf(out, " 0x%04x", bytes[n]);

    if (((n + 1) % 8) == 0) { fprintf(out, "\n"); }
  }

  fprintf(out, "\n");

  return 0;
}

int Intellivision::push_array_length()
{
  fprintf(out,
    "  ; push_array_length()\n"
    "  pulr r4\n"
    "  subi #1, r4\n"
    "  mvi@ r4, r0\n"
    "  pshr r0\n");

  return 0;
}

int Intellivision::push_array_length(std::string &name, int field_id)
{
  fprintf(out,
    "  ; push_array_length(%s, %d)\n"
    "  sdbd\n"
    "  mvii #%s & 0xff, r4\n"
    "  .dc16 %s >> 8\n"
    "  subi #2, r4\n"
    "  sdbd\n"
    "  mvi@ r4, r0\n"
    "  pshr r0\n",
    name.c_str(),
    field_id,
    name.c_str(),
    name.c_str());

  return 0;
}

int Intellivision::array_read_byte()
{
  fprintf(out,
    "  ; array_read_byte()\n"
    "  pulr r0\n"
    "  pulr r4\n"
    "  addr r0, r4\n"
    "  mvi@ r4, r0\n"
    "  pshr r0\n");

  return 0;
}

int Intellivision::array_read_short()
{
  fprintf(out,
    "  ; array_read_short()\n"
    "  pulr r0\n"
    "  pulr r4\n"
    "  sll r0\n"
    "  addr r0, r4\n"
    "  sdbd\n"
    "  mvi@ r4, r0\n"
    "  pshr r0\n");

  return 0;
}

int Intellivision::array_read_int()
{
  return array_read_short();
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
  return array_read_short(name, field_id);
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

int Intellivision::intellivision_plot_II()
{
  fprintf(out,
    "  ; intellivision_plot_II()\n"
    "  pulr r2\n"
    "  addi #0x200, r2\n"
    "  pulr r1\n"
    "  mvo@ r1, r2\n");

  return 0;
}

int Intellivision::intellivision_plot_III()
{
  need_multiply = true;

  fprintf(out,
    "  ; intellivision_plot_III()\n"
    "  mvii #20, r0\n"
    "  pshr r0\n"
    "  jsr r4, _multiply\n"
    "  pulr r2\n"
    "  pulr r1\n"
    "  addi #0x200, r2\n"
    "  addr r1, r2\n"
    "  pulr r1\n"
    "  mvo@ r1, r2\n");

  return 0;
}

int Intellivision::intellivision_readDisplay_II()
{
  need_multiply = true;

  fprintf(out,
    "  ; intellivision_readDisplay_II()\n"
    "  mvii #20, r0\n"
    "  pshr r0\n"
    "  jsr r4, _multiply\n"
    "  pulr r2\n"
    "  pulr r1\n"
    "  addi #0x200, r2\n"
    "  addr r1, r2\n"
    "  mvi@ r2, r0\n"
    "  pshr r0\n");

  return 0;
}

int Intellivision::intellivision_waitForVerticalBlank()
{
  fprintf(out,
    "  ; intellivision_waitVerticalBlank()\n"
    "  _wait_for_vertical_blank_%d:\n"
    "  mvi 0x102, r0\n"
    "  cmpi #1, r0\n"
    "  bneq _wait_for_vertical_blank_%d\n"
    "  mvii #0, r0\n"
    "  mvo r0, 0x0102\n",
    label_count,
    label_count);

  label_count++;

  return 0;
}

int Intellivision::intellivision_setHorizontalDelay_I()
{
  fprintf(out,
    "  ; intellivision_setHorizontalDelay_I()\n"
    "  pulr r0\n"
    "  mvo r0, 0x0103\n");

  return 0;
}

int Intellivision::intellivision_setVerticalDelay_I()
{
  fprintf(out,
    "  ; intellivision_setVerticalDelay_I()\n"
    "  pulr r0\n"
    "  mvo r0, 0x0104\n");

  return 0;
}

int Intellivision::intellivision_setVideoMode_I()
{
  fprintf(out,
    "  ; intellivision_setVideoMode_I()\n"
    "  pulr r0\n"
    "  mvo r0, 0x0105\n");

  return 0;
}

int Intellivision::intellivision_setColorStack_II()
{
  fprintf(out,
    "  ; intellivision_setColorStack_II()\n"
    "  pulr r0\n"
    "  pulr r4\n"
    "  addi #0x0106, r4\n"
    "  mvo@ r0, r4\n");

  return 0;
}

int Intellivision::intellivision_getControllerLeft()
{
  fprintf(out,
    "  ; intellivision_getControllerLeft()\n"
    "  mvi 0x01ff, r1\n"
    "  xori #0xff, r1\n"
    "  pshr r1\n");

  return 0;
}

int Intellivision::intellivision_getControllerRight()
{
  fprintf(out,
    "  ; intellivision_getControllerRight()\n"
    "  mvi 0x01fe, r1\n"
    "  xori #0xff, r1\n"
    "  pshr r1\n");

  return 0;
}

int Intellivision::intellivision_setSoundChannelPeriod_II()
{
  fprintf(out,
    "  ; intellivision_setSoundChannelPeriod_II()\n"
    "  pulr r1\n"
    "  pulr r0\n"
    "  mvii #0x1f0, r2\n"
    "  addr r0, r2\n"
    "  mvo@ r1, r2\n"
    "  swap r1\n"
    "  addi #4, r2\n"
    "  mvo@ r1, r2\n");

  return 0;
}

int Intellivision::intellivision_setSoundChannelVolume_II()
{
  fprintf(out,
    "  ; intellivision_setSoundChannelVolume_II()\n"
    "  pulr r1\n"
    "  pulr r0\n"
    "  mvii #0x1fb, r2\n"
    "  addr r0, r2\n"
    "  mvo@ r1, r2\n");

  return 0;
}

int Intellivision::intellivision_setSoundEnvelopePeriod_I()
{
  fprintf(out,
    "  ; intellivision_setSoundEnvelopePeriod_I()\n"
    "  pulr r1\n"
    "  mvii #0x1f3, r2\n"
    "  mvo@ r1, r2\n"
    "  swap r1\n"
    "  addi #4, r2\n"
    "  mvo@ r1, r2\n");

  return 0;
}

int Intellivision::intellivision_setSoundEnvelopeType_I()
{
  fprintf(out,
    "  ; intellivision_setSoundEnvelopeType_I()\n"
    "  pulr r1\n"
    "  mvii #0x1fa, r2\n"
    "  mvo@ r1, r2\n");

  return 0;
}

int Intellivision::intellivision_setSoundNoisePeriod_I()
{
  fprintf(out,
    "  ; intellivision_setSoundNoisePeriod_I()\n"
    "  pulr r1\n"
    "  mvii #0x1f9, r2\n"
    "  mvo@ r1, r2\n");

  return 0;
}

int Intellivision::intellivision_setSoundType_I()
{
  fprintf(out,
    "  ; intellivision_setSoundType_I()\n"
    "  pulr r1\n"
    "  mvii #0x1f8, r2\n"
    "  mvo@ r1, r2\n");

  return 0;
}

void Intellivision::add_mul()
{
  fprintf(out,
    "  ;; Multiply\n"
    "_multiply:\n"
    "  pulr r0\n"
    "  pulr r1\n"
    "  xorr r2, r2\n"
    "_multiply_loop:\n"
    "  addr r0, r2\n"
    "  decr r1\n"
    "  bneq _multiply_loop\n"
    "  pshr r2\n"
    "  movr r4, r7\n\n");
}

