/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn, Joe Davisson
 *
 * AVR8 written by Joe Davisson
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "AVR8.h"

// ABI is:
// r0 result0
// r1 result1
// r2 remainder0
// r3 remainder1
// r4 length0 
// r5 length1
// r6
// r7
// r8
// r9
// r10 zero
// r11 one
// r12 two
// r13 three
// r14 ff
// r15
// r16 value10
// r17 value11
// r18 value20
// r19 value21
// r20 value30
// r21 value31
// r22 temp
// r23 temp2
// r24 locals
// r25 SP
// r26 XL
// r27 XH
// r28 YL
// r29 YH
// r30 ZL
// r31 ZH

#define PUSH_LO(a) \
  fprintf(out, "; PUSH_LO\n"); \
  fprintf(out, "  ldi YL, stack_lo\n"); \
  fprintf(out, "  add YL, SP\n"); \
  fprintf(out, "  st Y, %s\n", a)

#define PUSH_HI(a) \
  fprintf(out, "; PUSH_HI\n"); \
  fprintf(out, "  ldi YL, stack_hi\n"); \
  fprintf(out, "  add YL, SP\n"); \
  fprintf(out, "  st Y, %s\n", a); \
  fprintf(out, "  dec SP\n")

#define POP_HI(a) \
  fprintf(out, "; POP_HI\n"); \
  fprintf(out, "  inc SP\n"); \
  fprintf(out, "  ldi YL, stack_hi\n"); \
  fprintf(out, "  add YL, SP\n"); \
  fprintf(out, "  ld %s, Y\n", a)

#define POP_LO(a) \
  fprintf(out, "; POP_LO\n"); \
  fprintf(out, "  ldi YL, stack_lo\n"); \
  fprintf(out, "  add YL, SP\n"); \
  fprintf(out, "  ld %s, Y\n", a)

#define JUMP(a) \
  if(need_farjump) \
    fprintf(out, "  jmp %s\n", a); \
  else \
    fprintf(out, "  rjmp %s\n", a)

#define CALL(a) \
  if(need_farjump) \
    fprintf(out, "  call %s\n", a); \
  else \
    fprintf(out, "  rcall %s\n", a)

#define LOCALS(a) (a)

//static const char *pin_string[4] = { "PINA", "PINB", "PINC", "PIND" };
static const char *ddr_string[4] = { "DDRA", "DDRB", "DDRC", "DDRD" };
static const char *port_string[4] = { "PORTA", "PORTB", "PORTC", "PORTD" };

static char *adc_in_string;
static char *adc_out_string;

AVR8::AVR8(uint8_t chip_type) :
  stack(0),
  is_main(0),
  need_farjump(0),
  need_memory_mapped_adc(0),
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
  need_inc_integer(0),
  need_integer_to_byte(0),
  need_jump_cond(0),
  need_jump_cond_integer(0),
  need_push_local_var_int(0),
  need_pop_local_var_int(0),
  need_dup(0),
  need_push_array_length(0),
  need_push_array_length2(0),
  need_array_byte_support(0),
  need_array_int_support(0),
  need_get_values_from_stack(0)
{
  switch(chip_type)
  {
    case ATTINY13:
      include_file = "tn13def.inc";
      need_farjump = 0;
      break;
    case ATTINY85:
      include_file = "tn85def.inc";
      need_farjump = 0;
      break;
    case ATTINY84:
      include_file = "tn84def.inc";
      need_farjump = 1;
      break;
    case ATTINY2313:
      include_file = "tn2313def.inc";
      need_farjump = 0;
      break;
    case ATMEGA328:
      include_file = "m328def.inc";
      need_farjump = 1;
      need_memory_mapped_adc = 1;
      break;
    case ATMEGA328P:
      include_file = "m328pdef.inc";
      need_farjump = 1;
      need_memory_mapped_adc = 1;
      break;
  }

  if(need_memory_mapped_adc)
  {
    adc_in_string = (char *)"lds";
    adc_out_string = (char *)"sts";
  }
  else
  {
    adc_in_string = (char *)"in";
    adc_out_string = (char *)"out";
  }
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
  if(need_inc_integer) { insert_inc_integer(); }
  if(need_integer_to_byte) { insert_integer_to_byte(); }
  if(need_jump_cond) { insert_jump_cond(); }
  if(need_jump_cond_integer) { insert_jump_cond_integer(); }
  if(need_push_local_var_int) { insert_push_local_var_int(); }
  if(need_pop_local_var_int) { insert_pop_local_var_int(); }
  if(need_dup) { insert_dup(); }
  if(need_push_array_length) { insert_push_array_length(); }
  if(need_push_array_length2) { insert_push_array_length2(); }
  if(need_array_byte_support) { insert_array_byte_support(); }
  if(need_array_int_support) { insert_array_int_support(); }
  if(need_get_values_from_stack) { insert_get_values_from_stack(); }
}

int AVR8::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  // include file
  fprintf(out, ".avr8\n");
  fprintf(out, ".include \"%s\"\n\n", include_file);

  // java stack base locations
  fprintf(out, ".if SRAM_SIZE > 256\n");
  fprintf(out, "  .define JAVA_STACK_SIZE 64\n");
  fprintf(out, ".else\n");
  fprintf(out, "  .define JAVA_STACK_SIZE (SRAM_SIZE / 4)\n");
  fprintf(out, ".endif\n\n");

  fprintf(out, "stack_lo equ (SRAM_START & 0xff)\n");
  fprintf(out, "stack_hi equ (SRAM_START & 0xff) + JAVA_STACK_SIZE\n");

  // heap
  fprintf(out, "ram_start equ SRAM_START + (JAVA_STACK_SIZE * 2)\n");
  fprintf(out, "heap_ptr equ ram_start\n\n");

  return 0;
}

int AVR8::start_init()
{
  // registers
  fprintf(out, "result0 equ r0\n");
  fprintf(out, "result1 equ r1\n");
  fprintf(out, "remainder0 equ r2\n");
  fprintf(out, "remainder1 equ r3\n");
  fprintf(out, "length0 equ r4\n");
  fprintf(out, "length1 equ r5\n");
  fprintf(out, "zero equ r10\n");
  fprintf(out, "one equ r11\n");
  fprintf(out, "two equ r12\n");
  fprintf(out, "three equ r13\n");
  fprintf(out, "ff equ r14\n");
  fprintf(out, "value10 equ r16\n");
  fprintf(out, "value11 equ r17\n");
  fprintf(out, "value20 equ r18\n");
  fprintf(out, "value21 equ r19\n");
  fprintf(out, "value30 equ r20\n");
  fprintf(out, "value31 equ r21\n");
  fprintf(out, "temp equ r22\n");
  fprintf(out, "temp2 equ r23\n");
  fprintf(out, "locals equ r24\n");
  fprintf(out, "SP equ r25\n\n");

  // startup
  fprintf(out, ".org 0x0000\n\n");
  fprintf(out, "  rjmp start\n");
  fprintf(out, "start:\n");

  // constants
  fprintf(out, "  ldi temp, 0\n");
  fprintf(out, "  mov zero, temp\n");
  fprintf(out, "  ldi temp, 1\n");
  fprintf(out, "  mov one, temp\n");
  fprintf(out, "  ldi temp, 2\n");
  fprintf(out, "  mov two, temp\n");
  fprintf(out, "  ldi temp, 3\n");
  fprintf(out, "  mov three, temp\n");
  fprintf(out, "  ldi temp, 0xff\n");
  fprintf(out, "  mov ff, temp\n\n");

  // java stack pointer
  fprintf(out, "  ldi SP, JAVA_STACK_SIZE - 1\n\n");

  // set YH register to java stack page
  fprintf(out, "  ldi YH, SRAM_START / 256\n\n");

  // processor stack pointer
  fprintf(out, "  ldi temp, RAMEND & 255\n");
  fprintf(out, "  out SPL, temp\n");
  fprintf(out, "  ldi temp, RAMEND >> 8\n");
  fprintf(out, "  out SPH, temp\n\n");

  return 0;
}

int AVR8::insert_static_field_define(const char *name, const char *type, int index)
{
  fprintf(out, "%s equ ram_start + %d\n", name, (index + 1) * 2);

  return 0;
}

int AVR8::init_heap(int field_count)
{
  fprintf(out, "; Set up heap and static initializers\n");
  fprintf(out, "  ldi temp, (ram_start + %d) & 0xff\n", (field_count + 1) * 2);
  fprintf(out, "  sts ram_start + 0, temp\n");
  fprintf(out, "  ldi temp, (ram_start + %d) >> 8\n", (field_count + 1) * 2);
  fprintf(out, "  sts ram_start + 1, temp\n");

  return 0;
}

int AVR8::field_init_int(char *name, int index, int value)
{
  if (value < -32768 || value > 65535) { return -1; }

  fprintf(out, "; field_init_short\n");
  fprintf(out, "  ldi temp, %d\n", value & 0xff);
  fprintf(out, "  sts %s + 0, temp\n", name);
  fprintf(out, "  ldi temp, %d\n", value >> 8);
  fprintf(out, "  sts %s + 1, temp\n", name);

  return 0;
}

int AVR8::field_init_ref(char *name, int index)
{
  fprintf(out, "; field_init_ref\n");
  fprintf(out, "  ldi temp, (_%s * 2) & 0xff\n", name);
  fprintf(out, "  sts %s + 0, temp\n", name);
  fprintf(out, "  ldi temp, (_%s * 2) >> 8\n", name);
  fprintf(out, "  sts %s + 1, temp\n", name);

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
    PUSH_LO("locals");
    PUSH_HI("zero");
  }

  fprintf(out, "  mov locals, SP\n");
  fprintf(out, "  ldi temp, 0x%02x\n", local_count);
  fprintf(out, "  sub SP, temp\n");
}

void AVR8::method_end(int local_count)
{
  fprintf(out, "\n");
}

int AVR8::push_local_var_int(int index)
{
  need_push_local_var_int = 1;

  fprintf(out, "  ldi temp2, %d\n", LOCALS(index));
  CALL("push_local_var_int");
  stack++;

  return 0;
}

int AVR8::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int AVR8::push_ref_static(const char *name, int index)
{
  return -1;
}

int AVR8::push_fake()
{
  return -1;
}

int AVR8::push_int(int32_t n)
{
  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);

    return -1;
  }

  uint16_t value = (n & 0xffff);

  fprintf(out, "; push_int\n");
  fprintf(out, "  ldi temp, 0x%02x\n", value & 0xff);
  PUSH_LO("temp");
  fprintf(out, "  ldi temp, 0x%02x\n", value >> 8);
  PUSH_HI("temp");
  stack++;

  return 0;
}

int AVR8::push_long(int64_t n)
{
  return push_int((int32_t)n);
}

#if 0
int AVR8::push_float(float f)
{
  return -1;
}

int AVR8::push_double(double f)
{
  return -1;
}
#endif

int AVR8::push_ref(char *name)
{
  fprintf(out, "; push_ref\n");
  fprintf(out, "  lds temp, %s + 0\n", name);
  PUSH_LO("temp");
  fprintf(out, "  lds temp, %s + 1\n", name);
  PUSH_HI("temp");
  stack++;

  return 0;
}

int AVR8::pop_local_var_int(int index)
{
  need_pop_local_var_int = 1;

  fprintf(out, "  ldi temp2, %d\n", LOCALS(index));
  CALL("pop_local_var_int");
  stack--;

  return 0;
}

int AVR8::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int AVR8::pop()
{
  fprintf(out, "; pop\n");
  POP_HI("result1");
  POP_LO("result0");
  stack--;

  return 0;
}

int AVR8::dup()
{
  need_dup = 1;

  CALL("dup");
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
  CALL("swap");

  return 0;
}

int AVR8::add_integer()
{
  need_add_integer = 1;
  CALL("add_integer");
  stack--;

  return 0;
}

int AVR8::add_integer(int const_val)
{
  if(const_val < 0 || const_val > 63) { return -1; }

  fprintf(out, "; add_integer (optimized)\n");
  POP_HI("ZH");
  POP_LO("ZL");
  fprintf(out, "  adiw ZL, 0x%02x\n", const_val);
  PUSH_LO("ZL");
  PUSH_HI("ZH");

  return 0;
}

int AVR8::sub_integer()
{
  need_sub_integer = 1;
  CALL("sub_integer");
  stack--;

  return 0;
}

int AVR8::sub_integer(int const_val)
{
  if(const_val < 0 || const_val > 63) { return -1; }

  fprintf(out, "; sub_integer (optimized)\n");
  POP_HI("ZH");
  POP_LO("ZL");
  fprintf(out, "  sbiw ZL, 0x%02x\n", const_val);
  PUSH_LO("ZL");
  PUSH_HI("ZH");

  return 0;
}

int AVR8::mul_integer()
{
  need_mul_integer = 1;
  CALL("mul_integer");
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
  CALL("div_integer");
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
  CALL("div_integer");
  CALL("mod_integer");
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
  CALL("neg_integer");

  return 0;
}

int AVR8::shift_left_integer()
{
  need_shift_left_integer = 1;
  CALL("shift_left_integer");
  stack--;

  return 0;
}

int AVR8::shift_left_integer(int const_val)
{
  int i;

  fprintf(out, "; shift_left_integer (optimized)\n");
  POP_HI("result1");
  POP_LO("result0");

  for(i = 0; i < const_val; i++)
  {
    fprintf(out, "  lsl result0\n");
    fprintf(out, "  rol result1\n");
  }

  PUSH_LO("result0");
  PUSH_HI("result1");

  return 0;
}

int AVR8::shift_right_integer()
{
  need_shift_right_integer = 1;
  CALL("shift_right_integer");
  stack--;

  return 0;
}

int AVR8::shift_right_integer(int const_val)
{
  // pointless
  return -1;
}

int AVR8::shift_right_uinteger()
{
  need_shift_right_uinteger = 1;
  CALL("shift_right_uinteger");
  stack--;

  return 0;
}

int AVR8::shift_right_uinteger(int const_val)
{
  int i;

  fprintf(out, "; shift_right_uinteger (optimized)\n");
  POP_HI("result1");
  POP_LO("result0");

  for(i = 0; i < const_val; i++)
  {
    fprintf(out, "  lsr result1\n");
    fprintf(out, "  ror result0\n");
  }

  PUSH_LO("result0");
  PUSH_HI("result1");

  return 0;
}

int AVR8::and_integer()
{
  need_and_integer = 1;
  CALL("and_integer");
  stack--;

  return 0;
}

int AVR8::and_integer(int const_val)
{
  fprintf(out, "; and_integer (optimized)\n");
  POP_HI("value11");
  POP_LO("value10");
  fprintf(out, "  andi value10, 0x%02x\n", const_val & 0xff);
  fprintf(out, "  andi value11, 0x%02x\n", const_val >> 8);
  PUSH_LO("value10");
  PUSH_HI("value11");

  return 0;
}

int AVR8::or_integer()
{
  need_or_integer = 1;
  CALL("or_integer");
  stack--;

  return 0;
}

int AVR8::or_integer(int const_val)
{
  fprintf(out, "; or_integer (optimized)\n");
  POP_HI("value11");
  POP_LO("value10");
  fprintf(out, "  ori value10, 0x%02x\n", const_val & 0xff);
  fprintf(out, "  ori value11, 0x%02x\n", const_val >> 8);
  PUSH_LO("value10");
  PUSH_HI("value11");

  return 0;
}

int AVR8::xor_integer()
{
  need_xor_integer = 1;
  CALL("xor_integer");
  stack--;

  return 0;
}

int AVR8::xor_integer(int const_val)
{
  fprintf(out, "xor_integer (optimized):\n");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "  ldi temp, %d\n", const_val & 0xff);
  fprintf(out, "  eor result0, temp\n");
  fprintf(out, "  ldi temp, %d\n", const_val >> 8);
  fprintf(out, "  eor result1, temp\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");

  return 0;
}

int AVR8::inc_integer(int index, int num)
{
  uint16_t value = num & 0xffff;

  if(num > 0 && num < 64)
  {
    fprintf(out, "; inc_integer (optimized, add)\n");
    fprintf(out, "  ldi XL, stack_lo - %d\n", LOCALS(index));
    fprintf(out, "  add XL, locals\n");
    fprintf(out, "  ld ZL, X\n");
    fprintf(out, "  ldi YL, stack_hi - %d\n", LOCALS(index));
    fprintf(out, "  add YL, locals\n");
    fprintf(out, "  ld ZH, Y\n");
    fprintf(out, "  adiw ZL, 0x%02x\n", num);
    fprintf(out, "  st X, ZL\n");
    fprintf(out, "  st Y, ZH\n");
  }
    else if(num > -64 && num < 0)
  {
    fprintf(out, "; inc_integer (optimized, sub)\n");
    fprintf(out, "  ldi XL, stack_lo - %d\n", LOCALS(index));
    fprintf(out, "  add XL, locals\n");
    fprintf(out, "  ld ZL, X\n");
    fprintf(out, "  ldi YL, stack_hi - %d\n", LOCALS(index));
    fprintf(out, "  add YL, locals\n");
    fprintf(out, "  ld ZH, Y\n");
    fprintf(out, "  sbiw ZL, 0x%02x\n", -num);
    fprintf(out, "  st X, ZL\n");
    fprintf(out, "  st Y, ZH\n");
  }
    else
  {
    need_inc_integer = 1;
    fprintf(out, "  ldi value10, 0x%02x\n", value & 0xff);
    fprintf(out, "  ldi value11, 0x%02x\n", value >> 8);
    fprintf(out, "  ldi temp, %d\n", LOCALS(index));
    CALL("inc_integer");
  }

  return 0;
}

int AVR8::integer_to_byte()
{
  need_integer_to_byte = 1;
  CALL("integer_to_byte");

  return 0;
}

int AVR8::integer_to_short()
{
  // integers are already 16-bit, do nothing
  return 0;
}

int AVR8::jump_cond(const char *label, int cond, int distance)
{
  bool reverse = false;

  char label_skip[32];
  char label_jump[32];

  if (stack > 0)
  {
    need_jump_cond = 1;
    sprintf(label_skip, "jump_cond_skip_%d", label_count++);
    sprintf(label_jump, "jump_cond_jump_%d", label_count++);
    CALL("jump_cond");

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
        fprintf(out, "  cp value10, zero\n");
        fprintf(out, "  brne %s\n", label_skip);
        fprintf(out, "  cp value11, zero\n");
        fprintf(out, "  brne %s\n", label_skip);
        JUMP(label);
        fprintf(out, "%s:\n", label_skip);
        break;
      case COND_NOT_EQUAL:
        fprintf(out, "  cp value10, zero\n");
        fprintf(out, "  brne %s\n", label_jump);
        fprintf(out, "  cp value11, zero\n");
        fprintf(out, "  breq %s\n", label_skip);
        fprintf(out, "%s:\n", label_jump);
        JUMP(label);
        fprintf(out, "%s:\n", label_skip);
        break;
      case COND_LESS:
        if(reverse == false)
        {
          fprintf(out, "  cp value10, zero\n");
          fprintf(out, "  cpc value11, zero\n");
          fprintf(out, "  brge %s\n", label_skip);
          JUMP(label);
          fprintf(out, "%s:\n", label_skip);
        }
          else
        {
          fprintf(out, "  cp zero, value10\n");
          fprintf(out, "  cpc zero, value11\n");
          fprintf(out, "  brge %s\n", label_skip);
          JUMP(label);
          fprintf(out, "%s:\n", label_skip);
        }
        break;
      case COND_GREATER_EQUAL:
        if(reverse == false)
        {
          fprintf(out, "  cp value10, zero\n");
          fprintf(out, "  cpc value11, zero\n");
          fprintf(out, "  brlt %s\n", label_skip);
          JUMP(label);
          fprintf(out, "%s:\n", label_skip);
        }
          else
        {
          fprintf(out, "  cp zero, value10\n");
          fprintf(out, "  cpc zero, value11\n");
          fprintf(out, "  brlt %s\n", label_skip);
          JUMP(label);
          fprintf(out, "%s:\n", label_skip);
        }
        break;
    }

    stack--;
  }

  return 0;
}

int AVR8::jump_cond_integer(const char *label, int cond, int distance)
{
  bool reverse = false;

  char label_skip[32];
  char label_jump[32];

  if (stack > 1)
  {
    need_jump_cond_integer = 1;
    sprintf(label_skip, "jump_cond_integer_skip_%d", label_count++);
    sprintf(label_jump, "jump_cond_integer_jump_%d", label_count++);
    CALL("jump_cond_integer");

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
        fprintf(out, "  cp value10, value20\n");
        fprintf(out, "  brne %s\n", label_skip);
        fprintf(out, "  cp value11, value21\n");
        fprintf(out, "  brne %s\n", label_skip);
        JUMP(label);
        fprintf(out, "%s:\n", label_skip);
        break;
      case COND_NOT_EQUAL:
        fprintf(out, "  cp value10, value20\n");
        fprintf(out, "  brne %s\n", label_jump);
        fprintf(out, "  cp value11, value21\n");
        fprintf(out, "  breq %s\n", label_skip);
        fprintf(out, "%s:\n", label_jump);
        JUMP(label);
        fprintf(out, "%s:\n", label_skip);
        break;
      case COND_LESS:
        if(reverse == false)
        {
          fprintf(out, "  cp value10, value20\n");
          fprintf(out, "  cpc value11, value21\n");
          fprintf(out, "  brge %s\n", label_skip);
          JUMP(label);
          fprintf(out, "%s:\n", label_skip);
        }
          else
        {
          fprintf(out, "  cp value20, value10\n");
          fprintf(out, "  cpc value21, value11\n");
          fprintf(out, "  brge %s\n", label_skip);
          JUMP(label);
          fprintf(out, "%s:\n", label_skip);
        }
        break;
      case COND_GREATER_EQUAL:
        if(reverse == false)
        {
          fprintf(out, "  cp value10, value20\n");
          fprintf(out, "  cpc value11, value21\n");
          fprintf(out, "  brlt %s\n", label_skip);
          JUMP(label);
          fprintf(out, "%s:\n", label_skip);
        }
          else
        {
          fprintf(out, "  cp value20, value10\n");
          fprintf(out, "  cpc value21, value11\n");
          fprintf(out, "  brlt %s\n", label_skip);
          JUMP(label);
          fprintf(out, "%s:\n", label_skip);
        }
        break;
    }

    stack -= 2;
  }

  return 0;
}

int AVR8::ternary(int cond, int value_true, int value_false)
{
  return -1;
}

int AVR8::ternary(int cond, int compare, int value_true, int value_false)
{
  return -1;
}

int AVR8::return_local(int index, int local_count)
{
  fprintf(out, "; return_local\n");
  fprintf(out, "  ldi YL, stack_lo - %d\n", LOCALS(index));
  fprintf(out, "  add YL, locals\n");
  fprintf(out, "  ld result0, Y\n");
  fprintf(out, "  ldi YL, stack_hi - %d\n", LOCALS(index));
  fprintf(out, "  add YL, locals\n");
  fprintf(out, "  ld result1, Y\n");
  fprintf(out, "  mov SP, locals\n");

  if (!is_main)
  {
    POP_HI("locals");
    POP_LO("locals");
  }

  fprintf(out, "  ret\n\n");

  return 0;
}

int AVR8::return_integer(int local_count)
{
  fprintf(out, "; return_integer\n");
  POP_HI("result1");
  POP_LO("result0");

  fprintf(out, "  mov SP, locals\n");

  if (!is_main)
  {
    POP_HI("locals");
    POP_LO("locals");
    stack--;
  }

  fprintf(out, "  ret\n\n");

  return 0;
}

int AVR8::return_void(int local_count)
{
  fprintf(out, "; return_void\n");
  fprintf(out, "  mov SP, locals\n");

  if (!is_main)
  {
    POP_HI("locals");
    POP_LO("locals");
    fprintf(out, "  ret\n\n");
  }
    else
  {
    // stop the cpu when program ends
    fprintf(out, "  break\n");
  }

  return 0;
}

int AVR8::jump(const char *name, int distance)
{
  JUMP(name);

  return 0;
}

int AVR8::call(const char *name)
{
  CALL(name);

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
      fprintf(out, "  ldi YL, stack_lo + 1 + %d\n", (stack - stack_vars));
      fprintf(out, "  add YL, SP\n");
      fprintf(out, "  ld temp, Y\n");
      fprintf(out, "  ldi YL, stack_lo + 1 %d\n", local - 1);
      fprintf(out, "  add YL, SP\n");
      fprintf(out, "  st Y, temp\n");
      fprintf(out, "  ldi YL, stack_hi + 1 + %d\n", (stack - stack_vars));
      fprintf(out, "  add YL, SP\n");
      fprintf(out, "  ld temp, Y\n");
      fprintf(out, "  ldi YL, stack_hi + 1 %d\n", local - 1);
      fprintf(out, "  add YL, SP\n");
      fprintf(out, "  st Y, temp\n");

      stack_vars--;
    }

    local++;
  }

  CALL(name);

  if ((stack - stack_vars) > 0)
  {
    fprintf(out, "  ldi temp, %d\n", (stack - stack_vars));
    fprintf(out, "  add SP, temp\n");

    params -= (stack - stack_vars);
  }

  if (!is_void)
  {
    PUSH_LO("result0");
    PUSH_HI("result1");
    stack++;
  }

  return 0;
}

int AVR8::put_static(const char *name, int index)
{
  if (stack > 0)
  {
    fprintf(out, "; put_static\n");
    POP_HI("temp");
    fprintf(out, "  sts %s + 1, temp\n", name);
    POP_LO("temp");
    fprintf(out, "  sts %s + 0, temp\n", name);
    stack--;
  }

  return 0;
}

int AVR8::get_static(const char *name, int index)
{
  fprintf(out, "; get_static\n");
  fprintf(out, "  lds temp, %s + 0\n", name);
  PUSH_LO("temp");
  fprintf(out, "  lds temp, %s + 1\n", name);
  PUSH_HI("temp");
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
      CALL("new_array_int");
    }
      else
    {
      need_array_byte_support = 1;
      CALL("new_array_byte");
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
  fprintf(out, ".align 16\n");
  fprintf(out, "dw %d\n", len);

  return insert_utf8(name, bytes, len);
}

int AVR8::push_array_length()
{
  if (stack > 0)
  {
    need_push_array_length = 1;
    CALL("push_array_length");
  }

  return 0;
}

int AVR8::push_array_length(const char *name, int field_id)
{
  need_push_array_length2 = 1;
  fprintf(out, "  lds ZL, %s + 0\n", name);
  fprintf(out, "  lds ZH, %s + 1\n", name);
  CALL("push_array_length2");
  stack++;

  return 0;
}

int AVR8::array_read_byte()
{
  need_array_byte_support = 1;

  get_values_from_stack(2);
  CALL("array_read_byte");
  stack++;

  return 0;
}

int AVR8::array_read_short()
{
  return array_read_int();
}

int AVR8::array_read_int()
{
  need_array_int_support = 1;

  get_values_from_stack(2);
  CALL("array_read_int");
  stack++;

  return 0;
}

int AVR8::array_read_byte(const char *name, int field_id)
{
  need_array_byte_support = 1;

  if (stack > 0)
  {
    fprintf(out, "  lds ZL, %s + 0\n", name);
    fprintf(out, "  lds ZH, %s + 1\n", name);
    CALL("array_read_byte2");
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
    fprintf(out, "  lds ZL, %s + 0\n", name);
    fprintf(out, "  lds ZH, %s + 1\n", name);
    CALL("array_read_int2");
  }

  return 0;
}

int AVR8::array_write_byte()
{
  need_array_byte_support = 1;

  get_values_from_stack(3);
  CALL("array_write_byte");

  return 0;
}

int AVR8::array_write_short()
{
  return array_write_int();
}

int AVR8::array_write_int()
{
  need_array_int_support = 1;

  get_values_from_stack(3);
  CALL("array_write_int");

  return 0;
}

int AVR8::array_write_byte(const char *name, int field_id)
{
//  get_values_from_stack(2);
//  fprintf(out, "; array_write_byte2:\n");
  printf("writing to static byte array unimplemented\n");

  return -1;
}

int AVR8::array_write_short(const char *name, int field_id)
{
  return array_write_int(name, field_id);
}

int AVR8::array_write_int(const char *name, int field_id)
{
//  get_values_from_stack(2);
//  fprintf(out, "; array_write_int2:\n");
  printf("writing to static int array unimplemented\n");

  return -1;
}

int AVR8::get_values_from_stack(int num)
{
  need_get_values_from_stack = 1;

  if(num > 0)
  {
    CALL("get_values_from_stack_1");
    stack--;
  }

  if(num > 1)
  {
    CALL("get_values_from_stack_2");
    stack--;
  }

  if(num > 2)
  {
    CALL("get_values_from_stack_3");
    stack--;
  }

  return 0;
}

// subroutines
void AVR8::insert_swap()
{
  fprintf(out, "swap:\n");
  fprintf(out, "  ldi XL, stack_lo\n");
  fprintf(out, "  add XL, SP\n");
  fprintf(out, "  ld value10, X-\n");
  fprintf(out, "  ld value20, X\n");
  fprintf(out, "  ldi YL, stack_hi\n");
  fprintf(out, "  add YL, SP\n");
  fprintf(out, "  ld value11, Y-\n");
  fprintf(out, "  ld value21, Y\n");
  fprintf(out, "  movw value30, value20\n");
  fprintf(out, "  movw value20, value10\n");
  fprintf(out, "  movw value10, value30\n");
  fprintf(out, "  st Y+, value21\n");
  fprintf(out, "  st Y, value11\n");
  fprintf(out, "  st X+, value10\n");
  fprintf(out, "  st X, value20\n");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_add_integer()
{
  fprintf(out, "add_integer:\n");
  POP_HI("value11");
  POP_LO("value10");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "  add result0, value10\n");
  fprintf(out, "  adc result1, value11\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_sub_integer()
{
  fprintf(out, "sub_integer:\n");
  POP_HI("value11");
  POP_LO("value10");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "  sub result0, value10\n");
  fprintf(out, "  sbc result1, value11\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_mul_integer()
{
  fprintf(out, "mul_integer:\n");
  POP_HI("value21");
  POP_LO("value20");
  POP_HI("value11");
  POP_LO("value10");
  fprintf(out, "  mov result0, zero\n");
  fprintf(out, "  mov result1, zero\n");
  fprintf(out, "  ldi temp, 16\n");
  fprintf(out, "mul_integer_loop:\n");
  fprintf(out, "  lsl result0\n");
  fprintf(out, "  rol result1\n");
  fprintf(out, "  lsl value10\n");
  fprintf(out, "  rol value11\n");
  fprintf(out, "  brcc mul_integer_next\n");
  fprintf(out, "  add result0, value20\n");
  fprintf(out, "  adc result1, value21\n");
  fprintf(out, "mul_integer_next:\n");
  fprintf(out, "  dec temp\n");
  fprintf(out, "  brne mul_integer_loop\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_div_integer()
{
  fprintf(out, "div_integer:\n");
  POP_HI("value21");
  POP_LO("value20");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "  mov remainder0, zero\n");
  fprintf(out, "  mov remainder1, zero\n");
  fprintf(out, "  ldi temp, 16\n");
  fprintf(out, "div_integer_loop:\n");
  fprintf(out, "  lsl result0\n");
  fprintf(out, "  rol result1\n");
  fprintf(out, "  rol remainder0\n");
  fprintf(out, "  rol remainder1\n");
  fprintf(out, "  cp remainder0, value20\n");
  fprintf(out, "  cpc remainder1, value21\n");
  fprintf(out, "  brcs div_integer_next\n");
  fprintf(out, "  sub remainder0, value20\n");
  fprintf(out, "  sbc remainder1, value21\n");
  fprintf(out, "  inc result0\n");
  fprintf(out, "div_integer_next:\n");
  fprintf(out, "  dec temp\n");
  fprintf(out, "  brne div_integer_loop\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_mod_integer()
{
  fprintf(out, "mod_integer:\n");
  POP_HI("temp");
  POP_LO("temp");
  fprintf(out, "  mov result0, remainder0\n");
  PUSH_LO("result0");
  fprintf(out, "  mov result1, remainder1\n");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_neg_integer()
{
  fprintf(out, "neg_integer:\n");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "  ldi temp, 0\n");
  fprintf(out, "  sub temp, result0\n");
  fprintf(out, "  sbc temp, result1\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_shift_left_integer()
{
  fprintf(out, "shift_left_integer:\n");
  POP_HI("temp");
  POP_LO("temp");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "shift_left_integer_loop:\n");
  fprintf(out, "  lsl result0\n");
  fprintf(out, "  rol result1\n");
  fprintf(out, "  dec temp\n");
  fprintf(out, "  brne shift_left_integer_loop\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_shift_right_integer()
{
  fprintf(out, "shift_right_integer:\n");
  POP_HI("temp");
  POP_LO("temp");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "shift_right_integer_loop:\n");
  fprintf(out, "  asr result1\n");
  fprintf(out, "  ror result0\n");
  fprintf(out, "  dec temp\n");
  fprintf(out, "  brne shift_right_integer_loop\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_shift_right_uinteger()
{
  fprintf(out, "shift_right_uinteger:\n");
  POP_HI("temp");
  POP_LO("temp");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "shift_right_uinteger_loop:\n");
  fprintf(out, "  lsr result1\n");
  fprintf(out, "  ror result0\n");
  fprintf(out, "  dec temp\n");
  fprintf(out, "  brne shift_right_uinteger_loop\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_and_integer()
{
  fprintf(out, "and_integer:\n");
  POP_HI("value11");
  POP_LO("value10");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "  and result0, value10\n");
  fprintf(out, "  and result1, value11\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_or_integer()
{
  fprintf(out, "or_integer:\n");
  POP_HI("value11");
  POP_LO("value10");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "  or result0, value10\n");
  fprintf(out, "  or result1, value11\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_xor_integer()
{
  fprintf(out, "xor_integer:\n");
  POP_HI("value11");
  POP_LO("value10");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "  eor result0, value10\n");
  fprintf(out, "  eor result1, value11\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_inc_integer()
{
  fprintf(out, "inc_integer:\n");
  fprintf(out, "  ldi XL, stack_lo\n");
  fprintf(out, "  sub XL, temp\n");
  fprintf(out, "  add XL, locals\n");
  fprintf(out, "  ld value20, X\n");
  fprintf(out, "  ldi YL, stack_hi\n");
  fprintf(out, "  sub YL, temp\n");
  fprintf(out, "  add YL, locals\n");
  fprintf(out, "  ld value21, Y\n");
  fprintf(out, "  add value20, value10\n");
  fprintf(out, "  adc value21, value11\n");
  fprintf(out, "  st X, value20\n");
  fprintf(out, "  st Y, value21\n");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_integer_to_byte()
{
  fprintf(out, "integer_to_byte:\n");
  POP_HI("result1");
  POP_LO("result0");
  PUSH_LO("result0");
  fprintf(out, "  lsl result0\n");
  fprintf(out, "  mov result1, zero\n");
  fprintf(out, "  adc result1, ff\n");
  fprintf(out, "  eor result1, ff\n");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_jump_cond()
{
  fprintf(out, "jump_cond:\n");
  fprintf(out, "  inc SP\n");

  // value10
  fprintf(out, "  ldi YL, stack_lo\n");
  fprintf(out, "  add YL, SP\n");
  fprintf(out, "  ld value10, Y\n");
  // value11
  fprintf(out, "  ldi YL, stack_hi\n");
  fprintf(out, "  add YL, SP\n");
  fprintf(out, "  ld value11, Y\n");

  fprintf(out, "  ret\n\n");
}

void AVR8::insert_jump_cond_integer()
{
  fprintf(out, "jump_cond_integer:\n");
  fprintf(out, "  inc SP\n");
  fprintf(out, "  inc SP\n");

  // value10
  fprintf(out, "  ldi YL, stack_lo\n");
  fprintf(out, "  add YL, SP\n");
  fprintf(out, "  ld value10, Y\n");
  // value20
  fprintf(out, "  dec YL\n");
  fprintf(out, "  ld value20, Y\n");
  // value11
  fprintf(out, "  ldi YL, stack_hi\n");
  fprintf(out, "  add YL, SP\n");
  fprintf(out, "  ld value11, Y\n");
  // value21
  fprintf(out, "  dec YL\n");
  fprintf(out, "  ld value21, Y\n");

  fprintf(out, "  ret\n\n");
}

void AVR8::insert_push_local_var_int()
{
  fprintf(out, "push_local_var_int:\n");
  fprintf(out, "  ldi YL, stack_lo\n");
  fprintf(out, "  sub YL, temp2\n");
  fprintf(out, "  add YL, locals\n");
  fprintf(out, "  ld temp, Y\n");
  PUSH_LO("temp");
  fprintf(out, "  ldi YL, stack_hi\n");
  fprintf(out, "  sub YL, temp2\n");
  fprintf(out, "  add YL, locals\n");
  fprintf(out, "  ld temp, Y\n");
  PUSH_HI("temp");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_pop_local_var_int()
{
  fprintf(out, "pop_local_var_int:\n");
  POP_HI("temp");
  fprintf(out, "  ldi YL, stack_hi\n");
  fprintf(out, "  sub YL, temp2\n");
  fprintf(out, "  add YL, locals\n");
  fprintf(out, "  st Y, temp\n");
  POP_LO("temp");
  fprintf(out, "  ldi YL, stack_lo\n");
  fprintf(out, "  sub YL, temp2\n");
  fprintf(out, "  add YL, locals\n");
  fprintf(out, "  st Y, temp\n");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_dup()
{
  fprintf(out, "dup:\n");
  fprintf(out, "  mov YL, stack_lo\n");
  fprintf(out, "  add YL, SP\n");
  fprintf(out, "  ld temp, YL\n");
  PUSH_LO("temp");
  fprintf(out, "  mov YL, stack_hi\n");
  fprintf(out, "  add YL, SP\n");
  fprintf(out, "  ld temp, YL\n");
  PUSH_HI("temp");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_push_array_length()
{
  // push_array_length
  fprintf(out, "push_array_length:\n");
  POP_HI("XH");
  POP_LO("XL");
  fprintf(out, "  sub XL, two\n");
  fprintf(out, "  sbc XH, zero\n");
  fprintf(out, "  ld result0, X+\n");
  fprintf(out, "  ld result1, X\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_push_array_length2()
{
  fprintf(out, "push_array_length2:\n");
  fprintf(out, "  sub ZL, two\n");
  fprintf(out, "  sbc ZH, zero\n");
  fprintf(out, "  lpm result0, Z+\n");
  fprintf(out, "  lpm result1, Z\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_array_byte_support()
{
  // new_array byte
  fprintf(out, "new_array_byte:\n");
  POP_HI("length1");
  POP_LO("length0");
  fprintf(out, "  lds result0, heap_ptr + 0\n");
  fprintf(out, "  lds result1, heap_ptr + 1\n");
  fprintf(out, "  mov XL, result0\n");
  fprintf(out, "  mov XH, result1\n");
  fprintf(out, "  st X+, length0\n");
  fprintf(out, "  st X, length1\n");
  fprintf(out, "  add length0, two\n");
  fprintf(out, "  adc length1, zero\n");
  fprintf(out, "  lds temp, heap_ptr + 0\n");
  fprintf(out, "  lds temp2, heap_ptr + 1\n");
  fprintf(out, "  add temp, length0\n");
  fprintf(out, "  adc temp2, length1\n");
  fprintf(out, "  sts heap_ptr + 0, temp\n");
  fprintf(out, "  sts heap_ptr + 1, temp2\n");
  fprintf(out, "  add result0, three\n");
  fprintf(out, "  adc result1, zero\n");
  fprintf(out, "  ldi temp, 254\n");
  fprintf(out, "  and result0, temp\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");

  // array_read_byte
  fprintf(out, "array_read_byte:\n");
  fprintf(out, "  add value20, value10\n");
  fprintf(out, "  adc value21, value11\n");
  fprintf(out, "  mov XL, value20\n");
  fprintf(out, "  mov XH, value21\n");
  fprintf(out, "  ld result0, X\n");
  PUSH_LO("result0");
  fprintf(out, "  lsl\n");
  fprintf(out, "  mov result1, zero\n");
  fprintf(out, "  adc result1, ff\n");
  fprintf(out, "  eor result1, ff\n");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");

  // array_read_byte2
  fprintf(out, "array_read_byte2:\n");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "  add ZL, result0\n");
  fprintf(out, "  adc ZH, result1\n");
  fprintf(out, "  lpm result0, Z\n");
  PUSH_LO("result0");
  fprintf(out, "  lsl\n");
  fprintf(out, "  mov result1, zero\n");
  fprintf(out, "  adc result1, ff\n");
  fprintf(out, "  eor result1, ff\n");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");

  // array_write_byte
  fprintf(out, "array_write_byte:\n");
  fprintf(out, "  add value30, value20\n");
  fprintf(out, "  adc value31, value21\n");
  fprintf(out, "  mov XL, value30\n");
  fprintf(out, "  mov XH, value31\n");
  fprintf(out, "  st X, value10\n");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_array_int_support()
{
  // new_array int
  fprintf(out, "new_array_int:\n");
  POP_HI("length1");
  POP_LO("length0");
  fprintf(out, "  lds result0, heap_ptr + 0\n");
  fprintf(out, "  lds result1, heap_ptr + 1\n");
  fprintf(out, "  mov XL, result0\n");
  fprintf(out, "  mov XH, result1\n");
  fprintf(out, "  st X+, length0\n");
  fprintf(out, "  st X, length1\n");
  fprintf(out, "  lsl length0\n");
  fprintf(out, "  rol length1\n");
  fprintf(out, "  add length0, two\n");
  fprintf(out, "  adc length1, zero\n");
  fprintf(out, "  lds temp, heap_ptr + 0\n");
  fprintf(out, "  lds temp2, heap_ptr + 1\n");
  fprintf(out, "  add temp, length0\n");
  fprintf(out, "  adc temp2, length1\n");
  fprintf(out, "  sts heap_ptr + 0, temp\n");
  fprintf(out, "  sts heap_ptr + 1, temp2\n");
  fprintf(out, "  add result0, three\n");
  fprintf(out, "  adc result1, zero\n");
  fprintf(out, "  ldi temp, 254\n");
  fprintf(out, "  and result0, temp\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");

  // array_read_int
  fprintf(out, "array_read_int:\n");
  fprintf(out, "  lsl value10\n");
  fprintf(out, "  rol value11\n");
  fprintf(out, "  add value20, value10\n");
  fprintf(out, "  adc value21, value11\n");
  fprintf(out, "  mov XL, value20\n");
  fprintf(out, "  mov XH, value21\n");
  fprintf(out, "  ld result0, X+\n");
  fprintf(out, "  ld result1, X\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");

  // array_read_int2
  fprintf(out, "array_read_int2:\n");
  POP_HI("result1");
  POP_LO("result0");
  fprintf(out, "  lsl result0\n");
  fprintf(out, "  rol result1\n");
  fprintf(out, "  add ZL, result0\n");
  fprintf(out, "  adc ZH, result1\n");
  fprintf(out, "  lpm result0, Z+\n");
  fprintf(out, "  lpm result1, Z\n");
  PUSH_LO("result0");
  PUSH_HI("result1");
  fprintf(out, "  ret\n\n");

  // array_write_int
  fprintf(out, "array_write_int:\n");
  fprintf(out, "  lsl value20\n");
  fprintf(out, "  rol value21\n");
  fprintf(out, "  add value30, value20\n");
  fprintf(out, "  adc value31, value21\n");
  fprintf(out, "  mov XL, value30\n");
  fprintf(out, "  mov XH, value31\n");
  fprintf(out, "  st X+, value10\n");
  fprintf(out, "  st X, value11\n");
  fprintf(out, "  ret\n\n");
}

void AVR8::insert_get_values_from_stack()
{
  fprintf(out, "get_values_from_stack_1:\n");
  POP_HI("value11");
  POP_LO("value10");
  fprintf(out, "  ret\n\n");
  fprintf(out, "get_values_from_stack_2:\n");
  POP_HI("value21");
  POP_LO("value20");
  fprintf(out, "  ret\n\n");
  fprintf(out, "get_values_from_stack_3:\n");
  POP_HI("value31");
  POP_LO("value30");
  fprintf(out, "  ret\n\n");
}

// Memory API
int AVR8::memory_read8_I()
{
  fprintf(out, "; memory_read8_I\n");
  POP_HI("XH");
  POP_LO("XL");
  fprintf(out, "  ld result0, X\n");
  PUSH_LO("result0");
  fprintf(out, "  lsl result0\n");
  fprintf(out, "  mov result1, zero\n");
  fprintf(out, "  adc result1, ff\n");
  fprintf(out, "  eor result1, ff\n");
  PUSH_HI("result1");

  return 0;
}

int AVR8::memory_write8_IB()
{
  fprintf(out, "; memory_write8_IB\n");
  POP_HI("temp");
  POP_LO("temp");
  POP_HI("XH");
  POP_LO("XL");
  fprintf(out, "  st X, temp\n");

  stack -= 2;

  return 0;
}

int AVR8::memory_read16_I()
{
  fprintf(out, "; memory_read16_I\n");
  POP_HI("XH");
  POP_LO("XL");
  fprintf(out, "  ld result0, X+\n");
  PUSH_LO("result0");
  fprintf(out, "  ld result1, X\n");
  PUSH_HI("result1");

  return 0;
}

int AVR8::memory_write16_IS()
{
  fprintf(out, "; memory_write16_IS\n");
  POP_HI("value11");
  POP_LO("value10");
  POP_HI("XH");
  POP_LO("XL");
  fprintf(out, "  st X+, value10\n");
  fprintf(out, "  st X, value11\n");

  stack -= 2;

  return 0;
}

// IOPort API
int AVR8::ioport_setPinsAsInput_I(int port)
{
  if(port < 0 || port > 3)
    return -1;

  fprintf(out, "; ioport_setpinsasinput\n");
  POP_HI("temp");
  POP_LO("temp");
  fprintf(out, "  eor temp, ff\n");
  fprintf(out, "  in temp2, %s\n", ddr_string[port]);
  fprintf(out, "  and temp2, temp\n");
  fprintf(out, "  out %s, temp2\n", ddr_string[port]);
  stack--;

  return 0;
}

int AVR8::ioport_setPinsAsInput_I(int port, int const_val)
{
  if(port < 0 || port > 3)
    return -1;

  fprintf(out, "; ioport_setPinsAsInput (optimized)\n");
  fprintf(out, "  ldi temp, 0x%02x\n", const_val & 0xff);
  fprintf(out, "  eor temp, ff\n");
  fprintf(out, "  in temp2, %s\n", ddr_string[port]);
  fprintf(out, "  and temp2, temp\n");
  fprintf(out, "  out %s, temp2\n", ddr_string[port]);

  return 0;
}

int AVR8::ioport_setPinsAsOutput_I(int port)
{
  if(port < 0 || port > 3)
    return -1;

  fprintf(out, "; ioport_setPinsAsOutput\n");
  POP_HI("temp");
  POP_LO("temp");
  fprintf(out, "  in temp2, %s\n", ddr_string[port]);
  fprintf(out, "  or temp2, temp\n");
  fprintf(out, "  out %s, temp2\n", ddr_string[port]);
  stack--;

  return 0;
}

int AVR8::ioport_setPinsAsOutput_I(int port, int const_val)
{
  if(port < 0 || port > 3)
    return -1;

  fprintf(out, "; ioport_setPinsAsOutput (optimized)\n");
  fprintf(out, "  ldi temp, 0x%02x\n", const_val & 0xff);
  fprintf(out, "  in temp2, %s\n", ddr_string[port]);
  fprintf(out, "  or temp2, temp\n");
  fprintf(out, "  out %s, temp2\n", ddr_string[port]);

  return 0;
}

int AVR8::ioport_setPinsValue_I(int port)
{
  if(port < 0 || port > 3)
    return -1;

  fprintf(out, "; ioport_setPinsValue\n");
  POP_HI("temp");
  POP_LO("temp");
  fprintf(out, "  out %s, temp\n", port_string[port]);
  stack--;

  return 0;
}

int AVR8::ioport_setPinsValue_I(int port, int const_val)
{
  if(port < 0 || port > 3)
    return -1;

  fprintf(out, "; ioport_setPinsValue (optimized)\n");
  fprintf(out, "  ldi temp, 0x%02x\n", const_val & 0xff);
  fprintf(out, "  out %s, temp\n", port_string[port]);

  return 0;
}

int AVR8::ioport_setPinsHigh_I(int port)
{
  if(port < 0 || port > 3)
    return -1;

  fprintf(out, "; ioport_setPinsHigh\n");
  POP_HI("temp");
  POP_LO("temp");
  fprintf(out, "  in temp2, %s\n", port_string[port]);
  fprintf(out, "  or temp2, temp\n");
  fprintf(out, "  out %s, temp2\n", port_string[port]);
  stack--;

  return 0;
}

int AVR8::ioport_setPinsLow_I(int port)
{
  if(port < 0 || port > 3)
    return -1;

  fprintf(out, "; ioport_setPinsLow\n");
  POP_HI("temp");
  POP_LO("temp");
  fprintf(out, "  eor temp, ff\n");
  fprintf(out, "  in temp2, %s\n", port_string[port]);
  fprintf(out, "  and temp2, temp\n");
  fprintf(out, "  out %s, temp2\n", port_string[port]);
  stack--;

  return 0;
}

int AVR8::ioport_setPinAsOutput_I(int port)
{
  return -1;
}

int AVR8::ioport_setPinAsInput_I(int port)
{
  return -1;
}

int AVR8::ioport_setPinHigh_I(int port)
{
  return -1;
}

int AVR8::ioport_setPinHigh_I(int port, int const_val)
{
  if(port < 0 || port > 3)
    return -1;

  if(const_val < 0 || const_val > 7) { return -1; }

  fprintf(out, "; ioport_setPinHigh (optimized)\n");
  fprintf(out, "  sbi %s, %d\n", port_string[port], const_val);

  return 0;
}

int AVR8::ioport_setPinLow_I(int port)
{
  return -1;
}

int AVR8::ioport_setPinLow_I(int port, int const_val)
{
  if(port < 0 || port > 3)
    return -1;

  if(const_val < 0 || const_val > 7) { return -1; }

  fprintf(out, "; ioport_setPinLow (optimized)\n");
  fprintf(out, "  cbi %s, %d\n", port_string[port], const_val);

  return 0;
}

int AVR8::ioport_isPinInputHigh_I(int port)
{
  // Uncomment pin_string at the top of the file for this function.
  return -1;
}

int AVR8::ioport_getPortInputValue(int port)
{
  return -1;
}

// ADC API
int AVR8::adc_enable()
{
  fprintf(out, "; adc_enable\n");
  fprintf(out, "  ldi temp, (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)\n");
  fprintf(out, "  %s ADCSRA, temp\n", adc_out_string);

  fprintf(out, "  %s temp, ADMUX\n", adc_in_string);
  fprintf(out, "  ori temp, (1 << REFS0)\n");
  fprintf(out, "  %s ADMUX, temp\n", adc_out_string);

  fprintf(out, "  %s temp, ADCSRA\n", adc_in_string);
  fprintf(out, "  ori temp, (1 << ADEN) | (1 << ADSC)\n");
  fprintf(out, "  %s ADCSRA, temp\n", adc_out_string);

  return 0;
} 

int AVR8::adc_disable()
{
  fprintf(out, "; adc_disable\n");
  fprintf(out, "  %s ADCSRA, zero\n", adc_out_string);

  return 0;
} 

int AVR8::adc_setChannel_I()
{
  return -1;
} 

int AVR8::adc_setChannel_I(int channel)
{
  if(channel < 0 || channel > 7)
    return -1;

  fprintf(out, "; adc_setChannel_I\n");
  fprintf(out, "  %s temp, ADMUX\n", adc_in_string);
  fprintf(out, "  andi temp, 248\n");
  fprintf(out, "  ori temp, %d\n", channel);
  fprintf(out, "  %s ADMUX, temp\n", adc_out_string);
  PUSH_LO("zero");
  PUSH_HI("zero");
  stack++;

  return 0;
} 

int AVR8::adc_read()
{
  fprintf(out, "; adc_read\n");

  fprintf(out, "  %s temp, ADCSRA\n", adc_in_string);
  fprintf(out, "  ori temp, (1 << ADSC)\n");
  fprintf(out, "  %s ADCSRA, temp\n", adc_out_string);

  fprintf(out, "adc_read_%d:\n", label_count);

  if(need_memory_mapped_adc)
  {
    fprintf(out, "  lds temp, ADCSRA\n");
    fprintf(out, "  sbrc temp, ADSC\n");
  }
  else
  {
    fprintf(out, "  sbic ADCSRA, ADSC\n");
  }

  fprintf(out, "  rjmp adc_read_%d\n", label_count++);

  fprintf(out, "  %s result0, ADCL\n", adc_in_string);
  PUSH_LO("result0");
  fprintf(out, "  %s result1, ADCH\n", adc_in_string);
  PUSH_HI("result1");
  stack++;
  
  return 0;
}

// UART functions
int AVR8::uart_init_I(int port)
{
  return -1;
}

int AVR8::uart_init_I(int port, int baud_rate)
{
  // UBRR = (fosc / (16 * BAUD) - 1)
  int rate_table[] = { 832,   415,  103,    51,    25,    16 };
  int baud_table[] = { 1200, 2400, 9600, 19200, 38400, 57600 };

  if (baud_rate < 0 || baud_rate > (int)(sizeof(rate_table) / sizeof(int)))
  {
    printf("Error: Illegal baud constant\n");
    return -1;
  }

  fprintf(out, "  ;; Set up UART baud rate\n");
  fprintf(out, "  ldi temp, %d\n", rate_table[baud_rate] >> 8);
  fprintf(out, "  sts UBRR0H, temp\n");
  fprintf(out, "  ldi temp, %d\n", rate_table[baud_rate] & 0xff);
  fprintf(out, "  sts UBRR0L, temp  ; %d @ 16MHz = %d baud\n\n",
     rate_table[baud_rate], baud_table[baud_rate]);

  fprintf(out, "  ;; Set up UART options\n");
  fprintf(out, "  ldi temp, (1<<UCSZ00)|(1<<UCSZ01)    ; sets up data as 8N1\n");
  fprintf(out, "  sts UCSR0C, temp\n");
  fprintf(out, "  ldi temp, (1<<TXEN0)|(1<<RXEN0)      ; enables send/receive\n");
  fprintf(out, "  sts UCSR0B, temp\n");
  fprintf(out, "  eor temp, temp\n");
  fprintf(out, "  sts UCSR0A, temp\n");
  //fprintf(out, "  ldi temp, (1<<U2X0)\n");
  //fprintf(out, "  sts UCSR0A, temp\n");

  return 0;
}

int AVR8::uart_send_I(int port)
{
  POP_HI("temp");
  POP_LO("temp");
  fprintf(out, "  sts UDR0, temp ; output a char over UART\n");

  return 0;
}

int AVR8::uart_read(int port)
{
  //fprintf(out, "  lds r17, UDR0 ; read char from uart\n");

  fprintf(out, "  lds result0, UDR0 ; read char from UART\n");
  PUSH_LO("result0");
  PUSH_HI("zero");
  stack++;

  return 0;
}

int AVR8::uart_isDataAvailable(int port)
{
  fprintf(out, "  mov result0, zero\n");
  fprintf(out, "  lds temp, UCSR0A  ; poll uart to see if there is a data waiting\n");
  fprintf(out, "  sbrc temp, RXC0\n");
  fprintf(out, "  mov result0, one\n");
  PUSH_LO("result0");
  PUSH_HI("zero");
  stack++;

  return 0;
}

int AVR8::uart_isSendReady(int port)
{
  fprintf(out, "  mov result0, zero\n");
  fprintf(out, "  lds temp, UCSR0A     ; check to see if it's okay to send a char\n");
  fprintf(out, "  sbrc temp, UDRE0\n");
  fprintf(out, "  mov result0, one\n");
  PUSH_LO("result0");
  PUSH_HI("zero");
  stack++;

  return 0;
}

