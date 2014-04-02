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
//static const char *alu_str[] = { "adc", "sbc", "and", "xor", "or" };
static const char *alu_str[] = { "add", "sub", "and", "xor", "or" };

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
  is_main(0)
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

int Z80::start_init()
{
  // Add any set up items (stack, registers, etc).
  //fprintf(out, ".org ???\n");
  fprintf(out, "start:\n");

  return 0;
}

int Z80::insert_static_field_define(const char *name, const char *type, int index)
{
  //fprintf(out, "%s equ ram_start+%d\n", name, (index + 1) * 4);
  return 0;
}


int Z80::init_heap(int field_count)
{
  return -1;
}

int Z80::insert_field_init_boolean(char *name, int index, int value)
{
  return -1;
}

int Z80::insert_field_init_byte(char *name, int index, int value)
{
  return -1;
}

int Z80::insert_field_init_short(char *name, int index, int value)
{
  return -1;
}

int Z80::insert_field_init_int(char *name, int index, int value)
{
  return -1;
}

int Z80::insert_field_init(char *name, int index)
{
  return -1;
}

void Z80::method_start(int local_count, int max_stack, int param_count, const char *name)
{
  stack = 0;

  is_main = (strcmp(name, "main") == 0) ? 1 : 0;

  fprintf(out, "%s:\n", name);
  if (!is_main) { fprintf(out, "  push iy\n"); }
  fprintf(out, "  mov iy, SP\n");
  fprintf(out, "  ld ix, -%d\n", local_count * 2);
  fprintf(out, "  add ix, SP\n");
  fprintf(out, "  ld SP, ix\n");
}

void Z80::method_end(int local_count)
{
  fprintf(out, "\n");
}

int Z80::push_integer(int32_t n)
{
  if (n > 65535 || n < -32768)
  {
    printf("Error: literal value %d bigger than 16 bit.\n", n);
    return -1;
  }

  uint16_t value = (n & 0xffff);

  fprintf(out, "  ;; push_integer(%d)\n", n);
  fprintf(out, "  ld bc, 0x%04x\n", value);
  fprintf(out, "  push bc\n");
  stack++;

  return 0;
}

int Z80::push_integer_local(int index)
{
  fprintf(out, "  ;; push_integer_local(%d)\n", index);
  fprintf(out, "  ld e, (iy+%d)\n", (index * 2));
  fprintf(out, "  ld d, (iy+%d)\n", (index * 2) + 1);
  fprintf(out, "  push de\n");
  stack++;
  return 0;
}

int Z80::push_ref_local(int index)
{
  return push_integer_local(index);
}

int Z80::push_fake()
{
  fprintf(out, "  ; push_fake() called...\n");
  return -1;
}

int Z80::set_integer_local(int index, int value)
{
  fprintf(out, "  ;; set_integer_local(%d,%d)\n", index, value);
  fprintf(out, "  ld de, 0x%04x\n", value);
  fprintf(out, "  ld (iy+%d), e\n", (index * 2));
  fprintf(out, "  ld (iy+%d), d\n", (index * 2) + 1);
  return 0;
}


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

int Z80::push_byte(int8_t b)
{
  uint16_t value = (((int16_t)b) & 0xffff);

  fprintf(out, "  ;; push_byte(%d)\n", b);
  fprintf(out, "  ld bc, 0x%04x\n", value);
  fprintf(out, "  push bc\n");
  stack++;

  return 0;
}

int Z80::push_short(int16_t s)
{
  fprintf(out, "  ;; push_short(%d)\n", s);
  fprintf(out, "  ld bc, 0x%04x\n", s);
  fprintf(out, "  push bc\n");
  stack++;

  return 0;
}

int Z80::pop_integer_local(int index)
{
  fprintf(out, "  ;; pop_integer_local(%d)\n", index);
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld l, (iy+%d)\n", (index * 2));
  fprintf(out, "  ld h, (iy+%d)\n", (index * 2) + 1);
  return 0;
}

int Z80::pop_ref_local(int index)
{
  return pop_integer_local(index);
}

int Z80::pop()
{
  fprintf(out, "  pop bc    ; pop()\n");
  stack--;

  return 0;
}

int Z80::dup()
{
  fprintf(out, "  pop bc     ; dup()\n");
  fprintf(out, "  push bc\n");
  fprintf(out, "  push bc\n");
  stack++;

  return 0;
}

int Z80::dup2()
{
  fprintf(out, "  pop bc    ; dup2()\n");
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
  fprintf(out, "  pop bc    ; swap()\n");
  fprintf(out, "  pop de\n");
  fprintf(out, "  push de\n");
  fprintf(out, "  push bc\n");
  stack++;

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
  return -1;
}

int Z80::div_integer()
{
  return -1;
}

int Z80::mod_integer()
{
  return -1;
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
  fprintf(out, "  ld bc, 0x%02x\n", num);
  fprintf(out, "  add hl, bc\n");
  fprintf(out, "  ld (iy+%d), h\n", (index * 2) + 1);
  fprintf(out, "  ld (iy+%d), l\n", (index * 2));
  return 0;
}

int Z80::integer_to_byte()
{
  return -1;
}

int Z80::jump_cond(const char *label, int cond)
{
  fprintf(out, "  ;; jump_cond(%s, %s)\n", label, cond_str[cond]);
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
      fprintf(out, "  jr z, %s\n", label);
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
      fprintf(out, "  jr nz, %s\n", label);
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
      fprintf(out, "  ld a, f\n");
      fprintf(out, "  and 0x84\n");
      fprintf(out, "  jp po, %s\n", label);
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
      fprintf(out, "  ld a, f\n");
      fprintf(out, "  and 0x84\n");
      fprintf(out, "  jp pe, %s\n", label);
      break;
    default:
      return -1;
  }

  stack--;
  return 0;
}

int Z80::jump_cond_integer(const char *label, int cond)
{
  fprintf(out, "  ;; jump_cond_integer(%s,%s)\n", label, cond_str[cond]);
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
      fprintf(out, "  jr z, %s\n", label);
      break;
    case COND_NOT_EQUAL:
      fprintf(out, "  and a\n");  // clear carry
      fprintf(out, "  sbc hl, bc\n");
      fprintf(out, "  jr nz, %s\n", label);
      label_count++;
      break;
    case COND_LESS:
      fprintf(out, "  and a\n");  // clear carry
      fprintf(out, "  sbc hl, bc\n");
      fprintf(out, "  ld a, f\n");
      fprintf(out, "  and 0x84\n");
      fprintf(out, "  jp po, %s\n", label);
      break;
    case COND_LESS_EQUAL:
      return -1;
    case COND_GREATER:
      return -1;
    case COND_GREATER_EQUAL:
      fprintf(out, "  and a\n");  // clear carry
      fprintf(out, "  sbc hl, bc\n");
      fprintf(out, "  ld a, f\n");
      fprintf(out, "  and 0x84\n");
      fprintf(out, "  jp pe, %s\n", label);
      break;
    default:
      return -1;
  }

  stack--;
  return 0;
}

int Z80::return_local(int index, int local_count)
{
  fprintf(out, "  ;; return_local(%d,%d)\n", index, local_count);
  fprintf(out, "  ld e, (iy+%d)\n", (index * 2));
  fprintf(out, "  ld d, (iy+%d)\n", (index * 2) + 1);
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
  while (stack > 0) { fprintf(out, "  pop bc\n"); stack--; }
  if (!is_main) { fprintf(out, "  pop iy\n"); }
  fprintf(out, "  ret\n");
  return 0;
}

int Z80::return_void(int local_count)
{
  fprintf(out, "  ;; return_void(%d)\n", local_count);
  while (stack > 0) { fprintf(out, "  pop bc\n"); stack--; }
  if (!is_main) { fprintf(out, "  pop iy\n"); }
  fprintf(out, "  ret\n");
  return 0;
}

int Z80::jump(const char *name)
{
  fprintf(out, "  jmp %s\n", name);
  return 0;
}

int Z80::call(const char *name)
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
    fprintf(out, "  ld e, (ix+%d)", (params * 2) + (n * 2));
    fprintf(out, "  ld (ix+%d), e", (n * 2));
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

int Z80::put_static(const char *name, int index)
{
  return -1;
}

int Z80::get_static(const char *name, int index)
{
  return -1;
}

int Z80::brk()
{
  return -1;
}

int Z80::new_array(uint8_t type)
{
  return -1;
}

int Z80::insert_array(const char *name, int32_t *data, int len, uint8_t type)
{
  fprintf(out, ".align 16\n");
  if (type == TYPE_BYTE)
  { return insert_db(name, data, len, TYPE_INT); }
    else
  if (type == TYPE_SHORT)
  { return insert_dw(name, data, len, TYPE_INT); } 
    else
  if (type == TYPE_INT)
  { return insert_dc32(name, data, len, TYPE_INT); } 

  return -1;
}

int Z80::insert_string(const char *name, uint8_t *bytes, int len)
{
  fprintf(out, ".align 16\n");
  fprintf(out, "  dc16 %d\n", len);
  return insert_utf8(name, bytes, len);
}

int Z80::push_array_length()
{
  return -1;
}

int Z80::push_array_length(const char *name, int field_id)
{
  return -1;
}

int Z80::array_read_byte()
{
  return -1;
}

int Z80::array_read_short()
{
  return -1;
}

int Z80::array_read_int()
{
  return -1;
}

int Z80::array_read_byte(const char *name, int field_id)
{
  return -1;
}

int Z80::array_read_short(const char *name, int field_id)
{
  return -1;
}

int Z80::array_read_int(const char *name, int field_id)
{
  return -1;
}

int Z80::array_write_byte()
{
  return -1;
}

int Z80::array_write_short()
{
  return -1;
}

int Z80::array_write_int()
{
  return -1;
}

int Z80::array_write_byte(const char *name, int field_id)
{
  return -1;
}

int Z80::array_write_short(const char *name, int field_id)
{
  return -1;
}

int Z80::array_write_int(const char *name, int field_id)
{
  return -1;
}

int Z80::stack_alu(int alu_op)
{
  fprintf(out, "  stack_alu(%d)\n", alu_op);
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
    fprintf(out, "  pop ix\n");
    fprintf(out, "  pop bc\n");
    fprintf(out, "  %s ix, bc\n", alu_str[alu_op]);
    fprintf(out, "  push ix\n");
    stack--;
  }
    else
  {
    // Logic instruction
    fprintf(out, "  pop hl\n");
    fprintf(out, "  pop bc\n");
    fprintf(out, "  ld a, b\n");
    fprintf(out, "  %s a, h\n", alu_str[alu_op]);
    fprintf(out, "  ld h, a\n");
    fprintf(out, "  ld a, c\n");
    fprintf(out, "  %s a, l\n", alu_str[alu_op]);
    fprintf(out, "  ld l, a\n");
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
    fprintf(out, "  ld bc, %04x\n", value);
    //fprintf(out, "  scf\n");   // set carry
    //fprintf(out, "  ccf\n");   // carry = not carry
    fprintf(out, "  and a   ; clear carry\n");   
    fprintf(out, "  %s hl, bc\n", alu_str[alu_op]);
    fprintf(out, "  push hl\n");
#endif
    fprintf(out, "  pop ix\n");
    fprintf(out, "  ld b, %04x\n", value >> 8);
    fprintf(out, "  ld c, %04x\n", value & 0xff);
    fprintf(out, "  %s ix, bc\n", alu_str[alu_op]);
    fprintf(out, "  push ix\n");
    return 0;
  }

  // Now we know this is a logic instruction
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld a, h\n");
  fprintf(out, "  %s %02x\n", alu_str[alu_op], value >> 8);
  fprintf(out, "  ld h, a\n");
  fprintf(out, "  ld a, l\n");
  fprintf(out, "  %s %02x\n", alu_str[alu_op], value & 0xff);
  fprintf(out, "  ld l, a\n");
  fprintf(out, "  push hl\n");

  return 0;
}


