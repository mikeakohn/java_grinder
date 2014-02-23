/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ARM.h"

#define REG_STACK(a) (reg)
#define LOCALS(i) (i * 4)

// ABI is:
// r0 Start of reg stack
// r1
// r2
// r4
// r5
// r6
// r7
// r8
// r9 End of reg stack
// r10 temp
// r11 Statics pointer
// r12 Constants pointer
// r13 SP
// r14 Link Register
// r15 PC


ARM::ARM() :
  reg(0),
  reg_max(10),
  stack(0),
  is_main(0)
{

}

ARM::~ARM()
{
  write_constants();
}

int ARM::open(char *filename)
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
  fprintf(out, "  mov r11, #0 ; Point to memory holding static vars\n");

  return 0;
}

int ARM::insert_static_field_define(const char *name, const char *type, int index)
{
  fprintf(out, "%s equ %d\n", name, (index + 1) * 4);
  return 0;
}

int ARM::init_heap(int field_count)
{
  return -1;
}

int ARM::insert_field_init_boolean(char *name, int index, int value)
{
  value = (value == 0) ? 0 : 1;

  fprintf(out, "  mov r10, #%d\n", value);
  fprintf(out, "  str r10, [r11,#%s]\n", name);
  return 0;
}

int ARM::insert_field_init_byte(char *name, int index, int value)
{
  return insert_field_init_int(name, index, value);
}

int ARM::insert_field_init_short(char *name, int index, int value)
{
  return insert_field_init_int(name, index, value);
}

int ARM::insert_field_init_int(char *name, int index, int value)
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

int ARM::insert_field_init(char *name, int index)
{
  return -1;
}

void ARM::method_start(int local_count, const char *name)
{
}

void ARM::method_end(int local_count)
{
}

int ARM::push_integer(int32_t n)
{
  return -1;
}

int ARM::push_integer_local(int index)
{
  return -1;
}

int ARM::push_ref_local(int index)
{
  return push_integer_local(index);
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

int ARM::push_byte(int8_t b)
{
  return -1;
}

int ARM::push_short(int16_t s)
{
  return -1;
}

int ARM::pop_integer_local(int index)
{
  return -1;
}

int ARM::pop_ref_local(int index)
{
  return pop_integer_local(index);
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
  return -1;
}

int ARM::sub_integer()
{
  return -1;
}

int ARM::mul_integer()
{
  return -1;
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
  return -1;
}

int ARM::shift_right_integer()
{
  return -1;
}

int ARM::shift_right_uinteger()
{
  return -1;
}

int ARM::and_integer()
{
  return -1;
}

int ARM::or_integer()
{
  return -1;
}

int ARM::xor_integer()
{
  return -1;
}

int ARM::inc_integer(int index, int num)
{
  return -1;
}

int ARM::jump_cond(const char *label, int cond)
{
  return -1;
}

int ARM::jump_cond_integer(const char *label, int cond)
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

int ARM::jump(const char *name)
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



