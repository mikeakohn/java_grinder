/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2023 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "generator/F100_L.h"

#define REG_STACK(a) (a)
#define LOCALS(i) (i * 4)

// Memory:
// 0x0000          lsp (hardware stack pointer)
// 0x0001          frame pointer
// 0x0002          java stack pointer
// 0x0003          heap pointer,
// 0x0004          start of static variables
//                 stack + java_stack
// 0x0100 - 0x03ff
// 0x0401          globals pointer
// 0x0402          heap pointer
// 0x0403 - 0x07ff heap

F100_L::F100_L() :
  stack(0),
  heap_address(0x403),
  frame_ptr(0x0001),
  java_stack_ptr(0x0002),
  heap_ptr(0x0003),
  global_vars(0x0004),
  is_main(false)
{

}

F100_L::~F100_L()
{
}

int F100_L::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".f100_l\n");

  // Set where RAM starts / ends
  //fprintf(out, "ram_start equ 0\n");
  fprintf(out, "java_stack_ptr equ 0x%04x\n", java_stack_ptr);
  fprintf(out, "heap_ptr equ 0x%04x\n", heap_ptr);
  fprintf(out, "frame_ptr equ 0x%04x\n", frame_ptr);
  fprintf(out, "lsp equ 0x0000\n");

  return 0;
}

int F100_L::start_init()
{
  // Add any set up items (stack, registers, etc).
  fprintf(out, ".org 0x2000\n");
  fprintf(out, "start:\n");

  return 0;
}

int F100_L::insert_static_field_define(
  std::string &name,
  std::string &type,
  int index)
{
  return -1;
}

int F100_L::init_heap(int field_count)
{
  // The stack ptr (lsp) points to the global_vars - 1 because it's
  // a +1 first then write value on push, read value then -1 on pop.
  fprintf(out,
    "  ;; Set up heap, stack, and static initializers\n"
    "  lda #0x0100\n"
    "  sto heap_ptr\n"
    "  lda #0x%04x\n"
    "  sto java_stack_ptr\n"
    "  sto frame_ptr\n"
    "  sto lsp\n",
    global_vars + field_count - 1);

  return 0;
}

int F100_L::field_init_int(std::string &name, int index, int value)
{
  value = value & 0xffff;

  fprintf(out,
    "  ;; field_init_int(): %s = %d\n"
    "  lda #0x%04x\n"
    "  sto %d\n",
    name.c_str(), value,
    value,
    global_vars + index);

  return 0;
}

int F100_L::field_init_ref(std::string &name, int index)
{
  return -1;
}

void F100_L::method_start(
  int local_count,
  int max_stack,
  int param_count,
  std::string &name)
{
}

void F100_L::method_end(int local_count)
{
}

int F100_L::push_local_var_int(int index)
{
  return -1;
}

int F100_L::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int F100_L::push_ref_static(std::string &name, int index)
{
  // Push the address of the static rather than what the static is
  // pointing to?
  fprintf(out,
    "  ;; push_ref_static(%s, %d);\n"
    "  lda #0x%04x\n"
    "  sto [java_stack_ptr]+\n",
    name.c_str(), index,
    global_vars + index);

  return 0;
}

int F100_L::push_fake()
{
  return -1;
}

int F100_L::push_int(int32_t n)
{
  int value = n & 0xffff;

  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);
    return -1;
  }

  fprintf(out,
    "  ;; push_int(%d);\n"
    "  lda #%d\n"
    "  sto [java_stack_ptr]+\n",
    n,
    value);

  return 0;
}

#if 0
int F100_L::push_long(int64_t n)
{
  return -1;
}

int F100_L::push_float(float f)
{
  return -1;
}

int F100_L::push_double(double f)
{
  return -1;
}
#endif

int F100_L::push_ref(std::string &name, int index)
{
  // Push what the reference is pointing to (unlike push_ref_static).
  fprintf(out,
    "  ;; push_ref(%s, %d);\n"
    "  lda 0x%04x\n"
    "  sto [java_stack_ptr]+\n",
    name.c_str(), index,
    global_vars + index);

  return 0;
}

int F100_L::pop_local_var_int(int index)
{
  return -1;
}

int F100_L::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int F100_L::pop()
{
  return -1;
}

int F100_L::dup()
{
  return -1;
}

int F100_L::dup2()
{
  return -1;
}

int F100_L::swap()
{
  return -1;
}

int F100_L::add_integer()
{
  fprintf(out,
    "  ;; add_integer();\n"
    "  lda [java_stack_ptr]-\n"
    "  ads [java_stack_ptr]\n");

  return 0;
}

int F100_L::add_integer(int num)
{
  int value = num & 0xffff;

  if (num > 65535 || num < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", num);
    return -1;
  }

  fprintf(out,
    "  ;; add_integer(%d);\n"
    "  lda #%d\n"
    "  ads [java_stack_ptr]\n",
    num,
    value);

  return 0;
}

int F100_L::sub_integer()
{
  fprintf(out,
    "  ;; sub_integer();\n"
    "  lda [java_stack_ptr]-\n"
    "  sbs [java_stack_ptr]\n");

  return 0;
}

int F100_L::sub_integer(int num)
{
  int value = num & 0xffff;

  if (num > 65535 || num < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", num);
    return -1;
  }

  fprintf(out,
    "  ;; sub_integer(%d);\n"
    "  lda #%d\n"
    "  sbs [java_stack_ptr]\n",
    num,
    value);

  return 0;
}

int F100_L::mul_integer()
{
  return -1;
}

int F100_L::div_integer()
{
  return -1;
}

int F100_L::mod_integer()
{
  return -1;
}

int F100_L::neg_integer()
{
  fprintf(out,
    "  ;; neg_integer();\n"
    "  lda [java_stack_ptr]\n"
    "  neq #0xffff\n"
    "  add #1\n"
    "  sto [java_stack_ptr]\n");

  return 0;
}

int F100_L::shift_left_integer()
{
  fprintf(out,
    "  ;; shift_left_integer();\n"
    "  lda [java_stack_ptr]-\n"
    "label_%d:\n"
    "  sll #1, [java_stack_ptr]\n"
    "  sub #1\n"
    "  jnz label_%d\n"
    "  sto [java_stack_ptr]\n",
    label_count,
    label_count);

  label_count--;

  return 0;
}

int F100_L::shift_left_integer(int num)
{
  fprintf(out,
    "  ;; shift_left_integer(%d);\n"
    "  sll #%d, [java_stack_ptr]\n",
    num,
    num);

  return 0;
}

int F100_L::shift_right_integer()
{
  fprintf(out,
    "  ;; shift_right_integer();\n"
    "  lda [java_stack_ptr]-\n"
    "label_%d:\n"
    "  sra #1, [java_stack_ptr]\n"
    "  sub #1\n"
    "  jnz label_%d\n"
    "  sto [java_stack_ptr]\n",
    label_count,
    label_count);

  label_count--;

  return 0;
}

int F100_L::shift_right_integer(int num)
{
  fprintf(out,
    "  ;; shift_right_integer(%d);\n"
    "  sra #%d, [java_stack_ptr]\n",
    num,
    num);

  return 0;
}

int F100_L::shift_right_uinteger()
{
  fprintf(out,
    "  ;; shift_right_uinteger();\n"
    "  lda [java_stack_ptr]-\n"
    "label_%d:\n"
    "  srl #1, [java_stack_ptr]\n"
    "  sub #1\n"
    "  jnz label_%d\n"
    "  sto [java_stack_ptr]\n",
    label_count,
    label_count);

  label_count--;

  return 0;
}

int F100_L::shift_right_uinteger(int num)
{
  fprintf(out,
    "  ;; shift_right_uinteger(%d);\n"
    "  srl #%d, [java_stack_ptr]\n",
    num,
    num);

  return 0;
}

int F100_L::and_integer()
{
  fprintf(out,
    "  ;; and_integer();\n"
    "  lda [java_stack_ptr]-\n"
    "  and [java_stack_ptr]\n"
    "  sto [java_stack_ptr]\n");

  return 0;
}

int F100_L::and_integer(int num)
{
  int value = num & 0xffff;

  if (num > 65535 || num < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", num);
    return -1;
  }

  fprintf(out,
    "  ;; and_integer(%d);\n"
    "  lda #%d\n"
    "  and [java_stack_ptr]\n"
    "  sto [java_stack_ptr]\n",
    num,
    value);

  return 0;
}

int F100_L::or_integer()
{
  // F100-L has no OR instruction.
  // Can be done with: (A ^ B) + (A & B);
  fprintf(out,
    "  ;; and_integer();\n"
    "  lda [java_stack_ptr]-\n"
    "  sto temp_2\n"
    "  and [java_stack_ptr]\n"
    "  sto temp_1\n"
    "  lda temp_2\n"
    "  xor [java_stack_ptr]\n"
    "  add temp_1\n"
    "  sto [java_stack_ptr]\n");

  return 0;
}

int F100_L::or_integer(int num)
{
  int value = num & 0xffff;

  if (num > 65535 || num < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", num);
    return -1;
  }

  // F100-L has no OR instruction.
  // Can be done with: (A ^ B) + (A & B);
  // or if only 1 bit is set, SET can be used.

  fprintf(out, "  ;; or_integer(%d);\n", value);

  if (num == 0) { return 0; }

  int bit_count = 0;
  int a = value;

  for (int i = 0; i < 16; i++)
  {
    if ((a & 1) == 1) { bit_count++; }
    a = a >> 1;
  }

  if (bit_count < 8)
  {
    a = value;

    for (int i = 0; i < 16; i++)
    {
      if ((a & 1) == 1)
      {
        fprintf(out, "  set #%d, [java_stack_ptr]\n", i);
      }

      a = a >> 1;
    }
  }
    else
  {
    fprintf(out,
      "  lda #%d\n"
      "  and [java_stack_ptr]\n"
      "  sto temp_1\n"
      "  lda #%d\n"
      "  xor [java_stack_ptr]\n"
      "  add temp_1\n"
      "  sto [java_stack_ptr]\n",
      value,
      value);
  }

  return 0;
}

int F100_L::xor_integer()
{
  fprintf(out,
    "  ;; and_integer();\n"
    "  lda [java_stack_ptr]-\n"
    "  neq [java_stack_ptr]\n"
    "  sto [java_stack_ptr]\n");

  return 0;
}

int F100_L::xor_integer(int num)
{
  int value = num & 0xffff;

  if (num > 65535 || num < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", num);
    return -1;
  }

  fprintf(out,
    "  ;; and_integer(%d);\n"
    "  lda #%d\n"
    "  neq [java_stack_ptr]\n"
    "  sto [java_stack_ptr]\n",
    num,
    value);

  return 0;
}

int F100_L::inc_integer(int index, int num)
{
  int16_t value = num & 0xffff;

  fprintf(out,
    "  ;; inc_integer(%d);\n"
    "  ads #%d, [java_stack_ptr]\n",
    num,
    value);

  return 0;
}

int F100_L::integer_to_byte()
{
  fprintf(out,
    "  ;; integer_to_short()\n"
    "  sll #8, [java_stack_ptr]\n"
    "  sra #8, [java_stack_ptr]\n");

  return 0;
}

int F100_L::integer_to_short()
{
  // Nothing to do here since this is 16 bit.
  fprintf(out, "  ;; integer_to_short()\n");
  return 0;
}

int F100_L::jump_cond(std::string &label, int cond, int distance)
{
  fprintf(out,
     "  ; jump_cond(%s, %d, %d)\n"
     "  lda [java_stack_ptr]-\n",
     label.c_str(),
     cond,
     distance);

  switch (cond)
  {
    case COND_EQUAL:
      fprintf(out, "  jz %s\n", label.c_str());
      return 0;
    case COND_NOT_EQUAL:
      fprintf(out, "  jnz %s\n", label.c_str());
      return 0;
    case COND_LESS:
      // Branch if cr.s != cr.v (bits 3 and 2).
      // (cr & 0xc0) + 0x04 .. if bit3 == bit2, then bit3 will now be 0.
      //      32           3
      // 0000 1100 -> 0001 0000   <-- bits are the same.
      // 0000 0000 -> 0000 0100   <-- bits are the same.
      // 0000 1000 -> 0000 1100   <-- bits are different.
      // 0000 0100 -> 0000 1000   <-- bits are different.
      fprintf(out,
        "  cmp #0\n"
        "  setm\n"
        "  sll.d #16, cr\n"
        "  clrm\n"
        "  add #0x04\n"
        "  jbs #3, a, %s\n",
        label.c_str());
      return 0;
    case COND_LESS_EQUAL:
      // Branch if cr.z == 1 or cr.s != cr.v (bits 3 and 2).
      fprintf(out,
        "  jz %s\n"
        "  cmp #0\n"
        "  setm\n"
        "  sll.d #16, cr\n"
        "  clrm\n"
        "  add #0x04\n"
        "  jbs #3, a, %s\n",
        label.c_str(),
        label.c_str());
      return 0;
    case COND_GREATER:
      // Branch if cr.z == 0 and cr.s == cr.v (bits 3 and 2).
      fprintf(out,
        "  cmp #0\n"
        "  setm\n"
        "  sll.d #16, cr\n"
        "  clrm\n"
        "  add #0x04\n"
        "  and #0x0a\n"
        "  cmp #0x08\n"
        "  jz %s\n",
        label.c_str());
      return -1;
    case COND_GREATER_EQUAL:
      // Branch if cr.s == cr.v (bits 3 and 2).
      fprintf(out,
        "  cmp #0\n"
        "  setm\n"
        "  sll.d #16, cr\n"
        "  clrm\n"
        "  add #0x04\n"
        "  jbc #3, a, %s\n",
        label.c_str());
      return 0;
    default:
      break;
  }

  return -1;
}

int F100_L::jump_cond_zero(std::string &label, int cond, int distance)
{
  if (cond == COND_EQUAL)
  {
    fprintf(out,
      "  ;; jump_cond_zero(%s, COND_EQUAL, %d)\n"
      "  lda [java_stack_ptr]-\n"
      "  jz %s\n",
      label.c_str(), distance,
      label.c_str());
  }
    else
  if (cond == COND_NOT_EQUAL)
  {
    fprintf(out,
      "  ;; jump_cond_zero(%s, COND_NOT_EQUAL, %d)\n"
      "  lda [java_stack_ptr]-\n"
      "  jnz %s\n",
      label.c_str(), distance,
      label.c_str());
  }

  return -1;
}

int F100_L::jump_cond_integer(std::string &label, int cond, int distance)
{
  fprintf(out,
     "  ; jump_cond_integer(%s, %d, %d)\n"
     "  lda [java_stack_ptr]-\n"
     "  cmp [java_stack_ptr]-\n",
     label.c_str(),
     cond,
     distance);

  switch (cond)
  {
    case COND_EQUAL:
      fprintf(out, "  jz %s\n", label.c_str());
      return 0;
    case COND_NOT_EQUAL:
      fprintf(out, "  jnz %s\n", label.c_str());
      return 0;
    case COND_LESS:
      // Branch if cr.s != cr.v (bits 3 and 2).
      // (cr & 0xc0) + 0x04 .. if bit3 == bit2, then bit3 will now be 0.
      //      32           3
      // 0000 1100 -> 0001 0000   <-- bits are the same.
      // 0000 0000 -> 0000 0100   <-- bits are the same.
      // 0000 1000 -> 0000 1100   <-- bits are different.
      // 0000 0100 -> 0000 1000   <-- bits are different.
      fprintf(out,
        "  setm\n"
        "  sll.d #16, cr\n"
        "  clrm\n"
        "  add #0x04\n"
        "  jbs #3, a, %s\n",
        label.c_str());
      return 0;
    case COND_LESS_EQUAL:
      // Branch if cr.z == 1 or cr.s != cr.v (bits 3 and 2).
      fprintf(out,
        "  jz %s\n"
        "  setm\n"
        "  sll.d #16, cr\n"
        "  clrm\n"
        "  add #0x04\n"
        "  jbs #3, a, %s\n",
        label.c_str(),
        label.c_str());
      return 0;
    case COND_GREATER:
      // Branch if cr.z == 0 and cr.s == cr.v (bits 3 and 2).
      fprintf(out,
        "  setm\n"
        "  sll.d #16, cr\n"
        "  clrm\n"
        "  add #0x04\n"
        "  and #0x0a\n"
        "  cmp #0x08\n"
        "  jz %s\n",
        label.c_str());
      return -1;
    case COND_GREATER_EQUAL:
      // Branch if cr.s == cr.v (bits 3 and 2).
      fprintf(out,
        "  setm\n"
        "  sll.d #16, cr\n"
        "  clrm\n"
        "  add #0x04\n"
        "  jbc #3, a, %s\n",
        label.c_str());
      return 0;
    default:
      break;
  }

  return -1;
}

int F100_L::ternary(int cond, int value_true, int value_false)
{
  return -1;
}

int F100_L::ternary(int cond, int compare, int value_true, int value_false)
{
  return -1;
}

int F100_L::return_local(int index, int local_count)
{
  return -1;
}

int F100_L::return_integer(int local_count)
{
  return -1;
}

int F100_L::return_void(int local_count)
{
  return -1;
}

int F100_L::jump(std::string &name, int distance)
{
  fprintf(out, "  jmp %s\n", name.c_str());

  return 0;
}

int F100_L::call(std::string &name)
{
  return -1;
}

int F100_L::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int F100_L::put_static(std::string &name, int index)
{
  fprintf(out,
    "  ;; put_static(%s, %d);\n"
    "  lda [java_stack_ptr]-\n"
    "  sto 0x%04x\n",
    name.c_str(), index,
    global_vars + index);

  return 0;
}

int F100_L::get_static(std::string &name, int index)
{
  // Seems to be the same as push_ref().
  fprintf(out,
    "  ;; get_static(%s, %d);\n"
    "  lda 0x%04x\n"
    "  sto [java_stack_ptr]+\n",
    name.c_str(), index,
    global_vars + index);

  return 0;
}

int F100_L::brk()
{
  fprintf(out,
    "  ;; brk();\n"
    "  halt\n");

  return 0;
}

int F100_L::new_array(uint8_t type)
{
  return -1;
}

int F100_L::insert_array(std::string &name, int32_t *data, int len, uint8_t type)
{
  if (type == TYPE_BYTE)
  {
    return insert_dw(name, data, len, TYPE_INT);
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

int F100_L::insert_string(std::string &name, uint8_t *bytes, int len)
{
  return -1;
}

int F100_L::push_array_length()
{
  return -1;
}

int F100_L::push_array_length(std::string &name, int field_id)
{
  return -1;
}

int F100_L::array_read_byte()
{
  return -1;
}

int F100_L::array_read_short()
{
  return -1;
}

int F100_L::array_read_int()
{
  return -1;
}

int F100_L::array_read_byte(std::string &name, int field_id)
{
  return -1;
}

int F100_L::array_read_short(std::string &name, int field_id)
{
  return -1;
}

int F100_L::array_read_int(std::string &name, int field_id)
{
  return -1;
}

int F100_L::array_write_byte()
{
  return -1;
}

int F100_L::array_write_short()
{
  return -1;
}

int F100_L::array_write_int()
{
  return -1;
}

int F100_L::array_write_byte(std::string &name, int field_id)
{
  return -1;
}

int F100_L::array_write_short(std::string &name, int field_id)
{
  return -1;
}

int F100_L::array_write_int(std::string &name, int field_id)
{
  return -1;
}

int F100_L::ioport_setPinsValue_I(int port)
{
  int peripheral = port == 0 ? 0x4008 : 0x400a;

  fprintf(out,
    "  ;; setPinsValue(port=%d)\n"
    "  lda [java_stack_ptr]-\n"
    "  sto 0x%04x\n",
    port,
    peripheral);

  return 0;
}

int F100_L::ioport_setPinsValue_I(int port, int const_val)
{
  int peripheral = port == 0 ? 0x4008 : 0x400a;

  fprintf(out,
    "  ;; setPinsValue(port=%d, const_val=%d)\n"
    "  lda #0x%04x\n"
    "  sto 0x%04x\n",
    port, const_val,
    const_val,
    peripheral);

  return 0;
}

int F100_L::ice_fun_setTone_I()
{
  return -1;
}

int F100_L::ice_fun_setServo_II()
{
  return -1;
}

