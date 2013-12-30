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

#ifndef _MSP430_H
#define _MSP430_H

struct msp430_t
{
  int place_holder;
};

int msp430_init(void **context, FILE *out);
void msp430_serial_init(void *context, FILE *out);
void msp430_method_start(void *context, FILE *out, int local_count, char *name);
void msp430_method_end(void *context, FILE *out, int local_count);
int msp430_push_integer(void *context, FILE *out, int32_t n);
int msp430_push_long(void *context, FILE *out, int64_t n);
int msp430_push_float(void *context, FILE *out, float f);
int msp430_push_double(void *context, FILE *out, double f);
int msp430_push_byte(void *context, FILE *out, char b);
void msp430_close(void *context, FILE *out);

#endif

