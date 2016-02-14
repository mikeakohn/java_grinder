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

#include "Epiphany.h"

#define REG_STACK(a) (a)
#define LOCALS(i) (i * 4)
// We want to use a full descending stack.
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
// r5
// r6
// r7
// r8 End of reg stack
// r9 temp
// r10 temp
// r11 Statics pointer
// r12 Constants pointer
// r13 SP
// r14 Link Register
// r15 PC

static const char *cond_str[] = { "eq", "ne", "lt", "le", "gt", "ge" };

Epiphany::Epiphany() :
  reg(0),
  reg_max(9),
  stack(0),
  is_main(0)
{

}

Epiphany::~Epiphany()
{
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
  // Add any set up items (stack, registers, etc).
  //fprintf(out, ".org ???\n");
  fprintf(out, "start:\n");

  return 0;
}

int Epiphany::insert_static_field_define(const char *name, const char *type, int index)
{
  fprintf(out, "%s equ %d\n", name, (index + 1) * 4);
  return 0;
}

int Epiphany::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");
  //fprintf(out, "  mov #ram_start+%d, &ram_start\n", (field_count + 1) * 2);
  fprintf(out, "  mov r11, #0 ; Point to memory holding static vars\n");
  return 0;
}

int Epiphany::field_init_int(char *name, int index, int value)
{
  if (immediate_is_possible(value))
  {
    fprintf(out, "  mov r10, #%d\n", value);
    fprintf(out, "  str r10, [r11,#%s]\n", name);
  }
    else
  {
    int n = get_constant(value);
    fprintf(out, "  ldr r10, [r12,#%d]\n", n * 4);
    fprintf(out, "  str r10, [r11,#%s]\n", name);
  }

  return 0;
}

int Epiphany::field_init_ref(char *name, int index)
{
  return -1;
}

void Epiphany::method_start(int local_count, int max_stack, int param_count, const char *name)
{
}

void Epiphany::method_end(int local_count)
{
}

int Epiphany::push_local_var_int(int index)
{
  return -1;
}

int Epiphany::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int Epiphany::push_ref_static(const char *name, int index)
{
  return -1;
}

int Epiphany::push_fake()
{
  if (stack != 0) { return -1; }

  reg++;
  return 0;
}

int Epiphany::push_int(int32_t n)
{
  return -1;
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
  return -1;
}

int Epiphany::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int Epiphany::pop()
{
  return -1;
}

int Epiphany::dup()
{
  return -1;
}

int Epiphany::dup2()
{
  return -1;
}

int Epiphany::swap()
{
  return -1;
}

int Epiphany::add_integer()
{
  return stack_alu("add");
}

int Epiphany::add_integer(int num)
{
  if (!immediate_is_possible(num)) { return -1; }
  fprintf(out, "  add r%d, r%d, #%d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int Epiphany::sub_integer()
{
  return stack_alu("sub");
}

int Epiphany::sub_integer(int num)
{
  if (!immediate_is_possible(num)) { return -1; }
  fprintf(out, "  sub r%d, r%d, #%d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int Epiphany::mul_integer()
{
  return stack_alu("mul");
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
  return -1;
}

int Epiphany::shift_left_integer()
{
  // mov r0, r1, lsl r2
  return -1;
}

int Epiphany::shift_left_integer(int num)
{
  fprintf(out, "  mov r%d, r%d, lsl #%d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int Epiphany::shift_right_integer()
{
  return -1;
}

int Epiphany::shift_right_integer(int num)
{
  fprintf(out, "  mov r%d, r%d, rsl #%d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int Epiphany::shift_right_uinteger()
{
  return -1;
}

int Epiphany::shift_right_uinteger(int num)
{
  fprintf(out, "  mov r%d, r%d, rsl #%d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int Epiphany::and_integer()
{
  return stack_alu("and");
}

int Epiphany::and_integer(int num)
{
  if (!immediate_is_possible(num)) { return -1; }
  fprintf(out, "  and r%d, r%d, #%d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int Epiphany::or_integer()
{
  return stack_alu("orr");
}

int Epiphany::or_integer(int num)
{
  if (!immediate_is_possible(num)) { return -1; }
  fprintf(out, "  orr r%d, r%d, #%d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int Epiphany::xor_integer()
{
  return stack_alu("eor");
}

int Epiphany::xor_integer(int num)
{
  if (!immediate_is_possible(num)) { return -1; }
  fprintf(out, "  eor r%d, r%d, #%d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int Epiphany::inc_integer(int index, int num)
{
  return -1;
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
  return -1;
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
  int n;

  for (n = 0; n < 16; n++)
  {
    if (i < 256) { return true; }
    i=((i & 0xc0000000) >> 30) | (i << 2);
  }

  return false;
}

int Epiphany::stack_alu(const char *instr)
{

  return -1;
}



