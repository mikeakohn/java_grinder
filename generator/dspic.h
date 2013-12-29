/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2013 by Michael Kohn
 *
 */

#ifndef _DSPIC_H
#define _DSPIC_H

struct dspic_t
{
  int place_holder;
};

int dspic_init(void **context);
void dspic_close(void *context);

#endif

