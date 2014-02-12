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

#include "Generator.h"
#include "JavaClass.h"
#include "table_java_instr.h"

// Static fields are done awfuly strange in Java.  Well, mostly it's the
// arrays that are done oddly.  So I think the trick is to actually run
// the Java code to create the arrays in flash.  Then need to figure out
// how to add the other code to the init section.  Maybe the trick is to
// generate a brand new Java function and compile it.

#define UNIMPL() printf("Unimplemented\n"); return -1;

int execute_static(JavaClass *java_class, int method_id, Generator *generator)
{
struct methods_t *method = java_class->get_method(method_id);
uint8_t *bytes = method->attributes[0].info;
int pc;
int wide = 0;
int pc_start;
int max_stack;
int max_locals;
int code_len;
int32_t *stack;
int32_t temp;
int stack_ptr;

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

  printf("max_stack=%d max_locals=%d code_len=%d\n", max_stack, max_locals, code_len);

  //generator->method_start(max_locals, method_name);
  stack = (int32_t *)alloca(max_stack * sizeof(int32_t));
  stack_ptr = 0;

  while(pc - pc_start < code_len)
  {
    int address = pc - pc_start;
#ifdef DEBUG
    printf("pc=%d %s opcode=%d (0x%02x)\n", address, table_java_instr[bytes[pc]].name, bytes[pc], bytes[pc]);

    switch(bytes[pc])
    {
      case 0: // nop (0x00)
      case 1: // aconst_null (0x01)
      case 2: // iconst_m1 (0x02)
        UNIMPL();
      case 3: // iconst_0 (0x03)
      case 4: // iconst_1 (0x04)
      case 5: // iconst_2 (0x05)
      case 6: // iconst_3 (0x06)
      case 7: // iconst_4 (0x07)
      case 8: // iconst_5 (0x08)
        stack[stack_ptr++] = bytes[pc] - 0x03;
        break;
      case 9: // lconst_0 (0x09)
      case 10: // lconst_1 (0x0a)
      case 11: // fconst_0 (0x0b)
      case 12: // fconst_1 (0x0c)
      case 13: // fconst_2 (0x0d)
      case 14: // dconst_0 (0x0e)
      case 15: // dconst_1 (0x0f)
        UNIMPL();
      case 16: // bipush (0x10)
        stack[stack_ptr++] = (int8_t)bytes[pc+1];
        break;
      case 17: // sipush (0x11)
      case 18: // ldc (0x12)
      case 19: // ldc_w (0x13)
      case 20: // ldc2_w (0x14)
      case 21: // iload (0x15)
      case 22: // lload (0x16)
      case 23: // fload (0x17)
      case 24: // dload (0x18)
      case 25: // aload (0x19)
      case 26: // iload_0 (0x1a)
      case 27: // iload_1 (0x1b)
      case 28: // iload_2 (0x1c)
      case 29: // iload_3 (0x1d)
      case 30: // lload_0 (0x1e)
      case 31: // lload_1 (0x1f)
      case 32: // lload_2 (0x20)
      case 33: // lload_3 (0x21)
      case 34: // fload_0 (0x22)
      case 35: // fload_1 (0x23)
      case 36: // fload_2 (0x24)
      case 37: // fload_3 (0x25)
      case 38: // dload_0 (0x26)
      case 39: // dload_1 (0x27)
      case 40: // dload_2 (0x28)
      case 41: // dload_3 (0x29)
      case 42: // aload_0 (0x2a)
      case 43: // aload_1 (0x2b)
      case 44: // aload_2 (0x2c)
      case 45: // aload_3 (0x2d)
      case 46: // iaload (0x2e)
      case 47: // laload (0x2f)
      case 48: // faload (0x30)
      case 49: // daload (0x31)
      case 50: // aaload (0x32)
      case 51: // baload (0x33)
      case 52: // caload (0x34)
      case 53: // saload (0x35)
      case 54: // istore (0x36)
      case 55: // lstore (0x37)
      case 56: // fstore (0x38)
      case 57: // dstore (0x39)
      case 58: // astore (0x3a)
      case 59: // istore_0 (0x3b)
      case 60: // istore_1 (0x3c)
      case 61: // istore_2 (0x3d)
      case 62: // istore_3 (0x3e)
      case 63: // lstore_0 (0x3f)
      case 64: // lstore_1 (0x40)
      case 65: // lstore_2 (0x41)
      case 66: // lstore_3 (0x42)
      case 67: // fstore_0 (0x43)
      case 68: // fstore_1 (0x44)
      case 69: // fstore_2 (0x45)
      case 70: // fstore_3 (0x46)
      case 71: // dstore_0 (0x47)
      case 72: // dstore_1 (0x48)
      case 73: // dstore_2 (0x49)
      case 74: // dstore_3 (0x4a)
      case 75: // astore_0 (0x4b)
      case 76: // astore_1 (0x4c)
      case 77: // astore_2 (0x4d)
      case 78: // astore_3 (0x4e)
      case 79: // iastore (0x4f)
      case 80: // lastore (0x50)
      case 81: // fastore (0x51)
      case 82: // dastore (0x52)
      case 83: // aastore (0x53)
      case 84: // bastore (0x54)
      case 85: // castore (0x55)
      case 86: // sastore (0x56)
      case 87: // pop (0x57)
        stack_ptr--;
        break;
      case 88: // pop2 (0x58)
        stack_ptr -= 2;
        break;
      case 89: // dup (0x59)
        temp = stack[stack_ptr];
        stack[stack_ptr++] = temp;
        break;
      case 90: // dup_x1 (0x5a)
      case 91: // dup_x2 (0x5b)
      case 92: // dup2 (0x5c)
      case 93: // dup2_x1 (0x5d)
      case 94: // dup2_x2 (0x5e)
      case 95: // swap (0x5f)
      case 96: // iadd (0x60)
      case 97: // ladd (0x61)
      case 98: // fadd (0x62)
      case 99: // dadd (0x63)
      case 100: // isub (0x64)
      case 101: // lsub (0x65)
      case 102: // fsub (0x66)
      case 103: // dsub (0x67)
      case 104: // imul (0x68)
      case 105: // lmul (0x69)
      case 106: // fmul (0x6a)
      case 107: // dmul (0x6b)
      case 108: // idiv (0x6c)
      case 109: // ldiv (0x6d)
      case 110: // fdiv (0x6e)
      case 111: // ddiv (0x6f)
      case 112: // irem (0x70)
      case 113: // lrem (0x71)
      case 114: // frem (0x72)
      case 115: // drem (0x73)
      case 116: // ineg (0x74)
      case 117: // lneg (0x75)
      case 118: // fneg (0x76)
      case 119: // dneg (0x77)
      case 120: // ishl (0x78)
      case 121: // lshl (0x79)
      case 122: // ishr (0x7a)
      case 123: // lshr (0x7b)
      case 124: // iushr (0x7c)
      case 125: // lushr (0x7d)
      case 126: // iand (0x7e)
      case 127: // land (0x7f)
      case 128: // ior (0x80)
      case 129: // lor (0x81)
      case 130: // ixor (0x82)
      case 131: // lxor (0x83)
      case 132: // iinc (0x84)
      case 133: // i2l (0x85)
      case 134: // i2f (0x86)
      case 135: // i2d (0x87)
      case 136: // l2i (0x88)
      case 137: // l2f (0x89)
      case 138: // l2d (0x8a)
      case 139: // f2i (0x8b)
      case 140: // f2l (0x8c)
      case 141: // f2d (0x8d)
      case 142: // d2i (0x8e)
      case 143: // d2l (0x8f)
      case 144: // d2f (0x90)
      case 145: // i2b (0x91)
      case 146: // i2c (0x92)
      case 147: // i2s (0x93)
      case 148: // lcmp (0x94)
      case 149: // fcmpl (0x95)
      case 150: // fcmpg (0x96)
      case 151: // dcmpl (0x97)
      case 152: // dcmpg (0x98)
      case 153: // ifeq (0x99)
      case 154: // ifne (0x9a)
      case 155: // iflt (0x9b)
      case 156: // ifge (0x9c)
      case 157: // ifgt (0x9d)
      case 158: // ifle (0x9e)
      case 159: // if_icmpeq (0x9f)
      case 160: // if_icmpne (0xa0)
      case 161: // if_icmplt (0xa1)
      case 162: // if_icmpge (0xa2)
      case 163: // if_icmpgt (0xa3)
      case 164: // if_icmple (0xa4)
      case 165: // if_acmpeq (0xa5)
      case 166: // if_acmpne (0xa6)
      case 167: // goto (0xa7)
      case 168: // jsr (0xa8)
      case 169: // ret (0xa9)
      case 170: // tableswitch (0xaa)
      case 171: // lookupswitch (0xab)
      case 172: // ireturn (0xac)
      case 173: // lreturn (0xad)
      case 174: // freturn (0xae)
      case 175: // dreturn (0xaf)
      case 176: // areturn (0xb0)
      case 177: // return (0xb1)
      case 178: // getstatic (0xb2)
      case 179: // putstatic (0xb3)
      case 180: // getfield (0xb4)
      case 181: // putfield (0xb5)
      case 182: // invokevirtual (0xb6)
      case 183: // invokespecial (0xb7)
      case 184: // invokestatic (0xb8)
      case 185: // invokeinterface (0xb9)
      case 186: // xxxunusedxxx1 (0xba)
      case 187: // new (0xbb)
      case 188: // newarray (0xbc)
      case 189: // anewarray (0xbd)
      case 190: // arraylength (0xbe)
      case 191: // athrow (0xbf)
      case 192: // checkcast (0xc0)
      case 193: // instanceof (0xc1)
      case 194: // monitorenter (0xc2)
      case 195: // monitorexit (0xc3)
        UNIMPL()
      case 196: // wide (0xc4)
        wide = 1;
        pc++;
        continue;
      case 197: // multianewarray (0xc5)
      case 198: // ifnull (0xc6)
      case 199: // ifnonnull (0xc7)
      case 200: // goto_w (0xc8)
      case 201: // jsr_w (0xc9)
      case 202: // breakpoint (0xca)
      case 203: // impdep1 (0xfe)
      case 204: // impdep1 (0xfe)
      case 205: // impdep1 (0xfe)
      case 206: // impdep1 (0xfe)
      case 207: // impdep1 (0xfe)
      case 208: // impdep1 (0xfe)
      case 209: // impdep1 (0xfe)
      case 210: // impdep1 (0xfe)
      case 211: // impdep1 (0xfe)
      case 212: // impdep1 (0xfe)
      case 213: // impdep1 (0xfe)
      case 214: // impdep1 (0xfe)
      case 215: // impdep1 (0xfe)
      case 216: // impdep1 (0xfe)
      case 217: // impdep1 (0xfe)
      case 218: // impdep1 (0xfe)
      case 219: // impdep1 (0xfe)
      case 220: // impdep1 (0xfe)
      case 221: // impdep1 (0xfe)
      case 222: // impdep1 (0xfe)
      case 223: // impdep1 (0xfe)
      case 224: // impdep1 (0xfe)
      case 225: // impdep1 (0xfe)
      case 226: // impdep1 (0xfe)
      case 227: // impdep1 (0xfe)
      case 228: // impdep1 (0xfe)
      case 229: // impdep1 (0xfe)
      case 230: // impdep1 (0xfe)
      case 231: // impdep1 (0xfe)
      case 232: // impdep1 (0xfe)
      case 233: // impdep1 (0xfe)
      case 234: // impdep1 (0xfe)
      case 235: // impdep1 (0xfe)
      case 236: // impdep1 (0xfe)
      case 237: // impdep1 (0xfe)
      case 238: // impdep1 (0xfe)
      case 239: // impdep1 (0xfe)
      case 240: // impdep1 (0xfe)
      case 241: // impdep1 (0xfe)
      case 242: // impdep1 (0xfe)
      case 243: // impdep1 (0xfe)
      case 244: // impdep1 (0xfe)
      case 245: // impdep1 (0xfe)
      case 246: // impdep1 (0xfe)
      case 247: // impdep1 (0xfe)
      case 248: // impdep1 (0xfe)
      case 249: // impdep1 (0xfe)
      case 250: // impdep1 (0xfe)
      case 251: // impdep1 (0xfe)
      case 252: // impdep1 (0xfe)
      case 253: // impdep1 (0xfe)
      case 254: // impdep1 (0xfe)
      case 255: // impdep2 (0xff)
        UNIMPL()
    }
#endif

    if (stack_ptr < 0 || stack_ptr > max_stack)
    {
      printf("Stack error: stack_ptr=%d max_stack=%d\n", stack_ptr, max_stack);
      return -1;
    }

    if (wide == 1) { pc += table_java_instr[bytes[pc]].normal; }
    else { pc += table_java_instr[bytes[pc]].wide; }

    wide = 0;
  }



  return 0;
}


