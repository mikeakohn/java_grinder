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
  int place_holder;
};

int dspic_init(void **context, FILE *out);
void dspic_serial_init(void *context, FILE *out);
void dspic_method(void *context, FILE *out, char *name);
int dspic_push_integer(void *context, FILE *out, int32_t n);
int dspic_push_long(void *context, FILE *out, int64_t n);
int dspic_push_float(void *context, FILE *out, float f);
int dspic_push_double(void *context, FILE *out, double f);
void dspic_close(void *context, FILE *out);

#endif

