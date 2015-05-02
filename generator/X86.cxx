/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "X86.h"

#define REG_STACK(a) (registers[a])
#define REG_STACK8(a) (registers8[a])
#define REG_STACK16(a) (registers16[a])
#define LOCALS(i) (((i) * 4) + 4)

// ABI is:

#define REG_MAX 3
static const char *registers[] =   { "eax", "ecx", "edx" };
static const char *registers8[] =  {  "al",  "cl",  "dl" };
static const char *registers16[] = {  "ax",  "cx",  "dx" };

//                                 EQ    NE     LESS  LESS-EQ GR   GR-E
static const char *cond_str[] = { "je", "jne", "jl", "jle", "jg", "jge" };

X86::X86() :
  reg(0),
  stack(0),
  is_main(0)
{

}

X86::~X86()
{

}

int X86::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, "BITS 32\n");
  fprintf(out, "SECTION .bss\n");
  fprintf(out, "\n");

  return 0;
}

int X86::start_init()
{
  // Add any set up items (stack, registers, etc).
  fprintf(out, "SECTION .text\n");
  fprintf(out, "global init\n");
  fprintf(out, "init:\n");

  return 0;
}

int X86::insert_static_field_define(const char *name, const char *type, int index)
{
  //fprintf(out, "%s: resb 32\n", name);
  fprintf(out, "%s: resb 4\n", name);
  return 0;
}


int X86::init_heap(int field_count)
{
  // Don't think we need a heap?
  return 0;
}

int X86::insert_field_init_boolean(char *name, int index, int value)
{
  fprintf(out, "  mov eax, %d\n", value);
  fprintf(out, "  mov [%s], eax\n", name);
  return 0;
}

int X86::insert_field_init_byte(char *name, int index, int value)
{
  //fprintf(out, "  mov eax, %d\n", ((uint32_t)((uint8_t)value)));
  fprintf(out, "  mov eax, %d\n", value);
  fprintf(out, "  mov [%s], eax\n", name);
  return 0;
}

int X86::insert_field_init_short(char *name, int index, int value)
{
  //fprintf(out, "  mov eax, %d\n", ((uint32_t)((uint16_t)value)));
  fprintf(out, "  mov eax, %d\n", value);
  fprintf(out, "  mov [%s], eax\n", name);
  return 0;
}

int X86::insert_field_init_int(char *name, int index, int value)
{
  fprintf(out, "  mov eax, %d\n", value);
  fprintf(out, "  mov [%s], eax\n", name);
  return 0;
}

int X86::insert_field_init(char *name, int index)
{
  fprintf(out, "  mov eax, _%s\n", name);
  fprintf(out, "  mov [%s], eax\n", name);
  return 0;
}

void X86::method_start(int local_count, int max_stack, int param_count, const char *name)
{
  int i;

  if (method_count == 0)
  {
    fprintf(out, "  ret\n\n");
  }

  method_count++;

  fprintf(out, "; int %s(", name);
  for (i = 0; i < param_count; i++)
  {
    if (i != 0) { fprintf(out, ", "); }
    if (i < 26) { fprintf(out, "int %c", 'a' + i); }
  }
  fprintf(out, ");\n");

  fprintf(out, "global %s\n", name);
  fprintf(out, "%s:\n", name);

  fprintf(out, "  push ebx\n");
  fprintf(out, "  push esi\n");
  fprintf(out, "  push edi\n");
  fprintf(out, "  push ebp\n");
  fprintf(out, "  mov ebp, esp\n");

  if (local_count != 0)
  {
    fprintf(out, "  ; Allocate space for %d local variables\n", local_count);
    fprintf(out, "  sub esp, %d\n", local_count * 4);
  }

  // This looks awkward but was done this way to preserve C compatibility.
  if (param_count != 0)
  {
    fprintf(out, "  ; Copy %d parameters to local variables\n", param_count);

    for (i = 0; i < param_count; i++)
    {
      fprintf(out, "  mov eax, [ebp+%d]\n", (i * 4) + 8 + 12);
      fprintf(out, "  mov [ebp-%d], eax\n", LOCALS(i));
    }
  }
}

void X86::method_end(int local_count)
{
  fprintf(out, "\n");
}

int X86::push_integer(int32_t n)
{
  fprintf(out, "  ; push_integer(%d)\n", n);

  if (reg < REG_MAX)
  {
    fprintf(out, "  mov %s, %d\n", REG_STACK(reg++), n);
  }
    else
  {
    fprintf(out, "  push %d\n", n);
    stack++;
  }

  return 0;
}

int X86::push_integer_local(int index)
{
  fprintf(out, "  ; push_integer_local(%d)\n", index);

  if (reg < REG_MAX)
  {
    fprintf(out, "  mov %s, [ebp-%d]\n", REG_STACK(reg++), LOCALS(index));
  }
    else
  {
    fprintf(out, "  mov ebx, [ebp-%d]\n", LOCALS(index));
    fprintf(out, "  push ebx\n");
    stack++;
  }

  return 0;
}

int X86::push_ref_static(const char *name, int index)
{
  fprintf(out, "  ; push_ref_static(%s, %d)\n", name, index);

  if (reg < REG_MAX)
  {
    fprintf(out, "  mov %s, _%s\n", REG_STACK(reg++), name);
  }
    else
  {
    fprintf(out, "  push _%s\n", name);
    stack++;
  }

  return 0;
}

int X86::push_ref_local(int index)
{
  return push_integer_local(index);
}

int X86::push_fake()
{
  if (reg < REG_MAX)
  {
    reg++;  
  }
    else
  {
    fprintf(out, "  push eax\n");
    stack++;
  }

  return 0;
}

int X86::push_long(int64_t n)
{
  return -1;
}

int X86::push_float(float f)
{
  return -1;
}

int X86::push_double(double f)
{
  return -1;
}

int X86::push_byte(int8_t b)
{
  return push_integer((int32_t)b);
}

int X86::push_short(int16_t s)
{
  return push_integer((int32_t)s);
}

int X86::push_ref(char *name)
{
  fprintf(out, "  ; push_ref(%s)\n", name);

  if (reg < REG_MAX)
  {
    fprintf(out, "  mov %s, [%s]\n", REG_STACK(reg++), name);
  }
    else
  {
    fprintf(out, "  push [%s]\n", name);
    stack++;
  }

  return 0;
}

int X86::pop_integer_local(int index)
{
  if (reg < REG_MAX)
  {
    fprintf(out, "  mov [ebp-%d], %s\n", LOCALS(index), REG_STACK(--reg));
  }
    else
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  mov [ebp-%d], ebx\n", LOCALS(index));
    stack--;
  }

  return 0;
}

int X86::pop_ref_local(int index)
{
  return pop_integer_local(index);
}

int X86::pop()
{
  if (stack > 0)
  {
    fprintf(out, "  add esp, 4\n");
    stack--;
  }
    else
  {
    reg--;
  }

  return 0;
}

int X86::dup()
{
  fprintf(out, "  ; dup()\n");

  if (reg < REG_MAX)
  {
    fprintf(out, "  mov %s, %s\n", REG_STACK(reg), REG_STACK(reg - 1));
    reg++;
  }
    else
  if (reg == REG_MAX)
  {
    fprintf(out, "  push %s\n", REG_STACK(reg - 1));
  }
    else
  {
    fprintf(out, "  mov ebx, [esp]\n");
    fprintf(out, "  push ebx\n");
    stack++;
  }

  return -1;
}

int X86::dup2()
{
  fprintf(out, "  ; dup2()\n");

  if (reg == 2)
  {
    fprintf(out, "  mov %s, %s\n",  REG_STACK(reg), REG_STACK(reg-2));
    fprintf(out, "  push %s\n", REG_STACK(reg-1));
    reg++;
    stack++;
  }
    else
  if (reg == 3)
  {
    fprintf(out, "  mov ebx, %s\n",  REG_STACK(reg-1));
    fprintf(out, "  mov edi, [esp]\n");
    fprintf(out, "  push ebx\n");
    fprintf(out, "  push edi\n");

    stack += 2;
  }
    else
  {
    fprintf(out, "  mov ebx, [esp+4]\n");
    fprintf(out, "  mov edi, [esp]\n");
    fprintf(out, "  push ebx\n");
    fprintf(out, "  push edi\n");

    stack += 2;
  }
  return 0;
}

int X86::swap()
{
  fprintf(out, "  ; swap()\n");

  if (reg < 2)
  {
    printf("Error: swap() requires 2 registers on the stack\n");
    return -1;
  }

  if (stack == 0)
  {
    fprintf(out, "  mov ebx, %s\n", REG_STACK(reg - 1));
    fprintf(out, "  mov %s, %s\n", REG_STACK(reg - 1), REG_STACK(reg - 2));
    fprintf(out, "  mov %s, ebx\n", REG_STACK(reg - 2));
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  mov ebx, [esp]\n");
    fprintf(out, "  mov [esp], ebx\n");
    fprintf(out, "  mov %s, ebx\n", REG_STACK(reg - 1));
  }
    else
  {
    fprintf(out, "  mov ebx, [esp]\n");
    fprintf(out, "  mov edi, [esp+4]\n");
    fprintf(out, "  mov [esp], edi\n");
    fprintf(out, "  mov [esp+4], ebx\n");
  }

  return 0;
}

int X86::add_integer()
{
  return stack_alu("add");
}

int X86::add_integer(int num)
{
  return stack_alu("add", num);
}

int X86::sub_integer()
{
  return stack_alu("sub");
}

int X86::sub_integer(int num)
{
  return stack_alu("sub", num);
}

int X86::mul_integer()
{
  if (stack == 1)
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  imul edx, ebx\n");
    stack--;
  }
    else
  if (stack > 1)
  {
    fprintf(out, "  pop edi\n");
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  imul ebx, edi\n");
    fprintf(out, "  push ebx\n");
    stack--;
  }
    else
  if (reg == 2)
  {
    fprintf(out, "  imul ecx\n");
    reg--;
  }
    else
  if (reg == 3)
  {
    fprintf(out, "  imul ecx, edx\n");
    reg--;
  }
    else
  {
    printf("Internal Error: %s:%d\n", __FILE__, __LINE__);
    return -1;
  }

  return 0;
}

int X86::div_integer()
{
  return stack_div(true);
}

int X86::mod_integer()
{
  return stack_div(false);
}

int X86::neg_integer()
{
  if (stack > 0)
  {
    fprintf(out, "  neg dword [esp]\n");
  }
    else
  {
    fprintf(out, "  neg %s\n", REG_STACK(reg - 1));
  }

  return 0;
}

int X86::shift_left_integer()
{
  return stack_shift("sal");
}

int X86::shift_left_integer(int num)
{
  return stack_alu("sal", num);
}

int X86::shift_right_integer()
{
  return stack_shift("sar");
}

int X86::shift_right_integer(int num)
{
  return stack_alu("sar", num);
}

int X86::shift_right_uinteger()
{
  return stack_shift("shr");
}

int X86::shift_right_uinteger(int num)
{
  return stack_alu("shr", num);
}

int X86::and_integer()
{
  return stack_alu("and");
}

int X86::and_integer(int num)
{
  return stack_alu("and", num);
}

int X86::or_integer()
{
  return stack_alu("or");
}

int X86::or_integer(int num)
{
  return stack_alu("or", num);
}

int X86::xor_integer()
{
  return stack_alu("xor");
}

int X86::xor_integer(int num)
{
  return stack_alu("xor", num);
}

int X86::inc_integer(int index, int num)
{
  fprintf(out, "  ; inc_integer(%d,%d)\n", index, num);
  fprintf(out, "  add dword [ebp-%d], %d\n", LOCALS(index), num);
  return 0;
}

int X86::integer_to_byte()
{
  fprintf(out, "  ; integer_to_byte() (sign extend)\n");

  if (stack > 0)
  {
    fprintf(out, "  mov ebx, [esp]\n");
    fprintf(out, "  movsx ebx, bl\n");
    fprintf(out, "  mov [esp], ebx\n");
  }
    else
  {
    fprintf(out, "  movsx %s, %s\n", REG_STACK(reg-1), REG_STACK8(reg-1));
  }

  return 0;
}

int X86::integer_to_short()
{
  fprintf(out, "  ; integer_to_short() (sign extend)\n");

  if (stack > 0)
  {
    fprintf(out, "  mov ebx, [esp]\n");
    fprintf(out, "  movsx ebx, bx\n");
    fprintf(out, "  mov [esp], ebx\n");
  }
    else
  {
    fprintf(out, "  movsx %s, %s\n", REG_STACK(reg-1), REG_STACK16(reg-1));
  }

  return 0;
}

int X86::jump_cond(const char *label, int cond, int distance)
{
  fprintf(out, "  ; jump_cond(%s, %d, %d)\n", label, cond, distance);

  if (stack > 0)
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  cmp ebx, 0\n");
    stack--;
  }
    else
  {
    fprintf(out, "  cmp %s, 0\n", REG_STACK(--reg));
  }

  fprintf(out, "  %s %s\n", cond_str[cond], label);

  return 0;
}

int X86::jump_cond_integer(const char *label, int cond, int distance)
{
  fprintf(out, "  ; jump_cond_integer(%s, %d, %d)\n", label, cond, distance);

  if (stack == 1)
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  cmp %s, ebx\n", REG_STACK(--reg));
    stack--;
  }
    else
  if (stack > 1)
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  pop edi\n");
    fprintf(out, "  cmp edi, ebx\n");
    stack -= 2;
  }
    else
  {
    fprintf(out, "  cmp %s, %s\n", REG_STACK(reg - 2), REG_STACK(reg - 1));
    reg -= 2;
  }

  fprintf(out, "  %s %s\n", cond_str[cond], label);

  return 0;
}

int X86::return_local(int index, int local_count)
{
  return -1;
}

int X86::return_integer(int local_count)
{
  if (local_count != 0)
  {
    fprintf(out, "  ; Free stack space for %d local variables\n", local_count);
    fprintf(out, "  add esp, %d\n", local_count * 4);
  }

  fprintf(out, "  pop ebp\n");
  fprintf(out, "  pop edi\n");
  fprintf(out, "  pop esi\n");
  fprintf(out, "  pop ebx\n");

  if (reg != 1)
  {
    fprintf(out, "  mov eax, %s\n", REG_STACK(reg - 1));
    reg--;
  }
    else
  if (reg == 1)
  {
    // Return value is top of stack (eax)
    reg--;
  }

  if (reg != 0 || stack != 0)
  {
    printf("Error: register stack not empty? (%d,%d)\n", reg, stack);
    return -1;
  }

  fprintf(out, "  ret\n");

  return 0;
}

int X86::return_void(int local_count)
{
  if (local_count != 0)
  {
    fprintf(out, "  ; Free stack space for %d local variables\n", local_count);
    fprintf(out, "  add esp, %d\n", local_count * 4);
  }

  fprintf(out, "  pop ebp\n");
  fprintf(out, "  pop edi\n");
  fprintf(out, "  pop esi\n");
  fprintf(out, "  pop ebx\n");

  fprintf(out, "  ret\n");

  return 0;
}

int X86::jump(const char *name, int distance)
{
  fprintf(out, "  jmp %s\n", name);

  return 0;
}

int X86::call(const char *name)
{
  return -1;
}

int X86::invoke_static_method(const char *name, int params, int is_void)
{
  bool push_ebx = false;
  int saved_register_count;
  int stack_params = 0;
  int n;

  fprintf(out, "  ; invoke_static_method() name=%s params=%d is_void=%d reg=%d stack=%d\n", name, params, is_void, reg, stack);

  // Save all registers except parameters
  //saved_register_count = reg - (params - stack) < 0 ? 0 : reg - params;
  saved_register_count = reg - (params - stack);
  if (saved_register_count != 0)
  {
    fprintf(out, "  ; save %d registers\n", saved_register_count);
    for (n = 0; n < saved_register_count; n++)
    {
      fprintf(out, "  push %s\n", REG_STACK(n));
    }
  }

  if (params != 0)
  {
    fprintf(out, "  ; push %d params on the stack\n", params);
    stack_params = params - (reg - saved_register_count);
    int distance = (stack_params * 4) - 4;

    fprintf(out, "  ; %d params are already on the stack\n", stack_params);
    for (n = 0; n < stack_params; n++)
    {
      fprintf(out, "  mov ebx, [esp+%d]\n", distance);
      fprintf(out, "  push ebx\n");
    }

    fprintf(out, "  ; %d params are in registers\n", reg - saved_register_count);
    for (n = reg; n > saved_register_count; n--)
    {
      fprintf(out, "  push %s\n", REG_STACK(n - 1));
    }
  }

  fprintf(out, "  call %s\n", name);

  if (params != 0)
  {
    fprintf(out, "  ; pop %d params off the stack\n", params);
    fprintf(out, "  add esp, %d\n", (params * 4) + (stack_params * 4));

    stack -= stack_params;
  }

  // FIXME - is this right?
  reg = saved_register_count;

  if (is_void == false)
  {
    if (reg == 0)
    {
      fprintf(out, "  ; mov eax, eax\n");
      reg++;
    }
      else
    if (reg < REG_MAX)
    {
      fprintf(out, "  mov %s, eax\n", REG_STACK(reg++));
    }
      else
    {
      fprintf(out, "  mov ebx, eax\n");
    }
  }

  // Restore all registers
  for (n = saved_register_count - 1; n >= 0; n--)
  {
    fprintf(out, "  pop %s\n", REG_STACK(n));
  }

  if (push_ebx)
  {
    fprintf(out, "  push ebx\n");
    stack++;
  }

  return 0;
}

int X86::put_static(const char *name, int index)
{
  if (stack > 0)
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  mov [%s], ebx\n", name);
    stack--;
  }
    else
  {
    fprintf(out, "  mov [%s], %s\n", name, REG_STACK(reg - 1));
    reg--;
  }

  return 0;
}

int X86::get_static(const char *name, int index)
{
  if (reg < REG_MAX)
  {
    fprintf(out, "  mov %s, [%s]\n", REG_STACK(reg++), name);
  }
    else
  {
    fprintf(out, "  push [%s]\n", name);
    stack++;
  }

  return 0;
}

int X86::brk()
{
  return -1;
}

int X86::new_array(uint8_t type)
{
  return -1;
}

int X86::insert_array(const char *name, int32_t *data, int len, uint8_t type)
{
  fprintf(out, "align 4\n");
  if (type == TYPE_BYTE)
  { return insert_db(name, data, len, TYPE_INT); }
    else
  if (type == TYPE_SHORT)
  { return insert_dw(name, data, len, TYPE_INT); } 
    else
  if (type == TYPE_INT)
  { return insert_dc32(name, data, len, TYPE_INT, "dd"); } 

  return -1;
}

int X86::insert_string(const char *name, uint8_t *bytes, int len)
{
  fprintf(out, "align 4\n");
  fprintf(out, "  dc32 %d\n", len);
  return insert_utf8(name, bytes, len);
}

int X86::push_array_length()
{
  if (stack > 0)
  {
    fprintf(out, "  pop esi\n");
    fprintf(out, "  push [esi-4]\n");
  }
    else
  {
    fprintf(out, "  mov %s, [%s-4]\n", REG_STACK(reg-1), REG_STACK(reg-1));
  }

  return 0;
}

int X86::push_array_length(const char *name, int field_id)
{
  if (reg < REG_MAX)
  {
    fprintf(out, "  mov %s, [%s-4]\n", REG_STACK(reg++), name);
  }
    else
  {
    fprintf(out, "  push [%s-4]\n", name);
    stack++;
  }

  return 0;
}

int X86::array_read_byte()
{
  fprintf(out, "  ; array_read_byte()\n");

  if (stack == 0)
  {
    fprintf(out, "  mov %s, [%s+%s]\n", REG_STACK8(reg-2), REG_STACK(reg-2), REG_STACK(reg-1));
    reg--;
    fprintf(out, "  movsx %s, %s\n", REG_STACK(reg-1), REG_STACK8(reg-1));
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  mov %s, [%s+ebx]\n", REG_STACK8(reg-1), REG_STACK(reg-1));
    fprintf(out, "  movsx %s, %s\n", REG_STACK(reg-1), REG_STACK8(reg-1));
    stack--;
  }
    else
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  pop esi\n");
    fprintf(out, "  mov bl, [esi+ebx]\n");
    fprintf(out, "  movsx ebx, bl\n");
    fprintf(out, "  push ebx\n");
    stack--;
  }

  return 0;
}

int X86::array_read_short()
{
  fprintf(out, "  ; array_read_short()\n");

  if (stack == 0)
  {
    //fprintf(out, "  sal %s, 1\n", REG_STACK(reg-1));
    fprintf(out, "  mov %s, [%s+%s*2]\n", REG_STACK16(reg-2), REG_STACK(reg-2), REG_STACK(reg-1));
    reg--;
    fprintf(out, "  movsx %s, %s\n", REG_STACK(reg-1), REG_STACK16(reg-1));
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop ebx\n");
    //fprintf(out, "  sal ebx, 1\n");
    fprintf(out, "  mov %s, [%s+ebx*2]\n", REG_STACK16(reg-1), REG_STACK(reg-1));
    fprintf(out, "  movsx %s, %s\n", REG_STACK(reg-1), REG_STACK16(reg-1));
    stack--;
  }
    else
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  pop esi\n");
    //fprintf(out, "  sal ebx, 1\n");
    fprintf(out, "  mov bx, [esi+ebx*2]\n");
    fprintf(out, "  movsx ebx, bx\n");
    fprintf(out, "  push ebx\n");
    stack--;
  }

  return 0;
}

int X86::array_read_int()
{
  fprintf(out, "  ; array_read_int()\n");

  if (stack == 0)
  {
    //fprintf(out, "  sal %s, 2\n", REG_STACK(reg-1));
    fprintf(out, "  mov %s, [%s+%s*4]\n", REG_STACK(reg-2), REG_STACK(reg-2), REG_STACK(reg-1));
    reg--;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop ebx\n");
    //fprintf(out, "  sal ebx, 2\n");
    fprintf(out, "  mov %s, [%s+ebx*4]\n", REG_STACK(reg-1), REG_STACK(reg-1));
    stack--;
  }
    else
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  pop esi\n");
    //fprintf(out, "  sal ebx, 2\n");
    fprintf(out, "  mov ebx, [esi+ebx*4]\n");
    fprintf(out, "  push ebx\n");
    stack--;
  }

  return 0;
}

int X86::array_read_byte(const char *name, int field_id)
{
  fprintf(out, "  ; array_read_byte(%s,%d)\n", name, field_id);

  if (stack == 0)
  {
    fprintf(out, "  mov %s, [%s+%s]\n", REG_STACK8(reg-1), name, REG_STACK(reg-1));
    fprintf(out, "  movsx %s, %s\n", REG_STACK(reg-1), REG_STACK8(reg-1));
  }
    else
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  mov bl, [%s+ebx]\n", name);
    fprintf(out, "  movsx ebx, bl\n");
    fprintf(out, "  push ebx\n");
  }

  return 0;
}

int X86::array_read_short(const char *name, int field_id)
{
  fprintf(out, "  ; array_read_short(%s,%d)\n", name, field_id);

  if (stack == 0)
  {
    //fprintf(out, "  sal %s, 1\n", REG_STACK8(reg-1));
    fprintf(out, "  mov %s, [%s+%s*2]\n", REG_STACK8(reg-1), name, REG_STACK(reg-1));
    fprintf(out, "  movsx %s, %s\n", REG_STACK(reg-1), REG_STACK8(reg-1));
  }
    else
  {
    fprintf(out, "  pop ebx\n");
    //fprintf(out, "  sal ebx, 1\n");
    fprintf(out, "  mov bl, [%s+ebx*2]\n", name);
    fprintf(out, "  movsx ebx, bl\n");
    fprintf(out, "  push ebx\n");
  }

  return 0;
}

int X86::array_read_int(const char *name, int field_id)
{
  fprintf(out, "  ; array_read_int(%s,%d)\n", name, field_id);

  if (stack == 0)
  {
    //fprintf(out, "  sal %s, 2\n", REG_STACK8(reg-1));
    fprintf(out, "  mov %s, [%s+%s*4]\n", REG_STACK8(reg-1), name, REG_STACK(reg-1));
    fprintf(out, "  movsx %s, %s\n", REG_STACK(reg-1), REG_STACK8(reg-1));
  }
    else
  {
    fprintf(out, "  pop ebx\n");
    //fprintf(out, "  sal ebx, 2\n");
    fprintf(out, "  mov bl, [%s+ebx*4]\n", name);
    fprintf(out, "  movsx ebx, bl\n");
    fprintf(out, "  push ebx\n");
  }

  return 0;
}

int X86::array_write_byte()
{
  fprintf(out, "  ; array_write_byte()\n");

  if (stack == 0)
  {
    fprintf(out, "  mov [%s+%s], %s\n", REG_STACK(reg-3), REG_STACK(reg-2), REG_STACK8(reg-1));
    reg -= 3;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  mov [%s+%s], bl\n", REG_STACK(reg-2), REG_STACK(reg-1));
    stack -= 1;
    reg -= 2;
  }
    else
  if (stack == 2)
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  pop edi\n");
    fprintf(out, "  mov [edi+%s], bl\n", REG_STACK(reg-1));
    stack -= 2;
    reg -= 1;
  }
    else
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  pop esi\n");
    fprintf(out, "  pop edi\n");
    fprintf(out, "  add edi, esi\n");
    fprintf(out, "  mov [esi], bl\n");
    stack -= 3;
  }

  return 0;
}

int X86::array_write_short()
{
  fprintf(out, "  ; array_write_short()\n");

  if (stack == 0)
  {
    //fprintf(out, "  sal %s, 1\n", REG_STACK(reg-2));
    fprintf(out, "  mov [%s+%s*2], %s\n", REG_STACK(reg-3), REG_STACK(reg-2), REG_STACK16(reg-1));
    reg -= 3;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop ebx\n");
    //fprintf(out, "  sal %s, 1\n", REG_STACK(reg-1));
    fprintf(out, "  mov [%s+%s*2], bx\n", REG_STACK(reg-2), REG_STACK(reg-1));
    stack -= 1;
    reg -= 2;
  }
    else
  if (stack == 2)
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  pop edi\n");
    //fprintf(out, "  sal edi, 1\n");
    fprintf(out, "  mov [edi+%s*2], bx\n", REG_STACK(reg-1));
    stack -= 2;
    reg -= 1;
  }
    else
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  pop esi\n");
    fprintf(out, "  pop edi\n");
    fprintf(out, "  sal esi, 1\n");
    fprintf(out, "  add edi, esi\n");
    fprintf(out, "  mov [esi], bx\n");
    stack -= 3;
  }

  return 0;
}

int X86::array_write_int()
{
  fprintf(out, "  ; array_write_int()\n");

  if (stack == 0)
  {
    //fprintf(out, "  sal %s, 2\n", REG_STACK(reg-2));
    fprintf(out, "  mov [%s+%s*4], %s\n", REG_STACK(reg-3), REG_STACK(reg-2), REG_STACK(reg-1));
    reg -= 3;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop ebx\n");
    //fprintf(out, "  sal %s, 2\n", REG_STACK(reg-1));
    fprintf(out, "  mov [%s+%s*4], ebx\n", REG_STACK(reg-2), REG_STACK(reg-1));
    stack -= 1;
    reg -= 2;
  }
    else
  if (stack == 2)
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  pop edi\n");
    //fprintf(out, "  sal edi, 2\n");
    fprintf(out, "  mov [edi+%s*4], ebx\n", REG_STACK(reg-1));
    stack -= 2;
    reg -= 1;
  }
    else
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  pop esi\n");
    fprintf(out, "  pop edi\n");
    fprintf(out, "  sal esi, 2\n");
    fprintf(out, "  add edi, esi\n");
    fprintf(out, "  mov [esi], ebx\n");
    stack -= 3;
  }

  return 0;
}

int X86::array_write_byte(const char *name, int field_id)
{
  fprintf(out, "  ; array_write_byte(%s,%d)\n", name, field_id);

  if (stack == 0)
  {
    fprintf(out, "  mov [%s+%s], %s\n", name, REG_STACK(reg-2), REG_STACK8(reg-1));
    reg -= 2;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  mov [%s+%s], bl\n", name, REG_STACK(reg-1));
    stack -= 1;
    reg -= 1;
  }
    else
  if (stack == 2)
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  pop edi\n");
    fprintf(out, "  mov [%s+edi], bl\n", name);
    stack -= 2;
  }

  return -1;
}

int X86::array_write_short(const char *name, int field_id)
{
  fprintf(out, "  ; array_write_short(%s,%d)\n", name, field_id);

  if (stack == 0)
  {
    //fprintf(out, "  sal %s, 1\n", REG_STACK(reg-2));
    fprintf(out, "  mov [%s+%s*2], %s\n", name, REG_STACK(reg-2), REG_STACK16(reg-1));
    reg -= 2;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop ebx\n");
    //fprintf(out, "  sal %s, 1\n", REG_STACK(reg-1));
    fprintf(out, "  mov [%s+%s*2], bx\n", name, REG_STACK(reg-1));
    stack -= 1;
    reg -= 1;
  }
    else
  if (stack == 2)
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  pop edi\n");
    //fprintf(out, "  sal edi, 1\n");
    fprintf(out, "  mov [%s+edi*2], bx\n", name);
    stack -= 2;
  }

  return -1;
}

int X86::array_write_int(const char *name, int field_id)
{
  fprintf(out, "  ; array_write_int(%s,%d)\n", name, field_id);

  if (stack == 0)
  {
    //fprintf(out, "  sal %s, 2\n", REG_STACK(reg-2));
    fprintf(out, "  mov [%s+%s*4], %s\n", name, REG_STACK(reg-2), REG_STACK(reg-1));
    reg -= 2;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop ebx\n");
    //fprintf(out, "  sal %s, 2\n", REG_STACK(reg-1));
    fprintf(out, "  mov [%s+%s*4], ebx\n", name, REG_STACK(reg-1));
    stack -= 1;
    reg -= 1;
  }
    else
  if (stack == 2)
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  pop edi\n");
    //fprintf(out, "  sal edi, 2\n");
    fprintf(out, "  mov [%s+edi*4], ebx\n", name);
    stack -= 2;
  }

  return -1;
}

int X86::stack_alu(const char *instr)
{
  fprintf(out, "  ; %s\n", instr);

  if (stack == 1)
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  %s %s, ebx\n", instr, REG_STACK(reg - 1));
    stack--;
  }
    else
  if (stack > 1)
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  %s [esp], ebx\n", instr);
    stack--;
  }
    else
  {
    fprintf(out, "  %s %s, %s\n", instr, REG_STACK(reg - 2), REG_STACK(reg - 1));
    reg--;
  }

  return 0;
}

int X86::stack_alu(const char *instr, int num)
{
  fprintf(out, "  ; %s %d\n", instr, num);

  if (stack > 0)
  {
    fprintf(out, "  mov ebx, [esp]\n");
    fprintf(out, "  %s ebx, %d\n", instr, num);
    fprintf(out, "  mov [esp], ebx\n");
  }
    else
  {
    fprintf(out, "  %s %s, %d\n", instr, REG_STACK(reg - 1), num);
  }

  return 0;
}

int X86::stack_shift(const char *instr)
{
  if (stack == 1)
  {
    fprintf(out, "  mov ebx, ecx\n");
    fprintf(out, "  pop ecx\n");
    fprintf(out, "  %s %s, cl\n", instr, REG_STACK(reg - 1));
    fprintf(out, "  mov ecx, ebx\n");
    stack--;
  }
    else
  if (stack > 1)
  {
    fprintf(out, "  mov ebx, ecx\n");
    fprintf(out, "  pop ecx\n");
    fprintf(out, "  %s dword [esp], cl\n", instr);
    fprintf(out, "  mov ecx, ebx\n");
    stack--;
  }
    else
  if (reg == 2)
  {
    fprintf(out, "  %s %s, cl\n", instr, REG_STACK(reg - 2));
    reg--;
  }
    else
  if (reg == 3)
  {
    fprintf(out, "  mov ebx, ecx\n");
    fprintf(out, "  mov ecx, edx\n");
    fprintf(out, "  %s ebx, cl\n", instr);
    fprintf(out, "  mov ecx, ebx\n");
    reg--;
  }
    else
  {
    printf("Internal Error: Wrong registers on stack\n");
    return -1;
  }

  return 0;
}

int X86::stack_div(bool is_quotient)
{
  fprintf(out, "  ; div reg=%d stack=%d is_quotient=%d\n", reg, stack, is_quotient);

  if (stack == 1)
  {
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  push eax\n");
    fprintf(out, "  mov eax, edx\n");
    fprintf(out, "  xor edx, edx\n");
    fprintf(out, "  idiv ebx\n");
    if (is_quotient)
    {
      fprintf(out, "  mov edx, eax\n");
    }
      else
    {
      //fprintf(out, "  mov edx, edx\n");
    }
    fprintf(out, "  pop eax\n");
    stack--;
  }
    else
  if (stack > 1)
  {
    fprintf(out, "  pop edi\n");
    fprintf(out, "  pop ebx\n");
    fprintf(out, "  push eax\n");
    fprintf(out, "  push edx\n");
    fprintf(out, "  mov eax, ebx\n");
    fprintf(out, "  xor edx, edx\n");
    fprintf(out, "  idiv edi\n");
    if (is_quotient)
    {
      fprintf(out, "  mov ebx, eax\n");
    }
      else
    {
      fprintf(out, "  mov ebx, edx\n");
    }
    fprintf(out, "  pop edx\n");
    fprintf(out, "  pop eax\n");
    fprintf(out, "  push ebx\n");
    stack--;
  }
    else
  if (reg == 2)
  {
    fprintf(out, "  xor edx, edx\n");
    fprintf(out, "  idiv ecx\n");
    if (is_quotient == false)
    {
      fprintf(out, "  mov eax, edx\n");
    }
    reg--;
  }
    else
  if (reg == 3)
  {
    fprintf(out, "  push eax\n");
    fprintf(out, "  mov eax, ecx\n");
    fprintf(out, "  mov ebx, edx\n");
    fprintf(out, "  xor edx, edx\n");
    fprintf(out, "  idiv ebx\n");
    if (is_quotient)
    {
      fprintf(out, "  mov ecx, eax\n");
    }
      else
    {
      fprintf(out, "  mov ecx, edx\n");
    }
    fprintf(out, "  pop eax\n");
    reg--;
  }
    else
  {
    printf("Internal Error: %s:%d\n", __FILE__, __LINE__);
    return -1;
  }

  return 0;
}


