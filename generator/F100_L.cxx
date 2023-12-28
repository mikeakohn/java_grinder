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
//        - 0x00fe "zero page" (frames: locals, frame_ptr save, java stack)
// 0x00ff          temp_ptr
// 0x0100 - 0x011f call stack (lsp points here)
// 0x0120 - 0x03fd heap
// 0x03fe          temp_1
// 0x03ff          temp_2
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
  frame_start(0x0004),
  call_stack(0x0100),
  heap(0x0120),
  temp_1(0x03fe),
  temp_2(0x03ff),
  temp_ptr(0x00ff),
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
  fprintf(out, "temp_ptr equ 0x%04x\n", temp_ptr);
  fprintf(out, "temp_1 equ 0x%04x\n", temp_1);
  fprintf(out, "temp_2 equ 0x%04x\n", temp_2);
  fprintf(out, "global_vars equ 0x%04x\n", global_vars);
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
  // The frame_start points to the global_vars - 1 because it's
  // a +1 first then write value on push, read value then -1 on pop.
  // java_stack_ptr and frame_ptr get computed at the start of main().
  fprintf(out,
    "  ;; Set up heap, stack, and static initializers\n"
    "  lda #0x%04x\n"
    "  sto lsp\n"
    "  lda #0x%04x\n"
    "  sto heap_ptr\n",
    call_stack - 1,
    heap);

  frame_start = global_vars + field_count;

  return 0;
}

int F100_L::field_init_int(std::string &name, int index, int value)
{
  value = value & 0xffff;

  fprintf(out,
    "  ;; field_init_int(index = %d): %s = %d\n"
    "  lda #0x%04x\n"
    "  sto global_vars + %d\n",
    index, name.c_str(), value,
    value,
    index);

  return 0;
}

int F100_L::field_init_ref(std::string &name, int index)
{
  fprintf(out,
    "  ;; field_init_ref(%s, %d)\n"
    "  lda #_%s\n"
    "  sto global_vars + %d\n",
    name.c_str(), index,
    name.c_str(),
    index);

  return 0;
}

void F100_L::method_start(
  int local_count,
  int max_stack,
  int param_count,
  std::string &name)
{
  is_main = name == "main";

  // main() function goes here
  fprintf(out,
    "  ;; %s(local_count=%d, max_stack=%d, param_count=%d)\n",
    name.c_str(),
    local_count,
    max_stack,
    param_count);

  fprintf(out, "%s:\n", name.c_str());

  if (is_main)
  {
    // local vars       (frame 0)
    // 0                (save frame_ptr, not needed)
    // 0                (save java_stack_ptr, not needed)
    // java stack       (frame 0)

    fprintf(out,
      "  lda #0x%04x\n"
      "  sto frame_ptr\n"
      "  lda #0x%04x\n"
      "  sto java_stack_ptr\n",
      frame_start,
      frame_start + local_count + 1);
  }
    else
  {
    // local vars       (frame 0)
    // 0                (save frame_ptr, not needed)
    // 0                (save java_stack_ptr, not needed)
    // java stack       (frame 0) (might not be empty)
    // local vars       (frame 1) <-- frame_ptr (passed paramters / locals)
    // frame_ptr 0      (save)
    // java_stack_ptr 0 (save)
    // java stack       (frame 1) <-- java_stack_ptr

    // Variables to save the frame_ptr and java_stack_ptr are used as
    // 2 extra local variables.
    // The new frame_ptr is the current java_stack_ptr.

    // temp_ptr = java_stack_ptr + 2
    // [temp_ptr]++ = frame_ptr
    // [temp_ptr]++ = java_stack_ptr
    fprintf(out,
      "  lda java_stack_ptr\n"
      "  add #%d\n"
      "  sto temp_ptr\n"
      "  lda frame_ptr\n"
      "  sto [temp_ptr]+\n"
      "  lda java_stack_ptr\n"
      "  sto [temp_ptr]+\n",
      local_count);

    fprintf(out,
      "  add #1\n"
      "  sto frame_ptr\n"
      "  lda temp_ptr\n"
      "  sto java_stack_ptr\n");
  }
}

void F100_L::method_end(int local_count)
{
  fprintf(out, "\n");
}

int F100_L::push_local_var_int(int index)
{
  fprintf(out, "  ;; push_local_var_int(%d)\n", index);

  if (index == 0)
  {
    fprintf(out,
      "  lda [frame_ptr]\n"
      "  sto [java_stack_ptr]+\n");
  }
    else
  {
    fprintf(out,
      "  lda frame_ptr\n"
      "  add #%d\n"
      "  sto temp_ptr\n"
      "  lda [temp_ptr]\n"
      "  sto [java_stack_ptr]+\n",
      index);
  }

  return 0;
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
    "  lda #global_vars + %d\n"
    "  sto [java_stack_ptr]+\n",
    name.c_str(), index,
    index);

  return 0;
}

int F100_L::push_fake()
{
#if 0
  fprintf(out,
    "  ;; push_fake()\n"
    "  lda #1\n"
    "  ads java_stack_ptr\n");
#endif

  // This is kind of a hack. icz is increment and jump not zero. This
  // instruction either skips the jump or jumps to the next address anyway.
  fprintf(out,
    "  ;; push_fake()\n"
    "  icz java_stack_ptr, $ + 2\n");

  return 0;
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
    "  lda global_vars + %d\n"
    "  sto [java_stack_ptr]+\n",
    name.c_str(), index,
    index);

  return 0;
}

int F100_L::pop_local_var_int(int index)
{
  fprintf(out,
    "  ;; pop_local_var_int(%d)\n"
    "  lda frame_ptr\n"
    "  add #%d\n"
    "  sto temp_ptr\n"
    "  lda [java_stack_ptr]-\n"
    "  sto [temp_ptr]\n",
    index,
    index);

  return 0;
}

int F100_L::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int F100_L::pop()
{
  fprintf(out,
    "  ;; pop()\n"
    "  lda #1\n"
    "  sbs java_stack_ptr\n");

  return 0;
}

int F100_L::dup()
{
  fprintf(out,
    "  ;; dup()\n"
    "  lda [java_stack_ptr]\n"
    "  sto [java_stack_ptr]+\n");

  return 0;
}

int F100_L::dup2()
{
  return -1;
}

int F100_L::swap()
{
  fprintf(out,
    "  ;; swap()\n"
    "  lda [java_stack_ptr]-\n"
    "  sto temp_1\n"
    "  lda [java_stack_ptr]-\n"
    "  sto temp_2\n"
    "  lda temp_1\n"
    "  lda [java_stack_ptr]+\n"
    "  lda temp_2\n"
    "  lda [java_stack_ptr]+\n");

  return 0;
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
    "  add #0xffff\n"
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
    "  add #0xffff\n"
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
    "  add #0xffff\n"
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
    "  ;; inc_integer(local=%d, num=%d)\n"
    "  lda frame_ptr\n"
    "  add #%d\n"
    "  sto temp_ptr\n"
    "  lda #0x%04x\n"
    "  ads [temp_ptr]\n",
    index, num,
    value,
    index);

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
     "  ; jump_cond(%s, %s, %d)\n"
     "  lda [java_stack_ptr]-\n",
     label.c_str(), show_cond(cond), distance);

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
     "  ; jump_cond_integer(%s, %s, %d)\n"
     "  lda [java_stack_ptr]-\n"
     "  cmp [java_stack_ptr]-\n",
     label.c_str(), show_cond(cond), distance);

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
  // Restore frame_ptr
  // Restore java_stack_ptr
  fprintf(out,
    "  ;; return_local(local_count=%d)\n"
    "  lda frame_ptr\n"
    "  add #%d\n"
    "  sto frame_ptr\n"
    "  lda [frame_ptr]\n"
    "  sto temp_1\n"
    "  lda frame_ptr\n"
    "  add #%d\n"
    "  sto temp_ptr\n"
    "  lda [temp_ptr]+\n"
    "  sto frame_ptr\n"
    "  lda [temp_ptr]+\n"
    "  sto java_stack_ptr\n"
    "  lda temp_1\n"
    "  sto [java_stack_ptr]+\n"
    "  rtn\n",
    local_count,
    index,
    local_count - 1);

  return 0;
}

int F100_L::return_integer(int local_count)
{
  // Restore frame_ptr
  // Restore java_stack_ptr
  fprintf(out,
    "  ;; return_integer(local_count=%d)\n"
    "  lda [java_stack_ptr]-\n"
    "  sto temp_1\n"
    "  lda frame_ptr\n"
    "  add #%d\n"
    "  sto temp_ptr\n"
    "  lda [temp_ptr]+\n"
    "  sto frame_ptr\n"
    "  lda [temp_ptr]+\n"
    "  sto java_stack_ptr\n"
    "  lda temp_1\n"
    "  sto [java_stack_ptr]+\n"
    "  rtn\n",
    local_count,
    local_count - 1);

  return 0;
}

int F100_L::return_void(int local_count)
{
  // Restore frame_ptr
  // Restore java_stack_ptr
  fprintf(out,
    "  ;; return_void(local_count=%d)\n"
    "  lda frame_ptr\n"
    "  add #%d\n"
    "  sto temp_ptr\n"
    "  lda [temp_ptr]+\n"
    "  sto frame_ptr\n"
    "  lda [temp_ptr]+\n"
    "  sto java_stack_ptr\n"
    "  rtn\n",
    local_count,
    local_count - 1);

  return 0;
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
  // This code is making the assumption that the Java stack is empty.
  // Seems like a bug if it's not, but if it's not this will be a
  // Java Grinder bug.

  // Before:
  // local vars (frame 0) <-- frame_ptr
  // 0          (save frame_ptr, not needed)
  // 0          (save java_stack_ptr, not needed)
  // java stack 0
  // java stack 1
  // ...                  <-- java_stack_ptr

  // After:
  // local vars       (frame 0)
  // 0
  // 0
  // java stack       (frame 0) (might not be empty)
  // local vars       (frame 1) <-- frame_ptr (locals include passed paramters)
  // frame_ptr 0      (save)
  // java_stack_ptr 0 (save)
  // java stack       (frame 1) <-- java_stack_ptr

  fprintf(out,
    "  ;; invoke_static_method(%s, params=%d, is_void=%d)\n",
    name,
    params,
    is_void);

  // local vars come from the stack, so should be able to just subtract
  // the num of params from java_stack_ptr.

  fprintf(out,
    "  lda #%d\n"
    "  sbs java_stack_ptr\n",
    params);

  // Call it.
  fprintf(out, "  cal %s\n", name);

  return 0;
}

int F100_L::put_static(std::string &name, int index)
{
  fprintf(out,
    "  ;; put_static(%s, %d);\n"
    "  lda [java_stack_ptr]-\n"
    "  sto global_vars + %d\n",
    name.c_str(), index,
    index);

  return 0;
}

int F100_L::get_static(std::string &name, int index)
{
  // Seems to be the same as push_ref().
  fprintf(out,
    "  ;; get_static(%s, %d);\n"
    "  lda global_vars + %d\n"
    "  sto [java_stack_ptr]+\n",
    name.c_str(), index,
    index);

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
    return insert_dw(name, data, len, TYPE_SHORT);
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

int F100_L::insert_string(std::string &name, uint8_t *bytes, int len)
{
  return -1;
}

int F100_L::push_array_length()
{
  fprintf(out,
    "  ;; push_array_length()\n"
    "  lda [java_stack_ptr]-\n"
    //"  sto temp_ptr\n"
    //"  lda [temp_ptr]\n"
    "  sto temp_ptr\n"
    "  lda #1\n"
    "  sbs temp_ptr\n"
    "  lda [temp_ptr]\n"
    "  sto [java_stack_ptr]+\n");

  return 0;
}

int F100_L::push_array_length(std::string &name, int field_id)
{
  fprintf(out,
    "  ;; push_array_length(%s, %d)\n"
    "  lda [global_vars + %d]\n"
    "  sto temp_ptr\n"
    "  lda #1\n"
    "  sbs temp_ptr\n"
    "  lda [temp_ptr]\n"
    "  sto [java_stack_ptr]+\n",
    name.c_str(), field_id,
    field_id);

  return 0;
}

int F100_L::array_read_byte()
{
  return -1;
}

int F100_L::array_read_short()
{
  // Java stack is:
  // 0: arrayref
  // 1: index
  fprintf(out,
    "  ;; array_read_short()\n"
    "  lda [java_stack_ptr]-\n"
    "  sto temp_1\n"
    "  lda [java_stack_ptr]-\n"
    "  sto temp_ptr\n"
    "  lda temp_1\n"
    "  ads temp_ptr\n"
    "  lda [temp_ptr]\n"
    "  sto [java_stack_ptr]+\n");

  return 0;
}

int F100_L::array_read_int()
{
  return array_read_short();
}

int F100_L::array_read_byte(std::string &name, int field_id)
{
  return -1;
}

int F100_L::array_read_short(std::string &name, int field_id)
{
  // Java stack is:
  // 0: arrayref
  // 1: index
  fprintf(out,
    "  ;; array_read_short(%s, %d)\n"
    "  lda [global_vars + %d]\n"
    "  sto temp_ptr\n"
    "  lda [java_stack_ptr]-\n"
    "  ads temp_ptr\n"
    "  lda [temp_ptr]\n"
    "  sto [java_stack_ptr]+\n",
    name.c_str(), field_id,
    field_id);

  return 0;
}

int F100_L::array_read_int(std::string &name, int field_id)
{
  return array_read_short(name, field_id);
}

int F100_L::array_write_byte()
{
  return -1;
}

int F100_L::array_write_short()
{
  // Java stack is:
  // 0: arrayref
  // 1: index
  // 2: value
  fprintf(out,
    "  ;; array_write_short()\n"
    "  lda [java_stack_ptr]-\n"
    "  sto temp_1\n"
    "  lda [java_stack_ptr]-\n"
    "  sto temp_2\n"
    "  lda [java_stack_ptr]-\n"
    "  sto temp_ptr\n"
    "  lda temp_2\n"
    "  ads temp_ptr\n"
    "  lda temp_1\n"
    "  sto [temp_ptr]\n");

  return 0;
}

int F100_L::array_write_int()
{
  return array_write_short();
}

int F100_L::array_write_byte(std::string &name, int field_id)
{
  return -1;
}

int F100_L::array_write_short(std::string &name, int field_id)
{
  // Java stack is:
  // 0: arrayref  <-- not on stack here
  // 1: index
  // 2: value
  fprintf(out,
    "  ;; array_write_short(%s, %d)\n"
    "  lda [java_stack_ptr]-\n"
    "  sto temp_1\n"
    "  lda [global_vars + %d]\n"
    "  sto temp_ptr\n"
    "  lda [java_stack_ptr]-\n"
    "  ads temp_ptr\n"
    "  lda temp_1\n"
    "  sto [temp_ptr]\n",
    name.c_str(), field_id,
    field_id);

  return 0;
}

int F100_L::array_write_int(std::string &name, int field_id)
{
  return array_write_short(name, field_id);
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

