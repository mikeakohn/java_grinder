/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2023 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <typeinfo>

#include "generator/TMS9900.h"

#define LOCALS(i) (i * 2)
#define CHECK_STACK() \
  if (reg >= reg_max) { printf("Internal error: Stack blown.\n"); return -1; }

// ABI is:
// r0 temp
// r1 temp
// r2 start of stack
// ..
// ..
// r9 end of stack
// r10 Pointer to next chunk of free RAM
// r11 return address
// r12 CRU base address / point to locals
// r13 Saved WP register
// r14 Saved PC register
// r15 Saved ST register

//                                  EQ     NE      <     <=      >     >= 
static const char *cond_str[] = { "jeq", "jne", "jlt", "???", "jgt", "???" };
//                                                      rev           rev

TMS9900::TMS9900() :
  reg(0),
  reg_max(9),
  is_main(false)
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

  //fprintf(out, "heap_ptr equ free_ram\n");
  //fprintf(out, "free_ram equ ram_start+0\n");

  return 0;
}

int TMS9900::start_init()
{
  // Add any set up items (stack, registers, etc).
  //fprintf(out, ".org ???\n");
  fprintf(out, "start:\n");

  return 0;
}

int TMS9900::insert_static_field_define(std::string &name, std::string &type, int index)
{
  fprintf(out, "%s equ ram_start+%d\n", name.c_str(), (index * 2) + 2);
  return 0;
}


int TMS9900::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");
  fprintf(out, "  li r10, ram_start+%d\n", (field_count * 2) + 2);
  fprintf(out, "  li r0, ram_start+%d\n", 256 - (16 * 2));
  fprintf(out, "  mov r0, @heap_ptr\n");

  return 0;
}

int TMS9900::field_init_int(std::string &name, int index, int value)
{
  if (value < -32768 || value > 65535) { return -1; }
  fprintf(out, "  li r0, %d\n", (int8_t)value);
  fprintf(out, "  mov r0, @%s\n", name.c_str());

  return 0;
}

int TMS9900::field_init_ref(std::string &name, int index)
{
  fprintf(out, "  li r0, _%s\n", name.c_str());
  fprintf(out, "  mov r0, @%s\n", name.c_str());
  return 0;
}

void TMS9900::method_start(
  int local_count,
  int max_stack,
  int param_count,
  std::string &name)
{
  is_main = name == "main";

  fprintf(out, "%s:\n", name.c_str());

  if (is_main)
  {
    fprintf(out, "  mov r10, r12\n");
    fprintf(out, "  ai r10, %d\n", local_count * 2);
  }
    else
  {
    fprintf(out, "  mov r11, *r10+\n");
    fprintf(out, "  mov r12, *r10+\n");
    fprintf(out, "  mov r10, r12\n");
    fprintf(out, "  ai r10, %d\n", (local_count * 2));
  }
}

void TMS9900::method_end(int local_count)
{
  fprintf(out, "\n");
}

int TMS9900::push_local_var_int(int index)
{
  if (reg < reg_max)
  {
    if (index == 0)
    {
      fprintf(out, "  mov *r12, r%d  ; push local_%d\n", REG_STACK(reg), index);
    }
      else
    {
      fprintf(out, "  mov @%d(r12), r%d  ; push local_%d\n", LOCALS(index), REG_STACK(reg), index);
    }
    reg++;
  }
    else
  {
    //fprintf(out, "  push %d(r12)  ; push local_%d\n", LOCALS(index), index);
    //stack++;
    // FIXME
    printf("ERROR: Need to add stack support\n");
    return -1;
  }

  return 0;
}

int TMS9900::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int TMS9900::push_ref_static(std::string &name, int index)
{
  CHECK_STACK();

  fprintf(out, "  li r%d, _%s\n", REG_STACK(reg), name.c_str());
  reg++;

  return 0;
}

int TMS9900::push_fake()
{
  reg++;
  return 0;
}

int TMS9900::push_int(int32_t n)
{
  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);
    return -1;
  }

  uint16_t value = (n & 0xffff);

  CHECK_STACK();

  if (n == 0)
  {
    fprintf(out, "  clr r%d     ; push_int(%d)\n", REG_STACK(reg), n);
  }
    else
  {
    fprintf(out, "  li r%d, %d  ; push_int(%d)\n", REG_STACK(reg), value, n);
  }
  reg++;

  return 0;
}

#if 0
int TMS9900::push_long(int64_t n)
{
  return -1;
}

int TMS9900::push_float(float f)
{
  return -1;
}

int TMS9900::push_double(double f)
{
  return -1;
}
#endif

int TMS9900::push_ref(std::string &name, int index)
{
  CHECK_STACK();

  fprintf(out, "  mov @%s, r%d\n", name.c_str(), REG_STACK(reg));
  reg++;

  return 0;
}

int TMS9900::pop_local_var_int(int index)
{
#if 0
  if (stack > 0)
  {
    fprintf(out, "  pop %d(r12)  ; pop local_%d\n", LOCALS(index), index);
    stack--;
  }
    else
#endif
  if (reg > 0)
  {
    if (index == 0)
    {
      fprintf(out, "  mov r%d, *r12 ; pop local_%d\n", REG_STACK(reg-1), index);
    }
      else
    {
      fprintf(out, "  mov r%d, @%d(r12) ; pop local_%d\n", REG_STACK(reg-1), LOCALS(index), index);
    }
    reg--;
  }
    else
  {
    printf("Internal Error: Nothing on the stack?\n");
    return -1;
  }

  return 0;
}

int TMS9900::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int TMS9900::pop()
{
  reg--;
  return 0;
}

int TMS9900::dup()
{
  CHECK_STACK();

  //fprintf(out, "  mov r%d, r0\n", REG_STACK(reg-1));
  //fprintf(out, "  mov r0, r%d\n", REG_STACK(reg));
  fprintf(out, "  mov r%d, r%d\n", REG_STACK(reg-1), REG_STACK(reg));
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
  //fprintf(out, "  mpy r%d, r%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
  fprintf(out, "  mpy r%d, r%d\n", REG_STACK(reg-1), REG_STACK(reg-2));
  fprintf(out, "  mov r%d, r%d\n", REG_STACK(reg-1), REG_STACK(reg-2));
  //fprintf(out, "  mpy r%d, r%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
  reg--;
  return 0;
}

int TMS9900::div_integer()
{
  fprintf(out, "  mov r%d, r0\n", REG_STACK(reg-1));
  fprintf(out, "  mov r%d, r%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
  fprintf(out, "  li r%d, 0\n", REG_STACK(reg-2));
  fprintf(out, "  div r0, r%d\n", REG_STACK(reg-2));
  //fprintf(out, "  mov r%d, r%d\n", REG_STACK(reg-1), REG_STACK(reg-2));
  reg--;
  return 0;
}

int TMS9900::mod_integer()
{
  fprintf(out, "  mov r%d, r0\n", REG_STACK(reg-1));
  fprintf(out, "  mov r%d, r%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
  fprintf(out, "  li r%d, 0\n", REG_STACK(reg-2));
  fprintf(out, "  div r0, r%d\n", REG_STACK(reg-2));
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
  char instr[16];

  if (num == 1)
  {
    sprintf(instr, "inc");
  }
    else
  if (num == 2)
  {
    sprintf(instr, "inct");
  }
    else
  {
    fprintf(out, "  li r0, %d\n", num);
    sprintf(instr, "a r0,");
  }

  if (index == 0)
  {
    fprintf(out, "  %s *r12 ; inc local_%d by %d\n", instr, index, num);
  }
    else
  {
    fprintf(out, "  %s @%d(r12) ; inc local_%d by %d\n", instr, LOCALS(index), index, num);
  }
  return 0;
}

int TMS9900::integer_to_byte()
{
  sign_extend();
  return 0;
}

int TMS9900::integer_to_short()
{
  // I think we do nothing here.  On TMS9900, integers are already 16 bit.
  return 0;
}

int TMS9900::jump_cond(std::string &label, int cond, int distance)
{
  fprintf(out, "  ci r%d, 0\n", REG_STACK(reg-1));
  reg--;

#if 0
  if (cond == COND_LESS_EQUAL)
  {
    if (distance < 60)
    {
      fprintf(out, "  jlt %s\n", label);
      fprintf(out, "  jeq %s\n", label);
    }
      else
    {
      fprintf(out, "  jgt _label_%d\n", label_count);
      fprintf(out, "  b @%s\n", label);
      fprintf(out, "_label_%d:\n", label_count);
      label_count++;
    }
  }
    else
  if (cond == COND_GREATER_EQUAL)
  {
    if (distance < 60)
    {
      fprintf(out, "  jgt %s\n", label);
      fprintf(out, "  jeq %s\n", label);
    }
      else
    {
      fprintf(out, "  jlt _label_%d\n", label_count);
      fprintf(out, "  b @%s\n", label);
      fprintf(out, "_label_%d:\n", label_count);
      label_count++;
    }
  }
    else
  {
    // FIXME - Add distance reversal here too
    fprintf(out, "  %s %s\n", cond_str[cond], label);
  }
#endif

  return insert_conditional(label, cond, distance);
}

int TMS9900::jump_cond_integer(std::string &label, int cond, int distance)
{
  fprintf(out, "  c r%d, r%d\n", REG_STACK(reg-2), REG_STACK(reg-1));
  reg -= 2;

#if 0
  if (cond == COND_LESS_EQUAL)
  {
    if (distance < 60)
    {
      fprintf(out, "  jlt %s\n", label);
      fprintf(out, "  jeq %s\n", label);
    }
      else
    {
      fprintf(out, "  jgt _label_%d\n", label_count);
      fprintf(out, "  b @%s\n", label);
      fprintf(out, "_label_%d:\n", label_count);
      label_count++;
    }
  }
    else
  if (cond == COND_GREATER_EQUAL)
  {
    if (distance < 60)
    {
      fprintf(out, "  jgt %s\n", label);
      fprintf(out, "  jeq %s\n", label);
    }
      else
    {
      fprintf(out, "  jlt _label_%d\n", label_count);
      fprintf(out, "  b @%s\n", label);
      fprintf(out, "_label_%d:\n", label_count);
      label_count++;
    }
  }
    else
  {
    // FIXME - Add distance reversal here too
    fprintf(out, "  %s %s\n", cond_str[cond], label);
  }
#endif

  return insert_conditional(label, cond, distance);
}

int TMS9900::ternary(int cond, int value_true, int value_false)
{
  return -1;
}

int TMS9900::ternary(int cond, int compare, int value_true, int value_false)
{
  return -1;
}

int TMS9900::return_local(int index, int local_count)
{
  return -1;
}

int TMS9900::return_integer(int local_count)
{
  // Top of stack goes to r0
  fprintf(out, "  mov r%d, r0\n", REG_STACK(reg - 1));
  fprintf(out, "  ai r10, -%d\n", (local_count * 2) + 4);
  fprintf(out, "  mov *r10, r11\n");
  fprintf(out, "  mov @2(r10), r12\n");
  fprintf(out, "  b *r11\n"); 

  return 0;
}

int TMS9900::return_void(int local_count)
{
  fprintf(out, "  ai r10, -%d\n", (local_count * 2) + 4);
  fprintf(out, "  mov *r10, r11\n");
  fprintf(out, "  mov @2(r10), r12\n");
  fprintf(out, "  b *r11\n"); 

  return 0;
}

int TMS9900::jump(std::string &name, int distance)
{
  fprintf(out, "  ;; jump(%s,%d)\n", name.c_str(), distance);

  if (distance < 50)
  {
    fprintf(out, "  jmp %s\n", name.c_str());
  }
    else
  {
    fprintf(out, "  b @%s\n", name.c_str());
  }

  return 0;
}

int TMS9900::call(std::string &name)
{
  return -1;
}

int TMS9900::invoke_static_method(const char *name, int params, int is_void)
{
  int n;

  fprintf(out, "  ; invoke_static_method(%s,%d,%d)\n", name, params, is_void);
  fprintf(out, "  mov r11, *r10+\n");

  // Push all registers from Java stack
  for (n = REG_START; n < reg - params; n++)
  {
    fprintf(out, "  mov r%d, *r10+\n", REG_STACK(n));
  }

  // Leave a spot for the return address
  //fprintf(out, "  inct r10\n");

  // Put args into locals address space
  int local_var = 0;
  for (n = reg - params; n < reg; n++)
  {
    int offset = (local_var * 2) + 4;
    fprintf(out, "  mov r%d, @%d(r10)  ; local_%d\n", REG_STACK(n), offset, local_var);
    local_var++;
  }

  // Call function (return value in r0)
  fprintf(out, "  bl @%s\n", name);

  // Remove return address (I think method can do this easier)
  //fprintf(out, "  dect r10\n");

  // Pop all registers from Java stack
  for (n = reg - params - 1; n >= REG_START; n--)
  {
    fprintf(out, "  ai r10, -2\n");
    fprintf(out, "  mov *r10, r%d\n", REG_STACK(n));
  }

  fprintf(out, "  ai r10, -2\n");
  fprintf(out, "  mov *r10, r11\n");

  reg -= params;

  if (!is_void)
  {
    // Put r0 on the top of the stack
    fprintf(out, "  mov r0, r%d\n", REG_STACK(reg));
    reg++;
  }

  return 0;
}

int TMS9900::put_static(std::string &name, int index)
{
  fprintf(out, "  mov r%d, @%s\n", REG_STACK(reg - 1), name.c_str());
  reg--;

  return 0;
}

int TMS9900::get_static(std::string &name, int index)
{
  return -1;
}

int TMS9900::brk()
{
  return -1;
}

int TMS9900::new_array(uint8_t type)
{
  fprintf(out, "  ;; new_array(%d)\n", type);
  //fprintf(out, "  mov @heap_ptr, r0\n");

  if (type != TYPE_BYTE)
  {
    fprintf(out, "  sla r%d, 1\n", REG_STACK(reg - 1));
  }
    else
  {
    fprintf(out, "  mov r%d, r0\n", REG_STACK(reg - 1));
    fprintf(out, "  andi r0, 0x01\n");
    fprintf(out, "  a r0, r%d\n", REG_STACK(reg - 1));
  }

  fprintf(out, "  s r%d, @heap_ptr\n", REG_STACK(reg - 1));
  fprintf(out, "  mov @heap_ptr, r%d\n", REG_STACK(reg - 1));

  return 0;
}

int TMS9900::insert_array(std::string &name, int32_t *data, int len, uint8_t type)
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

int TMS9900::insert_string(std::string &name, uint8_t *bytes, int len)
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

int TMS9900::push_array_length(std::string &name, int field_id)
{
  fprintf(out, "  mov &%s, r0\n", name.c_str());
  fprintf(out, "  mov @-2(r0), r%d\n", REG_STACK(reg));
  return 0;
}

int TMS9900::array_read_byte()
{
  fprintf(out, "  ;; array_read_byte()\n");
  fprintf(out, "  a r%d, r%d\n", REG_STACK(reg-1), REG_STACK(reg-2));
  fprintf(out, "  movb *r%d, r%d\n", REG_STACK(reg-2), REG_STACK(reg-2));
  fprintf(out, "  swpb r%d\n", REG_STACK(reg-2));
  fprintf(out, "  andi r%d, 0xff\n", REG_STACK(reg-2));
  reg--;
  sign_extend();
  return 0;
}

int TMS9900::array_read_short()
{
  fprintf(out, "  ;; array_read_short()\n");
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

int TMS9900::array_read_byte(std::string &name, int field_id)
{
  fprintf(out, "  ;; array_read_byte(%s,%d)\n", name.c_str(), field_id);
  fprintf(out, "  mov @%s, r13\n", name.c_str());
  fprintf(out, "  a r%d, r13\n", REG_STACK(reg-1));
  fprintf(out, "  movb *r13, r%d\n", REG_STACK(reg-1));
  fprintf(out, "  swpb r%d\n", REG_STACK(reg-1));
  fprintf(out, "  andi r%d, 0xff\n", REG_STACK(reg-2));
  sign_extend();

  return 0;
}

int TMS9900::array_read_short(std::string &name, int field_id)
{
  fprintf(out, "  ;; array_read_short(%s,%d)\n", name.c_str(), field_id);
  fprintf(out, "  mov @%s, r13\n", name.c_str());
  fprintf(out, "  sla r%d\n", REG_STACK(reg-1));
  fprintf(out, "  a r%d, r13\n", REG_STACK(reg-1));
  fprintf(out, "  mov @r13, r%d\n", REG_STACK(reg-1));
  return 0;
}

int TMS9900::array_read_int(std::string &name, int field_id)
{
  return array_read_short(name, field_id);
}

int TMS9900::array_write_byte()
{
  fprintf(out, "  ;; array_write_byte()\n");
  fprintf(out, "  a r%d, r%d\n", REG_STACK(reg-2), REG_STACK(reg-3));
  fprintf(out, "  swpb r%d\n", REG_STACK(reg-1));
  fprintf(out, "  movb r%d, *r%d\n", REG_STACK(reg-1), REG_STACK(reg-3));
  reg -= 3;
  return 0;
}

int TMS9900::array_write_short()
{
  fprintf(out, "  ;; array_write_short()\n");
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

int TMS9900::array_write_byte(std::string &name, int field_id)
{
  fprintf(out, "  ;; array_write_byte(%s,%d)\n", name.c_str(), field_id);
  fprintf(out, "  a @%s, r%d\n", name.c_str(), REG_STACK(reg-2));
  fprintf(out, "  movb r%d, *r%d\n", REG_STACK(reg-1), REG_STACK(reg-2));
  return 0;
}

int TMS9900::array_write_short(std::string &name, int field_id)
{
  fprintf(out, "  ;; array_write_short(%s,%d)\n", name.c_str(), field_id);
  fprintf(out, "  sla r%d, 1\n", REG_STACK(reg-2));
  fprintf(out, "  a @%s, r%d\n", name.c_str(), REG_STACK(reg-2));
  fprintf(out, "  mov r%d, *r%d\n", REG_STACK(reg-1), REG_STACK(reg-2));
  return 0;
}

int TMS9900::array_write_int(std::string &name, int field_id)
{
  return array_write_short(name, field_id);
}

void TMS9900::sign_extend()
{
  fprintf(out, "  li r0, 0x80\n");
  fprintf(out, "  coc r0, r%d\n", REG_STACK(reg-1));
  fprintf(out, "  jne label_%d\n", label_count);
  fprintf(out, "  ori r%d, 0xff00\n", REG_STACK(reg-1));
  fprintf(out, "label_%d:\n", label_count);
  label_count++;
}

int TMS9900::insert_conditional(std::string &label, int cond, int distance)
{
  bool reverse_cond = false;

  if (distance > 60)
  {
    reverse_cond = true;

    switch(cond)
    {
      case COND_EQUAL: cond = COND_NOT_EQUAL; break;
      case COND_NOT_EQUAL: cond = COND_EQUAL; break;
      case COND_LESS: cond = COND_GREATER_EQUAL; break;
      case COND_LESS_EQUAL: cond = COND_GREATER; break;
      case COND_GREATER: cond = COND_LESS_EQUAL; break;
      case COND_GREATER_EQUAL: cond = COND_LESS; break;
    }
  }

  if (cond == COND_LESS_EQUAL)
  {
    if (!reverse_cond)
    {
      fprintf(out, "  jlt %s\n", label.c_str());
      fprintf(out, "  jeq %s\n", label.c_str());
    }
      else
    {
      fprintf(out, "  jlt _label_%d\n", label_count);
      fprintf(out, "  jeq _label_%d\n", label_count);
      fprintf(out, "  b @%s\n", label.c_str());
      fprintf(out, "_label_%d:\n", label_count);
      label_count++;
    }
  }
    else
  if (cond == COND_GREATER_EQUAL)
  {
    if (!reverse_cond)
    {
      fprintf(out, "  jgt %s\n", label.c_str());
      fprintf(out, "  jeq %s\n", label.c_str());
    }
      else
    {
      fprintf(out, "  jgt _label_%d\n", label_count);
      fprintf(out, "  jeq _label_%d\n", label_count);
      fprintf(out, "  b @%s\n", label.c_str());
      fprintf(out, "_label_%d:\n", label_count);
      label_count++;
    }
  }
    else
  {
    if (!reverse_cond)
    {
      fprintf(out, "  %s %s\n", cond_str[cond], label.c_str());
    }
      else
    {
      fprintf(out, "  %s _label_%d\n", cond_str[cond], label_count);
      fprintf(out, "  b @%s\n", label.c_str());
      fprintf(out, "_label_%d:\n", label_count);
      label_count++;
    }
  }

  return 0;
}

