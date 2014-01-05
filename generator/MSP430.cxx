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

#define REG_STACK(a) (a + 4)
#define LOCALS(a) ((a * 2) + 2)

// FIXME - This isn't quite right
static const char *cond_str[] = { "jz", "jnz", "jl", "jle", "jg", "jge" };

MSP430::MSP430() : reg(0), stack(0), label_count(0)
{

}

MSP430::~MSP430()
{
  fprintf(out, "  .org 0xfffe\n");
  fprintf(out, "  dw start\n");
}

int MSP430::open(char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  // For now we only support a specific chip
  fprintf(out, ".msp430\n");
  fprintf(out, ".include \"msp430x2xx.inc\"\n\n");

  // Add any set up items (stack, registers, etc)
  fprintf(out, "  .org 0xf800\n");
  fprintf(out, "start:\n");
  fprintf(out, "  mov.w #(WDTPW|WDTHOLD), &WDTCTL\n");
  fprintf(out, "  mov.w #0x0280, SP\n");
  fprintf(out, "  jmp main\n\n");

  return 0;
}

void MSP430::serial_init()
{
}

void MSP430::method_start(int local_count, const char *name)
{
  reg = 0;
  stack = 0;

  // main() function goes here
  fprintf(out, "%s:\n", name);
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

  if (reg < 8)
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

  if (reg < 8)
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

  if (reg < 8)
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

  if (reg < 8)
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

int MSP430::pop()
{
  if (stack > 0)
  {
    fprintf(out, "  pop cg\n");
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
  if (reg == 8)
  {
    fprintf(out, "  push r%d\n", REG_STACK(7));
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
  printf("mul not supported yet\n");

  return -1;
}

int MSP430::div_integers()
{
  printf("div not supported yet\n");

  return -1;
}

int MSP430::neg_integer()
{
  if (stack >= 1)
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
  if (stack > 0)
  {
    fprintf(out, "  cmp.w #0, 0(SP)\n");
  }
    else
  {
    fprintf(out, "  cmp.w #0, r%d\n", REG_STACK(reg-1));
  }

  fprintf(out, "  %s %s\n", cond_str[cond], label);

  return 0;
}

int MSP430::jump_cond_integer(const char *label, int cond)
{
  if (stack > 1)
  {
    fprintf(out, "  cmp.w 2(SP), 4(SP)\n");
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  cmp.w 2(SP), r%d\n", REG_STACK(reg-1));
  }
    else
  {
    fprintf(out, "  cmp.w r%d, r%d\n", REG_STACK(reg-1), REG_STACK(reg-2));
  }

  fprintf(out, "  %s %s\n", cond_str[cond], label);

  return 0;
}

int MSP430::return_local(int index, int local_count)
{
  fprintf(out, "  mov.w -%d(w12), r15\n", LOCALS(index));

  fprintf(out, "  mov.w r12, SP\n");
  fprintf(out, "  ret\n");

  return 0;
}

int MSP430::return_integer(int local_count)
{
  if (stack > 0)
  {
    fprintf(out, "  mov.w @SP, r15\n");
  }
    else
  {
    fprintf(out, "  mov.w r%d, r15\n", REG_STACK(reg - 1));
  }

  fprintf(out, "  mov.w r12, SP\n");
  fprintf(out, "  ret\n");

  return 0;
}

int MSP430::return_void(int local_count)
{
  fprintf(out, "  mov r12, SP\n");
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
  fprintf(out, "  call #%s\n", name);
  return 0;
}

int MSP430::brk()
{
  printf("brk unimplemented\n");
  return -1;
}

void MSP430::close()
{
  fprintf(out, "    .org 0xfffe\n");
  fprintf(out, "    dw start\n");
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
    fprintf(out, "  %s.w 15, r%d\n", instr, REG_STACK(reg-1));
    stack--;
  }
    else
  {
    fprintf(out, "  pop r15\n");
    fprintf(out, "  %s.w r15, @SP\n", instr);
  }

  return 0;
}

// GPIO functions
int MSP430::ioport_setPinsAsInput()
{
  if (stack == 0)
  {
    fprintf(out, "  bic.b r%d, &P1DIR\n", REG_STACK(reg-1));
    reg--;
  }
    else
  {
    fprintf(out, "  pop.w r15\n");
    fprintf(out, "  bic.b r15, &P1DIR\n");
    stack--;
  }

  return 0;
}

int MSP430::ioport_setPinsAsOutput()
{
  if (stack == 0)
  {
    fprintf(out, "  bis.b r%d, &P1DIR\n", REG_STACK(reg-1));
    reg--;
  }
    else
  {
    fprintf(out, "  pop.w r15\n");
    fprintf(out, "  bis.b r15, &P1DIR\n");
    stack--;
  }

  return 0;
}

int MSP430::ioport_setPinsValue()
{
  return -1;
}

int MSP430::ioport_setPinsHigh()
{
  return -1;
}

int MSP430::ioport_setPinsLow()
{
  return -1;
}

int MSP430::ioport_setPinAsOutput()
{
  return -1;
}

int MSP430::ioport_setPinAsInput()
{
  return -1;
}

int MSP430::ioport_setPinHigh()
{
  return -1;
}

int MSP430::ioport_setPinLow()
{
  return -1;
}

int MSP430::ioport_isPinInputHigh()
{
  return -1;
}

int MSP430::ioport_getPortInputValue()
{
  return -1;
}

int MSP430::ioport_setPortOutputValue()
{
  if (stack == 0)
  {
    fprintf(out, "  mov.b r%d, &P1OUT\n", REG_STACK(reg-1));
    reg--;
  }
    else
  {
    fprintf(out, "  pop.w r15\n");
    fprintf(out, "  mov.b r15, &P1OUT\n");
    stack--;
  }

  return 0;
}


