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

#include "MSP430.h"

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

#define REG_STACK(a) (a + 4)
#define LOCALS(a) ((a * 2) + 2)

// FIXME - This isn't quite right
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
  is_main(0)
{
  switch(chip_type)
  {
    case MSP430G2231:
      flash_start = 0xf800;
      stack_start = 0x0280;
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
  if (need_read_spi)
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
  if (need_mul_integers)
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

  if (need_div_integers)
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

  fprintf(out, ".org 0xfffe\n");
  fprintf(out, "  dw start\n\n");
}

int MSP430::open(char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  // For now we only support a specific chip
  fprintf(out, ".msp430\n");
  fprintf(out, ".include \"msp430x2xx.inc\"\n\n");

  // Add any set up items (stack, registers, etc)
  fprintf(out, ".org 0x%04x\n", flash_start);
  fprintf(out, "start:\n");
  fprintf(out, "  mov.w #(WDTPW|WDTHOLD), &WDTCTL\n");
  fprintf(out, "  mov.w #0x%04x, SP\n", stack_start);
  fprintf(out, "  jmp main\n\n");

  return 0;
}

#if 0
void MSP430::serial_init()
{
}
#endif

void MSP430::method_start(int local_count, const char *name)
{
  reg = 0;
  stack = 0;

  is_main = (strcmp(name, "main") == 0) ? 1 : 0;

  // main() function goes here
  fprintf(out, "%s:\n", name);
  if (!is_main) { fprintf(out, "  push r12\n"); }
  fprintf(out, "  mov.w SP, r12\n");
  fprintf(out, "  sub.w #0x%x, SP\n", local_count * 2);
}

void MSP430::method_end(int local_count)
{
  fprintf(out, "\n");
}

int MSP430::push_integer(int32_t n)
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

int MSP430::push_integer_local(int index)
{
  //fprintf(out, "  mov.w r12, r15\n");
  //fprintf(out, "  sub.w #0x%02x, r15\n", LOCALS(index));

  if (reg < reg_max)
  {
    //fprintf(out, "  mov.w @r15, r%d\n", REG_STACK(reg));
    fprintf(out, "  mov.w -%d(r12), r%d\n", LOCALS(index), REG_STACK(reg));
    reg++;
  }
    else
  {
    //fprintf(out, "  push @r15\n");
    fprintf(out, "  push -%d(r12)\n", LOCALS(index));
    stack++;
  }

  return 0;
}

int MSP430::push_long(int64_t n)
{
  printf("long is not supported right now\n");
  return -1;
}

int MSP430::push_float(float f)
{
  printf("float is not supported right now\n");
  return -1;
}

int MSP430::push_double(double f)
{
  printf("double is not supported right now\n");
  return -1;
}

int MSP430::push_byte(int8_t b)
{
  int16_t n = b;
  uint16_t value = (n & 0xffff);

  if (reg < reg_max)
  {
    fprintf(out, "  mov #0x%02x, r%d\n", value, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  push #0x%02x\n", value);
    stack++;
  }

  return 0;
}

int MSP430::push_short(int16_t s)
{
  uint16_t value = (s & 0xffff);

  if (reg < reg_max)
  {
    fprintf(out, "  mov #0x%02x, r%d\n", value, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  push #0x%02x\n", value);
    stack++;
  }

  return 0;
}

int MSP430::pop_integer_local(int index)
{
  if (stack > 0)
  {
    fprintf(out, "  pop -%d(r12)\n", LOCALS(index));
    stack--;
  }
    else
  if (reg > 0)
  {
    fprintf(out, "  mov.w r%d, -%d(r12)\n", REG_STACK(reg-1), LOCALS(index));
    reg--;
  }

  return 0;
}

int MSP430::set_integer_local(int index, int value)
{
  // Optimization to remove Java stack operations
  if (value < -32768 || value > 0xffff) { return -1; }
  fprintf(out, "  mov.w #%d, -%d(r12)\n", value, LOCALS(index));

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

int MSP430::add_integers()
{
  return stack_alu("add");
}

int MSP430::sub_integers()
{
  return stack_alu("sub");
}

int MSP430::mul_integers()
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

int MSP430::div_integers()
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

int MSP430::mod_integers()
{
  return -1;
}

int MSP430::neg_integer()
{
  if (stack > 0)
  {
    fprintf(out, "  neg.w @SP\n");
    stack--;
  }
    else
  {
    fprintf(out, "  neg.w r%d\n", REG_STACK(reg-1));
  }

  return 0;
}

int MSP430::shift_left_integer()
{
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

int MSP430::shift_right_integer()
{
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
    fprintf(out, "  rra.w @SP\n");
  }
    else
  {
    fprintf(out, "  rra.w r%d\n", REG_STACK(reg-1));
  }

  fprintf(out, "  dec.w r15\n");
  fprintf(out, "  jnz label_%d\n", label_count);

  label_count++;

  return 0;
}

int MSP430::shift_right_uinteger()
{
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
    fprintf(out, "  rra.w @SP\n");
  }
    else
  {
    fprintf(out, "  rra.w r%d\n", REG_STACK(reg-1));
  }

  fprintf(out, "  dec.w r15\n");
  fprintf(out, "  jnz label_%d\n", label_count);

  label_count++;

  return 0;
}

int MSP430::and_integer()
{
  return stack_alu("and");
}

int MSP430::or_integer()
{
  return stack_alu("or");
}

int MSP430::xor_integer()
{
  return stack_alu("xor");
}

int MSP430::inc_integer(int index, int num)
{
  fprintf(out, "  add.w #%d, -%d(r12)\n", num, LOCALS(index));
  return 0;
}

int MSP430::jump_cond(const char *label, int cond)
{
bool reverse = false;

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

  fprintf(out, "  %s %s\n", cond_str[cond], label);

  return 0;
}

int MSP430::jump_cond_integer(const char *label, int cond)
{
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

  fprintf(out, "  %s %s\n", cond_str[cond], label);

  return 0;
}

int MSP430::jump_cond_integer(const char *label, int cond, int const_val)
{
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

  fprintf(out, "  %s %s\n", cond_str[cond], label);

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
  if (stack > 0)
  {
    fprintf(out, "  pop.w @SP, r15\n");
    stack--;
  }
    else
  {
    fprintf(out, "  mov.w r%d, r15\n", REG_STACK(reg - 1));
    reg--;
  }

  fprintf(out, "  mov.w r12, SP\n");
  if (!is_main) { fprintf(out, "  pop r12\n"); }
  fprintf(out, "  ret\n");

  return 0;
}

int MSP430::return_void(int local_count)
{
  fprintf(out, "  mov r12, SP\n");
  if (!is_main) { fprintf(out, "  pop r12\n"); }
  fprintf(out, "  ret\n");

  return 0;
}

int MSP430::jump(const char *name)
{
  fprintf(out, "  jmp %s\n", name);
  return 0;
}

int MSP430::call(const char *name)
{
  // FIXME - do we need to push the register stack?
  // This is for the Java instruction jsr.
  fprintf(out, "  call #%s\n", name);
  return 0;
}

int MSP430::invoke_static_method(const char *name, int params, int is_void)
{
int local;
int stack_vars = stack;
int reg_vars = reg;
int saved_registers;
int n;

  printf("invoke_static_method() name=%s params=%d is_void=%d\n", name, params, is_void);

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
      fprintf(out, "  mov.w %d(SP), %d(SP)\n", stack_vars, local-4);
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
    fprintf(out, "  sub.w #%d, SP\n", (stack - stack_vars) * 2);
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

int MSP430::brk()
{
  return -1;
}

#if 0
void MSP430::close()
{
  fprintf(out, "    .org 0xfffe\n");
  fprintf(out, "    dw start\n");
}
#endif

// GPIO functions
int MSP430::ioport_setPinsAsInput(int port)
{
  char periph[32];
  sprintf(periph, "P%dDIR", port+1);
  return set_periph("bic", periph);
}

int MSP430::ioport_setPinsAsInput(int port, int const_val)
{
  char periph[32];
  sprintf(periph, "P%dDIR", port+1);
  fprintf(out, "  bic.b #%d, &%s\n", const_val, periph);
  return 0;
}

int MSP430::ioport_setPinsAsOutput(int port)
{
  char periph[32];
  sprintf(periph, "P%dDIR", port+1);
  return set_periph("bis", periph);
}

int MSP430::ioport_setPinsAsOutput(int port, int const_val)
{
  char periph[32];
  sprintf(periph, "P%dDIR", port+1);
  fprintf(out, "  bis.b #%d, &%s\n", const_val, periph);
  return 0;
}

int MSP430::ioport_setPinsValue(int port)
{
  char periph[32];
  sprintf(periph, "P%dOUT", port+1);
  return set_periph("mov", periph);
}

int MSP430::ioport_setPinsValue(int port, int const_val)
{
  char periph[32];
  sprintf(periph, "P%dOUT", port+1);
  fprintf(out, "  mov.b #%d, &%s\n", const_val, periph);
  return 0;
}

int MSP430::ioport_setPinsHigh(int port)
{
  char periph[32];
  sprintf(periph, "P%dOUT", port+1);
  return set_periph("bis", periph);
}

int MSP430::ioport_setPinsLow(int port)
{
  char periph[32];
  sprintf(periph, "P%dOUT", port+1);
  return set_periph("bic", periph);
}

int MSP430::ioport_setPinAsOutput(int port)
{
  return -1;
}

int MSP430::ioport_setPinAsInput(int port)
{
  return -1;
}

int MSP430::ioport_setPinHigh(int port)
{
  return -1;
}

int MSP430::ioport_setPinLow(int port)
{
  return -1;
}

int MSP430::ioport_isPinInputHigh(int port)
{
  return -1;
}

int MSP430::ioport_getPortInputValue(int port)
{
  return -1;
}

#if 0
int MSP430::ioport_setPortOutputValue(int port)
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

// UART functions
int MSP430::uart_init(int port)
{
  return -1;
}

int MSP430::uart_send(int port)
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
int MSP430::spi_init(int port)
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

int MSP430::spi_init(int port, int clock_divisor, int mode)
{
  fprintf(out, ";; Set up SPI\n");
  fprintf(out, "mov.b #(USIPE7|USIPE6|USIPE5|USIMST|USIOE|USISWRST), &USICTL0\n");
  fprintf(out, "mov.b #%s, &USICTL1\n",
    (mode & 1) == 0 ? "0":"|USICKPH");
  fprintf(out, "mov.b #USIDIV_%d|USISSEL_2%s, &USICKCTL\n",
    clock_divisor,
    (mode & 2) == 0 ? "":"|USICKPL");
  fprintf(out, "bic.b #USISWRST, &USICTL0      ; clear reset\n\n");

  return 0;
}

int MSP430::spi_send(int port)
{
  if (port != 0) { return -1; }

  char dst[16];
  pop_reg(dst);

  fprintf(out, "  mov.b %s, r15\n", dst);
  fprintf(out, "  call #_read_spi\n");
  push_reg("r15");

  need_read_spi = 1;

  return 0;
}

int MSP430::spi_read(int port)
{
  if (port != 0) { return -1; }

  fprintf(out, "  call #_read_spi\n");
  push_reg("r15");

  need_read_spi = 1;

  return 0;
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

// Memory
int MSP430::memory_read8()
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

int MSP430::memory_write8()
{
  if (stack >= 2)
  {
    fprintf(out, "  mov.w 2(SP), r15\n");
    fprintf(out, "  mov.b @SP, 0(r15)\n");
    fprintf(out, "  sub.w #4, SP\n");
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

int MSP430::memory_read16()
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

int MSP430::memory_write16()
{
  if (stack >= 2)
  {
    fprintf(out, "  mov.w 2(SP), r15\n");
    fprintf(out, "  mov.w @SP, 0(r15)\n");
    fprintf(out, "  sub.w #4, SP\n");
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


