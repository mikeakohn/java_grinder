/**
 *  Java Grinder
 *  Author: The Naken Crew
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2013 by The Naken Crew
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "java_stack.h"
#include "java_class.h"
#include "java_compile.h"

// Size the stack/local_vars should be before malloc'ing a heap
#ifdef FRAME_STACK
#define STACK_SIZE 256
#endif

#define LOCAL_SIZE 32

#define POP_NULL() stack_values--; stack_ptr--;

#define PUSH_GENERIC(gen_value, gen_type) stack_types[stack_ptr++]=gen_type; *stack_values=gen_value; stack_values++;
#define POP_GENERIC(gen_type) *(--stack_values); \
              if (stack_types[--stack_ptr]!=gen_type) \
              { printf("Exception: Expected Integer\n"); goto leave; }

#define PUSH_INTEGER(value) PUSH_GENERIC(value, JAVA_TYPE_INTEGER);
#define POP_INTEGER() POP_GENERIC(JAVA_TYPE_INTEGER);

#define PUSH_FLOAT_I(value) PUSH_GENERIC(value, JAVA_TYPE_FLOAT);
#define POP_FLOAT_I() POP_GENERIC(JAVA_TYPE_FLOAT);

#define PUSH_REF(value) PUSH_GENERIC(value, JAVA_TYPE_REF);
#define POP_REF() POP_GENERIC(JAVA_TYPE_REF);

#define PUSH_BYTE PUSH_INTEGER
#define POP_BYTE POP_INTEGER

/*
#define PUSH_LONG(a) stack_types[stack_ptr]=JAVA_TYPE_LONG; stack_values[stack_ptr++]=a;
#define POP_LONG() stack_values[--stack_ptr]; \
              if (stack_types[stack_ptr]!=JAVA_TYPE_LONG) \
              { printf("Exception: Expected Long\n"); goto leave; }
*/

#define PUSH_LONG(gen_value) ((int16_t *)(void *)&stack_types)[stack_ptr]=(JAVA_TYPE_LONG<<8)|JAVA_TYPE_LONG; *(long long *)stack_values=gen_value; stack_values+=2;
#define POP_LONG() *(long long *)(stack_values-2); stack_values-=2; \
              stack_ptr-=2; \
              if (stack_types[stack_ptr]!=JAVA_TYPE_LONG) \
              { printf("Exception: Expected Long\n"); goto leave; }

#define PUSH_DOUBLE(gen_value) ((int16_t *)(void *)&stack_types)[stack_ptr]=(JAVA_TYPE_DOUBLE<<8)|JAVA_TYPE_DOUBLE; *(double *)stack_values=gen_value; stack_values+=2;
#define POP_DOUBLE() *((double *)(stack_values-2)); stack_values-=2; \
              stack_ptr-=2; \
              if (stack_types[stack_ptr]!=JAVA_TYPE_DOUBLE) \
              { printf("Exception: Expected Double\n"); goto leave; }

#define PUSH_FLOAT(gen_value) stack_types[stack_ptr++]=JAVA_TYPE_FLOAT; *(float *)stack_values=gen_value; stack_values++;
#define POP_FLOAT() *(float*)(--stack_values); \
              if (stack_types[--stack_ptr]!=JAVA_TYPE_FLOAT) \
              { printf("Exception: Expected Float\n"); goto leave; }

#define GET_PC_INT16(a) ((short int)(((unsigned short int)bytes[pc+a+0])<<8|bytes[pc+a+1]))
#define GET_PC_UINT16(a) (((unsigned short int)bytes[pc+a+0])<<8|bytes[pc+a+1])
#define GET_PC_INT32(a) ((int)((unsigned int)bytes[pc+a+0])<<24| \
                        ((unsigned int)bytes[pc+a+1])<<16|\
                        ((unsigned int)bytes[pc+a+2])<<8|\
                         bytes[pc+a+3])
#define GET_PC_UINT32(a) (((unsigned int)bytes[pc+a+0])<<24| \
                         ((unsigned int)bytes[pc+a+1])<<16|\
                         ((unsigned int)bytes[pc+a+2])<<8|\
                          bytes[pc+a+3])


#ifdef DEBUG
static void stack_dump(unsigned int *stack, unsigned char *stack_types, int stack_ptr)
{
int n;

  printf("---- Stack Dump (size=%d) -----\n", stack_ptr);
  for (n = 0; n < stack_ptr; n++)
  {
    printf("%d ",stack[n]);
    if (stack_types[n] == JAVA_TYPE_INTEGER)
    { printf("Integer"); }
      else
    if (stack_types[n] == JAVA_TYPE_LONG)
    { printf("Long"); }
      else
    if (stack_types[n] == JAVA_TYPE_FLOAT)
    { printf("Float %f", *((float*)(void*)(&stack[n]))); }
      else
    if (stack_types[n] == JAVA_TYPE_DOUBLE)
    { printf("Double"); }
      else
    if (stack_types[n] == JAVA_TYPE_REF)
    { printf("Ref"); }

    printf("\n");
  }
}
#endif

int java_compile_method(struct java_class_t *java_class, int method_index, struct generator_t *generator, struct java_stack_t *java_stack, int stack_start_ptr)
{
struct methods_t *method = ((void *)java_class->methods_heap) + java_class->methods[method_index];
unsigned char *bytes = method->attributes[0].info;
int pc;
const float fzero = 0.0;
const float fone = 1.0;
const float ftwo = 2.0;
int wide = 0;
int local_vars_stack[LOCAL_SIZE];
int *local_vars;
float fvalue1,fvalue2;
int value1,value2;
long long lvalue1,lvalue2;
double dvalue1,dvalue2;
int pc_start;
int max_stack;
int max_locals;
int code_len;
#ifdef FRAME_STACK
uint32_t stack_stack[STACK_SIZE];
uint8_t stack_types_stack[STACK_SIZE];
#endif
uint32_t *stack_values=0;
uint32_t *stack_values_start=0;
uint8_t *stack_types=0;
int stack_ptr=0;
uint32_t ref;
struct generic_32bit_t *gen32;
struct constant_float_t *constant_float;

  // bytes points to the method attributes info for the method.
  max_stack = ((int)bytes[0]<<8) | ((int)bytes[1]);
  max_locals = ((int)bytes[2]<<8) | ((int)bytes[3]);
  code_len = ((int)bytes[4]<<24) |
             ((int)bytes[5]<<16) |
             ((int)bytes[6]<<8) |
             ((int)bytes[7]);
  pc_start = (((int)bytes[code_len+8]<<8) |
             ((int)bytes[code_len+9])) + 8;
  pc = pc_start;

#ifdef DEBUG
printf("pc=%d\n", pc);
printf("max_stack=%d\n", max_stack);
printf("max_locals=%d\n", max_locals);
printf("code_len=%d\n", code_len);
#endif

#ifdef FRAME_STACK
  if (max_stack > STACK_SIZE)
  {
    stack = malloc(max_stack*(sizeof(int)));
    stack_types = malloc(max_stack);
  }
    else
  {
    stack_values = stack_stack;
    stack_types = stack_types;
  }
#else
  stack_values_start = java_stack->values + (stack_start_ptr*sizeof(int));
  stack_values = stack_values_start;
  stack_types = java_stack->types + (stack_start_ptr);
#endif

  if (max_locals > LOCAL_SIZE)
  { local_vars = malloc(max_locals * (sizeof(int))); }
    else
  { local_vars = local_vars_stack; }

  while(1)
  {
#ifdef DEBUG
    printf("pc=%d opcode=%d (0x%02x)\n", pc - pc_start, bytes[pc], bytes[pc]);
#endif
    switch(bytes[pc])
    {
      case 0: // nop (0x00)
        pc++;
        break;

      case 1: // aconst_null (0x01)
        printf("Opcode (0x01) aconst_null unimplemented\n");
        break;

      case 2: // iconst_m1 (0x02)
        PUSH_INTEGER(-1);
        pc++;
        break;

      case 3: // iconst_0 (0x03)
        PUSH_INTEGER(0);
        pc++;
        break;

      case 4: // iconst_1 (0x04)
        PUSH_INTEGER(1);
        pc++;
        break;

      case 5: // iconst_2 (0x05)
        PUSH_INTEGER(2);
        pc++;
        break;

      case 6: // iconst_3 (0x06)
        PUSH_INTEGER(3);
        pc++;
        break;

      case 7: // iconst_4 (0x07)
        PUSH_INTEGER(4);
        pc++;
        break;

      case 8: // iconst_5 (0x08)
        PUSH_INTEGER(5);
        pc++;
        break;

      case 9: // lconst_0 (0x09)
        PUSH_LONG(0);
        pc++;
        break;

      case 10: // lconst_1 (0x0a)
        PUSH_LONG(1);
        pc++;
        break;

      case 11: // fconst_0 (0x0b)
        PUSH_FLOAT(fzero);
        pc++;
        break;

      case 12: // fconst_1 (0x0c)
        // PUSH_INTEGER(*((unsigned int *)((void *)&fone)));
        PUSH_FLOAT(fone);
        pc++;
        break;

      case 13: // fconst_2 (0x0d)
        PUSH_FLOAT(ftwo);
        pc++;
        break;

      case 14: // dconst_0 (0x0e)
        printf("Opcode (0x0e) dconst_0 unimplemented\n");
        break;

      case 15: // dconst_1 (0x0f)
        printf("Opcode (0x0f) dconst_1 unimplemented\n");
        break;

      case 16: // bipush (0x10)
        PUSH_BYTE((char)bytes[pc+1])
        pc+=2;
        break;

      case 17: // sipush (0x11)
        printf("Opcode (0x11) sipush unimplemented\n");
        break;

      case 18: // ldc (0x12)
        gen32 = ((void *)java_class->constants_heap) + java_class->constant_pool[bytes[pc+1]];
        if (gen32->tag == CONSTANT_INTEGER)
        { PUSH_INTEGER(gen32->value); }
          else
        if (gen32->tag == CONSTANT_FLOAT)
        {
          constant_float = (void *)gen32;
          PUSH_FLOAT(constant_float->value);
        }
          else
        if (gen32->tag == CONSTANT_STRING)
        {
          printf("Can't do a string yet.. :(\n");
          goto leave;
        }
          else
        {
          printf("Cannot ldc this type\n");
          goto leave;
        }

        pc += 2;
        break;

      case 19: // ldc_w (0x13)
        printf("Opcode (0x13) ldc_w unimplemented\n");
        break;

      case 20: // ldc2_w (0x14)
        printf("Opcode (0x14) ldc2_w unimplemented\n");
        break;

      case 21: // iload (0x15)
        if (wide == 1)
        {
          wide = 0;
          PUSH_INTEGER(local_vars[GET_PC_UINT16(1)]);
          pc += 3;
        }
          else
        {
          PUSH_INTEGER(local_vars[bytes[pc+1]]);
          pc += 2;
        }
        break;

      case 22: // lload (0x16)
        if (wide == 1)
        {
          wide = 0;
          PUSH_LONG(*((long long *)(local_vars+GET_PC_UINT16(1))));
          pc += 3;
        }
          else
        {
          PUSH_LONG(*((long long *)(local_vars+bytes[pc+1])));
          pc += 2;
        }
        break;

      case 23: // fload (0x17)
        if (wide == 1)
        {
          wide = 0;
          PUSH_FLOAT_I(local_vars[GET_PC_UINT16(1)]);
          pc += 3;
        }
          else
        {
          PUSH_FLOAT_I(local_vars[bytes[pc+1]]);
          pc += 2;
        }
        break;

      case 24: // dload (0x18)
        if (wide == 1)
        {
          wide = 0;
          PUSH_LONG(*((long long *)(local_vars+GET_PC_UINT16(1))));
          pc += 3;
        }
          else
        {
          PUSH_LONG(*((long long *)(local_vars+bytes[pc+1])));
          pc += 2;
        }
        break;

      case 25: // aload (0x19)
        printf("Opcode (0x19) aload unimplemented\n");
        break;

      case 26: // iload_0 (0x1a)
        PUSH_INTEGER(local_vars[0]);
        pc++;
        break;

      case 27: // iload_1 (0x1b)
        PUSH_INTEGER(local_vars[1]);
        pc++;
        break;

      case 28: // iload_2 (0x1c)
        PUSH_INTEGER(local_vars[2]);
        pc++;
        break;

      case 29: // iload_3 (0x1d)
        PUSH_INTEGER(local_vars[3]);
        pc++;
        break;

      case 30: // lload_0 (0x1e)
        printf("Opcode (0x1e) lload_0 unimplemented\n");
        break;

      case 31: // lload_1 (0x1f)
        printf("Opcode (0x1f) lload_1 unimplemented\n");
        break;

      case 32: // lload_2 (0x20)
        printf("Opcode (0x20) lload_2 unimplemented\n");
        break;

      case 33: // lload_3 (0x21)
        printf("Opcode (0x21) lload_3 unimplemented\n");
        break;

      case 34: // fload_0 (0x22)
        PUSH_FLOAT_I(local_vars[0]);
        pc++;
        break;

      case 35: // fload_1 (0x23)
        PUSH_FLOAT_I(local_vars[1]);
        pc++;
        break;

      case 36: // fload_2 (0x24)
        PUSH_FLOAT_I(local_vars[2]);
        pc++;
        break;

      case 37: // fload_3 (0x25)
        PUSH_FLOAT_I(local_vars[3]);
        pc++;
        break;

      case 38: // dload_0 (0x26)
        printf("Opcode (0x26) dload_0 unimplemented\n");
        break;

      case 39: // dload_1 (0x27)
        printf("Opcode (0x27) dload_1 unimplemented\n");
        break;

      case 40: // dload_2 (0x28)
        printf("Opcode (0x28) dload_2 unimplemented\n");
        break;

      case 41: // dload_3 (0x29)
        printf("Opcode (0x29) dload_3 unimplemented\n");
        break;

      case 42: // aload_0 (0x2a)
        printf("Opcode (0x2a) aload_0 unimplemented\n");
        break;

      case 43: // aload_1 (0x2b)
        printf("Opcode (0x2b) aload_1 unimplemented\n");
        break;

      case 44: // aload_2 (0x2c)
        printf("Opcode (0x2c) aload_2 unimplemented\n");
        break;

      case 45: // aload_3 (0x2d)
        printf("Opcode (0x2d) aload_3 unimplemented\n");
        break;

      case 46: // iaload (0x2e)
        printf("Opcode (0x2e) iaload unimplemented\n");
        break;

      case 47: // laload (0x2f)
        printf("Opcode (0x2f) laload unimplemented\n");
        break;

      case 48: // faload (0x30)
        printf("Opcode (0x30) faload unimplemented\n");
        break;

      case 49: // daload (0x31)
        printf("Opcode (0x31) daload unimplemented\n");
        break;

      case 50: // aaload (0x32)
        printf("Opcode (0x32) aaload unimplemented\n");
        break;

      case 51: // baload (0x33)
        printf("Opcode (0x33) baload unimplemented\n");
        break;

      case 52: // caload (0x34)
        printf("Opcode (0x34) caload unimplemented\n");
        break;

      case 53: // saload (0x35)
        printf("Opcode (0x35) saload unimplemented\n");
        break;

      case 54: // istore (0x36)
        if (wide == 1)
        {
          local_vars[GET_PC_UINT16(1)]=POP_INTEGER();
          pc += 3;
          wide = 0;
        }
          else
        {
          local_vars[bytes[pc+1]]=POP_INTEGER();
          pc += 2;
        }
        break;

      case 55: // lstore (0x37)
        printf("Opcode (0x37) lstore unimplemented\n");
        break;

      case 56: // fstore (0x38)
        printf("Opcode (0x38) fstore unimplemented\n");
        break;

      case 57: // dstore (0x39)
        printf("Opcode (0x39) dstore unimplemented\n");
        break;

      case 58: // astore (0x3a)
        printf("Opcode (0x3a) astore unimplemented\n");
        break;

      case 59: // istore_0 (0x3b)
        local_vars[0] = POP_INTEGER();
        pc++;
        break;

      case 60: // istore_1 (0x3c)
        local_vars[1] = POP_INTEGER();
        pc++;
        break;

      case 61: // istore_2 (0x3d)
        local_vars[2] = POP_INTEGER();
        pc++;
        break;

      case 62: // istore_3 (0x3e)
        local_vars[3] = POP_INTEGER();
        pc++;
        break;

      case 63: // lstore_0 (0x3f)
        printf("Opcode (0x3f) lstore_0 unimplemented\n");
        break;

      case 64: // lstore_1 (0x40)
        printf("Opcode (0x40) lstore_1 unimplemented\n");
        break;

      case 65: // lstore_2 (0x41)
        printf("Opcode (0x41) lstore_2 unimplemented\n");
        break;

      case 66: // lstore_3 (0x42)
        printf("Opcode (0x42) lstore_3 unimplemented\n");
        break;

      case 67: // fstore_0 (0x43)
        local_vars[0] = POP_FLOAT_I()
        pc++;
        break;

      case 68: // fstore_1 (0x44)
        local_vars[1] = POP_FLOAT_I()
        pc++;
        break;

      case 69: // fstore_2 (0x45)
        local_vars[2] = POP_FLOAT_I()
        pc++;
        break;

      case 70: // fstore_3 (0x46)
        local_vars[3] = POP_FLOAT_I()
        pc++;
        break;

      case 71: // dstore_0 (0x47)
        printf("Opcode (0x47) dstore_0 unimplemented\n");
        break;

      case 72: // dstore_1 (0x48)
        printf("Opcode (0x48) dstore_1 unimplemented\n");
        break;

      case 73: // dstore_2 (0x49)
        printf("Opcode (0x49) dstore_2 unimplemented\n");
        break;

      case 74: // dstore_3 (0x4a)
        printf("Opcode (0x4a) dstore_3 unimplemented\n");
        break;

      case 75: // astore_0 (0x4b)
        printf("Opcode (0x4b) astore_0 unimplemented\n");
        break;

      case 76: // astore_1 (0x4c)
        printf("Opcode (0x4c) astore_1 unimplemented\n");
        break;

      case 77: // astore_2 (0x4d)
        printf("Opcode (0x4d) astore_2 unimplemented\n");
        break;

      case 78: // astore_3 (0x4e)
        printf("Opcode (0x4e) astore_3 unimplemented\n");
        break;

      case 79: // iastore (0x4f)
        printf("Opcode (0x4f) iastore unimplemented\n");
        break;

      case 80: // lastore (0x50)
        printf("Opcode (0x50) lastore unimplemented\n");
        break;

      case 81: // fastore (0x51)
        printf("Opcode (0x51) fastore unimplemented\n");
        break;

      case 82: // dastore (0x52)
        printf("Opcode (0x52) dastore unimplemented\n");
        break;

      case 83: // aastore (0x53)
        printf("Opcode (0x53) aastore unimplemented\n");
        break;

      case 84: // bastore (0x54)
        printf("Opcode (0x54) bastore unimplemented\n");
        break;

      case 85: // castore (0x55)
        printf("Opcode (0x55) castore unimplemented\n");
        break;

      case 86: // sastore (0x56)
        printf("Opcode (0x56) sastore unimplemented\n");
        break;

      case 87: // pop (0x57)
        POP_NULL();
        pc++;
        break;

      case 88: // pop2 (0x58)
        POP_NULL();
        POP_NULL();
        pc++;
        break;

      case 89: // dup (0x59)
        value1 = POP_INTEGER();
        PUSH_INTEGER(value1);
        PUSH_INTEGER(value1);
        pc++;
        break;

      case 90: // dup_x1 (0x5a)
        printf("Opcode (0x5a) dup_x1 unimplemented\n");
        break;

      case 91: // dup_x2 (0x5b)
        printf("Opcode (0x5b) dup_x2 unimplemented\n");
        break;

      case 92: // dup2 (0x5c)
        value2 = POP_INTEGER();
        value1 = POP_INTEGER();
        PUSH_INTEGER(value1);
        PUSH_INTEGER(value2);
        PUSH_INTEGER(value1);
        PUSH_INTEGER(value2);
        pc++;
        break;

      case 93: // dup2_x1 (0x5d)
        printf("Opcode (0x5d) dup2_x1 unimplemented\n");
        break;

      case 94: // dup2_x2 (0x5e)
        printf("Opcode (0x5e) dup2_x2 unimplemented\n");
        break;

      case 95: // swap (0x5f)
        value2 = POP_INTEGER();
        value1 = POP_INTEGER();
        PUSH_INTEGER(value2);
        PUSH_INTEGER(value1);
        pc++;
        break;

      case 96: // iadd (0x60)
        value1 = POP_INTEGER();
        value2 = POP_INTEGER();
        value1 = value1+value2;
        PUSH_INTEGER(value1);
        pc++;
        break;

      case 97: // ladd (0x61)
        lvalue1 = POP_LONG();
        lvalue2 = POP_LONG();
        lvalue1 = lvalue1+lvalue2;
        PUSH_LONG(lvalue1);
        pc++;
        break;

      case 98: // fadd (0x62)
        fvalue1 = POP_FLOAT();
        fvalue2 = POP_FLOAT();
        fvalue1 = fvalue1+fvalue2;
        PUSH_FLOAT(fvalue1);
        pc++;
        break;

      case 99: // dadd (0x63)
        dvalue1 = POP_DOUBLE();
        dvalue2 = POP_DOUBLE();
        dvalue1 = dvalue1+dvalue2;
        PUSH_DOUBLE(dvalue1);
        pc++;
        break;

      case 100: // isub (0x64)
        value1 = POP_INTEGER();
        value2 = POP_INTEGER();
        value1 = value2-value1;
        PUSH_INTEGER(value1);
        pc++;
        break;

      case 101: // lsub (0x65)
        lvalue1 = POP_LONG();
        lvalue2 = POP_LONG();
        lvalue1 = lvalue2-lvalue1;
        PUSH_LONG(lvalue1);
        pc++;
        break;

      case 102: // fsub (0x66)
        fvalue1 = POP_FLOAT();
        fvalue2 = POP_FLOAT();
        fvalue1 = fvalue2-fvalue1;
        PUSH_FLOAT(fvalue1);
        pc++;
        break;

      case 103: // dsub (0x67)
        dvalue1 = POP_DOUBLE();
        dvalue2 = POP_DOUBLE();
        dvalue1 = dvalue2-dvalue1;
        PUSH_DOUBLE(dvalue1);
        pc++;
        break;

      case 104: // imul (0x68)
        value1 = POP_INTEGER();
        value2 = POP_INTEGER();
        value1 = value2*value1;
        PUSH_INTEGER(value1);
        pc++;
        break;

      case 105: // lmul (0x69)
        lvalue1 = POP_LONG();
        lvalue2 = POP_LONG();
        lvalue1 = lvalue2*lvalue1;
        PUSH_LONG(lvalue1);
        pc++;
        break;

      case 106: // fmul (0x6a)
        fvalue1 = POP_FLOAT();
        fvalue2 = POP_FLOAT();
        fvalue1 = fvalue2*fvalue1;
        PUSH_FLOAT(fvalue1);
        pc++;
        break;

      case 107: // dmul (0x6b)
        dvalue1 = POP_DOUBLE();
        dvalue2 = POP_DOUBLE();
        dvalue1 = dvalue2*dvalue1;
        PUSH_DOUBLE(dvalue1);
        pc++;
        break;

      case 108: // idiv (0x6c)
        printf("Opcode (0x6c) idiv unimplemented\n");
        break;

      case 109: // ldiv (0x6d)
        printf("Opcode (0x6d) ldiv unimplemented\n");
        break;

      case 110: // fdiv (0x6e)
        printf("Opcode (0x6e) fdiv unimplemented\n");
        break;

      case 111: // ddiv (0x6f)
        printf("Opcode (0x6f) ddiv unimplemented\n");
        break;

      case 112: // irem (0x70)
        printf("Opcode (0x70) irem unimplemented\n");
        break;

      case 113: // lrem (0x71)
        printf("Opcode (0x71) lrem unimplemented\n");
        break;

      case 114: // frem (0x72)
        printf("Opcode (0x72) frem unimplemented\n");
        break;

      case 115: // drem (0x73)
        printf("Opcode (0x73) drem unimplemented\n");
        break;

      case 116: // ineg (0x74)
        printf("Opcode (0x74) ineg unimplemented\n");
        break;

      case 117: // lneg (0x75)
        printf("Opcode (0x75) lneg unimplemented\n");
        break;

      case 118: // fneg (0x76)
        printf("Opcode (0x76) fneg unimplemented\n");
        break;

      case 119: // dneg (0x77)
        printf("Opcode (0x77) dneg unimplemented\n");
        break;

      case 120: // ishl (0x78)
        printf("Opcode (0x78) ishl unimplemented\n");
        break;

      case 121: // lshl (0x79)
        printf("Opcode (0x79) lshl unimplemented\n");
        break;

      case 122: // ishr (0x7a)
        printf("Opcode (0x7a) ishr unimplemented\n");
        break;

      case 123: // lshr (0x7b)
        printf("Opcode (0x7b) lshr unimplemented\n");
        break;

      case 124: // iushr (0x7c)
        printf("Opcode (0x7c) iushr unimplemented\n");
        break;

      case 125: // lushr (0x7d)
        printf("Opcode (0x7d) lushr unimplemented\n");
        break;

      case 126: // iand (0x7e)
        value1 = POP_INTEGER();
        value2 = POP_INTEGER();
        PUSH_INTEGER(value1&value2);
        break;

      case 127: // land (0x7f)
        printf("Opcode (0x7f) land unimplemented\n");
        break;

      case 128: // ior (0x80)
        printf("Opcode (0x80) ior unimplemented\n");
        break;

      case 129: // lor (0x81)
        printf("Opcode (0x81) lor unimplemented\n");
        break;

      case 130: // ixor (0x82)
        printf("Opcode (0x82) ixor unimplemented\n");
        break;

      case 131: // lxor (0x83)
        printf("Opcode (0x83) lxor unimplemented\n");
        break;

      case 132: // iinc (0x84)
        if (wide == 1)
        {
          local_vars[GET_PC_UINT16(1)] += GET_PC_INT16(3);
          pc += 5;
          wide = 0;
        }
          else
        {
          local_vars[bytes[pc+1]] += ((char)bytes[pc+2]);
          pc += 3;
        }
        break;

      case 133: // i2l (0x85)
        value1 = POP_INTEGER();
        lvalue1 = value1;
        PUSH_LONG(lvalue1);
        pc++;
        break;

      case 134: // i2f (0x86)
        value1 = POP_INTEGER();
        fvalue1 = value1;
        PUSH_FLOAT(fvalue1);
        pc++;
        break;

      case 135: // i2d (0x87)
        value1 = POP_INTEGER();
        dvalue1 = value1;
        PUSH_DOUBLE(dvalue1);
        pc++;
        break;

      case 136: // l2i (0x88)
        lvalue1 = POP_LONG();
        value1 = (int)lvalue1;
        PUSH_INTEGER(value1);
        pc++;
        break;

      case 137: // l2f (0x89)
        lvalue1 = POP_LONG();
        fvalue1 = lvalue1;
        PUSH_FLOAT(fvalue1);
        pc++;
        break;

      case 138: // l2d (0x8a)
        lvalue1 = POP_LONG();
        dvalue1 = lvalue1;
        PUSH_DOUBLE(dvalue1);
        pc++;
        break;

      case 139: // f2i (0x8b)
        fvalue1 = POP_FLOAT();
        value1 = fvalue1;
        PUSH_INTEGER(value1);
        pc++;
        break;

      case 140: // f2l (0x8c)
        fvalue1 = POP_FLOAT();
        lvalue1 = fvalue1;
        PUSH_LONG(lvalue1);
        pc++;
        break;

      case 141: // f2d (0x8d)
        fvalue1 = POP_FLOAT();
        dvalue1 = fvalue1;
        PUSH_INTEGER(dvalue1);
        pc++;
        break;

      case 142: // d2i (0x8e)
        dvalue1 = POP_DOUBLE();
        value1 = dvalue1;
        PUSH_INTEGER(value1);
        pc++;
        break;

      case 143: // d2l (0x8f)
        dvalue1 = POP_DOUBLE();
        lvalue1 = dvalue1;
        PUSH_LONG(lvalue1);
        pc++;
        break;

      case 144: // d2f (0x90)
        dvalue1 = POP_DOUBLE();
        fvalue1 = dvalue1;
        PUSH_FLOAT(fvalue1);
        pc++;
        break;

      case 145: // i2b (0x91)
        printf("Opcode (0x91) i2b unimplemented\n");
        break;

      case 146: // i2c (0x92)
        printf("Opcode (0x92) i2c unimplemented\n");
        break;

      case 147: // i2s (0x93)
        printf("Opcode (0x93) i2s unimplemented\n");
        break;

      case 148: // lcmp (0x94)
        printf("Opcode (0x94) lcmp unimplemented\n");
        break;

      case 149: // fcmpl (0x95)
        printf("Opcode (0x95) fcmpl unimplemented\n");
        break;

      case 150: // fcmpg (0x96)
        printf("Opcode (0x96) fcmpg unimplemented\n");
        break;

      case 151: // dcmpl (0x97)
        printf("Opcode (0x97) dcmpl unimplemented\n");
        break;

      case 152: // dcmpg (0x98)
        printf("Opcode (0x98) dcmpg unimplemented\n");
        break;

      case 153: // ifeq (0x99)
        value1 = POP_INTEGER();
        if (value1 == 0)
        { pc += GET_PC_INT16(1); }
          else
        { pc += 3; }
        break;

      case 154: // ifne (0x9a)
        value1 = POP_INTEGER();
        if (value1 != 0)
        { pc += GET_PC_INT16(1); }
          else
        { pc += 3; }
        break;

      case 155: // iflt (0x9b)
        value1 = POP_INTEGER();
        if (value1 < 0)
        { pc += GET_PC_INT16(1); }
          else
        { pc += 3; }
        break;

      case 156: // ifge (0x9c)
        value1 = POP_INTEGER();
        if (value1 >= 0)
        { pc += GET_PC_INT16(1); }
          else
        { pc += 3; }
        break;

      case 157: // ifgt (0x9d)
        value1 = POP_INTEGER();
        if (value1 > 0)
        { pc += GET_PC_INT16(1); }
          else
        { pc += 3; }
        break;

      case 158: // ifle (0x9e)
        value1 = POP_INTEGER();
        if (value1 <= 0)
        { pc += GET_PC_INT16(1); }
          else
        { pc += 3; }
        break;

      case 159: // if_icmpeq (0x9f)
        value1 = POP_INTEGER();
        value2 = POP_INTEGER();
        if (value2 == value1)
        { pc += GET_PC_INT16(1); }
          else
        { pc += 3; }
        break;

      case 160: // if_icmpne (0xa0)
        value1 = POP_INTEGER();
        value2 = POP_INTEGER();
        if (value2 != value1)
        { pc += GET_PC_INT16(1); }
          else
        { pc += 3; }
        break;

      case 161: // if_icmplt (0xa1)
        value1 = POP_INTEGER();
        value2 = POP_INTEGER();
        if (value2 < value1)
        { pc += GET_PC_INT16(1); }
          else
        { pc += 3; }
        break;

      case 162: // if_icmpge (0xa2)
        value1 = POP_INTEGER();
        value2 = POP_INTEGER();
        if (value2 >= value1)
        { pc += GET_PC_INT16(1); }
          else
        { pc += 3; }
        break;

      case 163: // if_icmpgt (0xa3)
        value1 = POP_INTEGER();
        value2 = POP_INTEGER();
        if (value2 > value1)
        { pc += GET_PC_INT16(1); }
          else
        { pc += 3; }
        break;

      case 164: // if_icmple (0xa4)
        value1 = POP_INTEGER();
        value2 = POP_INTEGER();
        if (value2 <= value1)
        { pc += GET_PC_INT16(1); }
          else
        { pc += 3; }
        break;

      case 165: // if_acmpeq (0xa5)
        printf("Opcode (0xa5) if_acmpeq unimplemented\n");
        break;

      case 166: // if_acmpne (0xa6)
        printf("Opcode (0xaa) if_acmpne unimplemented\n");
        break;

      case 167: // goto (0xa7)
        pc += GET_PC_INT16(1);
        break;

      case 168: // jsr (0xa8)
        PUSH_INTEGER(pc+3);
        pc += GET_PC_INT16(1);
        break;

      case 169: // ret (0xa9)
        if (wide == 1)
        {
          pc = local_vars[GET_PC_UINT16(1)];
          wide = 0;
        }
          else
        {
          pc = local_vars[GET_PC_UINT32(1)];
        }
        break;

      case 170: // tableswitch (0xaa)
        printf("Opcode (0xaa) tableswitch unimplemented\n");
        break;

      case 171: // lookupswitch (0xab)
        printf("Opcode (0xab) lookupswitch unimplemented\n");
        break;

      case 172: // ireturn (0xac)
        value1 = POP_INTEGER()
        stack_values = java_stack->values;
        stack_types = java_stack->types;
        PUSH_INTEGER(value1);
        goto leave;
        break;

      case 173: // lreturn (0xad)
        lvalue1 = POP_LONG()
        stack_values = java_stack->values;
        stack_types = java_stack->types;
        PUSH_LONG(lvalue1);
        goto leave;
        break;

      case 174: // freturn (0xae)
        value1 = POP_INTEGER()
        stack_values = java_stack->values;
        stack_types = java_stack->types;
        PUSH_INTEGER(value1);
        goto leave;
        break;

      case 175: // dreturn (0xaf)
        dvalue1 = POP_DOUBLE()
        stack_values = java_stack->values;
        stack_types = java_stack->types;
        PUSH_LONG(dvalue1);
        goto leave;
        break;

      case 176: // areturn (0xb0)
        printf("Opcode (0xb0) areturn unimplemented\n");
        break;

      case 177: // return (0xb1)
        goto leave;
        break;

      case 178: // getstatic (0xb2)
        // printf("Opcode (0xb2) getstatic unimplemented\n");
        // FIXME - need to test for private/protected and that it's a field
        // printf("getstatic %d\n",GET_PC_UINT16(1));
        PUSH_REF(GET_PC_UINT16(1));
        pc+=3;
        break;

      case 179: // putstatic (0xb3)
        printf("Opcode (0xb3) putstatic unimplemented\n");
        break;

      case 180: // getfield (0xb4)
        printf("Opcode (0xb4) getfield unimplemented\n");
        break;

      case 181: // putfield (0xb5)
        printf("Opcode (0xb5) putfield unimplemented\n");
        break;

      case 182: // invokevirtual (0xb6)
        // printf("Opcode (0xb6) invokevirtual unimplemented\n");
        ref = GET_PC_UINT16(1);
        // FIXME - Hack (this should probably be in a function somewhere)
        {
          struct constant_methodref_t *constant_method;
          struct constant_class_t *constant_class;
          struct constant_utf8_t *constant_utf8;
          struct constant_nameandtype_t *constant_nameandtype;
#ifdef DEBUG
          int n;
#endif
          constant_method = ((void*)java_class->constants_heap) + java_class->constant_pool[ref];
          if (constant_method->tag != CONSTANT_METHODREF)
          {
            printf("Exception: reference is not a method\n");
            goto leave;
          }

          constant_class = ((void*)java_class->constants_heap) + java_class->constant_pool[constant_method->class_index];
          constant_utf8 = ((void*)java_class->constants_heap) + java_class->constant_pool[constant_class->name_index];
          constant_nameandtype = ((void*)java_class->constants_heap) + java_class->constant_pool[constant_method->name_and_type_index];
#ifdef DEBUG
          printf("Call to class: ");
          for (n = 0; n < constant_utf8->length; n++)
          {
            printf("%c", constant_utf8->bytes[n]);
          }
          printf("\n");
#endif
          constant_utf8 = ((void*)java_class->constants_heap) + java_class->constant_pool[constant_nameandtype->name_index];
#ifdef DEBUG
          printf("Call to method: ");
          for (n = 0; n < constant_utf8->length; n++)
          {
            printf("%c", constant_utf8->bytes[n]);
          }
          printf("\n");
#endif
          if (strncmp((char*)constant_utf8->bytes, "println", constant_utf8->length)!=0)
          {
            printf("We only support println right now (and not even very well :)\n");
            goto leave;
          }
          constant_utf8 = ((void*)java_class->constants_heap) + java_class->constant_pool[constant_nameandtype->descriptor_index];

#ifdef DEBUG
          printf("Call to signature: ");
          for (n = 0; n < constant_utf8->length; n++)
          {
            printf("%c", constant_utf8->bytes[n]);
          }
          printf("\n");
#endif

          if (constant_utf8->bytes[1] == 'I')
          {
            value1 = POP_INTEGER(); // FIXME.. wrong
            printf("%d\n",value1);
          }
            else
          if (constant_utf8->bytes[1] == 'F')
          {
            float f1;
            f1 = POP_FLOAT(); // FIXME.. wrong
            printf("%f\n", f1);
          }
            else
          {
            printf("Cannot println this type %c\n",constant_utf8->bytes[1]);
            goto leave;
          }
        }
        ref = POP_REF(); // FIXME.. wrong
        // printf("invokevirtual %d\n",GET_PC_UINT16(1));

        pc += 3;
        break;

      case 183: // invokespecial (0xb7)
        printf("Opcode (0xb7) invokespecial unimplemented\n");
        break;

      case 184: // invokestatic (0xb8)
        printf("Opcode (0xb8) invokestatic unimplemented\n");
        break;

      case 185: // invokeinterface (0xb9)
        printf("Opcode (0xb9) invokeinterface unimplemented\n");
        break;

      case 186: // xxxunusedxxx1 (0xba)
        printf("Opcode (0xba) xxxunusedxxx1 unimplemented\n");
        break;

      case 187: // new (0xbb)
        printf("Opcode (0xbb) new unimplemented\n");
        break;

      case 188: // newarray (0xbc)
        printf("Opcode (0xbc) newarray unimplemented\n");
        break;

      case 189: // anewarray (0xbd)
        printf("Opcode (0xbd) anewarray unimplemented\n");
        break;

      case 190: // arraylength (0xbe)
        printf("Opcode (0xbe) arraylength unimplemented\n");
        break;

      case 191: // athrow (0xbf)
        printf("Opcode (0xbf) athrow unimplemented\n");
        break;

      case 192: // checkcast (0xc0)
        printf("Opcode (0xc0) checkcast unimplemented\n");
        break;

      case 193: // instanceof (0xc1)
        printf("Opcode (0xc1) instanceof unimplemented\n");
        break;

      case 194: // monitorenter (0xc2)
        printf("Opcode (0xc2) monitorenter unimplemented\n");
        break;

      case 195: // monitorexit (0xc3)
        printf("Opcode (0xc3) monitorexit unimplemented\n");
        break;

      case 196: // wide (0xc4)
        wide=1;
        pc++;
        break;

      case 197: // multianewarray (0xc5)
        printf("Opcode (0xc5) multianewarray unimplemented\n");
        break;

      case 198: // ifnull (0xc6)
        printf("Opcode (0xc6) ifnull unimplemented\n");
        break;

      case 199: // ifnonnull (0xc7)
        printf("Opcode (0xc7) ifnonnull unimplemented\n");
        break;

      case 200: // goto_w (0xc8)
        pc += (short)((((unsigned int)bytes[pc+1])<<24) |
                      (((unsigned int)bytes[pc+2])<<16) |
                      (((unsigned int)bytes[pc+3])<<8) |
                      bytes[pc+4]);
        break;

      case 201: // jsr_w (0xc9)
        PUSH_INTEGER(pc+5);
        pc += GET_PC_INT32(1);
        break;

      case 202: // breakpoint (0xca)
        printf("Breakpoint reached!  pc=%d\n",pc);
        goto leave;
        break;

      case 203: // not_valid (0xfe)
      case 204: // not_valid (0xfe)
      case 205: // not_valid (0xfe)
      case 206: // not_valid (0xfe)
      case 207: // not_valid (0xfe)
      case 208: // not_valid (0xfe)
      case 209: // not_valid (0xfe)
      case 210: // not_valid (0xfe)
      case 211: // not_valid (0xfe)
      case 212: // not_valid (0xfe)
      case 213: // not_valid (0xfe)
      case 214: // not_valid (0xfe)
      case 215: // not_valid (0xfe)
      case 216: // not_valid (0xfe)
      case 217: // not_valid (0xfe)
      case 218: // not_valid (0xfe)
      case 219: // not_valid (0xfe)
      case 220: // not_valid (0xfe)
      case 221: // not_valid (0xfe)
      case 222: // not_valid (0xfe)
      case 223: // not_valid (0xfe)
      case 224: // not_valid (0xfe)
      case 225: // not_valid (0xfe)
      case 226: // not_valid (0xfe)
      case 227: // not_valid (0xfe)
      case 228: // not_valid (0xfe)
      case 229: // not_valid (0xfe)
      case 230: // not_valid (0xfe)
      case 231: // not_valid (0xfe)
      case 232: // not_valid (0xfe)
      case 233: // not_valid (0xfe)
      case 234: // not_valid (0xfe)
      case 235: // not_valid (0xfe)
      case 236: // not_valid (0xfe)
      case 237: // not_valid (0xfe)
      case 238: // not_valid (0xfe)
      case 239: // not_valid (0xfe)
      case 240: // not_valid (0xfe)
      case 241: // not_valid (0xfe)
      case 242: // not_valid (0xfe)
      case 243: // not_valid (0xfe)
      case 244: // not_valid (0xfe)
      case 245: // not_valid (0xfe)
      case 246: // not_valid (0xfe)
      case 247: // not_valid (0xfe)
      case 248: // not_valid (0xfe)
      case 249: // not_valid (0xfe)
      case 250: // not_valid (0xfe)
      case 251: // not_valid (0xfe)
      case 252: // not_valid (0xfe)
      case 253: // not_valid (0xfe)
        break;

      case 254: // impdep1 (0xfe)
        printf("Opcode (0xfe) impdep1 unimplemented\n");
        break;

      case 255: // impdep2 (0xff)
        printf("Opcode (0xff) impdep2 unimplemented\n");
        break;
    }

#ifdef DEBUG
    stack_dump(stack_values_start,stack_types,stack_ptr);
#endif

    // Is this even needed?
    if (pc >= code_len + 8) { pc = pc - code_len; }
    else if (pc < 8) { pc = pc + code_len; }
  }
leave:

  if (local_vars != local_vars_stack) { free(local_vars); }
#ifdef FRAME_STACK
  if (stack_values != stack_stack) { free(stack_value); }
  if (stack_types != stack_types_stack) { free(stack_types); }
#endif

  return 0;
}



