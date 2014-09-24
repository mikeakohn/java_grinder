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
#include <string.h>
#include <stdint.h>
#include <typeinfo>

#include "TMS9900.h"

#define REG_STACK(a) (a+2)
#define LOCALS(i) (i * 4)
#define CHECK_STACK() \
  if (reg >= reg_max) { printf("Internal error: Stack blown.\n"); return -1; }

// ABI is:
// r0 temp
// r1 temp
// r2 start of stack
// ..
// ..
// r9 end of stack
// r10 Pointer to locals (or should say, pointer to top of local heap?)
// r11 return address
// r12 CRU base address
// r13 Saved WP register
// r14 Saved PC register
// r15 Saved ST register

//                                  EQ     NE      <     <=      >     >= 
static const char *cond_str[] = { "jeq", "jne", "jlt", "???", "jgt", "???" };
//                                                      rev           rev

TMS9900::TMS9900() :
  reg(0),
  reg_max(9),
  is_main(0)
{

}

TMS9900::~TMS9900()
{

}

int TMS9900::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".tms9900\n");

  // Set where RAM starts / ends
  //Generator *generator = this;
  //if (dynamic_cast<TMS9900 *>(this) == NULL)
  if (typeid(this) == typeid(TMS9900 *))
  {
    // FIXME
    //fprintf(out, "free_ram equ 0\n");
  }

  return 0;
}

int TMS9900::start_init()
{
  // Add any set up items (stack, registers, etc).
  //fprintf(out, ".org ???\n");
  fprintf(out, "start:\n");

  return 0;
}

int TMS9900::insert_static_field_define(const char *name, const char *type, int index)
{
  fprintf(out, "%s equ free_ram+%d\n", name, index * 2);
  return 0;
}


int TMS9900::init_heap(int field_count)
{
  fprintf(out, "free_ram equ ram_start+32\n");

  // TI99 - We aren't going to use a heap here since the scratch pad is
  // only 256 bytes long.  Instread the heap will be in the VDP.
  fprintf(out, "  ;; Set up heap and static initializers\n");
  //fprintf(out, "  li r0, free_ram+%d\n", (field_count + 1) * 2);
  //fprintf(out, "  mov r0, @free_ram\n");
  fprintf(out, "  li r10, free_ram+%d\n", field_count * 2);

  return 0;
}

int TMS9900::insert_field_init_boolean(char *name, int index, int value)
{
  value = (value == 0) ? 0 : 1;

  fprintf(out, "  li r0, %d\n", value);
  fprintf(out, "  mov r0, @%s\n", name);

  return 0;
}

int TMS9900::insert_field_init_byte(char *name, int index, int value)
{
  if (value < -128 || value > 255) { return -1; }
  fprintf(out, "  li r0, %d\n", (int8_t)value);
  fprintf(out, "  mov r0, @%s\n", name);

  return 0;
}

int TMS9900::insert_field_init_short(char *name, int index, int value)
{
  if (value < -32768 || value > 65535) { return -1; }
  fprintf(out, "  li r0, %d\n", (int8_t)value);
  fprintf(out, "  mov r0, @%s\n", name);

  return 0;
}

int TMS9900::insert_field_init_int(char *name, int index, int value)
{
  return insert_field_init_short(name, index, value);
}

int TMS9900::insert_field_init(char *name, int index)
{
  fprintf(out, "  li r0, _%s\n", name);
  fprintf(out, "  mov r0, @%s\n", name);
  return 0;
}

void TMS9900::method_start(int local_count, int max_stack, int param_count, const char *name)
{
  is_main = (strcmp(name, "main") == 0) ? 1 : 0;
  fprintf(out, "%s:\n", name);
  fprintf(out, "  ai r10, %d\n", local_count * 2);
}

void TMS9900::method_end(int local_count)
{
  fprintf(out, "\n");
}

int TMS9900::push_integer(int32_t n)
{
  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);
    return -1;
  }

  uint16_t value = (n & 0xffff);

  CHECK_STACK();

  fprintf(out, "  li r%d, %d\n", REG_STACK(reg), value);
  reg++;

  return 0;
}

int TMS9900::push_integer_local(int index)
{
  if (reg < reg_max)
  {
    fprintf(out, "  mov @-%d(r10), r%d  ; push local_%d\n", LOCALS(index), REG_STACK(reg), index);
    reg++;
  }
    else
  {
    //fprintf(out, "  push -%d(r10)  ; push local_%d\n", LOCALS(index), index);
    //stack++;
    // FIXME
    printf("ERROR: Need to add stack support\n");
    return -1;
  }

  return 0;
}

int TMS9900::push_ref_local(int index)
{
  return push_integer_local(index);
}

int TMS9900::push_fake()
{
  reg++;
  return 0;
}

int TMS9900::push_long(int64_t n)
{
  printf("long is not supported right now\n");
  return -1;
}

int TMS9900::push_float(float f)
{
  printf("float is not supported right now\n");
  return -1;
}

int TMS9900::push_double(double f)
{
  printf("double is not supported right now\n");
  return -1;
}

int TMS9900::push_byte(int8_t b)
{
  int16_t n = b;
  uint16_t value = (n & 0xffff);

  CHECK_STACK();

  fprintf(out, "  li r%d, 0x%02x\n", REG_STACK(reg), value);
  reg++;

  return 0;
}

int TMS9900::push_short(int16_t s)
{
  uint16_t value = (s & 0xffff);

  CHECK_STACK();

  fprintf(out, "  li r%d, 0x%02x\n", REG_STACK(reg), value);
  reg++;

  return 0;
}

int TMS9900::push_ref(char *name)
{
  CHECK_STACK();

  fprintf(out, "  mov @%s, r%d\n", name, REG_STACK(reg));
  reg++;

  return 0;
}

int TMS9900::pop_integer_local(int index)
{
#if 0
  if (stack > 0)
  {
    fprintf(out, "  pop -%d(r10)  ; pop local_%d\n", LOCALS(index), index);
    stack--;
  }
    else
#endif
  if (reg > 0)
  {
    fprintf(out, "  mov r%d, @-%d(r10) ; pop local_%d\n", REG_STACK(reg-1), LOCALS(index), index);
    reg--;
  }
    else
  {
    printf("Internal Error: Nothing on the stack?\n");
    return -1;
  }

  return 0;
}

int TMS9900::pop_ref_local(int index)
{
  return pop_integer_local(index);
}

int TMS9900::pop()
{
  reg--;
  return 0;
}

int TMS9900::dup()
{
  CHECK_STACK();

  fprintf(out, "  mov r%d, r0\n", REG_STACK(reg-1));
  fprintf(out, "  mov r0, r%d\n", REG_STACK(reg));
  reg++;

  return 0;
}

int TMS9900::dup2()
{
  fprintf(out, "  mov r%d, r0\n", REG_STACK(reg-2));
  fprintf(out, "  mov r%d, r1\n", REG_STACK(reg-1));

  CHECK_STACK();
  fprintf(out, "  mov r0, r%d\n", REG_STACK(reg));
  reg++;

  CHECK_STACK();
  fprintf(out, "  mov r1, r%d\n", REG_STACK(reg));
  reg++;

  return 0;
}

int TMS9900::swap()
{
  fprintf(out, "  mov r%d, r0\n", REG_STACK(reg-1));
  fprintf(out, "  mov r%d, r%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
  fprintf(out, "  mov r0, r%d\n", REG_STACK(reg-2));
  return 0;
}

int TMS9900::add_integer()
{
  fprintf(out, "  a r%d, r%d\n", REG_STACK(reg-1), REG_STACK(reg-2));
  reg--;

  return 0;
}

int TMS9900::add_integer(int num)
{
  if (num == 1)
  {
    fprintf(out, "  inc r%d\n", REG_STACK(reg-1));
    return 0;
  }

  if (num == 2)
  {
    fprintf(out, "  inct r%d\n", REG_STACK(reg-1));
    return 0;
  }

  if (num == -1)
  {
    fprintf(out, "  dec r%d\n", REG_STACK(reg-1));
    return 0;
  }

  if (num == -2)
  {
    fprintf(out, "  dect r%d\n", REG_STACK(reg-1));
    return 0;
  }

  return -1;
}

int TMS9900::sub_integer()
{
  fprintf(out, "  s r%d, r%d\n", REG_STACK(reg-1), REG_STACK(reg-2));
  reg--;

  return 0;
}

int TMS9900::sub_integer(int num)
{
  if (num == 1)
  {
    fprintf(out, "  dec r%d\n", REG_STACK(reg-1));
    return 0;
  }

  if (num == 2)
  {
    fprintf(out, "  dect r%d\n", REG_STACK(reg-1));
    return 0;
  }

  if (num == -1)
  {
    fprintf(out, "  inc r%d\n", REG_STACK(reg-1));
    return 0;
  }

  if (num == -2)
  {
    fprintf(out, "  inct r%d\n", REG_STACK(reg-1));
    return 0;
  }
  return -1;
}

int TMS9900::mul_integer()
{
  fprintf(out, "  mpy r%d, r%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
  fprintf(out, "  mov r%d, r%d\n", REG_STACK(reg-1), REG_STACK(reg-2));
  reg--;
  return 0;
}

int TMS9900::div_integer()
{
  fprintf(out, "  div r%d, r%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
  reg--;
  return 0;
}

int TMS9900::mod_integer()
{
  fprintf(out, "  div r%d, r%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
  fprintf(out, "  mov r%d, r%d\n", REG_STACK(reg-1), REG_STACK(reg-2));
  reg--;
  return 0;
}

int TMS9900::neg_integer()
{
  fprintf(out, "  neg r%d\n", REG_STACK(reg-1));
  return 0;
}

int TMS9900::shift_left_integer()
{
  fprintf(out, "  mov r%d, r0\n", REG_STACK(reg-1));
  fprintf(out, "  sla r%d, r0\n", REG_STACK(reg-2));
  reg--;
  return 0;
}

int TMS9900::shift_left_integer(int num)
{
  if (num == 0) { return 0; }
  fprintf(out, "  sla r%d, %d\n", REG_STACK(reg-1), num);
  reg--;
  return 0;
}

int TMS9900::shift_right_integer()
{
  fprintf(out, "  mov r%d, r0\n", REG_STACK(reg-1));
  fprintf(out, "  sra r%d, r0\n", REG_STACK(reg-2));
  reg--;
  return 0;
}

int TMS9900::shift_right_integer(int num)
{
  if (num == 0) { return 0; }
  fprintf(out, "  sra r%d, %d\n", REG_STACK(reg-1), num);
  reg--;
  return 0;
}

int TMS9900::shift_right_uinteger()
{
  fprintf(out, "  mov r%d, r0\n", REG_STACK(reg-1));
  fprintf(out, "  srl r%d, r0\n", REG_STACK(reg-2));
  reg--;
  return 0;
}

int TMS9900::shift_right_uinteger(int num)
{
  if (num == 0) { return 0; }
  fprintf(out, "  srl r%d, %d\n", REG_STACK(reg-1), num);
  reg--;
  return 0;
}

int TMS9900::and_integer()
{
  fprintf(out, "  seto r0\n");
  fprintf(out, "  xor r%d, r0\n", REG_STACK(reg-1));
  fprintf(out, "  szc r%d, r%d\n", REG_STACK(reg-1), REG_STACK(reg-2));
  reg--;
  return 0;
}

int TMS9900::and_integer(int num)
{
  if (num < -32768 || num > 0xffff) { return -1; }
  fprintf(out, "  andi r%d, %d\n", REG_STACK(reg-1), num);
  return 0;
}

int TMS9900::or_integer()
{
  fprintf(out, "  soc r%d, r%d\n", REG_STACK(reg-1), REG_STACK(reg-2));
  reg--;
  return 0;
}

int TMS9900::or_integer(int num)
{
  if (num < -32768 || num > 0xffff) { return -1; }
  fprintf(out, "  ori r%d, %d\n", REG_STACK(reg-1), num);
  return 0;
}

int TMS9900::xor_integer()
{
  fprintf(out, "  xor r%d, r%d\n", REG_STACK(reg-1), REG_STACK(reg-2));
  reg--;

  return 0;
}

int TMS9900::xor_integer(int num)
{
  fprintf(out, "  li r0, %d\n", num);
  fprintf(out, "  xor r0, r%d\n", REG_STACK(reg-1));

  return 0;
}

int TMS9900::inc_integer(int index, int num)
{
  fprintf(out, "  li r0, %d\n", num);
  fprintf(out, "  a r0, @-%d(r10)\n", LOCALS(index));
  return 0;
}

int TMS9900::integer_to_byte()
{
  sign_extend();
  return 0;
}

int TMS9900::jump_cond(const char *label, int cond)
{
  fprintf(out, "  ci r%d, 0\n", REG_STACK(reg-1));
  reg--;

  if (cond == COND_LESS_EQUAL)
  {
    fprintf(out, "  jlt %s\n", label);
    fprintf(out, "  jeq %s\n", label);
  }
    else
  if (cond == COND_GREATER_EQUAL)
  {
    fprintf(out, "  jlt %s\n", label);
    fprintf(out, "  jeq %s\n", label);
  }
    else
  {
    fprintf(out, "  %s %s\n", cond_str[cond], label);
  }

  return 0;
}

int TMS9900::jump_cond_integer(const char *label, int cond)
{
  if (cond == COND_GREATER_EQUAL)
  {
    fprintf(out, "  c r%d, r%d\n", REG_STACK(reg-1), REG_STACK(reg-2));
  }
    else
  {
    fprintf(out, "  c r%d, r%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
  }
  reg -= 2;

  if (cond == COND_LESS_EQUAL)
  {
    fprintf(out, "  jlt %s\n", label);
    fprintf(out, "  jeq %s\n", label);
  }
    else
  if (cond == COND_GREATER_EQUAL)
  {
    fprintf(out, "  jlt %s\n", label);
    fprintf(out, "  jeq %s\n", label);
  }
    else
  {
    fprintf(out, "  %s %s\n", cond_str[cond], label);
  }

  return 0;
}

int TMS9900::return_local(int index, int local_count)
{
  return -1;
}

int TMS9900::return_integer(int local_count)
{
  return -1;
}

int TMS9900::return_void(int local_count)
{
  return -1;
}

int TMS9900::jump(const char *name)
{
  fprintf(out, "  jmp %s\n", name);
  return 0;
}

int TMS9900::call(const char *name)
{
  return -1;
}

int TMS9900::invoke_static_method(const char *name, int params, int is_void)
{
  return -1;
}

int TMS9900::put_static(const char *name, int index)
{
  return -1;
}

int TMS9900::get_static(const char *name, int index)
{
  return -1;
}

int TMS9900::brk()
{
  return -1;
}

int TMS9900::new_array(uint8_t type)
{
  return -1;
}

int TMS9900::insert_array(const char *name, int32_t *data, int len, uint8_t type)
{
  fprintf(out, ".align 16\n");
  if (type == TYPE_BYTE)
  { return insert_db(name, data, len, TYPE_INT); }
    else
  if (type == TYPE_SHORT)
  { return insert_dw(name, data, len, TYPE_INT); } 
    else
  if (type == TYPE_INT)
  { return insert_dw(name, data, len, TYPE_INT); } 

  return -1;
}

int TMS9900::insert_string(const char *name, uint8_t *bytes, int len)
{
  fprintf(out, ".align 16\n");
  fprintf(out, "  dc16 %d\n", len);
  return insert_utf8(name, bytes, len);
}

int TMS9900::push_array_length()
{
  fprintf(out, "  mov @-2(r%d), r%d\n", REG_STACK(reg-1), REG_STACK(reg-1));

  return 0;
}

int TMS9900::push_array_length(const char *name, int field_id)
{
  fprintf(out, "  mov &%s, r0\n", name);
  fprintf(out, "  mov @-2(r0), r%d\n", REG_STACK(reg));
  return 0;
}

int TMS9900::array_read_byte()
{
  fprintf(out, "  a r%d, r%d\n", REG_STACK(reg-1), REG_STACK(reg-2));
  fprintf(out, "  movb *r%d, r%d\n", REG_STACK(reg-2), REG_STACK(reg-2));
  reg--;
  sign_extend();
  return 0;
}

int TMS9900::array_read_short()
{
  fprintf(out, "  sla r%d, 1\n", REG_STACK(reg-1));
  fprintf(out, "  a r%d, r%d\n", REG_STACK(reg-1), REG_STACK(reg-2));
  fprintf(out, "  mov *r%d, r%d\n", REG_STACK(reg-2), REG_STACK(reg-2));
  reg--;
  return 0;
}

int TMS9900::array_read_int()
{
  return array_read_short();
}

int TMS9900::array_read_byte(const char *name, int field_id)
{
  fprintf(out, "  mov @%s, r13\n", name);
  fprintf(out, "  a r%d, r13\n", REG_STACK(reg-1));
  fprintf(out, "  movb *r13, r%d\n", REG_STACK(reg-1));
  sign_extend();

  return 0;
}

int TMS9900::array_read_short(const char *name, int field_id)
{
  fprintf(out, "  mov @%s, r13\n", name);
  fprintf(out, "  sla r%d\n", REG_STACK(reg-1));
  fprintf(out, "  a r%d, r13\n", REG_STACK(reg-1));
  fprintf(out, "  mov @r13, r%d\n", REG_STACK(reg-1));
  return 0;
}

int TMS9900::array_read_int(const char *name, int field_id)
{
  return array_read_short(name, field_id);
}

int TMS9900::array_write_byte()
{
  fprintf(out, "  a r%d, r%d\n", REG_STACK(reg-2), REG_STACK(reg-3));
  fprintf(out, "  movb r%d, *r%d\n", REG_STACK(reg-1), REG_STACK(reg-3));
  reg -= 3;
  return 0;
}

int TMS9900::array_write_short()
{
  fprintf(out, "  sla r%d, 1\n", REG_STACK(reg-2));
  fprintf(out, "  a r%d, r%d\n", REG_STACK(reg-2), REG_STACK(reg-3));
  fprintf(out, "  mov r%d, *r%d\n", REG_STACK(reg-1), REG_STACK(reg-3));
  reg -= 3;
  return 0;
}

int TMS9900::array_write_int()
{
  return array_write_short();
}

int TMS9900::array_write_byte(const char *name, int field_id)
{
  fprintf(out, "  a @%s, r%d\n", name, REG_STACK(reg-2));
  fprintf(out, "  movb r%d, *r%d\n", REG_STACK(reg-1), REG_STACK(reg-2));
  return 0;
}

int TMS9900::array_write_short(const char *name, int field_id)
{
  fprintf(out, "  sla r%d, 1\n", REG_STACK(reg-2));
  fprintf(out, "  a @%s, r%d\n", name, REG_STACK(reg-2));
  fprintf(out, "  mov r%d, *r%d\n", REG_STACK(reg-1), REG_STACK(reg-2));
  return 0;
}

int TMS9900::array_write_int(const char *name, int field_id)
{
  return array_write_short(name, field_id);
}

void TMS9900::sign_extend()
{
  fprintf(out, "  li r0, 0x80\n");
  fprintf(out, "  coc r%d, r0\n", REG_STACK(reg-1));
  fprintf(out, "  jne label_%d\n", label_count);
  fprintf(out, "  ori r%d, #0xff00\n", REG_STACK(reg-1));
  fprintf(out, "label_%d\n", label_count);
  label_count++;
}

