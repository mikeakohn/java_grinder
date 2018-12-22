/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "MSP430.h"
#include "MSP430X.h"

// ABI is:
// r4 top of stack
// r5
// r6
// r7
// r8
// r9
// r10
// r11 top of stack
// r12 points to locals
// r13 points to arrays
// r14 is temp
// r15 is temp

// Function calls:
// Push params left to right:  add_nums(a,b) =
//
// [   ] <-- SP
// [ret]
//  ...   (push used registers :( )
// [ b ]
// [ a ]
//
// ret value is r15

// RAM organization:
// RAM[0] = top of heap ptr
// RAM[1] = static field 1
// RAM[2] = static field 2
// RAM[N] = static field N

#define REG_STACK(a) (a + 4)
#define LOCALS(a) ((a * 2) + 2)

//                                EQ    NE     LESS  LESS EQ GR   GR E
static const char *cond_str[] = { "jz", "jnz", "jl", "jle", "jg", "jge" };
//                                                    rev    rev

MSP430::MSP430(uint8_t chip_type) :
  reg(0),
  reg_max(6),
  stack(0),
  label_count(0),
  need_read_spi(0),
  need_mul_integers(0),
  need_div_integers(0),
  need_timer_interrupt(0),
  is_main(0),
  is_interrupt(0)
{
  ram_start = 0x0200;
  vector_timer = 0xfff2;
  include_file = "msp430x2xx.inc";

  switch(chip_type)
  {
    case MSP430G2231:
      flash_start = 0xf800;
      stack_start = 0x0280;
      break;
    case MSP430G2452:
      flash_start = 0xe000;
      stack_start = 0x0300;
      break;
    case MSP430G2553:
      flash_start = 0xc000;
      stack_start = 0x0400;
      break;
    default:
      flash_start = 0xf800;
      stack_start = 0x0280;
  }
}

MSP430::~MSP430()
{
  if (need_read_spi) { insert_read_spi(); }
  if (need_mul_integers) { insert_mul_integers(); }
  if (need_div_integers) { insert_div_integers(); }

  if (need_timer_interrupt)
  {
    fprintf(out, ".org 0x%04x\n", vector_timer);
    fprintf(out, "  dw timerInterrupt\n");
  }

  fprintf(out, ".org 0xfffe\n");
  fprintf(out, "  dw start\n\n");
}

int MSP430::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  MSP430X *msp430x = dynamic_cast<MSP430X *>(this);

  // For now we only support a specific chip
  fprintf(out, ".%s\n", msp430x == NULL ? "msp430" : "msp430x");
  fprintf(out, ".include \"%s\"\n\n", include_file);

  // Set where RAM starts
  fprintf(out, "ram_start equ 0x%04x\n", ram_start);
  fprintf(out, "heap_ptr equ ram_start\n");

  return 0;
}

int MSP430::start_init()
{
  // Add any set up items (stack, registers, etc)
  fprintf(out, "\n");
  fprintf(out, ".org 0x%04x\n", flash_start);
  fprintf(out, "start:\n");
  fprintf(out, "  mov.w #(WDTPW|WDTHOLD), &WDTCTL\n");
  fprintf(out, "  dint\n");  // Do we need this?
  fprintf(out, "  mov.w #0x%04x, SP\n\n", stack_start);
  //fprintf(out, "  jmp main\n\n");

  return 0;
}

int MSP430::insert_static_field_define(std::string &name, std::string &type, int index)
{
  fprintf(out, "%s equ ram_start+%d\n", name.c_str(), (index + 1) * 2);
  return 0;
}

int MSP430::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");
  fprintf(out, "  mov.w #ram_start+%d, &ram_start\n", (field_count + 1) * 2);
  return 0;
}

int MSP430::field_init_int(std::string &name, int index, int value)
{
  if (value < -32768 || value > 65535) { return -1; }
  fprintf(out, "  mov.w #%d, &%s\n", value, name.c_str());
  return 0;
}

int MSP430::field_init_ref(std::string &name, int index)
{
  fprintf(out, "  mov.w #_%s, &%s\n", name.c_str(), name.c_str());
  return 0;
}

void MSP430::method_start(int local_count, int max_stack, int param_count, std::string &name)
{
  reg = 0;
  stack = 0;

  this->max_stack = max_stack;
  //printf("max_stack=%d\n", max_stack);

  is_main = (name == "main") ? 1 : 0;
  is_interrupt = (name == "timerInterrupt") ? 1 : 0;

  // main() function goes here
  fprintf(out, "%s:\n", name.c_str());

  if (is_interrupt)
  {
    // If this is an interrupt, we have to push all possible registers that
    // could be in use.  Right now we'll push all temporary registers and
    // and max_stack registers.  This is bad because if the user calls a
    // another method in the interrupt that has a bigger max_stack, it
    // could cause odd results.  Fix later maybe.
    for (int n = 0; n < max_stack; n++)
    {
      fprintf(out, "  push r%d\n", REG_STACK(n));
    }

    // Why push r13?  it's pretty much a temp
    //fprintf(out, "  push r13\n");
    fprintf(out, "  push r14\n");
    fprintf(out, "  push r15\n");
  }

  if (!is_main) { fprintf(out, "  push r12\n"); }
  fprintf(out, "  mov.w SP, r12\n");
  fprintf(out, "  sub.w #0x%x, SP\n", local_count * 2);
}

void MSP430::method_end(int local_count)
{
  fprintf(out, "\n");
}

int MSP430::push_local_var_int(int index)
{
  //fprintf(out, "  mov.w r12, r15\n");
  //fprintf(out, "  sub.w #0x%02x, r15\n", LOCALS(index));

  if (reg < reg_max)
  {
    //fprintf(out, "  mov.w @r15, r%d\n", REG_STACK(reg));
    fprintf(out, "  mov.w -%d(r12), r%d  ; push local_%d\n", LOCALS(index), REG_STACK(reg), index);
    reg++;
  }
    else
  {
    //fprintf(out, "  push @r15\n");
    fprintf(out, "  push -%d(r12)  ; push local_%d\n", LOCALS(index), index);
    stack++;
  }

  return 0;
}

int MSP430::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int MSP430::push_ref_static(std::string &name, int index)
{
  if (reg < reg_max)
  {
    fprintf(out, "  mov.w #_%s, r%d\n", name.c_str(), REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  push #_%s\n", name.c_str());
    stack++;
  }

  return 0;
}

int MSP430::push_fake()
{
  if (stack != 0) { return -1; }
  reg++;
  return 0;
}

int MSP430::push_int(int32_t n)
{
  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);
    return -1;
  }

  uint16_t value = (n & 0xffff);

  if (reg < reg_max)
  {
    fprintf(out, "  mov.w #0x%02x, r%d\n", value, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  push #0x%02x\n", value);
    stack++;
  }

  return 0;
}

# if 0
int MSP430::push_long(int64_t n)
{
  return -1;
}

int MSP430::push_float(float f)
{
  return -1;
}

int MSP430::push_double(double f)
{
  return -1;
}
#endif

int MSP430::push_ref(std::string &name)
{
  if (reg < reg_max)
  {
    fprintf(out, "  mov.w &%s, r%d\n", name.c_str(), REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  push &%s\n", name.c_str());
    stack++;
  }

  return 0;
}

int MSP430::pop_local_var_int(int index)
{
  if (stack > 0)
  {
    fprintf(out, "  pop -%d(r12)  ; pop local_%d\n", LOCALS(index), index);
    stack--;
  }
    else
  if (reg > 0)
  {
    fprintf(out, "  mov.w r%d, -%d(r12) ; pop local_%d\n", REG_STACK(reg-1), LOCALS(index), index);
    reg--;
  }
    else
  {
    printf("Internal Error: Nothing on the stack?\n");
    return -1;
  }

  return 0;
}

int MSP430::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int MSP430::set_integer_local(int index, int value)
{
  // Optimization to remove Java stack operations
  if (value < -32768 || value > 0xffff) { return -1; }
  fprintf(out, "  mov.w #%d, -%d(r12) ; local_%d = %d\n", value, LOCALS(index), index, value);

  return 0;
}

int MSP430::set_ref_local(int index, std::string &name)
{
  fprintf(out, "  mov.w &%s, -%d(r12) ; local_%d = %s\n",
    name.c_str(), LOCALS(index), index, name.c_str());

  return 0;
}

int MSP430::pop()
{
  if (stack > 0)
  {
    fprintf(out, "  pop r15\n");
    stack--;
  }
    else
  if (reg > 0)
  {
    reg--;
  }

  return 0;
}

int MSP430::dup()
{
  if (stack > 0)
  {
    fprintf(out, "  push @SP\n");
    stack++;
  }
    else
  if (reg == reg_max)
  {
    fprintf(out, "  push r%d\n", REG_STACK(reg-1));
    stack++;
  }
    else
  {
    fprintf(out, "  mov.w r%d, r%d\n", REG_STACK(reg-1), REG_STACK(reg));
    reg++;
  }
  return 0;
}

int MSP430::dup2()
{
  printf("Need to implement dup2()\n");
  return -1;
}

int MSP430::swap()
{
  if (stack == 0)
  {
    fprintf(out, "  mov.w r%d, r15\n", REG_STACK(reg-1));
    fprintf(out, "  mov.w r%d, r%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
    fprintf(out, "  mov.w r15, r%d\n", REG_STACK(reg-2));
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  mov.w r%d, r15\n", REG_STACK(reg-1));
    fprintf(out, "  mov.w @SP, r%d\n", REG_STACK(reg-1));
    fprintf(out, "  mov.w r15, 0(SP)\n");
  }
    else
  {
    fprintf(out, "  mov.w (2)SP, r15\n");
    fprintf(out, "  mov.w @SP, 2(SP)\n");
    fprintf(out, "  mov.w r15, 0(SP)\n");
  }

  return 0;
}

int MSP430::add_integer()
{
  return stack_alu("add");
}

int MSP430::add_integer(int num)
{
  fprintf(out, "  add.w #%d, %s\n", num, top_reg());
  return 0;
}

int MSP430::sub_integer()
{
  return stack_alu("sub");
}

int MSP430::sub_integer(int num)
{
  fprintf(out, "  sub.w #%d, %s\n", num, top_reg());
  return 0;
}

int MSP430::mul_integer()
{
  int n;
  int saved_registers;

  saved_registers = reg;

  if(saved_registers != 2)
  {
    for (n = 0; n < reg; n++)
    {
      fprintf(out, "  push r%d\n", REG_STACK(n));
    }
  }

  if (stack == 0)
  {
    if(saved_registers != 2)
    {
      fprintf(out, "  mov r%d, r5\n", REG_STACK(reg-1));
      fprintf(out, "  mov r%d, r4\n", REG_STACK(reg-2));
    }
    fprintf(out, "  call #_mul_integers\n");
    reg--;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  mov r15, r5\n");
    fprintf(out, "  mov r%d, r4\n", REG_STACK(reg-1));
    fprintf(out, "  call #_mul_integers\n");
    stack--;
  }
    else
  {
    fprintf(out, "  mov r15, r5\n");
    fprintf(out, "  mov @SP, r4\n");
    fprintf(out, "  call #_mul_integers\n");
  }

  if(saved_registers != 2)
  {
    for (n = saved_registers-1; n >= 0; n--)
    {
      fprintf(out, "  pop r%d\n", REG_STACK(n));
    }
  }

  need_mul_integers = 1;

  return 0;
}

int MSP430::div_integer()
{
  int n;
  int saved_registers;

  saved_registers = reg;

  if(saved_registers != 2)
  {
    for (n = 0; n < saved_registers; n++)
    {
      fprintf(out, "  push r%d\n", REG_STACK(n));
    }
  }

  if (stack == 0)
  {
    if(saved_registers != 2)
    {
      fprintf(out, "  mov r%d, r5\n", REG_STACK(reg-1));
      fprintf(out, "  mov r%d, r4\n", REG_STACK(reg-2));
    }
    fprintf(out, "  call #_div_integers\n");
    reg--;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  mov r15, r5\n");
    fprintf(out, "  mov r%d, r4\n", REG_STACK(reg-1));
    fprintf(out, "  call #_div_integers\n");
    stack--;
  }
    else
  {
    fprintf(out, "  mov r15, r5\n");
    fprintf(out, "  mov @SP, r4\n");
    fprintf(out, "  call #_div_integers\n");
  }

  if(saved_registers != 2)
  {
    for (n = saved_registers-1; n >= 0; n--)
    {
      fprintf(out, "  pop r%d\n", REG_STACK(n));
    }
  }

  need_div_integers = 1;

  return 0;
}

int MSP430::mod_integer()
{
  return -1;
}

int MSP430::neg_integer()
{
  fprintf(out, "  ;; neg_integer()\n");
  fprintf(out, "  neg.w %s\n", top_reg());
  return 0;
}

int MSP430::shift_left_integer()
{
  fprintf(out, "  ;; shift_left_integer()\n");

  // FIXME - for MSP430x, this can be sped up
  if (stack > 0)
  {
    fprintf(out, "  pop r15\n");
    stack--;
  }
    else
  if (reg > 0)
  {
    fprintf(out, "  mov.w r%d, r15\n", REG_STACK(reg-1));
    reg--;
  }

  fprintf(out, "label_%d:\n", label_count);

  if (stack > 0)
  {
    fprintf(out, "  rla.w @SP\n");
  }
    else
  {
    fprintf(out, "  rla.w r%d\n", REG_STACK(reg-1));
  }

  fprintf(out, "  dec.w r15\n");
  fprintf(out, "  jnz label_%d\n", label_count);

  label_count++;

  return 0;
}

int MSP430::shift_left_integer(int count)
{
  int n;

  fprintf(out, "  ;; shift_left_integer(%d)\n", count);

  if (stack != 0) { return -1; }

  if (count >= 8)
  {
    fprintf(out, "  swpb r%d\n", REG_STACK(reg-1));
    fprintf(out, "  and.w #0xff00, r%d\n", REG_STACK(reg-1));
    count = count - 8;
  }

  for (n = 0; n < count; n ++)
  {
    fprintf(out, "  rla.w r%d\n", REG_STACK(reg-1));
  }

  return 0;
}

int MSP430::shift_right_integer()
{
  fprintf(out, "  ;; shift_right_integer()\n");

  // FIXME - for MSP430x, this can be sped up
  fprintf(out, "  mov.w %s, r15\n", pop_reg());

  fprintf(out, "label_%d:\n", label_count);
  fprintf(out, "  rra.w %s\n", top_reg());
  fprintf(out, "  dec.w r15\n");
  fprintf(out, "  jnz label_%d\n", label_count);

  label_count++;

  return 0;
}

int MSP430::shift_right_integer(int count)
{
  int n;

  fprintf(out, "  ;; shift_right_integer(%d)\n", count);

  if (stack != 0) { return -1; }
  if (count >= 8) { return -1; }

#if 0
  if (count >= 8)
  {
    fprintf(out, "  swpb r%d\n", REG_STACK(reg));
    fprintf(out, "  and.w #0xff00, r%d\n", REG_STACK(reg));
    count = count - 8;
  }
#endif

  for (n = 0; n < count; n ++)
  {
    fprintf(out, "  rra.w %s\n", top_reg());
  }

  return 0;
}

int MSP430::shift_right_uinteger()
{
  fprintf(out, "  ;; shift_right_uinteger()\n");

  // FIXME - for MSP430x, this can be sped up
  fprintf(out, "  mov.w %s, r15\n", pop_reg());

  fprintf(out, "label_%d:\n", label_count);
  fprintf(out, "  rra.w %s\n", top_reg());
  fprintf(out, "  dec.w r15\n");
  fprintf(out, "  jnz label_%d\n", label_count);

  label_count++;

  return 0;
}

int MSP430::shift_right_uinteger(int count)
{
  int n;

  fprintf(out, "  ;; shift_right_uinteger(%d)\n", count);

  if (stack != 0) { return -1; }

  if (count >= 8)
  {
    fprintf(out, "  swpb r%d\n", REG_STACK(reg-1));
    fprintf(out, "  and.w #0x00ff, r%d\n", REG_STACK(reg-1));
    count = count - 8;
  }

  if (count != 0) { fprintf(out, "  clrc\n"); }
  for (n = 0; n < count; n ++)
  {
    fprintf(out, "  rrc.w %s\n", top_reg());
  }

  return 0;
}

int MSP430::and_integer()
{
  fprintf(out, "  ;; and_integer()\n");
  return stack_alu("and");
}

int MSP430::and_integer(int num)
{
  fprintf(out, "  ;; and_integer(num=%d)\n", num);
  fprintf(out, "  and.w #%d, %s\n", num, top_reg());
  return 0;
}

int MSP430::or_integer()
{
  fprintf(out, "  ;; or_integer()\n");
  return stack_alu("bis");
}

int MSP430::or_integer(int num)
{
  fprintf(out, "  ;; or_integer(num=%d)\n", num);
  fprintf(out, "  bis.w #%d, %s\n", num, top_reg());
  return 0;
}

int MSP430::xor_integer()
{
  fprintf(out, "  ;; xor_integer()\n");
  return stack_alu("xor");
}

int MSP430::xor_integer(int num)
{
  fprintf(out, "  ;; xor_integer(num=%d)\n", num);
  fprintf(out, "  xor.w #%d, %s\n", num, top_reg());
  return 0;
}

int MSP430::inc_integer(int index, int num)
{
  fprintf(out, "  ;; inc_integer(index=%d,num=%d)\n", index, num);
  fprintf(out, "  add.w #%d, -%d(r12)\n", num, LOCALS(index));
  return 0;
}

int MSP430::integer_to_byte()
{
  fprintf(out, "  ;; integer_to_byte()\n");
  fprintf(out, "  sxt %s\n", top_reg());
  return 0;
}

int MSP430::integer_to_short()
{
  // I think we do nothing here.  On MSP430, integers are already 16 bit.
  return 0;
}

int MSP430::jump_cond(std::string &label, int cond, int distance)
{
  bool reverse = false;

  fprintf(out, "  ;; jump_cond(%s, cond=%d, distance=%d)\n", label.c_str(), cond, distance);

  // MSP430 doesn't have LESS_EQUAL or GREATER so change them
  if (cond == COND_LESS_EQUAL)
  {
    reverse = true;
    cond = COND_GREATER_EQUAL;
    fprintf(out, "  mov.w #0, r15\n");
  }
    else
  if (cond == COND_GREATER)
  {
    reverse = true;
    cond = COND_LESS;
    fprintf(out, "  mov.w #0, r15\n");
  }

  if (stack > 0)
  {
    fprintf(out, "  add.w #2, SP\n");
    if (reverse == false) { fprintf(out, "  cmp.w #0, -2(SP)\n"); }
    else { fprintf(out, "  cmp.w -2(SP), r15\n"); }
    stack--;
  }
    else
  {
    if (reverse == false)
    { fprintf(out, "  cmp.w #0, r%d\n", REG_STACK(reg-1));}
      else
    { fprintf(out, "  cmp.w r%d, r15\n", REG_STACK(reg-1));}
    reg--;
  }

  fprintf(out, "  %s %s\n", cond_str[cond], label.c_str());

  return 0;
}

int MSP430::jump_cond_zero(std::string &label, int cond, int distance)
{
  fprintf(out, "  ;; jump_cond_zero(%s, cond=%d, distance=%d)\n", label.c_str(), cond, distance);

  if (stack > 0)
  {
    fprintf(out, "  add.w #2, SP\n");
    fprintf(out, "  cmp.w #0, -2(SP)\n");
  }
    else
  {
    fprintf(out, "  cmp.w #0, r%d\n", REG_STACK(reg-1));
    reg--;
  }

  if (cond == COND_EQUAL)
  {
    fprintf(out, "  jeq %s\n", label.c_str());
    return 0;
  }
    else
  if (cond == COND_NOT_EQUAL)
  {
    fprintf(out, "  jne %s\n", label.c_str());
    return 0;
  }

  return -1;
}

int MSP430::jump_cond_integer(std::string &label, int cond, int distance)
{
  bool reverse = false;

  fprintf(out, "  ;; jump_cond_integer(%s, cond=%d, distance=%d)\n", label.c_str(), cond, distance);

  // MSP430 doesn't have LESS_EQUAL or GREATER so change them
  if (cond == COND_LESS_EQUAL)
  {
    reverse = true;
    cond = COND_GREATER_EQUAL;
  }
    else
  if (cond == COND_GREATER)
  {
    reverse = true;
    cond = COND_LESS;
  }

  if (stack > 1)
  {
    fprintf(out, "  add.w #4, SP\n");

    if (reverse == false) { fprintf(out, "  cmp.w -4(SP), -2(SP)\n"); }
    else { fprintf(out, "  cmp.w -2(SP), -4(SP)\n"); }

    stack -= 2;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  add.w #2, SP\n");
    if (reverse == false)
    { fprintf(out, "  cmp.w -2(SP), r%d\n", REG_STACK(reg-1)); }
      else
    { fprintf(out, "  cmp.w r%d, -2(SP)\n", REG_STACK(reg-1)); }

    stack--;
    reg--;
  }
    else
  {
    if (reverse == false)
    {
      fprintf(out, "  cmp.w r%d, r%d\n", REG_STACK(reg-1), REG_STACK(reg-2));
    }
      else
    {
      fprintf(out, "  cmp.w r%d, r%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
    }

    reg -= 2;
  }

  fprintf(out, "  %s %s\n", cond_str[cond], label.c_str());

  return 0;
}

int MSP430::jump_cond_integer(std::string &label, int cond, int const_val, int distance)
{
  bool reverse = false;

  fprintf(out, "  ;; jump_cond_integer(%s, cond=%d, const_val=%d, distance=%d)\n", label.c_str(), cond, const_val, distance);

  // MSP430 doesn't have LESS_EQUAL or GREATER so change them
  if (cond == COND_LESS_EQUAL)
  {
    reverse = true;
    cond = COND_GREATER_EQUAL;
  }
    else
  if (cond == COND_GREATER)
  {
    reverse = true;
    cond = COND_LESS;
  }

  if (stack >= 1)
  {
    return -1;
  }
    else
  {
    if (reverse == false)
    {
      fprintf(out, "  cmp.w #%d, r%d\n", const_val, REG_STACK(reg-1));
    }
      else
    {
      //fprintf(out, "  cmp.w r%d, r%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
      return -1;
    }

    reg -= 1;
  }

  fprintf(out, "  %s %s\n", cond_str[cond], label.c_str());

  return 0;
}

int MSP430::ternary(int cond, int value_true, int value_false)
{
  bool reverse = false;
  char source[16];
  char dest[16];

  fprintf(out, "  ;; ternary(cond=%d ? %d : %d\n", cond, value_true, value_false);

  // MSP430 doesn't have LESS_EQUAL or GREATER so change them
  if (cond == COND_LESS_EQUAL)
  {
    reverse = true;
    cond = COND_GREATER_EQUAL;
  }
    else
  if (cond == COND_GREATER)
  {
    reverse = true;
    cond = COND_LESS;
  }

  if (stack > 1)
  {
    fprintf(out, "  add.w #2, SP\n");

    strcpy(source, "-2(SP)");
    strcpy(dest, "0(SP)");

    stack -= 1;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  add.w #2, SP\n");

    strcpy(source, "-2(SP)");
    sprintf(dest, "r%d", REG_STACK(reg-2));

    stack--;
  }
    else
  {
    sprintf(source, "r%d", REG_STACK(reg-1));
    sprintf(dest, "r%d", REG_STACK(reg-2));

    reg -= 1;
  }

  if (reverse == false)
  {
    fprintf(out, "  cmp.w %s, %s\n", source, dest);
  }
    else
  {
    fprintf(out, "  cmp.w %s, %s\n", dest, source);
  }

  fprintf(out, "  mov.w #%d, %s\n", value_true, dest);

  fprintf(out, "  %s ternary_%d\n", cond_str[cond], label_count);

  fprintf(out, "  mov.w #%d, %s\n", value_false, dest);
  fprintf(out, "ternary_%d:\n", label_count);

  label_count++;

  return 0;
}

int MSP430::ternary(int cond, int compare, int value_true, int value_false)
{
  char dest[16];
  bool reverse = false;

  // MSP430 doesn't have LESS_EQUAL or GREATER so change them
  if (cond == COND_LESS_EQUAL)
  {
    reverse = true;
    cond = COND_GREATER_EQUAL;
  }
    else
  if (cond == COND_GREATER)
  {
    reverse = true;
    cond = COND_LESS;
  }

  fprintf(out, "  ;; ternary(cond=%d #%d ? %d : %d\n", cond, compare, value_true, value_false);

  if (stack != 0)
  {
    strcpy(dest, "0(SP)");
  }
    else
  {
    sprintf(dest, "r%d", REG_STACK(reg - 1));
  }

  if (reverse == false)
  {
    fprintf(out, "  cmp.w #%d, %s\n", compare, dest);
  }
    else
  {
    fprintf(out, "  mov.w #%d, r15\n", compare);
    fprintf(out, "  cmp.w %s, r15\n", dest);
  }

  fprintf(out, "  mov.w #%d, %s\n", value_true, dest);

  fprintf(out, "  %s ternary_%d\n", cond_str[cond], label_count);

  fprintf(out, "  mov.w #%d, %s\n", value_false, dest);
  fprintf(out, "ternary_%d:\n", label_count);

  label_count++;

  return 0;
}

int MSP430::return_local(int index, int local_count)
{
  fprintf(out, "  mov.w -%d(w12), r15\n", LOCALS(index));

  fprintf(out, "  mov.w r12, SP\n");
  if (!is_main) { fprintf(out, "  pop r12\n"); }
  fprintf(out, "  ret\n");

  return 0;
}

int MSP430::return_integer(int local_count)
{
  fprintf(out, "  mov.w %s, r15\n", pop_reg());
  fprintf(out, "  mov.w r12, SP\n");
  if (!is_main) { fprintf(out, "  pop r12\n"); }
  fprintf(out, "  ret\n");

  return 0;
}

int MSP430::return_void(int local_count)
{
  fprintf(out, "  mov.w r12, SP\n");

  if (is_interrupt)
  {
    // This should be the only place we return from an interrupt since
    // interrupts should be void.
    fprintf(out, "  pop r12\n");
    fprintf(out, "  pop r15\n");
    fprintf(out, "  pop r14\n");
    // Again, r13 is pretty much a temp, why push it
    //fprintf(out, "  pop r13\n");

    for (int n = max_stack - 1; n >= 0; n--)
    {
      fprintf(out, "  pop r%d\n", REG_STACK(n));
    }

    fprintf(out, "  reti\n");
  }
    else
  {
    if (!is_main) { fprintf(out, "  pop r12\n"); }
    fprintf(out, "  ret\n");
  }

  return 0;
}

int MSP430::jump(std::string &name, int distance)
{
  fprintf(out, "  jmp %s\n", name.c_str());
  return 0;
}

int MSP430::call(std::string &name)
{
  // FIXME - does this need to push the register stack?
  // This is for the Java instruction jsr.
  fprintf(out, "  call #%s\n", name.c_str());
  return 0;
}

int MSP430::invoke_static_method(const char *name, int params, int is_void)
{
  int local;
  int stack_vars = stack;
  int reg_vars = reg;
  int saved_registers;
  int n;

  fprintf(out, "  ; invoke_static_method() name=%s params=%d is_void=%d\n", name, params, is_void);

  // Push all used registers on the stack except the ones that are pulled
  // out for parameters.
  saved_registers = reg - (params - stack);
  for (n = 0; n < saved_registers; n++)
  {
    fprintf(out, "  push r%d\n", REG_STACK(n));
  }

  // Push pointer to local variables to the stack because the called
  // method will trash it.
  //fprintf(out, "  push r12\n");

  // Copy parameters onto the stack so they are local variables in
  // the called method.  Start with -4 because the return address will
  // be at 0 and r12 will be at 2.
  local = (params * -2);
  while(local != 0)
  {
    if (stack_vars > 0)
    {
      fprintf(out, "  mov.w %d(SP), %d(SP)\n", (stack-stack_vars) * 2, local-4);
      stack_vars--;
    }
      else
    {
      fprintf(out, "  mov.w r%d, %d(SP)\n", REG_STACK(reg_vars-1), local-4);
      reg_vars--;
    }

    local += 2;
  }

  // Make the call
  fprintf(out, "  call #%s\n", name);

  // Pop the local variables pointer
  //fprintf(out, "  pop r12\n");

  // Pop all used registers off the stack
  for (n = saved_registers-1; n >= 0; n--)
  {
    fprintf(out, "  pop r%d\n", REG_STACK(n));
  }

  // Pop all params off the Java stack
  if ((stack - stack_vars) > 0)
  {
    fprintf(out, "  add.w #%d, SP\n", (stack - stack_vars) * 2);
    params -= (stack - stack_vars);
  }

  if (params != 0)
  {
    reg -= params;
  }

  if (!is_void)
  {
    // Put r15 on the top of the stack
    if (reg < reg_max)
    {
      fprintf(out, "  mov.w r15, r%d\n", REG_STACK(reg));
      reg++;
    }
      else
    {
      // REVIEW - This looks wrong
      fprintf(out, "  push r15\n");
      stack++;
    }
  }

  return 0;
}

int MSP430::put_static(std::string &name, int index)
{
  if (stack > 0)
  {
    fprintf(out, "  pop r15\n");
    fprintf(out, "  mov.w r15, &%s\n", name.c_str());
    stack--;
  }
    else
  {
    fprintf(out, "  mov.w r%d, &%s\n", REG_STACK(reg-1), name.c_str());
    reg--;
  }

  return 0;
}

int MSP430::get_static(std::string &name, int index)
{
  if (reg < reg_max)
  {
    fprintf(out, "  mov.w &%s, r%d\n", name.c_str(), REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  push &%s\n", name.c_str());
    stack++;
  }

  return 0;
}

int MSP430::brk()
{
  return -1;
}

int MSP430::new_array(uint8_t type)
{
  // ref = heap + 2
  // heap = heap + sizeof(array) + 2 (to hold the length of the array)

  // I can't think of why the stack version should ever get run.. this
  // probably could be removed.  It's not right probably anyway.
  if (stack > 0)
  {
    // r14 is the length of the array
    // r15 points to heap free area
    // array len goes to array[-1]
    fprintf(out, "  pop r14\n");
    fprintf(out, "  mov.w &heap_ptr, r15\n");
    fprintf(out, "  mov.w r14, 0(r15)\n");

    // Maybe this can be optimized by detecting a new array and a constant
    // so the compile module can double / pad it and pass it here.
    if (type == TYPE_SHORT || type == TYPE_CHAR || type == TYPE_INT)
    {
      // if int or short double the len of array for space (16 bit)
      fprintf(out, "  rla r14\n");
      // Add 2 to the length of the array to account for array[-1]
      fprintf(out, "  add.w #2, r14\n");
    }
      else
    {
      // Add 2 to the length of the array to account for array[-1]
      // Add 1 to the len and and with ~1 to word align
      fprintf(out, "  add.w #3, r14\n");
      fprintf(out, "  and.w #0xfffe, r14\n");
    }

    // Increase where the heap points to by num of bytes allocated
    fprintf(out, "  add.w r14, &heap_ptr\n");

    // r15 should point to array[0] instead of array[-1] and is now top of stack
    //fprintf(out, "  add.w #3, r15\n");  // Add 2 for len, add 1 and then
    //fprintf(out, "  and.w #1, r15\n");  // and 1 to word align
    fprintf(out, "  push r15\n");

  }
    else
  {
    // r15 points to heap free area
    // array len goes to array[-1]
    fprintf(out, "  mov.w &heap_ptr, r15\n");
    fprintf(out, "  mov.w r%d, 0(r15)\n", REG_STACK(reg-1));

    if (type == TYPE_SHORT || type == TYPE_CHAR || type == TYPE_INT)
    {
      // if int or short double the len of array for space (16 bit)
      fprintf(out, "  rla r%d\n", REG_STACK(reg-1));

      // Add 2 to the length of the array to account for array[-1]
      fprintf(out, "  add.w #2, r%d\n", REG_STACK(reg-1));

    }
      else
    {
      // Add 2 to the length of the array to account for array[-1]
      // Add 1 to the length of the array and 0xfffe to 16 bit align length
      fprintf(out, "  add.w #3, r%d\n", REG_STACK(reg-1));
      fprintf(out, "  and.w #0xfffe, r%d\n", REG_STACK(reg-1));
    }

    // Increase where the heap points to by num of bytes allocated
    fprintf(out, "  add.w r%d, &heap_ptr\n", REG_STACK(reg-1));

    // Top of stack now holds pointer to array ref
    fprintf(out, "  add.w #2, r15\n");
    fprintf(out, "  mov.w r15, r%d\n", REG_STACK(reg-1));
  }

  return 0;
}

int MSP430::insert_array(std::string &name, int32_t *data, int len, uint8_t type)
{
  fprintf(out, ".align 16\n");

  if (type == TYPE_BYTE)
  {
    return insert_db(name, data, len, TYPE_SHORT);
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

int MSP430::insert_string(std::string &name, uint8_t *bytes, int len)
{
  fprintf(out, ".align 16\n");
  fprintf(out, "  dw %d\n", len);
  return insert_utf8(name, bytes, len);
}

int MSP430::push_array_length()
{
  if (stack > 0)
  {
    fprintf(out, "  pop r15\n");
    fprintf(out, "  mov.w -2(r15), r15\n");
    fprintf(out, "  push r15\n");
  }
    else
  {
    fprintf(out, "  mov.w -2(r%d), r%d\n", REG_STACK(reg-1), REG_STACK(reg-1));
  }

  return 0;
}

int MSP430::push_array_length(std::string &name, int field_id)
{
  //fprintf(out, "  mov.w #%s-2, r13\n", name);
  fprintf(out, "  mov.w &%s, r13  ; push %s.length\n", name.c_str(), name.c_str());

  if (reg < reg_max)
  {
    //fprintf(out, "  mov.w @r13, r%d\n", REG_STACK(reg));
    fprintf(out, "  mov.w -2(r13), r%d\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    //fprintf(out, "  push @r13\n");
    fprintf(out, "  push -2(r13)\n");
    stack++;
  }

  return 0;
}

int MSP430::array_read_byte()
{
  int index_reg;
  int ref_reg;

  get_values_from_stack(&index_reg, &ref_reg);
  fprintf(out, "  add.w r%d, r%d\n", index_reg, ref_reg);

  if (reg < reg_max)
  {
    fprintf(out, "  mov.b @r%d, r%d\n", ref_reg, REG_STACK(reg));
    fprintf(out, "  sxt r%d\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  mov.b @r%d, r15\n", ref_reg);
    fprintf(out, "  sxt r15\n");
    fprintf(out, "  push r15\n");
    stack++;
  }

  return 0;
}

int MSP430::array_read_short()
{
  int index_reg;
  int ref_reg;

  get_values_from_stack(&index_reg, &ref_reg);
  fprintf(out, "  rla.w r%d\n", index_reg);
  fprintf(out, "  add.w r%d, r%d\n", index_reg, ref_reg);

  if (reg < reg_max)
  {
    fprintf(out, "  mov.w @r%d, r%d\n", ref_reg, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  push @r%d\n", ref_reg);
    stack++;
  }

  return 0;
}

int MSP430::array_read_int()
{
  return array_read_short();
}

int MSP430::array_read_byte(std::string &name, int field_id)
{
  fprintf(out, "  mov.w &%s, r13\n", name.c_str());

  if (stack > 0)
  {
    fprintf(out, "  pop r15\n");
    fprintf(out, "  add.w r15, r13\n");
    fprintf(out, "  mov.b @r13, r15\n");
    fprintf(out, "  sxt r15\n");
    fprintf(out, "  push r15\n");
  }
    else
  {
    fprintf(out, "  add.w r%d, r13\n", REG_STACK(reg-1));
    fprintf(out, "  mov.b @r13, r%d\n", REG_STACK(reg-1));
    fprintf(out, "  sxt r%d\n", REG_STACK(reg-1));
  }

  return 0;
}

int MSP430::array_read_short(std::string &name, int field_id)
{
  fprintf(out, "  mov.w &%s, r13\n", name.c_str());

  if (stack > 0)
  {
    fprintf(out, "  pop r15\n");
    fprintf(out, "  rla.w r15\n");
    fprintf(out, "  add.w r15, r13\n");
    fprintf(out, "  push.w @r13\n");
  }
    else
  {
    fprintf(out, "  rla.w r%d\n", REG_STACK(reg-1));
    fprintf(out, "  add.w r%d, r13\n", REG_STACK(reg-1));
    fprintf(out, "  mov.w @r13, r%d\n", REG_STACK(reg-1));
  }

  return 0;
}

int MSP430::array_read_int(std::string &name, int field_id)
{
  return array_read_short(name, field_id);
}

int MSP430::array_write_byte()
{
  int value_reg;
  int index_reg;
  int ref_reg;

  get_values_from_stack(&value_reg, &index_reg, &ref_reg);
  fprintf(out, "  add.w r%d, r%d\n", index_reg, ref_reg);
  fprintf(out, "  mov.b r%d, 0(r%d)\n", value_reg, ref_reg);

  return 0;
}

int MSP430::array_write_short()
{
  int value_reg;
  int index_reg;
  int ref_reg;

  get_values_from_stack(&value_reg, &index_reg, &ref_reg);
  fprintf(out, "  rla.w r%d\n", index_reg);
  fprintf(out, "  add.w r%d, r%d\n", index_reg, ref_reg);
  fprintf(out, "  mov.w r%d, 0(r%d)\n", value_reg, ref_reg);

  return 0;
}

int MSP430::array_write_int()
{
  return array_write_short();
}

int MSP430::array_write_byte(std::string &name, int field_id)
{
  int value_reg;
  int index_reg;

  get_values_from_stack(&value_reg, &index_reg);

  fprintf(out, "  add.w &%s, r%d\n", name.c_str(), index_reg);
  fprintf(out, "  mov.b r%d, 0(r%d)\n", value_reg, index_reg);

  return 0;
}

int MSP430::array_write_short(std::string &name, int field_id)
{
  int value_reg;
  int index_reg;

  get_values_from_stack(&value_reg, &index_reg);

  fprintf(out, "  rla.w r%d\n", index_reg);
  fprintf(out, "  add.w &%s, r%d\n", name.c_str(), index_reg);
  fprintf(out, "  mov.w r%d, 0(r%d)\n", value_reg, index_reg);

  return 0;
}

int MSP430::array_write_int(std::string &name, int field_id)
{
  return array_write_short(name, field_id);
}

#if 0
void MSP430::close()
{
  fprintf(out, "    .org 0xfffe\n");
  fprintf(out, "    dw start\n");
}
#endif

// GPIO functions
int MSP430::ioport_setPinsAsInput_I(int port)
{
  char periph[32];
  sprintf(periph, "P%dDIR", port+1);
  return set_periph("bic", periph);
}

int MSP430::ioport_setPinsAsInput_I(int port, int const_val)
{
  char periph[32];
  sprintf(periph, "P%dDIR", port+1);
  fprintf(out, "  bic.b #%d, &%s\n", const_val, periph);
  return 0;
}

int MSP430::ioport_setPinsAsOutput_I(int port)
{
  char periph[32];
  sprintf(periph, "P%dDIR", port+1);
  return set_periph("bis", periph);
}

int MSP430::ioport_setPinsAsOutput_I(int port, int const_val)
{
  char periph[32];
  sprintf(periph, "P%dDIR", port+1);
  fprintf(out, "  bis.b #0x%02x, &%s\n", const_val, periph);
  return 0;
}

int MSP430::ioport_setPinsValue_I(int port)
{
  char periph[32];
  sprintf(periph, "P%dOUT", port+1);
  return set_periph("mov", periph);
}

int MSP430::ioport_setPinsValue_I(int port, int const_val)
{
  char periph[32];
  sprintf(periph, "P%dOUT", port+1);
  fprintf(out, "  mov.b #%d, &%s\n", const_val, periph);
  return 0;
}

int MSP430::ioport_setPinsHigh_I(int port)
{
  char periph[32];
  sprintf(periph, "P%dOUT", port+1);
  return set_periph("bis", periph);
}

int MSP430::ioport_setPinsLow_I(int port)
{
  char periph[32];
  sprintf(periph, "P%dOUT", port+1);
  return set_periph("bic", periph);
}

int MSP430::ioport_setPinAsOutput_I(int port)
{
  return -1;
}

int MSP430::ioport_setPinAsOutput_I(int port, int const_val)
{
  if (const_val < 0 || const_val > 7) { return -1; }
  fprintf(out, "  bis.b #0x%02x, &P%dDIR\n", (1<<const_val), port+1);
  return 0;
}

int MSP430::ioport_setPinAsInput_I(int port)
{
  return -1;
}

int MSP430::ioport_setPinAsInput_I(int port, int const_val)
{
  if (const_val < 0 || const_val > 7) { return -1; }
  fprintf(out, "  bic.b #0x%02x, &P%dDIR\n", (1<<const_val), port+1);
  return 0;
}

int MSP430::ioport_setPinHigh_I(int port)
{
  return -1;
}

int MSP430::ioport_setPinHigh_I(int port, int const_val)
{
  if (const_val < 0 || const_val > 7) { return -1; }
  fprintf(out, "  bis.b #0x%02x, &P%dOUT\n", (1<<const_val), port+1);
  return 0;
}

int MSP430::ioport_setPinLow_I(int port)
{
  return -1;
}

int MSP430::ioport_setPinLow_I(int port, int const_val)
{
  if (const_val < 0 || const_val > 7) { return -1; }
  fprintf(out, "  bic.b #0x%02x, &P%dOUT\n", (1<<const_val), port+1);
  return 0;
}

int MSP430::ioport_isPinInputHigh_I(int port)
{
  return -1;
}

int MSP430::ioport_getPortInputValue(int port)
{
  if (reg < reg_max)
  {
    fprintf(out, "  mov.b &P%dIN, r%d\n", port + 1, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  mov.b &P%dIN, r15\n", port + 1);
    fprintf(out, "  push r15\n");
    stack++;
  }

  return 0;
}

#if 0
int MSP430::ioport_setPortOutputValue_I(int port)
{
  if (stack == 0)
  {
    fprintf(out, "  mov.b r%d, &P%dOUT\n", REG_STACK(reg-1), port+1);
    reg--;
  }
    else
  {
    fprintf(out, "  pop.w r15\n");
    fprintf(out, "  mov.b r15, &P%dOUT\n", port+1);
    stack--;
  }

  return 0;
}
#endif

int MSP430::ioport_setPinsResistorEnable_I(int port)
{
  char periph[32];
  sprintf(periph, "P%dREN", port+1);
  return set_periph("mov", periph);
}

int MSP430::ioport_setPinsResistorEnable_I(int port, int const_val)
{
  char periph[32];
  sprintf(periph, "P%dREN", port+1);
  fprintf(out, "  mov.b #%d, &%s\n", const_val, periph);
  return 0;
}

// UART functions
int MSP430::uart_init_I(int port)
{
  return -1;
}

int MSP430::uart_init_I(int port, int baud_rate)
{
  return -1;
}

int MSP430::uart_send_I(int port)
{
  return -1;
}

int MSP430::uart_read(int port)
{
  return -1;
}

int MSP430::uart_isDataAvailable(int port)
{
  return -1;
}

int MSP430::uart_isSendReady(int port)
{
  return -1;
}

// SPI functions
int MSP430::spi_init_II(int port)
{
  if (port != 0) { return -1; }

  char dst[16];
  fprintf(out, "  ;; Set up SPI\n");
  fprintf(out, "  mov.b #(USIPE7|USIPE6|USIPE5|USIMST|USIOE|USISWRST), &USICTL0\n");
  pop_reg(dst);
  fprintf(out, "  mov.b %s, r14\n", dst);
  fprintf(out, "  rrc.b r14\n");
  fprintf(out, "  rrc.b r14\n");
  fprintf(out, "  and.b #0x80, r14 ; CPHA/USICKPH\n");
  //fprintf(out, "  mov.b #USICKPH, &USICTL1\n");
  fprintf(out, "  mov.b r14, &USICTL1\n");
  //fprintf(out, "  mov.b #(USIDIV_7|USISSEL_2), &USICKCTL ; div 128, SMCLK\n");
  fprintf(out, "  mov.b %s, r14\n", dst);
  fprintf(out, "  and.b #0x02, r14\n");
  pop_reg(dst);
  // If this came off the stack, let's put it in a register, if not let's
  // just use the register.
  if (dst[0] != 'r')
  {
    fprintf(out, "  mov.b %s, r15\n", dst);
    strcpy(dst, "r15");
  }
  fprintf(out, "  rrc.b %s\n", dst);
  fprintf(out, "  rrc.b %s\n", dst);
  fprintf(out, "  rrc.b %s\n", dst);
  fprintf(out, "  rrc.b %s\n", dst);
  fprintf(out, "  and.b #0xe0, %s\n", dst);
  fprintf(out, "  bis.b %s, r14\n", dst);
  fprintf(out, "  bis.b #USISSEL_2, r14\n");
  fprintf(out, "  mov.b r14, &USICKCTL ; DIV and CPOL/USICKPL\n");
  fprintf(out, "  bic.b #USISWRST, &USICTL0      ; clear reset\n\n");

  return 0;
}

int MSP430::spi_init_II(int port, int clock_divisor, int mode)
{
  fprintf(out, "  ;; Set up SPI\n");
  fprintf(out, "  mov.b #(USIPE7|USIPE6|USIPE5|USIMST|USIOE|USISWRST), &USICTL0\n");
  fprintf(out, "  mov.b #%s, &USICTL1\n",
    (mode & 1) == 0 ? "0":"USICKPH");
  fprintf(out, "  mov.b #USIDIV_%d|USISSEL_2%s, &USICKCTL\n",
    clock_divisor,
    (mode & 2) == 0 ? "":"|USICKPL");
  fprintf(out, "  bic.b #USISWRST, &USICTL0      ; clear reset\n\n");

  return 0;
}

int MSP430::spi_init16_II(int port)
{
  return -1;
}

int MSP430::spi_init16_II(int port, int clock_divisor, int mode)
{
  return -1;
}

int MSP430::spi_send_I(int port)
{
  if (port != 0) { return -1; }

  char dst[16];
  pop_reg(dst);

  fprintf(out, "  ;; spi_send_I()\n");
  fprintf(out, "  mov.b %s, &USISRL\n", dst);
  fprintf(out, "  mov.b #8, &USICNT\n");

  return 0;
}

int MSP430::spi_send16_I(int port)
{
  if (port != 0) { return -1; }

  char dst[16];
  pop_reg(dst);

  fprintf(out, "  ;; spi_send16_I()\n");
  fprintf(out, "  mov.w %s, &USISRL\n", dst);
  fprintf(out, "  mov.b #16, &USICNT\n");

  return 0;
}

int MSP430::spi_read_I(int port)
{
  if (port != 0) { return -1; }

  char dst[16];
  pop_reg(dst);

  fprintf(out, "  ;; spi_read_I()\n");
  fprintf(out, "  mov.b %s, r15\n", dst);
  fprintf(out, "  call #_read_spi\n");
  push_reg("r15");

  need_read_spi = 1;

  return 0;
}

int MSP430::spi_read16_I(int port)
{
  return -1;
}

int MSP430::spi_isDataAvailable(int port)
{
  if (port != 0) { return -1; }

  fprintf(out, "  mov.b &USICTL1, r15\n");
  fprintf(out, "  and.b #USIIFG, r15\n");
  push_reg("r15");

  return 0;
}

int MSP430::spi_isBusy(int port)
{
  return -1;
}

int MSP430::spi_disable(int port)
{
  if (port != 0) { return -1; }

  fprintf(out, "  bic.b #USIPE7|USIPE6|USIPE5, &USICTL0\n");

  return 0;
}

int MSP430::spi_enable(int port)
{
  if (port != 0) { return -1; }

  fprintf(out, "  bis.b #USIPE7|USIPE6|USIPE5, &USICTL0\n");

  return 0;
}

// CPU functions
int MSP430::cpu_setClock8()
{
  fprintf(out, "  ;; Set MCLK to 8 MHz with DCO\n");
  fprintf(out, "  mov.b #DCO_5, &DCOCTL\n");
  fprintf(out, "  mov.b #RSEL_13, &BCSCTL1\n");
  fprintf(out, "  mov.b #0, &BCSCTL2\n\n");

  return 0;
}

int MSP430::cpu_setClock16()
{
  fprintf(out, "  ;; Set MCLK to 16 MHz with DCO\n");
  fprintf(out, "  mov.b #DCO_4, &DCOCTL\n");
  fprintf(out, "  mov.b #RSEL_15, &BCSCTL1\n");
  fprintf(out, "  mov.b #0, &BCSCTL2\n\n");

  return 0;
}

int MSP430::cpu_nop()
{
  fprintf(out, "  nop\n");

  return 0;
}

// ADC
int MSP430::adc_enable()
{
  fprintf(out, "  mov.w #ADC10ON|ADC10SHT_3, &ADC10CTL0 ; ADC On\n");
  return 0;
}

int MSP430::adc_disable()
{
  fprintf(out, "  mov.w #0, &ADC10CTL0 ; ADC Off\n");
  return 0;
}

int MSP430::adc_setChannel_I()
{
int value_reg;

  get_values_from_stack(&value_reg);

  fprintf(out, "  swpb r%d\n", value_reg);
  fprintf(out, "  rla.w r%d\n", value_reg);
  fprintf(out, "  rla.w r%d\n", value_reg);
  fprintf(out, "  rla.w r%d\n", value_reg);
  fprintf(out, "  rla.w r%d\n", value_reg);
  fprintf(out, "  bis.w #ADC10SSEL_2|ADC10DIV_7, r%d\n", value_reg);
  fprintf(out, "  mov.w r%d, &ADC10CTL1\n", value_reg);

  return 0;
}

int MSP430::adc_setChannel_I(int channel)
{
  fprintf(out, "  mov.w #INCH_%d|ADC10SSEL_2|ADC10DIV_7, &ADC10CTL1\n", channel);
  return 0;
}

int MSP430::adc_read()
{
  fprintf(out, "  bis.w #ENC|ADC10SC, &ADC10CTL0\n");
  fprintf(out, "adcbusy_%d:\n", label_count);
  fprintf(out, "  bit.w #ADC10BUSY, &ADC10CTL1\n");
  fprintf(out, "  jnz adcbusy_%d\n", label_count);
  label_count++;

  if (reg < reg_max)
  {
    fprintf(out, "  mov.w &ADC10MEM, r%d\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  mov.w &ADC10MEM, r15\n");
    fprintf(out, "  push r15\n");
    stack++;
  }

  return 0;
}

// Timer functions
int MSP430::timer_setInterval_II()
{
  return -1;
}

int MSP430::timer_setInterval_II(int cycles, int divider)
{
  if (cycles > 0xffff)
  {
    printf("** Error cycles is more than 16 bit\n");
    return -1;
  }

  if (divider == 1) { divider = 0; }
  else if (divider == 2) { divider = 1; }
  else if (divider == 4) { divider = 2; }
  else if (divider == 8) { divider = 3; }
  else { printf("** divider must be 1,2,4,8\n"); return -1; }

  fprintf(out, "  ;; MSP430::timer_setInterval_II(%d, %d)\n", cycles, divider);
  fprintf(out, "  mov.w #%d, &TACCR0\n", cycles);
  fprintf(out, "  mov.w #(TASSEL_2|MC_1|%d), &TACTL ; SMCLK, DIV1, COUNT to TACCR0\n", divider << 6);
  fprintf(out, "  mov.w #0, &TACCTL1\n");

  return 0;
}

int MSP430::timer_setListener_Z()
{
  need_timer_interrupt = true;
  return -1;
}

int MSP430::timer_setListener_Z(int const_value)
{
  fprintf(out, "  ;; MSP430X::timer_setListener_Z(%d)\n", const_value);

  if (const_value != 0)
  {
    need_timer_interrupt = true;
    fprintf(out, "  bis.w #CCIE, &TACCTL0\n");
    fprintf(out, "  eint\n");
  }
    else
  {
    fprintf(out, "  bic.w #CCIE, &TACCTL0\n");
  }
  return 0;
}

int MSP430::timer_getValue()
{
  if (reg < reg_max)
  {
    fprintf(out, "  mov.w &TAR, r%d\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  mov.w &TAR, r15\n");
    fprintf(out, "  push r15\n");
    stack++;
  }

  return 0;
}

int MSP430::timer_setValue_I()
{
  if (stack != 0)
  {
    fprintf(out, "  pop r15\n");
    fprintf(out, "  mov.w r15, &TAR\n");
    stack--;
  }
    else
  {
    fprintf(out, "  mov.w r%d, &TAR\n", REG_STACK(reg-1));
    reg--;
  }

  return 0;
}

int MSP430::timer_setValue_I(int const_value)
{
  fprintf(out, "  mov.w #%d, &TAR\n", const_value);
  return 0;
}

// Watchdog
int MSP430::watchdog_enable()
{
  fprintf(out, "  ;; Set watchdog to clock / 32768.\n");
  fprintf(out, "  mov.w #(WDTPW|WDTIS0), &WDTCTL\n");

  return 0;
}

int MSP430::watchdog_disable()
{
  fprintf(out, "  ;; Turn off watchdog\n");
  fprintf(out, "  ;mov.w #(WDTPW|WDTHOLD), &WDTCTL\n");

  return 0;
}

int MSP430::watchdog_kick()
{
  fprintf(out, "  ;; Kick watchdog\n");
  fprintf(out, "  mov.w #(WDTPW|WDTCNTCL), &WDTCTL\n");

  return 0;
}

// Memory
int MSP430::memory_read8_I()
{
  if (stack != 0)
  {
    fprintf(out, "  mov.b @SP, 0(SP)\n");
  }
    else
  {
    fprintf(out, "  mov.b @r%d, r%d\n", REG_STACK(reg-1), REG_STACK(reg-1));
  }

  return 0;
}

int MSP430::memory_write8_IB()
{
  if (stack >= 2)
  {
    fprintf(out, "  mov.w 2(SP), r15\n");
    fprintf(out, "  mov.b @SP, 0(r15)\n");
    fprintf(out, "  add.w #4, SP\n");
    stack -= 2;
  }
    else
  if (stack == 1)
  {
    //fprintf(out, "  mov.w @SP, 0(r%d)\n\n", REG_STACK(reg-1));
    fprintf(out, "  pop.b 0(r%d)\n", REG_STACK(reg-1));
    reg--;
    stack--;
  }
    else
  {
    fprintf(out, "  mov.b r%d, 0(r%d)\n", REG_STACK(reg-1), REG_STACK(reg-2));
    reg -= 2;
  }

  return 0;
}

int MSP430::memory_read16_I()
{
  if (stack != 0)
  {
    fprintf(out, "  mov.w @SP, 0(SP)\n");
  }
    else
  {
    fprintf(out, "  mov.w @r%d, r%d\n", REG_STACK(reg-1), REG_STACK(reg-1));
  }

  return 0;
}

int MSP430::memory_write16_IS()
{
  if (stack >= 2)
  {
    fprintf(out, "  mov.w 2(SP), r15\n");
    fprintf(out, "  mov.w @SP, 0(r15)\n");
    fprintf(out, "  add.w #4, SP\n");
    stack -= 2;
  }
    else
  if (stack == 1)
  {
    //fprintf(out, "  mov.w @SP, 0(r%d)\n\n", REG_STACK(reg-1));
    fprintf(out, "  pop 0(r%d)\n", REG_STACK(reg-1));
    reg--;
    stack--;
  }
    else
  {
    fprintf(out, "  mov.w r%d, 0(r%d)\n", REG_STACK(reg-1), REG_STACK(reg-2));
    reg -= 2;
  }

  return 0;
}

// Protected functions
void MSP430::push_reg(const char *dst)
{
  if (reg < reg_max)
  {
    fprintf(out, "  mov.w %s, r%d\n", dst, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  push %s\n", dst);
    stack++;
  }
}

void MSP430::pop_reg(char *dst)
{
  if (stack > 0)
  {
    stack--;
    fprintf(out, "  pop r15\n");
    sprintf(dst, "r15");
  }
    else
  {
    reg--;
    sprintf(dst, "r%d", REG_STACK(reg));
  }
}

int MSP430::set_periph(const char *instr, const char *periph)
{
  if (stack == 0)
  {
    fprintf(out, "  %s.b r%d, &%s\n", instr, REG_STACK(reg-1), periph);
    reg--;
  }
    else
  {
    fprintf(out, "  pop.w r15\n");
    fprintf(out, "  %s.b r15, &%s\n", instr, periph);
    stack--;
  }

  return 0;
}

char *MSP430::pop_reg()
{
  if (stack == 0)
  {
    sprintf(reg_string, "r%d", REG_STACK(reg-1));
    reg--;
  }
    else
  {
    strcpy(reg_string, "@SP+");
    stack--;
  }

  return reg_string;
}

char *MSP430::top_reg()
{
  if (stack == 0)
  {
    sprintf(reg_string, "r%d", REG_STACK(reg-1));
  }
    else
  {
    strcpy(reg_string, "@SP");
  }

  return reg_string;
}

int MSP430::stack_alu(const char *instr)
{
  if (stack == 0)
  {
    fprintf(out, "  %s.w r%d, r%d\n", instr, REG_STACK(reg-1), REG_STACK(reg-2));
    reg--;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop r15\n");
    fprintf(out, "  %s.w r15, r%d\n", instr, REG_STACK(reg-1));
    stack--;
  }
    else
  {
    fprintf(out, "  pop r15\n");
    fprintf(out, "  %s.w r15, @SP\n", instr);
  }

  return 0;
}

void MSP430::insert_read_spi()
{
  fprintf(out, "; _read_spi(r15)\n");
  fprintf(out, "_read_spi:\n");
  fprintf(out, "  mov.b r15, &USISRL\n");
  fprintf(out, "  mov.b #8, &USICNT\n");
  fprintf(out, "_read_spi_wait:\n");
  fprintf(out, "  bit.b #USIIFG, &USICTL1\n");
  fprintf(out, "  jz _read_spi_wait\n");
  fprintf(out, "  mov.b &USISRL, r15\n");
  fprintf(out, "  ret\n\n");
}

void MSP430::insert_mul_integers()
{
  fprintf(out, "; _mul a * b\n");
  fprintf(out, "_mul_integers:\n");
  fprintf(out, "  clr r7\n");
  fprintf(out, "  mov r7, r15\n");
  fprintf(out, "  mov r7, r6\n");
  fprintf(out, "  tst r4\n");
  fprintf(out, "  jge _mul2\n");
  fprintf(out, "  mov #-1, r6\n");
  fprintf(out, "  jmp _mul2\n");
  fprintf(out, "_mul6:\n");
  fprintf(out, "  add r4, r15\n");
  fprintf(out, "  addc r6, r7\n");
  fprintf(out, "_mul1:\n");
  fprintf(out, "  rla r4\n");
  fprintf(out, "  rlc r6\n");
  fprintf(out, "_mul2:\n");
  fprintf(out, "  rra r5\n");
  fprintf(out, "  jc _mul5\n");
  fprintf(out, "  jne _mul1\n");
  fprintf(out, "  jmp _mul4\n");
  fprintf(out, "_mul5:\n");
  fprintf(out, "  sub r4, r15\n");
  fprintf(out, "  subc r6, r7\n");
  fprintf(out, "_mul3:\n");
  fprintf(out, "  rla r4\n");
  fprintf(out, "  rlc r6\n");
  fprintf(out, "  rra r5\n");
  fprintf(out, "  jnc _mul6\n");
  fprintf(out, "  cmp #0FFFFh, r5\n");
  fprintf(out, "  jne _mul3\n");
  fprintf(out, "_mul4:\n");
  fprintf(out, "  mov r15, r4\n");
  fprintf(out, "  ret\n\n");
}

void MSP430::insert_div_integers()
{
  fprintf(out, "; _div a / b (remainder in r7)\n");
  fprintf(out, "_div_integers:\n");
  fprintf(out, "  mov #16, r6\n");
  fprintf(out, "  clr r7\n");
  fprintf(out, "_div1:\n");
  fprintf(out, "  rla r4\n");
  fprintf(out, "  rlc r7\n");
  fprintf(out, "  bis #1, r4\n");
  fprintf(out, "  sub r5, r7\n");
  fprintf(out, "  jge _div2\n");
  fprintf(out, "  add r5, r7\n");
  fprintf(out, "  bic #1, r4\n");
  fprintf(out, "_div2:\n");
  fprintf(out, "  dec r6\n");
  fprintf(out, "  jnz _div1\n");
  fprintf(out, "  ret\n");
}

int MSP430::get_values_from_stack(int *value1, int *value2, int *value3)
{
  if (stack > 0)
  {
    fprintf(out, "  pop r15\n");
    *value1 = 15;
    stack--;
  }
    else
  {
    *value1 = REG_STACK(reg-1);
    reg--;
  }

  if (stack > 0)
  {
    fprintf(out, "  pop r14\n");
    *value2 = 14;
    stack--;
  }
    else
  {
    *value2 = REG_STACK(reg-1);
    reg--;
  }

  if (stack > 0)
  {
    fprintf(out, "  pop r13\n");
    *value3 = 13;
    stack--;
  }
    else
  {
    *value3 = REG_STACK(reg-1);
    reg--;
  }

  return 0;
}

int MSP430::get_values_from_stack(int *value1, int *value2)
{
  if (stack > 0)
  {
    fprintf(out, "  pop r14\n");
    *value1 = 14;
    stack--;
  }
    else
  {
    *value1 = REG_STACK(reg-1);
    reg--;
  }

  if (stack > 0)
  {
    fprintf(out, "  pop r13\n");
    *value2 = 13;
    stack--;
  }
    else
  {
    *value2 = REG_STACK(reg-1);
    reg--;
  }

  return 0;
}

int MSP430::get_values_from_stack(int *value1)
{
  if (stack > 0)
  {
    fprintf(out, "  pop r15\n");
    *value1 = 15;
    stack--;
  }
    else
  {
    *value1 = REG_STACK(reg-1);
    reg--;
  }

  return 0;
}


