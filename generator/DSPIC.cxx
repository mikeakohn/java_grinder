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

#define __STDC_FORMAT_MACROS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#include "DSPIC.h"

#define REG_STACK(a) (a + 1)
#define LOCALS(i) (i * 2)

// ABI is:
// w0 temp
// w1 bottom of stack
// w2 ..
// w3 ..
// w4 ..
// w5 ..
// w6 top of stack
// w13 temp
// w14 pointer to locals
//
// Stack on dsPIC moves value to [sp] and then increments sp by 2 (odd).

static const char *cond_str[] = { "z", "nz", "lt", "le", "gt", "ge" };

DSPIC::DSPIC(uint8_t chip_type) :
  reg(0),
  reg_max(6),
  stack(0),
  is_main(0)
{
  this->chip_type = chip_type;
}

DSPIC::~DSPIC()
{

}

int DSPIC::open(char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  // For now we only support a specific chip
  fprintf(out, ".dspic\n");

  switch(chip_type)
  {
    case DSPIC30F3012:
      fprintf(out, ".include \"p30f3012.inc\"\n\n");
      flash_start = 0x100;
      break;
    case DSPIC33FJ06GS101A:
      fprintf(out, ".include \"p33fj06gs101a.inc\"\n\n");
      flash_start = 0x100;
      break;
    default:
      printf("Unknown chip type.\n");
  }

  fprintf(out, ".org 0\n");
  fprintf(out, "  goto start\n\n");

  // Add any set up items (stack, registers, etc).  On some CPU's (dsPICF3012)
  // SP is automatically set but some it's not.  So for now we'll just set it.
  // Also, not sure what to do with SPLIM.
  fprintf(out, ".org %d\n", flash_start);
  fprintf(out, "start:\n");
  fprintf(out, "  mov #0x800, SP\n\n");

  return 0;
}

#if 0
void DSPIC::serial_init()
{
}
#endif

void DSPIC::method_start(int local_count, const char *name)
{
  reg = 0;
  stack = 0;

  is_main = (strcmp(name, "main") == 0) ? 1 : 0;

  // main() function goes here
  fprintf(out, "%s:\n", name);
  if (!is_main)
  {
    //fprintf(out, "  push w14\n");
    //fprintf(out, "  mov sp, w14\n");
    //fprintf(out, "  add #0x%x, sp\n", local_count * 2);
    fprintf(out, "  lnk #0x%x\n", local_count * 2);
  }
    else
  {
    fprintf(out, "  mov sp, w14\n");
    fprintf(out, "  add #0x%x, sp\n", local_count * 2);
  }
}

void DSPIC::method_end(int local_count)
{
  //fprintf(out, "  add #0x%x, sp\n", local_count * 2);
  //fprintf(out, "  ret\n\n");
  fprintf(out, "\n");
}

int DSPIC::push_integer(int32_t n)
{
  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);
    return -1;
  }

  uint16_t value = (n & 0xffff);

  if (reg < reg_max)
  {
    fprintf(out, "  mov #0x%02x, w%d\n", value, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  mov #0x%02x, w0\n", value);
    fprintf(out, "  push w0\n");
    stack++;
  }

  return 0;
}

int DSPIC::push_integer_local(int index)
{
  fprintf(out, "  mov [w14+%d], w0\n", LOCALS(index));

  if (reg < reg_max)
  {
    fprintf(out, "  mov w0, w%d\n", REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  push w0\n");
    stack++;
  }

  return 0;
}

int DSPIC::push_long(int64_t n)
{
  printf("long is not supported right now\n");
  return -1;
}

int DSPIC::push_float(float f)
{
  printf("float is not supported right now\n");
  return -1;
}

int DSPIC::push_double(double f)
{
  printf("double is not supported right now\n");
  return -1;
}

int DSPIC::push_byte(int8_t b)
{
  uint16_t value = ((int32_t)b)&0xffff;

  if (reg < reg_max)
  {
    fprintf(out, "  mov #0x%02x, w%d\n", value, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  mov #0x%02x, w0\n", value);
    fprintf(out, "  push w0\n");
    stack++;
  }
  return 0;
}

int DSPIC::push_short(int16_t s)
{
  uint16_t value = s;

  if (reg < reg_max)
  {
    fprintf(out, "  mov #0x%02x, w%d\n", value, REG_STACK(reg));
    reg++;
  }
    else
  {
    fprintf(out, "  mov #0x%02x, w0\n", value);
    fprintf(out, "  push w0\n");
    stack++;
  }
  return 0;
}

int DSPIC::pop_integer_local(int index)
{
  if (stack > 0)
  {
    fprintf(out, "  pop w0\n");
    fprintf(out, "  mov.w w0, [w14+%d]\n", LOCALS(index));
    stack--;
  }
    else
  if (reg > 0)
  {
    fprintf(out, "  mov.w w%d, [w14+%d]\n", REG_STACK(reg-1), LOCALS(index));
    reg--;
  }

  return 0;
}

int DSPIC::pop()
{
  if (stack > 0)
  {
    fprintf(out, "  pop w0\n");
    stack--;
  }
    else
  if (reg > 0)
  {
    reg--;
  }

  return 0;
}

int DSPIC::dup()
{
  if (stack > 0)
  {
    fprintf(out, "  push [SP]\n");
    stack++;
  }
    else
  if (reg == reg_max)
  {
    fprintf(out, "  push w%d\n", REG_STACK(reg-1));
    stack++;
  }
    else
  {
    fprintf(out, "  mov.w w%d, w%d\n", REG_STACK(reg-1), REG_STACK(reg));
    reg++;
  }

  return 0;
}

int DSPIC::dup2()
{
  printf("Need to implement dup2()\n");
  return -1;
}

int DSPIC::swap()
{
  if (stack == 0)
  {
    fprintf(out, "  mov.w w%d, w0\n", REG_STACK(reg-1));
    fprintf(out, "  mov.w w%d, w%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
    fprintf(out, "  mov.w w0, w%d\n", REG_STACK(reg-2));
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  mov.w w%d, w0\n", REG_STACK(reg-1));
    fprintf(out, "  mov.w [SP-2], w%d\n", REG_STACK(reg-1));
    fprintf(out, "  mov.w w0, [SP-2]\n");
  }
    else
  {
    fprintf(out, "  mov.w [SP-4], w0\n");
    fprintf(out, "  mov.w [SP-2], w13\n");
    fprintf(out, "  mov.w w13, [SP-4]\n");
    fprintf(out, "  mov.w w0, [SP-2]\n");
  }

  return 0;
}

int DSPIC::add_integers()
{
  return stack_alu("add");
}

int DSPIC::sub_integers()
{
  return stack_alu("sub");
}

int DSPIC::mul_integers()
{
  return stack_alu("mul");
}

int DSPIC::div_integers()
{
  return stack_alu("div");
}

int DSPIC::neg_integer()
{
  if (stack > 0)
  {
    fprintf(out, "  mov.w [SP-2], w0\n");
    fprintf(out, "  neg.w w0, w0\n");
    fprintf(out, "  mov.w w0, [SP-2]\n");
    stack--;
  }
    else
  {
    fprintf(out, "  neg.w w%d, w%d\n", REG_STACK(reg-1), REG_STACK(reg-1));
  }

  return 0;
}

int DSPIC::shift_left_integer()
{
  return stack_shift("sl");
}

int DSPIC::shift_right_integer()
{
  return stack_shift("asr");
}

int DSPIC::shift_right_uinteger()
{
  return stack_shift("lr");
}

int DSPIC::and_integer()
{
  return stack_alu("and");
}

int DSPIC::or_integer()
{
  return stack_alu("or");
}

int DSPIC::xor_integer()
{
  return stack_alu("xor");
}

int DSPIC::inc_integer(int index, int num)
{
  fprintf(out, "  mov [w14+%d], w0\n", LOCALS(index));
  fprintf(out, "  add w0, #%d, w0\n", num);
  fprintf(out, "  mov w0, [w14+%d]\n", LOCALS(index));

  return 0;
}

int DSPIC::jump_cond(const char *label, int cond)
{
  if (stack > 0)
  {
    fprintf(out, "  mov [SP-2], w0\n");
    fprintf(out, "  cp #0, w0\n");
  }
    else
  {
    fprintf(out, "  cp #0, w%d\n", reg);
  }

  fprintf(out, "  bra %s, %s\n", cond_str[cond], label);
  return 0;
}

int DSPIC::jump_cond_integer(const char *label, int cond)
{
  if (stack > 1)
  {
    fprintf(out, "  mov [SP-2], w0\n");
    fprintf(out, "  mov [SP-4], w13\n");
    //fprintf(out, "  cmp.w 2(SP), 4(SP)\n");
    //fprintf(out, "  cp w0, w13\n");
    fprintf(out, "  cp w13, w0\n");
    stack -= 2;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  mov [SP-2], w0\n");
    fprintf(out, "  cp w%d, w0\n", REG_STACK(reg-1));
    stack--;
    reg--;
  }
    else
  {
    fprintf(out, "  cp w%d, w%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
    reg -= 2;
  }

  fprintf(out, "  bra %s, %s\n", cond_str[cond], label);

  return 0;
}

int DSPIC::return_local(int index, int local_count)
{
#if 0
  fprintf(out, "  mov [w14-#%d], w0\n", LOCALS(index));
  //fprintf(out, "  add #0x%x, sp\n", local_count * 2);
  fprintf(out, "  mov w14, sp\n");
  fprintf(out, "  ret\n");

  return 0;
#endif
  return -1;
}

int DSPIC::return_integer(int local_count)
{
  if (stack > 0)
  {
    fprintf(out, "  mov [sp-2], w0\n");
  }
    else 
  {
    fprintf(out, "  mov w%d, w0\n", REG_STACK(reg - 1));
  }

  fprintf(out, "  ulnk\n");
  //fprintf(out, "  mov w14, sp\n");
  //if (!is_main) { fprintf(out, "  pop w14\n"); }
  fprintf(out, "  return\n");
  return 0;
}

int DSPIC::return_void(int local_count)
{
  //fprintf(out, "  mov w14, sp\n");
  //if (!is_main) { fprintf(out, "  pop w14\n"); }
  fprintf(out, "  ulnk\n");
  fprintf(out, "  return\n");

  return 0;
}

int DSPIC::jump(const char *name)
{
  fprintf(out, "  bra %s\n", name);
  return 0;
}

int DSPIC::call(const char *name)
{
  fprintf(out, "  call %s\n", name);
  return 0;
}

int DSPIC::invoke_static_method(const char *name, int params, int is_void)
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
    fprintf(out, "  push w%d\n", REG_STACK(n));
  }

  // Copy parameters onto the stack so they are local variables in
  // the called method.  Start with -4 because the return address will
  // be at 0 and w14 will be at 2.
  local = (params * -2);
  while(local != 0)
  {
    if (stack_vars > 0)
    {
      fprintf(out, "  mov %d(SP), %d(SP)\n", stack_vars, local-4);
      stack_vars--;
    }
      else
    {
      fprintf(out, "  mov w%d, %d(SP)\n", REG_STACK(reg_vars-1), local-4);
      reg_vars--;
    }

    local += 2;
  }

  // Make the call
  fprintf(out, "  call %s\n", name);

  // Pop all used registers off the stack
  for (n = saved_registers-1; n >= 0; n--)
  {
    fprintf(out, "  pop w%d\n", REG_STACK(n));
  }

  // Pop all params off the Java stack
  if ((stack - stack_vars) > 0)
  {
    fprintf(out, "  sub #%d, SP\n", (stack - stack_vars) * 2);
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
      fprintf(out, "  mov.w w0, w%d\n", REG_STACK(reg));
      reg++;
    }
      else
    {
      // REVIEW - This looks wrong
      fprintf(out, "  push w0\n");
      stack++;
    }
  }

  return 0;
}

int DSPIC::brk()
{
  return -1;
}

#if 0
void DSPIC::close()
{
}
#endif

// GPIO functions
int DSPIC::ioport_setPinsAsInput(int port)
{
  char periph[32];
  sprintf(periph, "TRIS%c", port+'A');
  return set_periph("ior", periph);
}

int DSPIC::ioport_setPinsAsOutput(int port)
{
  char periph[32];
  sprintf(periph, "TRIS%c", port+'A');
  return set_periph("and", periph, true);
}

int DSPIC::ioport_setPinsValue(int port)
{
  char periph[32];
  sprintf(periph, "PORT%c", port+'A');

  if (stack == 0)
  {
    fprintf(out, "  mov w%d, %s\n", REG_STACK(reg-1), periph);
    reg--;
  }
    else
  {
    fprintf(out, "  pop w0\n");
    fprintf(out, "  mov %s\n", periph);
    stack--;
  }

  return 0;
}

int DSPIC::ioport_setPinsHigh(int port)
{
  char periph[32];
  sprintf(periph, "PORT%c", port+'A');
  return set_periph("ior", periph);
}

int DSPIC::ioport_setPinsLow(int port)
{
  char periph[32];
  sprintf(periph, "PORT%c", port+'A');
  return set_periph("and", periph, true);
}

int DSPIC::ioport_setPinAsOutput(int port)
{
  return -1;
}

int DSPIC::ioport_setPinAsInput(int port)
{
  return -1;
}

int DSPIC::ioport_setPinHigh(int port)
{
  return -1;
}

int DSPIC::ioport_setPinLow(int port)
{
  return -1;
}

int DSPIC::ioport_isPinInputHigh(int port)
{
  return -1;
}

int DSPIC::ioport_getPortInputValue(int port)
{
  return -1;
}

// SPI functions
int DSPIC::spi_init(int port)
{
char dst[16];

  if (port != 0) { return -1; }

  fprintf(out, "  ;; Set up SPI\n");
  fprintf(out, "  mov #0x00, w0\n");
  fprintf(out, "  mov w0, SPI0CON1\n");
  pop_reg(out, dst);

  return -1;
}

int DSPIC::spi_send(int port)
{
  return -1;
}

int DSPIC::spi_read(int port)
{
  return -1;
}

int DSPIC::spi_isDataAvailable(int port)
{
  return -1;
}

int DSPIC::spi_isBusy(int port)
{
  return -1;
}

int DSPIC::spi_disable(int port)
{
  return -1;
}

int DSPIC::spi_enable(int port)
{
  return -1;
}


// CPU functions
int DSPIC::cpu_setClock16()
{
  return -1;
}

int DSPIC::cpu_nop()
{
  fprintf(out, "  nop\n");
  return 0;
}


// Memory
int DSPIC::memory_read8()
{
  return -1;
}

int DSPIC::memory_write8()
{
  return -1;
}

int DSPIC::memory_read16()
{
  return -1;
}

int DSPIC::memory_write16()
{
  return -1;
}

// DSP (dsPIC stuff)
int DSPIC::dsp_add_ab_and_store_in_a()
{
  fprintf(out, "  add A\n");
  return 0;
}

int DSPIC::dsp_add_ab_and_store_in_b()
{
  fprintf(out, "  add B\n");
  return 0;
}


void DSPIC::pop_reg(FILE *out, char *dst)
{
  if (stack > 0)
  {
    stack--;
    fprintf(out, "  pop w0\n");
    sprintf(dst, "w0");
  }
    else
  {
    reg--;
    sprintf(dst, "w%d", REG_STACK(reg));
  }
}

int DSPIC::set_periph(const char *instr, const char *periph, bool reverse)
{
  if (stack == 0)
  {
    fprintf(out, "  mov w%d, w0\n", REG_STACK(reg-1));
    reg--;
  }
    else
  {
    fprintf(out, "  pop w0\n");
    stack--;
  }

  if (reverse) { fprintf(out, "  xor #0xff, w0\n"); }
  fprintf(out, "  %s %s\n", instr, periph);

  return 0;
}

int DSPIC::stack_alu(const char *instr)
{
  if (stack == 0)
  {
    fprintf(out, "  %s.w w%d, w%d\n", instr, REG_STACK(reg-1), REG_STACK(reg-2));
    reg--;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop w0\n");
    fprintf(out, "  %s.w w0, w%d\n", instr, REG_STACK(reg-1));
    stack--;
  }
    else
  {
    fprintf(out, "  pop w0\n");
    fprintf(out, "  %s.w w0, [SP-2]\n", instr);
  }

  return 0;
}

int DSPIC::stack_shift(const char *instr)
{
  if (stack >= 2)
  {
    fprintf(out, "  pop w0\n");
    fprintf(out, "  pop w13\n");
    fprintf(out, "  %s w13, w0, w13\n", instr);
    fprintf(out, "  push w13\n");
    stack--;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop w0\n");
    fprintf(out, "  %s w%d, w0, w%d\n", instr, REG_STACK(reg-1), REG_STACK(reg-1));
    stack--;
  }
    else
  if (reg > 0)
  {
    fprintf(out, "  %s w%d, w%d, w%d\n", instr, REG_STACK(reg-2), REG_STACK(reg-1), REG_STACK(reg-2));
    reg--;
  }

  return 0;
}

