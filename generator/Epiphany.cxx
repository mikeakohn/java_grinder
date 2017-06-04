/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2017 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "Epiphany.h"

#define REG_STACK(a) (reg_stack[a])
#define LOCALS(i) (i * 4)
// Use a full descending stack.
// SP points to last occupied.
// PUSH will decrement SP, and move value
// POP will move value, increment SP
// There are some STMFD and LDMFD instructions, but I don't see why I'd
// need them here
#define PUSH_REG(r) fprintf(out, "  str r%d, [SP,#-4]!\n", r);
#define POP_REG(r) fprintf(out, "  ldr r%d, [SP],#4\n", r);

// ABI is:
// r0 Start of reg stack
// r1
// r2
// r4
// r5 End of reg stack (extends from r15 to r63)
// r6 SP / Frame Pointer
// r7 temp
// r8 temp
// r9 temp
// r10 statics pointer
// r11 Shared memory pointer (Parallella)
// r12 Constants pointer
// r13
// r14 Link Register
// r15

static const uint8_t reg_stack[] = {
  0, 1, 2, 3, 4, 5,
  15, 16, 17, 18, 19, 20, 21,
  22, 23, 24, 25, 26, 27, 28,
  29, 30, 31, 32, 33, 34, 35,
  36, 37, 38, 39, 40, 41, 42,
  43, 44, 45, 46, 47, 48, 49,
  50, 51, 52, 53, 54, 55, 56,
  57, 58, 59, 60, 61, 62, 63,
};

static const char *cond_str[] = { "eq", "ne", "lt", "le", "gt", "ge" };

Epiphany::Epiphany() :
  reg(0),
  reg_max(9),
  stack(0),
  is_main(0),
  is_interrupt(0)
{

}

Epiphany::~Epiphany()
{
  fprintf(out,
    "_sw_exception_interrupt:\n"
    "_exception_interrupt:\n"
    "_memory_fault_interrupt:\n"
    "_timer0_interrupt:\n"
    "_timer1_interrupt:\n"
    "_message_interrupt:\n"
    "_dma0_interrupt:\n"
    "_dma1_interrupt:\n"
    "_wand_interrupt:\n"
    "_user_interrupt:\n"
    " rti\n\n");

  insert_constants_pool();
}

int Epiphany::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".epiphany\n");

  // Set where RAM starts / ends
  // FIXME - Not sure what to set this to right now
  //fprintf(out, "ram_start equ 0\n");
  //fprintf(out, "ram_end equ 0x8000\n");

  return 0;
}

int Epiphany::start_init()
{
  fprintf(out,
    ".org 0x00\n"
    "  b _sync_interrupt\n"
    ".org 0x04\n"
    "  b _sw_exception_interrupt\n"
    ".org 0x08\n"
    "  b _memory_fault_interrupt\n"
    ".org 0x0c\n"
    "  b _timer0_interrupt\n"
    ".org 0x10\n"
    "  b _timer1_interrupt\n"
    ".org 0x14\n"
    "  b _message_interrupt\n"
    ".org 0x18\n"
    "  b _dma0_interrupt\n"
    ".org 0x1c\n"
    "  b _dma1_interrupt\n"
    ".org 0x20\n"
    "  b _wand_interrupt\n"
    ".org 0x24\n"
    "  b _user_interrupt\n\n");

  fprintf(out, "static_vars:\n");

  // Add any set up items (stack, registers, etc).
  fprintf(out,
    ".org 0x2000\n"
    "_sync_interrupt:\n"
    "start:\n"
    "  ;; Setup stack pointer\n"
    "  mov r6, #0x8000\n"
    "  ;; Point to Parallella shared segment\n"
    "  mov r11, #0x0000\n"
    "  movt r11, #0x8e00\n");

  return 0;
}

int Epiphany::insert_static_field_define(const char *name, const char *type, int index)
{
  fprintf(out, "%s equ %d\n", name, (index + 1) * 4);
  return 0;
}

int Epiphany::init_heap(int field_count)
{
  fprintf(out, "  ;; Setup heap and static initializers\n");
  //fprintf(out, "  mov #ram_start+%d, &ram_start\n", (field_count + 1) * 2);
  fprintf(out, "  mov r10, #static_vars\n");

  return 0;
}

int Epiphany::field_init_int(char *name, int index, int value)
{
  if (immediate_is_possible(value))
  {
    fprintf(out, "  mov r7, #%d\n", value);
    fprintf(out, "  str r7, [r10,#%d]\n", index);
  }
    else
  {
    int n = get_constant(value);

    fprintf(out, "  ldr r7, [r12,#%d]\n", n);
    fprintf(out, "  str r7, [r10,#%d]\n", index);
  }

  return 0;
}

int Epiphany::field_init_ref(char *name, int index)
{
  return -1;
}

void Epiphany::method_start(int local_count, int max_stack, int param_count, const char *name)
{
  //this->max_stack = max_stack;
  printf("max_stack=%d\n", max_stack);

  is_interrupt = (strcmp(name, "userInterrupt") == 0) ? 1 : 0;

  // main() function goes here
  fprintf(out, "%s:\n", name);

  if (is_interrupt)
  {
    fprintf(out, "  sub r6, r6, #%d\n", (local_count * 4) + (max_stack * 4));

    int start = local_count;

    // If this is an interrupt, we have to push all possible registers that
    // could be in use.  Right now we'll push all temporary registers and
    // and max_stack registers.  This is bad because if the user calls a
    // another method in the interrupt that has a bigger max_stack, it
    // could cause odd results.  Fix later maybe.
    for (int n = 0; n < max_stack; n++)
    {
      fprintf(out, "  str r%d, [r6,#%d]\n", REG_STACK(n), start++);
    }
  }
    else
  {
    fprintf(out, "  ;; Setup frame pointer for this function\n");
    fprintf(out, "  sub r6, r6, #%d\n", (local_count * 4));
  }
}

void Epiphany::method_end(int local_count)
{
  fprintf(out, "\n");
}

int Epiphany::push_local_var_int(int index)
{
  fprintf(out, "  ldr r%d, [r6,#%d]\n", REG_STACK(reg++), index);

  return 0;
}

int Epiphany::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int Epiphany::push_ref_static(const char *name, int index)
{
  fprintf(out, "  str r%d, [r10,#%d]\n", REG_STACK(reg++), index);

  return 0;
}

int Epiphany::push_fake()
{
  reg++;
  return 0;
}

int Epiphany::push_int(int32_t n)
{
  fprintf(out, "  mov r%d, #%d\n", REG_STACK(reg), n & 0xffff);

  if ((n & 0xffff0000) != 0)
  {
    fprintf(out, "  movt r%d, #%d\n", REG_STACK(reg), (n >> 16) & 0xffff);
  }

  reg++;

  return 0;
}

int Epiphany::push_long(int64_t n)
{
  return -1;
}

int Epiphany::push_float(float f)
{
  return -1;
}

int Epiphany::push_double(double f)
{
  return -1;
}

int Epiphany::push_ref(char *name)
{
  // Need to move the address of name to the top of stack
  return -1;
}

int Epiphany::pop_local_var_int(int index)
{
  fprintf(out, "  str r%d, [r6,#%d]\n", REG_STACK(--reg), index);

  return 0;
}

int Epiphany::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int Epiphany::pop()
{
  reg--;

  return 0;
}

int Epiphany::dup()
{
  fprintf(out, "  mov r%d, r%d\n", REG_STACK(reg), REG_STACK(reg - 1));
  reg++;

  return 0;
}

int Epiphany::dup2()
{
  fprintf(out, "  mov r%d, r%d\n", REG_STACK(reg), REG_STACK(reg - 2));
  fprintf(out, "  mov r%d, r%d\n", REG_STACK(reg + 1), REG_STACK(reg - 1));
  reg += 2;

  return 0;
}

int Epiphany::swap()
{
  fprintf(out, "  mov r8, r%d\n", REG_STACK(reg - 2));
  fprintf(out, "  mov r%d, r%d\n", REG_STACK(reg - 2), REG_STACK(reg - 1));
  fprintf(out, "  mov r%d, r8\n", REG_STACK(reg - 1));

  return 0;
}

int Epiphany::add_integer()
{
  fprintf(out, "  iadd r%d, r%d, r%d\n",
     REG_STACK(reg - 2), REG_STACK(reg - 2), REG_STACK(reg - 1));

  reg--;

  return 0;
}

int Epiphany::add_integer(int num)
{
  if (num < -1024 || num > 1023) { return -1; }

  fprintf(out, "  add r%d, r%d, #%d\n", REG_STACK(reg - 1), REG_STACK(reg - 1), num);

  return 0;
}

int Epiphany::sub_integer()
{
  fprintf(out, "  isub r%d, r%d, r%d\n",
     REG_STACK(reg - 2), REG_STACK(reg - 2), REG_STACK(reg - 1));

  reg--;

  return 0;
}

int Epiphany::sub_integer(int num)
{
  if (num < -1024 || num > 1023) { return -1; }

  fprintf(out, "  sub r%d, r%d, #%d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int Epiphany::mul_integer()
{
  fprintf(out, "  imul r%d, r%d, r%d\n",
     REG_STACK(reg - 2), REG_STACK(reg - 2), REG_STACK(reg - 1));

  reg--;

  return 0;
}

int Epiphany::div_integer()
{
  return -1;
}

int Epiphany::mod_integer()
{
  return -1;
}

int Epiphany::neg_integer()
{
  // FIXME - Could keep one register set to 0 at all times and drop one
  // instruction here.
  fprintf(out, "  mov r8, #0\n");
  fprintf(out, "  sub r%d, r8, r%d\n", REG_STACK(reg - 1), REG_STACK(reg - 1));

  return 0;
}

int Epiphany::shift_left_integer()
{
  fprintf(out, "  lsl r%d, r%d, r%d\n",
     REG_STACK(reg - 2), REG_STACK(reg - 2), REG_STACK(reg - 1));

  reg--;

  return 0;
}

int Epiphany::shift_left_integer(int num)
{
  fprintf(out, "  lsl r%d, r%d, #%d\n", REG_STACK(reg - 1), REG_STACK(reg - 1), num);
  return 0;
}

int Epiphany::shift_right_integer()
{
  fprintf(out, "  asr r%d, r%d, r%d\n",
     REG_STACK(reg - 2), REG_STACK(reg - 2), REG_STACK(reg - 1));

  reg--;

  return 0;
}

int Epiphany::shift_right_integer(int num)
{
  fprintf(out, "  asr r%d, r%d, #%d\n", REG_STACK(reg - 1), REG_STACK(reg - 1), num);

  return 0;
}

int Epiphany::shift_right_uinteger()
{
  fprintf(out, "  asr r%d, r%d, r%d\n",
     REG_STACK(reg - 2), REG_STACK(reg - 2), REG_STACK(reg - 1));

  reg--;

  return 0;
}

int Epiphany::shift_right_uinteger(int num)
{
  fprintf(out, "  lsr r%d, r%d, #%d\n", REG_STACK(reg - 1), REG_STACK(reg - 1), num);

  return 0;
}

int Epiphany::and_integer()
{
  fprintf(out, "  and r%d, r%d, r%d\n",
     REG_STACK(reg - 2), REG_STACK(reg - 2), REG_STACK(reg - 1));

  reg--;

  return 0;
}

int Epiphany::and_integer(int num)
{
  return -1;
}

int Epiphany::or_integer()
{
  fprintf(out, "  orr r%d, r%d, r%d\n",
     REG_STACK(reg - 2), REG_STACK(reg - 2), REG_STACK(reg - 1));

  reg--;

  return 0;
}

int Epiphany::or_integer(int num)
{
  return -1;
}

int Epiphany::xor_integer()
{
  fprintf(out, "  eor r%d, r%d, r%d\n",
     REG_STACK(reg - 2), REG_STACK(reg - 2), REG_STACK(reg - 1));

  reg--;

  return 0;
}

int Epiphany::xor_integer(int num)
{
  return -1;
}

int Epiphany::inc_integer(int index, int num)
{
  fprintf(out, "  ldr r8, [r6,#%d]\n", index);
  fprintf(out, "  add r8, r8, #1\n");
  fprintf(out, "  str r8, [r6,#%d]\n", index);

  return 0;
}

int Epiphany::integer_to_byte()
{
  return -1;
}

int Epiphany::integer_to_short()
{
  return -1;
}

int Epiphany::jump_cond(const char *label, int cond, int distance)
{
  fprintf(out, "  cmp r%d, #0\n", REG_STACK(reg-1));
  reg--;

  fprintf(out, "  b%s %s\n", cond_str[cond], label);
  return 0;
}

int Epiphany::jump_cond_integer(const char *label, int cond, int distance)
{
  return -1;
}

int Epiphany::ternary(int cond, int value_true, int value_false)
{
  return -1;
}

int Epiphany::ternary(int cond, int compare, int value_true, int value_false)
{
  return -1;
}

int Epiphany::return_local(int index, int local_count)
{
  return -1;
}

int Epiphany::return_integer(int local_count)
{
  return -1;
}

int Epiphany::return_void(int local_count)
{
  return -1;
}

int Epiphany::jump(const char *name, int distance)
{
  fprintf(out, "  b %s\n", name);
  return 0;
}

int Epiphany::call(const char *name)
{
  return -1;
}

int Epiphany::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int Epiphany::put_static(const char *name, int index)
{
  return -1;
}

int Epiphany::get_static(const char *name, int index)
{
  return -1;
}

int Epiphany::brk()
{
  return -1;
}

int Epiphany::new_array(uint8_t type)
{
  return -1;
}

int Epiphany::insert_array(const char *name, int32_t *data, int len, uint8_t type)
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

int Epiphany::insert_string(const char *name, uint8_t *bytes, int len)
{
  return -1;
}

int Epiphany::push_array_length()
{
  return -1;
}

int Epiphany::push_array_length(const char *name, int field_id)
{
  return -1;
}

int Epiphany::array_read_byte()
{
  return -1;
}

int Epiphany::array_read_short()
{
  return -1;
}

int Epiphany::array_read_int()
{
  return -1;
}

int Epiphany::array_read_byte(const char *name, int field_id)
{
  return -1;
}

int Epiphany::array_read_short(const char *name, int field_id)
{
  return -1;
}

int Epiphany::array_read_int(const char *name, int field_id)
{
  return -1;
}

int Epiphany::array_write_byte()
{
  return -1;
}

int Epiphany::array_write_short()
{
  return -1;
}

int Epiphany::array_write_int()
{
  return -1;
}

int Epiphany::array_write_byte(const char *name, int field_id)
{
  return -1;
}

int Epiphany::array_write_short(const char *name, int field_id)
{
  return -1;
}

int Epiphany::array_write_int(const char *name, int field_id)
{
  return -1;
}

bool Epiphany::immediate_is_possible(int immediate)
{
  uint32_t i = immediate;

  if ((i & 0xffff0000) == 0) { return true; }

  return false;
}

int Epiphany::stack_alu(const char *instr)
{

  return -1;
}

// Parallella
int Epiphany::parallella_writeSharedRamByte_IB()
{
  int reg_data = --reg;
  int reg_offset = --reg;

  fprintf(out, "  strb r%d, [r11,r%d]\n", REG_STACK(reg_data), REG_STACK(reg_offset));

  return 0;
}

int Epiphany::parallella_writeSharedRamShort_IS()
{
  int reg_data = --reg;
  int reg_offset = --reg;

  fprintf(out, "  lsl r%d, r%d, #1\n", REG_STACK(reg_offset), REG_STACK(reg_offset));
  fprintf(out, "  strh r%d, [r11,r%d]\n", REG_STACK(reg_data), REG_STACK(reg_offset));

  return 0;
}

int Epiphany::parallella_writeSharedRamInt_II()
{
  int reg_data = --reg;
  int reg_offset = --reg;

  fprintf(out, "  lsl r%d, r%d, #2\n", REG_STACK(reg_offset), REG_STACK(reg_offset));
  fprintf(out, "  str r%d, [r11,r%d]\n", REG_STACK(reg_data), REG_STACK(reg_offset));

  return 0;
}

int Epiphany::parallella_writeSharedRamFloat_IF()
{
  return parallella_writeSharedRamInt_II();
}

int Epiphany::parallella_readSharedRamByte_I()
{
  fprintf(out, "  ldrb r%d, [r11,r%d]\n", REG_STACK(reg), REG_STACK(reg));

  return 0;
}

int Epiphany::parallella_readSharedRamShort_I()
{
  fprintf(out, "  lsl r%d, r%d, #1\n", REG_STACK(reg), REG_STACK(reg));
  fprintf(out, "  ldrh r%d, [r11,r%d]\n", REG_STACK(reg), REG_STACK(reg));

  return 0;
}

int Epiphany::parallella_readSharedRamInt_I()
{
  fprintf(out, "  lsl r%d, r%d, #2\n", REG_STACK(reg), REG_STACK(reg));
  fprintf(out, "  ldr r%d, [r11,r%d]\n", REG_STACK(reg), REG_STACK(reg));

  return 0;
}

int Epiphany::parallella_readSharedRamFloat_I()
{
  return parallella_readSharedRamInt_I();
}




