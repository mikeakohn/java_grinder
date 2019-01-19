/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2019 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "generator/X86_64.h"

#define REG_STACK64(a) (registers64[a])
#define REG_STACK(a) (registers[a])
#define REG_STACK8(a) (registers8[a])
#define REG_STACK16(a) (registers16[a])
#define LOCALS(i) (((i) * 4) + 4)

// ABI is:
// Function calls:
//   0    1    2    3   4   5  .. stack
// RDI, RSI, RDX, RCX, R8, R9
//
// XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6 and XMM7 (for floats)
//
// RAX is return value (XMM0 and XMM1 for float)
// RBX, RBP, and R12–R15 must be restored by called function

#define REG_MAX 7
static const char *registers64[] = { "rax", "rcx", "rdx", "r8",  "r9",  "r10", "r11" };
static const char *registers[] =   { "eax", "ecx", "edx", "r8d", "r9d", "r10d", "r11d" };
static const char *registers8[] =  {  "al",  "cl",  "dl", "r8b", "r9b", "r10b", "r11b" };
static const char *registers16[] = {  "ax",  "cx",  "dx", "r8w", "r9w", "r10w", "r11w" };

//                                 EQ    NE     LESS  LESS-EQ GR   GR-E
static const char *cond_str[] = { "je", "jne", "jl", "jle", "jg", "jge" };

X86_64::X86_64() :
  reg(0),
  stack(0),
  is_main(0)
{

}

X86_64::~X86_64()
{

}

int X86_64::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, "BITS 64\n");

  // Set where RAM starts / ends
  // FIXME - Not sure what to set this to right now
  //fprintf(out, "ram_start equ 0\n");
  //fprintf(out, "ram_end equ 0x8000\n");

  return 0;
}

int X86_64::start_init()
{
  // Add any set up items (stack, registers, etc).
  fprintf(out, "SECTION .text\n");
  fprintf(out, "global init\n");
  fprintf(out, "init:\n");

  return 0;
}

int X86_64::insert_static_field_define(std::string &name, std::string &type, int index)
{
  fprintf(out, "  %s: resb 8  ; %s\n", name.c_str(), type.c_str());
  return 0;
}


int X86_64::init_heap(int field_count)
{
  // Don't think a heap is needed?
  return 0;
}

int X86_64::field_init_int(std::string &name, int index, int value)
{
  fprintf(out, "  mov eax, %d\n", value);
  fprintf(out, "  mov [%s], eax\n", name.c_str());
  return 0;
}

int X86_64::field_init_ref(std::string &name, int index)
{
  fprintf(out, "  mov rax, _%s\n", name.c_str());
  fprintf(out, "  mov [%s], rax\n", name.c_str());
  return 0;
}

void X86_64::method_start(int local_count, int max_stack, int param_count, std::string &name)
{
  int i;

  if (method_count == 0)
  {
    fprintf(out, "  ret\n\n");
  }

  method_count++;

  fprintf(out, "; int %s(", name.c_str());
  for (i = 0; i < param_count; i++)
  {
    if (i != 0) { fprintf(out, ", "); }
    if (i < 26) { fprintf(out, "int %c", 'a' + i); }
  }
  fprintf(out, ");\n");

  fprintf(out, "global %s\n", name.c_str());
  fprintf(out, "%s:\n", name.c_str());

  fprintf(out, "  push rbx\n");
  fprintf(out, "  push rbp\n");

  if (local_count != 0)
  {
    fprintf(out, "  ; Allocate space for %d local variables\n", local_count);
    fprintf(out, "  sub rsp, %d\n", local_count * 4);
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

void X86_64::method_end(int local_count)
{
  fprintf(out, "\n");
}

int X86_64::push_local_var_int(int index)
{
  fprintf(out, "  ; push_local_var_int(%d)\n", index);

  if (reg < REG_MAX)
  {
    fprintf(out, "  mov %s, [rbp-%d]\n", REG_STACK(reg++), LOCALS(index));
  }
    else
  {
    fprintf(out, "  mov ebx, [rbp-%d]\n", LOCALS(index));
    fprintf(out, "  push rbx\n");
    stack++;
  }

  return 0;
}

int X86_64::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int X86_64::push_ref_static(std::string &name, int index)
{
  fprintf(out, "  ; push_ref_static(%s, %d)\n", name.c_str(), index);

  if (reg < REG_MAX)
  {
    fprintf(out, "  mov %s, _%s\n", REG_STACK64(reg++), name.c_str());
  }
    else
  {
    fprintf(out, "  push _%s\n", name.c_str());
    stack++;
  }

  return 0;
}

int X86_64::push_fake()
{
  if (reg < REG_MAX)
  {
    reg++;
  }
    else
  {
    fprintf(out, "  push rax\n");
    stack++;
  }

  return 0;
}

int X86_64::push_int(int32_t n)
{
  fprintf(out, "  ; push_int(%d)\n", n);

  if (reg < REG_MAX)
  {
    if (n == 0)
    {
      fprintf(out, "  xor %s, %s\n", REG_STACK(reg), REG_STACK(reg));
      reg++;
    }
      else
    {
      fprintf(out, "  mov %s, %d\n", REG_STACK(reg++), n);
    }
  }
    else
  {
    fprintf(out, "  push %d\n", n);
    stack++;
  }

  return 0;
}

#if 0
int X86_64::push_long(int64_t n)
{
  return -1;
}

int X86_64::push_float(float f)
{
  return -1;
}

int X86_64::push_double(double f)
{
  return -1;
}
#endif

int X86_64::push_ref(std::string &name)
{
  fprintf(out, "  ; push_ref(%s)\n", name.c_str());

  if (reg < REG_MAX)
  {
    fprintf(out, "  mov %s, [%s]\n", REG_STACK(reg++), name.c_str());
  }
    else
  {
    fprintf(out, "  push [%s]\n", name.c_str());
    stack++;
  }

  return 0;
}

int X86_64::pop_local_var_int(int index)
{
  if (reg < REG_MAX)
  {
    fprintf(out, "  mov [rbp-%d], %s\n", LOCALS(index), REG_STACK(--reg));
  }
    else
  {
    fprintf(out, "  pop rbp\n");
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  mov [rbp-%d], ebx\n", LOCALS(index));
    stack--;
  }

  return 0;
}

int X86_64::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int X86_64::pop()
{
  if (stack > 0)
  {
    fprintf(out, "  add rsp, 8\n");
    stack--;
  }
    else
  {
    reg--;
  }

  return 0;
}

int X86_64::dup()
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
    fprintf(out, "  mov rbx, [rsp]\n");
    fprintf(out, "  push rbx\n");
    stack++;
  }

  return 0;
}

int X86_64::dup2()
{
  fprintf(out, "  ; dup2()\n");

  if (reg == REG_MAX - 1)
  {
    fprintf(out, "  mov %s, %s\n",  REG_STACK(reg), REG_STACK(reg - 2));
    fprintf(out, "  push %s\n", REG_STACK64(reg - 1));
    reg++;
    stack++;
  }
    else
  if (reg == REG_MAX)
  {
    fprintf(out, "  mov ebx, %s\n",  REG_STACK(reg - 1));
    fprintf(out, "  mov rdi, [rsp]\n");
    fprintf(out, "  push rbx\n");
    fprintf(out, "  push rdi\n");

    stack += 2;
  }
    else
  {
    fprintf(out, "  mov rbx, [rsp+8]\n");
    fprintf(out, "  mov rdi, [rsp]\n");
    fprintf(out, "  push rbx\n");
    fprintf(out, "  push rdi\n");

    stack += 2;
  }

  return 0;
}

int X86_64::swap()
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
    fprintf(out, "  mov rbx, [rsp]\n");
    fprintf(out, "  mov [rsp], %s\n", REG_STACK(reg - 1));
    fprintf(out, "  mov %s, rbx\n", REG_STACK(reg - 1));
  }
    else
  {
    fprintf(out, "  mov rbx, [rsp]\n");
    fprintf(out, "  mov rdi, [rsp+8]\n");
    fprintf(out, "  mov [rsp], rdi\n");
    fprintf(out, "  mov [rsp+8], rbx\n");
  }

  return 0;
}

int X86_64::add_integer()
{
  return stack_alu("add");
}

int X86_64::add_integer(int num)
{
  return stack_alu("add", num);
}

int X86_64::sub_integer()
{
  return stack_alu("sub");
}

int X86_64::sub_integer(int num)
{
  return stack_alu("sub", num);
}

int X86_64::mul_integer()
{
  return stack_alu("imul");
}

int X86_64::div_integer()
{
  return stack_div(true);
}

int X86_64::mod_integer()
{
  return stack_div(false);
}

int X86_64::neg_integer()
{
  if (stack > 0)
  {
    fprintf(out, "  neg qword [rsp]\n");
  }
    else
  {
    fprintf(out, "  neg %s\n", REG_STACK(reg - 1));
  }

  return 0;
}

int X86_64::shift_left_integer()
{
  return stack_shift("sal");
}

int X86_64::shift_left_integer(int num)
{
  return stack_alu("sal", num);
}

int X86_64::shift_right_integer()
{
  return stack_shift("sar");
}

int X86_64::shift_right_integer(int num)
{
  return stack_alu("sar", num);
}

int X86_64::shift_right_uinteger()
{
  return stack_shift("shr");
}

int X86_64::shift_right_uinteger(int num)
{
  return stack_alu("shr", num);
}

int X86_64::and_integer()
{
  return stack_alu("and");
}

int X86_64::and_integer(int num)
{
  return stack_alu("and", num);
}

int X86_64::or_integer()
{
  return stack_alu("or");
}

int X86_64::or_integer(int num)
{
  return stack_alu("or", num);
}

int X86_64::xor_integer()
{
  return stack_alu("xor");
}

int X86_64::xor_integer(int num)
{
  return stack_alu("xor", num);
}

int X86_64::inc_integer(int index, int num)
{
  fprintf(out, "  ; inc_integer(%d,%d)\n", index, num);
  fprintf(out, "  add dword [rbp-%d], %d\n", LOCALS(index), num);
  return 0;
}

int X86_64::integer_to_byte()
{
  fprintf(out, "  ; integer_to_byte() (sign extend)\n");

  if (stack > 0)
  {
    fprintf(out, "  mov rbx, [rsp]\n");
    fprintf(out, "  movsx ebx, bl\n");
    fprintf(out, "  mov [rsp], rbx\n");
  }
    else
  {
    fprintf(out, "  movsx %s, %s\n", REG_STACK(reg-1), REG_STACK8(reg-1));
  }

  return 0;
}

int X86_64::integer_to_short()
{
  fprintf(out, "  ; integer_to_short() (sign extend)\n");

  if (stack > 0)
  {
    fprintf(out, "  mov rsi, [rsp]\n");
    fprintf(out, "  movsx esi, si\n");
    fprintf(out, "  mov [rsp], rsi\n");
  }
    else
  {
    fprintf(out, "  movsx %s, %s\n", REG_STACK(reg-1), REG_STACK16(reg-1));
  }

  return 0;
}

int X86_64::jump_cond(std::string &label, int cond, int distance)
{
  fprintf(out, "  ; jump_cond(%s, %d, %d)\n", label.c_str(), cond, distance);

  if (stack > 0)
  {
    fprintf(out, "  pop rsi\n");
    fprintf(out, "  cmp esi, 0\n");
    stack--;
  }
    else
  {
    fprintf(out, "  cmp %s, 0\n", REG_STACK(--reg));
  }

  fprintf(out, "  %s %s\n", cond_str[cond], label.c_str());

  return 0;
}

int X86_64::jump_cond_integer(std::string &label, int cond, int distance)
{
  fprintf(out, "  ; jump_cond_integer(%s, %d, %d)\n", label.c_str(), cond, distance);

  if (stack == 1)
  {
    fprintf(out, "  pop rsi\n");
    fprintf(out, "  cmp %s, esi\n", REG_STACK(--reg));
    stack--;
  }
    else
  if (stack > 1)
  {
    fprintf(out, "  pop rsi\n");
    fprintf(out, "  pop rdi\n");
    fprintf(out, "  cmp edi, esi\n");
    stack -= 2;
  }
    else
  {
    fprintf(out, "  cmp %s, %s\n", REG_STACK(reg - 2), REG_STACK(reg - 1));
    reg -= 2;
  }

  fprintf(out, "  %s %s\n", cond_str[cond], label.c_str());

  return 0;
}

int X86_64::ternary(int cond, int value_true, int value_false)
{
  return -1;
}

int X86_64::ternary(int cond, int compare, int value_true, int value_false)
{
  return -1;
}

int X86_64::return_local(int index, int local_count)
{
  return -1;
}

int X86_64::return_integer(int local_count)
{
  if (local_count != 0)
  {
    fprintf(out, "  ; Free stack space for %d local variables\n", local_count);
    fprintf(out, "  add rsp, %d\n", local_count * 4);
  }

  fprintf(out, "  pop rbp\n");
  fprintf(out, "  pop rbx\n");

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

int X86_64::return_void(int local_count)
{
  if (local_count != 0)
  {
    fprintf(out, "  ; Free stack space for %d local variables\n", local_count);
    fprintf(out, "  add rsp, %d\n", local_count * 4);
  }

  fprintf(out, "  pop rbp\n");
  fprintf(out, "  pop rbx\n");
  fprintf(out, "  ret\n");

  return 0;
}

int X86_64::jump(std::string &name, int distance)
{
  fprintf(out, "  jmp %s\n", name.c_str());

  return 0;
}

int X86_64::call(std::string &name)
{
  return -1;
}

int X86_64::invoke_static_method(const char *name, int params, int is_void)
{
  //bool push_rbx = false;
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
      fprintf(out, "  push %s\n", REG_STACK64(n));
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
      fprintf(out, "  mov ebx, [rsp+%d]\n", distance);
      fprintf(out, "  push rbx\n");
    }

    fprintf(out, "  ; %d params are in registers\n", reg - saved_register_count);
    for (n = reg; n > saved_register_count; n--)
    {
      fprintf(out, "  push %s\n", REG_STACK64(n - 1));
    }
  }

  fprintf(out, "  call %s\n", name);

  if (params != 0)
  {
    fprintf(out, "  ; pop %d params off the stack\n", params);
    fprintf(out, "  add rsp, %d\n", (params * 4) + (stack_params * 4));

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
    fprintf(out, "  pop %s\n", REG_STACK64(n));
  }

#if 0
  if (push_rbx)
  {
    fprintf(out, "  push rbx\n");
    stack++;
  }
#endif

  return 0;
}

int X86_64::put_static(std::string &name, int index)
{
  printf("  ; put_static(%s index=%d)\n", name.c_str(), index);

  if (stack > 0)
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  mov [%s], ebx\n", name.c_str());
    stack--;
  }
    else
  {
    fprintf(out, "  mov [%s], %s\n", name.c_str(), REG_STACK(reg - 1));
    reg--;
  }

  return 0;
}

int X86_64::get_static(std::string &name, int index)
{
  printf("  ; get_static(%s index=%d)\n", name.c_str(), index);

  if (reg < REG_MAX)
  {
    fprintf(out, "  mov %s, [%s]\n", REG_STACK(reg++), name.c_str());
  }
    else
  {
    fprintf(out, "  push [%s]\n", name.c_str());
    stack++;
  }

  return 0;
}

int X86_64::brk()
{
  return -1;
}

int X86_64::new_array(uint8_t type)
{
  return -1;
}

int X86_64::insert_array(std::string &name, int32_t *data, int len, uint8_t type)
{
  fprintf(out, ".align 32\n");

  if (type == TYPE_BYTE)
  {
    return insert_db(name, data, len, TYPE_INT);
  }
    else
  if (type == TYPE_SHORT)
  {
    return insert_dw(name, data, len, TYPE_INT);
  }
    else
  if (type == TYPE_INT)
  {
    return insert_dc32(name, data, len, TYPE_INT);
  }

  return -1;
}

int X86_64::insert_string(std::string &name, uint8_t *bytes, int len)
{
  fprintf(out, ".align 32\n");
  fprintf(out, "  dc32 %d\n", len);
  return insert_utf8(name, bytes, len);
}

int X86_64::push_array_length()
{
  if (stack > 0)
  {
    fprintf(out, "  pop rsi\n");
    fprintf(out, "  push [rsi-4]\n");
  }
    else
  {
    fprintf(out, "  mov %s, [%s-8]\n", REG_STACK(reg - 1), REG_STACK64(reg - 1));
  }

  return 0;
}

int X86_64::push_array_length(std::string &name, int field_id)
{
  if (reg < REG_MAX)
  {
    fprintf(out, "  mov %s, [%s-8]\n", REG_STACK(reg++), name.c_str());
  }
    else
  {
    fprintf(out, "  push [%s-8]\n", name.c_str());
    stack++;
  }

  return 0;
}

int X86_64::array_read_byte()
{
  fprintf(out, "  ; array_read_byte()\n");

  if (stack == 0)
  {
    fprintf(out, "  mov %s, [%s+%s]\n",
      REG_STACK8(reg - 2),
      REG_STACK64(reg - 2),
      REG_STACK64(reg - 1));
    reg--;

    fprintf(out, "  movsx %s, %s\n", REG_STACK(reg - 1), REG_STACK8(reg - 1));
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  mov %s, [%s+rbx]\n",
      REG_STACK8(reg - 1),
      REG_STACK64(reg - 1));
    fprintf(out, "  movsx %s, %s\n", REG_STACK(reg - 1), REG_STACK8(reg - 1));
    stack--;
  }
    else
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  pop rsi\n");
    fprintf(out, "  mov bl, [rsi+rbx]\n");
    fprintf(out, "  movsx rbx, bl\n");
    fprintf(out, "  push rbx\n");
    stack--;
  }

  return 0;
}

int X86_64::array_read_short()
{
  fprintf(out, "  ; array_read_short()\n");

  if (stack == 0)
  {
    fprintf(out, "  mov %s, [%s+%s*2]\n",
      REG_STACK16(reg - 2),
      REG_STACK64(reg - 2),
      REG_STACK64(reg - 1));
    reg--;

    fprintf(out, "  movsx %s, %s\n", REG_STACK(reg - 1), REG_STACK16(reg - 1));
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  mov %s, [%s+rbx*2]\n",
      REG_STACK16(reg - 1),
      REG_STACK64(reg - 1));
    fprintf(out, "  movsx %s, %s\n", REG_STACK(reg - 1), REG_STACK16(reg - 1));
    stack--;
  }
    else
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  pop rsi\n");
    fprintf(out, "  mov bx, [lsi+rbx*2]\n");
    fprintf(out, "  movsx ebx, bx\n");
    fprintf(out, "  push rbx\n");
    stack--;
  }

  return 0;
}

int X86_64::array_read_int()
{
  fprintf(out, "  ; array_read_int()\n");

  if (stack == 0)
  {
    fprintf(out, "  mov %s, [%s+%s*4]\n",
      REG_STACK(reg - 2),
      REG_STACK64(reg - 2),
      REG_STACK64(reg - 1));
    reg--;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  mov %s, [%s+rbx*4]\n",
      REG_STACK(reg - 1),
      REG_STACK64(reg - 1));
    stack--;
  }
    else
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  pop rsi\n");
    fprintf(out, "  mov ebx, [rsi+rbx*4]\n");
    fprintf(out, "  push rbx\n");
    stack--;
  }

  return 0;
}

int X86_64::array_read_byte(std::string &name, int field_id)
{
  fprintf(out, "  ; array_read_byte(%s,%d)\n", name.c_str(), field_id);

  if (stack == 0)
  {
    fprintf(out, "  mov %s, [%s+%s]\n",
      REG_STACK8(reg - 1),
      name.c_str(),
      REG_STACK64(reg - 1));
    fprintf(out, "  movsx %s, %s\n", REG_STACK(reg - 1), REG_STACK8(reg - 1));
  }
    else
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  mov bl, [%s+rbx]\n", name.c_str());
    fprintf(out, "  movsx ebx, bl\n");
    fprintf(out, "  push rbx\n");
  }

  return 0;
}

int X86_64::array_read_short(std::string &name, int field_id)
{
  fprintf(out, "  ; array_read_short(%s,%d)\n", name.c_str(), field_id);

  if (stack == 0)
  {
    fprintf(out, "  mov %s, [%s+%s*2]\n",
      REG_STACK8(reg - 1),
      name.c_str(),
      REG_STACK64(reg - 1));
    fprintf(out, "  movsx %s, %s\n", REG_STACK(reg - 1), REG_STACK8(reg - 1));
  }
    else
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  mov bl, [%s+rbx*2]\n", name.c_str());
    fprintf(out, "  movsx ebx, bl\n");
    fprintf(out, "  push rbx\n");
  }

  return 0;
}

int X86_64::array_read_int(std::string &name, int field_id)
{
  fprintf(out, "  ; array_read_int(%s,%d)\n", name.c_str(), field_id);

  if (stack == 0)
  {
    fprintf(out, "  mov %s, [%s+%s*4]\n",
      REG_STACK8(reg - 1),
      name.c_str(),
      REG_STACK64(reg - 1));
    fprintf(out, "  movsx %s, %s\n", REG_STACK(reg - 1), REG_STACK8(reg - 1));
  }
    else
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  mov bl, [%s+rbx*4]\n", name.c_str());
    fprintf(out, "  movsx ebx, bl\n");
    fprintf(out, "  push rbx\n");
  }

  return 0;
}

int X86_64::array_write_byte()
{
  fprintf(out, "  ; array_write_byte()\n");

  if (stack == 0)
  {
    fprintf(out, "  mov [%s+%s], %s\n",
      REG_STACK64(reg - 3),
      REG_STACK64(reg - 2),
      REG_STACK8(reg - 1));
    reg -= 3;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  mov [%s+%s], bl\n",
      REG_STACK64(reg - 2),
      REG_STACK64(reg - 1));
    stack -= 1;
    reg -= 2;
  }
    else
  if (stack == 2)
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  pop rdi\n");
    fprintf(out, "  mov [rdi+%s], bl\n", REG_STACK64(reg - 1));
    stack -= 2;
    reg -= 1;
  }
    else
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  pop rsi\n");
    fprintf(out, "  pop rdi\n");
    fprintf(out, "  add rdi, rsi\n");
    fprintf(out, "  mov [rsi], bl\n");
    stack -= 3;
  }

  return 0;

}

int X86_64::array_write_short()
{
  fprintf(out, "  ; array_write_short()\n");

  if (stack == 0)
  {
    fprintf(out, "  mov [%s+%s*2], %s\n",
      REG_STACK64(reg - 3),
      REG_STACK64(reg - 2),
      REG_STACK16(reg - 1));
    reg -= 3;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  mov [%s+%s*2], bx\n",
      REG_STACK64(reg - 2),
      REG_STACK64(reg - 1));
    stack -= 1;
    reg -= 2;
  }
    else
  if (stack == 2)
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  pop rdi\n");
    fprintf(out, "  mov [rdi+%s*2], bx\n", REG_STACK(reg - 1));
    stack -= 2;
    reg -= 1;
  }
    else
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  pop rsi\n");
    fprintf(out, "  pop rdi\n");
    fprintf(out, "  sal rsi, 1\n");
    fprintf(out, "  add rdi, rsi\n");
    fprintf(out, "  mov [rsi], bx\n");
    stack -= 3;
  }

  return 0;
}

int X86_64::array_write_int()
{
  fprintf(out, "  ; array_write_int()\n");

  if (stack == 0)
  {
    fprintf(out, "  mov [%s+%s*4], %s\n",
      REG_STACK64(reg - 3),
      REG_STACK64(reg - 2),
      REG_STACK(reg - 1));
    reg -= 3;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  mov [%s+%s*4], rbx\n",
      REG_STACK64(reg - 2),
      REG_STACK64(reg - 1));
    stack -= 1;
    reg -= 2;
  }
    else
  if (stack == 2)
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  pop rdi\n");
    fprintf(out, "  mov [rdi+%s*4], ebx\n", REG_STACK(reg - 1));
    stack -= 2;
    reg -= 1;
  }
    else
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  pop rsi\n");
    fprintf(out, "  pop rdi\n");
    fprintf(out, "  sal rsi, 2\n");
    fprintf(out, "  add rdi, rsi\n");
    fprintf(out, "  mov [rsi], ebx\n");
    stack -= 3;
  }

  return 0;
}

int X86_64::array_write_byte(std::string &name, int field_id)
{
  fprintf(out, "  ; array_write_byte(%s,%d)\n", name.c_str(), field_id);

  if (stack == 0)
  {
    fprintf(out, "  mov rdi, %s\n", name.c_str());
    fprintf(out, "  mov [rdi+%s], %s\n",
      REG_STACK64(reg - 2),
      REG_STACK8(reg - 1));
    reg -= 2;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  mov rdi, %s\n", name.c_str());
    fprintf(out, "  mov [rdi+%s], bl\n", REG_STACK64(reg - 1));
    stack -= 1;
    reg -= 1;
  }
    else
  if (stack == 2)
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  pop rdi\n");
    fprintf(out, "  add rdi, %s\n", name.c_str());
    fprintf(out, "  mov [rdi], bl\n");
    stack -= 2;
  }

  return 0;
}

int X86_64::array_write_short(std::string &name, int field_id)
{
  fprintf(out, "  ; array_write_short(%s,%d)\n", name.c_str(), field_id);

  if (stack == 0)
  {
    fprintf(out, "  mov rdi, %s\n", name.c_str());
    fprintf(out, "  mov [rdi+%s*2], %s\n",
      REG_STACK64(reg - 2),
      REG_STACK16(reg - 1));
    reg -= 2;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  mov rdi, %s\n", name.c_str());
    fprintf(out, "  sal %s, 1\n", REG_STACK64(reg - 1));
    fprintf(out, "  mov [rdi+%s], bx\n", REG_STACK64(reg - 1));
    stack -= 1;
    reg -= 1;
  }
    else
  if (stack == 2)
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  pop rdi\n");
    fprintf(out, "  sal %s, 1\n", REG_STACK64(reg - 1));
    fprintf(out, "  add rdi, %s\n", name.c_str());
    fprintf(out, "  mov [rdi], bx\n");
    stack -= 2;
  }

  return 0;
}

int X86_64::array_write_int(std::string &name, int field_id)
{
  fprintf(out, "  ; array_write_int(%s,%d)\n", name.c_str(), field_id);

  if (stack == 0)
  {
    fprintf(out, "  mov rdi, %s\n", name.c_str());
    fprintf(out, "  mov [rdi+%s*4], %s\n",
      REG_STACK64(reg - 2),
      REG_STACK(reg - 1));
    reg -= 2;
  }
    else
  if (stack == 1)
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  mov rdi, %s\n", name.c_str());
    fprintf(out, "  sal %s, 2\n", REG_STACK64(reg - 1));
    fprintf(out, "  mov [rdi+%s], ebx\n", REG_STACK64(reg - 1));
    stack -= 1;
    reg -= 1;
  }
    else
  if (stack == 2)
  {
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  pop rdi\n");
    fprintf(out, "  sal %s, 1\n", REG_STACK64(reg - 1));
    fprintf(out, "  add rdi, %s\n", name.c_str());
    fprintf(out, "  mov [rdi], bx\n");
    stack -= 2;
  }

  return 0;
}

int X86_64::stack_alu(const char *instr)
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

int X86_64::stack_alu(const char *instr, int num)
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

int X86_64::stack_shift(const char *instr)
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

int X86_64::stack_div(bool is_quotient)
{
  fprintf(out, "  ; div reg=%d stack=%d is_quotient=%d\n", reg, stack, is_quotient);

  if (stack == 1)
  {
    // This should be pretty rare.
    fprintf(out, "  pop rbx\n");
    fprintf(out, "  push rax\n");
    fprintf(out, "  push rdx\n");

    fprintf(out, "  mov eax, %s\n", REG_STACK(reg - 1));
    fprintf(out, "  xor edx, edx\n");
    fprintf(out, "  idiv ebx\n");

    if (is_quotient)
    {
      fprintf(out, "  mov %s, eax\n", REG_STACK(reg - 1));
    }
      else
    {
      fprintf(out, "  mov %s, edx\n", REG_STACK(reg - 1));
    }

    fprintf(out, "  pop rdx\n");
    fprintf(out, "  pop rax\n");
    stack--;
  }
    else
  if (stack > 1)
  {
    // This should be pretty rare.
    fprintf(out, "  push rax\n");
    fprintf(out, "  push rdx\n");
    fprintf(out, "  mov eax, [rsp+24]\n");
    fprintf(out, "  mov ebx, [rsp+16]\n");
    fprintf(out, "  xor edx, edx\n");

    fprintf(out, "  idiv ebx\n");

    if (is_quotient)
    {
      fprintf(out, "  mov [rsp+24], rax\n");
    }
      else
    {
      fprintf(out, "  mov [rsp+24], rdx\n");
    }

    fprintf(out, "  pop rdx\n");
    fprintf(out, "  pop rax\n");
    fprintf(out, "  pop rbx\n");
    stack--;
  }
    else
  {
    if (reg > 2)
    {
      fprintf(out, "  push rdx\n");
      fprintf(out, "  mov ebx, eax\n");
      fprintf(out, "  mov eax, %s\n", REG_STACK(reg - 2));
      fprintf(out, "  xor edx, edx\n");
      fprintf(out, "  idiv %s\n", REG_STACK(reg - 1));
    }
    else
    {
      fprintf(out, "  mov ebx, edx\n");
      fprintf(out, "  xor edx, edx\n");
      fprintf(out, "  idiv ebx\n");
    }

    if (is_quotient)
    {
      if (reg > 2)
      {
        fprintf(out, "  mov %s, eax\n", REG_STACK(reg - 2));
      }
    }
    else
    {
      fprintf(out, "  mov %s, edx\n", REG_STACK(reg - 2));
    }

    if (reg > 2)
    {
      fprintf(out, "  pop rdx\n");
      fprintf(out, "  mov eax, ebx\n");
    }

    reg--;
  }

  return 0;
}

