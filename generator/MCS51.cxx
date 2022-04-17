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

// ABI is:
// Bank 0:
//  0x00 r0: temp pointer
//  0x01 r1: temp pointer
//  0x02 r2:
//  0x03 r3:
//  0x04 r4:
//  0x05 r5:
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
  is_main(0)
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

  fprintf(out, "stack equ 0x20\n");
  fprintf(out, "locals equ r2\n");

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
  return -1;
}

int MCS51::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");
  //fprintf(out, "  mov #ram_start+%d, &ram_start\n", (field_count + 1) * 2);

  fprintf(out, "  mov SP, #0x10\n");
  fprintf(out, "  mov r7, #0x10\n");

  return -1;
}

int MCS51::field_init_int(std::string &name, int index, int value)
{
  return -1;
}

int MCS51::field_init_ref(std::string &name, int index)
{
  return -1;
}

void MCS51::method_start(
  int local_count,
  int max_stack,
  int param_count,
  std::string &name)
{
  is_main = name == "main";

  fprintf(out, "%s:\n", name.c_str());

  // main() function goes here
  if (!is_main)
  {
    fprintf(out, "  push 6");
  }
    else
  {
    fprintf(out, "  mov r6, SP\n");
  }
}

void MCS51::method_end(int local_count)
{
  fprintf(out, "\n");
}

int MCS51::push_local_var_int(int index)
{
  fprintf(out, "; push_local_var_int\n");

  if (index == 0)
  {
    fprintf(out,
      "  mov r0, r7\n"
      "  mov %d, @r0\n"
      "  inc r0\n"
      "  mov %d, @r0\n",
      REG_ADDRESS_STACK_LO(reg),
      REG_ADDRESS_STACK_HI(reg));
  }
    else
  {
    fprintf(out,
      "  mov A, r0\n"
      "  add A, #4\n"
      "  mov r0, A\n"
      "  mov %d, @r0\n"
      "  inc r0\n"
      "  mov %d, @r0\n",
      REG_ADDRESS_STACK_LO(reg),
      REG_ADDRESS_STACK_HI(reg));
  }

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
  return -1;
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
    "; push_int\n"
    "  mov %d, #0x%02x\n"
    "  mov %d, #0x%02x\n",
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
  // Need to move the address of name to the top of stack
  return -1;
}

int MCS51::pop_local_var_int(int index)
{
  fprintf(out, "; pop_local_var_int\n");

  return 0;
}

int MCS51::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int MCS51::pop()
{
  return -1;
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
  return -1;
}

int MCS51::sub_integer(int num)
{
  return -1;
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
  return -1;
}

int MCS51::shift_left_integer(int num)
{
  return -1;
}

int MCS51::shift_right_integer()
{
  return -1;
}

int MCS51::shift_right_integer(int num)
{
  return -1;
}

int MCS51::shift_right_uinteger()
{
  return -1;
}

int MCS51::shift_right_uinteger(int num)
{
  return -1;
}

int MCS51::and_integer()
{
  return -1;
}

int MCS51::and_integer(int num)
{
  return -1;
}

int MCS51::or_integer()
{
  return -1;
}

int MCS51::or_integer(int num)
{
  return -1;
}

int MCS51::xor_integer()
{
  return -1;
}

int MCS51::xor_integer(int num)
{
  return -1;
}

int MCS51::inc_integer(int index, int num)
{
  return -1;
}

int MCS51::integer_to_byte()
{
  return -1;
}

int MCS51::integer_to_short()
{
  return -1;
}

int MCS51::jump_cond(std::string &label, int cond, int distance)
{
  fprintf(out, "  ; jump_cond(%s, %d, %d)\n", label.c_str(), cond, distance);

  switch(cond)
  {
    case COND_EQUAL:
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_LO(reg - 1));
      fprintf(out, "  orl A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  jz %s\n", label.c_str());
      reg--;
      return 0;
    case COND_NOT_EQUAL:
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_LO(reg - 1));
      fprintf(out, "  orl A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  jnz %s\n", label.c_str());
      reg--;
      return 0;
    case COND_LESS:
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  anl A, #0x80\n");
      fprintf(out, "  jnz %s\n", label.c_str());
      reg -= 1;
      return 0;
    case COND_LESS_EQUAL:
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  anl A, #0x80\n");
      fprintf(out, "  jnz %s\n", label.c_str());
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_LO(reg - 1));
      fprintf(out, "  orl A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  jz %s\n", label.c_str());
      reg -= 1;
      return 0;
    case COND_GREATER:
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_LO(reg - 1));
      fprintf(out, "  orl A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  jz label_%d\n", label_count);
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  anl A, #0x80\n");
      fprintf(out, "  jz %s\n", label.c_str());
      fprintf(out, "label_%d:\n",label_count);
      label_count++;
      reg -= 1;
      return 0;
    case COND_GREATER_EQUAL:
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  anl A, #0x80\n");
      fprintf(out, "  jz %s\n", label.c_str());
      reg -= 1;
      return 0;
    default:
      break;
  }

  return -1;
}

int MCS51::jump_cond_integer(std::string &label, int cond, int distance)
{
  fprintf(out, "  ; jump_cond_integer(%s, %d, %d)\n", label.c_str(), cond, distance);

  switch (cond)
  {
    case COND_EQUAL:
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_LO(reg - 1));
      fprintf(out, "  xrl A, %d\n", REG_ADDRESS_STACK_LO(reg - 2));
      fprintf(out, "  mov r2, A\n");
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  xrl A, %d\n", REG_ADDRESS_STACK_HI(reg - 2));
      fprintf(out, "  orl A, r2\n");
      fprintf(out, "  jz %s\n", label.c_str());
      reg -= 2;
      return 0;
    case COND_NOT_EQUAL:
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_LO(reg - 1));
      fprintf(out, "  xrl A, %d\n", REG_ADDRESS_STACK_LO(reg - 2));
      fprintf(out, "  jnz %s\n", label.c_str());
      fprintf(out, "  mov A, %d\n", REG_ADDRESS_STACK_HI(reg - 1));
      fprintf(out, "  xrl A, %d\n", REG_ADDRESS_STACK_HI(reg - 2));
      fprintf(out, "  jnz %s\n", label.c_str());
      reg -= 2;
      return 0;
    case COND_LESS:
    case COND_LESS_EQUAL:
    case COND_GREATER:
    case COND_GREATER_EQUAL:
    default:
      break;
  }

  return -1;
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
  return -1;
}

int MCS51::return_integer(int local_count)
{
  return -1;
}

int MCS51::return_void(int local_count)
{
  fprintf(out, "; return void\n");
  fprintf(out, "  mov locals,SP\n");

  if(!is_main)
  {
    fprintf(out, "  pop locals\n");
    fprintf(out, "  ret\n");
  }

  return 0;
}

int MCS51::jump(std::string &name, int distance)
{
  return -1;
}

int MCS51::call(std::string &name)
{
  return -1;
}

int MCS51::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int MCS51::put_static(std::string &name, int index)
{
  return -1;
}

int MCS51::get_static(std::string &name, int index)
{
  return -1;
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

int MCS51::insert_string(std::string &name, uint8_t *bytes, int len)
{
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
#if 0
  fprintf(out,
    "  ;; ioport_setPinLow_I(%d, %d)\n"
    "  mov r2, %d\n"
    "  mov A, 0\n"
    "  setb C\n"
    "label_%d:\n"
    "  rlc\n"
    "  djnz r2, label_%d\n"
    "  cpl A\n"
    "  anl P%d, A\n",
    port, const_val,
    const_val,
    label_count,
    label_count,
    port);

  label_count++;
#endif

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

