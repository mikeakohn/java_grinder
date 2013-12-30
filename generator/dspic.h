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

#ifndef _DSPIC_H
#define _DSPIC_H

struct dspic_t
{
  int reg;            // count number of registers are are using as stack
  int stack_count;    // count how many things we put on the stack
  //uint8_t register_stack[8];
};

int dspic_init(void **context, FILE *out);
void dspic_serial_init(void *context, FILE *out);
void dspic_method_start(void *context, FILE *out, int local_count, char *name);
void dspic_method_end(void *context, FILE *out, int local_count);
int dspic_push_integer(void *context, FILE *out, int32_t n);
int dspic_push_long(void *context, FILE *out, int64_t n);
int dspic_push_float(void *context, FILE *out, float f);
int dspic_push_double(void *context, FILE *out, double f);
int dspic_push_byte(void *context, FILE *out, char b);
void dspic_close(void *context, FILE *out);

#endif

