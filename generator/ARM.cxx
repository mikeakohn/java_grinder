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

#include "ARM.h"

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

ARM::ARM() :
  reg(0),
  reg_max(9),
  stack(0),
  is_main(0)
{

}

ARM::~ARM()
{
  insert_constants_pool();
}

int ARM::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  // For now we only support a specific chip
  fprintf(out, ".arm\n");

  // Set where RAM starts / ends
  // FIXME - Not sure what to set this to right now
  //fprintf(out, "ram_start equ 0\n");
  //fprintf(out, "ram_end equ 0x8000\n");

  return 0;
}

int ARM::start_init()
{
  // Add any set up items (stack, registers, etc).
  //fprintf(out, ".org ???\n");
  fprintf(out, "start:\n");

  return 0;
}

int ARM::insert_static_field_define(const char *name, const char *type, int index)
{
  fprintf(out, "%s equ %d\n", name, (index + 1) * 4);
  return 0;
}

int ARM::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");
  //fprintf(out, "  mov #ram_start+%d, &ram_start\n", (field_count + 1) * 2);
  fprintf(out, "  mov r11, #0 ; Point to memory holding static vars\n");
  return 0;
}

int ARM::field_init_int(char *name, int index, int value)
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

int ARM::field_init_ref(char *name, int index)
{
  return -1;
}

void ARM::method_start(int local_count, int max_stack, int param_count, const char *name)
{
}

void ARM::method_end(int local_count)
{
}

int ARM::push_local_var_int(int index)
{
  return -1;
}

int ARM::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int ARM::push_ref_static(const char *name, int index)
{
  return -1;
}

int ARM::push_fake()
{
  if (stack != 0) { return -1; }

  reg++;
  return 0;
}

int ARM::push_int(int32_t n)
{
#if 0
int temp_reg;

  if (reg < reg_max)
  {
    temp_reg = REG_STACK(reg);
    fprintf(out, "  move.l #0x%02x, d%d\n", n, REG_STACK(reg));
    reg++;
  }
    else
  {
    temp_reg = 10;
    stack++;
  }

  if (immediate_is_possible(value))
  {
    fprintf(out, "  mov r%d, #%d\n", temp_reg, value);
  }
    else
  {
    int n = get_constant(value);
    fprintf(out, "  ldr r%d, [r12,#%d]\n", temp_reg, n * 4);
  }

  if (temp_reg == 10) { fprintf(out, "  push r10\n"); }

  return 0;
#endif
  return -1;
}

int ARM::push_long(int64_t n)
{
  return -1;
}

int ARM::push_float(float f)
{
  return -1;
}

int ARM::push_double(double f)
{
  return -1;
}

int ARM::push_ref(char *name)
{
  // Need to move the address of name to the top of stack
  return -1;
}

int ARM::pop_local_var_int(int index)
{
  // FIXME - This is NOT right
  if (stack > 0)
  {
    fprintf(out, "  ldr r10, ![SP,#4]\n");
    fprintf(out, "  str r10, [r14,#-%d]\n", LOCALS(index));
    stack--;
  }
    else
  if (reg > 0)
  {
    fprintf(out, "  str r%d, [r14,#-%d]\n", REG_STACK(reg-1), LOCALS(index));
    reg--;
  }

  return 0;
}

int ARM::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int ARM::pop()
{
  return -1;
}

int ARM::dup()
{
  return -1;
}

int ARM::dup2()
{
  return -1;
}

int ARM::swap()
{
  return -1;
}

int ARM::add_integer()
{
  return stack_alu("add");
}

int ARM::add_integer(int num)
{
  if (stack != 0) { return -1; }
  if (!immediate_is_possible(num)) { return -1; }
  fprintf(out, "  add r%d, r%d, #%d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int ARM::sub_integer()
{
  return stack_alu("sub");
}

int ARM::sub_integer(int num)
{
  if (stack != 0) { return -1; }
  if (!immediate_is_possible(num)) { return -1; }
  fprintf(out, "  sub r%d, r%d, #%d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int ARM::mul_integer()
{
  return stack_alu("mul");
}

int ARM::div_integer()
{
  return -1;
}

int ARM::mod_integer()
{
  return -1;
}

int ARM::neg_integer()
{
  return -1;
}

int ARM::shift_left_integer()
{
  // mov r0, r1, lsl r2
  return -1;
}

int ARM::shift_left_integer(int num)
{
  if (stack != 0) { return -1; }
  fprintf(out, "  mov r%d, r%d, lsl #%d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int ARM::shift_right_integer()
{
  return -1;
}

int ARM::shift_right_integer(int num)
{
  if (stack != 0) { return -1; }
  fprintf(out, "  mov r%d, r%d, rsl #%d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int ARM::shift_right_uinteger()
{
  return -1;
}

int ARM::shift_right_uinteger(int num)
{
  if (stack != 0) { return -1; }
  fprintf(out, "  mov r%d, r%d, rsl #%d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int ARM::and_integer()
{
  return stack_alu("and");
}

int ARM::and_integer(int num)
{
  if (stack != 0) { return -1; }
  if (!immediate_is_possible(num)) { return -1; }
  fprintf(out, "  and r%d, r%d, #%d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int ARM::or_integer()
{
  return stack_alu("orr");
}

int ARM::or_integer(int num)
{
  if (stack != 0) { return -1; }
  if (!immediate_is_possible(num)) { return -1; }
  fprintf(out, "  orr r%d, r%d, #%d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int ARM::xor_integer()
{
  return stack_alu("eor");
}

int ARM::xor_integer(int num)
{
  if (stack != 0) { return -1; }
  if (!immediate_is_possible(num)) { return -1; }
  fprintf(out, "  eor r%d, r%d, #%d\n", REG_STACK(reg-1), REG_STACK(reg-1), num);
  return 0;
}

int ARM::inc_integer(int index, int num)
{
  return -1;
}

int ARM::integer_to_byte()
{
  return -1;
}

int ARM::integer_to_short()
{
  return -1;
}

int ARM::jump_cond(const char *label, int cond, int distance)
{
  if (stack > 0)
  {
    fprintf(out, "  ldm SP!, {r10}\n");
    fprintf(out, "  cmp r10, #0\n");
    stack--;
  }
    else
  {
    fprintf(out, "  cmp r%d, #0\n", REG_STACK(reg-1));
    reg--;
  }

  fprintf(out, "  b%s %s\n", cond_str[cond], label);
  return 0;
}

int ARM::jump_cond_integer(const char *label, int cond, int distance)
{
  return -1;
}

int ARM::return_local(int index, int local_count)
{
  return -1;
}

int ARM::return_integer(int local_count)
{
  return -1;
}

int ARM::return_void(int local_count)
{
  return -1;
}

int ARM::jump(const char *name, int distance)
{
  return -1;
}

int ARM::call(const char *name)
{
  return -1;
}

int ARM::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int ARM::put_static(const char *name, int index)
{
  return -1;
}

int ARM::get_static(const char *name, int index)
{
  return -1;
}

int ARM::brk()
{
  return -1;
}

int ARM::new_array(uint8_t type)
{
  return -1;
}

int ARM::insert_array(const char *name, int32_t *data, int len, uint8_t type)
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

int ARM::insert_string(const char *name, uint8_t *bytes, int len)
{
  return -1;
}

int ARM::push_array_length()
{
  return -1;
}

int ARM::push_array_length(const char *name, int field_id)
{
  return -1;
}

int ARM::array_read_byte()
{
  return -1;
}

int ARM::array_read_short()
{
  return -1;
}

int ARM::array_read_int()
{
  return -1;
}

int ARM::array_read_byte(const char *name, int field_id)
{
  return -1;
}

int ARM::array_read_short(const char *name, int field_id)
{
  return -1;
}

int ARM::array_read_int(const char *name, int field_id)
{
  return -1;
}

int ARM::array_write_byte()
{
  return -1;
}

int ARM::array_write_short()
{
  return -1;
}

int ARM::array_write_int()
{
  return -1;
}

int ARM::array_write_byte(const char *name, int field_id)
{
  return -1;
}

int ARM::array_write_short(const char *name, int field_id)
{
  return -1;
}

int ARM::array_write_int(const char *name, int field_id)
{
  return -1;
}

#if 0
void ARM::close()
{
}
#endif

// GPIO functions
int ARM::ioport_setPinsAsInput(int port) { return -1; }
int ARM::ioport_setPinsAsOutput(int port) { return -1; }
int ARM::ioport_setPinsValue(int port) { return -1; }
int ARM::ioport_setPinsHigh(int port) { return -1; }
int ARM::ioport_setPinsLow(int port) { return -1; }
int ARM::ioport_setPinAsOutput(int port) { return -1; }
int ARM::ioport_setPinAsInput(int port) { return -1; }
int ARM::ioport_setPinHigh(int port) { return -1; }
int ARM::ioport_setPinLow(int port) { return -1; }
int ARM::ioport_isPinInputHigh(int port) { return -1; }
int ARM::ioport_getPortInputValue(int port) { return -1; }
//int ARM::ioport_setPortOutputValue(int port) { return -1; }

bool ARM::immediate_is_possible(int immediate)
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

int ARM::stack_alu(const char *instr)
{
  if (stack == 0)
  {
    fprintf(out, "  %s r%d, r%d, r%d\n", instr, REG_STACK(reg-2), REG_STACK(reg-2), REG_STACK(reg-1));
    reg--;
  }
    else
  if (stack == 1)
  {
    POP_REG(10);
    fprintf(out, "  %s r%d, r%d, r10\n", instr, REG_STACK(reg-1), REG_STACK(reg-1));
    stack--;
  }
    else
  {
    //POP_REG(10);
    //POP_REG(9);
    fprintf(out, "  ldmfd !SP, { r9, r10 }\n");
    fprintf(out, "  %s r9, r9, r10\n", instr);
    PUSH_REG(9);
  }

  return 0;
}



