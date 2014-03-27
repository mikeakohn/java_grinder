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

#include "AVR8.h"

// ABI is:
// r0 return0
// r1 return1
// r2 result0
// r3 result1
// r4 remainder0
// r5 remainder1
// r6 length0
// r7 length1
// r8 value10
// r9 value11
// r10 value20
// r11 value21
// r12 value30
// r13 value31
// r14
// r15
// r16 reserved for load/store immediate
// r17 reserved for load/store immediate
// r18
// r19
// r20
// r21
// r22
// r23
// r24
// r25
// r26 XL points to locals
// r27 XH always 0
// r28 YL java stack pointer
// r29 YH always 0
// r30 ZL
// r31 ZH

// java stack is first 128 bytes of RAM (2 parallel lo/hi byte stacks)
// processor stack is at RAMEND
// heap starts at 0x100


AVR8::AVR8() :
  stack(0),
  is_main(0),

  need_swap(0),
  need_add_integer(0),
  need_sub_integer(0),
  need_mul_integer(0),
  need_div_integer(0),
  need_mod_integer(0),
  need_neg_integer(0),
  need_shift_left_integer(0),
  need_shift_right_integer(0),
  need_shift_right_uinteger(0),
  need_and_integer(0),
  need_or_integer(0),
  need_xor_integer(0),
  need_push_array_length(0),
  need_push_array_length2(0),
  need_array_byte_support(0),
  need_array_int_support(0)
{

}

AVR8::~AVR8()
{
  if(need_swap) { insert_swap(); }
  if(need_add_integer) { insert_add_integer(); }
  if(need_sub_integer) { insert_sub_integer(); }
  if(need_mul_integer) { insert_mul_integer(); }
  if(need_div_integer) { insert_div_integer(); }
  if(need_mod_integer) { insert_mod_integer(); }
  if(need_neg_integer) { insert_neg_integer(); }
  if(need_shift_left_integer) { insert_shift_left_integer(); }
  if(need_shift_right_integer) { insert_shift_right_integer(); }
  if(need_shift_right_uinteger) { insert_shift_right_uinteger(); }
  if(need_and_integer) { insert_and_integer(); }
  if(need_or_integer) { insert_or_integer(); }
  if(need_xor_integer) { insert_xor_integer(); }
  if(need_push_array_length) { insert_push_array_length(); }
  if(need_push_array_length2) { insert_push_array_length2(); }
  if(need_array_byte_support) { insert_array_byte_support(); }
  if(need_array_int_support) { insert_array_int_support(); }
}

int AVR8::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".avr8\n");

  // heap
  fprintf(out, "ram_start equ 0x8000\n");
  fprintf(out, "heap_ptr equ ram_start\n");

  // temp vars (held in registers to save ram)
  fprintf(out, "return0 equ r0\n");
  fprintf(out, "return1 equ r1\n");
  fprintf(out, "result0 equ r2\n");
  fprintf(out, "result1 equ r3\n");
  fprintf(out, "remainder0 equ r4\n");
  fprintf(out, "remainder1 equ r5\n");
  fprintf(out, "length0 equ r6\n");
  fprintf(out, "length1 equ r7\n");
  fprintf(out, "value10 equ r8\n");
  fprintf(out, "value11 equ r9\n");
  fprintf(out, "value20 equ r10\n");
  fprintf(out, "value21 equ r11\n");
  fprintf(out, "value30 equ r12\n");
  fprintf(out, "value31 equ r13\n");

  // points to locals
  fprintf(out, "locals equ r26\n");

  // java stack pointer
  fprintf(out, "SP equ r28\n");

  // RAMEND (alter to chip)
  fprintf(out, "RAMEND equ 0x1ff\n");

  // startup
  fprintf(out, ".org 0x0000\n");
  fprintf(out, "start:\n");

  // processor stack pointer setup
  fprintf(out, "  ldi r16, RAMEND & 0xff\n");
  fprintf(out, "  out 0x5d, r16\n");
  fprintf(out, "  ldi r16, RAMEND >> 8\n");
  fprintf(out, "  out 0x5e, r16\n");

  // java stack pointer setup
  fprintf(out, "  ldi r28, 0x3f\n");
  fprintf(out, "  ldi r29, 0x00\n");

  return 0;
}

int AVR8::start_init()
{
  return 0;
}

int AVR8::insert_static_field_define(const char *name, const char *type, int index)
{
//FIXME test this
  fprintf(out, "%s equ ram_start + %d\n", name, (index + 1) * 2);

  return 0;
}

int AVR8::init_heap(int field_count)
{
  fprintf(out, "  ; Set up heap and static initializers\n");

  return 0;
}

int AVR8::insert_field_init_boolean(char *name, int index, int value)
{
//  value = (value == 0) ? 0 : 1;
  fprintf(out, "; insert_field_init_boolean\n");

  return 0;
}

int AVR8::insert_field_init_byte(char *name, int index, int value)
{
//  if (value < -128 || value > 255) { return -1; }
//  int16_t n = value;
//  uint16_t v = (n & 0xffff);


  fprintf(out, "; insert_field_init_short\n");

  return 0;
}

int AVR8::insert_field_init_short(char *name, int index, int value)
{
//  if (value < -32768 || value > 65535) { return -1; }

  fprintf(out, "; insert_field_init_short\n");

  return 0;
}

int AVR8::insert_field_init_int(char *name, int index, int value)
{
  return insert_field_init_short(name, index, value);
}

int AVR8::insert_field_init(char *name, int index)
{
  fprintf(out, "; insert_field_init\n");

  return 0;
}

void AVR8::method_start(int local_count, int max_stack, int param_count, const char *name)
{
  stack = 0;

  is_main = (strcmp(name, "main") == 0) ? 1 : 0;

  fprintf(out, "%s:\n", name);

  // main() function goes here
  if (!is_main)
  {
  }
}

void AVR8::method_end(int local_count)
{
  fprintf(out, "\n");
}

int AVR8::push_integer(int32_t n)
{
  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);

    return -1;
  }

//  uint16_t value = (n & 0xffff);

  fprintf(out, "; push_integer\n");
  stack++;

  return 0;
}

int AVR8::push_integer_local(int index)
{
  fprintf(out, "; push_integer_local\n");
  stack++;

  return 0;
}

int AVR8::push_ref_local(int index)
{
  return push_integer_local(index);
}

int AVR8::push_fake()
{
  return -1;
}

int AVR8::push_long(int64_t n)
{
  return push_integer((int32_t)n);
}

int AVR8::push_float(float f)
{
  printf("float is not supported right now\n");

  return -1;
}

int AVR8::push_double(double f)
{
  printf("double is not supported right now\n");

  return -1;
}

int AVR8::push_byte(int8_t b)
{
//  int16_t n = b;
//  uint16_t value = (n & 0xffff);

  fprintf(out, "; push_byte\n");
  stack++;

  return 0;
}

int AVR8::push_short(int16_t s)
{
//  uint16_t value = (s & 0xffff);

  fprintf(out, "; push_short\n");
  stack++;

  return 0;
}

int AVR8::pop_integer_local(int index)
{
  fprintf(out, "; pop_integer_local\n");
  stack--;

  return 0;
}

int AVR8::pop_ref_local(int index)
{
  return pop_integer_local(index);
}

int AVR8::pop()
{
  fprintf(out, "; pop\n");
  stack--;

  return 0;
}

int AVR8::dup()
{
  fprintf(out, "; dup\n");
  stack++;

  return 0;
}

int AVR8::dup2()
{
  printf("Need to implement dup2()\n");

  return -1;
}

int AVR8::swap()
{
  need_swap = 1;
  fprintf(out, "  call swap\n");

  return 0;
}

int AVR8::add_integer()
{
  need_add_integer = 1;
  fprintf(out, "  call add_integer\n");
  stack--;

  return 0;
}

int AVR8::add_integer(int const_val)
{
  return -1;
}

int AVR8::sub_integer()
{
  need_sub_integer = 1;
  fprintf(out, "  call sub_integer\n");
  stack--;

  return 0;
}

int AVR8::sub_integer(int const_val)
{
  return -1;
}

int AVR8::mul_integer()
{
  need_mul_integer = 1;
  fprintf(out, "  call mul_integer\n");
  stack--;

  return 0;
}

int AVR8::mul_integer(int const_val)
{
  return -1;
}

// unsigned only for now
int AVR8::div_integer()
{
  need_div_integer = 1;
  fprintf(out, "  call div_integer\n");
  stack--;

  return 0;
}

int AVR8::div_integer(int const_val)
{
  return -1;
}

// unsigned only for now
int AVR8::mod_integer()
{
  need_div_integer = 1;
  need_mod_integer = 1;
  fprintf(out, "  call div_integer\n");
  fprintf(out, "  call mod_integer\n");
  stack--;

  return 0;
}

int AVR8::mod_integer(int const_val)
{
  return -1;
}

int AVR8::neg_integer()
{
  need_neg_integer = 1;
  fprintf(out, "  call neg_integer\n");

  return 0;
}

int AVR8::shift_left_integer()
{
  need_shift_left_integer = 1;
  fprintf(out, "  call shift_left_integer\n");
  stack--;

  return 0;
}

int AVR8::shift_left_integer(int const_val)
{
  return -1;
}

int AVR8::shift_right_integer()
{
  need_shift_right_integer = 1;
  fprintf(out, "  call shift_right_integer\n");
  stack--;

  return 0;
}

int AVR8::shift_right_integer(int const_val)
{
  return -1;
}

int AVR8::shift_right_uinteger()
{
  need_shift_right_uinteger = 1;
  fprintf(out, "  call shift_right_uinteger\n");
  stack--;

  return 0;
}

int AVR8::shift_right_uinteger(int const_val)
{
  return -1;
}

int AVR8::and_integer()
{
  need_and_integer = 1;
  fprintf(out, "  call and_integer\n");
  stack--;

  return 0;
}

int AVR8::and_integer(int const_val)
{
  return -1;
}

int AVR8::or_integer()
{
  need_or_integer = 1;
  fprintf(out, "  call or_integer\n");
  stack--;

  return 0;
}

int AVR8::or_integer(int const_val)
{
  return -1;
}

int AVR8::xor_integer()
{
  need_xor_integer = 1;
  fprintf(out, "  call xor_integer\n");
  stack--;

  return 0;
}

int AVR8::xor_integer(int const_val)
{
  return -1;
}

int AVR8::inc_integer(int index, int num)
{
 // uint16_t value = num & 0xffff;

  fprintf(out, "; inc_integer num = %d\n", num);
  return 0;
}

int AVR8::integer_to_byte()
{
  fprintf(out, "; integer_to_byte\n");
  return 0;
}

int AVR8::jump_cond(const char *label, int cond)
{
  bool reverse = false;

  if (stack > 0)
  {
    fprintf(out, "; jump_cond\n");

    if(cond == COND_LESS_EQUAL)
    {
      reverse = true;
      cond = COND_GREATER_EQUAL;
    }
      else
    if(cond == COND_GREATER)
    {
      reverse = true;
      cond = COND_LESS;
    }

    switch(cond)
    {
      case COND_EQUAL:
        break;
      case COND_NOT_EQUAL:
        break;
      case COND_LESS:
        if(reverse == false)
        {
        }
          else
        {
        }
        break;
      case COND_GREATER_EQUAL:
        if(reverse == false)
        {
        }
          else
        {
        }
        break;
    }

    stack--;
  }

  return 0;
}

int AVR8::jump_cond_integer(const char *label, int cond)
{
  bool reverse = false;

  if (stack > 1)
  {
    fprintf(out, "; jump_cond_integer\n");

    if(cond == COND_LESS_EQUAL)
    {
      reverse = true;
      cond = COND_GREATER_EQUAL;
    }
      else
    if(cond == COND_GREATER)
    {
      reverse = true;
      cond = COND_LESS;
    }

    switch(cond)
    {
      case COND_EQUAL:
        break;
      case COND_NOT_EQUAL:
        break;
      case COND_LESS:
        if(reverse == false)
        {
        }
          else
        {
        }
        break;
      case COND_GREATER_EQUAL:
        if(reverse == false)
        {
        }
          else
        {
        }
        break;
    }

    stack -= 2;
  }

  return 0;
}

int AVR8::return_local(int index, int local_count)
{
  fprintf(out, "; return_local\n");

  if (!is_main)
  {
  }

  fprintf(out, "  ret\n");

  return 0;
}

int AVR8::return_integer(int local_count)
{
  fprintf(out, "; return_integer\n");

  if (!is_main)
  {
  }

  fprintf(out, "  ret\n");

  return 0;
}

int AVR8::return_void(int local_count)
{
  fprintf(out, "; return_void\n");

  if (!is_main)
  {
  }

  fprintf(out, "  ret\n");

  return 0;
}

int AVR8::jump(const char *name)
{
  fprintf(out, "  jmp %s\n", name);

  return 0;
}

int AVR8::call(const char *name)
{
  fprintf(out, "  call %s\n", name);

  return 0;
}

int AVR8::invoke_static_method(const char *name, int params, int is_void)
{
int local;
int stack_vars = stack;

  printf("invoke_static_method() name=%s params=%d is_void=%d\n", name, params, is_void);

  fprintf(out, "; invoke_static_method\n");

  local = -params;
  while(local != 0)
  {
    if (stack_vars > 0)
    {
      //fprintf(out, "  ldx SP\n");
      //fprintf(out, "  lda stack_lo + 1 + %d,x\n", (stack - stack_vars));
      //fprintf(out, "  sta stack_lo + 1 %d,x\n", local-1);
      //fprintf(out, "  lda stack_hi + 1 + %d,x\n", (stack - stack_vars));
      //fprintf(out, "  sta stack_hi + 1 %d,x\n", local-1);
      stack_vars--;
    }

    local++;
  }

  fprintf(out, "  call %s\n", name);

  if ((stack - stack_vars) > 0)
  {
    params -= (stack - stack_vars);
  }

  if (!is_void)
  {
    stack++;
  }

  return 0;
}

int AVR8::put_static(const char *name, int index)
{
  if (stack > 0)
  {
    stack--;
  }

  return 0;
}

int AVR8::get_static(const char *name, int index)
{
  stack++;

  return 0;
}

int AVR8::brk()
{
  return -1;
}

int AVR8::new_array(uint8_t type)
{
  if (stack > 0)
  {
    if (type == TYPE_SHORT || type == TYPE_CHAR || type == TYPE_INT)
    {
      need_array_int_support = 1;
      fprintf(out, "call new_array_int\n");
    }
      else
    {
      need_array_byte_support = 1;
      fprintf(out, "call new_array_byte\n");
    }
  }

  return 0;
}

int AVR8::insert_array(const char *name, int32_t *data, int len, uint8_t type)
{
  fprintf(out, "; insert_array\n");

  if (type == TYPE_BYTE)
  {
    fprintf(out, ".align 16\n");
    return insert_db(name, data, len, TYPE_SHORT);
  }
    else
  if (type == TYPE_SHORT)
  {
    fprintf(out, ".align 16\n");
    return insert_dw(name, data, len, TYPE_SHORT);
  }
    else
  if (type == TYPE_INT)
  {
    fprintf(out, ".align 16\n");
    return insert_dw(name, data, len, TYPE_SHORT);
  }

  return -1;
}

int AVR8::insert_string(const char *name, uint8_t *bytes, int len)
{
  return -1;
}

int AVR8::push_array_length()
{
  if (stack > 0)
  {
    need_push_array_length = 1;
    fprintf(out, "call push_array_length\n");
  }

  return 0;
}

int AVR8::push_array_length(const char *name, int field_id)
{
  need_push_array_length2 = 1;
  fprintf(out, "call push_array_length2\n");
  stack++;

  return 0;
}

int AVR8::array_read_byte()
{
  get_values_from_stack(2);
  fprintf(out, "call array_read_byte\n");
  stack++;

  return 0;
}

int AVR8::array_read_short()
{
  return array_read_int();
}

int AVR8::array_read_int()
{
  get_values_from_stack(2);
  fprintf(out, "call array_read_int\n");
  stack++;

  return 0;
}

int AVR8::array_read_byte(const char *name, int field_id)
{
  need_array_byte_support = 1;
  if (stack > 0)
  {
    fprintf(out, "call array_read_byte2\n");
    stack++;
  }

  return 0;
}

int AVR8::array_read_short(const char *name, int field_id)
{
  return array_read_int(name, field_id);
}

int AVR8::array_read_int(const char *name, int field_id)
{
  need_array_int_support = 1;

  if (stack > 0)
  {
    fprintf(out, "call array_read_int2\n");
    stack++;
  }

  return 0;
}

int AVR8::array_write_byte()
{
  get_values_from_stack(3);
  fprintf(out, "call array_write_byte\n");

  return 0;
}

int AVR8::array_write_short()
{
  return array_write_int();
}

int AVR8::array_write_int()
{
  get_values_from_stack(3);
  fprintf(out, "call array_write_int\n");

  return 0;
}

int AVR8::array_write_byte(const char *name, int field_id)
{
  get_values_from_stack(2);
  fprintf(out, "; array_write_byte2\n");

  return 0;
}

int AVR8::array_write_short(const char *name, int field_id)
{
  return array_write_int(name, field_id);
}

int AVR8::array_write_int(const char *name, int field_id)
{
  get_values_from_stack(2);
  fprintf(out, "; array_write_int2\n");

  return 0;
}

int AVR8::get_values_from_stack(int num)
{
  fprintf(out, "; get_values_from_stack, num = %d\n", num);
  if(num > 0)
  {
    stack--;
  }

  if(num > 1)
  {
    stack--;
  }

  if(num > 2)
  {
    stack--;
  }

  return 0;
}

// subroutines
void AVR8::insert_swap()
{
//FIXME untested
  fprintf(out, "swap:\n");
}

void AVR8::insert_add_integer()
{
  fprintf(out, "add_integer:\n");
}

void AVR8::insert_sub_integer()
{
  fprintf(out, "sub_integer:\n");
}

void AVR8::insert_mul_integer()
{
  fprintf(out, "mul_integer:\n");
}

void AVR8::insert_div_integer()
{
  fprintf(out, "div_integer:\n");
}

void AVR8::insert_mod_integer()
{
  fprintf(out, "mod_integer:\n");
}

void AVR8::insert_neg_integer()
{
  fprintf(out, "neg_integer:\n");
}

void AVR8::insert_shift_left_integer()
{
  fprintf(out, "shift_left_integer:\n");
}

void AVR8::insert_shift_right_integer()
{
  fprintf(out, "shift_right_integer:\n");
}

void AVR8::insert_shift_right_uinteger()
{
  fprintf(out, "shift_right_uinteger:\n");
}

void AVR8::insert_and_integer()
{
  fprintf(out, "and_integer:\n");
}

void AVR8::insert_or_integer()
{
  fprintf(out, "or_integer:\n");
}

void AVR8::insert_xor_integer()
{
  fprintf(out, "xor_integer:\n");
}

void AVR8::insert_push_array_length()
{
  // push_array_length
  fprintf(out, "push_array_length:\n");
}

void AVR8::insert_push_array_length2()
{
  fprintf(out, "push_array_length2:\n");
}

void AVR8::insert_array_byte_support()
{
  // new_array byte
  fprintf(out, "new_array_byte:\n");

  // array_read_byte
  fprintf(out, "array_read_byte:\n");

  // array_read_byte2
  fprintf(out, "array_read_byte2:\n");

  // array_write_byte
  fprintf(out, "array_write_byte:\n");
}

void AVR8::insert_array_int_support()
{
  // new_array int
  fprintf(out, "new_array_int:\n");

  // array_read_int
  fprintf(out, "array_read_int:\n");

  // array_read_int2
  fprintf(out, "array_read_int2:\n");

  // array_write_int
  fprintf(out, "array_write_int:\n");
}

// Memory API
int AVR8::memory_read8()
{
  fprintf(out, "; memory_read8\n");

  return 0;
}

int AVR8::memory_write8()
{
  return 0;
}

#if 0
void AVR8::close()
{
}
#endif

