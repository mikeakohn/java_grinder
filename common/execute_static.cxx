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

#include "Generator.h"
#include "JavaClass.h"
#include "stack.h"
#include "table_java_instr.h"

#define DEBUG_PRINT(a, ...) if (verbose == 1) { printf(a, ##__VA_ARGS__); }

// Static fields are done awfuly strange in Java.  Well, mostly it's the
// arrays that are done oddly.  So I think the trick is to actually run
// the Java code to create the arrays in memory.  Then need to figure out
// how to add the other code to the init section.  Maybe the trick is to
// generate a brand new Java function and compile it.

#define UNIMPL() { printf("Unimplemented opcode=%d %s:%d\n", bytes[pc], __FILE__, __LINE__); ret = -1; break; }
#define CHECK_STACK(n) \
        if (stack->length() < n) \
        { \
          printf("Error: stack < %d\n", n); \
          ret = -1; \
          break; \
        }
#define CHECK_BOUNDS() \
        if (index > array_len) \
        { \
          printf("Error: array index %d out of bounds\n", index); \
          ret = -1; \
          break; \
        }

int execute_static(JavaClass *java_class, int method_id, Generator *generator, bool do_arrays, bool verbose, JavaClass *parent_class)
{
  struct methods_t *method = java_class->get_method(method_id);
  uint8_t *bytes = method->attributes[0].info;
  generic_32bit_t *gen32;
  int pc;
  int wide = 0;
  int pc_start;
  int max_stack;
  int max_locals;
  int code_len;
  int32_t temp;
  int32_t value;
  float value_float;
  int32_t *value_bin;
  _stack *stack;
  int32_t *array = NULL;
  int array_len = -1;
  int array_type = -1;
  int array_alloc_size = 0;
  std::string field_name;
  std::string type;
  int index;
  int ret = 0;

  if (verbose)
  {
    printf("--- Executing static code\n");
  }

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

  DEBUG_PRINT("max_stack=%d max_locals=%d code_len=%d\n", max_stack, max_locals, code_len);

  //generator->method_start(max_locals, method_name);
  stack = (_stack *)alloca(max_stack * sizeof(int32_t) + sizeof(int32_t));
  stack->reset();

  while(pc - pc_start < code_len)
  {
    int address = pc - pc_start;
#ifdef DEBUG
    DEBUG_PRINT("pc=%d %s opcode=%d (0x%02x)\n", address, table_java_instr[bytes[pc]].name, bytes[pc], bytes[pc]);
#endif

    switch(bytes[pc])
    {
      case 0: // nop (0x00)
      case 1: // aconst_null (0x01)
        stack->push(0);
        break;
      case 2: // iconst_m1 (0x02)
      case 3: // iconst_0 (0x03)
      case 4: // iconst_1 (0x04)
      case 5: // iconst_2 (0x05)
      case 6: // iconst_3 (0x06)
      case 7: // iconst_4 (0x07)
      case 8: // iconst_5 (0x08)
        stack->push(bytes[pc] - 0x03);
        break;
      case 9:  // lconst_0 (0x09)
      case 10: // lconst_1 (0x0a)
        stack->push(bytes[pc] - 0x09);
        break;
      case 11: // fconst_0 (0x0b)
        value_float = 0.0;
        value_bin = (int32_t *)&value_float;
        stack->push(*value_bin);
        break;
      case 12: // fconst_1 (0x0c)
        value_float = 1.0;
        value_bin = (int32_t *)&value_float;
        stack->push(*value_bin);
        break;
      case 13: // fconst_2 (0x0d)
        value_float = 2.0;
        value_bin = (int32_t *)&value_float;
        stack->push(*value_bin);
        break;
      case 14: // dconst_0 (0x0e)
      case 15: // dconst_1 (0x0f)
        UNIMPL();
      case 16: // bipush (0x10)
        stack->push((int8_t)bytes[pc+1]);
        break;
      case 17: // sipush (0x11)
        stack->push((int16_t)((bytes[pc+1] << 8) | bytes[pc+2]));
        break;
      case 18: // ldc (0x12)
        index = bytes[pc+1];
      case 19: // ldc_w (0x13)
        if (bytes[pc] == 0x13) { index = (bytes[pc+1] << 8) | bytes[pc+2]; }

        DEBUG_PRINT("  index=%d\n", index);

        gen32 = (generic_32bit_t *)java_class->get_constant(index);

        if (gen32->tag == CONSTANT_INTEGER)
        {
          stack->push(gen32->value);
        }
          else
        if (gen32->tag == CONSTANT_FLOAT)
        {
          stack->push(gen32->value);
        }
          else
        if (gen32->tag == CONSTANT_STRING)
        {
          constant_string_t *constant_string = (constant_string_t *)gen32;

          if (bytes[pc] == 18 && bytes[pc+2] == 179)
          {
            index = (bytes[pc+3] << 8) | bytes[pc+4];
            pc += 2;
          }
            else
          if (bytes[pc] == 19 && bytes[pc+3] == 179)
          {
            index = (bytes[pc+4] << 8) | bytes[pc+5];
            pc += 3;
          }
            else
          {
            printf("Error: Why is there no putstatic?\n");
            ret = -1;
            break;
          }

          if (java_class->get_ref_name_type(field_name, type, index) != 0)
          {
            printf("Error retrieving field name %d\n", constant_string->string_index);
            ret = -1;
            break;
          }

          DEBUG_PRINT("  String %s;\n", field_name.c_str());

          if (do_arrays)
          {
            constant_utf8_t *constant_utf8 =(constant_utf8_t *)java_class->get_constant(constant_string->string_index);

            if (constant_utf8 == NULL)
            {
              printf("Error retrieving constant %s\n", field_name.c_str());
              ret = -1;
              break;
            }

            generator->insert_string(field_name, constant_utf8->bytes, constant_utf8->length);
          }
            else
          {
            generator->field_init_ref(field_name, index);
          }

          //printf("Can't do a string yet.. :( %d\n", constant_string->string_index);
          //ret = -1;
        }
          else
        {
          printf("Cannot ldc this type %d=>'%s' pc=%d\n", gen32->tag, JavaClass::tag_as_string(gen32->tag), pc);
          ret = -1;
        }
        break;
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
        UNIMPL();
      case 79: // iastore (0x4f)
        CHECK_STACK(3);
        value = stack->pop();
        index = stack->pop();
        CHECK_BOUNDS();
        array[index] = value;
        stack->pop();
        break;
      case 80: // lastore (0x50)
        UNIMPL();
      case 81: // fastore (0x51)
        CHECK_STACK(3);
        //value_float = stack->pop_float();
        value = stack->pop();
        //value_bin = (int32_t *)&value_float;
        index = stack->pop();
        CHECK_BOUNDS();
        //array[index] = *value_bin;
        array[index] = value;
        stack->pop();
        break;
      case 82: // dastore (0x52)
      case 83: // aastore (0x53)
        UNIMPL();
      case 84: // bastore (0x54)
        CHECK_STACK(3);
        value = stack->pop();
        index = stack->pop();
        CHECK_BOUNDS();
        array[index] = value;
        //stack_ptr -= 3;
        stack->pop();
        break;
      case 85: // castore (0x55)
        UNIMPL();
      case 86: // sastore (0x56)
        CHECK_STACK(3);
        value = stack->pop();
        index = stack->pop();
        //printf("index=%d value=%d\n", index, stack_ptr-1);
        CHECK_BOUNDS();
        array[index] = value;
        //stack_ptr -= 3;
        stack->pop();
        break;
      case 87: // pop (0x57)
        stack->pop();
        break;
      case 88: // pop2 (0x58)
        stack->pop();
        stack->pop();
        break;
      case 89: // dup (0x59)
        CHECK_STACK(1)
        temp = stack->pop();
        stack->push(temp);
        stack->push(temp);
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
        UNIMPL()
      case 177: // return (0xb1)
        ret = 1;
        break;
      case 178: // getstatic (0xb2)
        UNIMPL()
      case 179: // putstatic (0xb3)
        CHECK_STACK(1);
        index = (bytes[pc+1] << 8) | bytes[pc+2];
        temp = stack->pop();

        DEBUG_PRINT("id=%d index=%d\n", temp, index);

        //stack_ptr--;  // <-- this is our made up index which here is always 0
        if (java_class->get_ref_name_type(field_name, type, index) != 0)
        {
          printf("Error retrieving field name %d\n", temp);
          ret = -1;
          break;
        }

        DEBUG_PRINT("field_name=%s type=%s len=%d\n",
           field_name.c_str(), type.c_str(), array_len);

        if (type[0] == '[')
        {
          DEBUG_PRINT("array_type=%d\n", array_type);

          if (do_arrays)
          {
            // DEBUG output
            if (verbose)
            {
              for (int n = 0; n < array_len; n++)
              {
                printf(" %d", array[n]);
              }
              printf("\n");
            }

            if (array_type == ARRAY_TYPE_BOOLEAN ||
                array_type == ARRAY_TYPE_BYTE)
            {
              generator->insert_array(field_name, array, array_len, TYPE_BYTE);
            }
              else
            if (array_type == ARRAY_TYPE_CHAR ||
                array_type == ARRAY_TYPE_SHORT)
            {
              generator->insert_array(field_name, array, array_len, TYPE_SHORT);
            }
              else
            if (array_type == ARRAY_TYPE_INT)
            {
              generator->insert_array(field_name, array, array_len, TYPE_INT);
            }
              else
            if (array_type == ARRAY_TYPE_FLOAT)
            {
              generator->insert_array(field_name, array, array_len, TYPE_FLOAT);
            }
              else
            {
              printf("Unsupported array type\n");
              ret = -1;
            }
          }
            else
          {
            index = java_class->get_field_index(field_name);

            generator->field_init_ref(field_name, index);
          }
        }
          else
        if (!do_arrays)
        {
          int value = temp;
          index = java_class->get_field_index(field_name);

          if (index == -1)
          {
            printf("Couldn't find %s\n", field_name.c_str());
            ret = -1;
            break;
          }

          if (type == "Z") // boolean
          {
            value = value == 0 ? 0 : 1;
            ret = generator->field_init_int(field_name, index, value);
          }
            else
          if (type == "B") // byte
          {
            ret = generator->field_init_int(field_name, index, value);
          }
            else
          if (type == "S") // short
          {
            ret = generator->field_init_int(field_name, index, value);
          }
            else
          if (type == "C") // char (unsigned short)
          {
            ret = generator->field_init_int(field_name, index, value);
          }
            else
          if (type == "I") // int
          {
            ret = generator->field_init_int(field_name, index, value);
          }
            else
          {
            ret = -1;
          }
        }
        break;
      case 180: // getfield (0xb4)
        UNIMPL()
      case 181: // putfield (0xb5)
      case 182: // invokevirtual (0xb6)
      case 183: // invokespecial (0xb7)
      case 184: // invokestatic (0xb8)
      case 185: // invokeinterface (0xb9)
      case 186: // xxxunusedxxx1 (0xba)
      case 187: // new (0xbb)
        UNIMPL()
      case 188: // newarray (0xbc)
        CHECK_STACK(1);
        temp = stack->pop();
        array_len = temp;
        stack->push(0); // FIXME - put the new array on the stack
        array_type = bytes[pc+1];

        DEBUG_PRINT("array_len=%d type=%d\n", array_len, array_type);

        if (array_len > array_alloc_size)
        {
          array_alloc_size = array_len < 8192 ? 8192 : array_len;
          array = (int32_t *)realloc(array, array_alloc_size * sizeof(int32_t));
          memset(array, 0, array_len * sizeof(int32_t));
        }
        // mkohn 2014-Mar-9 - Why was this here?
        //stack_ptr--;
        break;
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

    if (ret != 0) { break; }
    if (stack->length() < 0 || stack->length() > max_stack)
    {
      printf("Stack error: stack_ptr=%d max_stack=%d\n", stack->length(), max_stack);
      ret = -1;
      break;
    }

    if (wide == 1) { pc += table_java_instr[bytes[pc]].wide; }
    else { pc += table_java_instr[bytes[pc]].normal; }

    wide = 0;
  }

  DEBUG_PRINT("stack->length()=%d after execute ends\n", stack->length());

  // printf("EXIT pc=%d ret=%d code_len=%d\n", pc, ret, code_len);
  if (array != NULL) { free(array); }
  if (ret == -1) { return -1; }

  return 0;
}

