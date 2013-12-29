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
void dspic_main(void *context, FILE *out);
void dspic_close(void *context, FILE *out);

#endif

