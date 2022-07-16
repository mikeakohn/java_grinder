/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2022 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "common/JavaClass.h"
#include "common/JavaCompiler.h"
#include "common/Util.h"
#include "common/execute_static.h"
#include "common/table_java_instr.h"
#include "api/invoke_static.h"
#include "api/invoke_virtual.h"

// http://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html

#define UNIMPL() printf("Opcode (%d) '%s' unimplemented\n", bytes[pc], table_java_instr[(int)bytes[pc]].name); ret = -1;

//#define CONST_STACK_SIZE 4

static inline bool needs_label(uint8_t *label_map, int pc, int pc_start)
{
  int address = pc - pc_start;
  if ((label_map[address / 8] & (1 << (address % 8))) != 0)
  {
    return true;
  }

  return false;
}

static int calc_distance(uint8_t *bytes, int pc, int pc_jump_to)
{
  int count = 0;

  if (pc > pc_jump_to)
  {
    int temp = pc_jump_to;
    pc_jump_to = pc;
    pc = temp;
  }

  while (pc < pc_jump_to)
  {
    if (bytes[pc] == 0xc4)
    {
      pc++;
      pc += table_java_instr[bytes[pc]].wide;
    }
      else
    {
      pc += table_java_instr[bytes[pc]].normal;
    }

    count++;
  }

  assert(pc == pc_jump_to);

  return count;
}

uint8_t JavaCompiler::cond_table[] =
{
  COND_EQUAL,         // 159 (0x9f) if_icmpeq
  COND_NOT_EQUAL,     // 160 (0xa0) if_icmpne
  COND_LESS,          // 161 (0xa1) if_icmplt
  COND_GREATER_EQUAL, // 162 (0xa2) if_icmpge
  COND_GREATER,       // 163 (0xa3) if_icmpgt
  COND_LESS_EQUAL,    // 164 (0xa4) if_icmple
};

const char *JavaCompiler::type_table[] =
{
  "[",  // 0 invalid
  //"[",  // 1 invalid
  //"[",  // 2 invalid
  //"[",  // 3 invalid
  "[Z", // 4 boolean
  "[C", // 5 char
  "[F", // 6 float
  "[D", // 7 double
  "[B", // 8 byte
  "[S", // 9 short
  "[I", // 10 int
  "[J", // 11 long
  "[Ljava/lang/String;", //
  NULL
};

JavaCompiler::JavaCompiler() :
  java_class(NULL)
{
  classpath[0] = 0;
}

JavaCompiler::~JavaCompiler()
{
  // Delete external classes
  std::map<std::string,JavaClass *>::iterator iter;
  for (iter = external_classes.begin(); iter != external_classes.end(); iter++)
  {
    JavaClass *java_class_external = iter->second;
    delete java_class_external;
  }

  delete java_class;
  fclose(in);
}

int JavaCompiler::find_external_fields(JavaClass *java_class, bool is_parent)
{
  int index;
  int external_field_count = 0;
  int constant_count = java_class->get_constant_count();

  // Collect list of external fields
  for (index = 0; index < constant_count; index++)
  {
    constant_fieldref_t *constant_fieldref =
      (constant_fieldref_t *)java_class->get_constant(index);

    if (constant_fieldref->tag == CONSTANT_FIELDREF ||
        constant_fieldref->tag == CONSTANT_METHODREF)
    {
      // Ignore fields that are in the parent class
      if (is_parent &&
          constant_fieldref->class_index == java_class->get_this_class())
      {
        continue;
      }

      // If this field / method is in the API, then skip it.  Only
      // need to find all classes that have to be loaded and their
      // fields so memory can be allocated properly.
      if (java_class->is_ref_in_api(index)) { continue; }

      std::string class_name;
      std::string field_name;
      std::string field_type;

      java_class->get_ref_name_type(field_name, field_type, index);

      java_class->get_class_name(class_name, index);

      DEBUG_PRINT("CLASSNAME '%s' field='%s'\n", class_name.c_str(), field_name.c_str());

      // If this field / method exists outside of this class...
      if (class_name != java_class->get_this_class_name())
      {
        // If this reference is to something outside of this class,
        // recursively load this class.
        std::map<std::string,JavaClass *>::iterator iter;

        iter = external_classes.find(class_name.c_str());
        if (iter == external_classes.end())
        {
          std::string filename = classpath + class_name + ".class";

          DEBUG_PRINT("find_external_fields: fopen('%s')\n", filename.c_str());

          FILE *in = fopen(filename.c_str(), "rb");

          if (in == NULL)
          {
            printf("Cannot open '%s'\n", filename.c_str());
            return -1;
          }

          JavaClass *java_class_external = new JavaClass(in, false);
          external_classes[class_name.c_str()] = java_class_external;

          fclose(in);

          external_field_count += find_external_fields(java_class_external, false);
        }
      }
        else
      {
        // Record this field in list of external fields.
        if (constant_fieldref->tag == CONSTANT_FIELDREF)
        {
          DEBUG_PRINT("  adding: %s\n", field_name.c_str());

          external_fields[field_name] = field_type_to_int(field_type.c_str());
          external_field_count++;
        }
#if 0
          else
        if (constant_fieldref->tag == CONSTANT_METHODREF)
        {
          // Do external methods need to be kept track of?
        }
#endif
      }
    }
  }

  return external_field_count;
}

void JavaCompiler::fill_label_map(
  uint8_t *label_map,
  int label_map_len,
  uint8_t *bytes,
  int code_len,
  int pc_start)
{
  int pc = pc_start;
  int wide = 0;
  int address;

  memset(label_map, 0, label_map_len);

  while (pc - pc_start < code_len)
  {
    int opcode = bytes[pc];

    if (opcode == 0xc4) { wide = 1; pc++; continue; }

    switch (opcode)
    {
        case 0x99:  // ifeq
        case 0x9a:  // ifne
        case 0x9b:  // iflt
        case 0x9c:  // ifge
        case 0x9d:  // ifgt
        case 0x9e:  // ifle
        case 0x9f:  // if_icmpeq
        case 0xa0:  // if_icmpne
        case 0xa1:  // if_icmplt
        case 0xa2:  // if_icmpge
        case 0xa3:  // if_icmpgt
        case 0xa4:  // if_icmple
        case 0xa5:  // if_acmpeq
        case 0xa6:  // if_acmpne
        case 0xa7:  // goto
        case 0xa8:  // jsr
        case 0xc6:  // ifnull
        case 0xc7:  // ifnonnull
        {
          int16_t offset = GET_PC_INT16(1);
          address = (pc + offset) - pc_start;
          if (address < 0) { printf("Internal error: %s:%d\n", __FILE__, __LINE__); return; }
          label_map[address / 8] |= 1 << (address % 8);
          break;
        }
        case 0xc8:  // goto_w
        case 0xc9:  // jsr_w
        {
          int32_t offset = GET_PC_INT32(1);
          address = (pc + offset) - pc_start;
          if (address < 0) { printf("Internal error: %s:%d\n", __FILE__, __LINE__); return; }
          label_map[address / 8] |= 1 << (address % 8);
          break;
        }
      default:
        break;
    }

    if (wide == 1)
    {
      pc += table_java_instr[opcode].wide;
      wide = 0;
    }
      else
    {
      pc += table_java_instr[opcode].normal;
    }
  }
}

// FIXME - Too many parameters :(.
int JavaCompiler::optimize_const(
  JavaClass *java_class,
  std::string &method_name,
  uint8_t *bytes,
  int pc,
  int pc_end,
  int address,
  int const_val)
{
  int const_vals[2];

  if (!optimize) { return 0; }

  if (pc + table_java_instr[bytes[pc]].normal > pc_end)
  {
    return 0;
  }

  switch (bytes[pc])
  {
    case 54: // istore (0x36)
      if (generator->set_integer_local(bytes[pc + 1], const_val) != 0)
      {
        return 0;
      }

      return 2;
    case 59: // istore_0 (0x3b)
    case 60: // istore_1 (0x3c)
    case 61: // istore_2 (0x3d)
    case 62: // istore_3 (0x3e)
      if (generator->set_integer_local(bytes[pc] - 0x3b, const_val) != 0)
      {
        return 0;
      }

      return 1;
    case 96: // iadd (0x60)
      if (generator->add_integer(const_val) != 0) { return 0; }
      return 1;
    case 100: // isub (0x64)
      if (generator->sub_integer(const_val) != 0) { return 0; }
      return 1;
    case 120: // ishl (0x78)
      if (generator->shift_left_integer(const_val) != 0) { return 0; }
      return 1;
    case 122: // ishr (0x7a)
      if (generator->shift_right_integer(const_val) != 0) { return 0; }
      return 1;
    case 124: // iushr (0x7c)
      if (generator->shift_right_uinteger(const_val) != 0) { return 0; }
      return 1;
    case 126: // iand (0x7e)
      if (generator->and_integer(const_val) != 0) { return 0; }
      return 1;
    case 128: // ior (0x80)
      if (generator->or_integer(const_val) != 0) { return 0; }
      return 1;
    case 130: // ixor (0x82)
      if (generator->xor_integer(const_val) != 0) { return 0; }
      return 1;
    case 159: // if_icmpeq (0x9f)
    case 160: // if_icmpne (0xa0)
    case 161: // if_icmplt (0xa1)
    case 162: // if_icmpge (0xa2)
    case 163: // if_icmpgt (0xa3)
    case 164: // if_icmple (0xa4)
    {
      //int start = pc - address;
      int ret = try_ternary(bytes, pc_end, pc, true, const_val);

      if (ret != -1)
      {
        return ret + 3;
      }

      std::string label;
      int byte_count = GET_PC_INT16(1);
      int jump_to = address + byte_count;

      label = method_name + "_" + std::to_string(jump_to);

      if (generator->jump_cond_integer(label, cond_table[bytes[pc] - 159], const_val, calc_distance(bytes, pc, pc + byte_count)) == -1)
      {
        return 0;
      }

      return 3;
    }
    default:
      break;
  }

  // istore wide
  if (pc + 2 < pc_end && bytes[pc] == 0xc4 && bytes[pc + 1] == 0x36)
  {
    if (generator->set_integer_local(GET_PC_UINT16(1), const_val) != 0)
    {
      return 0;
    }

    return 3;
  }

  // invokestatic with one const
  if (pc + 2 < pc_end && bytes[pc] == 0xb8)
  {
    int ref = GET_PC_UINT16(1);

    if (invoke_static(java_class, ref, generator, &const_val, 1) != 0)
    {
      return 0;
    }

    return 3;
  }

  // invokestatic with two const
  // 02 (0x02) iconst_m1
  // 03 (0x03) iconst_0
  // 04 (0x04) iconst_1
  // 05 (0x05) iconst_2
  // 06 (0x06) iconst_3
  // 07 (0x07) iconst_4
  // 08 (0x08) iconst_5
  if (pc + 3 < pc_end &&
      bytes[pc] >= 0x02 &&
      bytes[pc] <= 0x08 &&
      bytes[pc + 1] == 0xb8)
  {
    const_vals[0] = const_val;
    const_vals[1] = (int8_t)bytes[pc] - 3;

    int ref = GET_PC_UINT16(2);

    if (invoke_static(java_class, ref, generator, const_vals, 2) != 0)
    {
      return 0;
    }

    return 4;
  }

  // invokestatic with two const
  // 16 (0x10) bipush
  if (pc + 4 < pc_end &&
      bytes[pc + 0] == 0x10 &&
      bytes[pc + 2] == 0xb8)
  {
    const_vals[0] = const_val;
    const_vals[1] = (int8_t)bytes[pc + 1];

    int ref = GET_PC_UINT16(3);

    if (invoke_static(java_class, ref, generator, const_vals, 2) != 0)
    {
      return 0;
    }

    return 5;
  }

  // FIXME - add more invoke(const,const) combinations.

  return 0;
}

int JavaCompiler::optimize_const(
  JavaClass *java_class,
  std::string &method_name,
  uint8_t *bytes,
  int pc,
  int pc_end,
  int address,
  const char *const_val)
{
  // invokestatic with one String const
  if (pc + 2 < pc_end && bytes[pc] == 0xb8)
  {
    int ref = GET_PC_UINT16(1);

    if (invoke_static(java_class, ref, generator, const_val) != 0)
    {
      return 0;
    }

    return 3;
  }

  return 0;
}

int JavaCompiler::optimize_compare(
  JavaClass *java_class,
  std::string &method_name,
  uint8_t *bytes,
  int pc,
  int pc_end,
  int address,
  int index)
{
  int local_index = -1;
  bool check_for_compare = false;
  int skip_bytes = 0;
  int wide = 0;

  // If we did some kind of ALU instruction, it's maybe possible we don't
  // have to do a cmp #0, reg which would require pushing on the stack
  // and then doing a compare and then doing a jmp.  Might as well just jmp.

  if (!optimize) { return 0; }

  if (pc + table_java_instr[bytes[pc]].normal > pc_end)
  {
    return 0;
  }

  if (bytes[pc] == 0xc4) { skip_bytes++; pc++; wide = 1; }

  switch (bytes[pc])
  {
    case 21: // iload (0x15)
      if (wide == 1)
      {
        local_index = GET_PC_UINT16(1);
        skip_bytes += 3;
      }
        else
      {
        local_index = bytes[pc + 1];
        skip_bytes += 2;
      }
      if (local_index == index) { check_for_compare = true; }
      break;
    case 26: // iload_0 (0x1a)
    case 27: // iload_1 (0x1b)
    case 28: // iload_2 (0x1c)
    case 29: // iload_3 (0x1d)
      // Push a local integer variable on the stack
      local_index = bytes[pc] - 26;
      if (local_index == index)
      {
        check_for_compare = true;
        skip_bytes = 1;
      }
      break;
    default:
      break;
  }

  if (check_for_compare)
  {
    // ifeq (0x99)
    // ifne (0x9a)
    if (bytes[pc+skip_bytes] == 153 || bytes[pc+skip_bytes] == 154)
    {
      int cond = bytes[pc + skip_bytes] - 153;
      std::string label;

      pc += skip_bytes;
      address += skip_bytes;

      int byte_count = GET_PC_INT16(1);
      int jump_to = address + byte_count;

      label = method_name + "_" + std::to_string(jump_to);

      if (generator->jump_cond_zero(label, cond, calc_distance(bytes, pc, pc + byte_count)) != -1)
      {
        return skip_bytes + 3;
      }
    }
  }

  return 0;
}

int JavaCompiler::array_load(
  JavaClass *java_class,
  int constant_id,
  uint8_t array_type)
{
  generic_32bit_t *gen32;
  std::string field_name;
  std::string type;

  gen32 = (generic_32bit_t *)java_class->get_constant(constant_id);

  if (gen32->tag == CONSTANT_FIELDREF)
  {
    constant_fieldref_t *field_ref = (struct constant_fieldref_t *)gen32;

    DEBUG_PRINT("FIELD_REF: class_index=%d name_and_type=%d\n",
                field_ref->class_index, field_ref->name_and_type_index);

    if (java_class->get_ref_name_type(field_name, type, constant_id) != 0)
    {
      printf("Error retrieving field name const_index=%d\n", constant_id);
      return -1;
    }

    // FIXME - Does this come from the array or from the instruction
    if (array_type == ARRAY_TYPE_BYTE)
    {
      return generator->array_read_byte(field_name, 0);
    }
      else
    if (array_type == ARRAY_TYPE_SHORT)
    {
      return generator->array_read_short(field_name, 0);
    }
      else
    if (array_type == ARRAY_TYPE_INT)
    {
      return generator->array_read_int(field_name, 0);
    }
      else
    if (array_type == ARRAY_TYPE_FLOAT)
    {
      return generator->array_read_float(field_name, 0);
    }
      else
    if (array_type == ARRAY_TYPE_LONG)
    {
      return generator->array_read_long(field_name, 0);
    }
      else
    if (array_type == ARRAY_TYPE_OBJECT)
    {
      return generator->array_read_object(field_name, 0);
    }
  }
    else
  {
    printf("Error: tag not supported\n");
    return -1;
  }

  return -1;
}

int JavaCompiler::array_store(
  JavaClass *java_class,
  int constant_id,
  uint8_t array_type)
{
  generic_32bit_t *gen32;
  std::string field_name;
  std::string type;

  gen32 = (generic_32bit_t *)java_class->get_constant(constant_id);

  if (gen32->tag == CONSTANT_FIELDREF)
  {
    constant_fieldref_t *field_ref = (struct constant_fieldref_t *)gen32;

    DEBUG_PRINT("class_index=%d name_and_type=%d\n",
                field_ref->class_index, field_ref->name_and_type_index);

    if (java_class->get_ref_name_type(field_name, type, constant_id) != 0)
    {
      printf("Error retrieving field name const_index=%d\n", constant_id);
      return -1;
    }

    // FIXME - Does this come from the array or from the instruction
    if (array_type == ARRAY_TYPE_BYTE)
    {
      return generator->array_write_byte(field_name, 0);
    }
      else
    if (array_type == ARRAY_TYPE_SHORT)
    {
      return generator->array_write_short(field_name, 0);
    }
      else
    if (array_type == ARRAY_TYPE_INT)
    {
      return generator->array_write_int(field_name, 0);
    }
      else
    if (array_type == ARRAY_TYPE_FLOAT)
    {
      return generator->array_write_float(field_name, 0);
    }
      else
    if (array_type == ARRAY_TYPE_LONG)
    {
      return generator->array_write_long(field_name, 0);
    }
      else
    if (array_type == ARRAY_TYPE_OBJECT)
    {
      return generator->array_write_object(field_name, 0);
    }
  }
    else
  {
    printf("Error: tag not supported\n");
    return -1;
  }

  return -1;
}

int JavaCompiler::push_ref(int index, Stack &stack)
{
  int ref = stack.pop();
  std::string field_name;
  std::string type;
  int ret;

  if (java_class->get_ref_name_type(field_name, type, ref) != 0)
  {
    printf("Error retrieving field name %d\n", ref);
    return -1;
  }

  // Try to set local variable directly to the address of the static
  if (optimize)
  {
    ret = generator->set_ref_local(index, field_name);
  }
    else
  {
    ret = -1;
  }

  if (ret == -1)
  {
    ret = generator->push_ref(field_name);
    ret |= generator->pop_local_var_ref(index);
  }

  return ret;
}

int JavaCompiler::compile_method(
   JavaClass *java_class,
   int method_id,
   const char *alt_name)
{
  struct methods_t *method = java_class->get_method(method_id);
  uint8_t *bytes = method->attributes[0].info;
  int pc;
  const float fzero = 0.0;
  const float fone = 1.0;
  const float ftwo = 2.0;
  int wide = 0;
  int pc_start;
  int max_stack;
  int max_locals;
  int param_count;
  int code_len;
  uint32_t ref;
  struct generic_32bit_t *gen32;
  struct constant_float_t *constant_float;
  uint8_t *label_map;
  int ret = 0;
  std::string label;
  std::string method_name;
  std::string class_name;
  Stack stack;
  int const_val;
  int skip_bytes;
  int index;
  int instruction_length;

  if (java_class->get_method_name(method_name, method_id) != 0)
  {
    method_name = "error";
  }

  if (alt_name != NULL) { method_name = alt_name; }

  DEBUG_PRINT("--- Compiling method '%s' method_id=%d\n",
    method_name.c_str(), method_id);

  if (method_name == "<init>" || method_name == "")
  {
    DEBUG_PRINT("Skipping method <--\n");

    return 0;
  }

#if 0
  // This breaks interface methods
  if ((method->access_flags & ACC_STATIC) == 0)
  {
    printf("Error: Method %s is not static %s:%d.\n", method_name, __FILE__, __LINE__);
    return -1;
  }
#endif

  param_count = 0;

  if (method_name != "main")
  {
    std::string method_sig;

    java_class->get_name_constant(method_sig, method->descriptor_index);

    Util::method_sanitize(method_name, method_sig, param_count);

    DEBUG_PRINT("Using method name '%s' param_count=%d\n",
                method_name.c_str(), param_count);
  }
    else
  {
    param_count = 1;
  }

  // bytes points to the method attributes info for the method.
  max_stack  = ((int)bytes[0] << 8) | (int)bytes[1];
  max_locals = ((int)bytes[2] << 8) | (int)bytes[3];

  code_len = ((int)bytes[4] << 24) |
             ((int)bytes[5] << 16) |
             ((int)bytes[6] << 8) |
             ((int)bytes[7]);

  pc_start = (((int)bytes[code_len + 8] << 8) |
              ((int)bytes[code_len + 9])) + 8;
  pc = pc_start;

  generator->method_start(max_locals, max_stack, param_count, method_name);
  stack.reset();

  int label_map_len = (code_len / 8) + 1;
  label_map = (uint8_t *)alloca(label_map_len);
  fill_label_map(label_map, label_map_len, bytes, code_len, pc_start);

#ifdef DEBUG
  DEBUG_PRINT("pc=%d\n", pc);
  DEBUG_PRINT("max_stack=%d\n", max_stack);
  DEBUG_PRINT("max_locals=%d\n", max_locals);
  DEBUG_PRINT("code_len=%d\n", code_len);
#endif

  generator->instruction_count_clear();

  while (pc - pc_start < code_len)
  {
    int address = pc - pc_start;
    skip_bytes = 0;
#ifdef DEBUG
    DEBUG_PRINT("pc=%d %s opcode=%d (0x%02x)\n", address, table_java_instr[bytes[pc]].name, bytes[pc], bytes[pc]);
#endif
    //if ((label_map[address / 8] & (1 << (address % 8))) != 0)
    if (needs_label(label_map, pc, pc_start))
    {
      //sprintf(label, "%s_%d", method_name, address);
      label = method_name + "_" + std::to_string(address);
      generator->label(label);
    }

    // Instruction count can be used for optimization.  For example if
    // next instruction is an array access to the same array, maybe it's
    // possible to unpop the array pointer from the stack.
    generator->instruction_count_inc();

    switch (bytes[pc])
    {
      case 0: // nop (0x00)
        break;

      case 1: // aconst_null (0x01)
        ret = generator->push_int(0);
        break;

      case 2: // iconst_m1 (0x02)
      case 3: // iconst_0 (0x03)
      case 4: // iconst_1 (0x04)
      case 5: // iconst_2 (0x05)
      case 6: // iconst_3 (0x06)
      case 7: // iconst_4 (0x07)
      case 8: // iconst_5 (0x08)
        const_val = uint8_t(bytes[pc]) - 3;

        ret = optimize_const(
          java_class,
          method_name,
          bytes,
          pc + 1,
          pc_start + code_len,
          address + 1,
          const_val);

        if (ret == 0)
        {
          ret = generator->push_int(const_val);
        }
          else
        {
          skip_bytes = ret;
          ret = 0;
        }
        break;

      case 9:  // lconst_0 (0x09)
      case 10: // lconst_1 (0x0a)
        ret = generator->push_long(bytes[pc]-9);
        break;

      case 11: // fconst_0 (0x0b)
        ret = generator->push_float(fzero);
        break;

      case 12: // fconst_1 (0x0c)
        ret = generator->push_float(fone);
        break;

      case 13: // fconst_2 (0x0d)
        ret = generator->push_float(ftwo);
        break;

      case 14: // dconst_0 (0x0e)
        ret = generator->push_double(fzero);
        break;

      case 15: // dconst_1 (0x0f)
        ret = generator->push_double(fone);
        break;

      case 16: // bipush (0x10)
        const_val = (int8_t)bytes[pc + 1];

        ret = optimize_const(
          java_class,
          method_name,
          bytes,
          pc + 2,
          pc_start + code_len,
          address + 2,
          const_val);

        if (ret == 0)
        {
          ret = generator->push_int(const_val);
        }
          else
        {
          skip_bytes = ret;
          ret = 0;
        }
        break;

      case 17: // sipush (0x11)
        const_val = (int16_t)((bytes[pc + 1] << 8) | bytes[pc + 2]);

        ret = optimize_const(
          java_class,
          method_name,
          bytes,
          pc + 3,
          pc_start + code_len,
          address + 3,
          const_val);

        if (ret == 0)
        {
          ret = generator->push_int(const_val);
        }
          else
        {
          skip_bytes = ret;
          ret = 0;
        }
        break;

      case 18: // ldc (0x12)
        index = bytes[pc + 1];
        instruction_length = 2;
      case 19: // ldc_w (0x13)
        if (bytes[pc] == 0x13)
        {
          index = GET_PC_UINT16(1);
          instruction_length = 3;
        }

        gen32 = (generic_32bit_t *)java_class->get_constant(index);

        DEBUG_PRINT("  index=%d tag=%d\n", index, gen32->tag);

        if (gen32->tag == CONSTANT_INTEGER)
        {
          const_val = gen32->value;

          ret = optimize_const(
            java_class,
            method_name,
            bytes,
            pc + instruction_length,
            pc_start + code_len,
            address + instruction_length,
            const_val);

          if (ret == 0)
          {
            ret = generator->push_int(const_val);
          }
            else
          {
            skip_bytes = ret;
            ret = 0;
          }
        }
          else
        if (gen32->tag == CONSTANT_FLOAT)
        {
          constant_float = (constant_float_t *)gen32;
          ret = generator->push_float(constant_float->value);
        }
          else
        if (gen32->tag == CONSTANT_STRING)
        {
          constant_string_t *constant_string = (constant_string_t *)gen32;
          const_val = constant_string->string_index;

          // FIXME: I think this is wrong.. why use the index to the string?
          // 2022-Jan-29: This appears to be fine and is needed for the
          // CPU.asm(String code) feature. Not sure why I commented this out
          // earlier.
//#if 0
          ret = optimize_const(
            java_class,
            method_name,
            bytes,
            pc + instruction_length,
            pc_start + code_len,
            address + instruction_length,
            const_val);

          if (ret == 0)
//#endif
          {
            std::string data;

            java_class->get_name_constant(data, const_val);

            ret = optimize_const(
              java_class,
              method_name,
              bytes,
              pc + instruction_length,
              pc_start + code_len,
              address + instruction_length,
              data.c_str());
          }

          if (ret == 0)
          {
            std::string name = "string_" + std::to_string(const_val);

            ret = generator->push_ref_static(name, const_val);
            java_class->needed_constants[const_val] = 1;
          }
            else
          {
            skip_bytes = ret;
            ret = 0;
          }
        }
          else
        {
          printf("Cannot ldc this type %d=>'%s' pc=%d\n",
             gen32->tag, JavaClass::tag_as_string(gen32->tag), pc);
          ret = -1;
        }
        break;

      case 20: // ldc2_w (0x14)
        UNIMPL()
        break;

      case 21: // iload (0x15)
        if (wide == 1)
        {
          ret = generator->push_local_var_int(GET_PC_UINT16(1));
        }
          else
        {
          ret = generator->push_local_var_int(bytes[pc + 1]);
        }
        break;

      case 22: // lload (0x16)
        if (wide == 1)
        {
          ret = generator->push_local_var_long(GET_PC_UINT16(1));
        }
          else
        {
          ret = generator->push_local_var_long(bytes[pc + 1]);
        }
        break;

      case 23: // fload (0x17)
        if (wide == 1)
        {
          index = GET_PC_UINT16(1);
        }
          else
        {
          index = bytes[pc + 1];
        }

        ret = generator->push_local_var_float(index);
        break;

      case 24: // dload (0x18)
        UNIMPL()
        break;

      case 25: // aload (0x19)
        if (wide == 1)
        {
          index = GET_PC_UINT16(1);
        }
          else
        {
          index = bytes[pc + 1];
        }

        ret = generator->push_local_var_ref(index);
        break;

      case 26: // iload_0 (0x1a)
      case 27: // iload_1 (0x1b)
      case 28: // iload_2 (0x1c)
      case 29: // iload_3 (0x1d)
        // Push a local integer variable on the stack
        ret = generator->push_local_var_int(bytes[pc] - 26);
        break;

      case 30: // lload_0 (0x1e)
      case 31: // lload_1 (0x1f)
      case 32: // lload_2 (0x20)
      case 33: // lload_3 (0x21)
        // push a local long variable on the stack
        //ret = generator->push_long_local(bytes[pc]-30);
        UNIMPL()
        break;

      case 34: // fload_0 (0x22)
      case 35: // fload_1 (0x23)
      case 36: // fload_2 (0x24)
      case 37: // fload_3 (0x25)
        // Push a local integer variable on the stack
        ret = generator->push_local_var_float(bytes[pc] - 34);
        break;

      case 38: // dload_0 (0x26)
      case 39: // dload_1 (0x27)
      case 40: // dload_2 (0x28)
      case 41: // dload_3 (0x29)
        UNIMPL()
        break;

      case 42: // aload_0 (0x2a)
      case 43: // aload_1 (0x2b)
      case 44: // aload_2 (0x2c)
      case 45: // aload_3 (0x2d)
        ret = generator->push_local_var_ref(bytes[pc] - 42);
        break;

      case 46: // iaload (0x2e)
        if (stack.length() == 0)
        {
          ret = generator->array_read_int();
        }
          else
        {
          ret = array_load(java_class, stack.pop(), ARRAY_TYPE_INT);
        }

        break;

      case 47: // laload (0x2f)
        if (stack.length() == 0)
        {
          ret = generator->array_read_long();
        }
          else
        {
          ret = array_load(java_class, stack.pop(), ARRAY_TYPE_LONG);
        }
        break;
        break;

      case 48: // faload (0x30)
        if (stack.length() == 0)
        {
          ret = generator->array_read_float();
        }
          else
        {
          ret = array_load(java_class, stack.pop(), ARRAY_TYPE_FLOAT);
        }
        break;

      case 49: // daload (0x31)
        UNIMPL()
        break;

      case 50: // aaload (0x32)
        if (stack.length() == 0)
        {
          ret = generator->array_read_object();
        }
          else
        {
          ret = array_load(java_class, stack.pop(), ARRAY_TYPE_OBJECT);
        }
        break;

      case 51: // baload (0x33)
        if (stack.length() == 0)
        {
          ret = generator->array_read_byte();
        }
          else
        {
          ret = array_load(java_class, stack.pop(), ARRAY_TYPE_BYTE);
        }
        break;

      case 52: // caload (0x34)
        UNIMPL()
        break;

      case 53: // saload (0x35)
        if (stack.length() == 0)
        {
          ret = generator->array_read_short();
        }
          else
        {
          ret = array_load(java_class, stack.pop(), ARRAY_TYPE_SHORT);
        }
        break;

      case 54: // istore (0x36)
        if (wide == 1)
        {
          ret = generator->pop_local_var_int(GET_PC_UINT16(1));
        }
          else
        {
          ret = generator->pop_local_var_int(bytes[pc + 1]);
        }

        break;

      case 55: // lstore (0x37)
        // Pop long off stack and store in local variable
        if (wide == 1)
        {
          ret = generator->pop_local_var_long(GET_PC_UINT16(1));
        }
          else
        {
          ret = generator->pop_local_var_long(bytes[pc + 1]);
        }
        break;

      case 56: // fstore (0x38)
        // Pop float off stack and store in local variable
        if (wide == 1)
        {
          ret = generator->pop_local_var_float(GET_PC_UINT16(1));
        }
          else
        {
          ret = generator->pop_local_var_float(bytes[pc + 1]);
        }
        break;

      case 57: // dstore (0x39)
        UNIMPL()
        break;

      case 58: // astore (0x3a)
        if (wide == 1)
        {
          index = GET_PC_UINT16(1);
        }
          else
        {
          index = bytes[pc + 1];
        }

        if (stack.length() == 0)
        {
          ret = generator->pop_local_var_ref(index);
        }
          else
        {
          ret = push_ref(index, stack);
        }

        break;

      case 59: // istore_0 (0x3b)
      case 60: // istore_1 (0x3c)
      case 61: // istore_2 (0x3d)
      case 62: // istore_3 (0x3e)
        // Pop integer off stack and store in local variable
        ret = generator->pop_local_var_int(bytes[pc] - 59);

        if (optimize == 1 && !needs_label(label_map, pc + 1, pc_start) &&
            bytes[pc + 1] == 26 + (bytes[pc] - 59))
        {
          if (generator->push_fake() == 0)
          {
            pc += table_java_instr[bytes[pc + 1]].normal;
          }
        }

        break;

      case 63: // lstore_0 (0x3f)
      case 64: // lstore_1 (0x40)
      case 65: // lstore_2 (0x41)
      case 66: // lstore_3 (0x42)
        // Pop long off stack and store in local variable
        ret = generator->pop_local_var_long(bytes[pc] - 63);
        break;

      case 67: // fstore_0 (0x43)
      case 68: // fstore_1 (0x44)
      case 69: // fstore_2 (0x45)
      case 70: // fstore_3 (0x46)
        // Pop float off stack and store in local variable
        ret = generator->pop_local_var_float(bytes[pc] - 67);
        break;

      case 71: // dstore_0 (0x47)
        UNIMPL()
        break;

      case 72: // dstore_1 (0x48)
        UNIMPL()
        break;

      case 73: // dstore_2 (0x49)
        UNIMPL()
        break;

      case 74: // dstore_3 (0x4a)
        UNIMPL()
        break;

      case 75: // astore_0 (0x4b)
      case 76: // astore_1 (0x4c)
      case 77: // astore_2 (0x4d)
      case 78: // astore_3 (0x4e)
        // Pop ref off stack and store in local variable
        index = bytes[pc] - 75;
        if (stack.length() == 0)
        {
          ret = generator->pop_local_var_ref(index);
        }
          else
        {
          ret = push_ref(index, stack);
        }

        break;

      case 79: // iastore (0x4f)
        if (stack.length() == 0)
        {
          ret = generator->array_write_int();
        }
          else
        {
          ret = array_store(java_class, stack.pop(), ARRAY_TYPE_INT);
        }

        break;

      case 80: // lastore (0x50)
        UNIMPL()
        break;

      case 81: // fastore (0x51)
        if (stack.length() == 0)
        {
          ret = generator->array_write_float();
        }
          else
        {
          ret = array_store(java_class, stack.pop(), ARRAY_TYPE_FLOAT);
        }

        break;

      case 82: // dastore (0x52)
        UNIMPL()
        break;

      case 83: // aastore (0x53)
        if (stack.length() == 0)
        {
          ret = generator->array_write_object();
        }
          else
        {
          ret = array_store(java_class, stack.pop(), ARRAY_TYPE_OBJECT);
        }

        break;

      case 84: // bastore (0x54)
        if (stack.length() == 0)
        {
          ret = generator->array_write_byte();
        }
          else
        {
          ret = array_store(java_class, stack.pop(), ARRAY_TYPE_BYTE);
        }

        break;

      case 85: // castore (0x55)
        if (stack.length() == 0)
        {
          ret = generator->array_write_short();
        }
          else
        {
          ret = array_store(java_class, stack.pop(), ARRAY_TYPE_CHAR);
        }

        break;

      case 86: // sastore (0x56)
        if (stack.length() == 0)
        {
          ret = generator->array_write_short();
        }
          else
        {
          ret = array_store(java_class, stack.pop(), ARRAY_TYPE_SHORT);
        }

        break;

      case 87: // pop (0x57)
        // Pop off stack and discard
        ret = generator->pop();
        break;

      case 88: // pop2 (0x58)
        // Pop 2 things off stack and discard
        ret = generator->pop();
        ret = generator->pop();
        break;

      case 89: // dup (0x59)
        // Take top value on stack, and push it again
        ret = generator->dup();
        break;

      case 90: // dup_x1 (0x5a)
        UNIMPL()
        break;

      case 91: // dup_x2 (0x5b)
        UNIMPL()
        break;

      case 92: // dup2 (0x5c)
        // Take the top 2 values on the stack and push them again
        // value1,value2 becomes: value1,value2,value1,value2
        ret = generator->dup2();
        break;

      case 93: // dup2_x1 (0x5d)
        UNIMPL()
        break;

      case 94: // dup2_x2 (0x5e)
        UNIMPL()
        break;

      case 95: // swap (0x5f)
        // Take the top two values on the stack and switch them
        ret = generator->swap();
        break;

      case 96: // iadd (0x60)
        // Pop top two integers from stack, add them, push result
        ret = generator->add_integer();
        break;

      case 97: // ladd (0x61)
        // Pop top two longs from stack, add them, push result
        ret = generator->add_long();
        break;

      case 98: // fadd (0x62)
        // Pop top two floats from stack, add them, push result
        ret = generator->add_float();
        break;

      case 99: // dadd (0x63)
        // Pop top two doubles from stack, add them, push result
        // ret = generator->add_doubles();
        UNIMPL()
        break;

      case 100: // isub (0x64)
        // Pop top two integers from stack, subtract them, push result
        // *(stack-1) - *(stack-0)
        ret = generator->sub_integer();
        break;

      case 101: // lsub (0x65)
        // Pop top two longs from stack, subtract them, push result
        ret = generator->sub_long();
        break;

      case 102: // fsub (0x66)
        // Pop top two floats from stack, subtract them, push result
        ret = generator->sub_float();
        break;

      case 103: // dsub (0x67)
        // Pop top two doubles from stack, subtract them, push result
        // *(stack-1) - *(stack-0)
        // ret = generator->sub_doubles();
        UNIMPL()
        break;

      case 104: // imul (0x68)
        // Pop top two integers from stack, multiply them, push result
        ret = generator->mul_integer();
        break;

      case 105: // lmul (0x69)
        // Pop top two longs from stack, multiply them, push result
        ret = generator->mul_long();
        break;

      case 106: // fmul (0x6a)
        // Pop top two floats from stack, multiply them, push result
        ret = generator->mul_float();
        break;

      case 107: // dmul (0x6b)
        // Pop top two doubles from stack, multiply them, push result
        // ret = generator->mul_doubles();
        UNIMPL()
        break;

      case 108: // idiv (0x6c)
        // Pop top two integers from stack, divide them, push result
        ret = generator->div_integer();
        break;

      case 109: // ldiv (0x6d)
        ret = generator->div_long();
        break;

      case 110: // fdiv (0x6e)
        ret = generator->div_float();
        break;

      case 111: // ddiv (0x6f)
        UNIMPL()
        break;

      case 112: // irem (0x70)
        // Pop top two integers from stack, divide them, push result
        ret = generator->mod_integer();
        break;

      case 113: // lrem (0x71)
        ret = generator->mod_long();
        break;

      case 114: // frem (0x72)
        UNIMPL()
        break;

      case 115: // drem (0x73)
        UNIMPL()
        break;

      case 116: // ineg (0x74)
        // negate the top integer on the stack
        ret = generator->neg_integer();
        break;

      case 117: // lneg (0x75)
        // negate the top long on the stack
        ret = generator->neg_long();
        break;

      case 118: // fneg (0x76)
        ret = generator->neg_float();
        break;

      case 119: // dneg (0x77)
        UNIMPL()
        break;

      case 120: // ishl (0x78)
        // Pop two integer values from stack shift left and push result
        ret = generator->shift_left_integer();
        break;

      case 121: // lshl (0x79)
        // Pop two long values from stack shift left and push result
        ret = generator->shift_left_long();
        break;

      case 122: // ishr (0x7a)
        // Pop two integer values from stack shift right and push result
        ret = generator->shift_right_integer();
        break;

      case 123: // lshr (0x7b)
        // Pop two long values from stack shift right and push result
        ret = generator->shift_right_long();
        break;

      case 124: // iushr (0x7c)
        // Pop two unsigned integer values from stack shift left and push result
        ret = generator->shift_right_uinteger();
        break;

      case 125: // lushr (0x7d)
        // Pop two unsigned long values from stack shift left and push result
        ret = generator->shift_right_ulong();
        break;

      case 126: // iand (0x7e)
        // Pop top two integers from stack, and them, push result
        ret = generator->and_integer();
        break;

      case 127: // land (0x7f)
        // Pop top two longs from stack, and them, push result
        ret = generator->and_long();
        break;

      case 128: // ior (0x80)
        // Pop top two integers from stack, or them, push result
        ret = generator->or_integer();
        break;

      case 129: // lor (0x81)
        // Pop top two longs from stack, or them, push result
        ret = generator->or_long();
        break;

      case 130: // ixor (0x82)
        // Pop top two ints from stack, xor them, push result
        ret = generator->xor_integer();
        break;

      case 131: // lxor (0x83)
        // Pop top two longs from stack, xor them, push result
        ret = generator->xor_long();
        break;

      case 132: // iinc (0x84)
        if (wide == 1)
        {
          index = GET_PC_UINT16(1);
          ret = generator->inc_integer(index, (int16_t)(GET_PC_INT16(3)));
          instruction_length = table_java_instr[bytes[pc]].wide;
        }
          else
        {
          index = bytes[pc + 1];
          ret = generator->inc_integer(index, (int8_t)bytes[pc + 2]);
          instruction_length = table_java_instr[bytes[pc]].normal;
        }

        skip_bytes = optimize_compare(java_class, method_name, bytes,
                                      pc + instruction_length,
                                      pc_start + code_len,
                                      address + instruction_length, index);
        break;

      case 133: // i2l (0x85)
        // Pop top integer from stack and push as a long
        ret = generator->integer_to_long();
        break;

      case 134: // i2f (0x86)
        // Pop top integer from stack and push as a float
        ret = generator->integer_to_float();
        break;

      case 135: // i2d (0x87)
        // Pop top integer from stack and push as a double
        UNIMPL()
        break;

      case 136: // l2i (0x88)
        // Pop top long from stack and push as a integer
        ret = generator->long_to_integer();
        break;

      case 137: // l2f (0x89)
        // Pop top long from stack and push as a float
        UNIMPL()
        break;

      case 138: // l2d (0x8a)
        // Pop top long from stack and push as a double
        UNIMPL()
        break;

      case 139: // f2i (0x8b)
        // Pop top float from stack and push as a integer
        ret = generator->float_to_integer();
        break;

      case 140: // f2l (0x8c)
        // Pop top float from stack and push as a long
        UNIMPL()
        break;

      case 141: // f2d (0x8d)
        // Pop top float from stack and push as a double
        UNIMPL()
        break;

      case 142: // d2i (0x8e)
        // Pop top double from stack and push as a integer
        UNIMPL()
        break;

      case 143: // d2l (0x8f)
        // Pop top double from stack and push as a long
        UNIMPL()
        break;

      case 144: // d2f (0x90)
        // Pop top double from stack and push as a float
        UNIMPL()
        break;

      case 145: // i2b (0x91)
        // Pop top integer from stack and push as a byte
        ret = generator->integer_to_byte();
        break;

      case 146: // i2c (0x92)
        // Pop top integer from stack and push as a char
        ret = generator->integer_to_short();
        break;

      case 147: // i2s (0x93)
        // Pop top integer from stack and push as a short
        ret = generator->integer_to_short();
        break;

      case 148: // lcmp (0x94)
        ret = generator->compare_longs();
        break;

      case 149: // fcmpl (0x95)
      case 150: // fcmpg (0x96)
        ret = generator->compare_floats(bytes[pc] - 149);
        break;

      case 151: // dcmpl (0x97)
        UNIMPL()
        break;

      case 152: // dcmpg (0x98)
        UNIMPL()
        break;

      case 153: // ifeq (0x99)
      case 154: // ifne (0x9a)
      case 155: // iflt (0x9b)
      case 156: // ifge (0x9c)
      case 157: // ifgt (0x9d)
      case 158: // ifle (0x9e)
      {
        ret = try_ternary(bytes, code_len + pc_start, pc, true, 0);

        if (ret != -1)
        {
          skip_bytes = ret;
          ret = 0;
          break;
        }

        int byte_count = GET_PC_INT16(1);
        int jump_to = address + byte_count;

        label = method_name + "_" + std::to_string(jump_to);

        ret = generator->jump_cond(label, cond_table[bytes[pc] - 153], calc_distance(bytes, pc, pc + byte_count));
        break;
      }
      case 159: // if_icmpeq (0x9f)
      case 160: // if_icmpne (0xa0)
      case 161: // if_icmplt (0xa1)
      case 162: // if_icmpge (0xa2)
      case 163: // if_icmpgt (0xa3)
      case 164: // if_icmple (0xa4)
      {
        ret = try_ternary(bytes, code_len + pc_start, pc, false, 0);

        if (ret != -1)
        {
          skip_bytes = ret;
          ret = 0;
          break;
        }

        int byte_count = GET_PC_INT16(1);
        int jump_to = address + byte_count;

        label = method_name + "_" + std::to_string(jump_to);

        ret = generator->jump_cond_integer(label, cond_table[bytes[pc]-159], calc_distance(bytes, pc, pc + byte_count));

        break;
      }
      case 165: // if_acmpeq (0xa5)
        UNIMPL()
        break;

      case 166: // if_acmpne (0xa6)
        UNIMPL()
        break;

      case 167: // goto (0xa7)
      {
        int byte_count = GET_PC_INT16(1);
        int jump_to = address + byte_count;

        label = method_name + "_" + std::to_string(jump_to);

        ret = generator->jump(label, calc_distance(bytes, pc, pc + byte_count));
        break;
      }
      case 168: // jsr (0xa8)
        label = method_name + "_" + std::to_string(address + GET_PC_INT16(1));

        ret = generator->call(label);
        break;

      case 169: // ret (0xa9)
        // FIXME - "Continue execution from address taken from a local
        // variable #index (the asymmetry with jsr is intentional).
        // The hell does that mean?  jsr pushes the return address on the
        // stack.. this thing shouldn't have to have an index.
#if 0
        if (wide == 1)
        {
          //pc = local_vars[GET_PC_UINT16(1)];
          ret = generator->return_local(GET_PC_UINT16(1), max_locals);
          pc += 3;
        }
          else
        {
          //pc = local_vars[bytes[pc + 1]];
          ret = generator->return_local(bytes[pc + 1], max_locals);
          pc += 2;
        }
#endif
        ret = -1;
        break;

      case 170: // tableswitch (0xaa)
        UNIMPL()
        break;

      case 171: // lookupswitch (0xab)
        UNIMPL()
        break;

      case 172: // ireturn (0xac)
        ret = generator->return_integer(max_locals);
        break;

      case 173: // lreturn (0xad)
        ret = generator->return_long(max_locals);
        break;

      case 174: // freturn (0xae)
        UNIMPL()
        break;

      case 175: // dreturn (0xaf)
        UNIMPL()
        break;

      case 176: // areturn (0xb0)
        UNIMPL()
        break;

      case 177: // return (0xb1)
        ret = generator->return_void(max_locals);
        break;

      case 178: // getstatic (0xb2)
      {
        std::string field_name;
        std::string type;

        ref = GET_PC_UINT16(1);
        gen32 = (generic_32bit_t *)java_class->get_constant(ref);

        if (java_class->get_ref_name_type(field_name, type, ref) != 0)
        {
          printf("Error retrieving field name %d\n", ref);
          ret = -1;
          break;
        }

        int index = java_class->get_field_index(field_name);

        if (gen32->tag == CONSTANT_METHODREF)
        {
          stack.push(ref);
        }
          else
        if (type[0] == '[')
        {
          ret = generator->push_ref(field_name);
        }
          else
        if (type[0] == 'L')
        {
          if (generator->get_static(field_name, index) != 0)
          {
            printf("Internal Error: get_static not implemented\n");
            ret = -1;
            break;
          }

          java_class->get_ref_name_type(field_name, type, ref);

          // This fixes Joe's demo (meant to deal with strings) kind of
          // gross with a strcmp :(  Maybe revisit later.
#if 0
          if (type == "Ljava/lang/String;")
          {
            DEBUG_PRINT("  static is %s (will invoke)\n", field_name.c_str());
            //generator->push_ref(field_name);
            stack.push(ref);
          }
#endif
        }
          else
        {
          ret = generator->push_ref(field_name);
        }

        break;
      }
      case 179: // putstatic (0xb3)
      {
        std::string field_name;
        std::string type;

        ref = GET_PC_UINT16(1);
        gen32 = (generic_32bit_t *)java_class->get_constant(ref);

        if (java_class->get_ref_name_type(field_name, type, ref) != 0)
        {
          printf("Error retrieving field name %d\n", ref);
          ret = -1;
          break;
        }

        if (stack.length() != 0)
        {
          std::string field_name;
          std::string type;

          if (java_class->get_ref_name_type(field_name, type, stack.pop()) != 0)
          {
            printf("Error retrieving field name %d\n", ref);
            ret = -1;
            break;
          }

          ret = generator->push_ref(field_name);
        }

        index = java_class->get_field_index(field_name);
        ret = generator->put_static(field_name, index);

        break;
      }
      case 180: // getfield (0xb4)
        UNIMPL()
        break;

      case 181: // putfield (0xb5)
        UNIMPL()
        break;

      case 182: // invokevirtual (0xb6)
        ref = GET_PC_UINT16(1);

        if (stack.length() == 0)
        {
          ret = invoke_virtual(java_class, ref, generator);
        }
          else
        {
          ret = invoke_virtual(java_class, ref, stack.pop(), generator);
        }

        break;

      case 183: // invokespecial (0xb7)
        ref = GET_PC_UINT16(1);
        ret = invoke_virtual(java_class, ref, generator);
        break;

      case 184: // invokestatic (0xb8)
        ref = GET_PC_UINT16(1);
        ret = invoke_static(java_class, ref, generator);
        break;

      case 185: // invokeinterface (0xb9)
        UNIMPL()
        break;

      case 186: // invokedynamic (0xba)
        UNIMPL()
        break;

      case 187: // new (0xbb)
        index = GET_PC_UINT16(1);
        java_class->get_class_name(class_name, index);
        // FIXME - Field count is 0 here.  This needs to be filled in and
        // class needs to be loaded and compiled in order to support user
        // defined classes.  Only API classes can be new'd right now.
        ret = generator->new_object(class_name, 0);
        break;

      case 188: // newarray (0xbc)
        ret = generator->new_array(bytes[pc + 1]);
        break;

      case 189: // anewarray (0xbd)
        index = GET_PC_UINT16(1);
        java_class->get_class_name(class_name, index);
        ret = generator->new_object_array(class_name);
        break;

      case 190: // arraylength (0xbe)
        DEBUG_PRINT("stack.length()=%d\n", stack.length());

        // FIXME - This is may not be correct
        if (stack.length() > 0)
        {
          uint32_t value = stack.pop();
          gen32 = (generic_32bit_t *)java_class->get_constant(value);
          if (gen32->tag == CONSTANT_FIELDREF)
          {
            std::string field_name;
            std::string type;
            //char type[64];
            constant_fieldref_t *field_ref = (struct constant_fieldref_t *)gen32;

            DEBUG_PRINT("class_index=%d name_and_type=%d\n",
                        field_ref->class_index,
                        field_ref->name_and_type_index);

            if (java_class->get_ref_name_type(field_name, type, value) != 0)
            {
              printf("Error retrieving field name const_index=%d\n", value);
              ret = -1;
              break;
            }

            ret = generator->push_array_length(field_name, 0);
          }
            else
          {
            printf("Error: tag not supported\n");
            ret = -1;
            break;
          }
        }
          else
        {
          ret = generator->push_array_length();
        }

        break;

      case 191: // athrow (0xbf)
        UNIMPL()
        break;

      case 192: // checkcast (0xc0)
        UNIMPL()
        break;

      case 193: // instanceof (0xc1)
        UNIMPL()
        break;

      case 194: // monitorenter (0xc2)
        UNIMPL()
        break;

      case 195: // monitorexit (0xc3)
        UNIMPL()
        break;

      case 196: // wide (0xc4)
        wide=1;
        pc++;
        continue;

      case 197: // multianewarray (0xc5)
        UNIMPL()
        break;

      case 198: // ifnull (0xc6)
      {
        ret = try_ternary(bytes, code_len + pc_start, pc, true, 0);

        if (ret != -1)
        {
          skip_bytes = ret;
          ret = 0;
          break;
        }

        int byte_count = GET_PC_INT16(1);
        int jump_to = address + byte_count;

        label = method_name + "_" + std::to_string(jump_to);

        ret = generator->jump_cond_zero(label, COND_EQUAL, calc_distance(bytes, pc, pc + byte_count));
        break;
      }
      case 199: // ifnonnull (0xc7)
      {
        ret = try_ternary(bytes, code_len + pc_start, pc, true, 0);

        if (ret != -1)
        {
          skip_bytes = ret;
          ret = 0;
          break;
        }

        int byte_count = GET_PC_INT16(1);
        int jump_to = address + byte_count;

        label = method_name + "_" + std::to_string(jump_to);

        ret = generator->jump_cond_zero(label, COND_NOT_EQUAL, calc_distance(bytes, pc, pc + byte_count));
        break;
      }
      case 200: // goto_w (0xc8)
      {
        int byte_count = GET_PC_INT32(1);
        int jump_to = address + byte_count;

        label = method_name + "_" + std::to_string(jump_to);

        ret = generator->jump(label, calc_distance(bytes, pc, pc + byte_count));
        break;
      }
      case 201: // jsr_w (0xc9)
        //PUSH_INTEGER(pc+5);
        //pc += GET_PC_INT32(1);

        label = method_name + "_" + std::to_string(address + GET_PC_INT32(1));

        ret = generator->call(label);
        break;

      case 202: // breakpoint (0xca)
        ret = generator->brk();
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
        UNIMPL()
        break;

      case 254: // impdep1 (0xfe)
        UNIMPL()
        break;

      case 255: // impdep2 (0xff)
        UNIMPL()
        break;
    }

    if (ret != 0)
    {
      printf("** Error with %s (0x%02x) instruction\n", table_java_instr[bytes[pc]].name, bytes[pc]);
      break;
    }

    if (wide == 1)
    {
      if (table_java_instr[bytes[pc]].wide == 0)
      {
        printf("Error: This instruction can't be wide?\n");
        return -1;
      }
      pc += table_java_instr[bytes[pc]].wide;
    }
    else
    {
      pc += table_java_instr[bytes[pc]].normal;
    }

    pc += skip_bytes;

#ifdef DEBUG
    //stack_dump(stack_values_start, stack_types, stack_ptr);
#endif

    //printf("pc=%d opcode=%d (0x%02x)\n", pc - pc_start, bytes[pc], bytes[pc]);
    //if (pc - pc_start >= code_len) { break; }

    wide = 0;
  }

  generator->method_end(max_locals);

  return ret;
}

int JavaCompiler::load_class(const char *filename)
{
  int ptr;
  int last_slash = -1;

  DEBUG_PRINT("load_class(%s)\n", filename);

  ptr = 0;
  while (filename[ptr] != 0)
  {
    if (filename[ptr] == '/') { last_slash = ptr; }
    if (filename[ptr] == '\\') { last_slash = ptr; }  // DOS :( come on now :(
    ptr++;
  }

  if (last_slash != -1 && last_slash < (int)sizeof(classpath) - 1)
  {
    last_slash++;
    strncpy(classpath, filename, last_slash);
    classpath[last_slash] = 0;
  }

  DEBUG_PRINT("CLASSPATH: '%s'\n\n", classpath);

  in = fopen(filename, "rb");
  if (in == NULL) { return -1; }

  java_class = new JavaClass(in);

#ifdef DEBUG
  if (verbose)
  {
    java_class->print();
  }
#endif

  int external_field_count = find_external_fields(java_class, true);

  if (verbose)
  {
    printf("Total external field count: %d\n", external_field_count);

    // Just some debug code
    printf("external_class_count=%zu\n", external_classes.size());
    printf("external_field_count=%zu\n", external_fields.size());

    std::map<std::string,int>::iterator iter;

    for (iter = external_fields.begin(); iter != external_fields.end(); iter++)
    {
      printf("EXTERNAL %s %s\n",
             iter->first.c_str(), field_type_from_int(iter->second));
    }
  }

  return 0;
}

void JavaCompiler::insert_static_field_defines()
{
  const fields_t *field;
  int field_count = java_class->get_field_count();
  int index;
  int external_index;

  // Add all fields from this class
  for (index = 0; index < field_count; index++)
  {
    std::string field_name;
    std::string field_type;

    field = java_class->get_field(index);
    if ((field->access_flags & ACC_STATIC) == 0) { continue; }

    java_class->get_field_name(field_name, index);
    java_class->get_field_type(field_type, index);

    generator->insert_static_field_define(field_name, field_type, index);
  }

  external_index = field_count;

  // Add all fields from any external classes
  std::map<std::string,int>::iterator iter;

  for (iter = external_fields.begin(); iter != external_fields.end(); iter++)
  {
    std::string type = field_type_from_int(iter->second);

    // :(
    std::string field_name = iter->first;

    generator->insert_static_field_define(field_name, type, external_index++);
  }
}

void JavaCompiler::init_heap()
{
  int field_count = java_class->get_field_count();

  field_count += external_fields.size();

  generator->start_init();
  generator->init_heap(field_count);
}

int JavaCompiler::add_static_initializers()
{
  int index;

  // Add all the static initializers
  index = java_class->get_clinit_method();

  if (index != -1)
  {
    if (execute_static(java_class, index, generator, false, verbose) != 0)
    {
      printf("** Error setting statics %s:%d.\n", __FILE__, __LINE__);
      return -1;
    }
  }

  // Add static initializers for all external fields.
  std::map<std::string,JavaClass *>::iterator iter;

  for (iter = external_classes.begin(); iter != external_classes.end(); iter++)
  {
    std::string name;
    JavaClass *java_class_external = iter->second;
    index = java_class_external->get_clinit_method();

    if (index < 0) { continue; }

    java_class_external->get_method_name(name, index);

    DEBUG_PRINT("CLASS %s.%s  index=%d\n", iter->first.c_str(), name.c_str(), index);

    if (execute_static(java_class_external, index, generator, false, verbose, java_class) != 0)
    {
      printf("** Error setting statics %s:%d.\n", __FILE__, __LINE__);
      return -1;
    }
  }

  generator->add_newline();

  return 0;
}

int JavaCompiler::execute_statics(int index)
{
  // <clinit> method is executed for adding array static data
  if (index != -1 && execute_static(java_class, index, generator, true, verbose) != 0)
  {
    printf("** Error setting statics %s:%d.\n", __FILE__, __LINE__);
    return -1;
  }

  // Add arrays from external classes
  std::map<std::string,JavaClass *>::iterator iter;
  for (iter = external_classes.begin(); iter != external_classes.end(); iter++)
  {
    DEBUG_PRINT("Adding external class: %s\n", iter->first.c_str());

    JavaClass *java_class_external = iter->second;
    int index = java_class_external->get_clinit_method();

    if (index < 0) { continue; }

    if (execute_static(java_class_external, index, generator, true, verbose,
                       java_class) != 0)
    {
      printf("** Error setting statics %s:%d.\n", __FILE__, __LINE__);
      return -1;
    }
  }

  return 0;
}

int JavaCompiler::compile_methods(bool do_main)
{
  int method_count = java_class->get_method_count();
  std::string method_name;
  int index;
  bool did_execute_statics = false;

  for (index = 0; index < method_count; index++)
  {
    if (java_class->get_method_name(method_name, index) == 0)
    {
      if (method_name == "main")
      {
        if (!do_main) { continue ;}

        if (compile_method(java_class, index) != 0)
        {
          printf("** Error compiling class.\n");
          return -1;
        }

        break;
      }

      if (do_main) { continue; }

      if (method_name == "<clinit>")
      {
        if (execute_statics(index) != 0) { return -1; }
        did_execute_statics = true;
        continue;
      }

      if (compile_method(java_class, index) != 0)
      {
        printf("** Error compiling class.\n");
        return -1;
      }
    }
  }

  DEBUG_PRINT("external_classes.size()=%zu did_execute_statics=%d do_main=%d\n",
              external_classes.size(), did_execute_statics, do_main);

  //if (!did_execute_statics && !do_main && external_classes.size() != 0)
  if (!do_main && external_classes.size() != 0)
  {
    // Compile all needed methods from other classes.
    std::map<std::string,JavaClass *>::iterator iter;
    for (iter = external_classes.begin();
         iter != external_classes.end();
         iter++)
    {
      const char *class_name = iter->first.c_str();
      JavaClass *java_class = iter->second;

      //int constant_count = java_class->get_constant_count();
      int method_count = java_class->get_method_count();

      DEBUG_PRINT("Compile Class: %s (%p)\n", class_name, this);
      DEBUG_PRINT("  method_count=%d\n", method_count);

      // For all methods in class
      for (index = 0; index < method_count; index++)
      {
        struct methods_t *method = java_class->get_method(index);

        if (method == NULL)
        {
          printf("Error: get_method(%d) failed.\n", index);
          return -1;
        }

        std::string method_name;
        std::string alt_name;

        if (java_class->get_method_name(method_name, index) != 0)
        {
          printf("Error: get_method_name(%d) failed.\n", index);
          return -1;
        }

        printf("  METHOD INDEX=%d (%s)\n", index, method_name.c_str());

        if (method_name == "<clinit>") { continue; }
        if (method_name == "<init>") { continue; }

        alt_name = std::string(class_name) + "_" + method_name;

        DEBUG_PRINT("  compiling: %s.%s\n", class_name, method_name.c_str());

        if (compile_method(java_class, index, alt_name.c_str()) != 0)
        {
          return -1;
        }
      }
    }

    // Had to add this "if" to deal with static arrays in the main class...
    // it stopped compiling external methods otherwise.
    if (!did_execute_statics)
    {
      if (execute_statics(-1) != 0) { return -1; }
    }
  }

  return 0;
}

int JavaCompiler::add_constants(JavaClass *java_class)
{
  std::map<int,int>::iterator iter;

  for (iter = java_class->needed_constants.begin();
       iter != java_class->needed_constants.end();
       iter++)
  {
    constant_utf8_t *constant_utf8 =
      (constant_utf8_t *)java_class->get_constant(iter->first);

    std::string name = "string_" + std::to_string(iter->first);

    //sprintf(name, "string_%d", iter->first);
    generator->insert_string(name, constant_utf8->bytes, constant_utf8->length);
  }

  return 0;
}

int JavaCompiler::add_constants()
{
  // Add contants from main class.
  add_constants(java_class);

  // Add constants from external classes.
  std::map<std::string,JavaClass *>::iterator iter;

  for (iter = external_classes.begin(); iter != external_classes.end(); iter++)
  {
    JavaClass *java_class = iter->second;
    add_constants(java_class);
  }

  return 0;
}

int JavaCompiler::field_type_to_int(const char *field_type)
{
  int len = sizeof(type_table) / sizeof (char *);
  int n;

  for (n = 0; n < len; n++)
  {
    if (strcmp(field_type, type_table[n]) == 0) { return n | 0x80; }
    if (strcmp(field_type, type_table[n] + 1) == 0) { return n; }
  }

  return 0;
}

const char *JavaCompiler::field_type_from_int(int type)
{
  int is_array = (type & 0x80) >> 7;
  const char *field_type;

  type = type & 0x7f;

  if (type >= (int)(sizeof(type_table) / sizeof (char *))) { type = 0; }

  field_type = type_table[type];
  if (is_array == 0) { field_type++; }

  return field_type;
}

int JavaCompiler::get_const(uint8_t *bytes, int len, int address, int *value)
{
  if (bytes[address] == 0xc4)
  {
#if 0
    if (table_java_instr[bytes[address + 1]].op_type != OP_TYPE_CONST)
    {
      return -1;
    }

    switch (bytes[address + 1])
    {
    }

    return table_java_instr[bytes[address + 1]].wide + 1;
#endif
    return -1;
  }
    else
  {
    if (table_java_instr[bytes[address]].op_type != OP_TYPE_CONST)
    {
      return -1;
    }

    switch (bytes[address])
    {
      case 1: // aconst_null (0x01)
        *value = 0;
        break;
      case 2: // iconst_m1 (0x02)
      case 3: // iconst_0 (0x03)
      case 4: // iconst_1 (0x04)
      case 5: // iconst_2 (0x05)
      case 6: // iconst_3 (0x06)
      case 7: // iconst_4 (0x07)
      case 8: // iconst_5 (0x08)
        *value = ((uint8_t)bytes[address]) - 3;;
        break;
      default:
        return -1;
    }

    return table_java_instr[bytes[address]].normal;
  }
}

int JavaCompiler::get_cond(uint8_t *bytes, int len, int address, int *cond, int *label)
{
  if (table_java_instr[bytes[address]].op_type != OP_TYPE_IF) { return -1; }

  switch (bytes[address])
  {
    case 153: // ifeq (0x99)
    case 159: // if_icmpeq (0x9f)
    case 198: // ifnull (0xc6)
      *cond = COND_EQUAL;
      break;
    case 154: // ifne (0x9a)
    case 199: // ifnonnull (0xc7)
    case 160: // if_icmpne (0xa0)
      *cond = COND_NOT_EQUAL;
      break;
    case 155: // iflt (0x9b)
    case 161: // if_icmplt (0xa1)
      *cond = COND_LESS;
      break;
    case 156: // ifge (0x9c)
    case 162: // if_icmpge (0xa2)
      *cond = COND_GREATER_EQUAL;
      break;
    case 157: // ifgt (0x9d)
    case 163: // if_icmpgt (0xa3)
      *cond = COND_GREATER;
      break;
    case 158: // ifle (0x9e)
    case 164: // if_icmple (0xa4)
      *cond = COND_LESS_EQUAL;
      break;
    default:
      return -1;
  }

  *label = address;
  *label += ((uint16_t)bytes[address + 1]) << 8 | bytes[address + 2];

  return 3;
}

int JavaCompiler::try_ternary(uint8_t *bytes, int len, int pc, bool compare_with_value, int compare)
{
  int offset = pc;
  int value_true, value_false;
  int cond, label;
  int skip, if_len;

  skip = get_cond(bytes, len, offset, &cond, &label);

  if (skip == -1) { return -1; }
  offset += skip;
  if (offset >= len) { return -1; }

  if_len = skip;

  skip = get_const(bytes, len, offset, &value_false);

  if (skip == -1) { return -1; }
  offset += skip;
  if (offset >= len) { return -1; }

  // goto 0xa7
  if (bytes[offset] != 0xa7) { return -1; }

  offset += table_java_instr[bytes[offset]].normal;

  if (label != offset) { return -1; }

  skip = get_const(bytes, len, offset, &value_true);
  if (skip == -1) { return -1; }
  offset += skip;

  // printf(">> Ternary values (%d) ? %d : %d\n", cond, value_true, value_false);

  if (compare_with_value)
  {
    if (generator->ternary(cond, compare, value_true, value_false) == -1)
    {
      return -1;
    }
  }
    else
  {
    if (generator->ternary(cond, value_true, value_false) == -1)
    {
      return -1;
    }
  }

  // printf(">>> offset=%d %d   %d\n", offset, offset - pc, if_len);

  return offset - pc - if_len;
}

