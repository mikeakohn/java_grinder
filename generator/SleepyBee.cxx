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
#include <stdint.h>

#include "generator/SleepyBee.h"

SleepyBee::SleepyBee() :
  need_new_array_byte(false),
  need_new_array_int(false)
{
}

SleepyBee::~SleepyBee()
{
}

int SleepyBee::open(const char *filename)
{
  if (MCS51::open(filename) != 0) { return -1; }

  fprintf(out, ".include \"EFM8SB2.inc\"\n");

  return 0;
}

int SleepyBee::start_init()
{
  MCS51::start_init();

  fprintf(out,
    "  ;; Disable watchdog.\n"
    "  anl PCA0MD, #0xbf\n"
    "  mov PCA0MD, A\n\n");

#if 0
  fprintf(out,
    "  clr PSW.3\n"
    "  clr PSW.4\n");
#endif

  // FIXME: How to take care of XBR1.
  fprintf(out, "  mov XBR2, #0x40\n\n");

  // Set r1:r0 to point to flash.
  // Set r3:r2 to point to XRAM.
  fprintf(out,
    "  ;; memcpy() arrays from flash to XRAM\n"
    "  mov r0, #start_of_arrays & 0xff\n"
    "  mov r1, #start_of_arrays >> 8\n"
    "  mov r2, #0x%02x\n"
    "  mov r3, #0x%02x\n"
    "  mov r4, #(end_of_code - start_of_arrays) & 0xff\n"
    "  mov r5, #(end_of_code - start_of_arrays) >> 8\n",
    0, 0);

  int label_repeat = label_count++;
  int label_end = label_count++;
  int label_if = label_count++;

  // if (r1:r0 == r3:r2) goto end
  fprintf(out,
    "label_%d:\n"
    "  mov A, r2\n"
    "  xrl A, r4\n"
    "  jnz label_%d\n"
    "  mov A, r3\n"
    "  xrl A, r5\n"
    "  jnz label_%d\n"
    "  sjmp label_%d\n"
    "label_%d:\n",
    label_repeat,
    label_if,
    label_if,
    label_end,
    label_if);

  // Copy byte.
  fprintf(out,
    "  mov DPL, r0\n"
    "  mov DPH, r1\n"
    "  mov A, #0\n"
    "  movc A, @A+DPTR\n"
    "  inc DPTR\n"
    "  mov r0, DPL\n"
    "  mov r1, DPH\n");

  fprintf(out,
    "  mov DPL, r2\n"
    "  mov DPH, r3\n"
    "  movx @DPTR, A\n"
    "  inc DPTR\n"
    "  mov r2, DPL\n"
    "  mov r3, DPH\n");

#if 0
  // Increment r1:r0.
  fprintf(out,
    "  clr C\n"
    "  mov A, r0\n"
    "  add A, #1\n"
    "  mov r0, A\n"
    "  mov A, r1\n"
    "  addc A, #1\n"
    "  mov r1, A\n");

  // Increment r3:r1.
  fprintf(out,
    "  clr C\n"
    "  mov A, r2\n"
    "  add A, #1\n"
    "  mov r2, A\n"
    "  mov A, r3\n"
    "  addc A, #1\n"
    "  mov r3, A\n");
#endif

  fprintf(out,
    "  sjmp label_%d\n"
    "label_%d:\n",
    label_repeat,
    label_end);

  return 0;
}

int SleepyBee::finish()
{
  if (!has_array)
  {
    fprintf(out, "start_of_arrays:\n");
  }

  fprintf(out, "end_of_code:\n");

  if (need_new_array_byte) { add_new_array_byte(); }
  if (need_new_array_int)  { add_new_array_int(); }

  return 0;
}

int SleepyBee::new_array(uint8_t type)
{
  if (type == TYPE_SHORT || type == TYPE_CHAR || type == TYPE_INT)
  {
    fprintf(out,
      "  ;; new_array(INT)\n"
      "  mov r0, %d\n"
      "  mov r1, %d\n"
      "  lcall _new_array_int\n"
      "  mov %d, r2\n"
      "  mov %d, r3\n",
      REG_ADDRESS_STACK_LO(reg - 1),
      REG_ADDRESS_STACK_HI(reg - 1),
      REG_ADDRESS_STACK_LO(reg - 1),
      REG_ADDRESS_STACK_HI(reg - 1));

    need_new_array_int = true;

    return 0;
  }
    else
  if (type == TYPE_BYTE)
  {
    fprintf(out,
      "  ;; new_array(BYTE)\n"
      "  mov r0, %d\n"
      "  mov r1, %d\n"
      "  lcall _new_array_byte\n"
      "  mov %d, r2\n"
      "  mov %d, r3\n",
      REG_ADDRESS_STACK_LO(reg - 1),
      REG_ADDRESS_STACK_HI(reg - 1),
      REG_ADDRESS_STACK_LO(reg - 1),
      REG_ADDRESS_STACK_HI(reg - 1));

    need_new_array_byte = true;
  }
    else
  {
    return -1;
  }

  return 0;
}

int SleepyBee::push_array_length()
{
  fprintf(out,
    "  ;; push_array_length()\n"
    "  mov A, %d\n"
    "  add A, #0xfe\n"
    "  mov DPL, A\n"
    "  mov A, %d\n"
    "  addc A, #0xff\n"
    "  mov DPH, A\n"
    "  movx A, @DPTR\n"
    "  mov %d, A\n"
    "  inc DPTR\n"
    "  movx A, @DPTR\n"
    "  mov %d, A\n",
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1),
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1));

  return 0;
}

int SleepyBee::push_array_length(std::string &name, int field_id)
{
  fprintf(out,
    "  ;; push_array_length(%s, %d)\n"
    "  mov DPTR, #_%s - start_of_arrays\n"
    "  dec DPTR\n"
    "  dec DPTR\n"
    "  movx A, @DPTR\n"
    "  mov %d, A\n"
    "  inc DPTR\n"
    "  movx A, @DPTR\n"
    "  mov %d, A\n",
    name.c_str(), field_id,
    name.c_str(),
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1));

  return 0;
}

int SleepyBee::array_read_byte()
{
  fprintf(out,
    "  ;; array_read_byte()\n"
    "  mov A, %d\n"
    "  add A, %d\n"
    "  mov DPL, A\n"
    "  mov A, %d\n"
    "  addc A, %d\n"
    "  mov DPH, A\n",
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_LO(reg - 2),
    REG_ADDRESS_STACK_HI(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 2));

  fprintf(out,
    "  movx A, @DPTR\n"
    "  mov %d, A\n"
    "  anl A, #0x80\n"
    "  mov %d, #0x00\n"
    "  jz label_%d\n"
    "  mov %d, #0xff\n"
    "label_%d:\n",
    REG_ADDRESS_STACK_LO(reg - 2),
    REG_ADDRESS_STACK_HI(reg - 2),
    label_count,
    REG_ADDRESS_STACK_HI(reg - 2),
    label_count);

  label_count++;
  reg -= 1;

  return 0;
}

int SleepyBee::array_read_short()
{
  fprintf(out, "  ;; array_read_short()\n");

  // index = index * 2;
  fprintf(out,
    "  clr C\n"
    "  mov A, %d\n"
    "  rlc A\n"
    "  mov %d, A\n"
    "  mov A, %d\n"
    "  rlc A\n"
    "  mov %d, A\n",
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1));

  // address = address + index;
  fprintf(out,
    "  mov A, %d\n"
    "  add A, %d\n"
    "  mov DPL, A\n"
    "  mov A, %d\n"
    "  addc A, %d\n"
    "  mov DPH, A\n",
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_LO(reg - 2),
    REG_ADDRESS_STACK_HI(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 2));

  // data = *address;
  fprintf(out,
    "  movx A, @DPTR\n"
    "  mov %d, A\n"
    "  inc DPTR\n"
    "  movx A, @DPTR\n"
    "  mov %d, A\n",
    REG_ADDRESS_STACK_LO(reg - 2),
    REG_ADDRESS_STACK_HI(reg - 2));

  reg -= 1;

  return 0;
}

int SleepyBee::array_read_int()
{
  return array_read_short();
}

int SleepyBee::array_read_byte(std::string &name, int field_id)
{
  fprintf(out,
    "  ;; array_read_byte(%s, %d)\n"
    "  mov r0, #(%s - start_of_arrays) & 0xff\n"
    "  mov r1, #(%s - start_of_arrays) >> 8\n"
    "  mov A, %d\n"
    "  add A, r0\n"
    "  mov r0, A\n"
    "  mov A, %d\n"
    "  addc A, r1\n"
    "  mov r1, A\n"
    "  mov DPL, r0\n"
    "  mov DPH, r0\n",
    name.c_str(), field_id,
    name.c_str(),
    name.c_str(),
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1));

  fprintf(out,
    "  mov A, @DPTR\n"
    "  mov %d, A\n"
    "  anl A, #0x80\n"
    "  mov %d, #0x00\n"
    "  jz label_%d\n"
    "  mov %d, #0xff\n"
    "label_%d:\n",
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1),
    label_count,
    REG_ADDRESS_STACK_HI(reg - 1),
    label_count);

  label_count++;

  return 0;
}

int SleepyBee::array_read_short(std::string &name, int field_id)
{
  fprintf(out,
    "  ;; array_read_short(%s, %d)\n"
    "  mov r0, #(%s - start_of_arrays) & 0xff\n"
    "  mov r1, #(%s - start_of_arrays) >> 8\n"
    "  clr C\n"
    "  mov A, %d\n"
    "  rlc A\n"
    "  mov r2, A\n"
    "  mov A, %d\n"
    "  rlc A\n"
    "  mov r3, A\n"
    "  mov A, r2\n"
    "  add A, r0\n"
    "  mov r0, A\n"
    "  mov A, r3\n"
    "  addc A, r1\n"
    "  mov r1, A\n"
    "  mov DPL, r0\n"
    "  mov DPH, r0\n",
    name.c_str(), field_id,
    name.c_str(),
    name.c_str(),
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1));

  fprintf(out,
    "  mov A, @DPTR\n"
    "  mov %d, A\n"
    "  inc DPTR\n"
    "  mov A, @DPTR\n"
    "  mov %d, A\n",
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1));

  return 0;
}

int SleepyBee::array_read_int(std::string &name, int field_id)
{
  return array_read_int(name, field_id);
}

int SleepyBee::array_write_byte()
{
  fprintf(out,
    "  ;; array_write_byte()\n"
    "  mov A, %d\n"
    "  add A, %d\n"
    "  mov DPL, A\n"
    "  mov A, %d\n"
    "  addc A, %d\n"
    "  mov DPH, A\n",
    REG_ADDRESS_STACK_LO(reg - 2),
    REG_ADDRESS_STACK_LO(reg - 3),
    REG_ADDRESS_STACK_HI(reg - 2),
    REG_ADDRESS_STACK_HI(reg - 3));

  fprintf(out,
    "  mov A, %d\n"
    "  movx @DPTR, A\n",
    REG_ADDRESS_STACK_LO(reg - 1));

  reg -= 3;

  return 0;
}

int SleepyBee::array_write_short()
{
  fprintf(out, "  ;; array_write_short()\n");

  // index = index * 2;
  fprintf(out,
    "  clr C\n"
    "  mov A, %d\n"
    "  rlc A\n"
    "  mov %d, A\n"
    "  mov A, %d\n"
    "  rlc A\n"
    "  mov %d, A\n",
    REG_ADDRESS_STACK_LO(reg - 2),
    REG_ADDRESS_STACK_LO(reg - 2),
    REG_ADDRESS_STACK_HI(reg - 2),
    REG_ADDRESS_STACK_HI(reg - 2));

  // address = address + index;
  fprintf(out,
    "  mov A, %d\n"
    "  add A, %d\n"
    "  mov DPL, A\n"
    "  mov A, %d\n"
    "  addc A, %d\n"
    "  mov DPH, A\n",
    REG_ADDRESS_STACK_LO(reg - 2),
    REG_ADDRESS_STACK_LO(reg - 3),
    REG_ADDRESS_STACK_HI(reg - 2),
    REG_ADDRESS_STACK_HI(reg - 3));

  // data = *address;
  fprintf(out,
    "  mov A, %d\n"
    "  movx @DPTR, A\n"
    "  inc DPTR\n"
    "  mov A, %d\n"
    "  movx @DPTR, A\n",
    REG_ADDRESS_STACK_LO(reg - 1),
    REG_ADDRESS_STACK_HI(reg - 1));

  reg -= 3;

  return 0;
}

int SleepyBee::array_write_int()
{
  return array_write_short();
}

int SleepyBee::array_write_byte(std::string &name, int field_id)
{
  return -1;
}

int SleepyBee::array_write_short(std::string &name, int field_id)
{
  return -1;
}

int SleepyBee::array_write_int(std::string &name, int field_id)
{
  return array_write_short(name, field_id);
}

int SleepyBee::ioport_setPinsAsInput_I(int port)
{
  fprintf(out,
    "  ;; ioport_setPinsAsInput_I(%d)\n"
    "  mov A, %d\n"
    "  cpl A\n"
    "  anl P%dMDOUT, A\n",
    port,
    REG_ADDRESS_STACK_LO(--reg),
    port);

  return 0;
}

int SleepyBee::ioport_setPinsAsInput_I(int port, int const_val)
{
  fprintf(out,
    "  ;; ioport_setPinsAsInput_I(%d)\n"
    "  mov A, #%02x\n"
    "  cpl A\n"
    "  anl P%dMDOUT, A\n",
    port,
    const_val,
    port);

  return 0;
}

int SleepyBee::ioport_setPinsAsOutput_I(int port)
{
  fprintf(out,
    "  ;; ioport_setPinsAsOutput_I(%d)\n"
    "  mov A, %d\n"
    "  orl P%dMDOUT, A\n",
    port,
    REG_ADDRESS_STACK_LO(--reg),
    port);

  return 0;
}

int SleepyBee::ioport_setPinsAsOutput_I(int port, int const_val)
{
  fprintf(out,
    "  ;; ioport_setPinsAsOutput_I(%d, value=0x%02x)\n"
    "  mov A, #0x%02x\n"
    "  orl P%dMDOUT, A\n",
    port, const_val,
    const_val,
    port);

  return 0;
}

int SleepyBee::ioport_setPinAsOutput_I(int port)
{
  fprintf(out,
    "  ;; ioport_setPinAsOutput_I(%d)\n"
    "  mov A, 0\n"
    "  setb C\n"
    "label_%d:\n"
    "  rlc\n"
    "  djnz %d, label_%d\n"
    "  orl P%dMDOUT, A\n",
    port,
    label_count,
    REG_ADDRESS_STACK_LO(--reg), label_count,
    port);

  label_count++;

  return 0;
}

int SleepyBee::ioport_setPinAsOutput_I(int port, int const_val)
{
  fprintf(out,
    "  ;; ioport_setPinAsOutput_I(%d)\n"
    "  setb 0x%02x.%d\n",
    port,
    0xa4 + port, const_val);

  return 0;
}

int SleepyBee::ioport_setPinAsInput_I(int port)
{
  fprintf(out,
    "  ;; ioport_setPinAsInput_I(%d)\n"
    "  mov A, 0\n"
    "  setb C\n"
    "label_%d:\n"
    "  rlc\n"
    "  djnz %d, label_%d\n"
    "  cpl A\n"
    "  anl P%dMDOUT, A\n",
    port,
    label_count,
    REG_ADDRESS_STACK_LO(--reg), label_count,
    port);

  label_count++;

  return 0;
}

int SleepyBee::ioport_setPinAsInput_I(int port, int const_val)
{
  fprintf(out,
    "  ;; ioport_setPinAsInput_I(%d)\n"
    "  clr 0x%02x.%d\n",
    port,
    0xa4 + port, const_val);

  return 0;
}

int SleepyBee::adc_enable()
{
  fprintf(out,
    " ;; adc_enable()\n"
    "  mov ADC0CN0, #0x80\n"
    "  mov ADC0CF, #0xf8\n"
    "  mov REF0CN, #0x08");

  return 0;
}

int SleepyBee::adc_disable()
{
  fprintf(out,
    " ;; adc_disable()\n"
    "  mov ADC0CN0, #0x00\n");

  return 0;
}

int SleepyBee::adc_setChannel_I()
{
#if 0
  fprintf(out,
    " ;; adc_setChannel_I()\n"
#endif

  return -1;
}

int SleepyBee::adc_setChannel_I(int channel)
{
  int port = channel / 8;
  int pin = channel % 8;
  int mask = 0xff ^ (1 << pin);

  fprintf(out,
    " ;; adc_setChannel_I(%d)\n"
    "  mov A, P%dMDIN\n"
    "  anl A, #0x%02x\n"
    "  mov P%dMDIN, A\n"
    "  setb P%d.%d\n"
    "  mov A, P%dSKIP\n"
    "  orl A, #0x%02x\n"
    "  mov P%dSKIP, A\n"
    "  mov ADC0MX, #ADC0P%d\n",
    channel,
    port,
    mask,
    port,
    port, pin,
    port,
    1 << pin,
    port,
    channel);

  return 0;
}

int SleepyBee::adc_read()
{
  fprintf(out,
    " ;; adc_read()\n"
    "  mov ADC0CN0, #0x90\n"
    "label_%d:\n"
    "  mov A, ADC0CN0\n"
    "  anl A, #0x20\n"
    "  jnz label_%d\n"
    "  mov %d, ADC0L\n"
    "  mov %d, ADC0H\n",
    label_count,
    label_count,
    REG_ADDRESS_STACK_LO(reg),
    REG_ADDRESS_STACK_HI(reg));

  reg++;
  label_count += 1;

  return 0;
}

int SleepyBee::add_new_array_byte()
{
  fprintf(out,
    "  ;; _new_array_byte()\n"
    "_new_array_byte:\n"
    "  mov DPL, r4\n"
    "  mov DPH, r5\n"
    "  mov A, r0\n"
    "  movx @DPTR, A\n"
    "  inc DPTR\n"
    "  mov A, r1\n"
    "  movx @DPTR, A\n"
    "  inc DPTR\n"
    "  mov r4, DPL\n"
    "  mov r5, DPH\n"
    "  mov r2, 4\n"
    "  mov r3, 5\n"
    "  mov A, r4\n"
    "  add A, r0\n"
    "  mov r4, A\n"
    "  mov A, r5\n"
    "  addc A, r1\n"
    "  mov r5, A\n"
    "  ret\n\n");

  return 0;
}

int SleepyBee::add_new_array_int()
{
  fprintf(out,
    "  ;; _new_array_int()\n"
    "_new_array_int:\n"
    "  mov DPL, r4\n"
    "  mov DPH, r5\n"
    "  mov A, r0\n"
    "  movx @DPTR, A\n"
    "  inc DPTR\n"
    "  mov A, r1\n"
    "  movx @DPTR, A\n"
    "  inc DPTR\n"
    "  mov r4, DPL\n"
    "  mov r5, DPH\n"
    "  mov r2, 4\n"
    "  mov r3, 5\n"
    "  clr C\n"
    "  mov A, r0\n"
    "  rlc A\n"
    "  mov r0, A\n"
    "  mov A, r1\n"
    "  rlc A\n"
    "  mov r1, A\n"
    "  mov A, r4\n"
    "  add A, r0\n"
    "  mov r4, A\n"
    "  mov A, r5\n"
    "  addc A, r1\n"
    "  mov r5, A\n"
    "  ret\n\n");

  return 0;
}

