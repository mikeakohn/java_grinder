/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2022 by Michael Kohn, Joe Davisson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "generator/MCS51.h"

#define LOCALS_LO(a) (a * 2)
#define LOCALS_HI(a) ((a * 2) + 1)
#define STATICS_LO(a) ((a * 2) + 16)
#define STATICS_HI(a) ((a * 2) + 17)

// ABI is:
// Bank 0:
//  0x00 r0: temp pointer
//  0x01 r1: temp pointer
//  0x02 r2: temp var
//  0x03 r3: temp var
//  0x04 r4: lo byte top of array heap
//  0x05 r5: hi byte top of array heap
//  0x06 r6: frame pointer
//  0x07 r7: global pointer
// Bank 1:
//  0x08 r0: top of java stack
//  0x09 r1:
//  0x0a r2:
//  0x0b r3:
//  0x0c r4:
//  0x0d r5:
//  0x0e r6:
//  0x0f r7: bottom of java stack
// SP = 0x10 to 0xff

MCS51::MCS51() :
  reg(0),
  reg_max(4),
  is_main(false),
  has_array(false)
{

}

MCS51::~MCS51()
{
}

int MCS51::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  //fprintf(out, ".CPU_PART\n");

  // Set where RAM starts / ends
  //fprintf(out, "ram_start equ 0\n");
  //fprintf(out, "ram_end equ 0x8000\n");
  fprintf(out, ".8051\n");

  //fprintf(out, "stack equ 0x20\n");
  //fprintf(out, "locals equ r2\n");

  return 0;
}

int MCS51::start_init()
{
  // Add any set up items (stack, registers, etc).
  fprintf(out, ".org 0x0000\n");
  fprintf(out, "start:\n");

  return 0;
}

int MCS51::insert_static_field_define(
  std::string &name,
  std::string &type,
  int index)
{
  fprintf(out, "%s equ %d\n", name.c_str(), 0x10 + (index * 2));
  return 0;
}

int MCS51::init_heap(int field_count)
{
  fprintf(out,
    "  ;; Setup heap and static initializers (field_count=%d)\n",
    field_count);

  fprintf(out, "  mov SP, #0x%02x\n", 0x0f + (field_count * 2));
  fprintf(out, "  mov r7, #0x10\n");

  return -1;
}

int MCS51::field_init_int(std::string &name, int index, int value)
{
  fprintf(out, "  ;; field_init_int(name=%s, index=%d, value=%d)\n",
    name.c_str(),
    index,
    value);

  if (index == 0)
  {
    fprintf(out, "  mov r0, r7\n");
  }
    else
  {
    fprintf(out,
      "  mov A, r7\n"
      "  add A, #%d\n"
      "  mov r0, A\n",
      index * 2);
  }

  fprintf(out,
      "  mov @r0, #0x%02x\n"
      "  inc r0\n"
      "  mov @r0, #0x%02x\n",
      value & 0xff,
      (value >> 8) & 0xff);

  return 0;
}

int MCS51::field_init_ref(std::string &name, int index)
{
  fprintf(out,
    "  mov %d, #(_%s - start_of_arrays) & 0xff\n"
    "  mov %d, #(_%s - start_of_arrays) >> 8\n",
    (16 + (index * 2)) + 0,
    name.c_str(),
    (16 + (index * 2)) + 1,
    name.c_str());

  return 0;
}

void MCS51::method_start(
  int local_count,
  int max_stack,
  int param_count,
  std::string &name)
{
  is_main = name == "main";
  reg = 0;

  fprintf(out, "%s:\n", name.c_str());

  // main() function goes here
  if (local_count != 0)
  {
    if (!is_main)
    {
      fprintf(out, "  push 6\n");
    }

    fprintf(out,
      "  mov r6, SP\n"
      "  inc r6\n"
      "  mov A, SP\n"
      "  add A, #%d\n"
      "  mov SP, A\n",
      local_count * 2);
  }
}

void MCS51::method_end(int local_count)
{
  fprintf(out, "\n");
}

int MCS51::push_local_var_int(int index)
{
  fprintf(out, "  ;; push_local_var_int(index=%d)\n", index);

  if (index == 0)
  {
    fprintf(out, "  mov r0, 6\n");
  }
    else
  {
    fprintf(out,
      "  mov A, r6\n"
      "  add A, #%d\n"
      "  mov r0, A\n",
      index * 2);
  }

  fprintf(out,
      "  mov %d, @r0\n"
      "  inc r0\n"
      "  mov %d, @r0\n",
      REG_ADDRESS_STACK_LO(reg),
      REG_ADDRESS_STACK_HI(reg));

  reg += 1;

  return 0;
}

int MCS51::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int MCS51::push_ref_static(std::string &name, int index)
{
  return -1;
}

int MCS51::push_fake()
{
  fprintf(out, "  ;; push_fake()\n");

  reg++;

  return 0;
}

int MCS51::push_int(int32_t n)
{
  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);
    return -1;
  }

  uint16_t value = (n & 0xffff);

  fprintf(out,
    "  ;; push_int(%d)\n"
    "  mov %d, #0x%02x\n"
    "  mov %d, #0x%02x\n",
    n,
    REG_ADDRESS_STACK_LO(reg), value & 0xff,
    REG_ADDRESS_STACK_HI(reg), value >> 8);

  reg++;

  return 0;
}

# if 0
int MCS51::push_long(int64_t n)
{
  return push_int((int32_t)n);
}

int MCS51::push_float(float f)
{
  return -1;
}

int MCS51::push_double(double f)
{
  return -1;
}
#endif

int MCS51::push_ref(std::string &name)
{
  fprintf(out,
    "  ;; push_ref(%s)\n"
    "  mov r0, #%s + 0\n"
    "  mov r1, #%s + 1\n"
    "  mov %d, @r0\n"
    "  mov %d, @r1\n",
    name.c_str(),
    name.c_str(),
    name.c_str(),
    REG_ADDRESS_STACK_LO(reg),
    REG_ADDRESS_STACK_HI(reg));

  reg++;

  return 0;
}

int MCS51::pop_local_var_int(int index)
{
  fprintf(out, "  ;; pop_local_var_int(index=%d)\n", index);

  if (index == 0)
  {
    fprintf(out, "  mov r0, 6\n");
  }
    else
  {
    fprintf(out,
      "  mov A, r6\n"
      "  add A, #%d\n"
      "  mov r0, A\n",
      index * 2);
  }

  fprintf(out,
    "  mov @r0, %d\n"
    "  inc r0\n"
    "  mov @r0, %d\n",
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1));

  reg -= 1;

  return 0;
}

int MCS51::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int MCS51::pop()
{
  fprintf(out, "  ;; pop()\n");
  reg--;
  return 0;
}

int MCS51::dup()
{
  fprintf(out,
    "  ;; dup\n"
    "  mov %d, %d\n"
    "  mov %d, %d\n",
    REG_ADDRESS_STACK_LO(reg), REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg), REG_ADDRESS_STACK_HI(reg - 1));

  reg++;

  return 0;
}

int MCS51::dup2()
{
  return -1;
}

int MCS51::swap()
{
  fprintf(out,
    "  ;; dup\n"
    "  mov r0, %d\n"
    "  mov r1, %d\n"
    "  mov %d, %d\n"
    "  mov %d, %d\n"
    "  mov %d, r0\n"
    "  mov %d, r1\n",
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1),
    REG_ADDRESS_STACK_LO(reg - 1), REG_ADDRESS_STACK_LO(reg - 2),
    REG_ADDRESS_STACK_HI(reg - 1), REG_ADDRESS_STACK_HI(reg - 2),
    REG_ADDRESS_STACK_LO(reg - 2),
    REG_ADDRESS_STACK_HI(reg - 2));

  return 0;
}

int MCS51::add_integer()
{
  fprintf(out,
    "  ;; add_integer()\n"
    "  setb PSW.3\n"
    "  mov A, r%d\n"
    "  add A, r%d\n"
    "  mov r%d, A\n"
    "  mov A, r%d\n"
    "  addc A, r%d\n"
    "  mov r%d, A\n"
    "  clr PSW.3\n",
    REG_STACK_LO(reg - 1),
    REG_STACK_LO(reg - 2),
    REG_STACK_LO(reg - 2),
    REG_STACK_HI(reg - 1),
    REG_STACK_HI(reg - 2),
    REG_STACK_HI(reg - 2));

  reg--;

  return 0;
}

int MCS51::add_integer(int num)
{
  if (num > 65535 || num < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", num);
    return -1;
  }

  uint16_t value = (num & 0xffff);

  fprintf(out,
    "  ;; add_integer(%d)\n"
    "  setb PSW.3\n"
    "  mov A, r%d\n"
    "  add A, #0x%02x\n"
    "  mov r%d, A\n"
    "  mov A, r%d\n"
    "  addc A, #0x%02x\n"
    "  mov r%d, A\n"
    "  clr PSW.3\n",
    num,
    REG_STACK_LO(reg - 1),
    value & 0xff,
    REG_STACK_LO(reg - 1),
    REG_STACK_HI(reg - 1),
    value >> 8,
    REG_STACK_HI(reg - 1));

  return 0;
}

int MCS51::sub_integer()
{
  fprintf(out,
    "  ;; sub_integer()\n"
    "  mov A, %d\n"
    "  clr C\n"
    "  subb A, %d\n"
    "  mov %d, A\n"
    "  mov A, %d\n"
    "  subb A, %d\n"
    "  mov %d, A\n",
    REG_ADDRESS_STACK_LO(reg - 2),
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_LO(reg - 2),
    REG_ADDRESS_STACK_HI(reg - 2),
    REG_ADDRESS_STACK_HI(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 2));

  reg--;

  return 0;
}

int MCS51::sub_integer(int num)
{
  if (num > 65535 || num < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", num);
    return -1;
  }

  uint16_t value = (num & 0xffff);

  fprintf(out,
    "  ;; sub_integer()\n"
    "  mov A, %d\n"
    "  clr C\n"
    "  subb A, #0x%02x\n"
    "  mov %d, A\n"
    "  mov A, %d\n"
    "  subb A, #0x%02x\n"
    "  mov %d, A\n",
    REG_ADDRESS_STACK_LO(reg - 1),
    value & 0xff,
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1),
    value >> 8,
    REG_ADDRESS_STACK_HI(reg - 1));

  return 0;
}

int MCS51::mul_integer()
{
  return -1;
}

int MCS51::div_integer()
{
  return -1;
}

int MCS51::mod_integer()
{
  return -1;
}

int MCS51::neg_integer()
{
  return -1;
}

int MCS51::shift_left_integer()
{
  fprintf(out,
    "  ;; shift_left_integer()\n"
    "  setb PSW.3\n"
    "label_%d:\n"
    "  clr C\n"
    "  mov A, r%d\n"
    "  rlc A\n"
    "  mov r%d, A\n"
    "  mov A, r%d\n"
    "  rlc A\n"
    "  mov r%d, A\n"
    "  djnz r%d, label_%d\n"
    "  clr PSW.3\n",
    label_count,
    REG_STACK_LO(reg - 2),
    REG_STACK_LO(reg - 2),
    REG_STACK_HI(reg - 2),
    REG_STACK_HI(reg - 2),
    REG_STACK_LO(reg - 1), label_count);

  label_count++;
  reg--;

  return 0;
}

int MCS51::shift_left_integer(int num)
{
  fprintf(out, "  ;; shift_left_integer(%d)\n", num);

  if (num != 1)
  {
    fprintf(out,
      "  mov r1, #%d\n"
      "label_%d:\n",
      num,
      label_count);
  }

  fprintf(out,
    "  clr C\n"
    "  mov A, %d\n"
    "  rlc A\n"
    "  mov %d, A\n"
    "  mov A, %d\n"
    "  rlc A\n"
    "  mov %d, A\n",
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1));

  if (num != 1)
  {
    fprintf(out,
      "  djnz r1, label_%d\n",
      label_count);
  }

  label_count++;

  return 0;
}

int MCS51::shift_right_integer()
{
  fprintf(out,
    "  ;; shift_right_integer()\n"
    "  setb PSW.3\n"
    "label_%d:\n"
    "  mov A, r%d\n"
    "  rlc A\n"
    "  mov A, r%d\n"
    "  rrc A\n"
    "  mov r%d, A\n"
    "  mov A, r%d\n"
    "  rrc A\n"
    "  mov r%d, A\n"
    "  djnz r%d, label_%d\n"
    "  clr PSW.3\n",
    label_count,
    REG_STACK_HI(reg - 2),
    REG_STACK_HI(reg - 2),
    REG_STACK_HI(reg - 2),
    REG_STACK_LO(reg - 2),
    REG_STACK_LO(reg - 2),
    REG_STACK_LO(reg - 1), label_count);

  label_count++;
  reg--;

  return 0;
}

int MCS51::shift_right_integer(int num)
{
  fprintf(out, "  ;; shift_right_integer(%d)\n", num);

  if (num != 1)
  {
    fprintf(out,
      "  mov r1, #%d\n"
      "label_%d:\n",
      num,
      label_count);
  }

  fprintf(out,
    "  mov A, %d\n"
    "  rlc A\n"
    "  mov A, %d\n"
    "  rrc A\n"
    "  mov %d, A\n"
    "  mov A, %d\n"
    "  rrc A\n"
    "  mov %d, A\n",
    REG_ADDRESS_STACK_HI(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1),
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_LO(reg - 1));

  if (num != 1)
  {
    fprintf(out,
      "  djnz r1, label_%d\n",
      label_count);
  }

  label_count++;

  return 0;
}

int MCS51::shift_right_uinteger()
{
  fprintf(out,
    "  ;; shift_right_uinteger()\n"
    "  setb PSW.3\n"
    "label_%d:\n"
    "  clr C\n"
    "  mov A, r%d\n"
    "  rrc A\n"
    "  mov r%d, A\n"
    "  mov A, r%d\n"
    "  rrc A\n"
    "  mov r%d, A\n"
    "  djnz r%d, label_%d\n"
    "  clr PSW.3\n",
    label_count,
    REG_STACK_HI(reg - 2),
    REG_STACK_HI(reg - 2),
    REG_STACK_LO(reg - 2),
    REG_STACK_LO(reg - 2),
    REG_STACK_LO(reg - 1), label_count);

  label_count++;
  reg--;

  return 0;
}

int MCS51::shift_right_uinteger(int num)
{
  fprintf(out, "  ;; shift_right_uinteger(%d)\n", num);

  if (num != 1)
  {
    fprintf(out,
      "  mov r1, #%d\n"
      "label_%d:\n",
      num,
      label_count);
  }

  fprintf(out,
    "  clr C\n"
    "  mov A, %d\n"
    "  rrc A\n"
    "  mov %d, A\n"
    "  mov A, %d\n"
    "  rrc A\n"
    "  mov %d, A\n",
    REG_ADDRESS_STACK_HI(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1),
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_LO(reg - 1));

  if (num != 1)
  {
    fprintf(out,
      "  djnz r1, label_%d\n",
      label_count);
  }

  label_count++;

  return 0;
}

int MCS51::and_integer()
{
  fprintf(out,
    "  ;; and_integer()\n"
    "  mov A, %d\n"
    "  anl A, %d\n"
    "  mov %d, A\n"
    "  mov A, %d\n"
    "  anl A, %d\n"
    "  mov %d, A\n",
    REG_ADDRESS_STACK_LO(reg - 2),
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 2),
    REG_ADDRESS_STACK_HI(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1));

  reg--;

  return 0;
}

int MCS51::and_integer(int num)
{
  if (num > 65535 || num < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", num);
    return -1;
  }

  uint16_t value = (num & 0xffff);

  fprintf(out,
    "  ;; and_integer(%d)\n"
    "  mov A, %d\n"
    "  anl A, #%d\n"
    "  mov %d, A\n"
    "  mov A, %d\n"
    "  anl A, #%d\n"
    "  mov %d, A\n",
    num,
    REG_ADDRESS_STACK_LO(reg - 1),
    value & 0xff,
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1),
    value >> 8,
    REG_ADDRESS_STACK_HI(reg - 1));

  return 0;
}

int MCS51::or_integer()
{
  fprintf(out,
    "  ;; and_integer()\n"
    "  mov A, %d\n"
    "  orl A, %d\n"
    "  mov %d, A\n"
    "  mov A, %d\n"
    "  orl A, %d\n"
    "  mov %d, A\n",
    REG_ADDRESS_STACK_LO(reg - 2),
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 2),
    REG_ADDRESS_STACK_HI(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1));

  reg--;

  return 0;
}

int MCS51::or_integer(int num)
{
  if (num > 65535 || num < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", num);
    return -1;
  }

  uint16_t value = (num & 0xffff);

  fprintf(out,
    "  ;; and_integer(%d)\n"
    "  mov A, %d\n"
    "  orl A, #%d\n"
    "  mov %d, A\n"
    "  mov A, %d\n"
    "  orl A, #%d\n"
    "  mov %d, A\n",
    num,
    REG_ADDRESS_STACK_LO(reg - 1),
    value & 0xff,
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1),
    value >> 8,
    REG_ADDRESS_STACK_HI(reg - 1));

  return 0;
}

int MCS51::xor_integer()
{
  fprintf(out,
    "  ;; and_integer()\n"
    "  mov A, %d\n"
    "  xrl A, %d\n"
    "  mov %d, A\n"
    "  mov A, %d\n"
    "  xrl A, %d\n"
    "  mov %d, A\n",
    REG_ADDRESS_STACK_LO(reg - 2),
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 2),
    REG_ADDRESS_STACK_HI(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1));

  reg--;

  return 0;
}

int MCS51::xor_integer(int num)
{
  if (num > 65535 || num < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", num);
    return -1;
  }

  uint16_t value = (num & 0xffff);

  fprintf(out,
    "  ;; and_integer(%d)\n"
    "  mov A, %d\n"
    "  xrl A, #%d\n"
    "  mov %d, A\n"
    "  mov A, %d\n"
    "  xrl A, #%d\n"
    "  mov %d, A\n",
    num,
    REG_ADDRESS_STACK_LO(reg - 1),
    value & 0xff,
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1),
    value >> 8,
    REG_ADDRESS_STACK_HI(reg - 1));

  return 0;
}

int MCS51::inc_integer(int index, int num)
{
  if (num > 65535 || num < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", num);
    return -1;
  }

  uint16_t value = (num & 0xffff);

  fprintf(out, "  ;; inc_integer(index=%d, num=%d)\n", index, num);

  if (index == 0)
  {
    fprintf(out, "  mov r0, 6\n");
  }
    else
  {
    fprintf(out,
      "  mov A, r6\n"
      "  add A, #%d\n"
      "  mov r0, A\n",
      index * 2);
  }

  fprintf(out,
    "  mov A, @r0\n"
    "  add A, #%d\n"
    "  mov @r0, A\n"
    "  inc r0\n"
    "  mov A, @r0\n"
    "  addc A, #%d\n"
    "  mov @r0, A\n",
    value & 0xff,
    value >> 8);

  return 0;
}

int MCS51::integer_to_byte()
{
  fprintf(out,
    "  ;; integer_to_byte()\n"
    "  mov A, %d\n"
    "  anl A, #0x80\n"
    "  jz label_%d\n"
    "  mov %d, #0xff\n"
    "label_%d:\n",
    REG_ADDRESS_STACK_LO(reg - 1),
    label_count,
    REG_ADDRESS_STACK_HI(reg - 1),
    label_count);

  label_count++;

  return 0;
}

int MCS51::integer_to_short()
{
  // Do nothing here.
  return 0;
}

int MCS51::jump_cond(std::string &label, int cond, int distance)
{
  fprintf(out, "  ;; jump_cond(%s, %d, %d)\n", label.c_str(), cond, distance);

  switch(cond)
  {
    case COND_EQUAL:
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_LO(reg - 1));
      fprintf(out, "  orl A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  jnz label_%d\n", label_count);
      reg--;
      break;
    case COND_NOT_EQUAL:
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_LO(reg - 1));
      fprintf(out, "  orl A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  jz label_%d\n", label_count);
      reg--;
      break;
    case COND_LESS:
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  anl A, #0x80\n");
      fprintf(out, "  jz label_%d\n", label_count);
      reg -= 1;
      break;
    case COND_LESS_EQUAL:
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  anl A, #0x80\n");
      fprintf(out, "  jz label_%d\n", label_count);
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_LO(reg - 1));
      fprintf(out, "  orl A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  jnz label_%d\n", label_count);
      reg -= 1;
      break;
    case COND_GREATER:
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_LO(reg - 1));
      fprintf(out, "  orl A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  jz label_%d\n", label_count);
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  anl A, #0x80\n");
      fprintf(out, "  jnz label_%d\n", label_count);
      reg -= 1;
      break;
    case COND_GREATER_EQUAL:
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  anl A, #0x80\n");
      fprintf(out, "  jnz label_%d\n", label_count);
      reg -= 1;
      break;
    default:
      return -1;
  }

  fprintf(out,
    "  ljmp %s\n"
    "label_%d:\n",
    label.c_str(),
    label_count++);

  return 0;
}

int MCS51::jump_cond_integer(std::string &label, int cond, int distance)
{
  fprintf(out, "  ;; jump_cond_integer(%s, %d, %d)\n", label.c_str(), cond, distance);

  switch (cond)
  {
    case COND_EQUAL:
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_LO(reg - 1));
      fprintf(out, "  xrl A, %d\n", REG_ADDRESS_STACK_LO(reg - 2));
      fprintf(out, "  mov r2, A\n");
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  xrl A, %d\n", REG_ADDRESS_STACK_HI(reg - 2));
      fprintf(out, "  orl A, r2\n");
      fprintf(out, "  jnz label_%d\n", label_count);
      reg -= 2;
      break;
    case COND_NOT_EQUAL:
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_LO(reg - 1));
      fprintf(out, "  xrl A, %d\n", REG_ADDRESS_STACK_LO(reg - 2));
      fprintf(out, "  jnz %s\n", label.c_str());
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  xrl A, %d\n", REG_ADDRESS_STACK_HI(reg - 2));
      fprintf(out, "  jz label_%d\n", label_count);
      reg -= 2;
      break;
    case COND_LESS:
      // Check reg_0 < reg_1.
      // Subtract reg_0 - reg_1. If negative, condition matches.
      // N xor V == 1 (less than).
      // 0000 0000    N________ xor PSW.2 [C____V__]
      // 1000 0000
      // 0000 0100
      // 1000 0100
      fprintf(out, "  clr C\n");
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_LO(reg - 2));
      fprintf(out, "  subb A, %d\n", REG_ADDRESS_STACK_LO(reg - 1));
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_HI(reg - 2));
      fprintf(out, "  subb A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  swap A\n");
      fprintf(out, "  rr A\n");
      fprintf(out, "  anl A, #0x04\n");
      fprintf(out, "  mov r3, A\n");
      fprintf(out, "  mov A, PSW\n");
      fprintf(out, "  anl A, #0x04\n");
      fprintf(out, "  xrl A, r3\n");
      fprintf(out, "  jz label_%d\n", label_count);
      reg -= 2;
      break;
    case COND_LESS_EQUAL:
      // Check reg_0 <= reg_1.
      // Subtract reg_1 - reg_0. If positive or 0, condition matches.
      // N xor V == 0 (greater than or equal).
      // 0000 0000    N________ xor PSW.2 [C____V__]
      // 1000 0000
      // 0000 0100
      // 1000 0100
      fprintf(out, "  clr C\n");
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_LO(reg - 1));
      fprintf(out, "  subb A, %d\n", REG_ADDRESS_STACK_LO(reg - 2));
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  subb A, %d\n", REG_ADDRESS_STACK_HI(reg - 2));
      fprintf(out, "  swap A\n");
      fprintf(out, "  rr A\n");
      fprintf(out, "  anl A, #0x04\n");
      fprintf(out, "  mov r3, A\n");
      fprintf(out, "  mov A, PSW\n");
      fprintf(out, "  anl A, #0x04\n");
      fprintf(out, "  xrl A, r3\n");
      fprintf(out, "  jnz label_%d\n", label_count);
      reg -= 2;
      break;
    case COND_GREATER:
      // Check reg_0 > reg_1.
      // Subtract reg_1 - reg_0. If negative, condition matches.
      // N xor V == 1 (less than).
      // 0000 0000    N________ xor PSW.2 [C____V__]
      // 1000 0000
      // 0000 0100
      // 1000 0100
      fprintf(out, "  clr C\n");
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_LO(reg - 1));
      fprintf(out, "  subb A, %d\n", REG_ADDRESS_STACK_LO(reg - 2));
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  subb A, %d\n", REG_ADDRESS_STACK_HI(reg - 2));
      fprintf(out, "  swap A\n");
      fprintf(out, "  rr A\n");
      fprintf(out, "  anl A, #0x04\n");
      fprintf(out, "  mov r3, A\n");
      fprintf(out, "  mov A, PSW\n");
      fprintf(out, "  anl A, #0x04\n");
      fprintf(out, "  xrl A, r3\n");
      fprintf(out, "  jz label_%d\n", label_count);
      reg -= 2;
      break;
    case COND_GREATER_EQUAL:
      // Check reg_0 >= reg_1.
      // Subtract reg_0 - reg_1. If positive or 0, condition matches.
      // N xor V == 0 (greater than or equal).
      // 0000 0000    N________ xor PSW.2 [C____V__]
      // 1000 0000
      // 0000 0100
      // 1000 0100
      fprintf(out, "  clr C\n");
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_LO(reg - 2));
      fprintf(out, "  subb A, %d\n", REG_ADDRESS_STACK_LO(reg - 1));
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_HI(reg - 2));
      fprintf(out, "  subb A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  swap A\n");
      fprintf(out, "  rr A\n");
      fprintf(out, "  anl A, #0x04\n");
      fprintf(out, "  mov r3, A\n");
      fprintf(out, "  mov A, PSW\n");
      fprintf(out, "  anl A, #0x04\n");
      fprintf(out, "  xrl A, r3\n");
      fprintf(out, "  jnz label_%d\n", label_count);
      reg -= 2;
      break;
    default:
      return -1;
  }

  fprintf(out,
    "  ljmp %s\n"
    "label_%d:\n",
    label.c_str(),
    label_count++);

  return 0;
}

int MCS51::ternary(int cond, int value_true, int value_false)
{
  return -1;
}

int MCS51::ternary(int cond, int compare, int value_true, int value_false)
{
  return -1;
}

int MCS51::return_local(int index, int local_count)
{
  fprintf(out, "  ;; return_integer(%d)\n", local_count);

  if (local_count != 0)
  {
    if (!is_main)
    {
      fprintf(out,
        "  mov A, SP\n"
        "  clr C\n"
        "  subb A, #%d\n"
        "  mov SP, A\n"
        "  pop 6\n",
        local_count * 2);
    }
  }

  fprintf(out, "  mov A, r6\n");

  if (index != 0)
  {
    fprintf(out, "  add A, #%d\n", index * 2);
  }

  fprintf(out,
    "  mov r0, A\n"
    "  mov r2, @r0\n"
    "  inc r0\n"
    "  mov r3, @r0\n");

  fprintf(out, "  ret\n");

  return 0;
}

int MCS51::return_integer(int local_count)
{
  fprintf(out, "  ;; return_integer(%d)\n", local_count);

  if (local_count != 0)
  {
    if (!is_main)
    {
      fprintf(out,
        "  mov A, SP\n"
        "  clr C\n"
        "  subb A, #%d\n"
        "  mov SP, A\n"
        "  pop 6\n",
        local_count * 2);
    }
  }

  fprintf(out,
    "  mov r2, %d\n"
    "  mov r3, %d\n",
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1));

  fprintf(out, "  ret\n");

  return 0;
}

int MCS51::return_void(int local_count)
{
  fprintf(out, "  ;; return_void(%d)\n", local_count);

  if (local_count != 0)
  {
    if (!is_main)
    {
      fprintf(out,
        "  mov A, SP\n"
        "  clr C\n"
        "  subb A, #%d\n"
        "  mov SP, A\n"
        "  pop 6\n",
        local_count * 2);
    }
  }

  fprintf(out, "  ret\n");

  return 0;
}

int MCS51::jump(std::string &name, int distance)
{
  fprintf(out, "  ;; jump(%s, %d)\n", name.c_str(), distance);
  fprintf(out, "  ljmp %s\n", name.c_str());

  return 0;
}

int MCS51::call(std::string &name)
{
  return -1;
}

int MCS51::invoke_static_method(const char *name, int params, int is_void)
{
  int saved_registers = reg - params;
  int n;

  fprintf(out,
    "  ;; invoke_static_method(%s, params=%d, is_void=%d)\n",
    name,
    params,
    is_void);

  for (n = 0; n < saved_registers; n++)
  {
    fprintf(out, "  push %d\n", REG_ADDRESS_STACK_LO(n));
    fprintf(out, "  push %d\n", REG_ADDRESS_STACK_HI(n));
  }

  if (params != 0)
  {
    fprintf(out,
      "  mov A, SP\n"
      "  add A, #4\n"
      "  mov r0, A\n");
  }

  int count = 0;

  for (n = saved_registers; n < reg; n++)
  {
    fprintf(out,
      "  mov @r0, %d\n"
      "  inc r0\n"
      "  mov @r0, %d\n",
      REG_ADDRESS_STACK_LO(n),
      REG_ADDRESS_STACK_HI(n));

    if (n != reg - 1)
    {
      fprintf(out, "  inc r0\n");
    }

    count++;
  }

  reg -= count;

  fprintf(out, "  lcall %s\n", name);

  for (n = saved_registers - 1; n >= 0; n--)
  {
    fprintf(out, "  pop %d\n", REG_ADDRESS_STACK_HI(n));
    fprintf(out, "  pop %d\n", REG_ADDRESS_STACK_LO(n));
  }

  if (!is_void)
  {
    fprintf(out,
      "  mov %d, r2\n"
      "  mov %d, r3\n",
      REG_ADDRESS_STACK_LO(reg),
      REG_ADDRESS_STACK_HI(reg));

    reg++;
  }

  return 0;
}

int MCS51::put_static(std::string &name, int index)
{
  fprintf(out,
    "  ;; put_static(%s, %d)\n"
    "  mov %d, %d\n"
    "  mov %d, %d\n",
    name.c_str(),
    index,
    STATICS_LO(index),
    REG_ADDRESS_STACK_LO(reg - 1),
    STATICS_HI(index),
    REG_ADDRESS_STACK_HI(reg - 1));

  reg--;

  return 0;
}

int MCS51::get_static(std::string &name, int index)
{
  fprintf(out,
    "  ;; get_static(%s, %d)\n"
    "  mov r2, %d\n"
    "  mov r3, %d\n"
    "  mov %d, r2\n"
    "  mov %d, r3\n",
    name.c_str(),
    index,
    STATICS_LO(index),
    STATICS_HI(index),
    REG_ADDRESS_STACK_LO(reg),
    REG_ADDRESS_STACK_HI(reg));

  reg++;

  return 0;
}

int MCS51::brk()
{
  return -1;
}

int MCS51::new_array(uint8_t type)
{
  return -1;
}

int MCS51::insert_array(std::string &name, int32_t *data, int len, uint8_t type)
{
  if (!has_array)
  {
    fprintf(out, "start_of_arrays:\n\n");
    has_array = true;
  }

  if (type == TYPE_BYTE)
  {
    return insert_db(name, data, len, TYPE_SHORT);
  }
    else
  if (type == TYPE_SHORT)
  {
    return insert_dw(name, data, len, TYPE_SHORT);
  }
    else
  if (type == TYPE_INT)
  {
    return insert_dw(name, data, len, TYPE_SHORT);
  }

  return -1;
}

int MCS51::insert_string(std::string &name, uint8_t *bytes, int len)
{
  if (!has_array)
  {
    fprintf(out, "start_of_arrays:\n\n");
    has_array = true;
  }

  return -1;
}

int MCS51::push_array_length()
{
  return -1;
}

int MCS51::push_array_length(std::string &name, int field_id)
{
  return -1;
}

int MCS51::array_read_byte()
{
  return -1;
}

int MCS51::array_read_short()
{
  return -1;
}

int MCS51::array_read_int()
{
  return -1;
}

int MCS51::array_read_byte(std::string &name, int field_id)
{
  return -1;
}

int MCS51::array_read_short(std::string &name, int field_id)
{
  return -1;
}

int MCS51::array_read_int(std::string &name, int field_id)
{
  return -1;
}

int MCS51::array_write_byte()
{
  return -1;
}

int MCS51::array_write_short()
{
  return -1;
}

int MCS51::array_write_int()
{
  return -1;
}

int MCS51::array_write_byte(std::string &name, int field_id)
{
  return -1;
}

int MCS51::array_write_short(std::string &name, int field_id)
{
  return -1;
}

int MCS51::array_write_int(std::string &name, int field_id)
{
  return -1;
}

int MCS51::ioport_setPinsValue_I(int port)
{
  fprintf(out,
    "  ;; ioport_setPinsValue_I(%d)\n"
    "  mov A, %d\n"
    "  mov P%d, A\n",
    port,
    REG_ADDRESS_STACK_LO(--reg),
    port);

  return 0;
}

int MCS51::ioport_setPinsValue_I(int port, int const_val)
{
  fprintf(out,
    "  ;; ioport_setPinsValue_I(%d, 0x%02x)\n"
    "  mov P%d, 0x%02x\n",
    port, const_val,
    port, const_val);

  return 0;
}

int MCS51::ioport_setPinsHigh_I(int port)
{
  fprintf(out,
    "  ;; ioport_setPinsHigh_I(%d)\n"
    "  mov A, %d\n"
    "  orl P%d, A\n",
    port,
    REG_ADDRESS_STACK_LO(--reg),
    port);

  return 0;
}

int MCS51::ioport_setPinsLow_I(int port)
{
  fprintf(out,
    "  ;; ioport_setPinsLow_I(%d)\n"
    "  mov A, %d\n"
    "  cpl A\n"
    "  anl P%d, A\n",
    port,
    REG_ADDRESS_STACK_LO(--reg),
    port);

  return 0;
}

int MCS51::ioport_setPinHigh_I(int port)
{
  fprintf(out,
    "  ;; ioport_setPinHigh_I(%d)\n"
    "  mov A, 0\n"
    "  setb C\n"
    "label_%d:\n"
    "  rlc\n"
    "  djnz %d, label_%d\n"
    "  orl P%d, A\n",
    port,
    label_count,
    REG_ADDRESS_STACK_LO(--reg), label_count,
    port);

  label_count++;

  return 0;
}

int MCS51::ioport_setPinHigh_I(int port, int const_val)
{
#if 0
  fprintf(out,
    "  ;; ioport_setPinHigh_I(%d, %d)\n"
    "  mov r2, %d\n"
    "  mov A, 0\n"
    "  setb C\n"
    "label_%d:\n"
    "  rlc\n"
    "  djnz r2, label_%d\n"
    "  orl P%d, A\n",
    port, const_val,
    const_val,
    label_count,
    label_count,
    port);

  label_count++;
#endif

  fprintf(out,
    "  ;; ioport_setPinHigh_I(%d, %d)\n"
    "  setb P%d.%d\n",
    port, const_val,
    port, const_val);

  return 0;
}

int MCS51::ioport_setPinLow_I(int port)
{
  fprintf(out,
    "  ;; ioport_setPinLow_I(%d)\n"
    "  mov A, 0\n"
    "  setb C\n"
    "label_%d:\n"
    "  rlc\n"
    "  djnz %d, label_%d\n"
    "  cpl A\n"
    "  anl P%d, A\n",
    port,
    label_count,
    REG_ADDRESS_STACK_LO(--reg), label_count,
    port);

  label_count++;

  return 0;
}

int MCS51::ioport_setPinLow_I(int port, int const_val)
{
  fprintf(out,
    "  ;; ioport_setPinLow_I(%d, %d)\n"
    "  clr P%d.%d\n",
    port, const_val,
    port, const_val);

  return 0;
}

int MCS51::ioport_isPinInputHigh_I(int port)
{
  fprintf(out,
    "  ;; ioport_isPinInputHigh_I(%d)\n"
    "  mov %d, #0\n"
    "  mov %d, #0\n"
    "  mov A, P%d\n"
    "label_%d:\n"
    "  rrc A\n"
    "  djnz %d, label_%d\n"
    "  rlc A\n"
    "  anl A, #1\n"
    "  mov %d, A\n",
    port,
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1),
    port,
    label_count,
    REG_ADDRESS_STACK_LO(reg - 1), label_count,
    REG_ADDRESS_STACK_LO(reg - 1));

   reg++;

  return 0;
}

int MCS51::ioport_getPortInputValue(int port)
{
  fprintf(out,
    "  ;; ioport_getPortInputValue(%d)\n"
    "  mov %d, P%d\n"
    "  mov %d, #0\n",
    port,
    REG_ADDRESS_STACK_LO(reg - 1), port,
    REG_ADDRESS_STACK_HI(reg - 1));

  reg++;

  return 0;
}

