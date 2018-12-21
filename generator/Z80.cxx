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

#include "Z80.h"

#define REG_STACK(a) (stack_regs[a])
#define LOCALS(i) (i * 4)

// ABI is:
// Using the real stack for now.  This could be slow.
//
// ix = temp?
// iy = point to locals

//                                 Z    NZ    LT    LE    GT    GE
static const char *cond_str[] = { "eq", "ne", "lt", "le", "gt", "ge" };
static const char *alu_str[] = { "add", "sbc", "and", "xor", "or" };

enum
{
  ALU_ADD = 0,
  ALU_SUB,
  ALU_AND,  // everything past here should be logic, don't change the order
  ALU_XOR,
  ALU_OR,
};

Z80::Z80() :
  stack(0),
  is_main(0),
  need_mul16_integer(0),
  need_div16_integer(0)

  //,need_memory_read8(0),
  //need_memory_write8(0),
  //need_memory_read16(0),
  //need_memory_write16(0)
  
{
}

Z80::~Z80()
{

}

int Z80::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".z80\n");

  // Set where RAM starts / ends
  // FIXME - Not sure what to set this to right now
  //fprintf(out, "ram_start equ 0\n");
  //fprintf(out, "ram_end equ 0x8000\n");

  return 0;
}

int Z80::add_functions()
{
  // Math
  if(need_mul16_integer) { insert_mul16_integer(); }
  if(need_div16_integer) { insert_div16_integer(); }
  
/*  //Memory API 
  if(need_memory_read8) { insert_memory_read8(); }
  if(need_memory_write8) { insert_memory_write8(); } 
  if(need_memory_read16) { insert_memory_read16(); }
  if(need_memory_write16) { insert_memory_write16(); } 
*/
  return 0;
}

int Z80::start_init()
{
  // Add any set up items (stack, registers, etc).
  //fprintf(out, ".org ???\n");
  fprintf(out, "start:\n");

  return 0;
}

int Z80::insert_static_field_define(const char *name, const char *type, int index)
{
  fprintf(out, "%s equ ram_start+%d\n", name, (index + 1) * 2);
  return 0;
}


int Z80::init_heap(int field_count)
{
  fprintf(out, "  ;; Set up heap and static initializers\n");
  fprintf(out, "  ld hl, ram_start+%d\n", (field_count + 1) * 2);
  fprintf(out, "  ld (heap_ptr), hl\n");
  return 0;
}

int Z80::field_init_int(std::string &name, int index, int value)
{
  if (value < -32768 || value > 65535) { return -1; }
  fprintf(out, "  ld hl, %d\n", value);
  fprintf(out, "  ld (%s), hl\n", name.c_str());
  return 0;
}

int Z80::field_init_ref(std::string &name, int index)
{
  fprintf(out, "  ld hl, _%s\n", name.c_str());
  fprintf(out, "  ld (%s), hl\n", name.c_str());
  return 0;
}

void Z80::method_start(int local_count, int max_stack, int param_count, std::string &name)
{
  stack = 0;

  is_main = (name == "main") ? 1 : 0;

  fprintf(out, "%s:\n", name.c_str());
  fprintf(out, "  ;; Save iy if needed.  iy = alloca(params * 2)\n");
  if (is_main)
  {
    fprintf(out, "  ld (save_iy),iy\n");
  }

  // FIXME - this might be extra since there's a save_iy
  fprintf(out, "  push iy\n");

  fprintf(out, "  ld iy, -%d\n", local_count * 2);
  fprintf(out, "  add iy, SP\n");
  fprintf(out, "  ld SP, iy\n");
}

void Z80::method_end(int local_count)
{
  fprintf(out, "\n");
}

int Z80::push_local_var_int(int index)
{
  fprintf(out, "  ;; push_local_var_int(%d)\n", index);
  fprintf(out, "  ld e, (iy+%d)\n", (index * 2));
  fprintf(out, "  ld d, (iy+%d)\n", (index * 2) + 1);
  fprintf(out, "  push de\n");
  stack++;
  return 0;
}

int Z80::push_local_var_ref(int index)
{
  return push_local_var_int(index);
}

int Z80::push_ref_static(std::string &name, int index)
{
  fprintf(out, "  ;; push_ref_static(%d)\n", index);
  fprintf(out, "  ld hl, _%s\n", name.c_str());
  fprintf(out, "  push hl\n");
  stack++;

  return 0;
}

int Z80::push_fake()
{
  fprintf(out, "  ; push_fake() called...\n");
  return -1;
}

int Z80::set_integer_local(int index, int value)
{
  fprintf(out, "  ;; set_integer_local(%d,%d)\n", index, value);
  //fprintf(out, "  ld de, 0x%04x\n", value);
  //fprintf(out, "  ld (iy+%d), e\n", (index * 2));
  //fprintf(out, "  ld (iy+%d), d\n", (index * 2) + 1);
  fprintf(out, "  ld (iy+%d), 0x%02x\n", (index * 2), value & 0xff);
  fprintf(out, "  ld (iy+%d), 0x%02x\n", (index * 2) + 1, (value >> 8) & 0xff);
  return 0;
}

int Z80::push_int(int32_t n)
{
  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);
    return -1;
  }

  uint16_t value = (n & 0xffff);

  fprintf(out, "  ;; push_int(%d)\n", n);
  fprintf(out, "  ld hl, 0x%04x\n", value);
  fprintf(out, "  push hl\n");
  stack++;

  return 0;
}

#if 0
int Z80::push_long(int64_t n)
{
  return -1;
}

int Z80::push_float(float f)
{
  return -1;
}

int Z80::push_double(double f)
{
  return -1;
}
#endif

int Z80::push_ref(std::string &name)
{
  fprintf(out, "  ;; push_short(%s)\n", name.c_str());
  fprintf(out, "  ld hl, (%s)\n", name.c_str());
  fprintf(out, "  push hl\n");
  stack++;

  return 0;
}

int Z80::pop_local_var_int(int index)
{
  fprintf(out, "  ;; pop_local_var_int(%d)\n", index);
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld (iy+%d), l\n", (index * 2));
  fprintf(out, "  ld (iy+%d), h\n", (index * 2) + 1);
  stack--;

  return 0;
}

int Z80::pop_local_var_ref(int index)
{
  return pop_local_var_int(index);
}

int Z80::pop()
{
  fprintf(out, "  ; pop()\n");
  fprintf(out, "  pop bc\n");
  stack--;

  return 0;
}

int Z80::dup()
{
  fprintf(out, "  ; dup()\n");
  fprintf(out, "  pop hl\n");
  fprintf(out, "  push hl\n");
  fprintf(out, "  push hl\n");
  stack++;

  return 0;
}

int Z80::dup2()
{
  fprintf(out, "  ; dup2()\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  pop de\n");
  fprintf(out, "  push bc\n");
  fprintf(out, "  push de\n");
  fprintf(out, "  push bc\n");
  fprintf(out, "  push de\n");
  stack += 2;

  return 0;
}

int Z80::swap()
{
  fprintf(out, "  ; swap()\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  pop de\n");
  fprintf(out, "  push bc\n");
  fprintf(out, "  push de\n");

  return 0;
}

int Z80::add_integer()
{
  return stack_alu(ALU_ADD);
}

int Z80::add_integer(int num)
{
  return stack_alu_const(ALU_ADD, num);
}

int Z80::sub_integer()
{
  return stack_alu(ALU_SUB);
}

int Z80::sub_integer(int num)
{
  return stack_alu_const(ALU_SUB, num);
}

int Z80::mul_integer()
{
  need_mul16_integer = 1;
  fprintf(out, "  call mul16_integer\n");
  //stack--;

  return 0;
}

int Z80::div_integer()
{
  need_div16_integer = 1;
  fprintf(out, "  call div16_integer\n");
  fprintf(out, "  pop hl\n"); // del reminder from stack, result is on stack
  //stack--;

  return 0;
}

int Z80::mod_integer()
{
  need_div16_integer = 1;
  fprintf(out, "  call div16_integer\n");
  fprintf(out, "  pop hl\n"); // get reminder
  fprintf(out, "  pop bc\n"); // del result from stack
  fprintf(out, "  push hl\n"); // store reminder
   
  
  //stack--;

  return 0;
}

int Z80::neg_integer()
{
  // OUCH :(
  fprintf(out, "  pop bc    ; neg()\n");
  fprintf(out, "  ld a,b\n");
  fprintf(out, "  cpl\n");
  fprintf(out, "  ld b,a\n");
  fprintf(out, "  ld a,c\n");
  fprintf(out, "  cpl\n");
  fprintf(out, "  ld c,a\n");
  fprintf(out, "  push bc\n");

  return 0;
}

int Z80::shift_left_integer()
{
  fprintf(out, "  pop de    ; shift_left_integer()\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  ld b,c\n");
  fprintf(out, "label_%d:\n", label_count);
  fprintf(out, "  sla e\n");
  fprintf(out, "  rl d\n");
  fprintf(out, "  djnz label_%d:\n", label_count);
  fprintf(out, "  push de\n");
  stack--;
  label_count++;

  return 0;
}

int Z80::shift_left_integer(int num)
{
int n;

  if (num == 0) { return 0; }
  fprintf(out, "  pop de    ; shift_left_integer(int)\n");
  for (n = 0; n < num; n++)
  {
    fprintf(out, "  sla e\n");
    fprintf(out, "  rl d\n");
  }
  fprintf(out, "  push de\n");

  return 0;
}

int Z80::shift_right_integer()
{
  fprintf(out, "  pop de    ; shift_right_integer()\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  ld b,c\n");
  fprintf(out, "label_%d:\n", label_count);
  fprintf(out, "  sra d\n");
  fprintf(out, "  rr e\n");
  fprintf(out, "  djnz label_%d:\n", label_count);
  fprintf(out, "  push de\n");
  stack--;
  label_count++;

  return 0;
}

int Z80::shift_right_integer(int num)
{
int n;

  if (num == 0) { return 0; }
  fprintf(out, "  pop de    ; shift_right_integer(int)\n");
  for (n = 0; n < num; n++)
  {
    fprintf(out, "  sra d\n");
    fprintf(out, "  rr e\n");
  }
  fprintf(out, "  push de\n");

  return 0;
}

int Z80::shift_right_uinteger()
{
  fprintf(out, "  pop de    ; shift_right_uinteger()\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  ld b,c\n");
  fprintf(out, "label_%d:\n", label_count);
  fprintf(out, "  srl d\n");
  fprintf(out, "  rr e\n");
  fprintf(out, "  djnz label_%d:\n", label_count);
  fprintf(out, "  push de\n");
  stack--;
  label_count++;

  return 0;
}

int Z80::shift_right_uinteger(int num)
{
int n;

  if (num == 0) { return 0; }
  fprintf(out, "  pop de    ; shift_right_uinteger(int)\n");
  for (n = 0; n < num; n++)
  {
    fprintf(out, "  srl d\n");
    fprintf(out, "  rr e\n");
  }
  fprintf(out, "  push de\n");

  return 0;
}

int Z80::and_integer()
{
  return stack_alu(ALU_AND);
}

int Z80::and_integer(int num)
{
  return stack_alu_const(ALU_AND, num);
}

int Z80::or_integer()
{
  return stack_alu(ALU_OR);
}

int Z80::or_integer(int num)
{
  return stack_alu_const(ALU_OR, num);
}

int Z80::xor_integer()
{
  return stack_alu(ALU_XOR);
}

int Z80::xor_integer(int num)
{
  return stack_alu_const(ALU_XOR, num);
}

int Z80::inc_integer(int index, int num)
{
  fprintf(out, "  ;; inc_integer(%d,%d)\n", index, num);
  fprintf(out, "  ld h, (iy+%d)\n", (index * 2) + 1);
  fprintf(out, "  ld l, (iy+%d)\n", (index * 2));
  fprintf(out, "  ld bc, %d\n", num);
  fprintf(out, "  add hl, bc\n");
  fprintf(out, "  ld (iy+%d), h\n", (index * 2) + 1);
  fprintf(out, "  ld (iy+%d), l\n", (index * 2));
  return 0;
}

int Z80::integer_to_byte()
{
  fprintf(out, "  ;; integer_to_byte() (sign extend byte)\n");
  fprintf(out, "  pop hl\n");
  fprintf(out, "  bit 7, l\n");
  fprintf(out, "  jr z, label_%d\n", label_count);
  fprintf(out, "  ld h, 0xff\n");
  fprintf(out, "label_%d:\n", label_count);
  fprintf(out, "  push hl\n");
  label_count++;
  return 0;
}

int Z80::integer_to_short()
{
  // I think we do nothing here.  On Z80, integers are already 16 bit.
  return 0;
}

int Z80::jump_cond(std::string &label, int cond, int distance)
{
  fprintf(out, "  ;; jump_cond(%s, %s)\n", label.c_str(), cond_str[cond]);

  if (cond == COND_GREATER)
  {
    fprintf(out, "  pop bc\n");
    fprintf(out, "  ld hl, 0\n");
    cond = COND_LESS;
  }
    else
  if (cond == COND_LESS_EQUAL)
  {
    fprintf(out, "  pop bc\n");
    fprintf(out, "  ld hl, 0\n");
    cond = COND_GREATER_EQUAL;
  }
    else
  {
    fprintf(out, "  ld bc, 0\n");
    fprintf(out, "  pop hl\n");
  }

  switch(cond)
  {
    case COND_EQUAL:
      fprintf(out, "  and a\n");  // clear carry
      fprintf(out, "  sbc hl, bc\n");
      fprintf(out, "  jp z, %s\n", label.c_str());
      //fprintf(out, "  cp d\n");
      //fprintf(out, "  jr nz, label_%d\n", label_count);
      //fprintf(out, "  cp e\n");
      //fprintf(out, "  jr z, %s\n", label);
      //fprintf(out, "label_%d:\n", label_count);
      //label_count++;
      break;
    case COND_NOT_EQUAL:
      fprintf(out, "  and a\n");  // clear carry
      fprintf(out, "  sbc hl, bc\n");
      fprintf(out, "  jp nz, %s\n", label.c_str());
      //fprintf(out, "  cp d\n");
      //fprintf(out, "  jr z, label_%d\n", label_count);
      //fprintf(out, "  cp e\n");
      //fprintf(out, "  jr nz, %s\n", label);
      //fprintf(out, "label_%d:\n", label_count);
      //label_count++;
      break;
    case COND_LESS:
      fprintf(out, "  and a\n");  // clear carry
      fprintf(out, "  sbc hl, bc\n");
      fprintf(out, "  push af\n");
      fprintf(out, "  pop hl\n");
      fprintf(out, "  ld a, l\n");
      //fprintf(out, "  ld a, f\n");
      fprintf(out, "  and 0x84\n");
      fprintf(out, "  jp po, %s\n", label.c_str());
      //fprintf(out, "  cp d\n");
      //fprintf(out, "  jr z %s\n", label);  // if d=0 try lower
      //fprintf(out, "  ld a, f\n");
      //fprintf(out, "  jp pe, %s\n", label_count);
      //fprintf(out, "  xor 0x80\n");
      //fprintf(out, "label_%d:\n", label_count);
      //label_count++;
      break;
    case COND_LESS_EQUAL:
      return -1;
    case COND_GREATER:
      return -1;
    case COND_GREATER_EQUAL:
      fprintf(out, "  and a\n");  // clear carry
      fprintf(out, "  sbc hl, bc\n");
      fprintf(out, "  push af\n");
      fprintf(out, "  pop hl\n");
      fprintf(out, "  ld a, l\n");
      //fprintf(out, "  ld a, f\n");
      fprintf(out, "  and 0x84\n");
      fprintf(out, "  jp pe, %s\n", label.c_str());
      break;
    default:
      return -1;
  }

  stack--;
  return 0;
}

int Z80::jump_cond_integer(std::string &label, int cond, int distance)
{
  fprintf(out, "  ;; jump_cond_integer(%s,%s)\n", label.c_str(), cond_str[cond]);
  if (cond == COND_GREATER)
  {
    fprintf(out, "  pop hl\n");
    fprintf(out, "  pop bc\n");
    cond = COND_LESS;
  }
    else
  if (cond == COND_LESS_EQUAL)
  {
    fprintf(out, "  pop hl\n");
    fprintf(out, "  pop bc\n");
    cond = COND_GREATER_EQUAL;
  }
    else
  {
    fprintf(out, "  pop bc\n");
    fprintf(out, "  pop hl\n");
  }

  switch(cond)
  {
    case COND_EQUAL:
      fprintf(out, "  and a\n");  // clear carry
      fprintf(out, "  sbc hl, bc\n");
      fprintf(out, "  jp z, %s\n", label.c_str());
      break;
    case COND_NOT_EQUAL:
      fprintf(out, "  and a\n");  // clear carry
      fprintf(out, "  sbc hl, bc\n");
      fprintf(out, "  jp nz, %s\n", label.c_str());
      label_count++;
      break;
    case COND_LESS:
      fprintf(out, "  and a\n");  // clear carry
      fprintf(out, "  sbc hl, bc\n");
      fprintf(out, "  push af\n");
      fprintf(out, "  pop hl\n");
      fprintf(out, "  ld a, l\n");
      //fprintf(out, "  ld a, f\n");
      fprintf(out, "  and 0x84\n");
      fprintf(out, "  jp po, %s\n", label.c_str());
      break;
    case COND_LESS_EQUAL:
      return -1;
    case COND_GREATER:
      return -1;
    case COND_GREATER_EQUAL:
      fprintf(out, "  and a\n");  // clear carry
      fprintf(out, "  sbc hl, bc\n");
      fprintf(out, "  push af\n");
      fprintf(out, "  pop hl\n");
      fprintf(out, "  ld a, l\n");
      //fprintf(out, "  ld a, f\n");
      fprintf(out, "  and 0x84\n");
      fprintf(out, "  jp pe, %s\n", label.c_str());
      break;
    default:
      return -1;
  }

  stack -= 2;

  return 0;
}

int Z80::ternary(int cond, int value_true, int value_false)
{
  return -1;
}

int Z80::ternary(int cond, int compare, int value_true, int value_false)
{
  return -1;
}

int Z80::return_local(int index, int local_count)
{
  fprintf(out, "  ;; return_local(%d,%d)\n", index, local_count);
  fprintf(out, "  ld e, (iy+%d)\n", (index * 2));
  fprintf(out, "  ld d, (iy+%d)\n", (index * 2) + 1);
  restore_stack(local_count);
  while (stack > 0) { fprintf(out, "  pop bc\n"); stack--; }
  if (!is_main) { fprintf(out, "  pop iy\n"); }
  fprintf(out, "  ret\n");
  return 0;
}

int Z80::return_integer(int local_count)
{
  fprintf(out, "  ;; return_integer(%d)\n", local_count);
  fprintf(out, "  pop de\n");
  stack--;
  restore_stack(local_count);
  while (stack > 0) { fprintf(out, "  pop bc\n"); stack--; }
  if (!is_main) { fprintf(out, "  pop iy\n"); }
  fprintf(out, "  ret\n");
  return 0;
}

int Z80::return_void(int local_count)
{
  fprintf(out, "  ;; return_void(%d)\n", local_count);
  restore_stack(local_count);
  while (stack > 0) { fprintf(out, "  pop bc\n"); stack--; }
  if (!is_main) { fprintf(out, "  pop iy\n"); }
  fprintf(out, "  ret\n");
  return 0;
}

int Z80::jump(std::string &name, int distance)
{
  fprintf(out, "  jp %s\n", name.c_str());
  return 0;
}

int Z80::call(std::string &name)
{
  return -1;
}

int Z80::invoke_static_method(const char *name, int params, int is_void)
{
int n;

  printf("invoke_static_method() name=%s params=%d is_void=%d\n", name, params, is_void);
  fprintf(out, "  ;; invoke_static_method(%s,%d,%d)\n", name, params, is_void);

  // Pop all params off stack
  fprintf(out, "  ld hl, -%d\n", params * 2);
  fprintf(out, "  add hl, SP\n");
  fprintf(out, "  ld SP, hl\n");

  // Copy all params to local area of new method (SP - 4 - (params * 2))
  // since we will push iy and the return address.
  fprintf(out, "  ld ix, -%d\n", (params * 2) + 4);
  fprintf(out, "  add ix, SP\n");

  for (n = 0; n < params; n++)
  {
    fprintf(out, "  ld e, (ix+%d)\n", (params * 2) + (n * 2));
    fprintf(out, "  ld (ix+%d), e\n", (n * 2));
  }

  // Params are removed from the stack
  stack -= params;

  // Make the call
  fprintf(out, "  call %s\n", name);

  if (!is_void)
  {
    fprintf(out, "  push de\n");
  }

  return 0;
}

int Z80::put_static(std::string &name, int index)
{
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld (%s), hl\n", name.c_str());
  return 0;
}

int Z80::get_static(std::string &name, int index)
{
  fprintf(out, "  ld hl, (%s)\n", name.c_str());
  fprintf(out, "  push hl\n");
  return 0;
}

int Z80::brk()
{
  return -1;
}

int Z80::new_array(uint8_t type)
{
  fprintf(out, "  ;; new_array() size=bc\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  ld hl, (heap_ptr)\n");
  fprintf(out, "  ld (hl), c\n");
  fprintf(out, "  inc hl\n");
  fprintf(out, "  ld (hl), b\n");

  if (type == TYPE_SHORT || type == TYPE_CHAR || type == TYPE_INT)
  {
    // If 2 byte size, then multiply array.length by 2 to compute new heap
    fprintf(out, "  sla c\n");
    fprintf(out, "  rlc b\n");
  }
    else
  {
    // Align heap by 2 bytes
    fprintf(out, "  bit 0, c\n");
    fprintf(out, "  jr z, label_%d\n", label_count);
    fprintf(out, "  inc bc\n");
    fprintf(out, "label_%d:\n", label_count);
    label_count++;
  }

  // Add length of allocated bytes to hl and store as new heap pointer
  fprintf(out, "  and a\n");
  fprintf(out, "  adc hl, bc\n");
  fprintf(out, "  ld (heap_ptr), hl\n");

  return 0;
}

int Z80::insert_array(std::string &name, int32_t *data, int len, uint8_t type)
{
  fprintf(out, ".align 16\n");
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

int Z80::insert_string(std::string &name, uint8_t *bytes, int len)
{
  fprintf(out, ".align 16\n");
  fprintf(out, "  dc16 %d\n", len);
  return insert_utf8(name, bytes, len);
}

int Z80::push_array_length()
{
  fprintf(out, "  pop ix\n");
  //fprintf(out, "  dec ix\n");
  //fprintf(out, "  dec ix\n");
  //fprintf(out, "  ld c, (ix)\n");
  //fprintf(out, "  ld b, (ix+1)\n");
  fprintf(out, "  ld c, (ix-2)\n");
  fprintf(out, "  ld b, (ix-1)\n");
  fprintf(out, "  push bc\n");
  return 0;
}

int Z80::push_array_length(std::string &name, int field_id)
{
  fprintf(out, "  ld hl, (%s-2)\n", name.c_str());
  fprintf(out, "  push hl\n");
  return 0;
}

int Z80::array_read_byte()
{
  fprintf(out, "  ;; array_read_byte()\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  pop hl\n");
  fprintf(out, "  and a\n");
  fprintf(out, "  adc hl, bc\n");
  fprintf(out, "  ld c, (hl)\n");
  fprintf(out, "  ld b, 0x00\n");
  fprintf(out, "  bit 7, c\n");
  fprintf(out, "  jr z, label_%d\n", label_count);
  fprintf(out, "  ld b, 0xff\n");
  fprintf(out, "label_%d:\n", label_count);
  fprintf(out, "  push bc\n");
  label_count++;
  return 0;
}

int Z80::array_read_short()
{
  fprintf(out, "  ;; array_read_short()\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  sla c\n");
  fprintf(out, "  rlc b\n");
  fprintf(out, "  pop hl\n");
  fprintf(out, "  and a\n");
  fprintf(out, "  adc hl, bc\n");
  fprintf(out, "  ld c, (hl)\n");
  fprintf(out, "  inc hl\n");
  fprintf(out, "  ld b, (hl)\n");
  fprintf(out, "  push bc\n");
  return 0;
}

int Z80::array_read_int()
{
  return array_read_short();
}

int Z80::array_read_byte(std::string &name, int field_id)
{
  fprintf(out, "  ;; array_read_byte(name,field_id);\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  ld hl, (%s)\n", name.c_str());
  fprintf(out, "  and a\n");
  fprintf(out, "  adc hl, bc\n");
  fprintf(out, "  ld c, (hl)\n");
  fprintf(out, "  ld b, 0x00\n");
  fprintf(out, "  bit 7, c\n");
  fprintf(out, "  jr z, label_%d\n", label_count);
  fprintf(out, "  ld b, 0xff\n");
  fprintf(out, "label_%d:\n", label_count);
  fprintf(out, "  push bc\n");
  label_count++;
  return 0;
}

int Z80::array_read_short(std::string &name, int field_id)
{
  fprintf(out, "  ;; array_read_short()\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  sla c\n");
  fprintf(out, "  rlc b\n");
  fprintf(out, "  ld hl, (%s)\n", name.c_str());
  fprintf(out, "  and a\n");
  fprintf(out, "  adc hl, bc\n");
  fprintf(out, "  ld c, (hl)\n");
  fprintf(out, "  inc hl\n");
  fprintf(out, "  ld b, (hl)\n");
  fprintf(out, "  push bc\n");
  return 0;
}

int Z80::array_read_int(std::string &name, int field_id)
{
  return array_read_short(name, field_id);
}

int Z80::array_write_byte()
{
  fprintf(out, "  ;; array_write_byte()\n");
  fprintf(out, "  pop de\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  pop hl\n");
  fprintf(out, "  and a\n");
  fprintf(out, "  adc hl, bc\n");
  fprintf(out, "  ld (hl), e\n");
  return 0;
}

int Z80::array_write_short()
{
  fprintf(out, "  ;; array_write_short()\n");
  fprintf(out, "  pop de\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  sla c\n");
  fprintf(out, "  rlc b\n");
  fprintf(out, "  pop hl\n");
  fprintf(out, "  and a\n");
  fprintf(out, "  adc hl, bc\n");
  fprintf(out, "  ld (hl), e\n");
  fprintf(out, "  inc hl\n");
  fprintf(out, "  ld (hl), d\n");
  return 0;
}

int Z80::array_write_int()
{
  return array_write_short();
}

int Z80::array_write_byte(std::string &name, int field_id)
{
  fprintf(out, "  ;; array_write_byte(name,field_id)\n");
  fprintf(out, "  pop de\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  ld hl, (%s)\n", name.c_str());
  fprintf(out, "  and a\n");
  fprintf(out, "  adc hl, bc\n");
  fprintf(out, "  ld (hl), e\n");
  return 0;
}

int Z80::array_write_short(std::string &name, int field_id)
{
  fprintf(out, "  ;; array_write_byte(name,field_id)\n");
  fprintf(out, "  pop de\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  ld hl, (%s)\n", name.c_str());
  fprintf(out, "  and a\n");
  fprintf(out, "  adc hl, bc\n");
  fprintf(out, "  ld (hl), e\n");
  fprintf(out, "  inc hl\n");
  fprintf(out, "  ld (hl), d\n");
  return 0;
}

int Z80::array_write_int(std::string &name, int field_id)
{
  return array_write_short(name, field_id);
}

int Z80::stack_alu(int alu_op)
{
  fprintf(out, "  ;; stack_alu(%s)\n", alu_str[alu_op]);
  if (alu_op <= ALU_SUB)
  {
#if 0
    fprintf(out, "  pop bc\n");
    fprintf(out, "  pop hl\n");
    //fprintf(out, "  scf\n");   // set carry
    //fprintf(out, "  ccf\n");   // carry = not carry
    fprintf(out, "  and a   ; clear carry\n");
    fprintf(out, "  %s hl, bc\n", alu_str[alu_op]);
    fprintf(out, "  push hl\n");
#endif
    //fprintf(out, "  pop ix\n");
    fprintf(out, "  pop hl\n");
    fprintf(out, "  pop bc\n");
    if (alu_op == ALU_SUB)
    {
      fprintf(out, "  and a   ; clear carry\n");
    }
    fprintf(out, "  %s hl, bc\n", alu_str[alu_op]);
    fprintf(out, "  push hl\n");
    stack--;
  }
    else
  {
    // Logic instruction
    fprintf(out, "  pop hl\n");
    fprintf(out, "  pop bc\n");
    fprintf(out, "  ld a, b\n");
    if (alu_op == ALU_OR || alu_op == ALU_XOR)
    {
      fprintf(out, "  %s h\n", alu_str[alu_op]);
      fprintf(out, "  ld h, a\n");
      fprintf(out, "  ld a, c\n");
      fprintf(out, "  %s l\n", alu_str[alu_op]);
      fprintf(out, "  ld l, a\n");
    }
    else
    {
      fprintf(out, "  %s a, h\n", alu_str[alu_op]);
      fprintf(out, "  ld h, a\n");
      fprintf(out, "  ld a, c\n");
      fprintf(out, "  %s a, l\n", alu_str[alu_op]);
      fprintf(out, "  ld l, a\n");
    }
    
    fprintf(out, "  push hl\n");
  }

  return 0;
}

int Z80::stack_alu_const(int alu_op, int num)
{
  if (num > 65535 || num < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", num);
    return -1;
  }

  fprintf(out, "  ;; stack_alu_const(%s, %d)\n", alu_str[alu_op], num);

  uint16_t value = (((int16_t)num) & 0xffff);

  if (value == 0 && alu_op <= ALU_SUB) { return 0; }
  if (value == 1 && alu_op == ALU_ADD)
  {
    fprintf(out, "  pop de\n");
    fprintf(out, "  inc de\n");
    fprintf(out, "  push de\n");
    return 0;
  }
  if (value == 1 && alu_op == ALU_SUB)
  {
    fprintf(out, "  pop de\n");
    fprintf(out, "  dec de\n");
    fprintf(out, "  push de\n");
    return 0;
  }

  if (alu_op <= ALU_SUB)
  {
#if 0
    fprintf(out, "  pop hl\n");
    fprintf(out, "  ld bc, 0x%04x\n", value);
    //fprintf(out, "  scf\n");   // set carry
    //fprintf(out, "  ccf\n");   // carry = not carry
    fprintf(out, "  and a   ; clear carry\n");
    fprintf(out, "  %s hl, bc\n", alu_str[alu_op]);
    fprintf(out, "  push hl\n");
#endif
    fprintf(out, "  pop hl\n");
    fprintf(out, "  ld b, 0x%02x\n", value >> 8);
    fprintf(out, "  ld c, 0x%02x\n", value & 0xff);
    if (alu_op == ALU_SUB)
    {
      fprintf(out, "  and a   ; clear carry\n");
    }
    //fprintf(out, "  %s ix, bc\n", alu_str[alu_op]);
    fprintf(out, "  %s hl, bc\n", alu_str[alu_op]);
    fprintf(out, "  push hl\n");
    return 0;
  }

  // Now we know this is a logic instruction
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld a, h\n");
  fprintf(out, "  %s 0x%02x\n", alu_str[alu_op], value >> 8);
  fprintf(out, "  ld h, a\n");
  fprintf(out, "  ld a, l\n");
  fprintf(out, "  %s 0x%02x\n", alu_str[alu_op], value & 0xff);
  fprintf(out, "  ld l, a\n");
  fprintf(out, "  push hl\n");

  return 0;
}

void Z80::restore_stack(int count)
{
  fprintf(out, "  ld iy, %d\n", count * 2);
  fprintf(out, "  add iy, SP\n");
  fprintf(out, "  ld SP, iy\n");
  fprintf(out, "  pop iy\n");
}

void Z80::save_registers()
{
  fprintf(out, "  push iy\n");
}

void Z80::restore_registers()
{
  fprintf(out, "  pop iy\n");
}

// Memory API

int Z80::memory_read8_I()
{
  fprintf(out, ";;memory_read8\n");
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld a,(hl)\n");
  fprintf(out, "  ld l,a\n");
  fprintf(out, "  ld h,0\n");
  fprintf(out, "  push hl\n");
  
  return 0;
}

int Z80::memory_write8_IB()
{
  fprintf(out, "  pop hl  ;;memory_write8\n");
  fprintf(out, "  ld a,l\n");
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld (hl),a\n");

  return 0;
}

int Z80::memory_read16_I()
{
  fprintf(out, ";;memory_read16\n");
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld c,(hl)\n");
  fprintf(out, "  inc hl\n");
  fprintf(out, "  ld b,(hl)\n");
  fprintf(out, "  push bc\n");
  
  return 0;  
}

int Z80::memory_write16_IS()
{
  /*need_memory_write16 = 1;
  fprintf(out, "  call memory_write16\n");
  return 0;*/
  fprintf(out, ";;memory_write16\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld (hl),c\n");
  fprintf(out, "  inc hl\n");
  fprintf(out, "  ld (hl),b\n");
  
  return 0;  
}

int Z80::memory_read8_I(int adr)
{
  fprintf(out, "  ld hl, 0x%02x  ;;memory_read8_I_C\n", adr);
  fprintf(out, "  ld a,(hl)\n");
  fprintf(out, "  ld l,a\n");
  fprintf(out, "  ld h,0\n");
  fprintf(out, "  push hl\n");
  
  return 0;
}

int Z80::memory_write8_IB(int adr, int8_t val)
{
#if 0
  // signed 8 bit values can't be more than 127 and it could be -128
  // this is causing a compiler warning.
  if (val > 255 || val < -127)
  {
    printf("Error: literal value %d bigger than 8 bit.\n", val);
    return -1;
  }
#endif
  
  fprintf(out, "  ld a,0x%02x  ;;memory_write8\n", val);
  fprintf(out, "  ld hl,0x%04x\n", adr);
  fprintf(out, "  ld (hl),a\n");

  return 0;
}


int Z80::memory_read16_I(int adr)
{
  fprintf(out, "  ld hl, 0x%04x  ;;memory_read16_I_C\n", adr);
  fprintf(out, "  ld c,(hl)\n");
  fprintf(out, "  inc hl\n");
  fprintf(out, "  ld b,(hl)\n");
  fprintf(out, "  push bc\n");
  
  return 0;
}

int Z80::memory_write16_IS(int adr, short val)
{
#if 0
  // signed 16 bit values can't be more than 32767 or less than -32768 
  // this is causing a compiler warning.
  if (val > 65535 || val < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", val);
    return -1;
  }
#endif
  
  fprintf(out, "  ld bc, 0x%04x  ;;memory_write16_C\n", val);
  fprintf(out, "  ld hl, 0x%04x\n", adr);
  fprintf(out, "  ld (hl),c\n");
  fprintf(out, "  inc hl\n");
  fprintf(out, "  ld (hl),b\n");
  
  return 0;  
}



// *****************************
// now the new private functions
// *****************************

void Z80::insert_mul16_integer()
{
  fprintf(out, "  ;Multiply 16-bit values (with 16-bit result)\n");
  //In: Multiply BC with DE
  //Out: HL = result
  fprintf(out, "mul16_integer:\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  pop de\n");
  fprintf(out, "  Mult16:\n");
  fprintf(out, "  ld a,b\n");
  fprintf(out, "  ld b,16\n");
  fprintf(out, "  Mult16_Loop:\n");
  fprintf(out, "  add hl,hl\n");
  fprintf(out, "  sla c\n");
  fprintf(out, "  rla\n");
  fprintf(out, "  jr nc,Mult16_NoAdd\n");
  fprintf(out, "  add hl,de\n");
  fprintf(out, "  Mult16_NoAdd:\n");
  fprintf(out, "  djnz Mult16_Loop\n");
  fprintf(out, "  push hl\n");
  fprintf(out, "  ret\n");
  
}


void Z80::insert_div16_integer()
{  
  
  fprintf(out, ";Divide 16-bit values (with 16-bit result)\n");
  //In: Divide BC by divider DE
  //Out: BC = result, HL = rest
  fprintf(out, "div16_integer:\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  pop de\n");
  fprintf(out, "  ld hl,0\n");
  fprintf(out, "  ld a,b\n");
  fprintf(out, "  ld b,8\n");
  fprintf(out, "Div16_Loop1:\n");
  fprintf(out, "  rla\n");
  fprintf(out, "  adc hl,hl\n");
  fprintf(out, "  sbc hl,de\n");
  fprintf(out, "  jr nc,Div16_NoAdd1\n");
  fprintf(out, "  add hl,de\n");
  fprintf(out, "Div16_NoAdd1:\n");
  fprintf(out, "  djnz Div16_Loop1\n");
  fprintf(out, "  ld b,a\n");
  fprintf(out, "  ld a,c\n");
  fprintf(out, "  ld c,b\n");
  fprintf(out, "  ld b,8\n");
  fprintf(out, "Div16_Loop2:\n");
  fprintf(out, "  rla\n");
  fprintf(out, "  adc hl,hl\n");
  fprintf(out, "  sbc hl,de\n");
  fprintf(out, "  jr nc,Div16_NoAdd2\n");
  fprintf(out, "  add hl,de\n");
  fprintf(out, "Div16_NoAdd2:\n");
  fprintf(out, "  djnz Div16_Loop2\n");
  fprintf(out, "  rla\n");
  fprintf(out, "  cpl\n");
  fprintf(out, "  ld b,a\n");
  fprintf(out, "  ld a,c\n");
  fprintf(out, "  ld c,b\n");
  fprintf(out, "  rla\n");
  fprintf(out, "  cpl\n");
  fprintf(out, "  ld b,a\n");
  fprintf(out, "  push bc ;save result\n");
  fprintf(out, "  push hl ;save modulo\n");
  fprintf(out, "  ret\n");
}

